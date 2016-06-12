#include "DLGPoint.h"

using namespace USGSDLGLib;

DLGPoint::DLGPoint(void)
{
  _x = 0.0;
  _y = 0.0;
}

DLGPoint::DLGPoint(double x, double y)
{
  _x = x;
  _y = y;
}

DLGPoint::DLGPoint(const DLGPoint& point)
{
  operator=(point);
}

DLGPoint::~DLGPoint(void)
{
}

DLGPoint& DLGPoint::operator=(const DLGPoint& point)
{
  if (&point != this)
  {
    _x = point._x;
    _y = point._y;
  }
  return *this;
}

bool DLGPoint::operator==(const DLGPoint& point) const
{
  if ((_x == point._x) &&
      (_y == point._y))
  return true;

  return false;
}

bool DLGPoint::operator<(const DLGPoint& point) const
{
  if (_x < point._x)
    return true;
  
  if ((_x == point._x) &&
      (_y < point._y))
    return true;
  
  return false;
}

