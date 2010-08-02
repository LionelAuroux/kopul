#ifndef __STATICARRAY___H
# define __STATICARRAY___H
# include "StaticType.h"

namespace kpl
{
	class	StaticArray : public StaticType
	{
		int			_size;

	public:
            StaticArray(const StaticType& type, int size);
	};
};

#endif //__STATICARRAY___H
