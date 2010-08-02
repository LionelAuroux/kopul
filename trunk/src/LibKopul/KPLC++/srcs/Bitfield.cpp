#include "Bitfield.h"

using namespace kpl;

Bitfield::Bitfield(int size)
{
	_size = size;
	_type = llvm::IntegerType::get(llvm::getGlobalContext(), size);
}

int		Bitfield::GetSize() const
{
	return (this->_size);
}

void		Bitfield::SetSize(int size)
{
	this->_size = size;
	_type = llvm::IntegerType::get(llvm::getGlobalContext(), size);
}
