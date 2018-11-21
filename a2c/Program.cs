using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Diagnostics;
using Microsoft.SqlServer.Server;

namespace asn_compile_cs
{
    enum CompileVersion { V2002, V2008 };
    public enum Emitters { Unknown, C, CSharp, Viewer };
    class CmdLineArgs
    {
        public Boolean fAnonUnion = false;
        public int iInline = 1; // 0=no, 1=yes, 2=static __inline
        public String strFileOut = null;
        public Emitters iEmitter = Emitters.Unknown;
        public bool fEmit = true;
        public Boolean fEOLHint = false;
        public CompileVersion iVersion = CompileVersion.V2002;
        public string LookupFile = null;

        public bool Parse(string str)
        {
            if (str.StartsWith("-debug=")) {
                Program.DebugLevel = Convert.ToInt32(str.Substring(7));
            }
            else if (str.StartsWith("-out=")) {
                if (strFileOut != null) {
                    Console.Error.WriteLine("-out specified twice");
                    Error.CErrors += 1;
                }
                strFileOut = str.Substring(5);
            }
            else if (str.StartsWith("-emit=")) {
                if (str == "-emit=yes") {
                    fEmit = true;
                }
                else if (str == "-emit=no") {
                    fEmit = false;
                }
                else {
                    Console.Error.WriteLine("Must be either yes or no for emit option");
                    PrintUsage();
                    Error.CErrors += 1;
                }
            }
            else if (str.StartsWith("-eolhint=")) {
                if (str == "-eolhint=yes") {
                    fEOLHint = true;
                }
                else if (str == "-eolhint=no") {
                    fEOLHint = false;
                }
                else {
                    Console.Error.WriteLine("Must be either yes or no for eolhint option");
                    Error.CErrors += 1;
                }
            }
            else if (str == "-eolhint") {
                fEOLHint = true;
            }
            else if (str.StartsWith("-format=")) {
                String str1;
                if (iEmitter != Emitters.Unknown) {
                    Console.Error.WriteLine("Cannot specify two emitters");
                    PrintUsage();
                    Error.CErrors += 1;
                }

                str1 = str.Substring(8);
                if (str1 == "c") {
                    iEmitter = Emitters.C;
                }
#if EMITTER_CSHARP
                else if (str1 == "CSharp") {
                    iEmitter = Emitters.CSharp;
                }
#endif
                else if (str1 == "Viewer") {
                    iEmitter = Emitters.Viewer;
                }
            }
            else if (str.StartsWith("-anonymous-union=")) {
                if (str == "-anonymous-union=no") {
                    fAnonUnion = false;
                }
                else if (str == "-anonymous-union=yes") {
                    fAnonUnion = true;
                }
                else {
                    Console.Error.WriteLine("Must be either yes or no for anonymous-union option");
                    PrintUsage();
                    Error.CErrors += 1;
                }
            }
            else if (str == "-anonymous-union") {
                fAnonUnion = true;
            }
            else if (str.StartsWith("-inline=")) {
                if (str == "-inline=no") {
                    iInline = 0;
                }
                else if (str == "-inline=yes") {
                    iInline = 1;
                }
                else if (str == "-inline=static") {
                    iInline = 2;
                }
                else if (str == "-inline=none") {
                    iInline = -1;
                }
                else {
                    Console.Error.WriteLine("Must be either yes, no or static for inline option");
                    PrintUsage();
                    Error.CErrors += 1;
                }
            }
            else if (str.StartsWith("-int-size=")) {
                Program.IntSize = Convert.ToInt32(str.Substring(10));
            }
            else if (str.StartsWith("-const=")) {
                if (str == "-const=yes") {
                    Program.UseConst = 1;
                }
                else if (str == "-const=no") {
                    Program.UseConst = 0;
                }
            }
            else if (str.StartsWith("-version=")) {
                if (str == "-version=2002") {
                    iVersion = CompileVersion.V2002;
                }
                else if (str == "-version=2008") {
                    iVersion = CompileVersion.V2008;
                }
                else {
                    Console.Error.WriteLine("Not a recognized version");
                    PrintUsage();
                    Error.CErrors += 1;
                }
            }
            else if (str.StartsWith("-lookup=")) {
                LookupFile = str.Substring(8);
            }
            else {
                Console.Error.WriteLine("Unknown option '{0}'", str);
                Error.CErrors += 1;
            }
            return true;
        }

        ////    PrintUsage
        //
        //  Description:
        //      Give the command line options
        //

