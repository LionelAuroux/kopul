#ifndef __SWITCH___H
# define __SWITCH___H
# include <vector>
# include <map>

# include "Type.h"
# include "Container.h"
# include "SwitchCondition.h"
# include "Variable.h"
# include "IComposedType.h"

namespace kpl
{
    typedef struct  s_switch
    {
        void        **_buffer;
        int         _succesCase;
    }               SwitchBuffer;

    class	Switch : public Type, public IComposedType, public Container<SwitchCondition>
    {
    public:
        Switch();
        Switch(const Switch &);
        ~Switch();
        Switch& operator = (const Switch &);
        void                SetName(const std::string& name);
        const std::string&  ToString() const ;
        const std::string&  GetType() const;
        bool                Equals(const IObject &value) const;
        IObject*            Clone() const;
        void*               AllocBuffer(void *oldBuffer = NULL) const;
        void                FreeBuffer(void *oldBuffer) const;
        int                 GetNbType() const;
        const Type          &GetTypeN(int i) const;

    protected:
        const llvm::Type*   GetLLVMType() const;
        bool                Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*   BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*   BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*   BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*   BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*   BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*   BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
    private:
        std::string     *_objectToStr;
        std::string     _objectType;
    };

    SwitchCondition    operator , (const ICondition &, const Type &);
    SwitchCondition    operator , (const Type &, const ICondition &);
};

#endif //__RWSWITCH___H
