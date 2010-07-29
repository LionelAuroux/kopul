#ifndef __BITFIELD___H
# define __BITFIELD___H
# include "Type.h"

namespace kpl
{
	class	Bitfield : public Type
	{
		int		_size;
	protected:
		int		buildEncodeFunction(llvm::Function *);
		int		buildDecodeFunction(llvm::Function *);
	public:
		Bitfield(int size = 32);
		int		getSize() const;
		void		setSize(int size);
	};
};

#endif //__BITFIELD___H
