#include <iostream>
#include <stdexcept>

#include "Type.h"

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

llvm::BasicBlock*               Type::CreateAddSizeToStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, int sizeInBytes, llvm::BasicBlock *whereToBuild) const
{
    llvm::IRBuilder<>               builder(llvm::getGlobalContext());
    llvm::BasicBlock                *finalyzeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "finalyze", whereToBuild->getParent());
    llvm::BasicBlock                *addSizeToStreamBlock = whereToBuild;
    llvm::BasicBlock                *incStreamBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "incStream", whereToBuild->getParent());
    llvm::BasicBlock                *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "action", whereToBuild->getParent());

    builder.SetInsertPoint(addSizeToStreamBlock);
    llvm::Value *stream = builder.CreateLoad(streamAdr, "Stream");
    llvm::Value *nbBytes = builder.CreateLoad(nbBytesAdr, "nbBytes");
    llvm::Value *castStreamToInt = builder.CreatePtrToInt(stream, llvm::IntegerType::get(llvm::getGlobalContext(), 64));
    llvm::Value *addSizeToStream = builder.CreateAdd(castStreamToInt, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, sizeInBytes / 8, false)));
    llvm::Value *addSizeToNbBytes = builder.CreateAdd(nbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, sizeInBytes % 8, false)));

    llvm::Value   *nbBytesSupTo8 = builder.CreateICmpSGT(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 7, false)), "nbBytesSupTo8");
    builder.CreateCondBr(nbBytesSupTo8, incStreamBlock, finalyzeBlock);
    builder.SetInsertPoint(incStreamBlock);
    llvm::Value   *add1ToStream = builder.CreateAdd(addSizeToStream, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(64, 1, false)));
    llvm::Value   *add1ToNbBytes = builder.CreateSub(addSizeToNbBytes, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 8, false)));
    llvm::Value   *castIntToStream = builder.CreateIntToPtr(add1ToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
    builder.CreateStore(castIntToStream, streamAdr);
    builder.CreateStore(add1ToNbBytes, nbBytesAdr);
    builder.CreateBr(newActionBlock);
    builder.SetInsertPoint(finalyzeBlock);
    castIntToStream = builder.CreateIntToPtr(addSizeToStream, llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(llvm::getGlobalContext())), "newStream");
    builder.CreateStore(castIntToStream, streamAdr);
    builder.CreateStore(addSizeToNbBytes, nbBytesAdr);
    builder.CreateBr(newActionBlock);
    return (newActionBlock);
}

llvm::BasicBlock*               Type::CreateStoreParamToStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, int sizeParamInBytes, llvm::BasicBlock *whereToBuild, const std::string &newBlockName) const
{
    llvm::IRBuilder<>               builder(llvm::getGlobalContext());
    // iterator pour les basickBlocks, ont veut recuperer le 3eme (error).
    llvm::Function::iterator        bIterator = whereToBuild->getParent()->begin();++bIterator;++bIterator;
    llvm::BasicBlock                *errorBlock = bIterator;
    llvm::BasicBlock                *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), newBlockName.c_str(), whereToBuild->getParent());
    llvm::BasicBlock                *caseNotFoundBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "caseNotFound", whereToBuild->getParent());
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

    builder.SetInsertPoint(whereToBuild);
    llvm::Value *i8Stream = builder.CreateLoad(streamAdr, "i8Stream");
    llvm::Value *nbBytes = builder.CreateLoad(nbBytesAdr, "nbBytes");
    llvm::SwitchInst*   sw = builder.CreateSwitch(nbBytes, caseNotFoundBlock, 8);
    builder.SetInsertPoint(caseNotFoundBlock);
    builder.CreateBr(errorBlock);
    for (int i = 0; i < 8; ++i)
    {
        listType.clear();
        nameCaseBlock = "case";
        nameCaseBlock += ('0' + (char)i);
        caseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), nameCaseBlock.c_str(), whereToBuild->getParent());
        builder.SetInsertPoint(caseBlock);
        if (i == 0)
        {
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8)), "stream");
            param = builder.CreateLoad(paramAdr, "param");
            builder.CreateStore(param, stream);
            builder.CreateBr(newActionBlock);
        }
        else
        {
            // On recupere les bytes a cheval puis on bitcast pour obtenir le type definitif storable dans le flux
            streamBytes = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i)), "streamBytes");
            tmp = builder.CreateLoad(streamBytes, "tmp");
