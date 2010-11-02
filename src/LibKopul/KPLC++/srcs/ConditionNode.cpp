/* 
 * File:   ConditionNode.cpp
 * Author: deveza
 * 
 * Created on September 1, 2010, 3:39 AM
 */

#include "ConditionNode.h"

using namespace kpl;

ConditionNode::ConditionNode(const ICondition &left, const ICondition &right, OPERAND op)
{
    _left = static_cast<ICondition *>(left.Clone());
    _right = static_cast<ICondition *>(right.Clone());
    _op = op;
    _objectType = "ConditionNode";
    _objectToStr = _objectType + "__" + left.ToString() + CONVERT_OP_TO_STRING(op) + right.ToString();
}

ConditionNode::ConditionNode(const ConditionNode& orig)
{
    _left = static_cast<ICondition *>(orig._left->Clone());
    _right = static_cast<ICondition *>(orig._right->Clone());
    _op = orig._op;
    _objectType = orig._objectType;
    _objectToStr = orig._objectToStr;
}

ConditionNode::~ConditionNode()
{
    delete (this->_left);
    delete (this->_right);
}

ConditionNode & ConditionNode::operator =(const ConditionNode &old)
{
    delete (this->_left);
    delete (this->_right);
    this->_left = static_cast<ICondition *>(old._left->Clone());
    this->_right = static_cast<ICondition *>(old._right->Clone());
    this->_op = old._op;
    this->_objectType = old._objectType;
    this->_objectToStr = old._objectToStr;
    return (*this);
}

void                            ConditionNode::BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const
{
    llvm::BasicBlock            *leftConditionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "leftConditionBlock", actionBlock->getParent());
    llvm::BasicBlock            *rightConditionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "rightConditionBlock", actionBlock->getParent());
    llvm::IRBuilder<>           builder(llvm::getGlobalContext());

    builder.SetInsertPoint(actionBlock);
    builder.CreateBr(leftConditionBlock);
    if (this->_op == AND)
    {
        this->_left->BuildCondition(leftConditionBlock, rightConditionBlock, falseBlock);
        this->_right->BuildCondition(rightConditionBlock, trueBlock, falseBlock);
    }
    else
    {
        this->_left->BuildCondition(leftConditionBlock, trueBlock, rightConditionBlock);
        this->_right->BuildCondition(rightConditionBlock, trueBlock, falseBlock);
    }
}

std::vector<const Variable *>   ConditionNode::GetVariables() const
{
    std::vector<const Variable *>   victor1;
    std::vector<const Variable *>   victor2;
    unsigned int                    i;

    victor1 = this->_left->GetVariables();
    victor2 = this->_right->GetVariables();
    for (i = 0; i < victor2.size(); ++i)
        victor1.push_back(victor2[i]);
    return (victor1);
}

// Get a string representation of the object
const std::string& ConditionNode::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string& ConditionNode::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool ConditionNode::Equals(const IObject &value) const
{
    if (this->GetType() == value.GetType())
    {
        const ConditionNode &condN = static_cast<const ConditionNode &>(value);
        if (this->_left->Equals(*condN._left) && this->_right->Equals(*condN._right) && this->_op == condN._op)
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject* ConditionNode::Clone() const
{
    return (new ConditionNode(*this));
}

ConditionNode   kpl::operator ||(const ICondition &left, const ICondition &right)
{
    return (ConditionNode(left, right, OR));
}

ConditionNode   kpl::operator &&(const ICondition &left, const ICondition &right)
{
    return (ConditionNode(left, right, AND));
}

