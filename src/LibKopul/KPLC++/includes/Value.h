#ifndef __VALUE___H
# define __VALUE___H
# include <string>

# include "Type.h"

namespace kpl
{
	class	Value : public Type
	{
		std::string 	_val;
	protected:
		int			buildEncodeFunction(llvm::Function *);
		int			buildDecodeFunction(llvm::Function *);

	public:
	};
};

#endif //__VALUE___H
