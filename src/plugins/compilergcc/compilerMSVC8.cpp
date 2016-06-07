#ifdef __WXMSW__
// this compiler is valid only in windows

#include "ca/sdk.h"
#include "compilerMSVC8.h"
#include <wx/wx.h>
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/msw/registry.h>

CompilerMSVC8::CompilerMSVC8()
    : Compiler(_("Microsoft Visual C++ 2005/2008"), wxT_2("msvc8"))
{
    Reset();
}

CompilerMSVC8::~CompilerMSVC8()
{
    //dtor
}

Compiler * CompilerMSVC8::CreateCopy()
{
    Compiler* c = new CompilerMSVC8(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerMSVC8::Reset()
{
    m_Programs.C = wxT_2("cl.exe");
    m_Programs.CPP = wxT_2("cl.exe");
    m_Programs.LD = wxT_2("link.exe");
    m_Programs.LIB = wxT_2("link.exe");
    m_Programs.WINDRES = wxT_2("rc.exe");
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

    //Language
    m_Options.AddOption(_("Produce debugging symbols"),
                        wxT_2("/Zi /D_DEBUG"),
                        _("Language"),
                        wxT_2("/DEBUG"),
                        true,
                        wxT_2("/Og /O1 /O2 /Os /Ot /Ox /NDEBUG"),
                        _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
    m_Options.AddOption(_("Disable extensions"), wxT_2("/Za"), _("Language"));
    // /vd{0|1|2} disable/enable vtordisp
    // /vm<x> type of pointers to members
    m_Options.AddOption(_("Enforce Standard C++ for scoping rules"), wxT_2("/Zc:forScope"), _("Language"));
    m_Options.AddOption(_("wchar_t is the native type, not a typedef"), wxT_2("/Zc:wchar_t"), _("Language"));
    m_Options.AddOption(_("Enable Edit and Continue debug info"), wxT_2("/ZI"), _("Language"));
    m_Options.AddOption(_("Enable OpenMP 2.0 language extensions"), wxT_2("/openmp"), _("Language"));

    //Warnings
    m_Options.AddOption(_("Disable all warnings"), wxT_2("/w"), _("Warnings"));
    m_Options.AddOption(_("Enable all compiler warnings"), wxT_2("/Wall"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 1"), wxT_2("/W1"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 2"), wxT_2("/W2"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 3"), wxT_2("/W3"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 4"), wxT_2("/W4"), _("Warnings"));
    m_Options.AddOption(_("Enable one line diagnostics"), wxT_2("/WL"), _("Warnings"));
    m_Options.AddOption(_("Enable 64bit porting warnings"), wxT_2("/Wp64"), _("Warnings"));
    m_Options.AddOption(_("Treat warnings as errors"), wxT_2("/WX"), _("Warnings"));

    //Optimization
    //m_Options.AddOption(_("Enable global optimization"), wxT_2("/Og"), _("Optimization")); // Deprecated in MSVC 8
    m_Options.AddOption(_("Maximum optimization (no need for other options)"), wxT_2("/Ox"), _("Optimization"));
    m_Options.AddOption(_("Disable optimizations"), wxT_2("/Od"), _("Optimization")); //added no optimization
    m_Options.AddOption(_("Minimize space"), wxT_2("/O1"), _("Optimization"));
    m_Options.AddOption(_("Maximize speed"), wxT_2("/O2"), _("Optimization"));
    m_Options.AddOption(_("Favor code space"), wxT_2("/Os"), _("Optimization"));
    m_Options.AddOption(_("Favor code speed"), wxT_2("/Ot"), _("Optimization"));
    m_Options.AddOption(_("Enable intrinsic functions"), wxT_2("/Oi"), _("Optimization"));
    m_Options.AddOption(_("Enable frame pointer omission"), wxT_2("/Oy"), _("Optimization"));
    m_Options.AddOption(_("Inline expansion"), wxT_2("/Ob"), _("Optimization"));

    //Code generation
    m_Options.AddOption(_("Enable read-only string pooling"), wxT_2("/GF"), _("Code generation"));
    m_Options.AddOption(_("Separate functions for linker"), wxT_2("/Gy"), _("Code generation"));
    m_Options.AddOption(_("Enable security checks"), wxT_2("/GS"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ RTTI"), wxT_2("/GR"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ exception handling (no SEH)"), wxT_2("/EHs"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ exception handling (w/ SEH)"), wxT_2("/EHa"), _("Code generation"));
    m_Options.AddOption(_("extern \"C\" defaults to nothrow"), wxT_2("/EHc"), _("Code generation"));
    m_Options.AddOption(_("Consider floating-point exceptions when generating code"), wxT_2("/fp:except"), _("Code generation"));
    m_Options.AddOption(_("Do not consider floating-point exceptions when generating code"), wxT_2("/fp:except-"), _("Code generation"));
    m_Options.AddOption(_("\"fast\" floating-point model; results are less predictable"), wxT_2("/fp:fast"), _("Code generation"));
    m_Options.AddOption(_("\"precise\" floating-point model; results are predictable"), wxT_2("/fp:precise"), _("Code generation"));
    m_Options.AddOption(_("\"strict\" floating-point model (implies /fp:except)"), wxT_2("/fp:strict"), _("Code generation"));
    m_Options.AddOption(_("Enable minimal rebuild"), wxT_2("/Gm"), _("Code generation"));
    m_Options.AddOption(_("Enable link-time code generation"), wxT_2("/GL"), _("Code generation"), wxT_2(""), true, wxT_2("/Zi /ZI"), _("Link-time code generation is incompatible with debugging info"));
    m_Options.AddOption(_("Optimize for windows application"), wxT_2("/GA"), _("Code generation"));
    //m_Options.AddOption(_("Force stack checking for all funcs"), wxT_2("/Ge"), _("Code generation")); // Deprecated in MSVC 8
    // /Gs[num] control stack checking calls
    m_Options.AddOption(_("Enable _penter function call"), wxT_2("/Gh"), _("Code generation"));
    m_Options.AddOption(_("Enable _pexit function call"), wxT_2("/GH"), _("Code generation"));
    m_Options.AddOption(_("Generate fiber-safe TLS accesses"), wxT_2("/GT"), _("Code generation"));
    m_Options.AddOption(_("Enable fast checks (/RTCsu)"), wxT_2("/RTC1"), _("Code generation"));
    m_Options.AddOption(_("Convert to smaller type checks"), wxT_2("/RTCc"), _("Code generation"));
    m_Options.AddOption(_("Stack Frame runtime checking"), wxT_2("/RTCs"), _("Code generation"));
    m_Options.AddOption(_("Uninitialized local usage checks"), wxT_2("/RTCu"), _("Code generation"));
    // /clr[:option] compile for common language runtime, where option is:
    //  pure - produce IL-only output file (no native executable code)
    //  safe - produce IL-only verifiable output file
    //  oldSyntax - accept the Managed Extensions syntax from Visual C++ 2002/2003
    //  initialAppDomain - enable initial AppDomain behavior of Visual C++ 2002
    //  noAssembly - do not produce an assembly
    m_Options.AddOption(_("__cdecl calling convention"), wxT_2("/Gd"), _("Code generation"));
    m_Options.AddOption(_("__fastcall calling convention"), wxT_2("/Gr"), _("Code generation"));
    m_Options.AddOption(_("__stdcall calling convention"), wxT_2("/Gz"), _("Code generation"));
    //m_Options.AddOption(_("use FIST instead of ftol("), wxT_2("/QIfist"), _("Code generation")); // Deprecated in MSVC 8
    //m_Options.AddOption(_("Ensure function padding for hotpatchable images"), wxT_2("/hotpatch"), _("Code generation"));
    m_Options.AddOption(_("Enable SSE instruction set"), wxT_2("/arch:SSE"), _("Code generation"));
    m_Options.AddOption(_("Enable SSE2 instruction set"), wxT_2("/arch:SSE2"), _("Code generation"));

    //Misc
    m_Options.AddOption(_("Default char type is unsigned"), wxT_2("/J"), _("Other"));
    m_Options.AddOption(_("Compile all files as .c"), wxT_2("/TC"), _("Other"));
    m_Options.AddOption(_("Compile all files as .cpp"), wxT_2("/TP"), _("Other"));

    // Added Runtime options for cl.exe, that is the runtime library selection
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

void CompilerMSVC8::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\) :[ \t]([Ww]arning[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\) :[ \t](.*[Ee]rror[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")[ \t]+:[ \t]+(.*warning LNK[0-9]+.*)"), 2, 1, 0));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")[ \t]+:[ \t]+(.*error LNK[0-9]+.*)"), 2, 1, 0));
}

AutoDetectResult CompilerMSVC8::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    wxString idepath;

    // Read the VCToolkitInstallDir environment variable
    wxGetEnv(wxT_2("VS90COMNTOOLS"), &m_MasterPath);
    if(m_MasterPath.IsEmpty())
    {
        wxGetEnv(wxT_2("VS80COMNTOOLS"), &m_MasterPath);
    }

    if ( !m_MasterPath.IsEmpty() )
    {
        wxFileName name = wxFileName::DirName(m_MasterPath);

        name.RemoveLastDir();
        name.AppendDir(wxT_2("IDE"));
        idepath = name.GetPath();
        if ( !wxDirExists(idepath) )
            idepath = wxT_2("");

        name.RemoveLastDir();
        name.RemoveLastDir();
        name.AppendDir(wxT_2("VC"));
        m_MasterPath = name.GetPath();
        if ( !wxDirExists(m_MasterPath) )
            m_MasterPath = wxT_2("");
    }

    if (m_MasterPath.IsEmpty())
    {
        // just a guess; the default installation dir
        wxString Programs = wxT_2("C:\\Program Files");
        // what's the "Program Files" location
        // TO DO : support 64 bit ->    32 bit apps are in "ProgramFiles(x86)"
        //                              64 bit apps are in "ProgramFiles"
        wxGetEnv(wxT_2("ProgramFiles"), &Programs);
        m_MasterPath = Programs + wxT_2("\\Microsoft Visual Studio 9.0\\VC");
        idepath = Programs + wxT_2("\\Microsoft Visual Studio 9.0\\Common7\\IDE");
        if(!wxDirExists(m_MasterPath))
        {
            m_MasterPath = Programs + wxT_2("\\Microsoft Visual Studio 8\\VC");
            idepath = Programs + wxT_2("\\Microsoft Visual Studio 8\\Common7\\IDE");
        }
    }

    if (!m_MasterPath.IsEmpty())
    {
        wxRegKey key; // defaults to HKCR
        bool sdkfound = false;
        wxString dir;

        // we need to add the IDE path, as the compiler requires some DLL present there
        m_ExtraPaths.Add(idepath);

        // try to detect Platform SDK (old versions)
        key.SetName(wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\Win32SDK\\Directories"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            key.QueryValue(wxT_2("Install Dir"), dir);
            if (!dir.IsEmpty() && wxDirExists(dir))
                sdkfound = true;
            key.Close();
        }

        // try to detect Platform SDK (newer versions)
        wxString msPsdkKeyName[2] = { wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\MicrosoftSDK\\InstalledSDKs"),
                                      wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\Microsoft SDKs\\Windows") };
        wxString msPsdkKeyValue[2] = { wxT_2("Install Dir"), wxT_2("InstallationFolder") };
        for (int i = 0; i < 2; ++i)
        {
            key.SetName(msPsdkKeyName[i]);
            if (!sdkfound && key.Exists() && key.Open(wxRegKey::Read))
            {
                wxString name;
                long idx;
                bool cont = key.GetFirstKey(name, idx);

                while(cont)
                {
                    wxRegKey subkey(key.GetName(), name);

                    if (subkey.Open(wxRegKey::Read) &&
                        (subkey.QueryValue(msPsdkKeyValue[i], dir), !dir.IsEmpty()) &&
                        wxDirExists(dir))
                    {
                        sdkfound = true;
                        cont = false;
                    }
                    else
                        cont = key.GetNextKey(name, idx);

                    subkey.Close();
                }
                key.Close();
            }

            if (sdkfound)
                break;
        }

        // add include dirs for MS Platform SDK too (let them come before compiler's path)
        if (sdkfound)
        {
            if (dir.GetChar(dir.Length() - 1) != '\\')
                dir += sep;
            AddIncludeDir(dir + wxT_2("include"));
            AddResourceIncludeDir(dir + wxT_2("include"));
            AddLibDir(dir + wxT_2("lib"));
            m_ExtraPaths.Add(dir + wxT_2("bin"));
        }

        // now the compiler's include directories
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));
        AddResourceIncludeDir(m_MasterPath + sep + wxT_2("include"));

        // add extra paths for "Debugging tools" too
        key.SetName(wxT_2("HKEY_CURRENT_USER\\Software\\Microsoft\\DebuggingTools"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            key.QueryValue(wxT_2("WinDbg"), dir);
            if (!dir.IsEmpty() && wxDirExists(dir))
            {
                if (dir.GetChar(dir.Length() - 1) == '\\')
                    dir.Remove(dir.Length() - 1, 1);
                m_ExtraPaths.Add(dir);
            }
        }
        key.Close();
    }

    return wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
