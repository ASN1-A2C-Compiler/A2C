// ParserGenerator by Malcolm Crowe August 1995, 2000

using System;
using System.Collections;
using System.IO;
using System.Text;
using Tools;


public class ParserGenerator : SymbolsGen
{
	public bool m_showParser;
	public bool m_parserseen = false;
	public ParserGenerator() { m_showParser = false;  }
	public CsReader m_inFile;
	public string m_actions;
	string m_actvars = "";
	public bool m_namespace = false;
	public void Create(string infname,string outbase) 
	{
		Create(infname,outbase,"tokens");
	}
	public void Create(string infname,string outbase,string tokbase) 
	{
		ptokens tks = new ptokens();
		m_outname = outbase;
		tks.m_sgen = this;
		m_lexer = tks;
		m_symbols = new Symbols();
		m_symbols.ClassInit(this);
		try 
		{
			GetDefs(tokbase+".cs");
		} 
		catch (Exception ex) 
		{
			Error("Could not open tokens file "+tokbase+".cs ("+ex.Message+"). Parser cannot continue.");
			return;
		}
		try 
		{
			m_inFile = new CsReader(infname);
		}
		catch(Exception ex)
		{
			Error("Could not open input file "+infname+" ("+ex.Message+").");
			return;
		}
		m_outname = outbase;
		try 
		{
			m_outFile = new StreamWriter(outbase+".cs",false);
		} 
		catch (Exception ex) 
		{
			Error("Could not create output file "+outbase+".cs ("+ex.Message+").");
			return;
		}
		m_outFile.WriteLine("using System;using Tools;");
		Production special = new Production(this,m_symbols.Special);
		m_lexer.yytext = "error";
		CSymbol e = (new CSymbol(this)).Resolve();
		e.m_symtype = CSymbol.SymType.nonterminal;
		e.m_defined=true;
		m_actions = "[Serializable] public class yy"+m_outname+": Symbols {\n" +
			"  public override object Action(Parser yyq,SYMBOL yysym, int yyact) {\n" +
			"    switch(yyact) {\n" +
			"	 case -1: break; //// keep compiler happy";
		// 1: INPUT
		// top-down parsing of script
		m_lexer.Start(m_inFile);
		m_tok = (TOKEN)m_lexer.Next();
		//Console.WriteLine("Token <{0}> {1}",m_tok.yytext,m_tok.GetType().Name);
		while (m_tok!=null)
			ParseProduction();
		// that's the end of the script
		if (!m_parserseen)
			Error("no parser directive detected - possibly incorrect text encoding?");
		m_outFile.WriteLine(m_actions); // output the action function
		m_outFile.WriteLine("}  return null; }");
		special.AddToRhs(m_symbols.m_startSymbol);
		special.m_lhs.m_follow.AddIn(m_symbols.EOFSymbol);
		// 2: PROCESSING
		Console.WriteLine("First");
		DoFirst();
		Console.WriteLine("Follow");
		DoFollow();
		Console.WriteLine("Parse Table");
		ParseState start = new ParseState(this);
		m_symbols.m_states[0] = start;
		start.MaybeAdd(new ProdItem(special,0));
		start.Closure();
		start.AddEntries();
		ParserShift pe = (ParserShift)m_symbols.m_startSymbol.m_parsetable[0];
		m_symbols.m_accept = pe.m_next;
		if (m_symbols.m_accept==null) 
		{
			Console.WriteLine("No accept state. ParserGenerator cannot continue.");
			Environment.Exit(-1);
		}
		// 2A: Reduce States
		foreach (ParseState ps in m_symbols.m_states.Values)
			ps.ReduceStates();
		// 3: OUTPUT
		// output the run-time ParsingInfo table
		Console.WriteLine("Building parse table");
		foreach (CSymbol v in m_symbols.symbols.Values)
		{
			ParsingInfo pi = new ParsingInfo(m_symbols,v.yytext);
			CSymbol r = v;
			while (r.m_symtype==CSymbol.SymType.nodesymbol)
				r = r.m_refSymbol;
			pi.m_parsetable = r.m_parsetable;
		}
		foreach (Literal t in m_symbols.literals.Values)
		{
			ParsingInfo pi = new ParsingInfo(m_symbols,t.yytext);
			pi.m_parsetable = t.m_parsetable;
		}
		Console.WriteLine("Writing the output file");
		m_outFile.WriteLine("public yy"+outbase+"() { arr = new byte[] { ");
		m_symbols.Emit(m_outFile);
		// output the class factories
		CSymbol s;
		Console.WriteLine("Class factories");
		IDictionaryEnumerator de = m_symbols.symbols.GetEnumerator();
		for (int pos = 0; pos<m_symbols.symbols.Count; pos++) 
		{
			de.MoveNext();
			string str = (string)de.Key;
			s = (CSymbol)de.Value;
			if ((s==null) // might happen because of error recovery
				|| (s.m_symtype!=CSymbol.SymType.nonterminal && s.m_symtype!=CSymbol.SymType.nodesymbol))
				continue;
			//Console.WriteLine("{0} {1}",s.yytext,s.m_symtype);
			m_outFile.WriteLine("new Sfactory(this,\"{0}\",new SCreator({0}_factory));",str);
		}
		m_outFile.WriteLine("}");
		de.Reset();
		for (int pos = 0; pos<m_symbols.symbols.Count; pos++) 
		{
			de.MoveNext();
			string str = (string)de.Key;
			s = (CSymbol)de.Value;
			if ((s==null) // might happen because of error recovery
				|| (s.m_symtype!=CSymbol.SymType.nonterminal && s.m_symtype!=CSymbol.SymType.nodesymbol))
				continue;
			//Console.WriteLine("{0} {1}",s.yytext,s.m_symtype);
			m_outFile.WriteLine("public static object "+str+"_factory(Parser yyp) { return new "+str+"(yyp); }");
		}
		m_outFile.WriteLine("}");
		m_outFile.WriteLine("public class "+m_outname+": Parser {");
		m_outFile.WriteLine("public "+m_outname+"(Lexer yyl):base(new yy"+m_outname+"(),yyl) {}");
		m_outFile.WriteLine("public "+m_outname+"(Symbols syms,Lexer yyl):base(syms,yyl) {}");
		m_outFile.WriteLine(m_actvars);
		m_outFile.WriteLine(" }");
		if (m_namespace)
			m_outFile.WriteLine("}");
		m_outFile.Close();
		Console.WriteLine("Done");
		if (m_showParser) 
		{
			de = m_symbols.m_states.GetEnumerator();
			for (int i=0;i<m_symbols.m_states.Count; i++) 
			{
				de.MoveNext();
				ParseState ps = (ParseState)de.Value;
				ps.Print();
				if (ps==m_symbols.m_accept)
					Console.WriteLine("    EOF  accept");
			}
		}
	}
	public void GetDefs(string fname) 
	{
		StreamReader sr = new StreamReader(fname);
		for (string buf=sr.ReadLine(); buf!=null; buf=sr.ReadLine()) 
			if (buf.Length>3 && buf.Substring(0,3).Equals("//%")) 
			{
				if (buf.Substring(3,1).Equals("+")) 
				{
					m_lexer.yytext = buf.Substring(4);
					new SymbolType(this,m_lexer.yytext,true);
				} 
				else 
				{
					m_lexer.yytext = buf.Substring(3);
					new SymbolType(this,m_lexer.yytext,false);
				}
			}
		sr.Close();
	}

