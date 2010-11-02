#ifndef __DYNAMICARRAY___H
# define __DYNAMICARRAY___H
# include "Type.h"
# include "StaticType.h"
# include "ConstantValue.h"
#include "SwitchCondition.h"
#include "Switch.h"
#include "IComposedType.h"

namespace kpl
{
	class	DynamicArray : public Type, public IComposedType
	{
	public:
            DynamicArray(const SwitchCondition&, int nbMax = 1024);
            DynamicArray(const DynamicArray&);
            ~DynamicArray();
            DynamicArray&       operator = (const DynamicArray&);
            const SwitchCondition&      GetSwitchCondition() const;
            void                        SetName(const std::string&);
            void*                       AllocBuffer(void *oldBuffer = NULL) const;
            void                        FreeBuffer(void *oldBuffer) const;
            int                         GetNbType() const;
            const Type                  &GetTypeN(int i) const;
            int                         GetNbMax() const;
            // Get a string representation of the object
            const std::string&          ToString() const;

            // Get the type of the object
            const std::string&          GetType() const;

            // returns true if the given type and content are equal.
            bool                        Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
            IObject*                    Clone() const;
        protected:
            const llvm::Type*           GetLLVMType() const;
            bool                        Build(llvm::Module *, MODE) const;
            llvm::BasicBlock*           BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
            llvm::BasicBlock*           BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
            llvm::BasicBlock*           BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
            llvm::BasicBlock*           BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
            llvm::BasicBlock*           BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
            llvm::BasicBlock*           BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        private:
            SwitchCondition*    _sCond;
            std::string         _objectToStr;
            std::string         _objectType;
            int                 _nbMax;
            enum MEMORY_TYPE {ENCODE, DECODE,};
            llvm::BasicBlock*           MemoryBuilding(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, MEMORY_TYPE flag) const;
	};
};

#endif //__DYNAMICARRAY___H
