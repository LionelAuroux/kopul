#include "DynamicArray.h"

using namespace kpl;

DynamicArray::DynamicArray(const Type& type)
{
    _type = llvm::PointerType::getUnqual(type.GetLLVMType());
}

int			DynamicArray::buildEncodeFunction(llvm::Function *)
{
	std::cout << "build function encode de DYNAMICARRAY" << std::endl;
	return (0);
}

int			DynamicArray::buildDecodeFunction(llvm::Function *)
{
	std::cout << "build function decode de DYNAMICARRAY" << std::endl;
	return (0);
}
