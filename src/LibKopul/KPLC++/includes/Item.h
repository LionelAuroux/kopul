/* 
 * File:   Item.h
 * Author: deveza
 *
 * Created on July 31, 2010, 9:47 PM
 */

#ifndef _ITEM_H
# define _ITEM_H
# include <iostream>
# include <string>
# include <llvm/LLVMContext.h>
# include <llvm/Module.h>
# include <llvm/Support/IRBuilder.h>
# include <llvm/Analysis/Verifier.h>
# include <llvm/ValueSymbolTable.h>

namespace kpl
{
    enum    BUILDER_MODE
    {
        ENCODE_TO_MEMORY,
        DECODE_FROM_MEMORY,
        ENCODE_TO_FILE,
        DECODE_FROM_FILE,
        ENCODE_TO_SOCKET,
        DECODE_FROM_SOCKET,
    };

    enum    MODE
    {
        MEMORY_MODE,
        FILE_MODE,
        SOCKET_MODE,
    };

    class   Item
    {

    public:
        virtual ~Item(){};
        virtual llvm::BasicBlock*   BuildCode(llvm::BasicBlock *, BUILDER_MODE) const = 0;
    };
};

#endif	/* _ITEM_H */

