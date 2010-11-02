/* 
 * File:   VariableIterator.h
 * Author: deveza
 *
 * Created on October 29, 2010, 12:29 AM
 */

#ifndef _VARIABLEITERATOR_H
# define _VARIABLEITERATOR_H
# include <vector>

# include "Variable.h"

namespace kpl
{
    class VariableIterator : public Variable
    {
    public:
        VariableIterator(const Variable &, void *value);
        VariableIterator(const VariableIterator& orig);
        ~VariableIterator();
        VariableIterator        &operator = (const VariableIterator& old);
        VariableIterator        operator[] (unsigned int i);
        void*                   operator *();
        llvm::Value             *GetLLVMValue(llvm::BasicBlock *) const;
        const std::vector<int>  GetDeepList() const;
        void                    SetDeepList(const std::vector<int>&);
        Value                   *Duplicate() const;
        template <typename T>
        T                       Get() const
        {
            return (*(T *)*(*const_cast<VariableIterator *>(this)));
        }

        template <typename T>
        void                    Set(T val)
        {
            *(T *)*(*const_cast<VariableIterator *>(this)) = val;
        }

    private:
        std::vector<int>    _deepList;
    };
}

#endif	/* _VARIABLEITERATOR_H */

