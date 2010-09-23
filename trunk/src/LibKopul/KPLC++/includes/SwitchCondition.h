/* 
 * File:   SwitchCondition.h
 * Author: deveza
 *
 * Created on September 20, 2010, 3:50 PM
 */

#ifndef _SWITCHCONDITION_H
#define _SWITCHCONDITION_H
# include "ConditionNode.h"
# include "Variable.h"

namespace kpl
{

    class       SwitchCondition : public IObject
    {
    public:
        SwitchCondition(const ConditionNode &cond, const Type &type);
        SwitchCondition(const SwitchCondition&);
        ~SwitchCondition();
        SwitchCondition&    operator = (const SwitchCondition &);

        std::vector<const Variable *>   GetVariables() const;
        const std::string&              ToString() const ;
        const std::string&              GetType() const;
        bool                            Equals(const IObject &value) const;
        IObject*                        Clone() const;
    protected:

    private:
        Type            *_type;
        ConditionNode   *_cond;
        std::string     _objectToStr;
        std::string     _objectType;
    };
};

#endif	/* _SWITCHCONDITION_H */

