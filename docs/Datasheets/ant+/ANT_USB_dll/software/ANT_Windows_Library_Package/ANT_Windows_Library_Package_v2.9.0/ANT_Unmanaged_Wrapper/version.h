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

#if !defined(__VERSION_H__)
#define __VERSION_H__

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Version Information
#define SW_VER_PPP_WRAP                "AOA"
#define SW_VER_MAJNUM_WRAP             2
#define SW_VER_MINNUM_WRAP             9
#define SW_VER_BUILD_MAJNUM_WRAP       0
#define SW_VER_BUILD_MINNUM_WRAP       0
#define SW_VER_SUFFIX_WRAP				   ""
#define SW_VER_WRAP                    SW_VER_PPP_WRAP " " TOSTRING(SW_VER_MAJNUM_WRAP) "." TOSTRING(SW_VER_MINNUM_WRAP) "." TOSTRING(SW_VER_BUILD_MAJNUM_WRAP) "." TOSTRING(SW_VER_BUILD_MINNUM_WRAP) SW_VER_SUFFIX_WRAP
#define SW_DESCR_WRAP					   ""
#define SW_VER_NUMONLY_COMMA_WRAP      SW_VER_MAJNUM_WRAP, SW_VER_MINNUM_WRAP, SW_VER_BUILD_MAJNUM_WRAP, SW_VER_BUILD_MINNUM_WRAP

#endif // !defined(CONFIG_H)
