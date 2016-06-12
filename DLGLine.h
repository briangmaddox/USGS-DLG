#ifndef INCLUDED_DLGLINE_H
#define INCLUDED_DLGLINE_H

#ifndef INCLUDED_DLGELEMENT_H
#include "DLGElement.h"
#endif

#ifndef INCLUDED_DLGPOINT_H
#include "DLGPoint.h"
#endif


namespace USGSDLGLib
{

class DLGLine : public DLGElement
{
 public:

  DLGLine(void);
  DLGLine(const DLGLine& le);
  ~DLGLine(void);
  DLGLine& operator=(const DLGLine& le);
  bool operator==(const DLGLine& le);

  friend std::ostream& operator<<(std::ostream& s, DLGLine& le);
  friend std::istream& operator>>(std::istream& s, DLGLine& le);

  // Accessors
  long getStartNodeID(void) const;
  long getEndNodeID(void) const;
  long getLeftAreaID(void) const;
  long getRightAreaID(void) const;

  // Modifiers 
  std::vector<DLGPoint>& getPoints(void);

 protected:

  long _startNodeID;
  long _endNodeID;
  long _areaLeftID;
  long _areaRightID;
  std::vector<DLGPoint> _points;
};

} // namespace

#endif  // INCLUDED_DLGLINE_H
