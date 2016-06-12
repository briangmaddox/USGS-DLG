#include <iostream>
#include <strstream>
#include <iomanip>

#ifndef INCLUDED_DLGELEMENT_H
#include "DLGElement.h"
#endif

#ifndef INCLUDED_DLGUTIL_H
#include "DLGUtil.h"
#endif

using namespace USGSDLGLib;

DLGElement::DLGElement(void)
{
  _recordType = "";
  _id = 0;
}

DLGElement::DLGElement(const DLGElement& element)
{
  operator=(element);
}

DLGElement& DLGElement::operator=(const DLGElement& element)
{
  if (&element != this)
  {
    _recordType = element._recordType;
    _id = element._id;
    _attrs = element._attrs;
  }

  return *this;
}

bool DLGElement::operator==(const DLGElement& element)
{
  return _attrs == element._attrs;
}

DLGElement::~DLGElement(void)
{
}

bool DLGElement::readAttributes(std::istream& s, long numAttributes)
{
  _attrs.erase(_attrs.begin(),_attrs.end());

  if (numAttributes > 0)
  {
    _attrs.reserve( numAttributes );
    std::string bufstr;
    long curmajor, curminor;
    long pos;
    for (long i = 0; i < numAttributes; i++)
    {
      if (i % 6 == 0)
        DLGUtil::getRecord(s,bufstr);
      pos = (i % 6) * 12;
      curmajor = atoi(bufstr.substr(pos, 6).c_str());
      curminor = atoi(bufstr.substr(pos + 6, 6).c_str());
      _attrs.push_back(DLGAttribute(curmajor,curminor));
    }
  }

  return true;
}

bool DLGElement::writeAttributes(std::ostream& s)
{
  if (_attrs.size() > 0)
  {
    std::ostrstream ostr;
    int count = 0;
    std::vector<DLGAttribute>::iterator i;
    for (i = _attrs.begin(); i != _attrs.end(); i++)
    {
      short major,minor;
      major = i->getMajorCode();
      minor = i->getMinorCode();
      ostr << std::setw(6) << major << std::setw(6) << minor;
      count++;
      if ((count > 0) && (count % 6) == 0)
      {
        ostr << std::ends;
        s << std::setw(80) << ostr.str() << std::endl;
        ostr.freeze(0);
        ostr.seekp(0,std::ios::beg);
      }
    }
    // Any left over?
    if ((_attrs.size() % 6) != 0)
    {
      ostr << std::ends;
      s << std::setw(80) << ostr.str() << std::endl;
      ostr.freeze(0);
    }
  }

  return true;
}

