/* 
 * File:   ConditionNode.h
 * Author: deveza
 *
 * Created on September 1, 2010, 3:39 AM
 */

#ifndef _CONDITIONNODE_H
# define _CONDITIONNODE_H
# include <vector>
# include "Condition.h"
# include "IObject.h"

# define CONVERT_OP_TO_STRING(x) ((x == 1) ? "&&" : (x == 2) ? "||" : "")

namespace kpl
{
    enum    OPERAND
    {
        NOP,
        AND,
        OR,
    };

    class ConditionNode : public IObject
    {
    public:
        ConditionNode(const ConditionNode *, const ConditionNode *, const Condition *, OPERAND);
        ConditionNode(const ConditionNode& orig);
        virtual ~ConditionNode();
        ConditionNode   &operator = (const ConditionNode &);

        std::vector<const Variable *> GetVariables() const;

        // Get a string representation of the object
        const std::string&  ToString() const;

        // Get the type of the object
        const std::string&  GetType() const;

        // returns true if the given type and content are equal.
        bool                Equals(const IObject &value) const;

        // create a new instance by making a deep copy of the current object data
        IObject*            Clone() const;
    private:
        ConditionNode   *_left;
        ConditionNode   *_right;
        Condition       *_cond;
        OPERAND         _op;
        std::string     _objectToStr;
        std::string     _objectType;
    };
/*
    ConditionNode   &operator || (ConditionNode &, ConditionNode &);
    ConditionNode   &operator && (ConditionNode &, ConditionNode &);
    ConditionNode   &operator || (Condition &, ConditionNode &);
    ConditionNode   &operator && (Condition &, ConditionNode &);
    ConditionNode   &operator || (ConditionNode &, Condition &);
    ConditionNode   &operator && (ConditionNode &, Condition &);
    ConditionNode   &operator || (Condition &, Condition &);
    ConditionNode   &operator && (Condition &, Condition &);
 */
    ConditionNode   operator || (const ConditionNode &, const ConditionNode &);
    ConditionNode   operator && (const ConditionNode &, const ConditionNode &);
    ConditionNode   operator || (const Condition &, const ConditionNode &);
    ConditionNode   operator && (const Condition &, const ConditionNode &);
    ConditionNode   operator || (const ConditionNode &, const Condition &);
    ConditionNode   operator && (const ConditionNode &, const Condition &);
    ConditionNode   operator || (const Condition &, const Condition &);
    ConditionNode   operator && (const Condition &, const Condition &);
};
#endif	/* _CONDITIONNODE_H */

