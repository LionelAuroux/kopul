#ifndef __SWITCH___H
# define __SWITCH___H
# include <vector>
# include <map>

# include "Type.h"
# include "Container.h"
# include "SwitchCondition.h"
# include "Variable.h"

namespace kpl
{

    class	Switch : public Type, public Container<SwitchCondition>
    {
    public:
        Switch();
        Switch(const Switch &);
        ~Switch();
        Switch& operator = (const Switch &);
        const std::string&  ToString() const ;
        const std::string&  GetType() const;
        bool                Equals(const IObject &value) const;
        IObject*            Clone() const;

    protected:
        bool                Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*   BuildEncodingToMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*   BuildDecodingFromMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*   BuildEncodingToFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*   BuildDecodingFromFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*   BuildEncodingToSocket(llvm::BasicBlock *) const;
        llvm::BasicBlock*   BuildDecodingFromSocket(llvm::BasicBlock *) const;
    private:
        std::string     *_objectToStr;
        std::string     _objectType;
    };
/*
    SwitchCondition&    operator , (ConditionNode &, const Type &);
    SwitchCondition&    operator , (Condition &, const Type &);
    SwitchCondition&    operator , (const Type &, ConditionNode &);
    SwitchCondition&    operator , (const Type &, Condition &);
 */
    SwitchCondition    operator , (const ConditionNode &, const Type &);
    SwitchCondition    operator , (const Condition &, const Type &);
    SwitchCondition    operator , (const Type &, const ConditionNode &);
    SwitchCondition    operator , (const Type &, const Condition &);
};

#endif //__RWSWITCH___H
