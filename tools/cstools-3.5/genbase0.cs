using System;
using System.IO;

namespace Tools
{
	public class GenBase
	{
		protected StreamWriter m_outFile;
		public string m_outname;
		// convenience functions
		protected int Braces(int a,string b,ref int p,int max) 
		{
			int rv = a;
			int quote = 0;
			for (;p<max;p++)
				if (b[p]=='\\')
					p++;
				else if (quote==0 && b[p]=='{')
					rv++;
				else if (quote==0 && b[p]=='}') 
				{
					if (--rv ==0) 
					{
						p++;
						break;
					}
				}
				else if (b[p]==quote)
					quote=0;
				else if (b[p]=='\'' || b[p]=='"')
					quote = b[p];
			return rv;
		}
		protected string ToBraceIfFound(ref string buf,ref int p,ref int max,CsReader inf)
		{
			int q = p;
			int brack = Braces(0,buf,ref p,max);
			string rv = buf.Substring(q,p-q);
			while (inf!=null && brack>0) 
			{
				buf=inf.ReadLine();
				max=buf.Length;
				if (max==0)
					Error("EOF in action or class def??");
				p=0; 
				rv += '\n';
				brack = Braces(brack,buf,ref p,max);
				rv += buf.Substring(0,p);
			}
			return rv;
		}
		// convenience functions
		public bool White(string buf, ref int offset,int max) 
		{
			while (offset<max && 
				(buf[offset]==' '||buf[offset]=='\t'))
				offset++;
			return offset<max; // false if nothing left
		} 
		public bool NonWhite(string buf, ref int offset,int max) 
		{
			while (offset<max && 
				(buf[offset]!=' ' && buf[offset]!='\t'))
				offset++;
			return offset<max; // false if nothing left
		}
		public void EmitClassDefin(string b,ref int p,int max,CsReader inf,string defbas,out string bas, out string name,bool lx) 
		{
			name = ""; 
			bas = defbas;
			NonWhite(b,ref p,max);
			White(b,ref p,max);
			for(;p<max&&b[p]!=':'&&b[p]!=';';p++)
				name += b[p];
			m_outFile.WriteLine("//%+{0}",name);
			m_outFile.Write("[Serializable] public class ");
			m_outFile.Write(name);
			if (b[p]==':') 
				for(p++,bas="";p<max&&b[p]!=';';p++)
					bas += b[p];
			if (b[p]!=';')
				throw(new Exception("Bad script"));
			new TokClassDef(this,name,bas);
			m_outFile.Write(" : "+bas);
			m_outFile.WriteLine("{");
			m_outFile.WriteLine("public override string yyname() { return \""+name+"\"; }");
			if (lx) 
				m_outFile.WriteLine("public "+name+"(Lexer yym):base(yym){ }}");
			else 
				m_outFile.WriteLine("public "+name+"(Parser yyq):base(yyq){ }}");
		}
		public void Error(string str) 
		{
			Console.WriteLine(str);
			if (m_outFile!=null) 
			{
				m_outFile.WriteLine();
				m_outFile.WriteLine("#error Generator failed earlier. Fix the parser script and run ParserGenerator again.");
			}
			Environment.Exit(-1);
		}
	}
}
