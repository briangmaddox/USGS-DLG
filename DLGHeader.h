#ifndef INCLUDED_DLGHEADER_H
#define INCLUDED_DLGHEADER_H

#include <string>
#include <iostream>

namespace USGSDLGLib
{

struct DLGControlPoint
{
  long   id;
  std::string label;
  double latitude;
  double longitude;
  double x;
  double y;
};

struct DLGCategory
{
  long   id;
  std::string name;
  long   formatCodes;
  long   numNodesReferenced;
  long   numActualNodes;
  long   nodeToAreaLinkFlag;
  long   nodeToLineLinkFlag;
  long   numAreasReferenced;
  long   numActualAreas;
  long   areaToNodeLinkFlag;
  long   areaToLineLinkFlag;
  long   areaCoordListFlag;
  long   numLinesReferenced;
  long   numActualLines;
  long   lineCoordListFlag;
};


class DLGHeader
{
 public:
  
  DLGHeader(void);
  ~DLGHeader(void);
  friend std::ostream& operator<<(std::ostream& s, DLGHeader& header);
  friend std::istream& operator>>(std::istream& s, DLGHeader& header);

  const std::string& getBanner(void) const;
  // Descriptive text.

  const std::string& getDCU_Name(void) const;
  // "Digital Cartographic Unit Name"
  // When practical, the name of the source map followed by the State
  // two-character designators.

  const std::string& getOrigSourceMaterialDate(void) const;
  // Year of original source material followed by the latest revision
  // date if applicable; for example, 1956, 1965.

  const std::string& getDateQualifier(void) const;
  // Qualifier to descriminate revision date if present. 
  // "P" = photorevision
  // "I" = photo inspection
  // "L" = limited revision

  long getOrigSourceMaterialScale(void) const;
  // Scale denominator of source material. For example, 24000.

  // Line three of header currently not implemented.

  long getLevelCode(void) const;
  // Code = 3, DLG-3.

  long getGrndRefSys(void) const;
  // Code = 1, UTM.
         
  long getGrndRefSysZone(void) const;
  // Code defining zone in ground planimeteric reference system.
  // Codes for UTM coordinate zones are given in Appendix C of the standard.

  long getGrndRefSysUnits(void) const;
  // Code defining units of measure for ground planimetric coordinates
  // throughout the file.
  // Code = 2, meters.

  double getResolution(void) const;
  // The true ground distance corresponding to one unit (0.001 inch at 
  // map scale) in the file internal coordinate system used in data
  // collection.
  //    Scale     Resolutions
  //  1:24000       0.61 meters
  //  1:25000       0.635 meters
  //  1:48000       1.22 meters
  //  1:62500       1.587 meters
  
  long getNumFileToMapParams(void) const;
  // Number of file to map transformation parameters. Usually 4.

  long getNumAccuracyMiscRecs(void) const;
  // Number of Accuracy / Miscellaneous records. Currently zero,
  // none included.

  long getNumSidesInCellPoly(void) const;
  // Number of sides in the polygon which define the coverage of the cell.
  // Also defines the number of control points. Always 4.

  long getNumCategories(void) const;
  // Number of categories in the DLG file. Value may be from 1 to 32. 
  // The value will be 1.

  void getControlPointInfo(long controlpointid, DLGControlPoint& cp) const;
  // Returns the information for the given control point. The control
  // point ids start at zero.

  bool setControlPointInfo(long controlPointId, DLGControlPoint const& cp);

  void getCategoryInfo(long category, DLGCategory& dlgcat) const;
  // Returns the information for the requested categories. Category
  // ids are zero based (The first category in the file is category zero (0).

  bool setCategoryInfo(long category, const DLGCategory& value);
  
  double getProjectionParameter(int index) const;
  // Returns the value of the requested projection parameter.
  // Valid indices range from zero to 14. These parameters
  // are useful when using the gctpc package.  This function
  // returns zero if the index is out of bounds.

  long getHorizontalDatum() const;
  // Returns the horizontal datum code.  A value of
  // zero indicates NAD27, a one indicates NAD83

  long getVerticalDatum() const;
  // Returns the vertical datum code.  A value of zero
  // indicates vertical datum 29, a one indicates vertical
  // datum 87

 private:

  std::string _banner;
  std::string _dcu_name;
  std::string _origSourceMatDate;
  std::string _dateQualifier;
  long _origSourceMatScale;
  std::string _line3;
  std::string _line10;
  long _levelCode;
  long _grndRefSys;
  long _grndRefSysZone;
  long _grndRefSysUnits;
  double _resolution;
  long _numFileToMapParams;
  long _numAccuracyMiscRecs;    // Currently 0, none included.
  long _numSidesInCellPoly;     // Always 4.
  long _numCategories;          // 1 <= numCategories <= 32. 
  long _hDatum;					// Horizontal datum
  long _vDatum;					// Vertical datum
  DLGControlPoint _controlPoints[4];
  DLGCategory _categories[32];
  double _projParams[15];
};

} // namespace

#endif  // INCLUDED_DLGHEADER_H

