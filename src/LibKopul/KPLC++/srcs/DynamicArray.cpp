#include "DynamicArray.h"

using namespace kpl;

DynamicArray::DynamicArray(const Value& endValue)
{
    _endValue = static_cast<Value *>(endValue.Clone());
    _objectType = "DynamicArray";
    _objectToStr = _objectType + "_where_end_value_is_" + endValue.ToString();
}

DynamicArray::DynamicArray(const DynamicArray& old)
{
    _endValue = static_cast<Value *>(old._endValue->Clone());
    this->SetName(old.GetName());
    _objectType = old._objectType;
    _objectToStr = old._objectToStr;
}

DynamicArray::~DynamicArray()
{
    delete (this->_endValue);
}

DynamicArray&       DynamicArray::operator = (const DynamicArray& old)
{
    delete (this->_endValue);
    this->_endValue = static_cast<Value *>(old._endValue->Clone());
    this->SetName(old.GetName());
    _objectType = old._objectType;
    _objectToStr = old._objectToStr;
    return (*this);
}

const Value&        DynamicArray::GetEndValue() const
{
    return (*this->_endValue);
}

void                DynamicArray::SetEndValue(const Value& newEndValue)
{
    delete (this->_endValue);
    this->_endValue = static_cast<Value *>(newEndValue.Clone());
    _objectToStr = _objectType + "_where_end_value_is_" + newEndValue.ToString();
}

// Get a string representation of the object
const std::string&  DynamicArray::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  DynamicArray::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                DynamicArray::Equals(const IObject &val) const
{
    if (this->GetType() == val.GetType() && this->_endValue->Equals(*static_cast<const DynamicArray&>(val)._endValue))
        return (true);
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            DynamicArray::Clone() const
{
    return (new DynamicArray(*this));
}

bool                DynamicArray::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    mapVariable[this->_endValue->GetTypeOfValue().GetName() + "paramAdr"] = llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), this->_endValue->GetSizeInOctet() * 8));
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    llvm::Value         *i8StreamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value         *nbBytesWriteAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite");

    builder.SetInsertPoint(actionBlock);
    llvm::Value         *streamParam = builder.CreateAlloca(llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), this->_endValue->GetSizeInOctet() * 8)), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    builder.CreateStore(actionBlock->getValueSymbolTable()->lookup(this->_endValue->GetTypeOfValue().GetName() + "paramAdr"), streamParam);
    llvm::Value         *paramAdr = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->_endValue->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    llvm::Value         *nbBytes = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    builder.CreateStore(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), nbBytes);
    llvm::BasicBlock    *loop = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loop", actionBlock->getParent());
    builder.CreateBr(loop);

    llvm::BasicBlock    *addSizeToParamStreamBlock = this->CreateLoadParamFromStream(streamParam, nbBytes, paramAdr, this->_endValue->GetSize(), loop, std::string("addSizeTo"));
    llvm::BasicBlock    *storeParamBlock = this->CreateAddSizeToStream(streamParam, nbBytes, this->_endValue->GetSize(), addSizeToParamStreamBlock);
    llvm::BasicBlock    *addSizeToI8StreamBlock = this->CreateStoreParamToStream(i8StreamAdr, nbBytesWriteAdr, paramAdr, this->_endValue->GetSize(), storeParamBlock, std::string("addSizeTo"));
    llvm::BasicBlock    *cmpBlock = this->CreateAddSizeToStream(i8StreamAdr, nbBytesWriteAdr, this->_endValue->GetSize(), addSizeToI8StreamBlock);
    llvm::BasicBlock    *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());
    builder.SetInsertPoint(cmpBlock);
    llvm::Value         *param = builder.CreateLoad(paramAdr, "param");
    this->_endValue->CreateCmp(param, newActionBlock, loop, cmpBlock);
    return (newActionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());
    llvm::Value         *i8StreamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value         *nbBytesReadAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead");

    builder.SetInsertPoint(actionBlock);
    llvm::Value         *streamParam = builder.CreateAlloca(llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), this->_endValue->GetSizeInOctet() * 8)), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    builder.CreateStore(actionBlock->getValueSymbolTable()->lookup(this->_endValue->GetTypeOfValue().GetName() + "paramAdr"), streamParam);
    llvm::Value         *paramAdr = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), this->_endValue->GetSizeInOctet() * 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    llvm::Value         *nbBytes = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), 8), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "value");
    builder.CreateStore(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), nbBytes);
    llvm::BasicBlock    *loop = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loop", actionBlock->getParent());
    builder.CreateBr(loop);

    llvm::BasicBlock    *addSizeToI8StreamBlock = this->CreateLoadParamFromStream(i8StreamAdr, nbBytesReadAdr, paramAdr, this->_endValue->GetSize(), loop, std::string("addSizeTo"));
    llvm::BasicBlock    *storeParamBlock = this->CreateAddSizeToStream(i8StreamAdr, nbBytesReadAdr, this->_endValue->GetSize(), addSizeToI8StreamBlock);
    llvm::BasicBlock    *addSizeToParamStreamBlock = this->CreateStoreParamToStream(streamParam, nbBytes, paramAdr, this->_endValue->GetSize(), storeParamBlock, std::string("addSizeTo"));
    llvm::BasicBlock    *cmpBlock = this->CreateAddSizeToStream(streamParam, nbBytes, this->_endValue->GetSize(), addSizeToParamStreamBlock);
    llvm::BasicBlock    *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());
    builder.SetInsertPoint(cmpBlock);
    llvm::Value         *param = builder.CreateLoad(paramAdr, "param");
    this->_endValue->CreateCmp(param, newActionBlock, loop, cmpBlock);
    return (newActionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}