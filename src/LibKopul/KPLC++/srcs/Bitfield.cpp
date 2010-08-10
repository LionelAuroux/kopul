#include <sstream>

#include "Bitfield.h"

using namespace kpl;

Bitfield::Bitfield(int size)
{
    std::ostringstream  oss;

    _size = size;
    _objectType = "Bitfield";
    oss << _size;
    _objectToStr = _objectType + oss.str();
}

Bitfield::Bitfield(const Bitfield& bit)
{
    _size = bit._size;
    _objectType = bit._objectType;
    _objectToStr = bit._objectToStr;
    _name = bit._name;
}

Bitfield::~Bitfield()
{

}

Bitfield&   Bitfield::operator = (const Bitfield& bit)
{
    this->_size = bit._size;
    this->_objectType = bit._objectType;
    this->_objectToStr = bit._objectToStr;
    this->_name = bit._name;
    return (*this);
}

int		Bitfield::GetSize() const
{
	return (this->_size);
}

void		Bitfield::SetSize(int size)
{
    std::ostringstream  oss;
    
    this->_size = size;
    oss << this->_size;
    this->_objectToStr = this->_objectType + oss.str();
}

const llvm::Type*   Bitfield::GetLLVMType() const
{
    return (llvm::IntegerType::get(llvm::getGlobalContext(), this->_size));
}

// Get a string representation of the object
const std::string&  Bitfield::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  Bitfield::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                Bitfield::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        if (this->_size == static_cast<const Bitfield&>(value)._size)
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            Bitfield::Clone() const
{
    return (new Bitfield(*this));
}

