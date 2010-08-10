#ifndef __DYNAMICARRAY___H
# define __DYNAMICARRAY___H
# include "Type.h"

namespace kpl
{
	class	DynamicArray : public Type
	{
	protected:
            Type*   _type;
	public:
            DynamicArray(const Type& type);
	};
};

#endif //__DYNAMICARRAY___H
