/* 
 * File:   Condition.cpp
 * Author: deveza
 * 
 * Created on September 1, 2010, 3:38 AM
 */

#include "Condition.h"
#include "ConstantValue.h"

using namespace kpl;

Condition::Condition(cmpMptr fptr, const Value &LHS, const Value &RHS, const std::string &cmp)
{
    _objectType = "Condition";
    _objectToStr = _objectType + "_" + LHS.ToStr() + cmp + RHS.ToStr();
    _fptr = fptr;
    _LHS = LHS.Duplicate();
    _RHS = RHS.Duplicate();
}

Condition::Condition(const Condition& orig)
{
    _objectType = orig._objectType;
    _objectToStr = orig._objectToStr;
    _fptr = orig._fptr;
    _LHS = orig._LHS->Duplicate();
    _RHS = orig._RHS->Duplicate();
}

Condition::~Condition()
{
    //delete (this->_LHS);
    //delete (this->_RHS);
}

Condition   &Condition::operator = (const Condition &old)
{
    this->_objectType = old._objectType;
    this->_objectToStr = old._objectToStr;
    this->_fptr = old._fptr;
    delete (this->_LHS);
    delete (this->_RHS);
    this->_LHS = old._LHS->Duplicate();
    this->_RHS = old._RHS->Duplicate();
    return (*this);
}

std::vector<const Variable *> Condition::GetVariables() const
{
    std::vector<const Variable *>   victor;
    const Variable                  *var;

    if ((var = dynamic_cast<const Variable *>(this->_LHS)) != NULL)
        victor.push_back(var);
    if ((var = dynamic_cast<const Variable *>(this->_RHS)) != NULL)
        victor.push_back(var);
    return (victor);
}

// Get a string representation of the object
const std::string&  Condition::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  Condition::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                Condition::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        const Condition&  cond = static_cast<const Condition&>(value);
        if (this->_LHS->ToStr() == cond._LHS->ToStr() && this->_RHS->ToStr() == cond._RHS->ToStr() && this->_fptr == cond._fptr)
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            Condition::Clone() const
{
    return (new Condition(*this));
}

void                Condition::BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());

    builder.SetInsertPoint(actionBlock);
    llvm::Value* cmp = (builder.*this->_fptr)(this->_LHS->GetLLVMValue(actionBlock), this->_RHS->GetLLVMValue(actionBlock), "cmp");
    builder.CreateCondBr(cmp, trueBlock, falseBlock);
}


Condition       kpl::operator == (const Value & LHS, const Value & RHS)
{
    //  icmp eq
    return (Condition(&llvm::IRBuilder<>::CreateICmpEQ, LHS, RHS, "_EQ_"));
}

Condition       kpl::operator != (const Value &LHS, const Value &RHS)
{
    //  icmp ne
    return (Condition(&llvm::IRBuilder<>::CreateICmpNE, LHS, RHS, "_NE_"));
}

Condition       kpl::operator <= (const Value &LHS, const Value &RHS)
{
    //  icmp sle
    return (Condition(&llvm::IRBuilder<>::CreateICmpSLE, LHS, RHS, "_SLE_"));
}

Condition       kpl::operator >= (const Value &LHS, const Value &RHS)
{
    //  icmp sge
    return (Condition(&llvm::IRBuilder<>::CreateICmpSGE, LHS, RHS, "_SGE_"));
}

Condition       kpl::operator < (const Value &LHS, const Value &RHS)
{
    //  icmp slt
    return (Condition(&llvm::IRBuilder<>::CreateICmpSLT, LHS, RHS, "_SLT_"));
}

Condition       kpl::operator > (const Value &LHS, const Value &RHS)
{
    //  icmp sgt
    return (Condition(&llvm::IRBuilder<>::CreateICmpSGT, LHS, RHS, "_SGT_"));
}