using System;
using System.Collections.Generic;
using System.Text;

namespace asn_compile_cs
{
    abstract class CodeEmitter
    {
        abstract public void EmitSymbol(Symbol sym);
        abstract public void PreEmitSymbol(Symbol sym);
        abstract public void Close();
    }
}
