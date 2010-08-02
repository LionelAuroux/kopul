#ifndef __TYPE___H
# define __TYPE___H
# include <map>

# include "Item.h"


namespace kpl
{
    class   Type;
    typedef llvm::BasicBlock* (Type::*CodeBuilder)(llvm::BasicBlock *) const;
    typedef llvm::Function* (Type::*FunctionBuilder)(llvm::Module *, const std::string &name) const;

    class	Type : public Item
    {
    private:
        // Pointeur sur methode de creation des fonctions
        std::map<MODE, FunctionBuilder>         _functionBuilderMap;
        // Pointeur sur methode de construction du code.
        std::map<BUILDER_MODE, CodeBuilder>     _codeBuilderMap;
        // Un mode est associe a deux mode de constructions, 1 de decodage et 1 d'encodage
        std::map<MODE, std::pair<BUILDER_MODE, BUILDER_MODE> >  _modeMap;
    protected:
        // type equivalent en llvm
	const llvm::Type*		_type;
		
	// Nom qui servira pour les noms des functions genere
	std::string			_name;

	/* 
		liste des parametre du prototype des fonctions d'encode et de decode
		la valeur de retour sera toujours un int. Gestion d'erreur.
		par default juste une stream dans la liste des params et un param
	*/
	std::vector<const llvm::Type*>	_listVariable;

    public:
	Type();
        ~Type();
	const std::string&		GetName() const;
	void				SetName(const std::string&);
        const llvm::Type*               GetLLVMType() const;
        // Ajoute une fonction au module
        llvm::Function*			CreateFunctionForMemory(llvm::Module *, const std::string &name) const;
        llvm::Function*			CreateFunctionForFile(llvm::Module *, const std::string &name) const;
        llvm::Function*			CreateFunctionForSocket(llvm::Module *, const std::string &name) const;
        llvm::Function*			CreateFunction(llvm::Module *, const std::string &name, MODE) const;
       	// Fonction qui construit les fonctions d'encode et de decode. retourne -1 si le module est corrompue 0 sinon.
        bool				BuildFunctions(llvm::Module *, MODE);
        llvm::BasicBlock*               BuildCode(llvm::BasicBlock *, BUILDER_MODE) const;
        virtual llvm::BasicBlock*       BuildEncodingToMemory(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromMemory(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToFile(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromFile(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToSocket(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromSocket(llvm::BasicBlock *) const = 0;

    };
};

#endif //__TYPE___H
