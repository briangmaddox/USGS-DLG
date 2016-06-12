#ifndef INCLUDED_DLGPOINT_H
#define INCLUDED_DLGPOINT_H

namespace USGSDLGLib
{

class DLGPoint
{
 public:
  DLGPoint();
  DLGPoint(double x, double y);
  DLGPoint(const DLGPoint& point);
  virtual ~DLGPoint();

  virtual DLGPoint& operator=(const DLGPoint& point);
  virtual bool operator==(const DLGPoint& point) const;
  virtual bool operator<(const DLGPoint& point) const;

  // Accessors
  double getX(void) const;
  double getY(void) const;
  void getXY(double& x, double& y) const;

  // Modifiers
  void  setX(double x);
  void  setY(double y);
  void setXY(double x, double y);

 protected:
  double _x, _y;
};

// Inline Function Definitions

inline double DLGPoint::getX(void) const
{
  return _x;
}

inline void DLGPoint::setX(double x)
{
   _x = x;
}

inline double DLGPoint::getY(void) const
{
  return _y;
}

inline void DLGPoint::setY(double y)
{
  _y = y;
}

inline void DLGPoint::getXY(double& x, double& y) const
{
  x = _x;
  y = _y;
}

inline void DLGPoint::setXY(double x, double y)
{
  _x = x;
  _y = y;
}

} // namespace

#endif  // INCLUDED_DLGPOINT_H
