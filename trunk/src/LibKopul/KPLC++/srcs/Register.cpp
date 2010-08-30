/* 
 * File:   Register.cpp
 * Author: deveza
 * 
 * Created on August 4, 2010, 12:14 AM
 */

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

Register&   Register::operator = (const Register&)
{
    this->_moduleName = "Kopul";
    return (*this);
}

void    Register::Dump(MODE mode) const
{
    llvm::Module    *_module;

    if (this->_list.size() == 0)
    {
        std::cout << "No type register to dump" << std::endl;
        return ;
    }
    _module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytesRead = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesRead");
    llvm::GlobalVariable    *nbBytesWrite = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesWrite");
    _module->getGlobalList().push_back(nbBytesRead);
    _module->getGlobalList().push_back(nbBytesWrite);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        this->_list[i]->Build(_module, mode);
    _module->dump();
    delete (_module);
}

FunctionList<int (*)(stream, ...)> *Register::CompileInMemoryMode() const
{
    llvm::Module    *_module;

    if (this->_list.size() == 0)
    {
        std::cout << "No type register to compile" << std::endl;
        return (NULL);
    }
    _module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytesRead = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesRead");
    llvm::GlobalVariable    *nbBytesWrite = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesWrite");
    _module->getGlobalList().push_back(nbBytesRead);
    _module->getGlobalList().push_back(nbBytesWrite);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        this->_list[i]->Build(_module, MEMORY_MODE);
    return (new FunctionList<int (*)(stream, ...)>(_module, this->_list));
}

FunctionList<int (*)(fd, ...)>     *Register::CompileInFileMode() const
{
    llvm::Module    *_module;

    if (this->_list.size() == 0)
    {
        std::cout << "No type register to compile" << std::endl;
        return (NULL);
    }
    _module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytesRead = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesRead");
    llvm::GlobalVariable    *nbBytesWrite = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesWrite");
    _module->getGlobalList().push_back(nbBytesRead);
    _module->getGlobalList().push_back(nbBytesWrite);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        this->_list[i]->Build(_module, FILE_MODE);
    return (new FunctionList<int (*)(fd, ...)>(_module, this->_list));
}

FunctionList<int (*)(socket, ...)> *Register::CompileInSocketMode() const
{
    llvm::Module    *_module;

    if (this->_list.size() == 0)
    {
        std::cout << "No type register to compile" << std::endl;
        return (NULL);
    }
    _module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytesRead = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesRead");
    llvm::GlobalVariable    *nbBytesWrite = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytesWrite");
    _module->getGlobalList().push_back(nbBytesRead);
    _module->getGlobalList().push_back(nbBytesWrite);
    for (unsigned int i = 0; i < this->_list.size(); ++i)
        this->_list[i]->Build(_module, SOCKET_MODE);
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