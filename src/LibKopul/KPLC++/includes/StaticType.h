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
        llvm::BasicBlock*	BuildEncodingToMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromMemory(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromFile(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildEncodingToSocket(llvm::BasicBlock *) const;
        llvm::BasicBlock*	BuildDecodingFromSocket(llvm::BasicBlock *) const;
    private:
    };
};

#endif	/* _STATICTYPE_H */

