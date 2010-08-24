#include <iostream>
#include <stdexcept>

#include "Type.h"
#include "Bitfield.h"

using namespace kpl;

Type::Type()
{
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

const std::string&              Type::GetEncodeFunctionName() const
{
    return (this->_encodeFunctionName);
}

const std::string&              Type::GetDecodeFunctionName() const
{
    return (this->_decodeFunctionName);
}

const std::string&		Type::GetName() const
{
    return (this->_name);
}

void				Type::SetName(const std::string& name)
{
    this->_name = name;
    this->_encodeFunctionName = "__kpl__" + this->_name + "__encode__";
    this->_decodeFunctionName = "__kpl__" + this->_name + "__decode__";
}

llvm::Function*			Type::CreateFunctionForMemory(llvm::Module *module, const std::string &name, const std::map<std::string, const llvm::Type*>& mapVariable) const
{
    std::vector<const llvm::Type*>	listParamProto;

    // Generation d'un pointeur de pointeur sur la stream (i8**)
    llvm::PointerType *ptr_stream = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext()));
    llvm::PointerType *ptr_ptr_stream = llvm::PointerType::getUnqual(ptr_stream);
    listParamProto.push_back(ptr_ptr_stream);

    for (std::map<std::string, const llvm::Type*>::const_iterator it = mapVariable.begin(); it != mapVariable.end(); ++it)
    	listParamProto.push_back(it->second);

    // cree un fonction type du prototype
    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()), listParamProto, false);

    // creation de la fonction dans le module
    llvm::Function *newFunction = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name.c_str(), module);
    // donne un nom au premiere argument qui est la stream et au deuxieme qui est le param.
    llvm::Function::arg_iterator	alist = newFunction->arg_begin();
    alist->setName("i8StreamAdr");
    ++alist;
    for (std::map<std::string, const llvm::Type*>::const_iterator it = mapVariable.begin(); it != mapVariable.end(); ++it, ++alist)
    	alist->setName(it->first);


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
//    builder.CreateBitCast(newFunction->getValueSymbolTable().lookup("i8StreamAdr"), llvm::PointerType::getUnqual(ptr_stream), "streamAdr");
    builder.CreateLoad(newFunction->getValueSymbolTable().lookup("i8StreamAdr"), "streamAdrBackUp");
    builder.CreateLoad(newFunction->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"), "nbBytesWriteBackUp");
    builder.CreateLoad(newFunction->getParent()->getValueSymbolTable().lookup("_nbBytesRead"), "nbBytesReadBackUp");
    builder.CreateBr(actionBlock);
    builder.SetInsertPoint(errorBlock);
    builder.CreateStore(newFunction->getValueSymbolTable().lookup("streamAdrBackUp"), newFunction->getValueSymbolTable().lookup("i8StreamAdr"));
    builder.CreateStore(newFunction->getValueSymbolTable().lookup("nbBytesWriteBackUp"), newFunction->getParent()->getValueSymbolTable().lookup("_nbBytesWrite"));
    builder.CreateStore(newFunction->getValueSymbolTable().lookup("nbBytesReadBackUp"), newFunction->getParent()->getValueSymbolTable().lookup("_nbBytesRead"));
    builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, -1, false)));
    return (newFunction);
}

llvm::Function*			Type::CreateFunctionForFile(llvm::Module *module, const std::string &name, const std::map<std::string, const llvm::Type*>& mapVariable) const
{
/*
 * TODO
 */
    return (NULL);
}

llvm::Function*			Type::CreateFunctionForSocket(llvm::Module *module, const std::string &name, const std::map<std::string, const llvm::Type*>& mapVariable) const
{
/*
 * TODO
 */
    return (NULL);
}

// Fontion qui insert une fonction dans le module et la retourne
llvm::Function*			Type::CreateFunction(llvm::Module* module, const std::string& name, const std::map<std::string, const llvm::Type*>& mapVariable, MODE mode) const
{	
    if (this->_functionBuilderMap.find(mode) == this->_functionBuilderMap.end())
        throw (std::logic_error("Unknown Mode"));
    return ((this->*this->_functionBuilderMap.find(mode)->second)(module, name, mapVariable));
}

llvm::BasicBlock*               Type::BuildCode(llvm::BasicBlock *actionBlock, BUILDER_MODE builder_mode) const
{
    if (this->_codeBuilderMap.find(builder_mode) == this->_codeBuilderMap.end())
        throw (std::logic_error("Unknown Builder Mode"));
    return ((this->*this->_codeBuilderMap.find(builder_mode)->second)(actionBlock));
   
}

bool			Type::BuildFunctions(llvm::Module *module,const std::map<std::string, const llvm::Type*>& mapVariable, MODE mode) const
{
    llvm::Function*	encodeFunction = this->CreateFunction(module, this->GetEncodeFunctionName(), mapVariable, mode);
    llvm::Function*	decodeFunction = this->CreateFunction(module, this->GetDecodeFunctionName(), mapVariable, mode);

    if (this->_modeMap.find(mode) == this->_modeMap.end())
        throw (std::logic_error("Unknow mode"));
    llvm::BasicBlock    *encodeActionBlockEnd = this->BuildCode(++encodeFunction->begin(), this->_modeMap.find(mode)->second.first);
    llvm::BasicBlock    *decodeActionBlockEnd = this->BuildCode(++decodeFunction->begin(), this->_modeMap.find(mode)->second.second);

    llvm::IRBuilder<> builder(llvm::getGlobalContext());
    builder.SetInsertPoint(encodeActionBlockEnd);
    builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
    builder.SetInsertPoint(decodeActionBlockEnd);
    builder.CreateRet(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(32, 0, false)));
    return (llvm::verifyFunction(*encodeFunction) && llvm::verifyFunction(*decodeFunction));
}

