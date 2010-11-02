#include <exception>
#include <sstream>

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
    *this->_objectToStr = *old._objectToStr;
    static_cast<Container<SwitchCondition>& >(*this) = static_cast<const Container<SwitchCondition>& >(old);
    return (*this);
}

void                    Switch::SetName(const std::string& name)
{
    std::ostringstream  oss;

    Type::SetName(name);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
    {
        //this->_list[i];
    }
}

int                     Switch::GetNbType() const
{
    return (this->_list.size());
}

const Type              &Switch::GetTypeN(int i) const
{
    if (!(i >= 0 && i < (int)this->_list.size()))
        throw std::exception();
    return (this->_list[i]->GetTypeAssociateWithCondition());
}

const std::string&  Switch::ToString() const
{
    std::string&    toStr = *this->_objectToStr;

    std::cout << this->_list.size() << std::endl;
    toStr = this->GetType() + "_";
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        toStr += this->_list[i]->ToString() + (i == this->_list.size() - 1 ? "__" : "__else__");
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

void*               Switch::AllocBuffer(void *oldBuffer) const
{
    SwitchBuffer    *sBuff = new SwitchBuffer;

    sBuff->_buffer = new void*[this->_list.size()];
    sBuff->_succesCase = 0;
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        sBuff->_buffer[i] = this->_list[i]->GetTypeAssociateWithCondition().AllocBuffer(oldBuffer == NULL ? NULL : ((SwitchBuffer *)oldBuffer)->_buffer[i]);
    return (sBuff);
}

void                Switch::FreeBuffer(void *oldBuffer) const
{
    SwitchBuffer    *sBuff = (SwitchBuffer *)oldBuffer;

    if (sBuff == NULL)
        return ;
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        this->_list[i]->GetTypeAssociateWithCondition().FreeBuffer(sBuff->_buffer[i]);
    delete[] sBuff->_buffer;
    delete sBuff;
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
            mapVariable[victor[j]->GetVariableName()] = llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), 32/*victor[j]->GetSizeInOctet() * 8*/));
        }
    }
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*   Switch::BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   Switch::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

const llvm::Type*   Switch::GetLLVMType() const
{
    return (NULL);
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

SwitchCondition    kpl::operator , (const ICondition &cond, const Type &type)
{
    return (SwitchCondition(cond, type));
}

SwitchCondition    kpl::operator , (const Type &type, const ICondition &cond)
{
    return (SwitchCondition(cond, type));
}