#include "Variable.h"
#include "Switch.h"

using namespace kpl;

// -----------------SWITCH

Switch::Switch() : _objectToStr(new std::string())
{
    _objectType = "Switch";
}

Switch::Switch(const Switch &sw) : Container< SwitchCondition>(sw), _objectToStr(new std::string())
{
    _objectType = "Switch";
}

Switch::~Switch()
{
    delete (this->_objectToStr);
}

Switch&             Switch::operator = (const Switch &old)
{
    this->SetName(old.GetName());
    *this->_objectToStr = *old._objectToStr;
    static_cast<Container<SwitchCondition>& >(*this) = static_cast<const Container<SwitchCondition>& >(old);
    return (*this);
}

const std::string&  Switch::ToString() const
{
    std::string&    toStr = *this->_objectToStr;

    std::cout << this->_list.size() << std::endl;
    toStr = this->GetType() + ":\n";
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        toStr += this->_list[i]->ToString() + "\n";
    return (toStr);
}

const std::string&  Switch::GetType() const
{
    return (this->_objectType);
}

bool                Switch::Equals(const IObject &value) const
{
    if (this->GetType() != value.GetType())
        return (false);
    const Switch &sw = static_cast<const Switch &>(value);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
    {
        if (!this->_list[i]->Equals(*sw._list[i]))
            return (false);
    }
    return (true);
}

IObject*            Switch::Clone() const
{
    return (new Switch(*this));
}

bool                Switch::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;
    std::vector<const Variable *>               victor;

    for (unsigned int i = 0; i < this->_list.size(); ++i)
    {
        victor = this->_list[i]->GetVariables();
        for (unsigned int j = 0; j < victor.size(); ++j)
        {
            mapVariable[victor[j]->GetVariableName()] = llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), victor[j]->GetSizeInOctet() * 8));
        }
    }
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*   Switch::BuildEncodingToMemory(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildEncodingToFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromFile(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildEncodingToSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock) const
{
    return (actionBlock);
}

/*
SwitchCondition&    kpl::operator , (ConditionNode &condNode, const Type &type)
{
    return (*new SwitchCondition(condNode, type));
}

SwitchCondition&    kpl::operator , (Condition &cond, const Type &type)
{
    return (*new SwitchCondition(*new ConditionNode(NULL, NULL, &cond, NOP), type));
}

SwitchCondition&    kpl::operator , (const Type &type, ConditionNode &condNode)
{
    return (*new SwitchCondition(condNode, type));
}

SwitchCondition&    kpl::operator , (const Type &type, Condition &cond)
{
    return (*new SwitchCondition(*new ConditionNode(NULL, NULL, &cond, NOP), type));
}
 */

SwitchCondition    kpl::operator , (const ConditionNode &condNode, const Type &type)
{
    return (SwitchCondition(condNode, type));
}

SwitchCondition    kpl::operator , (const Condition &cond, const Type &type)
{
    return (SwitchCondition(ConditionNode(NULL, NULL, &cond, NOP), type));
}

SwitchCondition    kpl::operator , (const Type &type, const ConditionNode &condNode)
{
    return (SwitchCondition(condNode, type));
}

SwitchCondition    kpl::operator , (const Type &type, const Condition &cond)
{
    return (SwitchCondition(ConditionNode(NULL, NULL, &cond, NOP), type));
}