	public TOKEN m_tok; // current input token as returned by Parser.Next
	public void Advance() 
	{ 
		m_tok = (TOKEN)m_lexer.Next();
	}
	public Production m_prod; // current production being parsed

	internal void DoFirst() 
	{
		// Rule 1: terminals only
		Production p;
		foreach (CSymbol v in m_symbols.symbols.Values)
		{
			if (v.m_symtype==CSymbol.SymType.unknown)
				v.m_symtype = CSymbol.SymType.terminal;
			if (v.IsTerminal()) 
			{
				v.m_first.CheckIn(v);
				if (!Find(v))
					Console.WriteLine("Warning: lexer script should define symbol {0}", v.yytext);
			}
		}
		foreach (CSymbol s in m_symbols.literals.Values)
			s.m_first.CheckIn(s);
		m_symbols.EOFSymbol.m_first.CheckIn(m_symbols.EOFSymbol);
		// Rule 2: Nonterminals with the rhs consisting only of actions
		int j,k;
		for (k=1;k<prods.Count;k++) 
		{
			p = (Production)prods[k];
			if (p.m_actionsOnly)
				p.m_lhs.m_first.CheckIn(m_symbols.EmptySequence);
		}

		// Rule 3: The real work begins
		bool donesome = true;
		while (donesome) 
		{
			donesome = false;
			for (k=1;k<prods.Count;k++) 
			{
				p = (Production)prods[k];
				int n = p.m_rhs.Count;
				for (j=0;j<n;j++) 
				{
					CSymbol t = (CSymbol)p.m_rhs[j];
					if (t.IsAction())
						donesome |= t.m_first.CheckIn(m_symbols.EmptySequence);
					int pos = 0;
					foreach (CSymbol a in t.m_first.Keys)
						if ((a!=m_symbols.EmptySequence || pos++==t.m_first.Count-1))
							donesome |= p.m_lhs.m_first.CheckIn(a);
					if (!t.m_first.Contains(m_symbols.EmptySequence))
						break;
				}
			}
		}
	}

