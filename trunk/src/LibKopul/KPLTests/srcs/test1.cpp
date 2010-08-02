#include "Bitfield.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetSelect.h"


using namespace llvm;
using namespace kpl;

int main()
{
	InitializeNativeTarget();

	//Creation d'un module
	Module 		*mod = new Module("test", getGlobalContext());
	Bitfield	bit(1);

	bit.SetName("i1");
	bit.BuildFunctions(mod, MEMORY_MODE);
	bit.SetName("i7");
	bit.SetSize(7);
	bit.BuildFunctions(mod, MEMORY_MODE);
	mod->dump();
	return (0);
}
