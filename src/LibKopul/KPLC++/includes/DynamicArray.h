#ifndef __DYNAMICARRAY___H
# define __DYNAMICARRAY___H
# include "Type.h"
# include "StaticType.h"
# include "Value.h"

namespace kpl
{
	class	DynamicArray : public Type
	{
	public:
            DynamicArray(const Value&);
            DynamicArray(const DynamicArray&);
            ~DynamicArray();
            DynamicArray&       operator = (const DynamicArray&);
            const Value&        GetEndValue() const;
            void                SetEndValue(const Value&);

            // Get a string representation of the object
            const std::string&  ToString() const;

            // Get the type of the object
            const std::string&  GetType() const;

            // returns true if the given type and content are equal.
            bool                Equals(const IObject &value) const;

            // create a new instance by making a deep copy of the current object data
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
            Value           *_endValue;
            std::string     _objectToStr;
            std::string     _objectType;
	};
};

#endif //__DYNAMICARRAY___H
