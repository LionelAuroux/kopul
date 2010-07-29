#ifndef __TYPE___H
# define __TYPE___H
# include <iostream>
# include <string>
# include <llvm/LLVMContext.h>
# include <llvm/Module.h>
# include <llvm/Support/IRBuilder.h>
# include <llvm/Analysis/Verifier.h>
# include "llvm/ValueSymbolTable.h"

namespace kpl
{
	class	Type
	{
	protected:
		// type equivalent en llvm
		const llvm::Type*		_type;
		
		// Nom qui servira pour les noms des functions genere
		std::string			_name;

		/* 
			liste des parametre du prototype des fonctions d'encode et de decode
		 	la valeur de retour sera toujours un int. Gestion d'erreur.
			par default juste une stream dans la liste des params
		*/
		std::vector<const llvm::Type*>	_listVariable;
		
		// Fontion qui insert une fonction dans le module et la retourne
		llvm::Function*			createFunction(llvm::Module *, const std::string &name) const;

		// Methodes a implementer par chaque class qui herite de type. Ces methodes sont appelle depuis BuildFunctions de Type
		virtual int			buildEncodeFunction(llvm::Function *) = 0;
		virtual int			buildDecodeFunction(llvm::Function *) = 0;


	public:
		Type();

		const std::string&		getName() const;
		void				setName(const std::string&);

		// Fonction qui construit les fonctions d'encode et de decode. retourne -1 si le module est corrompue 0 sinon.
		bool				buildFunctions(llvm::Module *);
	};
};

#endif //__TYPE___H
