/* 
 * File:   ConditionNode.cpp
 * Author: deveza
 * 
 * Created on September 1, 2010, 3:39 AM
 */

#include "ConditionNode.h"

using namespace kpl;

ConditionNode::ConditionNode(const ConditionNode *left, const ConditionNode *right, const Condition *cond, OPERAND op)
{
    _left = left == NULL ? NULL : static_cast<ConditionNode *>(left->Clone());
    _right = right == NULL ? NULL : static_cast<ConditionNode *>(right->Clone());
    _cond = cond == NULL ? NULL : static_cast<Condition *>(cond->Clone());
    _op = op;
    _objectType = "ConditionNode";
    if (cond == NULL)
        _objectToStr = _objectType + ": " + left->ToString() + CONVERT_OP_TO_STRING(op) + right->ToString();
    else
        _objectToStr = _objectType + ": " + cond->ToString();
}

ConditionNode::ConditionNode(const ConditionNode& orig)
{
    _left = orig._left != NULL? static_cast<ConditionNode *>(orig._left->Clone()) : NULL;
    _right = orig._right != NULL ? static_cast<ConditionNode *>(orig._right->Clone()) : NULL;
    _cond = orig._cond != NULL ? static_cast<Condition *>(orig._cond->Clone()) : NULL;
    _op = orig._op;
    _objectType = orig._objectType;
    _objectToStr = orig._objectToStr;
}

ConditionNode::~ConditionNode()
{
    delete (this->_left);
    delete (this->_right);
    delete (this->_cond);
}

ConditionNode & ConditionNode::operator =(const ConditionNode &old)
{
    delete (this->_left);
    delete (this->_right);
    delete (this->_cond);
    this->_left = static_cast<ConditionNode *>(old._left->Clone());
    this->_right = static_cast<ConditionNode *>(old._right->Clone());
    this->_cond = static_cast<Condition *>(old._cond->Clone());
    this->_op = old._op;
    this->_objectType = old._objectType;
    this->_objectToStr = old._objectToStr;
    return (*this);
}

std::vector<const Variable *>   ConditionNode::GetVariables() const
{
    std::vector<const Variable *>   victor1;
    std::vector<const Variable *>   victor2;
    unsigned int                    i;

    if (this->_cond != NULL)
        victor1 = this->_cond->GetVariables();
    else
    {
        victor1 = this->_left->GetVariables();
        victor2 = this->_right->GetVariables();
        for (i = 0; i < victor2.size(); ++i)
        {
            victor1.push_back(victor2[i]);
//            for (j = 0; j < victor1.size() && victor1[j] != victor2[i]; ++j);
//            if (j != victor1.size())
//                victor1.push_back(victor2[i]);
        }
    }
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
        if (this->_cond == NULL && condN._cond == NULL && this->_left->Equals(*condN._left) && this->_right->Equals(*condN._right) && this->_op == condN._op)
            return (true);
        if (this->_cond != NULL && condN._cond != NULL && this->_cond->Equals(*condN._cond))
            return (true);
    }
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject* ConditionNode::Clone() const
{
    return (new ConditionNode(*this));
}

/*
ConditionNode   &kpl::operator ||(ConditionNode &left, ConditionNode &right)
{
    return (*new ConditionNode(&left, &right, NULL, OR));
}

ConditionNode   &kpl::operator &&(ConditionNode &left, ConditionNode &right)
{
    return (*new ConditionNode(&left, &right, NULL, AND));
}

ConditionNode   &kpl::operator ||(Condition &left, ConditionNode &right)
{
    return (*new ConditionNode(new ConditionNode(NULL, NULL, &left, NOP), &right, NULL, OR));
}

ConditionNode   &kpl::operator &&(Condition &left, ConditionNode &right)
{
    return (*new ConditionNode(new ConditionNode(NULL, NULL, &left, NOP), &right, NULL, AND));
}

ConditionNode   &kpl::operator ||(ConditionNode &left, Condition &right)
{
    return (*new ConditionNode(&left, new ConditionNode(NULL, NULL, &right, NOP), NULL, OR));
}

ConditionNode   &kpl::operator &&(ConditionNode &left, Condition &right)
{
    return (*new ConditionNode(&left, new ConditionNode(NULL, NULL, &right, NOP), NULL, AND));
}

ConditionNode   &kpl::operator ||(Condition &left, Condition &right)
{
    return (*new ConditionNode(new ConditionNode(NULL, NULL, &left, NOP), new ConditionNode(NULL, NULL, &right, NOP), NULL, OR));
}

ConditionNode   &kpl::operator &&(Condition &left, Condition &right)
{
    return (*new ConditionNode(new ConditionNode(NULL, NULL, &left, NOP), new ConditionNode(NULL, NULL, &right, NOP), NULL, AND));
}
*/

ConditionNode   kpl::operator ||(const ConditionNode &left, const ConditionNode &right)
{
    return (ConditionNode(&left, &right, NULL, OR));
}

ConditionNode   kpl::operator &&(const ConditionNode &left, const ConditionNode &right)
{
    return (ConditionNode(&left, &right, NULL, AND));
}

ConditionNode   kpl::operator ||(const Condition &left, const ConditionNode &right)
{
    ConditionNode   cNode(NULL, NULL, &left, NOP);

    return (ConditionNode(&cNode, &right, NULL, OR));
}

ConditionNode   kpl::operator &&(const Condition &left, const ConditionNode &right)
{
    ConditionNode   cNode(NULL, NULL, &left, NOP);
    
    return (ConditionNode(&cNode, &right, NULL, AND));
}

ConditionNode   kpl::operator ||(const ConditionNode &left, const Condition &right)
{
    ConditionNode   cNode(NULL, NULL, &right, NOP);
    
    return (ConditionNode(&left, &cNode, NULL, OR));
}

ConditionNode   kpl::operator &&(const ConditionNode &left, const Condition &right)
{
    ConditionNode   cNode(NULL, NULL, &right, NOP);

    return (ConditionNode(&left, &cNode, NULL, AND));
}

ConditionNode   kpl::operator ||(const Condition &left, const Condition &right)
{
    ConditionNode   cNode1(NULL, NULL, &left, NOP);
    ConditionNode   cNode2(NULL, NULL, &right, NOP);

    return (ConditionNode(&cNode1, &cNode2, NULL, OR));
}

ConditionNode   kpl::operator &&(const Condition &left, const Condition &right)
{
    ConditionNode   cNode1(NULL, NULL, &left, NOP);
    ConditionNode   cNode2(NULL, NULL, &right, NOP);

    return (ConditionNode(&cNode1, &cNode2, NULL, AND));
}