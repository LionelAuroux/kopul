#ifndef __BITFIELD___H
# define __BITFIELD___H
# include "StaticType.h"

namespace kpl
{
	class	Bitfield : public StaticType
	{
		int		_size;

	public:
		Bitfield(int size = 32);
		int		GetSize() const;
		void		SetSize(int size);
        private:

	};
};

#endif //__BITFIELD___H
