#include "Variable.h"

using namespace kpl;

Variable::Variable(const std::string &name, const StaticType *type) : Value(*type, 0)
{
    _objectType = "Variable";
    _objectToStr = _objectType + "_of_type_" + type->ToString();
    _variableName = name;
}

Variable::Variable(const std::string &name, const StaticType &type) : Value(type, 0)
{
    _objectType = "Variable";
    _objectToStr = _objectType + "_of_type_" + type.ToString();
    _variableName = name;
}

Variable::Variable(const std::string &name, const Type *type, int sizeInBytes) : Value(*type, "", sizeInBytes)
{
    _objectType = "Variable";
    _objectToStr = _objectType + "_of_type_" + type->ToString();
    _variableName = name;
}

Variable::Variable(const std::string &name, const Type &type, int sizeInBytes) : Value(type, "", sizeInBytes)
{
    _objectType = "Variable";
    _objectToStr = _objectType + "_of_type_" + type.ToString();
    _variableName = name;
}

Variable::Variable(const Variable& var) : Value(var)
{
    _objectType = var._objectType;
    _objectToStr = var._objectToStr;
    _variableName = var._variableName;
}

Variable::~Variable()
{

}

char* Variable::operator *()
{
    return (this->_value);
}

char* Variable::GetValue()
{
    return (this->_value);
}

void Variable::Set(const void *value, int sizeInBytes)
{
    if (this->_sizeInBytes < sizeInBytes)
    {
        delete (this->_value);
        this->_value = new char[CONVERT_NBBYTES_TO_NBOCTET(sizeInBytes) + 1];
        this->_value[CONVERT_NBBYTES_TO_NBOCTET(sizeInBytes)] = 0;
    }
    memcpy(this->_value, value, CONVERT_NBBYTES_TO_NBOCTET(sizeInBytes));
}

void                Variable::SetVariableName(const std::string &variableName)
{
    this->_variableName = variableName;
}

const std::string   &Variable::GetVariableName() const
{
    return (this->_variableName);
}

llvm::Value         *Variable::GetLLVMValue(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock->getParent()->getValueSymbolTable().lookup(this->_variableName.c_str()));
}

// Get a string representation of the object
const std::string& Variable::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string& Variable::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool Variable::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType() &&
        this->_sizeInBytes == static_cast<const Variable &>(value)._sizeInBytes
        && strncmp(this->_value, static_cast<const Variable &>(value)._value, CONVERT_NBBYTES_TO_NBOCTET(this->_sizeInBytes)) == 0)
        return (true);
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject* Variable::Clone() const
{
    return (new Variable(*this));
}