        /// <summary>
        /// PrintUsage generates a command line help dump
        /// </summary>
        static void PrintUsage()
        {
            TextWriter errorWriter = Console.Error;

            errorWriter.WriteLine("Usage: a2c -v[file name] -p[base name] <asn files>");
            errorWriter.WriteLine();
            errorWriter.WriteLine("\te\tEmit File");
            errorWriter.WriteLine("\tp\tParse File");
            errorWriter.WriteLine("\tv\tEmit data for AsnView");
            errorWriter.WriteLine();
            errorWriter.WriteLine("\to\tOutput file name base");
            errorWriter.WriteLine();

            errorWriter.WriteLine("\tp\tEmit data for parser");
            Environment.Exit(-1);
        }
    }

    class Program
    {
        public static int DebugLevel = 0;
        public static int IntSize = 32;
        public static bool FNoOptimize = false;        // Use internal program optimizations
        public static int UseConst = 0;
        public static CmdLineArgs args = new CmdLineArgs();
        private static List<AsnFile> _newList = null;

        static void Main(string[] rgstrArgs)
        {
            AF_FLAGS afFlags = AF_FLAGS.None;
            Boolean fFailure;
            Boolean fRepeat = false;
        List<AsnFile> lst = new List<AsnFile>();

        //
        //  Setup some global items that we use later
        //

        Type.Init();
            OIDNode.Init();

            //
            //  Start by parsing the command line.  The command line is a mix of arguments and 
            //  file names so we start parsing as we go along.
            //

            foreach (String str in rgstrArgs) {
                if (str[0] == '-') {
                    args.Parse(str);
                }
                else {
                    TokenizeFile(str);
                }
            }

            if (Error.CErrors > 0) {
                Debug.Assert(Error.CErrors == 0);
                Environment.Exit(-1);
            }

            if (args.LookupFile != null) {
                LoadLookupFile(args.LookupFile);
            }

            args.fEmit = false;


            //
            //  Start doing the parsing of the file now
            //

            lst = Program._newList;
            Program._newList = null;

            if (args.fEOLHint) {
                foreach (AsnFile af in lst) {
                    af.UseEOLHints();
                }
            }


            do {
                fRepeat = false;
                fFailure = false;

                foreach (AsnFile af in lst) {
                    if (DebugLevel > 1) Console.Error.WriteLine(af.fileName);
                    switch (af.Reduce(afFlags)) {
                    case MatchResult.Progress:
                    case MatchResult.ChoiceEnd:
                    case MatchResult.ChoiceLeft:
                    case MatchResult.Success:
                        fRepeat = true;
                        break;


                    case MatchResult.Fail:
                    case MatchResult.NoProgress:
                    case MatchResult.Unknown:
                        fFailure = true;
                        break;

                    case MatchResult.Finished:
                        break;

                    default:
                        Debug.Assert(false);
                        break;
                    }
                }

                //

                if (Program._newList != null) {
                    lst.AddRange(Program._newList);
                    Program._newList = null;
                    fRepeat = true;
                }

                //
                //  If we are marked as no repeat and
                //  we have a failure, then pass in the flag to print errors out
                //

                if (fFailure && !fRepeat) {
                    if ((afFlags & AF_FLAGS.PrintErrors) == 0) {
                        afFlags |= AF_FLAGS.PrintErrors;
                        fRepeat = true;
                        Debug.Assert(false, "Starting Print Error Pass");
                    }
                }
            } while (fRepeat);

            if (Error.CErrors > 0) {
                Debug.Assert(Error.CErrors == 0);
                Environment.Exit(-1);
            }

            //
            //  Need to possibly propagate information forward
            //
            //  A2C requires that a name space be defined - so use A2C if none is set

            AsnFile afLast = null;
            if ((args.iEmitter == Emitters.CSharp) && (lst[0].nameSpace == null)) lst[0].nameSpace = "A2C";
            foreach (AsnFile af in lst) {
                af.CopyForward(afLast);
                afLast = af;
            }

            if (Error.CErrors > 0) {
                Environment.Exit(-1);
            }

            //
            //  Do what re-writing needs to be done on the table
            //

            if (DebugLevel > 1) Console.Error.WriteLine("PHASE: Rewrite...");
            foreach (AsnFile af in lst) {
                if (DebugLevel > 1) Console.Error.WriteLine(af.fileName);
                af.Rewrite();
            }

            if (Error.CErrors > 0) {
                Environment.Exit(-1);
            }

            //
            //  Dump out the symbol table if requested
            //

            if (DebugLevel > 0) {
                foreach (AsnFile af in lst) {
                    af.Dump();
                }

                if (Error.CErrors > 0) {
                    Environment.Exit(-1);
                }
            }

            //
            //  Setup a default output file name
            //

            if (args.strFileOut == null) {
                args.strFileOut = lst[0].fileName;
                if (args.strFileOut.EndsWith(".asn", StringComparison.OrdinalIgnoreCase)) {
                    args.strFileOut = args.strFileOut.Substring(0, args.strFileOut.Length - 4);
                }
            }

            CodeEmitter emitter = null;
            switch (args.iEmitter) {
            case Emitters.Unknown:
            case Emitters.C:
                emitter = new C_CodeEmitter(args.strFileOut, args);
                break;

#if EMITTER_CSHARP
            case Emitters.CSharp:
                emitter = new CSharp_CodeEmitter(args.strFileOut, args);
                break;
#endif

            case Emitters.Viewer:
                emitter = new Viewer_Emitter(args.strFileOut, args);
                break;

            default:
                Console.Error.WriteLine("Unknown emitter");
                break;
            }

            if (emitter != null) {
                foreach (AsnFile af in lst) af.PreEmit(emitter);

                foreach (AsnFile af in lst) {
                    af.Emit(emitter);
                }

                emitter.Close();
            }

            if (Error.CErrors > 0) {
                Environment.Exit(-1);
            }
        }

