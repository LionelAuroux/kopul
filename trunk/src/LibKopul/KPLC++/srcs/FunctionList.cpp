#include "FunctionList.h"

using namespace kpl;

/*
 * FunctionPair
 */

template <typename T>
FunctionPair<T>::FunctionPair()
{
    encode = NULL;
    decode = NULL;
}

template <typename T>
FunctionPair<T>::FunctionPair(const T& _encode, const T& _decode) : encode(_encode), decode(_decode)
{

}

template <typename T>
FunctionPair<T>::FunctionPair(const FunctionPair<T>& oldPair)
{
    encode = oldPair.encode;
    decode = oldPair.decode;
}

template <typename T>
FunctionPair<T>::~FunctionPair()
{

}

template <typename T>
FunctionPair<T>&    FunctionPair<T>::operator = (const FunctionPair<T>& oldPair)
{
    encode = oldPair.encode;
    decode = oldPair.decode;
    return (*this);
}

template <typename T>
T                   FunctionPair<T>::operator [] (FUNCTIONTYPE type) const
{
    if (type == ENCODE)
        return (this->encode);
    if (type == DECODE)
        return (this->decode);
    return (NULL);
}

/*
 * FunctionList
 */

template <typename T>
FunctionList<T>::FunctionList(llvm::Module *module, const std::vector<Type *> &listType)
{
    llvm::ExecutionEngine   *exec = llvm::EngineBuilder(module).create();
    llvm::Module::iterator  itb = module->begin();
    llvm::Module::iterator  ite = module->end();
    void                    *pfencode;
    void                    *pfdecode;

    this->_module = module;
    for (unsigned int i = 0; itb != ite; ++itb, ++i)
    {
        pfencode = exec->recompileAndRelinkFunction(itb);
        ++itb;
        pfdecode = exec->recompileAndRelinkFunction(itb);
        this->_mapFunctions[static_cast<Type *>(listType[i]->Clone())] = FunctionPair<T>((T)pfencode, (T)pfdecode);
    }
}

template <typename T>
FunctionList<T>::~FunctionList()
{
    for (typename std::map<Type *, FunctionPair<T> >::iterator it = this->_mapFunctions.begin(); it != this->_mapFunctions.end(); ++it)
        delete (it->first);
    delete (this->_module);
}

template <typename T>
const FunctionPair<T>&   FunctionList<T>::operator [] (const Type *type) const
{
    for (typename std::map<Type *, FunctionPair<T> >::const_iterator it = this->_mapFunctions.begin(); it != this->_mapFunctions.end(); ++it)
    {
        if (it->first->Equals(*type))
            return (it->second);
    }
    return (this->_end);
}

template <typename T>
const FunctionPair<T>&   FunctionList<T>::operator [] (const Type &type) const
{
    for (typename std::map<Type *, FunctionPair<T> >::const_iterator it = this->_mapFunctions.begin(); it != this->_mapFunctions.end(); ++it)
    {
        if (it->first->Equals(type))
            return (it->second);
    }
    return (this->_end);
}

template <typename T>
const FunctionPair<T>&   FunctionList<T>::operator [] (const std::string &name) const
{
    for (typename std::map<Type *, FunctionPair<T> >::const_iterator it = this->_mapFunctions.begin(); it != this->_mapFunctions.end(); ++it)
    {
        if (it->first->GetName() == name)
            return (it->second);
    }
    return (this->_end);
}

template <typename T>
const FunctionPair<T>&   FunctionList<T>::operator [] (unsigned int i) const
{
    for (typename std::map<Type *, FunctionPair<T> >::const_iterator it = this->_mapFunctions.begin(); it != this->_mapFunctions.end(); ++it, --i)
    {
        if (i == 0)
            return (it->second);
    }
    return (this->_end);
}

template class FunctionList<int (*)(char **, ...)>;
template class FunctionList<int (*)(int, ...)>;
template class FunctionPair<int (*)(char **, ...)>;
template class FunctionPair<int (*)(int, ...)>;