// Malcolm Crowe 1995,2000
// As far as possible the regular expression notation follows that of lex

using System;
using System.Collections;
using System.IO;
using System.Text;
using System.Globalization;
using System.Runtime.Serialization.Formatters.Binary;

namespace Tools 
{

	// We cleverly arrange for Tokens to serialize itself out of a simple integer array.
	// So: to use the lexer generated for a script, include the generated tokens.cs file in the build,
	// This defines a class Tokens which exports public static Lexer GetLexer()

	// Call Start() to start the input engine going, and then use the
	// Next() function to get successive (TOKEN *)s.
	// Note that if you are using ParserGenerator, this is done for you. 

	public class Tokens // we will gather all formerly static definitions for lexing here and in LexerGenerate
	{
		// Deserializing 
		public void GetDfa() 
		{
			if (tokens.Count>0)
				return;
			MemoryStream ms = new MemoryStream(arr);
			BinaryFormatter f = new BinaryFormatter();
			m_encoding = (Encoding)f.Deserialize(ms);
			cats = (Hashtable)f.Deserialize(ms);
			m_gencat = (UnicodeCategory)f.Deserialize(ms);
			usingEOF = (bool)f.Deserialize(ms);
			starts = (Hashtable)f.Deserialize(ms);
			Dfa.SetTokens(this,starts);
			tokens = (Hashtable)f.Deserialize(ms);
		}
		public void EmitDfa(StreamWriter outFile)
		{
			Console.WriteLine("Serializing the lexer"); Console.Out.Flush();
			MemoryStream ms = new MemoryStream();
			BinaryFormatter f = new BinaryFormatter();
			f.Serialize(ms,m_encoding);
			f.Serialize(ms,cats);  
			f.Serialize(ms,m_gencat);
			f.Serialize(ms,usingEOF);
			f.Serialize(ms,starts);  
			f.Serialize(ms,tokens); 
			ms.Position=0;
			int k=0;
			for (int j=0;j<ms.Length;j++) 
			{
				int b = ms.ReadByte();
				if (k++ ==10) 
				{
					outFile.WriteLine();
					k = 0;
				}
				outFile.Write("{0},",b);
			}
			outFile.WriteLine("0};");
		}

