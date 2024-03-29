#ifndef __TYPE___H
# define __TYPE___H
# include <iostream>
# include <string>
# include <map>
# include <llvm/LLVMContext.h>
# include <llvm/Module.h>
# include <llvm/Support/IRBuilder.h>
# include <llvm/Analysis/Verifier.h>
# include <llvm/ValueSymbolTable.h>
# include <llvm/Target/TargetData.h>
# include <llvm/Target/TargetSelect.h>

# include "IObject.h"

# define CONVERT_NBBYTES_TO_NBOCTET(x) (((x) % 8 > 0) ? ((x) / 8 + 1) : ((x) / 8))

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

    class   Type;
    typedef llvm::BasicBlock* (Type::*CodeBuilder)(llvm::BasicBlock *, llvm::Value *, llvm::Value *, llvm::Value *) const;
    typedef llvm::Function* (Type::*FunctionBuilder)(llvm::Module *, const std::string &, const std::map<std::string, const llvm::Type*>&) const;

    class	Type : public IObject
    {
    public:
	Type();
        ~Type();
        const std::string&              GetEncodeFunctionName() const;
        const std::string&              GetDecodeFunctionName() const;
	const std::string&		GetName() const;
	virtual void                    SetName(const std::string&);
        // cree un buffer du type. Si oldbuffer different de null est copie dans le nouveau buffer
        virtual void*                   AllocBuffer(void *oldBuffer = NULL) const = 0;
        virtual void                    FreeBuffer(void *oldBuffer) const = 0;
        // Return number of type present an a specific type
        friend class Register;
        friend class ConstantValue;
        friend class SwitchCondition;
        friend class DynamicArray;
    protected:

        // BlockOut (Stream, nbBytes, sizeInBytes, WhereToBuild)
        llvm::BasicBlock*               CreateAddSizeToStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, int sizeInBytes, llvm::BasicBlock *whereToBuild) const;
        llvm::BasicBlock*               CreateStoreParamToStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, int sizeParamInBytes, llvm::BasicBlock *whereToBuild, const std::string &newBlockName = "") const;
        llvm::BasicBlock*               CreateLoadParamFromStream(llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, int sizeParamInBytes, llvm::BasicBlock *whereToBuild, const std::string &newBlockName = "") const;

        // Ajoute une fonction au module
        llvm::Function*			CreateFunctionForMemory(llvm::Module *, const std::string &name, const std::map<std::string, const llvm::Type*>&) const;
        llvm::Function*			CreateFunctionForFile(llvm::Module *, const std::string &name, const std::map<std::string, const llvm::Type*>&) const;
        llvm::Function*			CreateFunctionForSocket(llvm::Module *, const std::string &name, const std::map<std::string, const llvm::Type*>&) const;
        llvm::Function*			CreateFunction(llvm::Module *, const std::string &name, const std::map<std::string, const llvm::Type*>&, MODE) const;

        // Fonction qui construit les fonctions d'encode et de decode. retourne -1 si le module est corrompue 0 sinon.
        bool                            BuildFunctions(llvm::Module *, const std::map<std::string, const llvm::Type*>&, MODE) const;

        // Build le type
        virtual bool                    Build(llvm::Module *, MODE) const = 0;

        // Construit le code dans un block donne et retourne le nouveau block actif
        llvm::BasicBlock*               BuildCode(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr, BUILDER_MODE) const;
        virtual llvm::BasicBlock*       BuildEncodingToMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromMemory(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromFile(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromSocket(llvm::BasicBlock *actionBlock, llvm::Value *streamAdr, llvm::Value *nbBytesAdr, llvm::Value *paramAdr) const = 0;

        // pour obtenir le type equivalent en llvm
        virtual const llvm::Type*       GetLLVMType() const = 0;

    private:
        // Pointeur sur methode de creation des fonctions
        std::map<MODE, FunctionBuilder>                         _functionBuilderMap;
        // Pointeur sur methode de construction du code.
        std::map<BUILDER_MODE, CodeBuilder>                     _codeBuilderMap;
        // Un mode est associe a deux mode de constructions, 1 de decodage et 1 d'encodage
        std::map<MODE, std::pair<BUILDER_MODE, BUILDER_MODE> >  _modeMap;
        // Un type peut prendre des variable comme parametre
        // Nom qui servira pour les noms des functions genere
	std::string                                             _name;
        std::string                                             _encodeFunctionName;
        std::string                                             _decodeFunctionName;
    };
};

#endif //__TYPE___H
