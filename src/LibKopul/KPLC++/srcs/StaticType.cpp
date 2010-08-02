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

llvm::BasicBlock*	StaticType::BuildEncodingToMemory(llvm::BasicBlock *actionBlock) const
{
	llvm::IRBuilder<> builder(llvm::getGlobalContext());

	builder.SetInsertPoint(actionBlock);
	builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("paramAdr"), "param");
	builder.CreateStore(actionBlock->getValueSymbolTable()->lookup("param"), actionBlock->getValueSymbolTable()->lookup("stream"));
	//llvm::Value *tmp = builder.CreateAdd(encodeFunction->getValueSymbolTable().lookup("stream"), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "tmp");
	//builder.CreateStore(tmp, encodeFunction->getValueSymbolTable().lookup("streamAdr"));
	return (actionBlock);
}

llvm::BasicBlock*	StaticType::BuildDecodingFromMemory(llvm::BasicBlock *actionBlock) const
{
    	llvm::IRBuilder<> builder(llvm::getGlobalContext());

	builder.SetInsertPoint(actionBlock);
	builder.CreateLoad(actionBlock->getValueSymbolTable()->lookup("stream"), "buffer");
	builder.CreateStore(actionBlock->getValueSymbolTable()->lookup("buffer"), actionBlock->getValueSymbolTable()->lookup("paramAdr"));
	//llvm::Value *tmp = builder.CreateAdd(decodeFunction->getValueSymbolTable().lookup("stream"), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_size, 1, false)), "tmp");
	//builder.CreateStore(tmp, decodeFunction->getValueSymbolTable().lookup("streamAdr"));
	return (actionBlock);
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