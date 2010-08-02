#include <iostream>
#include <stdexcept>

#include "Type.h"
#include "Bitfield.h"

using namespace kpl;

Type::Type()
{
    _type = NULL;
    _modeMap[MEMORY_MODE] = std::pair<BUILDER_MODE, BUILDER_MODE>(ENCODE_TO_MEMORY, DECODE_FROM_MEMORY);
    _modeMap[FILE_MODE] = std::pair<BUILDER_MODE, BUILDER_MODE>(ENCODE_TO_FILE, DECODE_FROM_FILE);
    _modeMap[SOCKET_MODE] = std::pair<BUILDER_MODE, BUILDER_MODE>(ENCODE_TO_SOCKET, DECODE_FROM_SOCKET);
    _codeBuilderMap[ENCODE_TO_MEMORY] = &Type::BuildEncodingToMemory;
    _codeBuilderMap[DECODE_FROM_MEMORY] = &Type::BuildDecodingFromMemory;
    _codeBuilderMap[ENCODE_TO_FILE] = &Type::BuildEncodingToFile;
    _codeBuilderMap[DECODE_FROM_FILE] = &Type::BuildDecodingFromFile;
    _codeBuilderMap[ENCODE_TO_SOCKET] = &Type::BuildEncodingToSocket;
    _codeBuilderMap[DECODE_FROM_SOCKET] = &Type::BuildDecodingFromSocket;
    _functionBuilderMap[MEMORY_MODE] = &Type::CreateFunctionForMemory;
    _functionBuilderMap[FILE_MODE] = &Type::CreateFunctionForFile;
    _functionBuilderMap[SOCKET_MODE] = &Type::CreateFunctionForSocket;
}

Type::~Type()
{

}

const std::string&		Type::GetName() const
{
    return (this->_name);
}

void				Type::SetName(const std::string& name)
{
	this->_name = name;
}

const llvm::Type*               Type::GetLLVMType() const
{
    return (this->_type);
}

llvm::Function*			Type::CreateFunctionForMemory(llvm::Module *module, const std::string &name) const
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
    llvm::Function *newFunction = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name.c_str(), module);

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

llvm::Function*			Type::CreateFunctionForFile(llvm::Module *module, const std::string &name) const
{
/*
 * TODO
 */
    return (NULL);
}

llvm::Function*			Type::CreateFunctionForSocket(llvm::Module *module, const std::string &name) const
{
/*
 * TODO
 */
    return (NULL);
}

// Fontion qui insert une fonction dans le module et la retourne
llvm::Function*			Type::CreateFunction(llvm::Module* module, const std::string& name, MODE mode) const
{	
    if (this->_functionBuilderMap.find(mode) == this->_functionBuilderMap.end())
        throw (std::logic_error("Unknown Mode"));
    return ((this->*this->_functionBuilderMap.find(mode)->second)(module, name));
}

llvm::BasicBlock*               Type::BuildCode(llvm::BasicBlock *actionBlock, BUILDER_MODE builder_mode) const
{
    if (this->_codeBuilderMap.find(builder_mode) == this->_codeBuilderMap.end())
        throw (std::logic_error("Unknown Builder Mode"));
    return ((this->*this->_codeBuilderMap.find(builder_mode)->second)(actionBlock));
   
}

bool			Type::BuildFunctions(llvm::Module *module, MODE mode)
{
    llvm::Function*	encodeFunction = this->CreateFunction(module, std::string("__kpl__" + this->_name + "__encode__"), mode);
    llvm::Function*	decodeFunction = this->CreateFunction(module, std::string("__kpl__" + this->_name + "__decode__"), mode);

    if (this->_modeMap.find(mode) == this->_modeMap.end())
        throw (std::logic_error("Unknown Mode"));
    this->BuildCode(++encodeFunction->begin(), this->_modeMap[mode].first);
    this->BuildCode(++decodeFunction->begin(), this->_modeMap[mode].second);

    llvm::IRBuilder<> builder(llvm::getGlobalContext());
    builder.SetInsertPoint(++encodeFunction->begin());
    builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
    builder.SetInsertPoint(++decodeFunction->begin());
    builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
    return (llvm::verifyFunction(*encodeFunction) && llvm::verifyFunction(*decodeFunction));
}

