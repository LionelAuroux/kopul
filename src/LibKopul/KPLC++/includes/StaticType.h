/* 
 * File:   StaticType.h
 * Author: deveza
 *
 * Created on July 31, 2010, 9:42 PM
 */

#ifndef __STATICTYPE__H
# define __STATICTYPE__H
# include "Type.h"

namespace kpl
{
    class StaticType : public Type
    {
    public:
        StaticType();
        ~StaticType();
        // Size in bytes
        virtual int     GetSize() const = 0;
        // Size in octet
        virtual int     GetSizeInOctet() const = 0;
        void*           AllocBuffer(void *oldBuffer = NULL) const;
        void            FreeBuffer(void *oldBuffer) const;

        friend class Bitfield;
        friend class StaticArray;
        friend class StaticStruct;
        friend class Value;
    protected:
        const llvm::Type*       GetLLVMType() const;

        // Build le type
        bool                    Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*	BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
        llvm::BasicBlock*	BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const;
    };
};

#endif	/* _STATICTYPE_H */

