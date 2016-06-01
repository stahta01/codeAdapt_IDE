#ifdef __WXMSW__
// this compiler is valid only in windows

#include "compilerCYGWIN.h"
#include <wx/filefn.h>
#include <wx/msw/registry.h>

CompilerCYGWIN::CompilerCYGWIN()
    : CompilerMINGW(_("Cygwin GCC"), wxT_2("cygwin"))
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

	m_Programs.C = wxT_2("gcc.exe");
	m_Programs.CPP = wxT_2("g++.exe");
	m_Programs.LD = wxT_2("g++.exe");
	m_Programs.DBG = wxT_2("gdb.exe");
	m_Programs.LIB = wxT_2("ar.exe");
	m_Programs.WINDRES = wxT_2("windres.exe");
	m_Programs.MAKE = wxT_2("make.exe");

    m_Switches.forceFwdSlashes = true;

}

AutoDetectResult CompilerCYGWIN::AutoDetectInstallationDir()
{
    m_MasterPath = wxT_2("C:\\Cygwin"); // just a guess

    // look in registry for Cygwin

    wxRegKey key; // defaults to HKCR
    key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\Cygnus Solutions\\Cygwin\\mounts v2\\/"));
    if (key.Exists() && key.Open(wxRegKey::Read))
    {
        // found; read it
        key.QueryValue(wxT_2("native"), m_MasterPath);
    }
    AutoDetectResult ret = wxFileExists(m_MasterPath + wxFILE_SEP_PATH +
                                        wxT_2("bin") + wxFILE_SEP_PATH +
                                        m_Programs.C)
                            ? adrDetected
                            : adrGuessed;
    return ret;
}

#endif // __WXMSW__
