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

	bit.setName("i1");
	bit.buildFunctions(mod);
	bit.setName("i7");
	bit.setSize(7);
	bit.buildFunctions(mod);
	mod->dump();
	return (0);
}
