#include <iostream>
#include <iomanip>
#include <strstream>
#include <stdlib.h>

#ifndef INCLUDED_DLGLINE_H
#include "DLGLine.h"
#endif

#ifndef INCLUDED_DLGPOINT_H
#include "DLGPoint.h"
#endif

#ifndef INCLUDED_DLGUTIL_H
#include "DLGUtil.h"
#endif

// Again, this is done this way here because of Visual C++'s handling of
// friends.

namespace USGSDLGLib
{

DLGLine::DLGLine(void)
{
  _startNodeID = 0;
  _endNodeID = 0;
  _areaLeftID = 0;
  _areaRightID = 0;
}

DLGLine::DLGLine(const DLGLine& le)
{
  operator=(le);
}

DLGLine& DLGLine::operator=(const DLGLine& le)
{
  if (&le != this)
  {
    DLGElement::operator=(le);
    _startNodeID = le._startNodeID;
    _endNodeID = le._endNodeID;
    _areaLeftID = le._areaLeftID;
    _areaRightID = le._areaRightID;
    _points = le._points;
  }
  
  return *this;
}

bool DLGLine::operator==(const DLGLine& le)
{
  if (&le == this)
    return true;
  
  if (DLGElement::operator==(le) != true)
    return false;

  if (_startNodeID != le._startNodeID)
    return false;

  if (_endNodeID != le._endNodeID)
    return false;

  if (_areaLeftID != le._areaLeftID)
    return false;
  
  if (_areaRightID != le._areaRightID)
    return false;

  if (_points != le._points)
    return false;

  return true;
}

DLGLine::~DLGLine(void)
{
}

long DLGLine::getStartNodeID(void) const
{
  return _startNodeID;
}

long DLGLine::getEndNodeID(void) const
{
  return _endNodeID;
}

long DLGLine::getLeftAreaID(void) const
{
  return _areaLeftID;
}

long DLGLine::getRightAreaID(void) const
{
  return _areaRightID;
}

std::vector<DLGPoint>& DLGLine::getPoints(void)
{
  return _points;
}

std::ostream& operator<<(std::ostream& s, DLGLine& le)
{
  std::ostrstream ostr;
  
  s.setf(std::ios::left);
  ostr << le._recordType.substr(0,1);
  ostr << std::setw(5) << le._id;
  ostr << std::setw(6) << le._startNodeID;
  ostr << std::setw(6) << le._endNodeID;
  ostr << std::setw(6) << le._areaLeftID;
  ostr << std::setw(6) << le._areaRightID;
  ostr << std::setw(12) << " ";  // Filler  
  ostr << std::setw(6) << le._points.size();
  ostr << std::setw(6) << le._attrs.size();
  ostr << std::setw(6) << 0; // Number of text attributes.
  ostr << std::ends;
  s << std::setw(80) << ostr.str() << std::endl;
  ostr.freeze(0);

  if (le._points.size() > 0)
  {
    ostr.seekp(0, std::ios::beg);
    ostr.setf(std::ios::fixed, std::ios::floatfield);
    ostr.precision(2);
    int count = 0;
    std::vector<DLGPoint>::iterator i;
    for (i = le._points.begin(); i != le._points.end(); i++)
    {
      double tmpx, tmpy;
      i->getXY(tmpx, tmpy);
      ostr << std::setw(12) << tmpx;
      ostr << std::setw(12) << tmpy;
      count++;
      if ((count > 0) && (count % 3) == 0)
      {
        ostr << std::ends;
        s << std::setw(80) << ostr.str() << std::endl;
        ostr.freeze(0);
        ostr.seekp(0,std::ios::beg);
      }
    }
    // Any left over?
    if ((le._points.size() % 3) != 0)
    {
      ostr << std::ends;
      s << std::setw(80) << ostr.str() << std::endl;
      ostr.freeze(0);
    }
  }
  
  if (le._attrs.size() > 0)
    le.writeAttributes(s);

  return s;
}

std::istream& operator>>(std::istream& s, DLGLine& le)
{
  //string bufstr;
  char bufstr[100];
  char tmpstr[100];

  le._startNodeID = 0;
  le._endNodeID = 0;
  le._areaLeftID = 0;
  le._areaRightID = 0;

  DLGUtil::getRecord(s,bufstr);
  strncpy( tmpstr, bufstr, 1 );
  tmpstr[1] = '\0';
  le._recordType = tmpstr;
  //le._recordType = bufstr.substr(0,1);
  
  strncpy( tmpstr, bufstr + 1, 6 );
  tmpstr[6] = '\0';
  le._id = atoi( tmpstr );
  //le._id = atoi(bufstr.substr(1,6).c_str());
  strncpy( tmpstr, bufstr + 6, 6 );
  tmpstr[6] = '\0';
  le._startNodeID = atoi( tmpstr );
  //le._startNodeID = atoi(bufstr.substr(6,6).c_str());
  strncpy( tmpstr, bufstr + 12, 6 );
  tmpstr[6] = '\0';
  le._endNodeID = atoi( tmpstr );
  //le._endNodeID = atoi(bufstr.substr(12,6).c_str());
  strncpy( tmpstr, bufstr + 18, 6 );
  tmpstr[6] = '\0';
  le._areaLeftID = atoi( tmpstr );
  //le._areaLeftID = atoi(bufstr.substr(18,6).c_str());
  strncpy( tmpstr, bufstr + 24, 6 );
  tmpstr[6] = '\0';
  le._areaRightID = atoi( tmpstr );
  //le._areaRightID = atoi(bufstr.substr(24,6).c_str());

  strncpy( tmpstr, bufstr + 42, 6 );
  tmpstr[6] = '\0';
  long numPoints = atoi( tmpstr );
  //long numPoints = atoi(bufstr.substr(42,6).c_str());
  strncpy( tmpstr, bufstr + 48, 6 );
  tmpstr[6] = '\0';
  long numAttributes = atoi( tmpstr );
  //long numAttributes = atoi(bufstr.substr(48,6).c_str());
  
  le._points.erase(le._points.begin(),le._points.end());
  if (numPoints > 0)
  {
    le._points.reserve(numPoints);
    double tmpx, tmpy;
    long pos;
    for (long i = 0; i < numPoints; i++)
    {
      if (i % 3 == 0)
        DLGUtil::getRecord(s,bufstr);
      pos = (i % 3) * 24;
      strncpy( tmpstr, bufstr + pos, 12 );
      tmpstr[12] = '\0';
      tmpx = atof( tmpstr );
      //tmpx = atof(bufstr.substr(pos,12).c_str());
      strncpy( tmpstr, bufstr + pos + 12, 12 );
      tmpstr[12] = '\0';
      tmpy = atof( tmpstr );
      //tmpy = atof(bufstr.substr(pos + 12, 12).c_str());
      le._points.push_back(DLGPoint(tmpx,tmpy));
    }
  }
  
  le.readAttributes(s, numAttributes);

  return s;
}


} // namespace
