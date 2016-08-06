#ifndef COMPILER_TCC_H
#define COMPILER_TCC_H

#include "compiler.h"

class CompilerTcc : public Compiler
{
	public:
		CompilerTcc();
		virtual ~CompilerTcc();
        virtual void Reset();
		virtual void LoadDefaultRegExArray();
    protected:
        virtual Compiler* CreateCopy();
};

#endif // COMPILER_TCC_H
