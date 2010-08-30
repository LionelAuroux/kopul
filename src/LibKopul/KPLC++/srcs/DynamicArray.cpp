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

llvm::BasicBlock*   DynamicArray::BuildCode(llvm::BasicBlock *, BUILDER_MODE) const
{

}

llvm::BasicBlock*   DynamicArray::BuildEncodingToMemory(llvm::BasicBlock *) const
{

}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromMemory(llvm::BasicBlock *) const
{

}

llvm::BasicBlock*   DynamicArray::BuildEncodingToFile(llvm::BasicBlock *) const
{

}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromFile(llvm::BasicBlock *) const
{

}

llvm::BasicBlock*   DynamicArray::BuildEncodingToSocket(llvm::BasicBlock *) const
{

}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromSocket(llvm::BasicBlock *) const
{

}