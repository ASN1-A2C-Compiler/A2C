using System;

namespace TUVienna.CS_Lex
{
	/// <summary>
	/// Summary description for CError.
	/// </summary>
    /********************************************************
  Class: CError
  *******************************************************/
    public class CError 
    {
        /********************************************************
          Function: impos
          Description:
          *******************************************************/
       public  static void impos
            (
            string message
            )
        {
            System.Console.WriteLine("JLex Error: " + message);
        }

        /********************************************************
          Constants
          Description: Error codes for parse_error().
          *******************************************************/
         public const int E_BADEXPR = 0;
        public  const int E_PAREN = 1;
        public  const int E_LENGTH = 2;
        public  const int E_BRACKET = 3;
        public  const int E_BOL = 4;
        public  const int E_CLOSE = 5;
        public  const int E_NEWLINE = 6;
        public  const int E_BADMAC = 7;
        public  const int E_NOMAC = 8;
        public  const int E_MACDEPTH = 9;
        public  const int E_INIT = 10;
        public  const int E_EOF = 11;
        public  const int E_DIRECT = 12;
        public  const int E_INTERNAL = 13;
        public  const int E_STATE = 14;
        public  const int E_MACDEF = 15;
        public  const int E_SYNTAX = 16;
        public  const int E_BRACE = 17;
        public  const int E_DASH = 18;
        public  const int E_ZERO = 19;
        public  const int E_BADCTRL = 20;
  
        /********************************************************
          Constants
          Description: string messages for parse_error();
          *******************************************************/
        static readonly string[] errmsg = new string[]
    {
        "Malformed regular expression.",
        "Missing close parenthesis.",
        "Too many regular expressions or expression too long.",
        "Missing [ in character class.",
        "^ must be at start of expression or after [.",
        "+ ? or * must follow an expression or subexpression.",
        "Newline in quoted string.",
        "Missing } in macro expansion.",
        "Macro does not exist.",
        "Macro expansions nested too deeply.",
        "JLex has not been successfully initialized.",
        "Unexpected end-of-file found.",
        "Undefined or badly-formed JLex directive.",
        "Internal JLex error.",
        "Unitialized state name.",
        "Badly formed macro definition.",
        "Syntax error.",
        "Missing brace at start of lexical action.",
        "Special character dash - in character class [...] must\n"
        + "\tbe preceded by start-of-range character.",
        "Zero-length regular expression.",
        "Illegal \\^C-style escape sequence (character following caret must\n"
        + "\tbe alphabetic).",
        };
  
        /********************************************************
          Function: parse_error
          Description:
          *******************************************************/
        public static void parse_error
            (
            int error_code,
            int line_number
            )
        {
            System.Console.WriteLine("Error: Parse error at line " 
                + line_number + ".");
            System.Console.WriteLine("Description: " + errmsg[error_code]);
            throw new System.Exception("Parse error.");
        }
    }

}