        public static void TokenizeFile(string str)
        {
            try {
                //
                StreamReader sr;

                //  Open the file and create a tokenizer for it

                sr = File.OpenText(str);

                Lexer lex = new Lexer(sr);

                do {
                    AsnFile af = new AsnFile(str, args.fEmit);

                    //
                    //  Tokenize the file
                    //

                    if (af.ToTokens(lex)) {
                        //
                        //  Add to the list of files to be processed
                        //

                        if (_newList == null) _newList = new List<AsnFile>();
                        if (args.fEOLHint) {
                            af.UseEOLHints();

                        }

                        _newList.Add(af);
                    }
                    else {
                        break;
                    }
                } while (true);
            }
            catch (FileNotFoundException e) {
                Error err = new Error(ErrorNumber.FileNotFound);
                err.AddObject(e.FileName);

                Console.Error.WriteLine(err);
                Error.CErrors += 1;
            }
        }

        public static bool FindModule(Symbol symModule)
        {
            //  We don't have anything to do the lookup with

            if (args.LookupFile == null) return false;
            
            //  Do we have an OID to do the look with?

            if (symModule.value == null) return false;

            if (symModule.value.valueType != ValueType.List) return false;
            ValueList vList =  symModule.value.valueList;
            string oid = "";
            foreach (Value v in vList) {
                oid += "." + v.oidNode.iValue.ToString();
            }

            oid = oid.Substring(1);

            do {
                if (ModuleLookup.ContainsKey(oid)) {
                    LookupItem item = ModuleLookup[oid];
                    if (item.Op) {
                        oid = item.Value;
                    }
                    else {
                        if (!item.Added) {
                            TokenizeFile(item.Value);
                            item.Added = true;
                        }
                        return true;
                    }
                }
                else {
                    return false;
                }
            } while (oid != null);

            return false;
        }

        private static readonly Dictionary<string, LookupItem> ModuleLookup = new Dictionary<string, LookupItem>();

        /*
         *
         * File format:
         * FILE: <OID string value>  <File name>
         * NEW: <OID string value>  <OID string value>
         */
        private static void LoadLookupFile(string fileName)
        {
            string[] lines = File.ReadAllLines(fileName);
            foreach (string line in lines) {
                string[] fields = line.Split();
                if (fields.Length != 3) {
                    Console.WriteLine($"Lookup file error. Line '{line}' ignored.");
                    continue;
                }

                LookupItem item = new LookupItem();
                item.Value = fields[2];

                switch (fields[0].ToLower()) {
                    case "file:":
                        break;

                    case "new:":
                        item.Op = true;
                        break;

                    default:
                        item = null;
                        break;
                }

                if (item == null) {
                    Console.WriteLine($"Lookup file error. Line '{line}' ignored.");
                    continue;
                }

                ModuleLookup.Add(fields[1], item);
            }
        }

        class LookupItem
        {
            public string Value { get; set; }
            public bool Op { get; set; }
            public bool Added { get; set; }
        }
    }
}
