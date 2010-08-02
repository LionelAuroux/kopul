#ifndef __STRUCT___H
# define __STRUCT___H
# include <string>
# include <vector>

# include "Type.h"

namespace kpl
{
	class	StaticStruct : public Type
	{
		std::vector<Type>	_listType;
	public:
	};
};

#endif //__STRUCT___H
