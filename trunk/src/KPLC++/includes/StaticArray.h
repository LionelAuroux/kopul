#ifndef __STATICARRAY___H
# define __STATICARRAY___H
# include "Type.h"

namespace kpl
{
	class	StaticArray : public Type
	{
		int			_size;
	protected:
		int			buildEncodeFunction(llvm::Function *);
		int			buildDecodeFunction(llvm::Function *);
	public:
	};
};

#endif //__STATICARRAY___H