	internal void DoFollow() 
	{
		// Rule 1: 
		m_symbols.m_startSymbol.m_follow.CheckIn(m_symbols.EOFSymbol);
		// Rule 2 & 3:
		bool donesome = true;
		while (donesome) 
		{
			donesome = false;
			for (int k=1; k<prods.Count; k++) 
			{
				Production p = (Production)prods[k];
				int n = p.m_rhs.Count;
				for (int j=0; j<n; j++) 
				{
					CSymbol b = (CSymbol)p.m_rhs[j];
					// Rule 2
					p.AddFirst(b,j+1);
					// Rule 3
					if (p.CouldBeEmpty(j+1)) 
						donesome |= b.AddFollow(p.m_lhs.m_follow);
				}
			}
		}
	}

	// routines called by Lexer actions
	public override void CopySegment() 
	{
		string str="";
		int ch;
		for (;(ch=m_lexer.GetChar())!=0;) 
		{
			str += (char)ch;
			if (ch=='\n') 
			{
				if (str.Equals("%}\n"))
					return;
				m_outFile.WriteLine(str);
				str ="";
			}
		}
		m_outFile.Write(str);
	}
	public override void ParserDirective() 
	{
		m_parserseen = true;
		int ch;
		do
			ch = m_lexer.GetChar();
		while (ch!='\n' && ch!=0);
	}
	public override void Declare()
	{
		int rv = 1;
		int quote = 0;
		int ch;
		for(;;)
		{
			ch = m_lexer.GetChar();
			if (ch==0) 
			{
				Error("EOF in Declare?");
				return;
			}
			if (ch=='\\') 
			{
				m_actvars += ch;
				ch = (char)m_lexer.GetChar();
			}
			else if (quote==0 && ch=='{')
				 rv++;
			else if (quote==0 && ch=='}') 
			{
				if (--rv==0)
					break;
			} 
			else if (ch==quote)
				quote = 0;
			else if (ch=='\''||ch=='"')
				quote = ch;
			m_actvars += (char)ch;
		}
	}
	public override void ClassDefinition(string bas) 
	{
		string line, name;
		int n = m_lexer.yytext.Length;
		line = m_lexer.yytext;
		EmitClassDefin(m_lexer.m_buf,ref m_lexer.m_pch,m_lexer.m_buf.Length,null,bas,out bas,out name,false);
		m_lexer.Advance();
		m_lexer.yytext = name;
		CSymbol s = (new CSymbol(this)).Resolve();
		s.m_defined = true;
		s.m_emitted = true;
		if (line[1]=='n') 
		{
			s.m_symtype = CSymbol.SymType.nodesymbol;
			if (m_symbols.symbols.Contains(bas))
				s.m_refSymbol = (CSymbol)m_symbols.symbols[bas];
			else
				s.m_refSymbol = null;
		}
	}

	internal int prec = 0;

