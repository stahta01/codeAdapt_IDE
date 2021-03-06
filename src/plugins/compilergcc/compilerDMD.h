#if defined(_WIN32) || defined(__linux__)
// this compiler is valid only in windows and linux

#ifndef COMPILERDMD_H
#define COMPILERDMD_H

#include <compiler.h>

class CompilerDMD : public Compiler
{
    public:
        CompilerDMD();
        virtual ~CompilerDMD();
        virtual void Reset();
        virtual void LoadDefaultRegExArray();
    protected:
        Compiler * CreateCopy();
    private:
};

#endif // COMPILERDMD_H

#endif // _WIN32 || linux
