#include <sstream>

#include "Value.h"

using namespace kpl;


Value::Value(const StaticType &type, char value)
{
    std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    if (_type->GetSizeInOctet() > (int)sizeof (char))
        memcpy(_value, &value, sizeof (char));
    else
        memcpy(_value, &value, _type->GetSizeInOctet());
    _objectType = "Value";
    oss << value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const StaticType &type, short value)
{
    std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    if (_type->GetSizeInOctet() > (int)sizeof (short))
        memcpy(_value, &value, sizeof (short));
    else
        memcpy(_value, &value, _type->GetSizeInOctet());
    _objectType = "Value";
    oss << value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const StaticType &type, int value)
{
     std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    if (_type->GetSizeInOctet() > (int)sizeof (int))
        memcpy(_value, &value, sizeof (int));
    else
        memcpy(_value, &value, _type->GetSizeInOctet());
    _objectType = "Value";
    oss << value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const StaticType &type, const std::string &value)
{
    std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    memcpy(_value, value.c_str(), _type->GetSizeInOctet());
    _objectType = "Value";
    oss << _value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const StaticType &type, char *value)
{
    std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    memcpy(_value, value, _type->GetSizeInOctet());
    _objectType = "Value";
    oss << _value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const StaticType &type, void *value)
{
    std::ostringstream  oss;

    _type = static_cast<const StaticType *>(type.Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    memcpy(_value, value, _type->GetSizeInOctet());
    _objectType = "Value";
    oss << _value;
    _objectToStr = _objectType + "_of_" + _type->ToString() + "_type_and_value_" + oss.str();
}

Value::Value(const Value &val)
{
    _type = static_cast<const StaticType *>(val._type->Clone());
    _value = new char[_type->GetSizeInOctet() + 1];
    _value[_type->GetSizeInOctet()] = 0;
    memcpy(_value, val._value, _type->GetSizeInOctet());
    _objectType = val._objectType;
    _objectToStr = val._objectToStr;
    SetName(val.GetName());
}

Value::~Value()
{
    delete (this->_type);
    delete (this->_value);
}

Value&  Value::operator = (const Value &val)
{
    delete (this->_type);
    delete (this->_value);
    this->_type = static_cast<const StaticType *>(val._type->Clone());
    this->_value = new char[this->_type->GetSizeInOctet() + 1];
    this->_value[this->_type->GetSizeInOctet()] = 0;
    memcpy(this->_value, val._value, this->_type->GetSizeInOctet());
    this->_objectType = val._objectType;
    this->_objectToStr = val._objectToStr;
    this->SetName(val.GetName());
    return (*this);
}

int    Value::GetSize() const
{
    return (this->_type->GetSize());
}

int    Value::GetSizeInOctet() const
{
    return (this->_type->GetSizeInOctet());
}

const char      *Value::GetValue() const
{
    return (this->_value);
}

bool                    Value::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*	Value::BuildEncodingToMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    builder.SetInsertPoint(actionBlock);
    llvm::Value         *bufferToStore = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->_type->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), this->_type->GetName() + "paramAdr");
    llvm::Value         *value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, 0, false));

    for (int i = 0; i < this->_type->GetSizeInOctet(); ++i)
        value = builder.CreateAdd(value, builder.CreateMul(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, (int)this->_value[i], false)), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, pow(pow(2, 8), i), false))));
    builder.CreateStore(value, bufferToStore);
    llvm::BasicBlock    *newActionBlock = this->_type->BuildEncodingToMemory(actionBlock);
    return (newActionBlock);
}

llvm::BasicBlock*	Value::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    builder.SetInsertPoint(actionBlock);
    llvm::Value         *bufferToStore = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->_type->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), this->_type->GetName() + "paramAdr");
    llvm::Value         *value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, 0, false));
    llvm::BasicBlock    *newActionBlock = this->_type->BuildDecodingFromMemory(actionBlock);
    builder.SetInsertPoint(newActionBlock);
    llvm::Value         *toCompare = builder.CreateLoad(bufferToStore, "ToCompare");

    for (int i = 0; i < this->_type->GetSizeInOctet(); ++i)
        value = builder.CreateAdd(value, builder.CreateMul(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, (int)this->_value[i], false)), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_type->GetSizeInOctet() * 8, pow(pow(2, 8), i), false))));
    newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());
    llvm::Value* cmp = builder.CreateICmpEQ(value, toCompare, "cmp");
	// Si le nombre de bouteille est vide on passe par le block ret sinon on boit
    builder.CreateCondBr(cmp, newActionBlock, static_cast<llvm::BasicBlock *>(actionBlock->getParent()->getValueSymbolTable().lookup("error")));
    return (newActionBlock);
}

llvm::BasicBlock*	Value::BuildEncodingToFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*	Value::BuildDecodingFromFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*	Value::BuildEncodingToSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*	Value::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

const llvm::Type*       Value::GetLLVMType() const
{
    return (this->_type->GetLLVMType());
}

const std::string&  Value::ToString() const
{
    return (this->_objectToStr);
}

const std::string&  Value::GetType() const
{
    return (this->_objectType);
}

bool                Value::Equals(const IObject &value) const
{
    if (this->_objectType == value.GetType())
    {
        if (this->_type->Equals(*static_cast<const Value&>(value)._type) && strcmp(this->_value, static_cast<const Value&>(value)._value) == 0)
            return (true);
    }
    return (false);
}

IObject*            Value::Clone() const
{
    return (new Value(*this));
}