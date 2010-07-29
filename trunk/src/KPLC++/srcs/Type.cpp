#include <iostream>
#include <stdexcept>

#include "Type.h"
#include "Bitfield.h"

using namespace kpl;

Type::Type()
{
	_type = NULL;
}

// Fontion qui insert une fonction dans le module et la retourne
llvm::Function*			Type::createFunction(llvm::Module* mod, const std::string& name) const
{	
	std::vector<const llvm::Type*>	listParamProto;
	
	if (this->_type == NULL)
		throw (std::logic_error("Type not initialized"));
	// Generation d'un pointeur de pointeur sur le type
	llvm::PointerType *ptr_type = llvm::PointerType::getUnqual(this->_type);
	llvm::PointerType *ptr_ptr_type = llvm::PointerType::getUnqual(ptr_type);
	listParamProto.push_back(ptr_ptr_type);
	listParamProto.push_back(ptr_type);
	for (std::vector<const llvm::Type*>::const_iterator it = this->_listVariable.begin(); it != this->_listVariable.end(); it++)
		listParamProto.push_back(*it);
  	// cree un fonction type du prototype
  	llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), listParamProto, false);

	// creation de la fonction dans le module  
	llvm::Function *newFunction = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name.c_str(), mod);
  
	// donne un nom au premiere argument qui est la stream et au deuxieme qui est le param.
  	llvm::Function::arg_iterator	alist = newFunction->arg_begin();
  	alist->setName("streamAdr");
	alist++;
	alist->setName("paramAdr");

  	/* 
		creation: du block d'entree (entry) de la function, 
			  du block ou sera mis le code d'encode ou de decode (action)
			  du block d'erreur appelle si l'encode ou le decode failed (error)
	*/
	llvm::IRBuilder<> builder(llvm::getGlobalContext());
  	llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", newFunction);
 	llvm::BasicBlock *actionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", newFunction); 
  	llvm::BasicBlock *errorBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "error", newFunction);
	builder.SetInsertPoint(entryBlock);
	builder.CreateLoad(newFunction->getValueSymbolTable().lookup("streamAdr"), "stream");
	builder.CreateLoad(newFunction->getValueSymbolTable().lookup("streamAdr"), "streamAdrBackUp");
 	builder.CreateBr(actionBlock);
	builder.SetInsertPoint(errorBlock);
	builder.CreateStore(newFunction->getValueSymbolTable().lookup("streamAdrBackUp"), newFunction->getValueSymbolTable().lookup("streamAdr"));
 	builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, -1, false)));
  	return (newFunction);
}

const std::string&		Type::getName() const
{
	return (this->_name);
}

void				Type::setName(const std::string& name)
{
	this->_name = name;
}

bool			Type::buildFunctions(llvm::Module *mod)
{
	llvm::Function*	encodeFunction = this->createFunction(mod, std::string("__kpl__" + this->_name + "__encode__"));
	llvm::Function*	decodeFunction = this->createFunction(mod, std::string("__kpl__" + this->_name + "__decode__"));

	this->buildEncodeFunction(encodeFunction);
	this->buildDecodeFunction(decodeFunction);

	llvm::IRBuilder<> builder(llvm::getGlobalContext());
	builder.SetInsertPoint(++encodeFunction->begin());
	builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
	builder.SetInsertPoint(++decodeFunction->begin());
 	builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
	return (llvm::verifyFunction(*encodeFunction) && llvm::verifyFunction(*decodeFunction));
}

