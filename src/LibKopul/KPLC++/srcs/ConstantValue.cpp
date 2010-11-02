/* 
 * File:   ConstantValue.cpp
 * Author: deveza
 * 
 * Created on October 1, 2010, 2:38 PM
 */

#include "ConstantValue.h"
#include "StaticType.h"

using namespace kpl;

ConstantValue::ConstantValue(const ConstantValue &val) : Value(static_cast<const Value &>(val))
{
    _objectType = val._objectType;
    _objectToStr = val._objectToStr;
    _sizeInBytes = val._sizeInBytes;
}

ConstantValue::~ConstantValue()
{
    
}

ConstantValue&      ConstantValue::operator = (const ConstantValue &val)
{
    static_cast<Value &>(*this) = static_cast<const Value &>(val);
    _objectType = val._objectType;
    _objectToStr = val._objectToStr;
    _sizeInBytes = val._sizeInBytes;
    return (*this);
}

void*               ConstantValue::AllocBuffer(void *oldBuffer) const
{
    return (this->_type->AllocBuffer(oldBuffer));
}

void                ConstantValue::FreeBuffer(void *oldBuffer) const
{
    this->_type->FreeBuffer(oldBuffer);
}

void                ConstantValue::SetName(const std::string& name)
{
    Type::SetName(name);
    this->_type->SetName(this->_type->ToString() + "_in_" + name);
}

int    ConstantValue::GetSize() const
{
    return (this->_sizeInBytes);
}

int    ConstantValue::GetSizeInOctet() const
{
    return (CONVERT_NBBYTES_TO_NBOCTET(this->_sizeInBytes));
}

llvm::Value     *ConstantValue::GetLLVMValue(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    llvm::Value         *value = llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSizeInOctet() * 8, 0, false));

    builder.SetInsertPoint(actionBlock);
    for (int i = 0; i < this->GetSizeInOctet(); ++i)
    {
        value = builder.CreateAdd(value, builder.CreateMul(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSizeInOctet() * 8, (int)((char *)this->_value)[i], false)), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSizeInOctet() * 8, pow(pow(2, 8), i), false))));
    }
    return (value);
}

const llvm::Type*       ConstantValue::GetLLVMType() const
{
    return (this->_type->GetLLVMType());
}

bool                    ConstantValue::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*	ConstantValue::BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    builder.SetInsertPoint(actionBlock);
    llvm::Value         *bufferToStore = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), this->_type->GetName());

    builder.CreateStore(this->GetLLVMValue(actionBlock), bufferToStore);
    llvm::BasicBlock    *newActionBlock = this->_type->BuildEncodingToMemory(actionBlock, streamAdr, nbBytesAdr, bufferToStore);
    return (newActionBlock);
}

llvm::BasicBlock*	ConstantValue::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());

    builder.SetInsertPoint(actionBlock);
    llvm::Value         *bufferToStore = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), this->_type->GetName());
    llvm::BasicBlock    *newActionBlock = this->_type->BuildDecodingFromMemory(actionBlock, streamAdr, nbBytesAdr, bufferToStore);

    builder.SetInsertPoint(newActionBlock);
    llvm::Value         *toCompare = builder.CreateLoad(bufferToStore, "ToCompare");
    llvm::BasicBlock    *trueBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());

    this->CreateCmp(newActionBlock, trueBlock, static_cast<llvm::BasicBlock *>(actionBlock->getParent()->getValueSymbolTable().lookup("error")), toCompare);
    return (trueBlock);
}

llvm::BasicBlock*	ConstantValue::BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	ConstantValue::BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	ConstantValue::BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	ConstantValue::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

const std::string&  ConstantValue::ToString() const
{
    return (this->_objectToStr);
}

const std::string&  ConstantValue::ToStr() const
{
    return (this->_objectToStr);
}

const std::string&  ConstantValue::GetType() const
{
    return (this->_objectType);
}

bool                ConstantValue::Equals(const IObject &value) const
{
    if (this->_objectType == value.GetType())
    {
        if (this->_type->Equals(*static_cast<const ConstantValue&>(value)._type))
            return (true);
    }
    return (false);
}

IObject*            ConstantValue::Clone() const
{   
    return (new ConstantValue(*this));
}

Value               *ConstantValue::Duplicate() const
{
    return (new ConstantValue(*this));
}