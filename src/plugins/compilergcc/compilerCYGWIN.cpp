#ifdef __WXMSW__
// this compiler is valid only in windows

#include "compilerCYGWIN.h"
#include <wx/filefn.h>
#include <wx/msw/registry.h>

CompilerCYGWIN::CompilerCYGWIN()
    : CompilerMINGW(_("Cygwin GCC"), _T("cygwin"))
{
    Reset();
}

CompilerCYGWIN::~CompilerCYGWIN()
{
}

Compiler * CompilerCYGWIN::CreateCopy()
{
    Compiler* c = new CompilerCYGWIN(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerCYGWIN::Reset()
{
    CompilerMINGW::Reset();

	m_Programs.C = _T("gcc.exe");
	m_Programs.CPP = _T("g++.exe");
	m_Programs.LD = _T("g++.exe");
	m_Programs.DBG = _T("gdb.exe");
	m_Programs.LIB = _T("ar.exe");
	m_Programs.WINDRES = _T("windres.exe");
	m_Programs.MAKE = _T("make.exe");

    m_Switches.forceFwdSlashes = true;

	m_Options.AddOption(_("Do not use cygwin specific functionality"), _T("-mno-cygwin"), _("General"));
}

#endif // __WXMSW__
