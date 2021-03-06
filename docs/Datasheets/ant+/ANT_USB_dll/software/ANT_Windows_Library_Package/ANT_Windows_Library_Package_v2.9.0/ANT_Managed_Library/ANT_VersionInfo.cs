//////////////////////////////////////////////////////////////////////////
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
 * $Header: /repository/Support_Utilities/ANT_NET/ANT_Managed_Library/ANT_Managed_Library/ANT_VersionInfo.cs,v 1.19 2011-11-14 18:37:56 gallup Exp $
 *
 **************************************************************************
 *
 * To view log use the CVS log command
 */
#endif

#if (!REFERENCE_DESIGN)
// When updating the version info of this class remember to update the project properties Assembly Information version number as well.
// This is the number that shows up when you hover over the library in windows with your mouse, and is accesible without executing any code.
// Open the project properties to the 'Application' tab and click the 'Assembly Information...' button. 
// Keep the Assembly and file information the same: there are four boxes, the first two are for versionNumber the last two are for developmentVersion.
// Also note the unmanaged wrapper and the ANT-FS library have a version string of their own.
// Also change the date to the current date when you change version numbers
// Also change the AssemblyInfo.cs file
#endif

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ANT_Managed_Library
{
    /// <summary>
    /// The information for this version of the ANT Managed Library
    /// </summary>
    public static class ANT_VersionInfo
    {
        private static string applicationCode = "AMO";
        private static string versionSuffix = "";

        /// <summary>
        /// This string shows the date the library received its current version number
        /// </summary>
        public static string versionNumberLastChangedOn = "Jun 17 2011";

        /// <summary>
        /// Returns the version information as a string
        /// </summary>
        /// <returns>Managed Library Version String</returns>
        public static string getManagedLibraryVersion()
        {
            return applicationCode + Assembly.GetExecutingAssembly().GetName().Version.ToString(4) + versionSuffix;
        }


        [DllImport(ANT_Common.ANT_UNMANAGED_WRAPPER, CallingConvention=CallingConvention.Cdecl)]
        private static extern string getUnmanagedVersion();

        /// <summary>
        /// Gets the version string of the underlying unmanaged wrapper library, ANT_WrappedLib.dll
        /// </summary>
        /// <returns>Unmanaged Wrapper Version String</returns>
        public static string getUnmanagedLibraryVersion()
        {
            return getUnmanagedVersion();
        }
    }
}
