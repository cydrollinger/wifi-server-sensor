﻿//////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009 Dynastream Innovations Inc.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
//////////////////////////////////////////////////////////////////////////


#if (!REFERENCE_DESIGN)
/*
 * CVS Information - autogenerated - do not modify
 *
 * $Header: /repository/Support_Utilities/ANT_NET/ANT_Managed_Library/ANT_Managed_Library/ANT_Exception.cs,v 1.4 2010-06-22 16:07:30 canotinoco Exp $
 *
 **************************************************************************
 *
 * To view log use the CVS log command
 */
#endif

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ANT_Managed_Library
{
    /// <summary>
    /// An exception occuring in the ANT Managed Library
    /// </summary>
    public class ANT_Exception: Exception
    {    
        /// <summary>
        /// Prefixes given string with "ANTLibrary Exception: "
        /// </summary>
        /// <param name="exceptionDetail">String to prefix</param>
        public ANT_Exception(String exceptionDetail) : base("ANTLibrary Exception: " + exceptionDetail) { }

        /// <summary>
        /// Prefixes given string with "ANTLibrary Exception: " and propates inner exception
        /// </summary>
        /// <param name="exceptionDetail">String to prefix</param>
        /// <param name="innerException">Inner exception</param>
        public ANT_Exception(String exceptionDetail, Exception innerException) : base("ANTLibrary Exception: " + exceptionDetail, innerException) { }

        /// <summary>
        /// Copy constructor
        /// </summary>
        /// <param name="aex">ANTException to copy</param>
        public ANT_Exception(ANT_Exception aex) : base(aex.Message) { }   //C++ exceptions like to have a copy constructor
    }
}
