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

// Build le type
bool                    StaticType::Build(llvm::Module *module, MODE mode) const
{
    std::map<std::string, const llvm::Type *>   mapVariable;

    mapVariable[this->GetName() + "paramAdr"] = llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8));
    return (this->BuildFunctions(module, mapVariable, mode));
}

llvm::BasicBlock*	StaticType::BuildEncodingToMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::Value *streamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value *nbBytesAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite");
    llvm::BasicBlock    *addSizeToStreamBlock = this->CreateStoreParamToStream(streamAdr, nbBytesAdr, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->CreateAddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
}

llvm::BasicBlock*	StaticType::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::Value *streamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value *nbBytesAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead");
    llvm::BasicBlock    *addSizeToStreamBlock = this->CreateLoadParamFromStream(streamAdr, nbBytesAdr, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->CreateAddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
}

llvm::BasicBlock*	StaticType::BuildEncodingToFile(llvm::BasicBlock *actionBlock) const
{

    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromFile(llvm::BasicBlock *actionBlock) const
{

    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildEncodingToSocket(llvm::BasicBlock *actionBlock) const
{

    return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromSocket(llvm::BasicBlock *actionBlock) const
{
    
    return (actionBlock);
}