		// support for Unicode character sets
		public Encoding m_encoding = new ASCIIEncoding();
		public bool usingEOF = false;
		public Hashtable cats = new Hashtable(); // UnicodeCategory -> Charset
		public UnicodeCategory m_gencat; // not a UsingCat unless all usbale cats in use
		// support for lexer states
		public Hashtable starts = new Hashtable(); // string->Dfa
		// support for serialization
		protected byte[] arr; // defined in generated tokens class
		// support for token classes
		public Hashtable types = new Hashtable(); // string->TCreator
		public Hashtable tokens = new Hashtable(); // string->TokClassDef
		public Tokens() 
		{
			UsingCat(UnicodeCategory.OtherPunctuation);
			m_gencat = UnicodeCategory.OtherPunctuation;
			new Tfactory(this,"TOKEN",new TCreator(Tokenfactory));
		}
		protected object Tokenfactory(Lexer yyl) 
		{
			return new TOKEN(yyl);
		}
		public Charset UsingCat(UnicodeCategory cat) 
		{
			if (cat==m_gencat) 
			{
				UnicodeCategory[] x = (UnicodeCategory[])Enum.GetValues(typeof (UnicodeCategory));
				for (int j=0;j<x.Length;j++) 
				{
					UnicodeCategory u = x[j];
					if (u==UnicodeCategory.Surrogate)
						continue;
					if (cats[u]==null) 
					{
						UsingCat(u);
						m_gencat = u;						
					}
				}
				return (Charset)cats[cat];
			} 
			if (cats[cat]!=null)
				return (Charset)cats[cat];
			Charset rv = new Charset(cat);
			cats[cat] = rv;
			return rv;
		}
		internal void UsingChar(char ch) 
		{
			UnicodeCategory cat = Char.GetUnicodeCategory(ch);
			Charset cs = UsingCat(cat);
			if (cs.m_generic==ch) 
			{
				do 
				{
					if (cs.m_generic==char.MaxValue) 
					{
						cs.m_generic = ch; // all used: this m_generic will never be used
						return;
					}
					cs.m_generic++;
				} while (Char.GetUnicodeCategory(cs.m_generic)!=cs.m_cat ||
					cs.m_chars.Contains(cs.m_generic)); 
				cs.m_chars[cs.m_generic] = true;
			} 
			else
				cs.m_chars[ch] = true;
		}
		internal char Filter(char ch) 
		{
			UnicodeCategory cat = Char.GetUnicodeCategory(ch);
			Charset cs = (Charset)cats[cat];
			if (cs==null)
				cs = (Charset)cats[m_gencat];
			if (cs.m_chars.Contains(ch))
				return ch;
			return cs.m_generic;
		}
		bool testEOF(char ch) 
		{
			UnicodeCategory cat = Char.GetUnicodeCategory(ch);
			return (cat==UnicodeCategory.OtherNotAssigned);
		}
		internal ChTest GetTest(string name) 
		{
			switch (name) 
			{
				case "Symbol": 
					UsingCat(UnicodeCategory.OtherSymbol);
					UsingCat(UnicodeCategory.ModifierSymbol);
					UsingCat(UnicodeCategory.CurrencySymbol);
					UsingCat(UnicodeCategory.MathSymbol);
					return new ChTest(Char.IsSymbol); 
				case "Punctuation": 
					UsingCat(UnicodeCategory.OtherPunctuation);
					UsingCat(UnicodeCategory.FinalQuotePunctuation);
					UsingCat(UnicodeCategory.InitialQuotePunctuation);
					UsingCat(UnicodeCategory.ClosePunctuation);
					UsingCat(UnicodeCategory.OpenPunctuation);
					UsingCat(UnicodeCategory.DashPunctuation);
					UsingCat(UnicodeCategory.ConnectorPunctuation);
					return new ChTest(Char.IsPunctuation); 
					/*			case "PrivateUse": 
									UsingCat(UnicodeCategory.PrivateUse);
									return new ChTest(Char.IsPrivateUse); */
				case "Separator": 
					UsingCat(UnicodeCategory.ParagraphSeparator);
					UsingCat(UnicodeCategory.LineSeparator);
					UsingCat(UnicodeCategory.SpaceSeparator);
					return new ChTest(Char.IsSeparator); 
				case "WhiteSpace": 
					UsingCat(UnicodeCategory.Control);
					UsingCat(UnicodeCategory.ParagraphSeparator);
					UsingCat(UnicodeCategory.LineSeparator);
					UsingCat(UnicodeCategory.SpaceSeparator);
					return new ChTest(Char.IsWhiteSpace); 
				case "Number": 
					UsingCat(UnicodeCategory.OtherNumber);
					UsingCat(UnicodeCategory.LetterNumber);
					UsingCat(UnicodeCategory.DecimalDigitNumber);
					return new ChTest(Char.IsNumber); 
				case "Digit": 
					UsingCat(UnicodeCategory.DecimalDigitNumber);
					return new ChTest(Char.IsDigit); 
					/*			case "Mark": 
									UsingCat(UnicodeCategory.EnclosingMark);
									UsingCat(UnicodeCategory.SpacingCombiningMark);
									UsingCat(UnicodeCategory.NonSpacingMark);
									return new ChTest(Char.IsMark); */
				case "Letter": 
					UsingCat(UnicodeCategory.OtherLetter);
					UsingCat(UnicodeCategory.ModifierLetter);
					UsingCat(UnicodeCategory.TitlecaseLetter);
					UsingCat(UnicodeCategory.LowercaseLetter);
					UsingCat(UnicodeCategory.UppercaseLetter);
					return new ChTest(Char.IsLetter); 
				case "Lower": 
					UsingCat(UnicodeCategory.LowercaseLetter);
					return new ChTest(Char.IsLower); 
				case "Upper": 
					UsingCat(UnicodeCategory.UppercaseLetter);
					return new ChTest(Char.IsUpper); 
				case "EOF":
					UsingCat(UnicodeCategory.OtherNotAssigned);
					UsingChar((char)0xFFFF);
					usingEOF=true;
					return new ChTest(testEOF);
				default:
					Error("No such Charset "+name);
					break;
			}
			return new ChTest(Char.IsControl); // not reached
		}
		public virtual TOKEN OldAction(Lexer yyl,string yytext,int action,ref bool reject) 
		{
			return null;
		}
		// error handling (definition phase)
		public int m_errors = 0;
		public void Error(string s) 
		{
			System.Console.WriteLine(s);
			m_errors++;
		}
	}


