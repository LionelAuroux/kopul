#ifndef __TYPE___H
# define __TYPE___H
# include <map>

# include "Item.h"


namespace kpl
{
    class   Type;
    typedef llvm::BasicBlock* (Type::*CodeBuilder)(llvm::BasicBlock *) const;
    typedef llvm::Function* (Type::*FunctionBuilder)(llvm::Module *, const std::string &, const std::map<std::string, const llvm::Type*>&) const;

    class	Type : public Item
    {
    public:
	Type();
        ~Type();
	const std::string&		GetName() const;
	void				SetName(const std::string&);
        friend class Register;

    protected:
	// Nom qui servira pour les noms des functions genere
	std::string			_name;

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
        llvm::BasicBlock*               BuildCode(llvm::BasicBlock *, BUILDER_MODE) const;
        virtual llvm::BasicBlock*       BuildEncodingToMemory(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromMemory(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToFile(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromFile(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildEncodingToSocket(llvm::BasicBlock *) const = 0;
        virtual llvm::BasicBlock*       BuildDecodingFromSocket(llvm::BasicBlock *) const = 0;

    private:
        // Pointeur sur methode de creation des fonctions
        std::map<MODE, FunctionBuilder>                         _functionBuilderMap;
        // Pointeur sur methode de construction du code.
        std::map<BUILDER_MODE, CodeBuilder>                     _codeBuilderMap;
        // Un mode est associe a deux mode de constructions, 1 de decodage et 1 d'encodage
        std::map<MODE, std::pair<BUILDER_MODE, BUILDER_MODE> >  _modeMap;
        // Un type peut prendre des variable comme parametre
    };
};

#endif //__TYPE___H
