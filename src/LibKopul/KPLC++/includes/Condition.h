/* 
 * File:   Condition.h
 * Author: deveza
 *
 * Created on September 1, 2010, 3:38 AM
 */

#ifndef _CONDITION_H
# define _CONDITION_H
# include <vector>
# include <string>

# include "ICondition.h"
# include "Value.h"
# include "IObject.h"
# include "Variable.h"

typedef llvm::Value* (llvm::IRBuilder<>::*cmpMptr)(llvm::Value *, llvm::Value *, const llvm::Twine &);

namespace kpl
{

    class Condition : public ICondition
    {
    public:
        Condition(cmpMptr, const Value &, const Value &, const std::string & = "");
        Condition(const Condition& orig);
        ~Condition();
        Condition           &operator = (const Condition &);

        std::vector<const Variable *> GetVariables() const;
        // Get a string representation of the object
        const std::string&      ToString() const;

        // Get the type of the object
        const std::string&      GetType() const;

        // returns true if the given type and content are equal.
        bool                    Equals(const IObject &value) const;

        // create a new instance by making a deep copy of the current object data
        IObject*                Clone() const;
    protected:
        void                    BuildCondition(llvm::BasicBlock *actionBlock, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock) const;

    private:
        cmpMptr     _fptr;
        Value       *_LHS;
        Value       *_RHS;
        std::string _objectToStr;
        std::string _objectType;
    };

    Condition   operator == (const Value &, const Value &);
    Condition   operator != (const Value &, const Value &);
    Condition   operator <= (const Value &, const Value &);
    Condition   operator >= (const Value &, const Value &);
    Condition   operator < (const Value &, const Value &);
    Condition   operator > (const Value &, const Value &);
};

#endif	/* _CONDITION_H */