	public class LineManager
	{
		public int lines = 1;  // for error messages etc
		public LineList list = null;
		public LineManager() {}
		public void newline(int pos) 
		{ 
			lines++;
			backto(pos);
			list = new LineList(pos,list); 
		}
		public void backto(int pos) 
		{
			while (list!=null && list.head>=pos) 
			{
				list = list.tail;
				lines--;
			}
		}
		public void comment(int pos,int len) 
		{ // only for C-style comments not C++
			if (list==null) 
			{
				list = new LineList(0,list);
				lines = 1;
			}
			list.comments = new CommentList(pos,len,list.comments);
		}
		public string saypos(int pos) 
		{
			int n = lines;
			for (LineList p = list; p!=null; p = p.tail, n-- )
				if (p.head<pos)
					return string.Format("line {0}, char {1}: ", n, p.getpos(pos));
			return string.Format("line 1, char {0}: ",pos);
		}
	}

	public abstract class TokensGen : GenBase
	{
		protected bool m_showDfa;
		public Tokens m_tokens; // the Tokens class under construction
		// %defines in script
		public Hashtable defines = new Hashtable(); // string->string
		// support for Nfa networks
		int state = 0;
		public int NewState() { return ++state; } // for LNodes
		public ObjectList states = new ObjectList(); // Dfa
		public string FixActions(string str)
		{
			return str.Replace("yybegin","yym.yy_begin").Replace("yyl","(("+m_outname+")yym)");
		}
	}

	// support for Unicode character sets

	internal delegate bool ChTest(char ch);

	[Serializable] public class Charset 
	{
		internal UnicodeCategory m_cat;
		internal char m_generic; // not explicitly Using'ed allUsed
		internal Hashtable m_chars = new Hashtable(); // char->bool
		internal Charset(UnicodeCategory cat) 
		{ 
			m_cat = cat;
			for (m_generic=char.MinValue;Char.GetUnicodeCategory(m_generic)!=cat;m_generic++)
				;
			m_chars[m_generic] = true;
		}
	}

	// Support for runtime object creation

	public delegate object TCreator(Lexer yyl);

	public class Tfactory
	{
		public static object create(string cls_name,Lexer yyl) 
		{
			TCreator cr = (TCreator) yyl.m_tokens.types[cls_name];
			// Console.WriteLine("TCreating {0} <{1}>",cls_name,yyl.yytext);
			if (cr==null) 
				throw(new Exception(String.Format("no factory for {0}",cls_name)));
			try 
			{
				return cr(yyl);
			} 
			catch (Exception e) 
			{ 
				throw(new Exception(String.Format("Line {0}: Create of {1} failed ({2})",yyl.Saypos(yyl.m_pch),cls_name,e.Message)));
			}
			return null;
		}
		public Tfactory(Tokens tks,string cls_name,TCreator cr) 
		{
			tks.types[cls_name] = cr;
		}
	}

	public class LineList 
	{  // based on Appel's ErrorMsg class
		public int head;
		public CommentList comments = null;
		public LineList tail;
		public LineList(int h, LineList t) 
		{ 
			head=h; 
			comments = null;
			tail=t; 
		}
		public int getpos(int pos) 
		{
			int n = pos-head;
			for (CommentList c = comments; c!=null; c=c.tail)
				if (pos>c.spos)
					n += c.len;
			return n;
		}
	}
	public class CommentList 
	{ // similar for comments on a line
		public int spos,len;
		public CommentList tail = null;
		public CommentList(int st,int ln, CommentList t) 
		{
			spos = st; len = ln;
			tail = t;
		}
	}


