/* 
 * File:   ConstantValue.h
 * Author: deveza
 *
 * Created on October 1, 2010, 2:38 PM
 */
#ifndef _CONSTANTVALUE_H
# define _CONSTANTVALUE_H
# include <sstream>
# include <typeinfo>

# include "StaticType.h"
# include "Type.h"
# include "Value.h"

namespace kpl
{
    class ConstantValue : public Type, public Value
    {
    public:
        template <typename T>
        ConstantValue(const StaticType &type, T value) : Value(type)
        {
            std::ostringstream  oss;

            memcpy(_value, &value, (int)sizeof (T) > type.GetSizeInOctet() || typeid(T).name()[0] == 'P' ? type.GetSizeInOctet() : sizeof (T));
            _objectType = "Value";
            oss << value;
            _objectToStr = _objectType + "_of_type_" + _type->ToString() + "_and_value_" + oss.str();
            _sizeInBytes = type.GetSize();
        }

        ConstantValue(const ConstantValue &);
        ~ConstantValue();
        ConstantValue&              operator = (const ConstantValue &);

        llvm::Value *       GetLLVMValue(llvm::BasicBlock *) const;
        void*               AllocBuffer(void *oldBuffer = NULL) const;
        void                FreeBuffer(void *oldBuffer) const;
        int                 GetSize() const;
        int                 GetSizeInOctet() const;
	void                SetName(const std::string&);

        Value               *Duplicate() const;
        // Get a string representation of the object
        const std::string&  ToString() const;
        const std::string&  ToStr() const;
        // Get the type of the object
        const std::string&  GetType() const;

        // returns true if the given type and content are equal.
        bool                Equals(const IObject &value) const;

        // create a new instance by making a deep copy of the current object data
        IObject*            Clone() const;

    protected:
        const llvm::Type*           GetLLVMType() const;

        // Build le type
        bool                    Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*	BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;

    private:
        template <typename T>
        void            Init(const StaticType &, T);

        int             _sizeInBytes;
        std::string     _objectToStr;
        std::string     _objectType;
    };
};

#endif	/* _CONSTANTVALUE_H */

