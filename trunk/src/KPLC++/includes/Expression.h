#ifndef __EXPRESSION___H
# define __EXPRESSION___H
# include <llvm/LLVMContext.h>
# include <llvm/Module.h>
# include <llvm/Support/IRBuilder.h>

namespace kpl
{
	class Expression
	{
	public:
		// prend en parametre le block d'entree d'une fonction, cree une expression dans ce block et retourne le block cree.
		virtual llvm::BasicBlock *		buildExpression(llvm::BasicBlock *) = 0;
	};
};

#endif // __EXPRESSION___H
