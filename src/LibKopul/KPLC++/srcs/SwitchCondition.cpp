/* 
 * File:   SwitchCondition.cpp
 * Author: deveza
 * 
 * Created on September 20, 2010, 3:50 PM
 */

#include "SwitchCondition.h"

using namespace kpl;

SwitchCondition::SwitchCondition(const ConditionNode &cond, const Type &type)
{
    _type = static_cast<Type *>(type.Clone());
    _cond = static_cast<ConditionNode *>(cond.Clone());
    _objectType = "SwitchCondition";
    _objectToStr = _objectType + ": if (" + this->_cond->ToString() + ") load or store: " + this->_type->ToString();
}

SwitchCondition::SwitchCondition(const SwitchCondition& orig)
{
    _type = static_cast<Type *>(orig._type->Clone());
    _cond = static_cast<ConditionNode *>(orig._cond->Clone());
    _objectType = "SwitchCondition";
    _objectToStr = _objectType + ": if (" + this->_cond->ToString() + ") load or store: " + this->_type->ToString();
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
    _cond = static_cast<ConditionNode *>(old._cond->Clone());
    _objectToStr = old._objectToStr;
    return (*this);
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


