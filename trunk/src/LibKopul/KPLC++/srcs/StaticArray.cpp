#include <sstream>

#include "StaticArray.h"

using namespace kpl;

StaticArray::StaticArray(const StaticType& type, int size)
{
   std::ostringstream  oss;

   _staticType = static_cast<StaticType *>(type.Clone());
   _size = size;
   _objectType = "StaticArray";
    oss << _size;
    _objectToStr = _objectType + "_of_" + oss.str() + "_" + _staticType->ToString();
}

StaticArray::StaticArray(const StaticArray& array)
{
    std::ostringstream  oss;

    _staticType = static_cast<StaticType *>(array._staticType->Clone());
    _size = array._size;
    _objectType = "StaticArray";
    oss << _size;
    _objectToStr = _objectType + "_of_" + oss.str() + "_" + _staticType->ToString();
}

StaticArray::~StaticArray()
{
    delete (this->_staticType);
}

StaticArray&    StaticArray::operator = (const StaticArray& array)
{
    delete (this->_staticType);
    this->_staticType = static_cast<StaticType *>(array._staticType->Clone());
    this->_size = array._size;
    return (*this);
}

int             StaticArray::GetSize() const
{
    return (this->_staticType->GetSize() * this->_size);
}

int             StaticArray::GetSizeInOctet() const
{
    if (this->GetSize() % 8 > 0)
        return (this->GetSize() / 8 + 1);
    return (this->GetSize() / 8);
}

// Get a string representation of the object
const std::string&  StaticArray::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  StaticArray::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                StaticArray::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        if (this->_size == static_cast<const StaticArray&>(value)._size &&
            this->_staticType->Equals(*static_cast<const StaticArray&>(value)._staticType))
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            StaticArray::Clone() const
{
    return (new StaticArray(*this));
}