#ifndef COMPILER_GNUTRICORE_H
#define COMPILER_GNUTRICORE_H
#include <compiler.h>
class CompilerGNUTRICORE : public Compiler
{
	public:
		CompilerGNUTRICORE();
		virtual ~CompilerGNUTRICORE();
		virtual void Reset();
		virtual void LoadDefaultRegExArray();
	protected:
		virtual Compiler* CreateCopy();
};
#endif // COMPILER_GNUTRICORE_H
