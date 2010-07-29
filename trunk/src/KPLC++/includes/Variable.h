#ifndef __VARIABLE___H
# define __VARIABLE___H
# include <string>

# include "Value.h"

namespace kpl
{
	class	Variable : public Value
	{
		std::string	_name;
	};
};

#endif //__VARIABLE___H
