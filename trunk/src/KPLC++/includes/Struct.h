#ifndef __STRUCT___H
# define __STRUCT___H
# include <string>
# include <vector>

# include "Type.h"

namespace kpl
{
	class	Struct : public Type
	{
		std::string		_name; //optionnel
		std::vector<Type>	_listType;
	protected:
		int			buildEncodeFunction(llvm::Function *);
		int			buildDecodeFunction(llvm::Function *);
	public:
	};
};

#endif //__STRUCT___H
