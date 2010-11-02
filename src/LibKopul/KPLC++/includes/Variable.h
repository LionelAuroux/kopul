#ifndef __VARIABLE___H
#define __VARIABLE___H
#include <vector>
#include <string>

#include "StaticType.h"
#include "Value.h"

namespace kpl {

    class VariableIterator;
    
    class Variable : public Value {
    public:
        Variable(const std::string &name, const Type &, bool isLastBytesRead = false);
        Variable(const Variable&);
        ~Variable();

        Variable &                  operator =(const Variable &);
        virtual void*               operator *();
        virtual VariableIterator    operator [](unsigned int i);

        bool                        IsLastBytesRead() const;

        void                        SetVariableName(const std::string &variableName);
        const std::string           &GetVariableName() const;

        virtual llvm::Value         *GetLLVMValue(llvm::BasicBlock *) const;
        void                        SetLLVMValuePointerToLastBytesRead(llvm::Value *);

        virtual Value               *Duplicate() const;
        const std::string           &ToStr() const;

        template <typename T>
        T                           Get() const
        {
            return (*(T *)this->_value);
        }

        template <typename T>
        void                        Set(T val)
        {
            *(T *)this->_value = val;
        }

    protected:
        llvm::Value         *_lastBytes;
        bool                _isLastBytesRead;
        std::string         _variableName;
    private:
    };
};

#endif //__VARIABLE___H
