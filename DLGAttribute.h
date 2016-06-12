#ifndef INCLUDED_DLGATTRIBUTE_H
#define INCLUDED_DLGATTRIBUTE_H

namespace USGSDLGLib
{

class DLGAttribute
{
 public:

  DLGAttribute(void);
  DLGAttribute(short major, short minor);
  DLGAttribute(const DLGAttribute& attr);
  virtual ~DLGAttribute(void);
  virtual DLGAttribute& operator=(const DLGAttribute& attr);
  virtual bool operator==(const DLGAttribute& attr) const;
  virtual bool operator<(const DLGAttribute& attr) const;

  // Accessors
  short getMajorCode(void) const;
  short getMinorCode(void) const;

  // Modifiers
  void  setMajorCode(short majorCode);
  void  setMinorCode(short minorCode);

 protected:

  short _majorCode;
  short _minorCode;
};

// Inline Function Definitions

inline short DLGAttribute::getMajorCode(void) const
{
  return _majorCode;
}

inline void DLGAttribute::setMajorCode(short majorCode)
{
  _majorCode = majorCode;
}

inline short DLGAttribute::getMinorCode(void) const
{
  return _minorCode;
}

inline void DLGAttribute::setMinorCode(short minorCode)
{
  _minorCode = minorCode;
}

} // namespace


#endif  // INCLUDED_DLGATTRIBUTE_H
