#include "Variable.h"

using namespace kpl;

Variable::Variable(int size)
{
    _type = NULL;
    _buf = new char[size];
    _bufSize = size;
    _objectType = "Variable";
    _objectToStr = _objectType;
}

Variable::Variable(const Type *type, int size)
{
    _type = static_cast<Type *>(type->Clone());
    _buf = new char[size];
    _bufSize = size;
    _objectType = "Variable";
    _objectToStr = _objectType + " of " + type->ToString();
}

Variable::Variable(const Type &type, int size)
{
    _type = static_cast<Type *>(type.Clone());
    _buf = new char[size];
    _bufSize = 0;
    _objectType = "Variable";
    _objectToStr = _objectType + " of " + type.ToString();
}

Variable::Variable(const Variable& var)
{
    _type = static_cast<Type *>(var._type->Clone());
    _bufSize = var._bufSize;
    _buf = new char[_bufSize];
    memcpy(_buf, var._buf, _bufSize);
    _objectType = "Variable";
    _objectToStr = _objectType + " of " + var._type->ToString();
}

Variable::~Variable()
{
    delete (this->_type);
    delete (this->_buf);
}

Variable&   Variable::operator = (const Variable& var)
{
    delete (this->_type);
    delete (this->_buf);
    _type = static_cast<Type *>(var._type->Clone());
    _bufSize = var._bufSize;
    _buf = new char[_bufSize];
    memcpy(_buf, var._buf, _bufSize);
   _objectToStr = _objectType + " of " + var._type->ToString();
   return (*this);
}

const Type& Variable::GetKPLType() const
{
    return (*this->_type);
}

void        Variable::SetType(const Type& type)
{
    this->_type = static_cast<Type *>(type.Clone());
    _objectToStr = _objectType + " of " + type.ToString();
}

void        Variable::SetType(const Type* type)
{
    this->_type = static_cast<Type *>(type->Clone());
    _objectToStr = _objectType + " of " + type->ToString();
}

char*       Variable::Value()
{
    return (this->_buf);
}

// Get a string representation of the object
const std::string&  Variable::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  Variable::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                Variable::Equals(const IObject &value) const
{
    if (this->_objectToStr == value.ToString())
    {
        if (this->_bufSize == static_cast<const Variable&>(value)._bufSize && strncmp(this->_buf, static_cast<const Variable&>(value)._buf, this->_bufSize))
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            Variable::Clone() const
{
    return (new Variable(*this));
}