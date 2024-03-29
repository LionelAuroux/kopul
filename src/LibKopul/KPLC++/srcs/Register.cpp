/* 
 * File:   Register.cpp
 * Author: deveza
 * 
 * Created on August 4, 2010, 12:14 AM
 */

#include <sstream>

#include "Register.h"

using namespace kpl;

bool Register::_llvmIsInit;

Register::Register()
{
    _moduleName = "Kopul";
    if (!_llvmIsInit)
    {
	llvm::InitializeNativeTarget();
        _llvmIsInit = true;
    }

}

Register::Register(const Register&reg) : Container<Type>(reg)
{
    _moduleName = "Kopul";
}

Register::~Register()
{

}

llvm::Module                        *Register::BuildModule(MODE mode) const
{
    llvm::Module        *_module;
    std::ostringstream  oss;

    if (this->_list.size() == 0)
    {
        std::cout << "No type register" << std::endl;
        return (NULL);
    }
    _module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytesRead = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesRead");
    llvm::GlobalVariable    *nbBytesWrite = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesWrite");
    _module->getGlobalList().push_back(nbBytesRead);
    _module->getGlobalList().push_back(nbBytesWrite);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
    {
        oss << i;
        this->_list[i]->SetName("_type" + oss.str() + "_" + this->_list[i]->ToString());
        this->_list[i]->Build(_module, mode);
        oss.str("");
    }
    return (_module);
}

void    Register::Dump(MODE mode) const
{
    llvm::Module    *_module = this->BuildModule(mode);
    _module->dump();
    delete (_module);
}

FunctionList<int (*)(stream, ...)> *Register::CompileInMemoryMode() const
{
    llvm::Module    *_module = this->BuildModule(MEMORY_MODE);

    return (new FunctionList<int (*)(stream, ...)>(_module, this->_list));
}

FunctionList<int (*)(fd, ...)>     *Register::CompileInFileMode() const
{
    llvm::Module    *_module = this->BuildModule(FILE_MODE);

    return (new FunctionList<int (*)(fd, ...)>(_module, this->_list));
}

FunctionList<int (*)(socket, ...)> *Register::CompileInSocketMode() const
{
    llvm::Module    *_module = this->BuildModule(SOCKET_MODE);

    return (new FunctionList<int (*)(socket, ...)>(_module, this->_list));
}

const std::string&  Register::GetRegisterName() const
{
    return (this->_moduleName);
}

void                Register::SetRegisterName(const std::string& moduleName)
{
    this->_moduleName = moduleName;
}