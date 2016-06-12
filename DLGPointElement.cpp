#include <iostream>
#include <iomanip>
#include <strstream>
#include <vector>

#include <stdlib.h>

#ifndef INCLUDED_DLGPOINTELEMENT_H
#include "DLGPointElement.h"
#endif

#ifndef INCLUDED_DLPPOINT_H
#include "DLGPoint.h"
#endif

#ifndef INCLUDED_DLGUTIL_H
#include "DLGUtil.h"
#endif

namespace USGSDLGLib
{

DLGPointElement::DLGPointElement(void)
{
  _recordType = "";
  _id = 0;
  _numIslands = 0;
}

DLGPointElement::DLGPointElement(const DLGPointElement& pe)
{
  operator=(pe);
}

DLGPointElement::~DLGPointElement(void)
{
}

DLGPointElement& DLGPointElement::operator=(const DLGPointElement& pe)
{
  if (this == &pe)
    return *this;

  DLGElement::operator=(pe);
  _point = pe._point;
  _lineSegments = pe._lineSegments;
  _numIslands = pe._numIslands;

  return *this;
}

bool DLGPointElement::operator==(DLGPointElement& pe)
{
  if (&pe == this)
    return true;

  // XXX This _really_ needs to be looked at.

#if 0
  if (_point != pe._point)
    return false;

  if (_numIslands != pe._numIslands)
    return false;

  if (_lineSegments != pe._lineSegments)
    return false;

  if (_attrs != pe._attrs)
    return false;
#endif

  return true;
}

std::ostream& operator<<(std::ostream& s, DLGPointElement& pe)
{
  std::ostrstream ostr;

  s.setf(std::ios::left);

  ostr << pe._recordType.substr(0,1);
  ostr << std::setw(5) << pe._id;
  ostr.setf(std::ios::fixed, std::ios::floatfield);
  ostr.precision(2);
  ostr << std::setw(12) << pe._point.getX();
  ostr << std::setw(12) << pe._point.getY();
  ostr << std::setw(6) << 0;   // Number of elements in area list or node list.
                               // Not currently included.
  ostr << std::setw(6) << pe._lineSegments.size();
  ostr << std::setw(6) << 0;  // Number of x,y or lat-long points in area 
                              // coordinate lists.
  // Not currently included.
  ostr << std::setw(6) << pe._attrs.size();
  ostr << std::setw(6) << 0;  // Number of text attributes.
  if (pe._recordType == "N")
    ostr << "      ";
  else
    ostr << std::setw(6) << pe._numIslands;
  
  ostr << std::ends;
  s << std::setw(80) << ostr.str() << std::endl;
  ostr.freeze(0);

  if (pe._lineSegments.size() > 0)
  {
    ostr.seekp(0,std::ios::beg);
    int count = 0;
    for (std::vector<long>::iterator i = pe._lineSegments.begin();
         i != pe._lineSegments.end(); i++)
    {
      long tmplong;
      tmplong = *i;
      ostr << std::setw(6) << tmplong;
      count++;
      if ((count > 0) && ((count % 12) == 0))
      {
        ostr << std::ends;
        s << std::setw(80) << ostr.str() << std::endl;
        ostr.freeze(0);
        ostr.seekp(0, std::ios::beg);
      }
    }

    // Any left over?
    if ((pe._lineSegments.size() % 12) != 0)
    {
      ostr << std::ends;
      s << std::setw(80) << ostr.str() << std::endl;
      ostr.freeze(0);
    }
  }

  if (pe._attrs.size() > 0)
    pe.writeAttributes(s);
  
  return s;
}

std::istream& operator>>(std::istream& s, DLGPointElement& pe)
{
  //istream_iterator< string, char, char_traits<char> > j(s); 
  ///string bufstr;//(*j);
  char bufstr[100];
  char tmpstr[100];

  pe._recordType = "";
  pe._id = 0;
  pe._numIslands = 0;

  DLGUtil::getRecord(s,bufstr);
  //pe._recordType = bufstr.substr(0,1);
  strncpy( tmpstr, bufstr, 1 );
  tmpstr[1] = '\0';
  pe._recordType = tmpstr;

  //pe._id = atoi(bufstr.substr(1,6).c_str());
  strncpy( tmpstr, bufstr + 1, 6 );
  tmpstr[6] = '\0';
  pe._id = atoi( tmpstr );

  //double tmpx = atof(bufstr.substr(6,12).c_str());
  //double tmpy = atof(bufstr.substr(18,12).c_str());
  strncpy( tmpstr, bufstr + 6, 12 );
  tmpstr[12] = '\0';
  double tmpx = atof( tmpstr );
  strncpy( tmpstr, bufstr + 18, 12 );
  tmpstr[12] = '\0';
  double tmpy = atof( tmpstr );
  pe._point.setXY(tmpx,tmpy);
  
  //long numLineSegs = atoi(bufstr.substr(36,6).c_str());
  //long numAttrs = atoi(bufstr.substr(48,6).c_str());
  strncpy( tmpstr, bufstr + 36, 6 );
  tmpstr[6] = '\0';
  long numLineSegs = atoi( tmpstr );
  strncpy( tmpstr, bufstr + 48, 6 );
  tmpstr[6] = '\0';
  long numAttrs = atoi( tmpstr );
  
  if (pe._recordType == "A")
  {
    //pe._numIslands = atoi(bufstr.substr(60,6).c_str());
    strncpy( tmpstr, bufstr + 60, 6 );
    tmpstr[6] = '\0';
    pe._numIslands = atoi( tmpstr );
  }
  
  pe._lineSegments.erase(pe._lineSegments.begin(), pe._lineSegments.end());
  if (numLineSegs > 0)
  {
    pe._lineSegments.reserve(numLineSegs);
    long curlineseg;
    for (long i = 0; i < numLineSegs; i++)
    {
      if (i % 12 == 0)
        DLGUtil::getRecord(s,bufstr);
      strncpy( tmpstr, bufstr + (i%12) * 6, 6 );
      tmpstr[6]= '\0';
      curlineseg = atoi( tmpstr );
      //curlineseg = atoi(bufstr.substr((i%12) * 6, 6).c_str());
      pe._lineSegments.push_back(curlineseg);
    }
  }
	
  pe.readAttributes(s, numAttrs);

  return s;
}


} // namespace
