#include "StaticArray.h"

using namespace kpl;

StaticArray::StaticArray(const StaticType& type, int size)
{
    _type = llvm::ArrayType::get(type.GetLLVMType(), size);
}