	// the following class gets rid of comments for us

	public class CsReader
	{
		StreamReader m_stream; 
		public LineManager lm;
		int back; // one-char pushback
		enum State 
		{
			copy, sol, c_com, cpp_com, c_star, at_eof, transparent 
		}
		State state;
		int pos = 0;
		public CsReader(string fileName) 
		{
			lm = new LineManager();
			FileStream fs = new FileStream(fileName,FileMode.Open);
			m_stream = new StreamReader(fs); 
			state= State.copy; back = -1;}
		public CsReader(CsReader inf,Encoding enc) 
		{
			lm = new LineManager();
			m_stream = new StreamReader(inf.m_stream.BaseStream,enc);
			state= State.copy; back = -1;}
		public bool Eof() { return state==State.at_eof; }
		public int Read(char[] arr,int offset,int count) 
		{
			int c,n;
			for (n=0;count>0;count--,n++) 
			{
				c = Read();
				if (c<0)
					break;
				arr[offset+n] = (char)c;
			}
			return n;
		}
		public string ReadLine() 
		{
			int c=0,n;
			char[] buf = new char[1024];
			int count = 1024;
			for (n=0;count>0;count--) 
			{
				c = Read();
				if (((char)c)=='\r')
					continue;
				if (c<0 || ((char)c)=='\n')
					break;
				buf[n++] = (char)c;
			}
			if (c<0)
				state = State.at_eof;
			return new string(buf,0,n);
		}
		public int Read() 
		{
			int c,comlen = 0;
			if (state==State.at_eof)
				return -1;
			while (true) 
			{
				// get a character
				if (back>=0) 
				{ // back is used only in copy mode
					c = back; back = -1;	
				} 
				else if (state==State.at_eof)
					c = -1;
				else
					c = m_stream.Read();
				if (c=='\r')
					continue;
				if (c<0) 
				{  // at EOF we must leave the loop
					if (state==State.sol)
						c = '/';
					state = State.at_eof;
					pos++;
					return c;
				}
				// otherwise work through a state machine
				switch (state) 
				{
					case State.copy:
						if (c=='/')
							state = State.sol;
						else 
						{
							pos++; 
							return c;
						} continue;
					case State.sol: // solidus '/'
						if (c=='*')
							state = State.c_com;
						else if (c=='/') 
						{
							comlen = 2;
							state = State.cpp_com;
						} 
						else 
						{
							back = c;
							state = State.copy;
							pos++; 
							return '/';
						}
						continue;
					case State.c_com:
						comlen++;
						if (c=='\n') 
						{
							lm.newline(pos);
							comlen=0;
						}
						if (c=='*')
							state = State.c_star;
						continue;
					case State.c_star:
						comlen++;
						if (c=='/') 
						{
							lm.comment(pos,comlen);
							state = State.copy;
						} 
						else 
							state = State.c_com;
						continue;
					case State.cpp_com:
						if (c=='\n') 
						{
							state = State.copy;
							pos++;
							return c;
						} 
						else
							comlen++;
						continue;
				}
			}
			/* notreached */
		}
	}

	[Serializable] public class SYMBOL
	{
		public object m_dollar;
		public int pos;
		[NonSerialized]
		public Lexer yylx;
		public object yylval { get { return m_dollar; } set { m_dollar=value; } }
		public SYMBOL(Lexer yyl) { yylx=yyl; }
		public virtual bool IsTerminal() { return false; }
		public virtual bool IsAction() { return false; }
		public virtual bool IsCSymbol() { return false; }
		[NonSerialized]
		public Parser yyps = null;
		public Symbols yyact { get { return (yyps!=null)?yyps.m_symbols:null; }}
		public SYMBOL(Parser yyp) { yyps=yyp; yylx=yyp.m_lexer; }
		public virtual bool Pass(Symbols syms,int snum,out ParserEntry entry) 
		{
			ParsingInfo pi = (ParsingInfo)syms.parsingInfo[yyname()];
			if (pi==null) 
			{
				syms.Error(string.Format("No parsinginfo for symbol {0}",yyname()));
				Environment.Exit(-1);
			}
			bool r = pi.m_parsetable.Contains(snum);
			entry = r?((ParserEntry)pi.m_parsetable[snum]):null;
			return r;
		}
		public virtual string yyname() { return "SYMBOL"; }
		public virtual bool Matches(string s) { return false; }
		public virtual void Print() { Console.WriteLine("SYMBOL"); }
	}

