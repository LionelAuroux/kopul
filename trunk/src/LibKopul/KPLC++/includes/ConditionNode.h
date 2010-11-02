/* 
 * File:   ConditionNode.h
 * Author: deveza
 *
 * Created on September 1, 2010, 3:39 AM
 */

#ifndef _CONDITIONNODE_H
# define _CONDITIONNODE_H
# include <vector>
# include "ICondition.h"
# include "Condition.h"
# include "IObject.h"

# define CONVERT_OP_TO_STRING(x) ((x == 1) ? "_AND_" : (x == 2) ? "_OR_" : "")

namespace kpl
{
    enum    OPERAND
    {
        NOP,
        AND,
        OR,
    };

    class ConditionNode : public ICondition
    {
    public:
        ConditionNode(const ICondition &, const ICondition &, OPERAND);
        ConditionNode(const ConditionNode& orig);
        virtual ~ConditionNode();
        ConditionNode   &operator = (const ConditionNode &);

        std::vector<const Variable *> GetVariables() const;
        void                BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const;

        // Get a string representation of the object
        const std::string&  ToString() const;

        // Get the type of the object
        const std::string&  GetType() const;

        // returns true if the given type and content are equal.
        bool                Equals(const IObject &value) const;

        // create a new instance by making a deep copy of the current object data
        IObject*            Clone() const;
    private:
        ICondition      *_left;
        ICondition      *_right;
        OPERAND         _op;
        std::string     _objectToStr;
        std::string     _objectType;
    };

    ConditionNode   operator || (const ICondition &, const ICondition &);
    ConditionNode   operator && (const ICondition &, const ICondition &);
};
#endif	/* _CONDITIONNODE_H */

