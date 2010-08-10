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

    mapVariable[this->GetName() + "paramAdr"] = llvm::PointerType::getUnqual(this->GetLLVMType());
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
        llvm::Value                     *tmp;
        llvm::Value                     *resultOfInsertValue;
        llvm::Value                     *param;
        std::vector<const llvm::Type*>  listType;
        std::string                     nameCaseBlock;

        builder.SetInsertPoint(actionBlock);
        llvm::Value *i8Stream = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"), "i8Stream");
        llvm::Value *nbBytes = builder.CreateLoad(actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"), "nbBytes");
        llvm::SwitchInst*   sw = builder.CreateSwitch(nbBytes, caseNotFoundBlock, 8);
        builder.SetInsertPoint(caseNotFoundBlock);
        builder.CreateBr(errorBlock);
        for (int i = 1; i < 8; ++i)
        {
            listType.clear();
            nameCaseBlock = "case";
            nameCaseBlock += ('0' + (char)i);
            caseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), nameCaseBlock.c_str(), actionBlock->getParent());
            builder.SetInsertPoint(caseBlock);
            listType.push_back(llvm::IntegerType::get(llvm::getGlobalContext(), i));
            listType.push_back(this->GetLLVMType());
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::StructType::get(llvm::getGlobalContext(), listType)), "stream");
            tmp = builder.CreateLoad(stream, "tmp");
            param = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"), "param");
            resultOfInsertValue = builder.CreateInsertValue(tmp, param, 1);
            builder.CreateStore(resultOfInsertValue, stream);
            builder.CreateBr(addSizeToStreamBlock);
            sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
        }
        builder.SetInsertPoint(addSizeToStreamBlock);
        llvm::Value *castStreamToInt = builder.CreatePtrToInt(i8Stream, llvm::IntegerType::get(llvm::getGlobalContext(), 64));
        llvm::Value *addSizeToStream = builder.CreateAdd(castStreamToInt, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, this->GetSize() / 8, false)));
        llvm::Value *addSizeToNbBytes = builder.CreateAdd(nbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, this->GetSize() % 8, false)));

        llvm::Value   *nbBytesSupTo8 = builder.CreateICmpUGT(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 7, false)), "nbBytesSupTo8");
	builder.CreateCondBr(nbBytesSupTo8, incStreamBlock, finalyzeBlock);
        builder.SetInsertPoint(incStreamBlock);
        llvm::Value   *add1ToStream = builder.CreateAdd(addSizeToStream, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 1, false)));
        llvm::Value   *add1ToNbBytes = builder.CreateSub(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 8, false)));
        llvm::Value   *castIntToStream = builder.CreateIntToPtr(add1ToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(add1ToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"));
        builder.CreateBr(newActionBlock);
        builder.SetInsertPoint(finalyzeBlock);
        castIntToStream = builder.CreateIntToPtr(addSizeToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(addSizeToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"));
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
        llvm::Value                     *param;
        llvm::Value                     *readFromStream;
        std::vector<const llvm::Type*>  listType;
        std::string                     nameCaseBlock;

        builder.SetInsertPoint(actionBlock);
        llvm::Value *i8Stream = builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"), "i8Stream");
        llvm::Value *nbBytes = builder.CreateLoad(actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"), "nbBytes");
        llvm::SwitchInst*   sw = builder.CreateSwitch(nbBytes, caseNotFoundBlock, 8);
        builder.SetInsertPoint(caseNotFoundBlock);
        builder.CreateBr(errorBlock);
        for (int i = 1; i < 8; ++i)
        {
            listType.clear();
            nameCaseBlock = "case";
            nameCaseBlock += ('0' + (char)i);
            caseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), nameCaseBlock.c_str(), actionBlock->getParent());
            builder.SetInsertPoint(caseBlock);
            listType.push_back(llvm::IntegerType::get(llvm::getGlobalContext(), i));
            listType.push_back(this->GetLLVMType());
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::StructType::get(llvm::getGlobalContext(), listType)), "stream");
            tmp = builder.CreateLoad(stream, "tmp");
            readFromStream = builder.CreateExtractValue(tmp, 1, "readFromStream");
            builder.CreateStore(readFromStream, actionBlock->getValueSymbolTable()->lookup(this->GetName() + "paramAdr"));
            builder.CreateBr(addSizeToStreamBlock);
            sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
        }
        builder.SetInsertPoint(addSizeToStreamBlock);
        llvm::Value *castStreamToInt = builder.CreatePtrToInt(i8Stream, llvm::IntegerType::get(llvm::getGlobalContext(), 64));
        llvm::Value *addSizeToStream = builder.CreateAdd(castStreamToInt, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, this->GetSize() / 8, false)));
        llvm::Value *addSizeToNbBytes = builder.CreateAdd(nbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, this->GetSize() % 8, false)));

        llvm::Value   *nbBytesSupTo8 = builder.CreateICmpUGT(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 7, false)), "nbBytesSupTo8");
	builder.CreateCondBr(nbBytesSupTo8, incStreamBlock, finalyzeBlock);
        builder.SetInsertPoint(incStreamBlock);
        llvm::Value   *add1ToStream = builder.CreateAdd(addSizeToStream, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 1, false)));
        llvm::Value   *add1ToNbBytes = builder.CreateSub(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 8, false)));
        llvm::Value   *castIntToStream = builder.CreateIntToPtr(add1ToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(add1ToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"));
        builder.CreateBr(newActionBlock);
        builder.SetInsertPoint(finalyzeBlock);
        castIntToStream = builder.CreateIntToPtr(addSizeToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
        builder.CreateStore(castIntToStream, actionBlock->getValueSymbolTable()->lookup("i8StreamAdr"));
        builder.CreateStore(addSizeToNbBytes, actionBlock->getParent()->getParent()->getValueSymbolTable().lookup("_nbBytes"));
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