#ifndef INCLUDED_DLGUTIL_H
#define INCLUDED_DLGUTIL_H

#include <iostream>
#include <string>

namespace USGSDLGLib
{

class DLGUtil
{
 public:

  static bool getRecord(std::istream& s, std::string& strbuf,
                        long reclength = 80);
  // Reads a single record from a DLG. 
  // This is more difficult than it should be. According to the standard,
  // DLG-Optional format DLGs should have a fixed record length of 80
  // characters.
  // It says nothing about line termination. In practice, I've seen:
  //    * 80 character records and no line termination.
  //    * 80 character records terminated with '\n'.
  //    * varying length records terminated with '\n'.
  //
  // Returns true if succesful. Returns false if EOF or error.

  static bool getRecord(std::istream& s, char* buf, long reclength = 80);
  // Same as getRecord above. buf should be at least reclength+1 in size.

};

} // namespace
#endif  // INCLUDED_DLGUTIL_HXX
