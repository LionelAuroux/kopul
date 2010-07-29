#ifndef __SWITCH___H
# define __SWITCH___H
# include <map>

# include "Type.h"
# include "Expression.h"

namespace kpl
{
	class	Switch : public Type
	{
		std::map<Expression, Type>	_caseMap;
	protected:
		int				buildEncodeFunction(llvm::Function *);
		int				buildDecodeFunction(llvm::Function *);
	public:
	};
};

#endif //__RWSWITCH___H
