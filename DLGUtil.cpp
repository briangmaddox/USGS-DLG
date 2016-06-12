
#include "DLGUtil.h"

using namespace USGSDLGLib;

bool DLGUtil::getRecord(std::istream& s, std::string& strbuf, long reclength)
{
   char buf[500];
   
   bool flag = DLGUtil::getRecord(s,buf,reclength);
   if (flag == true)
     strbuf = buf;
   
   return flag;
}

bool DLGUtil::getRecord(std::istream& s, char* buf, long reclength)
{
  // buf is assumed to be at least reclength+1 in size.

  if (!s)
    return false;

  long curpos = 0;
  buf[curpos] = s.get();
  while ((buf[curpos] != EOF) && 
         (buf[curpos] != '\n') &&
         (curpos < reclength-1))
  {
    curpos++;
    buf[curpos] = s.get();
  }
  buf[curpos] = '\0';
  
  if (s.peek() == '\n')
    s.get();

  return true;
}
