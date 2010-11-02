#include "Variable.h"


#include "VariableIterator.h"
#include "IComposedType.h"

using namespace kpl;

Variable::Variable(const std::string &name, const Type &type, bool isLastBytesRead) : Value(type)
{
    _variableName = name;
    _isLastBytesRead = isLastBytesRead;
}

Variable::Variable(const Variable& var) : Value(var)
{
    _variableName = var._variableName;
    _isLastBytesRead = var._isLastBytesRead;
}

Variable::~Variable()
{

}

Variable&           Variable::operator = (const Variable &var)
{
    static_cast<Value&>(*this) = static_cast<const Value &>(var);
    this->_isLastBytesRead = var._isLastBytesRead;
    return (*this);
}

VariableIterator    Variable::operator [](unsigned int i)
{
    VariableIterator    var(*this, this->_value);
    std::vector<int>    deepList;
    IComposedType       *cType;

    if (!((cType = dynamic_cast<IComposedType *>(this->_type)) && cType->GetNbType() > (int)i))
    {
        throw std::exception();
    }
    deepList.push_back(i);
    var.SetDeepList(deepList);
    return (var);
}

void*               Variable::operator *()
{
    return (this->_value);
}

bool                Variable::IsLastBytesRead() const
{
    return (this->_isLastBytesRead);
}

void                Variable::SetVariableName(const std::string &variableName)
{
    this->_variableName = variableName;
}

const std::string   &Variable::GetVariableName() const
{
    return (this->_variableName);
}

llvm::Value         *Variable::GetLLVMValue(llvm::BasicBlock *actionBlock) const
{
    llvm::IRBuilder<>   builder(llvm::getGlobalContext());

    builder.SetInsertPoint(actionBlock);
    if (this->IsLastBytesRead())
    {
        return (builder.CreateLoad(this->_lastBytes));
    }
    return (actionBlock->getParent()->getValueSymbolTable().lookup(this->_variableName.c_str()));
}

void                Variable::SetLLVMValuePointerToLastBytesRead(llvm::Value *lastBytes)
{
    this->_lastBytes = lastBytes;
}

Value               *Variable::Duplicate() const
{
    return (new Variable(*this));
}

const std::string&  Variable::ToStr() const
{
    return (this->_variableName);
}