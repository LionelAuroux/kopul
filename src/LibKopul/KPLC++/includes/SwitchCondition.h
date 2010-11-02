/* 
 * File:   SwitchCondition.h
 * Author: deveza
 *
 * Created on September 20, 2010, 3:50 PM
 */

#ifndef _SWITCHCONDITION_H
# define _SWITCHCONDITION_H
# include "ICondition.h"
# include "ConditionNode.h"
# include "Variable.h"

namespace kpl
{

    class       SwitchCondition : public ICondition
    {
    public:
        SwitchCondition(const ICondition &cond, const Type &type);
        SwitchCondition(const SwitchCondition&);
        ~SwitchCondition();
        SwitchCondition&    operator = (const SwitchCondition &);

        const Type&                     GetTypeAssociateWithCondition() const;
        const ICondition&               GetCondition() const;
        void                            BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const;
        std::vector<const Variable *>   GetVariables() const;
        const std::string&              ToString() const ;
        const std::string&              GetType() const;
        bool                            Equals(const IObject &value) const;
        IObject*                        Clone() const;
    protected:

    private:
        Type            *_type;
        ICondition      *_cond;
        std::string     _objectToStr;
        std::string     _objectType;
    };
};

#endif	/* _SWITCHCONDITION_H */

