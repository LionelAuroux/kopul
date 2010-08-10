/* 
 * File:   StaticType.h
 * Author: deveza
 *
 * Created on July 31, 2010, 9:42 PM
 */

#ifndef __STATICTYPE_H
# define __STATICTYPE_H
# include "Type.h"

namespace kpl
{
    class StaticType : public Type
    {
    public:
        StaticType();
        ~StaticType();
        virtual int             GetSize() const = 0;

        friend class Bitfield;
        friend class StaticArray;
        friend class StaticStruct;
    protected:

        // Build le type
        bool                    Build(llvm::Module *, MODE) const;
        llvm::BasicBlock*	BuildEncodingToMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToSocket(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromSocket(llvm::BasicBlock *) const;

        // retourne le type equivalent en LLVM a un StaticType de KOPUL
        virtual const llvm::Type*       GetLLVMType() const = 0;
    };
};

#endif	/* _STATICTYPE_H */

