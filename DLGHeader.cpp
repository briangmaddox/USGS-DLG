#include <iomanip>
#include <strstream>
#include <stdlib.h>

#ifndef INCLUDED_DLGHEADER_H
#include "DLGHeader.h"
#endif

#ifndef INCLUDED_DLGUTIL_H
#include "DLGUtil.h"
#endif

namespace USGSDLGLib
{

static double myatof(char const* str)
{
  // Beleive it or not, atof acts differently on Windows than under 
  // Solaris/gcc. 
  // Under windows, atof accepts 'D' as an exponent. It does _not_ under gcc.
  // This functions wraps atof, replacing 'D' with 'E'.

  std::string buf(str);
  for (long i = 0; i < buf.size(); i++)
    if (buf[i] == 'D')
      buf[i] = 'E';
  
  return atof(buf.c_str());
}

DLGHeader::DLGHeader(void)
  :  _banner(""),
     _dcu_name(""),
     _origSourceMatDate(""),
     _dateQualifier(""),
     _origSourceMatScale(0),
     _line3(""),
     _line10(""),
     _levelCode(0),
     _grndRefSys(0),
     _grndRefSysZone(0),
     _grndRefSysUnits(0),
     _resolution(0.0),
     _numFileToMapParams(0),
     _numAccuracyMiscRecs(0),
     _numSidesInCellPoly(0),
     _numCategories(0),
     _hDatum(0),
     _vDatum(0)
{
}

DLGHeader::~DLGHeader(void)
{
}

std::ostream& operator<<(std::ostream& s, DLGHeader& header)
{
  std::ostrstream ostr;
  int i;             // dummy iteration variable

  // LINE 1
  // Banner
  s.setf(std::ios::left);
  s << std::setw(80) << header._banner << std::endl;

  // LINE 2
  ostr.seekp(0, std::ios::beg);
  ostr << header._dcu_name.substr(0,40);
  ostr << " ";
  ostr << header._origSourceMatDate.substr(0,10);
  ostr << header._dateQualifier.substr(0,1);
  ostr << std::setw(8) << header._origSourceMatScale;
  ostr << std::ends;
  s << std::setw(80) << ostr.str() << std::endl;
  ostr.freeze(0);

  // Line 3
  s << std::setw(80) << header._line3 << std::endl;

  // Line 4
  ostr.seekp(0,std::ios::beg);
  ostr << std::setw(6) << header._levelCode;
  ostr << std::setw(6) << header._grndRefSys;
  ostr << std::setw(6) << header._grndRefSysZone;
  ostr << std::setw(6) << header._grndRefSysUnits;
  ostr.setf(std::ios::fixed, std::ios::floatfield);
  ostr.precision(11);
  ostr << std::setw(18) << header._resolution;
  ostr << std::setw(6) << header._numFileToMapParams;
  ostr << std::setw(6) << header._numAccuracyMiscRecs;
  ostr << std::setw(6) << header._numSidesInCellPoly;
  ostr << std::setw(6) << header._numCategories;
  // START lines added by Mike Childs 5/13/97
  ostr << std::setw(3) << header._hDatum;
  ostr << std::setw(3) << header._vDatum;
  // END lines added by Mike Childs 5/13/97
  ostr << std::ends;
  s << std::setw(80) << ostr.str() << std::endl;
  ostr.freeze(0);
  
  // Lines 5-9.
  ostr.seekp(0, std::ios::beg);
  ostr.flags(std::ios::scientific);
  ostr.precision(15);
  for( i = 0; i < 5; i++ )
  {
    for( int j = 0; j < 3; j++ )
    {
      ostr << std::setw(24) << header._projParams[i * 3 + j];
    }
    
    ostr << std::ends;
    s << std::setw(80) << ostr.str() << std::endl;
    ostr.freeze(0);
    ostr.seekp(0);
  }
  
  // Line 10
  s << std::setw(80) << header._line10 << std::endl;
  
  // Control Point Identification Records
  ostr.flags(std::ios::fixed);
  for (i = 0; i < header._numSidesInCellPoly; i++)
  {
    ostr.seekp(0, std::ios::beg);
    ostr << header._controlPoints[i].label.substr(0,2);
    ostr << "    ";
    ostr.precision(6);
    ostr << std::setw(12) << header._controlPoints[i].latitude;
    ostr << std::setw(12) << header._controlPoints[i].longitude;
    ostr << "      ";
    ostr.precision(2);
    ostr << std::setw(12) << header._controlPoints[i].x;
    ostr << std::setw(12) << header._controlPoints[i].y;
    ostr << std::ends;
    s << std::setw(80) << ostr.str() << std::endl;
    ostr.freeze(0);
  }
  
  // Data Category Identification Records
  for (i = 0; i < header._numCategories; i++)
  {
    ostr.seekp(0, std::ios::beg);
    ostr << header._categories[i].name.substr(0,20);
    ostr << std::setw(4) << header._categories[i].formatCodes;
    ostr << std::setw(6) << header._categories[i].numNodesReferenced;
    ostr << std::setw(6) << header._categories[i].numActualNodes;
    ostr << " ";
    ostr << std::setw(1) << header._categories[i].nodeToAreaLinkFlag;
    ostr << std::setw(1) << header._categories[i].nodeToLineLinkFlag;
    ostr << " ";
    ostr << std::setw(6) << header._categories[i].numAreasReferenced;
    ostr << std::setw(6) << header._categories[i].numActualAreas;
    ostr << " ";
    ostr << std::setw(1) << header._categories[i].areaToNodeLinkFlag;
    ostr << std::setw(1) << header._categories[i].areaToLineLinkFlag;
    ostr << std::setw(1) << header._categories[i].areaCoordListFlag;
    ostr << std::setw(6) << header._categories[i].numLinesReferenced;
    ostr << std::setw(6) << header._categories[i].numActualLines;
    ostr << "   ";
    ostr << std::setw(1) << header._categories[i].lineCoordListFlag;
    ostr << std::ends;
    s << std::setw(80) << ostr.str() << std::endl;
    ostr.freeze(0);
  }
  
  return s;
}

std::istream& operator>>(std::istream& s, DLGHeader& header)
{
  std::string bufstr;
  int i;          // dummy iteration variable

  // LINE 1
  // Banner
  DLGUtil::getRecord(s,bufstr);
  header._banner = bufstr.substr(0,72);

  // LINE 2
  DLGUtil::getRecord(s,bufstr);

  // DCU Name
  header._dcu_name = bufstr.substr(0,40);

  // Original Source Material Date
  header._origSourceMatDate = bufstr.substr(41,10);

  // Date Qualifier
  header._dateQualifier = bufstr.substr(51,1);
  header._origSourceMatScale = atoi(bufstr.substr(52,8).c_str());

  // LINE 3
  DLGUtil::getRecord(s,header._line3);

  // LINE 4
  DLGUtil::getRecord(s,bufstr);

  header._levelCode = atoi(bufstr.substr(0,6).c_str());
  header._grndRefSys = atoi(bufstr.substr(6,6).c_str());
  header._grndRefSysZone = atoi(bufstr.substr(12,6).c_str());
  header._grndRefSysUnits = atoi(bufstr.substr(18,6).c_str());
  header._resolution = atof(bufstr.substr(24,18).c_str());
  header._numFileToMapParams = atoi(bufstr.substr(42,6).c_str());
  header._numAccuracyMiscRecs = atoi(bufstr.substr(48,6).c_str());
  header._numSidesInCellPoly = atoi(bufstr.substr(54,6).c_str());
  header._numCategories = atoi(bufstr.substr(60,6).c_str());

  header._hDatum = strtol(bufstr.substr(66,3).c_str(), 0, 10);
  header._vDatum = strtol(bufstr.substr(69,3).c_str(), 0, 10);


  // START lines added by Justin Ferguson 6/7/97
  // Redundant value checking.  Just in case.
  if ((header._hDatum < 0) || (header._hDatum > 4)) 
    header._hDatum = 0;

  if ((header._vDatum < 0) || (header._vDatum > 2))
    header._vDatum = 0;
  // END lines added by Justin Ferguson 6/7/97

  // Projection parameter records (Lines 5 - 9).
  for (i = 0; i < 5; i++ )
  {
    DLGUtil::getRecord(s,bufstr);
    header._projParams[i*3] = myatof(bufstr.substr(0,24).c_str());
    header._projParams[i*3 + 1] = myatof(bufstr.substr(24, 24).c_str());
    header._projParams[i*3 + 2] = myatof(bufstr.substr(48, 24).c_str());
  }

  // Line 10
  DLGUtil::getRecord(s, header._line10);

  // Control Point Identification Records
  for (i = 0; i < header._numSidesInCellPoly; i++)
  {
    DLGUtil::getRecord(s,bufstr);
    header._controlPoints[i].id = i;
    header._controlPoints[i].label = bufstr.substr(0,2);
    header._controlPoints[i].latitude = atof(bufstr.substr(6,12).c_str());
    header._controlPoints[i].longitude = atof(bufstr.substr(18,12).c_str());
    header._controlPoints[i].x = atof(bufstr.substr(36,12).c_str());
    header._controlPoints[i].y = atof(bufstr.substr(48,12).c_str());
  }
  
  // Data Category Identification Records
  for (i = 0; i < header._numCategories; i++)
  {
    DLGUtil::getRecord(s,bufstr);
    header._categories[i].id = i;
    header._categories[i].name = bufstr.substr(0,20);
    header._categories[i].formatCodes = atoi(bufstr.substr(20,4).c_str());
    header._categories[i].numNodesReferenced = 
      atoi(bufstr.substr(24,6).c_str());
    header._categories[i].numActualNodes = atoi(bufstr.substr(30,6).c_str());
    header._categories[i].nodeToAreaLinkFlag = 
      atoi(bufstr.substr(37,1).c_str());
    header._categories[i].nodeToLineLinkFlag = 
      atoi(bufstr.substr(38,1).c_str());
    header._categories[i].numAreasReferenced = 
      atoi(bufstr.substr(40,6).c_str());
    header._categories[i].numActualAreas = atoi(bufstr.substr(46,6).c_str());
    header._categories[i].areaToNodeLinkFlag = 
      atoi(bufstr.substr(53,1).c_str());
    header._categories[i].areaToLineLinkFlag = 
      atoi(bufstr.substr(54,1).c_str());
    header._categories[i].areaCoordListFlag = 
      atoi(bufstr.substr(55,1).c_str());
    header._categories[i].numLinesReferenced = 
      atoi(bufstr.substr(56,6).c_str());
    header._categories[i].numActualLines = atoi(bufstr.substr(62,6).c_str());
    header._categories[i].lineCoordListFlag = 
      atoi(bufstr.substr(71,1).c_str());
  }
   
  return s;
}

const std::string& DLGHeader::getBanner(void) const
{
  return _banner;
}

const std::string& DLGHeader::getDCU_Name(void) const
{
  // "Digital Cartographic Unit Name"
  // When practical, the name of the source map followed by the State
  // two-character designators.

  return _dcu_name;
}

const std::string& DLGHeader::getOrigSourceMaterialDate(void) const
{
  // Year of original source material followed by the latest revision
  // date if applicable; for example, 1956, 1965.

  return _origSourceMatDate;
}

const std::string& DLGHeader::getDateQualifier(void) const
{
  // Qualifier to descriminate revision date if present. 
  // "P" = photorevision
  // "I" = photo inspection
  // "L" = limited revision

  return _dateQualifier;
}

long DLGHeader::getOrigSourceMaterialScale(void) const
{
  // Scale denominator of source material. For example, 24000.

  return _origSourceMatScale;
}

long DLGHeader::getLevelCode(void) const
{
  // Code = 3, DLG-3.

  return _levelCode;
}

long DLGHeader::getGrndRefSys(void) const
{
  // Code = 1, UTM.

  return _grndRefSys;
}
         
long DLGHeader::getGrndRefSysZone(void) const
{
  // Code defining zone in ground planimeteric reference system.
  // Codes for UTM coordinate zones are given in Appendix C of the standard.

  return _grndRefSysZone;
}

long DLGHeader::getGrndRefSysUnits(void) const
{
  // Code defining units of measure for ground planimetric coordinates
  // throughout the file.
  // Code = 2, meters.

  return _grndRefSysUnits;
}

double DLGHeader::getResolution(void) const
{
  // The true ground distance corresponding to one unit (0.001 inch at 
  // map scale) in the file internal coordinate system used in data
  // collection.
  //    Scale     Resolutions
  //  1:24000       0.61 meters
  //  1:25000       0.635 meters
  //  1:48000       1.22 meters
  //  1:62500       1.587 meters

  return _resolution;
}
         
long DLGHeader::getNumFileToMapParams(void) const
{
  // Number of file to map transformation parameters. Usually 4.

  return _numFileToMapParams;
}

long DLGHeader::getNumAccuracyMiscRecs(void) const
{
  // Number of Accuracy / Miscellaneous records. Currently zero,
  // none included.

  return _numAccuracyMiscRecs;
}

long DLGHeader::getNumSidesInCellPoly(void) const
{
  // Number of sides in the polygon which define the coverage of the cell.
  // Also defines the number of control points. Always 4.

  return _numSidesInCellPoly;
}

long DLGHeader::getNumCategories(void) const
{
  // Number of categories in the DLG file. Value may be from 1 to 32. 
  // The value will be 1.

  return _numCategories;
}

void DLGHeader::getControlPointInfo(long controlPointId, 
                                    DLGControlPoint& cp) const
{
  // Returns the information for the given control point. The control
  // point ids start at zero.

  cp = _controlPoints[controlPointId];
}

bool DLGHeader::setControlPointInfo(long controlPointId,
                                    DLGControlPoint const& cp)
{
  _controlPoints[controlPointId] = cp;
  return true;
}

void DLGHeader::getCategoryInfo(long category, DLGCategory& dlgcat) const
{
  // Returns the information for the requested categories. Category
  // ids are zero based (The first category in the file is category zero (0).

  dlgcat = _categories[category];
}

bool DLGHeader::setCategoryInfo(long category, DLGCategory const& value)
{

  if ((category < 0)||(category > 32))
    return false;

  if (_numCategories <= category)
    return false;

  _categories[category].id = value.id;
  _categories[category].name = value.name;
  _categories[category].formatCodes = value.formatCodes;
  _categories[category].numNodesReferenced = value.numNodesReferenced;
  _categories[category].numActualNodes = value.numActualNodes;
  _categories[category].nodeToAreaLinkFlag = value.nodeToAreaLinkFlag;
  _categories[category].nodeToLineLinkFlag = value.nodeToLineLinkFlag;
  _categories[category].numAreasReferenced = value.numAreasReferenced;
  _categories[category].numActualAreas = value.numActualAreas;
  _categories[category].areaToNodeLinkFlag = value.areaToNodeLinkFlag;
  _categories[category].areaToLineLinkFlag = value.areaToLineLinkFlag;
  _categories[category].areaCoordListFlag = value.areaCoordListFlag;
  _categories[category].numLinesReferenced = value.numLinesReferenced;
  _categories[category].numActualLines = value.numActualLines;
  _categories[category].lineCoordListFlag = value.lineCoordListFlag;
  
  return true;
}

double DLGHeader::getProjectionParameter(int index) const
{
  if ( index >= 0 && index < 15 )
    return _projParams[index];
  else
    return 0;
}

long DLGHeader::getHorizontalDatum() const
{
  return _hDatum;
}

long DLGHeader::getVerticalDatum() const
{
  return _vDatum;
}

} // namespace
