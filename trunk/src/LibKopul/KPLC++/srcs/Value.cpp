#include <sstream>

#include "Value.h"

using namespace kpl;

Value::Value(const Type &type)
{
    _type = static_cast<Type *>(type.Clone());
    _value = _type->AllocBuffer();
}

Value::Value(const Value &val)
{
    _type = static_cast<Type *>(val._type->Clone());
    _value = _type->AllocBuffer(val._value);
}

Value&  Value::operator = (const Value &val)
{
    this->_type->FreeBuffer(this->_value);
    delete _type;
    _type = static_cast<Type *>(val._type->Clone());
    _value = _type->AllocBuffer(val._value);
    return (*this);
}

Value::~Value()
{
    this->_type->FreeBuffer(this->_value);
    delete _type;
}

const Type&     Value::GetTypeOfValue() const
{
    return (*this->_type);
}

const void *    Value::GetValue() const
{
    return (this->_value);
}

void            Value::CreateCmp(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock, llvm::Value *toCompare) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    builder.SetInsertPoint(actionBlock);

    llvm::Value* cmp = builder.CreateICmpEQ(this->GetLLVMValue(actionBlock), toCompare, "cmp");
    builder.CreateCondBr(cmp, trueBlock, falseBlock);
}
