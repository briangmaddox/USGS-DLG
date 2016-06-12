#include "DLGAttribute.h"


using namespace USGSDLGLib;

DLGAttribute::DLGAttribute(void)
{
  _majorCode = 0;
  _minorCode = 0;
}

DLGAttribute::DLGAttribute(short major, short minor)
{
  _majorCode = major;
  _minorCode = minor;
}

DLGAttribute::DLGAttribute(const DLGAttribute& attr)
{
  operator=(attr);
}

DLGAttribute::~DLGAttribute(void)
{
}

DLGAttribute& DLGAttribute::operator=(const DLGAttribute& attr)
{
  if (&attr != this)
  {
    _majorCode = attr._majorCode;
    _minorCode = attr._minorCode;
  }
  return *this;
}

bool DLGAttribute::operator==(const DLGAttribute& attr) const
{
  if ((_majorCode == attr._majorCode) &&
      (_minorCode == attr._minorCode))
  return true;

  return false;
}

bool DLGAttribute::operator<(const DLGAttribute& attr) const
{
  if (_majorCode < attr._majorCode)
    return true;

  if ((_majorCode == attr._majorCode) &&
      (_minorCode < attr._minorCode))
    return true;

  return false;
}
