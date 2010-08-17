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
	llvm::IRBuilder<>               builder(llvm::getGlobalContext());
        // iterator pour les basickBlocks, ont veut recuperer le 3eme (error).
        llvm::Function::iterator        bIterator = actionBlock->getParent()->begin();++bIterator;++bIterator;
        llvm::BasicBlock                *errorBlock = bIterator;
        llvm::BasicBlock                *addSizeToStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "addSizeTo", actionBlock->getParent());
        llvm::BasicBlock                *incStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "incStream", actionBlock->getParent());
        llvm::BasicBlock                *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());
        llvm::BasicBlock                *caseNotFoundBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "caseNotFound", actionBlock->getParent());
        llvm::BasicBlock                *finalyzeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "finalyze", actionBlock->getParent());
        llvm::BasicBlock                *caseBlock;
        llvm::Value                     *stream;
        llvm::Value                     *streamBytes;
//        llvm::Value                     *streamParam;
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
        builder.SetInsertPoint(addSizeToStreamBlock);
        llvm::Value *castStreamToInt = builder.CreatePtrToInt(i8Stream, llvm::IntegerType::get(llvm::getGlobalContext(), 64));
        llvm::Value *addSizeToStream = builder.CreateAdd(castStreamToInt, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, this->GetSize() / 8, false)));
        llvm::Value *addSizeToNbBytes = builder.CreateAdd(nbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, this->GetSize() % 8, false)));

        llvm::Value   *nbBytesSupTo8 = builder.CreateICmpSGT(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 7, false)), "nbBytesSupTo8");
	builder.CreateCondBr(nbBytesSupTo8, incStreamBlock, finalyzeBlock);
        builder.SetInsertPoint(incStreamBlock);
        llvm::Value   *add1ToStream = builder.CreateAdd(addSizeToStream, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 1, false)));
        llvm::Value   *add1ToNbBytes = builder.CreateSub(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 8, false)));
        llvm::Value   *castIntToStream = builder.CreateIntToPtr(add1ToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(add1ToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"));
        builder.CreateBr(newActionBlock);
        builder.SetInsertPoint(finalyzeBlock);
        castIntToStream = builder.CreateIntToPtr(addSizeToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(addSizeToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"));
        builder.CreateBr(newActionBlock);
	return (newActionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
	llvm::IRBuilder<>               builder(llvm::getGlobalContext());
        // iterator pour les basickBlocks, ont veut recuperer le 3eme (error).
        llvm::Function::iterator        bIterator = actionBlock->getParent()->begin();++bIterator;++bIterator;
        llvm::BasicBlock                *errorBlock = bIterator;
        llvm::BasicBlock                *addSizeToStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "addSizeTo", actionBlock->getParent());
        llvm::BasicBlock                *incStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "incStream", actionBlock->getParent());
        llvm::BasicBlock                *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", actionBlock->getParent());
        llvm::BasicBlock                *caseNotFoundBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "caseNotFound", actionBlock->getParent());
        llvm::BasicBlock                *finalyzeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "finalyze", actionBlock->getParent());
        llvm::BasicBlock                *caseBlock;
        llvm::Value                     *stream;
        llvm::Value                     *tmp;
        llvm::Value                     *readFromStream;
        std::vector<const llvm::Type*>  listType;
        std::string                     nameCaseBlock;

        builder.SetInsertPoint(actionBlock);
        llvm::Value *i8Stream = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"), "i8Stream");
        llvm::Value *nbBytes = builder.CreateLoad(actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead"), "nbBytes");
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
                tmp = builder.CreateLoad(stream, "tmp");
                tmp = builder.CreateMul(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSize(), pow(2, (this->GetSizeInOctet() * 8) - this->GetSize()), false)), "tmp");
                readFromStream = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSize(), pow(2, (this->GetSizeInOctet() * 8) - this->GetSize()), false)), "tmp");
                //readFromStream = builder.CreateIntCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8), false, "readFromStream");
                builder.CreateStore(readFromStream, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"));
//                builder.CreateStore(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(i + this->GetSize(), 2, false)), actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"));
                builder.CreateBr(addSizeToStreamBlock);
            }
            else
            {
                stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize())), "streamBytes");
                tmp = builder.CreateLoad(stream);
                tmp = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(i + this->GetSize(), pow(2, i), false)), "resDiv");
                tmp = builder.CreateIntCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), this->GetSizeInOctet() * 8), false, "resCast");
                tmp = builder.CreateMul(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSizeInOctet() * 8, pow(2, (this->GetSizeInOctet() * 8) - this->GetSize()), false)), "tmp");
                tmp = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->GetSizeInOctet() * 8, pow(2, (this->GetSizeInOctet() * 8) - this->GetSize()), false)), "tmp");
                builder.CreateStore(tmp, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"));
                builder.CreateBr(addSizeToStreamBlock);
            }
            sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
        }
        builder.SetInsertPoint(addSizeToStreamBlock);
        llvm::Value *castStreamToInt = builder.CreatePtrToInt(i8Stream, llvm::IntegerType::get(llvm::getGlobalContext(), 64));
        llvm::Value *addSizeToStream = builder.CreateAdd(castStreamToInt, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, this->GetSize() / 8, false)));
        llvm::Value *addSizeToNbBytes = builder.CreateAdd(nbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, this->GetSize() % 8, false)));

        llvm::Value   *nbBytesSupTo8 = builder.CreateICmpSGT(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 7, false)), "nbBytesSupTo8");
	builder.CreateCondBr(nbBytesSupTo8, incStreamBlock, finalyzeBlock);
        builder.SetInsertPoint(incStreamBlock);
        llvm::Value   *add1ToStream = builder.CreateAdd(addSizeToStream, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 1, false)));
        llvm::Value   *add1ToNbBytes = builder.CreateSub(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 8, false)));
        llvm::Value   *castIntToStream = builder.CreateIntToPtr(add1ToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(add1ToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead"));
        builder.CreateBr(newActionBlock);
        builder.SetInsertPoint(finalyzeBlock);
        castIntToStream = builder.CreateIntToPtr(addSizeToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(addSizeToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytesRead"));
        builder.CreateBr(newActionBlock);
	return (newActionBlock);
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