	public override void AssocType(Precedence.PrecType pt, int p) 
	{
		string line;
		int len,action=0;
		CSymbol s;
		line = m_lexer.yytext;
		prec += 10;
		if (line[p]!=' '&&line[p]!='\t')
			Error("Expected white space after precedence directive");
		for (p++;p<line.Length && (line[p]==' '||line[p]=='\t');p++)
			;
		while (p<line.Length) 
		{
			len = m_lexer.m_start.Match(line,p,ref action);
			if (len<0) 
			{
				Console.WriteLine(line.Substring(p));
				Error("Expected token");
				break;
			}
			m_lexer.yytext = line.Substring(p,len);
			if (action!=181&&action!=255&&action!=201&&action!=275)  // yuk: see ptokens.cs - actions for Literal are 181,201,255,275
				s = (new CSymbol(this)).Resolve();
			else
				s = (new Literal(this)).Resolve();
			s.m_prec = new Precedence(pt,prec,s.m_prec);
			for (p+=len; p<line.Length && (line[p]==' '||line[p]=='\t'); p++)
				;
		}
	}

	public override void SetNamespace() 
	{
		m_tok = m_lexer.Next();
		((CSymbol)m_tok).m_symtype = CSymbol.SymType.nspace;
		m_namespace = true;
		m_outFile.WriteLine("namespace "+m_tok.yytext+" {");
	}

	public override void SetStartSymbol() 
	{
		m_tok = m_lexer.Next(); // recursive call of lexer.Next should be okay
		m_symbols.m_startSymbol = ((CSymbol)m_tok).Resolve();
		m_symbols.m_startSymbol.m_symtype = CSymbol.SymType.nonterminal;
	}

	// proxies for constructors
	internal void NewConstructor(ref CSymbol s,string str) 
	{ // may update s to a new node type
		// we have just seen a new initialiser for s
		if (str.Length==0)
			return;
		CSymbol bas = s;
		string newname;
		for (int variant=1;;variant++) 
		{ // get a genuinely new identifier
			newname = String.Format("{0}_{1}", bas.yytext, variant);
			s = (CSymbol)m_symbols.symbols[newname];
			if (s==null)
				break;
		}
		m_lexer.yytext = newname;
		s = (new CSymbol(this)).Resolve();
		s.m_symtype = CSymbol.SymType.nodesymbol;
		s.m_refSymbol = bas;
		s.m_emitted = true;
		m_outFile.WriteLine();
		m_outFile.WriteLine("[Serializable] public class "+newname+" : "+bas.yytext+" {");
		m_outFile.WriteLine("  public "+newname+"(Parser yyq):base(yyq"+str+"}");
	}

	void NextNonWhite(out int ch) 
	{
		while ((ch=m_lexer.PeekChar())==' '||ch=='\t'||ch=='\n')
			m_lexer.GetChar();
	}

	public override void SimpleAction(ParserSimpleAction a) 
	{
		string str = a.yytext.Substring(1, a.yytext.Length-1);
		if (str.Equals("null")) 
		{
			m_lexer.yytext = "Null";
			a.m_sym = (new CSymbol(this)).Resolve();
			NewConstructor(ref a.m_sym,",\""+m_prod.m_lhs.yytext+"\"){}");
			return;
		}
		if (str.Length==0)
			str = m_prod.m_lhs.yytext;
		CSymbol s = (CSymbol)m_symbols.symbols[str];
		if (s==null) 
		{ // not there: define a new node type
			//Console.WriteLine("Symbol {0} needs defining",str);
			m_lexer.yytext = str;
			s = new CSymbol(this);
			m_symbols.symbols[str] = s;
			s.m_symtype = CSymbol.SymType.nodesymbol;
			s.m_refSymbol = (CSymbol)m_prod.m_lhs;
			s.m_emitted = true;
			m_outFile.WriteLine("[Serializable] public class "+str+" : "+s.m_refSymbol.yytext+" { ");
			m_outFile.WriteLine("  public "+str+"(Parser yyq):base(yyq) { }}");
		}
		a.m_sym = s;
		int ch;
		str = ")";
		NextNonWhite(out ch);
		if (ch=='(') 
		{
			ch = m_lexer.GetChar();
			str = ","+GetBracketedSeq(ref ch,')').Substring(1);
			NextNonWhite(out ch);
		}
		string init = "{}";
		if (ch=='{') 
		{
			ch = m_lexer.GetChar();
			init = "yyp=("+m_outname+")yyq;\n"+ GetBracketedSeq(ref ch,'}');
			NextNonWhite(out ch);
		}
		init = str + init;
		NewConstructor(ref a.m_sym,init);
	}

