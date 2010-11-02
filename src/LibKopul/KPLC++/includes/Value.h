#ifndef __VALUE___H
# define __VALUE___H
# include <string>

# include "Type.h"

namespace kpl
{
    class   Value
    {
    public:
        Value(const Type &);
        Value(const Value &);
        Value&  operator = (const Value &);
        ~Value();
        virtual llvm::Value         *GetLLVMValue(llvm::BasicBlock *) const = 0;
        virtual Value               *Duplicate() const = 0;
        virtual const std::string   &ToStr() const = 0;
        const void *                GetValue() const;
        const Type&                 GetTypeOfValue() const;

    protected:

        void    CreateCmp(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock, llvm::Value *toCompare) const;
        void    *_value;
        Type    *_type;

    };
};

#endif //__VALUE___H
