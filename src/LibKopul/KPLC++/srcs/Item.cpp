#include "Item.h"

using namespace kpl;

bool Item::llvmIsInit;

Item::Item()
{
    if (!llvmIsInit)
    {
	llvm::InitializeNativeTarget();
        llvmIsInit = true;
    }
}

Item::~Item()
{

}