	[Serializable] public class TOKEN : SYMBOL
	{
		public string yytext { get { return m_str; } set { m_str=value; } }
		string m_str;
		public TOKEN(Parser yyp):base(yyp) {}
		public TOKEN(Lexer yyl) : base(yyl) { if (yyl!=null) m_str=yyl.yytext; }
		public TOKEN(Lexer yyl,string s) :base(yyl) { m_str=s; }
		public override bool Pass(Symbols syms,int snum, out ParserEntry entry) 
		{
			if (!yyname().Equals("TOKEN")) // derived classes' parsetable do not depend on yytext
				return base.Pass(syms,snum,out entry);
			//Console.WriteLine("Retrieving Parsetable for literal {0}",m_str);
			ParsingInfo pi = (ParsingInfo)syms.parsingInfo[m_str];
			if (pi==null) 
				throw(new Exception(String.Format("Parser does not recognise literal <{0}>",m_str)));
			bool r = pi.m_parsetable.Contains(snum);
			entry = r?((ParserEntry)pi.m_parsetable[snum]):null;
			return r;
		}
		public override bool IsTerminal() { return true; }
		public override string yyname() { return "TOKEN"; }
		public override bool Matches(string s) { return s.Equals(m_str); }
		public override void Print() { Console.WriteLine(yytext); }
	}

	public class Lexer
	{
		public bool m_debug = false;
		LineManager m_LineManager = new LineManager();
		public string Saypos(int pos) { return m_LineManager.saypos(pos); }

		// the heart of the lexer is the DFA
		public Dfa m_start { get { return (Dfa)m_tokens.starts[m_state]; }}
		protected string m_state = "YYINITIAL";

		public Lexer(Tokens tks) 
		{
			m_state="YYINITIAL";  
			m_tokens = tks;
		}

		public Tokens m_tokens;

		public string yytext; // for collection when a TOKEN is created
		public int m_pch = 0;
		public int yypos { get { return m_pch; }}

		public void yy_begin(string newstate) 
		{
			m_state = newstate;
		}
		public string m_buf;
		bool m_matching;
		int m_startMatch;
		// match a Dfa against lexer's input
		bool Match(ref TOKEN tok,Dfa dfa) 
		{
			char ch=PeekChar();
			int op=m_pch, mark=0;
			Dfa next;
		
			if (m_debug) 
			{
				Console.Write("state {0} with ",dfa.m_state);
				if (char.IsLetterOrDigit(ch)||char.IsPunctuation(ch))
					Console.WriteLine(ch);
				else
					Console.WriteLine("#"+(int)ch);
			}
			if (dfa.m_actions!=null) 
			{
				mark = Mark();
			}
			if (// ch==0 || 
				(next=((Dfa)dfa.m_map[m_tokens.Filter(ch)]))==null) 
			{
				if (m_debug)
					Console.Write("{0} no arc",dfa.m_state);
				if (dfa.m_actions!=null) 
				{
					if (m_debug)
						Console.WriteLine(" terminal");
					return TryActions(dfa,ref tok); // fails on REJECT
				}
				if (m_debug)
					Console.WriteLine(" fails");
				return false;
			}
			Advance();
			if (!Match(ref tok, next)) 
			{ // rest of string fails
				if (m_debug)
					Console.WriteLine("back to {0} with {1}",dfa.m_state,ch);
				if (dfa.m_actions!=null) 
				{ // this is still okay at a terminal
					if (m_debug)
						Console.WriteLine("{0} succeeds",dfa.m_state);
					Restore(mark);
					return TryActions(dfa,ref tok);
				}
				if (m_debug)
					Console.WriteLine("{0} fails",dfa.m_state);
				return false;
			}
			if (m_debug) 
			{
				Console.Write("{0} matched ",dfa.m_state);
				if (m_pch<=m_buf.Length)
					Console.WriteLine(m_buf.Substring(op,m_pch-op));
				else
					Console.WriteLine(m_buf.Substring(op));
			}
			return true;
		}

