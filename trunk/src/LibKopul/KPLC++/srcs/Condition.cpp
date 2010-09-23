/* 
 * File:   Condition.cpp
 * Author: deveza
 * 
 * Created on September 1, 2010, 3:38 AM
 */

#include "Condition.h"

using namespace kpl;

Condition::Condition(cmpMptr fptr, const Value &LHS, const Value &RHS, const std::string &cmp)
{
    _objectType = "Condition";
    _objectToStr = _objectType + ":" + LHS.ToString() + cmp + RHS.ToString();
    _fptr = fptr;
    _LHS = static_cast<Value *>(LHS.Clone());
    _RHS = static_cast<Value *>(RHS.Clone());
}

Condition::Condition(const Condition& orig)
{
    _objectType = orig._objectType;
    _objectToStr = orig._objectToStr;
    _fptr = orig._fptr;
    _LHS = static_cast<Value *>(orig._LHS->Clone());
    _RHS = static_cast<Value *>(orig._RHS->Clone());
}

Condition::~Condition()
{
    delete (this->_LHS);
    delete (this->_RHS);
}

Condition   &Condition::operator = (const Condition &old)
{
    this->_objectType = old._objectType;
    this->_objectToStr = old._objectToStr;
    this->_fptr = old._fptr;
    this->_LHS = static_cast<Value *>(old._LHS->Clone());
    this->_RHS = static_cast<Value *>(old._RHS->Clone());
    return (*this);
}

std::vector<const Variable *> Condition::GetVariables() const
{
    std::vector<const Variable *> victor;

    if (this->_LHS->GetType() == "Variable")
        victor.push_back(static_cast<const Variable *>(this->_LHS));
    if (this->_RHS->GetType() == "Variable")
        victor.push_back(static_cast<const Variable *>(this->_RHS));
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
        if (this->_LHS->Equals(*cond._LHS) && this->_RHS->Equals(*cond._RHS) && this->_fptr == cond._fptr)
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            Condition::Clone() const
{
    return (new Condition(*this));
}

void        Condition::BuildCode(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());

    builder.SetInsertPoint(actionBlock);
    llvm::Value* cmp = (builder.*this->_fptr)(this->_LHS->GetLLVMValue(actionBlock), this->_RHS->GetLLVMValue(actionBlock), "cmp");
    builder.CreateCondBr(cmp, trueBlock, falseBlock);
}

/*
Condition       &kpl::operator == (const Value & LHS, const Value & RHS)
{
    //  icmp eq
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpEQ, LHS, RHS, "==");

    return (*newCondition);
}

Condition       &kpl::operator != (const Value &LHS, const Value &RHS)
{
    //  icmp ne
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpNE, LHS, RHS, "!=");

    return (*newCondition);
}

Condition       &kpl::operator <= (const Value &LHS, const Value &RHS)
{
    //  icmp sle
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpSLE, LHS, RHS, "<=");

    return (*newCondition);
}

Condition       &kpl::operator >= (const Value &LHS, const Value &RHS)
{
    //  icmp sge
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpSGE, LHS, RHS, ">=");

    return (*newCondition);
}

Condition       &kpl::operator < (const Value &LHS, const Value &RHS)
{
    //  icmp slt
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpSLT, LHS, RHS, "<");

    return (*newCondition);
}

Condition       &kpl::operator > (const Value &LHS, const Value &RHS)
{
    //  icmp sgt
    Condition   *newCondition = new Condition(&llvm::IRBuilder<>::CreateICmpSGT, LHS, RHS, ">");

    return (*newCondition);
}
*/

Condition       kpl::operator == (const Value & LHS, const Value & RHS)
{
    //  icmp eq
    return (Condition(&llvm::IRBuilder<>::CreateICmpEQ, LHS, RHS, "=="));
}

Condition       kpl::operator != (const Value &LHS, const Value &RHS)
{
    //  icmp ne
    return (Condition(&llvm::IRBuilder<>::CreateICmpNE, LHS, RHS, "!="));
}

Condition       kpl::operator <= (const Value &LHS, const Value &RHS)
{
    //  icmp sle
    return (Condition(&llvm::IRBuilder<>::CreateICmpSLE, LHS, RHS, "<="));
}

Condition       kpl::operator >= (const Value &LHS, const Value &RHS)
{
    //  icmp sge
    return (Condition(&llvm::IRBuilder<>::CreateICmpSGE, LHS, RHS, ">="));
}

Condition       kpl::operator < (const Value &LHS, const Value &RHS)
{
    //  icmp slt
    return (Condition(&llvm::IRBuilder<>::CreateICmpSLT, LHS, RHS, "<"));
}

Condition       kpl::operator > (const Value &LHS, const Value &RHS)
{
    //  icmp sgt
    return (Condition(&llvm::IRBuilder<>::CreateICmpSGT, LHS, RHS, ">"));
}