#include "DynamicArray.h"

using namespace kpl;

DynamicArray::DynamicArray(const Type& type)
{
    _type = static_cast<Type *>(type.Clone());
}

