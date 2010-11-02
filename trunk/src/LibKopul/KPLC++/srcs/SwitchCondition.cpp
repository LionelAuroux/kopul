/* 
 * File:   SwitchCondition.cpp
 * Author: deveza
 * 
 * Created on September 20, 2010, 3:50 PM
 */

#include "SwitchCondition.h"
#include "IComposedType.h"

using namespace kpl;

SwitchCondition::SwitchCondition(const ICondition &cond, const Type &type)
{
    _type = static_cast<Type *>(type.Clone());
    _cond = static_cast<ICondition *>(cond.Clone());
    _objectType = "SwitchCondition";
    _objectToStr = _objectType + "_if_" + this->_cond->ToString() + "_so_load_or_store_" + this->_type->ToString();
}

SwitchCondition::SwitchCondition(const SwitchCondition& orig)
{
    _type = static_cast<Type *>(orig._type->Clone());
    _cond = static_cast<ICondition *>(orig._cond->Clone());
    _objectType = "SwitchCondition";
    _objectToStr = _objectType + "_if_" + this->_cond->ToString() + "_so_load_or_store_" + this->_type->ToString();
}

SwitchCondition::~SwitchCondition()
{
    delete (this->_type);
    delete (this->_cond);
}

SwitchCondition & SwitchCondition::operator =(const SwitchCondition &old)
{
    delete (this->_type);
    delete (this->_cond);
    _type = static_cast<Type *>(old._type->Clone());
    _cond = static_cast<ICondition *>(old._cond->Clone());
    _objectToStr = old._objectToStr;
    return (*this);
}

const Type&                     SwitchCondition::GetTypeAssociateWithCondition() const
{
    return (*this->_type);
}

const ICondition&               SwitchCondition::GetCondition() const
{
    return (*this->_cond);
}

void                            SwitchCondition::BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const
{
    this->_cond->BuildCondition(actionBlock, trueBlock, falseBlock);
}

std::vector<const Variable *>   SwitchCondition::GetVariables() const
{
    return (this->_cond->GetVariables());
}

const std::string& SwitchCondition::ToString() const
{
    return (this->_objectToStr);
}

const std::string& SwitchCondition::GetType() const
{
    return (this->_objectType);
}

bool SwitchCondition::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        const SwitchCondition&  sCond = static_cast<const SwitchCondition &>(value);
        if (this->_cond->Equals(*sCond._cond) && this->_type->Equals(*sCond._type))
            return (true);
    }
    return (false);
}

IObject* SwitchCondition::Clone() const
{
    return (new SwitchCondition(*this));
}


