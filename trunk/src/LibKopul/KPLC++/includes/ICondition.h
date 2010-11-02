/* 
 * File:   ICondition.h
 * Author: deveza
 *
 * Created on September 30, 2010, 10:50 AM
 */

#ifndef _ICONDITION_H
# define _ICONDITION_H
# include <vector>
# include "Variable.h"
# include "IObject.h"

namespace kpl
{
    class   ICondition : public IObject
    {
    public:
        virtual         ~ICondition(){}
        virtual void    BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const = 0;
        virtual std::vector<const Variable *>   GetVariables() const = 0;
    };
};

#endif	/* _ICONDITION_H */