		// start lexing
		public void Start(StreamReader inFile) 
		{
			m_tokens.GetDfa();
			inFile = new StreamReader(inFile.BaseStream,m_tokens.m_encoding);
			m_buf = inFile.ReadToEnd();
			for (m_pch=0; m_pch<m_buf.Length; m_pch++)
				if (m_buf[m_pch]=='\n')
					m_LineManager.newline(m_pch);
			m_pch = 0;
		}
		public void Start(CsReader inFile) 
		{
			m_tokens.GetDfa();
			inFile = new CsReader(inFile,m_tokens.m_encoding);
			m_LineManager = inFile.lm;
			if (!inFile.Eof())
				for (m_buf = inFile.ReadLine(); !inFile.Eof(); m_buf += inFile.ReadLine()) 
					m_buf+="\n";
			m_pch = 0;
		}
		public void Start(string buf) 
		{ 
			m_tokens.GetDfa();
			m_buf = buf+"\n"; 
			for (m_pch=0; m_pch<m_buf.Length; m_pch++)
				if (m_buf[m_pch]=='\n')
					m_LineManager.newline(m_pch);
			m_pch = 0; 
		}
		public TOKEN Next() 
		{
			TOKEN rv = null;
			while (PeekChar()!=0) 
			{
				Matching(true);
				if (!Match(ref rv,(Dfa)m_tokens.starts[m_state])) 
				{
					if (yypos==0)
						System.Console.Write("Check text encoding.. ");
					Error(String.Format("{0} illegal character {1}",Saypos(yypos), (char)PeekChar()));
					return null;
				}
				Matching (false);
				if (rv!=null) 
				{ // or special value for empty action? 
					rv.pos = m_pch-yytext.Length;
					return rv;
				}
			}
			return null;
		}
		bool TryActions(Dfa dfa,ref TOKEN tok) 
		{
			int len = m_pch-m_startMatch;
			if (len==0)
				return false;
			if (m_startMatch+len<=m_buf.Length)
				yytext = m_buf.Substring(m_startMatch,len);
			else // can happen with {EOF} rules
				yytext = m_buf.Substring(m_startMatch);
			// actions is a list of old-style actions for this DFA in order of priority
			// there is a list because of the chance that any of them may REJECT
			Dfa.Action a = dfa.m_actions;
			bool reject = true;
			while (reject && a!=null) 
			{
				int action = a.a_act;
				reject = false;
				a = a.a_next;
				if (a==null && dfa.m_tokClass!="")  
				{ // last one might not be an old-style action
					tok=(TOKEN)Tfactory.create(dfa.m_tokClass,this);
				} 
				else
					tok = m_tokens.OldAction(this,yytext,action,ref reject);
			}
			return !reject;
		}
		public char PeekChar() 
		{
			if (m_pch<m_buf.Length) 
				return m_buf[m_pch];
			if (m_pch==m_buf.Length && m_tokens.usingEOF)
				return (char)0xFFFF;
			return (char)0;
		}
		public void Advance() { ++m_pch; }
		public virtual int GetChar() 
		{
			int r=PeekChar(); ++m_pch; 
			return r; 
		}
		public void UnGetChar() { if (m_pch>0) --m_pch; }
		internal int Mark() 
		{ 
			return m_pch-m_startMatch; 
		}
		internal void Restore(int mark) 
		{
			m_pch = m_startMatch + mark;
			m_LineManager.backto(m_pch);
		}
		void Matching(bool b) 
		{
			m_matching = b;
			if (b)
				m_startMatch = m_pch;
		}
		internal void Error(string s) 
		{
			m_tokens.Error(s);
			Environment.Exit(-1);
		}
	}
}