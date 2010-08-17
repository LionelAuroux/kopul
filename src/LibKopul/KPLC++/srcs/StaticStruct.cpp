#include "StaticStruct.h"

using namespace kpl;

StaticStruct::StaticStruct()
{
    _objectType = "StaticStruct";
    _objectToStr = _objectType + "_of_";
}


StaticStruct::StaticStruct(const StaticStruct& staticStruct)
{
    _objectType = "StaticStruct";
    _objectToStr = _objectType + "_of_";
    _name = staticStruct._name;
    for (unsigned int i = 0; i < staticStruct._listType.size(); ++i)
    {
        this->_listType.push_back(static_cast<StaticType *>(staticStruct._listType[i]->Clone()));
        this->_objectToStr += "_" + staticStruct._listType[i]->ToString();
    }
}

StaticStruct::~StaticStruct()
{
    this->Clear();
}

StaticStruct&   StaticStruct::operator = (const StaticStruct& staticStruct)
{
    this->Clear();
    _objectType = "StaticStruct";
    _objectToStr = _objectType + "_of_";
    _name = staticStruct._name;
    for (unsigned int i = 0; i < staticStruct._listType.size(); ++i)
    {
        this->_listType.push_back(static_cast<StaticType *>(staticStruct._listType[i]->Clone()));
        this->_objectToStr += "_" + staticStruct._listType[i]->ToString();
    }
    return (*this);
}

const std::vector<StaticType *>&  StaticStruct::GetListType() const
{
    return (this->_listType);
}

void                            StaticStruct::Add(const StaticType& type)
{
    this->_listType.push_back(static_cast<StaticType *>(type.Clone()));
    this->_objectToStr += "_" + type.ToString();
}

void                            StaticStruct::Add(const StaticType* type)
{
    this->_listType.push_back(static_cast<StaticType *>(type->Clone()));
    this->_objectToStr += "_" + type->ToString();
}

StaticStruct&                   kpl::operator << (StaticStruct& staticStruct, const StaticType& type)
{
    staticStruct.Add(type);
    return (staticStruct);
}

StaticStruct&                   kpl::operator << (StaticStruct& staticStruct, const StaticType * type)
{
    staticStruct.Add(type);
    return (staticStruct);
}

void                            StaticStruct::Clear()
{
    for (unsigned int i = 0; i < this->_listType.size(); ++i)
        delete (this->_listType[i]);
    _objectToStr = _objectType + "_of_";
    this->_listType.clear();
}

int                 StaticStruct::GetSize() const
{
    int size = 0;

    for (unsigned int i = 0; i < this->_listType.size(); ++i)
        size += this->_listType[i]->GetSize();
    return (size);
}

int                 StaticStruct::GetSizeInOctet() const
{
    if (this->GetSize() % 8 > 0)
        return (this->GetSize() / 8 + 1);
    return (this->GetSize() / 8);
}

const llvm::Type*   StaticStruct::GetLLVMType() const
{
    std::vector<const llvm::Type*> listTypeLLVM;

    for (unsigned int i = 0; i < this->_listType.size(); ++i)
        listTypeLLVM.push_back(this->_listType[i]->GetLLVMType());
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
        if (this->_listType.size() != listTypeOfValue.size())
            return (false);
        for (unsigned int i = 0; i < this->_listType.size(); ++i)
        {
            if (!this->_listType[i]->Equals(*listTypeOfValue[i]))
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