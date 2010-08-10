/* 
 * File:   Register.cpp
 * Author: deveza
 * 
 * Created on August 4, 2010, 12:14 AM
 */

#include "Register.h"

using namespace kpl;

bool Register::_llvmIsInit;

Register::Register(MODE mode)
{
    _module = NULL;
    _moduleName = "Kopul";
    _mode = mode;
    if (!_llvmIsInit)
    {
	llvm::InitializeNativeTarget();
        _llvmIsInit = true;
    }

}

Register::Register(const Register&reg)
{
    _module = NULL;
    _moduleName = "Kopul";
    _mode = reg._mode;
    for (unsigned int i = 0; i < reg._listType.size(); ++i)
        this->Add(reg._listType[i]);
}

Register::~Register()
{
    this->Clear();
}

Register&   Register::operator = (const Register& reg)
{
    this->_moduleName = "Kopul";
    this->_mode = reg._mode;
    return (*this);
}

void    Register::Add(const Type& type)
{
    this->_listType.push_back(static_cast<Type *>(type.Clone()));
}

Register&    kpl::operator << (Register& reg, const Type& type)
{
    reg.Add(type);
    return (reg);
}

void    Register::Add(const Type* type)
{
    this->Add(*type);
}

Register&    kpl::operator << (Register& reg, const Type* type)
{
    reg.Add(*type);
    return (reg);
}

void    Register::Clear()
{
    for (unsigned int i = 0; i < this->_listType.size(); ++i)
        delete (this->_listType[i]);
    this->_listType.clear();
}

void    Register::Dump()
{
    if (this->_listType.size() == 0)
    {
        std::cout << "No type register to dump" << std::endl;
        return ;
    }
    this->_module = new llvm::Module(this->_moduleName.c_str(), llvm::getGlobalContext());
    llvm::GlobalVariable    *nbBytes = new llvm::GlobalVariable(llvm::IntegerType::get(llvm::getGlobalContext(), 8), false, llvm::GlobalVariable::InternalLinkage, llvm::ConstantInt::get(llvm::getGlobalContext(), llvm::APInt(8, 0, false)), "_nbBytes");
    this->_module->getGlobalList().push_back(nbBytes);
    for (unsigned int i = 0; i < this->_listType.size(); ++i)
        this->_listType[i]->Build(this->_module, this->_mode);
    this->_module->dump();
    delete (this->_module);
}

void    Register::Compile()
{
    //TODO
}

const std::string&  Register::GetRegisterName() const
{
    return (this->_moduleName);
}

void                Register::SetRegisterName(const std::string& moduleName)
{
    this->_moduleName = moduleName;
}