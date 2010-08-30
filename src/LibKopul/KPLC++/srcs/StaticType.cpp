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
    /*
	llvm::IRBuilder<>               builder(llvm::getGlobalContext());
        // iterator pour les basickBlocks, ont veut recuperer le 3eme (error).
        llvm::Function::iterator        bIterator = actionBlock->getParent()->begin();++bIterator;++bIterator;
        llvm::BasicBlock                *errorBlock = bIterator;
        llvm::BasicBlock                *addSizeToStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "addSizeTo", actionBlock->getParent());
        llvm::BasicBlock                *caseNotFoundBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "caseNotFound", actionBlock->getParent());
        llvm::BasicBlock                *caseBlock;
        llvm::Value                     *stream;
        llvm::Value                     *streamBytes;
        llvm::Value                     *bytesBitCasted;
        llvm::Value                     *paramBitCasted;
        llvm::Value                     *tmp;
        llvm::Value                     *param;
        llvm::Value                     *tmpMask;
        std::vector<const llvm::Type*>  listType;
        std::string                     nameCaseBlock;

        builder.SetInsertPoint(actionBlock);
        llvm::Value *i8Stream = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"), "i8Stream");
        llvm::Value *nbBytes = builder.CreateLoad(actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"), "nbBytes");
        llvm::SwitchInst*   sw = builder.CreateSwitch(nbBytes, caseNotFoundBlock, 8);
        builder.SetInsertPoint(caseNotFoundBlock);
        builder.CreateBr(errorBlock);
        for (int i = 0; i < 8; ++i)
        {
            listType.clear();
            nameCaseBlock = "case";
            nameCaseBlock += ('0' + (char)i);
            caseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), nameCaseBlock.c_str(), actionBlock->getParent());
            builder.SetInsertPoint(caseBlock);
            if (i == 0)
            {
                stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8)), "stream");
                param = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), "param");
                builder.CreateStore(param, stream);
                builder.CreateBr(addSizeToStreamBlock);
            }
            else
            {
                // On recupere les bytes a cheval puis on bitcast pour obtenir le type definitif storable dans le flux
                streamBytes = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i)), "streamBytes");
                tmp = builder.CreateLoad(streamBytes, "tmp");
//                bytesBitCasted = builder.CreateBitCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize()), "bytesBitCasted");
                bytesBitCasted = builder.CreateIntCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize()), false, "bytesCasted");


                // Load le parametre a ecrire dans le flux puis on bitcast pour obtenir le type definitif storable dans le flux
                param = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), "param");
                paramBitCasted = builder.CreateIntCast(param, llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize()), false, "paramCasted");
                
                // Calcul du mask
                // TODO gestion du little et Big Endian
                tmpMask = builder.CreateMul(paramBitCasted, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(i + this->GetSize(), pow(2, i), false)), "tmpMask");
                tmpMask = builder.CreateAdd(tmpMask, bytesBitCasted, "toStore");

                // Store le resultat du calcul du mask
                stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize())), "stream");
                builder.CreateStore(tmpMask, stream);
                builder.CreateBr(addSizeToStreamBlock);
            }
            sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
        }
	return (this->AddSizeToStream(actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"), actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"), this->GetSize(), addSizeToStreamBlock));
    */
    llvm::Value *streamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value *nbBytesAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite");
    llvm::BasicBlock    *addSizeToStreamBlock = this->StoreParamToStream(streamAdr, nbBytesAdr, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->AddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
}

llvm::BasicBlock*	StaticType::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    llvm::Value *streamAdr = actionBlock->getValueSymbolTable()->lookup("i8StreamAdr");
    llvm::Value *nbBytesAdr = actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead");
    llvm::BasicBlock    *addSizeToStreamBlock = this->LoadParamFromStream(streamAdr, nbBytesAdr, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), this->GetSize(), actionBlock, std::string("addSizeTo"));

    return (this->AddSizeToStream(streamAdr, nbBytesAdr, this->GetSize(), addSizeToStreamBlock));
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