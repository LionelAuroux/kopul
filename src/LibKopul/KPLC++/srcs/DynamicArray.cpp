#include <exception>

#include "DynamicArray.h"

using namespace kpl;

DynamicArray::DynamicArray(const SwitchCondition& sCond, int nbMax)
{
    _sCond = static_cast<SwitchCondition *>(sCond.Clone());
    _objectType = "DynamicArray";
    _objectToStr = _objectType + "_" + sCond.ToString();
    _nbMax = nbMax;
}

DynamicArray::DynamicArray(const DynamicArray& old)
{
    _sCond = static_cast<SwitchCondition *>(old._sCond->Clone());
    _objectType = old._objectType;
    _objectToStr = old._objectToStr;
    _nbMax = old._nbMax;
}

DynamicArray::~DynamicArray()
{
    delete (this->_sCond);
}

DynamicArray&       DynamicArray::operator = (const DynamicArray& old)
{
    delete (this->_sCond);
    this->_sCond = static_cast<SwitchCondition *>(old._sCond->Clone());
    _objectType = old._objectType;
    _objectToStr = old._objectToStr;
    _nbMax = old._nbMax;
    return (*this);
}

int                 DynamicArray::GetNbType() const
{
    return (this->_nbMax);
}

const Type          &DynamicArray::GetTypeN(int i) const
{
    if (!(i >= 0 && i < this->_nbMax))
        throw (std::exception());
    return (this->_sCond->GetTypeAssociateWithCondition());
}

void                DynamicArray::SetName(const std::string& name)
{
    Type::SetName(name);
//    this->_sCond->GetTypeAssociateWithCondition().SetName(this->_sCond->GetTypeAssociateWithCondition().ToString() + "_in_" + name);
}

void*               DynamicArray::AllocBuffer(void *oldBuffer) const
{
    void            **newBuffer = new void*[this->_nbMax + 1];

    for (int i = 0; i < this->_nbMax; ++i)
        newBuffer[i] = this->_sCond->GetTypeAssociateWithCondition().AllocBuffer(oldBuffer == NULL ? NULL : ((void **)oldBuffer)[i]);
    return (newBuffer);
}

void                DynamicArray::FreeBuffer(void *oldBuffer) const
{
    void            **buffer = (void **)oldBuffer;

    if (buffer == NULL)
        return ;
    for (int i = 0; i < this->_nbMax; ++i)
        this->_sCond->GetTypeAssociateWithCondition().FreeBuffer(buffer[i]);
    delete[] buffer;
}

const SwitchCondition&        DynamicArray::GetSwitchCondition() const
{
    return (*this->_sCond);
}

// Get a string representation of the object
const std::string&  DynamicArray::ToString() const
{
    return (this->_objectToStr);
}

// Get the type of the object
const std::string&  DynamicArray::GetType() const
{
    return (this->_objectType);
}

// returns true if the given type and content are equal.
bool                DynamicArray::Equals(const IObject &val) const
{
    if (this->GetType() == val.GetType() && this->_sCond->Equals(*static_cast<const DynamicArray&>(val)._sCond))
        return (true);
    return (false);
}

// create a new instance by making a deep copy of the current object data
IObject*            DynamicArray::Clone() const
{
    return (new DynamicArray(*this));
}

const llvm::Type*   DynamicArray::GetLLVMType() const
{
    return (llvm::PointerType::getUnqual(llvm::PointerType::getUnqual(this->_sCond->GetTypeAssociateWithCondition().GetLLVMType())));
}

bool                DynamicArray::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    mapVariable[this->GetName()] = this->GetLLVMType();
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*   DynamicArray::MemoryBuilding(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, MEMORY_TYPE flag) const
{
    const std::vector<const Variable *>   &listVariable = this->_sCond->GetVariables();
    llvm::IRBuilder<>               builder(llvm::getGlobalContext());
    llvm::BasicBlock                *loop;
    llvm::BasicBlock                *newBlock;
    llvm::BasicBlock                *typeBuildingBlock;
    llvm::Value                     *lastBytes;
    llvm::Value                     *index;
    llvm::Value                     *i;
    llvm::Value                     *tmpAdr;
    llvm::Value                     *tmp;
    llvm::Value                     *resAdd;

    builder.SetInsertPoint(actionBlock);
    // Create a variable for the LastBytes read
    lastBytes = builder.CreateAlloca(this->_sCond->GetTypeAssociateWithCondition().GetLLVMType(), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "LastBytes");
    for (unsigned int i = 0; i < listVariable.size(); ++i)
        if (listVariable[i]->IsLastBytesRead())
            const_cast<Variable *>(listVariable[i])->SetLLVMValuePointerToLastBytesRead(lastBytes);
    // Create a variable for the index of the array
    index = builder.CreateAlloca(llvm::IntegerType::get(llvm::getGlobalContext(), 32), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "index");
    // Store 0 to the index
    builder.CreateStore(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)), index);
    // Create the loop block
    loop = llvm::BasicBlock::Create(llvm::getGlobalContext(), "loop", actionBlock->getParent());
    // Create the new action block
    newBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "newBlock", actionBlock->getParent());
    // Go to loop
    builder.CreateBr(loop);
    // Set insert point in the loop block
    builder.SetInsertPoint(loop);

    i = builder.CreateLoad(index, "i");
    // extract an element of the array
    tmpAdr = builder.CreateInBoundsGEP(paramAdr, i);
    tmpAdr = builder.CreateLoad(tmpAdr);
    // Add 1 to the index
    resAdd = builder.CreateAdd(i, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "resAdd");
    // store the result of the add to the index
    builder.CreateStore(resAdd, index);
    if (flag == ENCODE)
        typeBuildingBlock = this->_sCond->GetTypeAssociateWithCondition().BuildEncodingToMemory(loop, streamAdr, nbBytesAdr, tmpAdr);
    else
        typeBuildingBlock = this->_sCond->GetTypeAssociateWithCondition().BuildDecodingFromMemory(loop, streamAdr, nbBytesAdr, tmpAdr);
    // load the element
    builder.SetInsertPoint(typeBuildingBlock);
    tmp = builder.CreateLoad(tmpAdr, "tmp");
    // store in the lastBytes read
    builder.CreateStore(tmp, lastBytes);
    // build condition in loop if true return to loop else go to newblock
    this->_sCond->BuildCondition(typeBuildingBlock, loop, newBlock);
    return (newBlock);
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    return (this->MemoryBuilding(actionBlock, streamAdr, nbBytesAdr, paramAdr, ENCODE));
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    return (this->MemoryBuilding(actionBlock, streamAdr, nbBytesAdr, paramAdr, DECODE));
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*   DynamicArray::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}