#ifdef __WXMSW__
// this compiler is valid only in windows

#include "ca/sdk.h"
#include "compilerMSVC.h"
#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/msw/registry.h>

CompilerMSVC::CompilerMSVC()
    : Compiler(_("Microsoft Visual C++ Toolkit 2003"), wxT_2("msvctk"))
{
    Reset();
}

CompilerMSVC::~CompilerMSVC()
{
	//dtor
}

Compiler * CompilerMSVC::CreateCopy()
{
    Compiler* c = new CompilerMSVC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerMSVC::Reset()
{
	m_Programs.C = wxT_2("cl.exe");
	m_Programs.CPP = wxT_2("cl.exe");
	m_Programs.LD = wxT_2("link.exe");
	m_Programs.LIB = wxT_2("link.exe");
	m_Programs.WINDRES = wxT_2("rc.exe"); // platform SDK is needed for this
	m_Programs.MAKE = wxT_2("nmake.exe");
	m_Programs.DBG = wxT_2("cdb.exe");

	m_Switches.includeDirs = wxT_2("/I");
	m_Switches.libDirs = wxT_2("/LIBPATH:");
	m_Switches.linkLibs = wxT_2("");
	m_Switches.defines = wxT_2("/D");
	m_Switches.genericSwitch = wxT_2("/");
	m_Switches.objectExtension = wxT_2("obj");
	m_Switches.needDependencies = false;
	m_Switches.forceCompilerUseQuotes = false;
	m_Switches.forceLinkerUseQuotes = false;
	m_Switches.logging = clogNone;
	m_Switches.libPrefix = wxT_2("");
	m_Switches.libExtension = wxT_2("lib");
	m_Switches.linkerNeedsLibPrefix = false;
	m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();
	m_Options.AddOption(_("Produce debugging symbols"),
				wxT_2("/Zi /D_DEBUG"),
				_("Debugging"),
				wxT_2("/DEBUG"),
				true,
				wxT_2("/Og /O1 /O2 /Os /Ot /Ox /NDEBUG"),
				_("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
	m_Options.AddOption(_("Enable all compiler warnings"), wxT_2("/Wall"), _("Warnings"));
	m_Options.AddOption(_("Enable warnings level 1"), wxT_2("/W1"), _("Warnings"));
	m_Options.AddOption(_("Enable warnings level 2"), wxT_2("/W2"), _("Warnings"));
	m_Options.AddOption(_("Enable warnings level 3"), wxT_2("/W3"), _("Warnings"));
	m_Options.AddOption(_("Enable warnings level 4"), wxT_2("/W4"), _("Warnings"));
	m_Options.AddOption(_("Enable 64bit porting warnings"), wxT_2("/Wp64"), _("Warnings"));
	m_Options.AddOption(_("Treat warnings as errors"), wxT_2("/WX"), _("Warnings"));
	m_Options.AddOption(_("Enable global optimization"), wxT_2("/Og"), _("Optimization"));
	m_Options.AddOption(_("Maximum optimization (no need for other options)"), wxT_2("/Ox"), _("Optimization"));
	m_Options.AddOption(_("Disable optimizations"), wxT_2("/Od"), _("Optimization")); //added no optimization
	m_Options.AddOption(_("Minimize space"), wxT_2("/O1"), _("Optimization"));
	m_Options.AddOption(_("Maximize speed"), wxT_2("/O2"), _("Optimization"));
	m_Options.AddOption(_("Favor code space"), wxT_2("/Os"), _("Optimization"));
	m_Options.AddOption(_("Favor code speed"), wxT_2("/Ot"), _("Optimization"));
	m_Options.AddOption(_("Enable C++ RTTI"), wxT_2("/GR"), _("C++ Features"));
	m_Options.AddOption(_("Enable C++ exception handling"), wxT_2("/GX"), _("C++ Features"));
	m_Options.AddOption(_("Optimize for 80386"), wxT_2("/G3"), _("Architecture"));
	m_Options.AddOption(_("Optimize for 80486"), wxT_2("/G4"), _("Architecture"));
	m_Options.AddOption(_("Optimize for Pentium"), wxT_2("/G5"), _("Architecture"));
	m_Options.AddOption(_("Optimize for Pentium Pro, Pentium II, Pentium III"), wxT_2("/G6"), _("Architecture"));
	m_Options.AddOption(_("Optimize for Pentium 4 or Athlon"), wxT_2("/G7"), _("Architecture"));
	m_Options.AddOption(_("Enable SSE instruction set"), wxT_2("/arch:SSE"), _("Architecture"));
	m_Options.AddOption(_("Enable SSE2 instruction set"), wxT_2("/arch:SSE2"), _("Architecture"));
	m_Options.AddOption(_("Enable minimal rebuild"), wxT_2("/Gm"), _("Others"));
	m_Options.AddOption(_("Enable link-time code generation"), wxT_2("/GL"), _("Others"), wxT_2(""), true, wxT_2("/Zi /ZI"), _("Link-time code generation is incompatible with debugging info"));
	m_Options.AddOption(_("Optimize for windows application"), wxT_2("/GA"), _("Others"));
	m_Options.AddOption(_("__cdecl calling convention"), wxT_2("/Gd"), _("Others"));
	m_Options.AddOption(_("__fastcall calling convention"), wxT_2("/Gr"), _("Others"));
	m_Options.AddOption(_("__stdcall calling convention"), wxT_2("/Gz"), _("Others"));
    // Added Runtime options for cl.exe, that is the runtime library selection
    m_Options.AddOption(_("Single-threaded Runtime Library"), wxT_2("/ML"), _("Runtime"));
    m_Options.AddOption(_("Single-threaded Debug Runtime Library"), wxT_2("/MLd"), _("Runtime"));
    m_Options.AddOption(_("Multi-threaded Runtime Library"), wxT_2("/MT"), _("Runtime"), wxT_2(""), true);
    m_Options.AddOption(_("Multi-threaded Debug Runtime Library"), wxT_2("/MTd"), _("Runtime"));
    m_Options.AddOption(_("Multi-threaded DLL Runtime Library"), wxT_2("/MD"), _("Runtime"));
    m_Options.AddOption(_("Multi-threaded DLL Debug Runtime Library"), wxT_2("/MDd"), _("Runtime"));


    m_Commands[(int)ctCompileObjectCmd].push_back( CompilerTool(wxT_2("$compiler /nologo $options $includes /c $file /Fo$object")) );
    m_Commands[(int)ctCompileResourceCmd].push_back( CompilerTool(wxT_2("$rescomp $res_includes -fo$resource_output $file")) );
    m_Commands[(int)ctLinkExeCmd].push_back( CompilerTool(wxT_2("$linker /nologo /subsystem:windows $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkConsoleExeCmd].push_back( CompilerTool(wxT_2("$linker /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkDynamicCmd].push_back( CompilerTool(wxT_2("$linker /dll /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkStaticCmd].push_back( CompilerTool(wxT_2("$lib_linker /lib /nologo $libdirs /out:$static_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkNativeCmd].push_back( CompilerTool(wxT_2("$linker /nologo /subsystem:native $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerMSVC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\) :[ \t]([Ww]arning[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\) :[ \t](.*[Ee]rror[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")[ \t]+:[ \t]+(.*warning LNK[0-9]+.*)"), 2, 1, 0));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")[ \t]+:[ \t]+(.*error LNK[0-9]+.*)"), 2, 1, 0));
}

AutoDetectResult CompilerMSVC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();

    // Read the VCToolkitInstallDir environment variable
    wxGetEnv(wxT_2("VCToolkitInstallDir"), &m_MasterPath);

    if (m_MasterPath.IsEmpty())
    {
        // just a guess; the default installation dir
        wxString Programs = wxT_2("C:\\Program Files");
        // what's the "Program Files" location
        // TO DO : support 64 bit ->    32 bit apps are in "ProgramFiles(x86)"
        //                              64 bit apps are in "ProgramFiles"
        wxGetEnv(wxT_2("ProgramFiles"), &Programs);
        m_MasterPath = Programs + wxT_2("\\Microsoft Visual C++ Toolkit 2003");
    }
    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));

        // add include dirs for MS Platform SDK too
        wxRegKey key; // defaults to HKCR
        key.SetName(wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\Win32SDK\\Directories"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            wxString dir;
            key.QueryValue(wxT_2("Install Dir"), dir);
            if (!dir.IsEmpty())
            {
                if (dir.GetChar(dir.Length() - 1) != '\\')
                    dir += sep;
                AddIncludeDir(dir + wxT_2("include"));
                AddLibDir(dir + wxT_2("lib"));
                m_ExtraPaths.Add(dir + wxT_2("bin"));
            }
        }

        // add extra paths for "Debugging tools" too
        key.SetName(wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\DebuggingTools"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            wxString dir;
            key.QueryValue(wxT_2("WinDbg"), dir);
            if (!dir.IsEmpty())
            {
                if (dir.GetChar(dir.Length() - 1) == '\\')
                    dir.Remove(dir.Length() - 1, 1);
                m_ExtraPaths.Add(dir);
            }
        }
    }

    return wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
