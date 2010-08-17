/* 
 * File:   FunctionsList.h
 * Author: deveza
 *
 * Created on August 11, 2010, 11:42 PM
 */

#ifndef _FUNCTIONLIST_H
# define _FUNCTIONLIST_H
# include <llvm/LLVMContext.h>
# include <llvm/Module.h>
# include <llvm/Support/IRBuilder.h>
# include <llvm/Analysis/Verifier.h>
# include <llvm/ValueSymbolTable.h>
# include <llvm/Target/TargetData.h>
# include <llvm/Target/TargetSelect.h>
# include <llvm/ExecutionEngine/ExecutionEngine.h>
# include <llvm/ExecutionEngine/Interpreter.h>
# include <llvm/ExecutionEngine/JIT.h>
# include <llvm/ExecutionEngine/JITEventListener.h>
# include <map>

# include "Type.h"

namespace kpl
{
    enum FUNCTIONTYPE
    {
        ENCODE,
        DECODE,
    };

    template <typename T>
    class FunctionPair
    {
    public:
        FunctionPair();
        FunctionPair(const T&, const T&);
        FunctionPair(const FunctionPair<T>&);
        ~FunctionPair();
        FunctionPair<T>&    operator = (const FunctionPair<T>&);
        T                   operator [] (FUNCTIONTYPE) const;

        T           encode;
        T           decode;
    private:

    };

    template <typename T>
    class FunctionList
    {
    public:
        friend class Register;

        ~FunctionList();
        const FunctionPair<T>&  operator [] (const Type *) const;
        const FunctionPair<T>&  operator [] (const Type &) const;
        const FunctionPair<T>&  operator [] (const std::string &) const;
        const FunctionPair<T>&  operator [] (unsigned int i) const;

        const FunctionPair<T>&  Get(const Type *) const;
        const FunctionPair<T>&  Get(const Type &) const;
        const FunctionPair<T>&  Get(const std::string &) const;
        const FunctionPair<T>&  Get(unsigned int i) const;

        int                     GetNbBytesWrite() const;
        int                     GetNbBytesRead() const;
    protected:
        FunctionList(llvm::Module *, const std::vector<Type *>&);
    private:
        std::map<Type *, FunctionPair<T> >  _mapFunctions;
        llvm::Module                        *_module;
        FunctionPair<T>                     _end;
        char                                *_nbBytesWriteAdr;
        char                                *_nbBytesReadAdr;
    };
};


#endif	/* _FUNCTIONLIST_H */