	string FixDollars(ref int ch) 
	{  
		bool minus = false;
		int ix;
		int ln = m_prod.m_rhs.Count+1;
		string type="int";
		string str = "";
		ch = m_lexer.GetChar();
		if (ch=='<') 
		{
			type = ((TOKEN)m_lexer.Next()).yytext;
			ch = m_lexer.GetChar();
			if (ch!='>')
				Error("Expected >");
			ch = m_lexer.GetChar();
		}
		if (ch=='$') 
		{
			//		ch = m_lexer.GetChar();
			str += "yylval";
			if (m_prod.m_lhs.m_defined)
				Error(String.Format("Production {0}: symbol {1} has been typed, and should not use $$",m_prod.m_pno, m_prod.m_lhs.yytext));
			return str;
		}
		if (ch=='-') 
		{
			minus = true;
			//		ch = m_lexer.GetChar();
		}
		ix =1;
		if (ch<'0' || ch>'9')
			Error(String.Format("expected $ or digit, got {0}",(char)ch));
		else
			ix = minus?('0'-ch):(ch-'0');
		for (ch = m_lexer.PeekChar(); ch>='0' && ch<='9'; m_lexer.Advance())
			ix = ix*10+ (minus?('0'-ch):(ch-'0'));
		if (ix<=0) 
		{
			str += String.Format("\n\t({0})(yyps.StackAt({0}).m_value)\n\t",type,ln-ix);
			if ((char)ch!='.')
				str += ".m_dollar";
		} 
		else 
			m_prod.FixDollarInt(ref str, ch, ix);
		return str;
	}

