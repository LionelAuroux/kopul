/* 
 * File:   VariableIterator.cpp
 * Author: deveza
 * 
 * Created on October 29, 2010, 12:29 AM
 */

#include <exception>

#include "VariableIterator.h"
#include "DynamicArray.h"

using namespace kpl;

VariableIterator::VariableIterator(const Variable &var, void *value) : Variable(var)
{
    _type->FreeBuffer(_value);
    _value = value;
}

VariableIterator::VariableIterator(const VariableIterator& orig) : Variable(orig)
{
    _type->FreeBuffer(_value);
    _value = orig._value;
    _deepList = orig._deepList;
}

VariableIterator::~VariableIterator()
{
    _value = 0;
}

VariableIterator        &VariableIterator::operator = (const VariableIterator& old)
{
    this->_isLastBytesRead = old._isLastBytesRead;
    this->_variableName = old._variableName;
    this->_value = old._value;
    this->_deepList = old._deepList;
    return (*this);
}

VariableIterator    VariableIterator::operator [] (unsigned int i)
{
    VariableIterator    var(*this, this->_value);
    const Type          *type = this->_type;
    const IComposedType *cType;

    for (unsigned int j = 0; j < this->_deepList.size(); ++j)
        type = &dynamic_cast<const IComposedType*>(type)->GetTypeN(this->_deepList[j]);
    if (!((cType = dynamic_cast<const IComposedType*>(type)) && cType->GetNbType() > (int)i))
        throw (std::exception());
    this->_deepList.push_back(i);
    var.SetDeepList(this->_deepList);
    this->_deepList.pop_back();
    return (var);
}

void*                   VariableIterator::operator *()
{
    void                *buffer = this->_value;

    for (unsigned int i = 0; i < this->_deepList.size(); ++i)
        buffer = ((void **)buffer)[this->_deepList[i]];
    return (buffer);
}

llvm::Value             *VariableIterator::GetLLVMValue(llvm::BasicBlock *) const
{
//TODO
}

const std::vector<int>  VariableIterator::GetDeepList() const
{
    return (this->_deepList);
}

void                    VariableIterator::SetDeepList(const std::vector<int>& deepList)
{
    this->_deepList = deepList;
}

Value                   *VariableIterator::Duplicate() const
{
    return (new VariableIterator(*this));
}