//                bytesBitCasted = builder.CreateBitCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), i + this->GetSize()), "bytesBitCasted");
            bytesBitCasted = builder.CreateIntCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), i + sizeParamInBytes), false, "bytesCasted");


            // Load le parametre a ecrire dans le flux puis on bitcast pour obtenir le type definitif storable dans le flux
            param = builder.CreateLoad(paramAdr, "param");
            paramBitCasted = builder.CreateIntCast(param, llvm::IntegerType::get(llvm::getGlobalContext(), i + sizeParamInBytes), false, "paramCasted");

            // Calcul du mask
            // TODO gestion du little et Big Endian
            tmpMask = builder.CreateMul(paramBitCasted, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(i + sizeParamInBytes, pow(2, i), false)), "tmpMask");
            tmpMask = builder.CreateAdd(tmpMask, bytesBitCasted, "toStore");

            // Store le resultat du calcul du mask
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i + sizeParamInBytes)), "stream");
            builder.CreateStore(tmpMask, stream);
            builder.CreateBr(newActionBlock);
        }
        sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
    }
    return (newActionBlock);
}

llvm::BasicBlock*               Type::CreateLoadParamFromStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, int sizeParamInBytes, llvm::BasicBlock *whereToBuild, const std::string &newBlockName) const
{
    llvm::IRBuilder<>               builder(llvm::getGlobalContext());
    llvm::Function::iterator        bIterator = whereToBuild->getParent()->begin();++bIterator;++bIterator;
    llvm::BasicBlock                *errorBlock = bIterator;
    llvm::BasicBlock                *newActionBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), newBlockName.c_str(), whereToBuild->getParent());
    llvm::BasicBlock                *caseNotFoundBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "caseNotFound", whereToBuild->getParent());
    llvm::BasicBlock                *caseBlock;
    llvm::Value                     *stream;
    llvm::Value                     *tmp;
    llvm::Value                     *readFromStream;
    std::vector<const llvm::Type*>  listType;
    std::string                     nameCaseBlock;

    builder.SetInsertPoint(whereToBuild);
    llvm::Value *i8Stream = builder.CreateLoad(streamAdr, "Stream");
    llvm::Value *nbBytes = builder.CreateLoad(nbBytesAdr, "nbBytes");
    llvm::SwitchInst*   sw = builder.CreateSwitch(nbBytes, caseNotFoundBlock, 8);
    builder.SetInsertPoint(caseNotFoundBlock);
    builder.CreateBr(errorBlock);
    for (int i = 0; i < 8; ++i)
    {
        listType.clear();
        nameCaseBlock = "case";
        nameCaseBlock += ('0' + (char)i);
        caseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), nameCaseBlock.c_str(), whereToBuild->getParent());
        builder.SetInsertPoint(caseBlock);
        if (i == 0)
        {
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8)), "stream");
            tmp = builder.CreateLoad(stream, "tmp");
            tmp = builder.CreateMul(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8, pow(2, (CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8) - sizeParamInBytes), false)), "tmp");
            readFromStream = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8, pow(2, (CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8) - sizeParamInBytes), false)), "tmp");
            builder.CreateStore(readFromStream, paramAdr);
            builder.CreateBr(newActionBlock);
        }
        else
        {
            stream = builder.CreateBitCast(i8Stream, llvm::PointerType::getUnqual(llvm::IntegerType::get(llvm::getGlobalContext(), i + sizeParamInBytes)), "streamBytes");
            tmp = builder.CreateLoad(stream);
            tmp = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(i + sizeParamInBytes, pow(2, i), false)), "resDiv");
            tmp = builder.CreateIntCast(tmp, llvm::IntegerType::get(llvm::getGlobalContext(), CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8), false, "resCast");
            tmp = builder.CreateMul(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8, pow(2, (CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8) - sizeParamInBytes), false)), "tmp");
            tmp = builder.CreateUDiv(tmp, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8, pow(2, (CONVERT_NBBYTES_TO_NBOCTET(sizeParamInBytes) * 8) - sizeParamInBytes), false)), "tmp");
            builder.CreateStore(tmp, paramAdr);
            builder.CreateBr(newActionBlock);
        }
        sw->addCase(llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, i, false)), caseBlock);
    }
    return (newActionBlock);
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