	string GetBracketedSeq(ref int ch,char cend) 
	{ // ch is the start bracket, already returned
		string str = "";
		int quoteseen = 0;
		int brackets = 1;
		char cstart = (char)ch;
		str += cstart; 
		//Console.WriteLine("GetBracketedSeq with {0}",str);
		while ((ch=m_lexer.GetChar())!=0) 
		{
			if (ch==cstart) 
			{
				if (quoteseen==0)
					brackets++;
			} 
			else if (ch==cend) 
			{
				if (quoteseen==0 && --brackets==0)
					goto labout;
			} 
			else if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||ch=='_') 
			{
				m_lexer.UnGetChar();
				TOKEN a = m_lexer.Next();
				if (m_prod.m_alias.Contains(a.yytext)) 
				{
					int ix = (int)m_prod.m_alias[a.yytext];
					ch = m_lexer.PeekChar();
					m_prod.FixDollarInt(ref str,ch,ix);
				} 
				else
					str += a.yytext;
				continue;
			} 
			else
				switch(ch) 
				{
					case '$':
						str += FixDollars(ref ch);
						continue;  // don't add the ch
					case '\\':
						str += (char)ch;
						ch = m_lexer.GetChar();
						break;
					case '\n':
						break;
					case '"': case '\'':
						if (quoteseen==0)    // start of quoted string
							quoteseen = ch;
						else if (quoteseen==ch) // matching end of string
							quoteseen = 0;
						break;
				}
			str += (char)ch;
		}
		Error("EOF in Action: check bracket counts, quotes");
		labout:
			str += cend;
		str = str.Replace("yyp","(("+m_outname+")yyq)");
		return str;
	}

	public override void OldAction(ParserOldAction a) 
	{
		int ch = '{';
		a.m_symtype = CSymbol.SymType.oldaction;
		a.m_action = ++action;
		a.m_initialisation = GetBracketedSeq(ref ch,'}');
		a.m_sym = (CSymbol)m_prod.m_lhs;
		
		NextNonWhite(out ch);
		if (ch==';'||ch=='|')
		{ // an old action at the end is converted into a simple action
			m_lexer.yytext = "%";
			ParserSimpleAction sa = new ParserSimpleAction(this);
			SimpleAction(sa);
			NewConstructor(ref sa.m_sym, ")"+a.m_initialisation);
			a.m_sym = (CSymbol)sa;
			sa.yytext += sa.m_sym.yytext;
			a.yytext = "#"+sa.yytext;
			a.m_action = -1; // mark the old action for deletion
		}
	}

	// parsing routines
	internal void ParseProduction() 
	{
		CSymbol lhs = null;
		try 
		{
			lhs = ((CSymbol)m_tok).Resolve();
		} 
		catch(Exception e) 
		{
			Console.WriteLine("Syntax error in Parser script - possibly extra semicolon?",e.Message);
			Environment.Exit(-1);
		}
		m_tok = lhs;
		if (m_tok.IsTerminal())
			Error(String.Format("Illegal left hand side <{0}> for production",m_tok.yytext));
		if (m_symbols.m_startSymbol==null)
			m_symbols.m_startSymbol = lhs;
		if (lhs.m_symtype==CSymbol.SymType.unknown)
			lhs.m_symtype = CSymbol.SymType.nonterminal;
		if ((!lhs.m_defined) && lhs.m_prods.Count==0) 
		{ // lhs not defined in %symbol statement and not previously a lhs
			// so declare it as a new symbol
			m_outFile.WriteLine("[Serializable] public class "+lhs.yytext+" : SYMBOL {");
			m_outFile.WriteLine("	public "+lhs.yytext+"(Parser yyq):base(yyq) { }");
			m_outFile.WriteLine("  public override string yyname() { return \""+lhs.yytext+"\"; }}");
		}
		if (!Find(lhs))
			new SymbolType(this,lhs.yytext);
		m_prod = new Production(this,lhs);
		m_lexer.yy_begin("rhs");
		Advance();
		if (!m_tok.Matches(":"))
			Error(String.Format("Colon expected for production {0}",lhs.yytext));
		Advance();
		RhSide(m_prod);
		while(m_tok!=null && m_tok.Matches("|")) 
		{
			Advance();
			m_prod = new Production(this,lhs);
			RhSide(m_prod);
		}
		if (m_tok==null || !m_tok.Matches(";"))
			Error("Semicolon expected");	
		Advance();
		m_prod = null;
		m_lexer.yy_begin("YYINITIAL");
	}

	public void RhSide(Production p) 
	{
		CSymbol s;
		ParserOldAction a = null; // last old action seen
		while (m_tok!=null) 
		{
			if (m_tok.Matches(";"))
				break;
			if (m_tok.Matches("|"))
				break;
			if (m_tok.Matches(":")) 
			{
				Advance();
				p.m_alias[m_tok.yytext] = p.m_rhs.Count;
				Advance();
			} 
			else 
			{
				s = (CSymbol)m_tok;
				if (s.m_symtype==CSymbol.SymType.oldaction) 
				{
					if (a!=null)
						Error("adjacent actions");
					a = (ParserOldAction)s;
					if (a.m_action<0) 
					{ // an OldAction that has been converted to a SimpleAction: discard it
						s = a.m_sym; // add the SimpleAction instead
						s.m_symtype = CSymbol.SymType.simpleaction;
						ParserSimpleAction sa = (ParserSimpleAction)s;
					} 
					else  // add it to the Actions function
						m_actions += String.Format("\ncase {0} : {1} break;", a.m_action, a.m_initialisation);
					a = null;
				} 
				else if (s.m_symtype!=CSymbol.SymType.simpleaction)
					s = ((CSymbol)m_tok).Resolve();
				p.AddToRhs(s);
				Advance();
			}
		}
		Precedence.Check(p);
	}

	public static void Main(string[] argv) 
	{
		int argc = argv.Length;
		ParserGenerator parser = new ParserGenerator();
		string tokfname = "tokens";
		int j;
		for (j = 0; argc>0 && argv[j][0]=='-'; j++,argc--)
			switch(argv[j][1]) 
			{
				case 'D': parser.m_showParser = true; break;
				case 'I': tokfname = argv[j].Substring(2); break;
			}
		if (argc==1)
			parser.Create(argv[j],"syntax",tokfname);
		else if (argc==2)
			parser.Create(argv[j],argv[j+1],tokfname);
		else
			parser.Create("test.parser","syntax",tokfname);
		if (parser.m_symbols.errors==0)
			Console.WriteLine("ParserGenerator completed successfully");
		else
			Console.WriteLine("Parser error count="+parser.m_symbols.errors);
	}
}

