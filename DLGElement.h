#ifndef INCLUDED_DLGELEMENT_H
#define INCLUDED_DLGELEMENT_H

#include <iostream>
#include <vector>
#include <string>

#include "DLGAttribute.h"

namespace USGSDLGLib
{

class DLGElement
{
 public:
  
  DLGElement(void);
  DLGElement(const DLGElement& element);
  virtual DLGElement& operator=(const DLGElement& element);
  virtual bool operator==(const DLGElement& element);
  virtual ~DLGElement(void);

  // Accessors
  const std::string& getRecordType(void) const;
  long getID(void) const;
  std::vector<DLGAttribute>& getAttributes(void);

 protected:
  
  bool readAttributes(std::istream& s, long numAttributes);
  bool writeAttributes(std::ostream& s);

  std::string _recordType;
  long _id;
  std::vector<DLGAttribute> _attrs;
};

// Inline Member Function Definitions

inline std::vector<DLGAttribute>& DLGElement::getAttributes(void)
{
  return _attrs;
}

inline const std::string& DLGElement::getRecordType(void) const
{
  return _recordType;
}

inline long DLGElement::getID(void) const
{
  return _id;
}

} // namespace

#endif  // INCLUDED_DLGELEMENT_H
