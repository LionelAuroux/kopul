#include "StaticStruct.h"

using namespace kpl;

StaticStruct::StaticStruct()
{
    _objectType = "StaticStruct";
    _objectToStr = _objectType + "_of_";
}


StaticStruct::StaticStruct(const StaticStruct& staticStruct) : Container<StaticType>(staticStruct)
{
    _objectType = "StaticStruct";
    _objectToStr = _objectType + "_of_";
    SetName(staticStruct.GetName());
}

StaticStruct::~StaticStruct()
{
    this->Clear();
}

StaticStruct&   StaticStruct::operator = (const StaticStruct& staticStruct)
{
    this->Clear();
    this->_objectType = "StaticStruct";
    this->_objectToStr = _objectType + "_of_";
    this->SetName(staticStruct.GetName());
    return (*this);
}

const std::vector<StaticType *>&  StaticStruct::GetListType() const
{
    return (this->_list);
}

int                    StaticStruct::GetSize() const
{
    int size = 0;

    for (unsigned int i = 0; i < this->_list.size(); ++i)
        size += this->_list[i]->GetSize();
    return (size);
}

int                    StaticStruct::GetSizeInOctet() const
{
    if (this->GetSize() % 8 > 0)
        return (this->GetSize() / 8 + 1);
    return (this->GetSize() / 8);
}

const llvm::Type*   StaticStruct::GetLLVMType() const
{
    std::vector<const llvm::Type*> listTypeLLVM;

    for (unsigned int i = 0; i < this->_list.size(); ++i)
        listTypeLLVM.push_back(this->_list[i]->GetLLVMType());
    return (llvm::StructType::get(llvm::getGlobalContext(), listTypeLLVM));
}

// Get a string representation of the object
const std::string&  StaticStruct::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  StaticStruct::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                StaticStruct::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        const std::vector<StaticType*>& listTypeOfValue = static_cast<const StaticStruct&>(value).GetListType();
        if (this->_list.size() != listTypeOfValue.size())
            return (false);
        for (unsigned int i = 0; i < this->_list.size(); ++i)
        {
            if (!this->_list[i]->Equals(*listTypeOfValue[i]))
                return (false);
        }
        return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            StaticStruct::Clone() const
{
    return (new StaticStruct(*this));
}