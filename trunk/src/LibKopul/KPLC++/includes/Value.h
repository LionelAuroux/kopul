#ifndef __VALUE___H
# define __VALUE___H
# include <string>

# include "StaticType.h"

namespace kpl
{
    class   Value : public Type
    {
    public:
        Value(const StaticType &, char);
        Value(const StaticType &, short);
        Value(const StaticType &, int);
        Value(const StaticType &, const std::string&);
        Value(const StaticType &, char *);
        Value(const StaticType &, void *);
        Value(const Type &type, const void *value, int sizeInBytes);
        Value(const Value &);
        ~Value();
        Value&              operator = (const Value &);

        int                 GetSize() const;
        int                 GetSizeInOctet() const;

        virtual llvm::Value *GetLLVMValue(llvm::BasicBlock *) const;

        const char *        GetValue() const;
        const Type&         GetTypeOfValue() const;

        // Get a string representation of the object
        virtual const std::string&  ToString() const;

        // Get the type of the object
        virtual const std::string&  GetType() const;

        // returns true if the given type and content are equal.
        virtual bool                Equals(const IObject &value) const;

        // create a new instance by making a deep copy of the current object data
        virtual IObject*            Clone() const;

        friend class DynamicArray;
    protected:

        void                    CreateCmp(llvm::Value *toCompare, llvm::BasicBlock *trueBlock, llvm::BasicBlock *falseBlock, llvm::BasicBlock *whereToBuild) const;

        // Build le type
        bool                    Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*	BuildEncodingToMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToSocket(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromSocket(llvm::BasicBlock *) const;

        int             _sizeInBytes;
        char            *_value;
        Type            *_type;
    private:
        std::string     _objectToStr;
        std::string     _objectType;
    };
};

#endif //__VALUE___H
