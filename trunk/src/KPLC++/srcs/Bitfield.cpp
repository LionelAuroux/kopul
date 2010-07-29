#include "Bitfield.h"

using namespace kpl;

Bitfield::Bitfield(int size)
{
	_size = size;
	_type = llvm::IntegerType::get(llvm::getGlobalContext(), size);
}

int		Bitfield::getSize() const
{
	return (this->_size);
}

void		Bitfield::setSize(int size)
{
	this->_size = size;
	_type = llvm::IntegerType::get(llvm::getGlobalContext(), size);
}

int		Bitfield::buildEncodeFunction(llvm::Function *encodeFunction)
{
	llvm::IRBuilder<> builder(llvm::getGlobalContext());
	llvm::BasicBlock* actionBlock = ++encodeFunction->begin();

	builder.SetInsertPoint(actionBlock);
	builder.CreateLoad(encodeFunction->getValueSymbolTable().lookup("paramAdr"), "param");
	builder.CreateStore(encodeFunction->getValueSymbolTable().lookup("param"), encodeFunction->getValueSymbolTable().lookup("stream"));
	//llvm::Value *tmp = builder.CreateAdd(encodeFunction->getValueSymbolTable().lookup("stream"), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 1, false)), "tmp");
	//builder.CreateStore(tmp, encodeFunction->getValueSymbolTable().lookup("streamAdr"));
	return (0);
}

int		Bitfield::buildDecodeFunction(llvm::Function *decodeFunction)
{
	llvm::IRBuilder<> builder(llvm::getGlobalContext());
	llvm::BasicBlock* actionBlock = ++decodeFunction->begin();

	builder.SetInsertPoint(actionBlock);
	builder.CreateLoad(decodeFunction->getValueSymbolTable().lookup("stream"), "buffer");
	builder.CreateStore(decodeFunction->getValueSymbolTable().lookup("buffer"), decodeFunction->getValueSymbolTable().lookup("paramAdr"));
	//llvm::Value *tmp = builder.CreateAdd(decodeFunction->getValueSymbolTable().lookup("stream"), llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(this->_size, 1, false)), "tmp");
	//builder.CreateStore(tmp, decodeFunction->getValueSymbolTable().lookup("streamAdr"));
	return (0);
}
