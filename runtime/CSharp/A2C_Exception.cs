using System;
using System.Collections.Generic;
using System.Text;

namespace A2C
{
    /// <summary>
    /// A2C_Exception provides a single generic exception class that all of the A2C exceptions will derive from.
    /// This class can be used to catch all A2C exceptions, or you can can specific exceptions.
    /// </summary>
    public class A2C_Exception : Exception
    {
        public A2C_Exception() { }
        public A2C_Exception(string message) : base(message) { }
        public A2C_Exception(string message, Exception inner) : base(message, inner) { }
    }

    /// <summary>
    /// The ValueOutOfRangeException is thrown when a program tries to retrieve the value of an item
    /// through an accessor method and the value is too large to fit in the type.  For example, if one
    /// tries to retrieve an integer value through the Int64 accessor, but the value is 9 bytes long.
    /// </summary>
    public class ValueOutOfRangeException : A2C_Exception
    {
        public ValueOutOfRangeException(string message) : base(message) { }
    }

    public class TagMismatchException : A2C_Exception
    {
        public TagMismatchException(string message) : base(message) { }
    }

    /// <summary>
    /// The parser is declaring that it needs more data to complete parsing.
    /// The current structure is partially field, but its state is not reliable in terms of default and optional fields.
    /// </summary>
    public class NeedMoreDataException : A2C_Exception
    {
        public Context context;

        public NeedMoreDataException() 
        {
            context = new Context ();
        }

        public NeedMoreDataException (ContextFrame frame)
        {
            context = new Context ();
            context.Frames.Push (frame);
        }

        public NeedMoreDataException(string message) : base(message)
        {
            context = new Context ();
        }

        public NeedMoreDataException (string message, ContextFrame frame)
        {
            context = new Context ();
            context.Frames.Push (frame);
        }
    }

    public class MalformedEncodingException : A2C_Exception
    {
        public MalformedEncodingException() { }
        public MalformedEncodingException(string message) : base(message) { }
    }

    public class InvalidState : A2C_Exception
    {
        public InvalidState () { }
        public InvalidState (string msg) : base (msg) { }
    }
}
