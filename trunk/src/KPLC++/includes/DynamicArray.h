#ifndef __DYNAMICARRAY___H
# define __DYNAMICARRAY___H
# include "Type.h"

namespace kpl
{
	class	DynamicArray : public Type
	{
	protected:
		int			buildEncodeFunction(llvm::Function *);
		int			buildDecodeFunction(llvm::Function *);
	public:
	};
};

#endif //__DYNAMICARRAY___H
