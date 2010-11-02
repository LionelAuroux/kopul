/* 
 * File:   StaticType.cpp
 * Author: deveza
 * 
 * Created on July 31, 2010, 9:42 PM
 */

#include "StaticType.h"

using namespace kpl;

StaticType::StaticType()
{

}

StaticType::~StaticType()
{

}

void*           StaticType::AllocBuffer(void *oldBuffer) const
{
    void        *newBuffer;

    newBuffer = new char[this->GetSizeInOctet() + 1];
    if (oldBuffer != NULL)
        memcpy(newBuffer, oldBuffer, this->GetSizeInOctet());
    return (newBuffer);
}

void            StaticType::FreeBuffer(void *oldBuffer) const
{
    if (oldBuffer == NULL)
        return ;
    delete[] ((char *)oldBuffer);
}

// Build le type
bool                    StaticType::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    mapVariable[this->GetName()] = llvm::PointerType::getUnqual(this->GetLLVMType());
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*	StaticType::BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    llvm::BasicBlock    *addSizeToStreamBlock = this->CreateStoreParamToStream(streamAdr, nbBytesAdr, paramAdr, this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->CreateAddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
}

llvm::BasicBlock*	StaticType::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    llvm::BasicBlock    *addSizeToStreamBlock = this->CreateLoadParamFromStream(streamAdr, nbBytesAdr, paramAdr, this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->CreateAddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
}

llvm::BasicBlock*	StaticType::BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const
{
    // TODO
    return (actionBlock);
}

const llvm::Type*       StaticType::GetLLVMType() const
{
    return (llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8));
}