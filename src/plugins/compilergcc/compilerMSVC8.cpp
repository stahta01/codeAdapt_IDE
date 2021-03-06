#ifdef __WXMSW__
// this compiler is valid only in windows

#include <sdk.h>
#include "compilerMSVC8.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/msw/registry.h>

CompilerMSVC8::CompilerMSVC8()
    : Compiler(_("Microsoft Visual C++ 2005/2008"), _T("msvc8"))
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
    m_Programs.C = _T("cl.exe");
    m_Programs.CPP = _T("cl.exe");
    m_Programs.LD = _T("link.exe");
    m_Programs.LIB = _T("link.exe");
    m_Programs.WINDRES = _T("rc.exe");
    m_Programs.MAKE = _T("nmake.exe");
    m_Programs.DBG = _T("cdb.exe");

    m_Switches.includeDirs = _T("/I");
    m_Switches.libDirs = _T("/LIBPATH:");
    m_Switches.linkLibs = _T("");
    m_Switches.defines = _T("/D");
    m_Switches.genericSwitch = _T("/");
    m_Switches.objectExtension = _T("obj");
    m_Switches.needDependencies = false;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = clogNone;
    m_Switches.libPrefix = _T("");
    m_Switches.libExtension = _T("lib");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();

    //Language
    m_Options.AddOption(_("Produce debugging symbols"),
                        _T("/Zi /D_DEBUG"),
                        _("Language"),
                        _T("/DEBUG"),
                        true,
                        _T("/Og /O1 /O2 /Os /Ot /Ox /NDEBUG"),
                        _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));
    m_Options.AddOption(_("Disable extensions"), _T("/Za"), _("Language"));
    // /vd{0|1|2} disable/enable vtordisp
    // /vm<x> type of pointers to members
    m_Options.AddOption(_("Enforce Standard C++ for scoping rules"), _T("/Zc:forScope"), _("Language"));
    m_Options.AddOption(_("wchar_t is the native type, not a typedef"), _T("/Zc:wchar_t"), _("Language"));
    m_Options.AddOption(_("Enable Edit and Continue debug info"), _T("/ZI"), _("Language"));
    m_Options.AddOption(_("Enable OpenMP 2.0 language extensions"), _T("/openmp"), _("Language"));

    //Warnings
    m_Options.AddOption(_("Disable all warnings"), _T("/w"), _("Warnings"));
    m_Options.AddOption(_("Enable all compiler warnings"), _T("/Wall"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 1"), _T("/W1"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 2"), _T("/W2"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 3"), _T("/W3"), _("Warnings"));
    m_Options.AddOption(_("Enable warnings level 4"), _T("/W4"), _("Warnings"));
    m_Options.AddOption(_("Enable one line diagnostics"), _T("/WL"), _("Warnings"));
    m_Options.AddOption(_("Enable 64bit porting warnings"), _T("/Wp64"), _("Warnings"));
    m_Options.AddOption(_("Treat warnings as errors"), _T("/WX"), _("Warnings"));

    //Optimization
    //m_Options.AddOption(_("Enable global optimization"), _T("/Og"), _("Optimization")); // Deprecated in MSVC 8
    m_Options.AddOption(_("Maximum optimization (no need for other options)"), _T("/Ox"), _("Optimization"));
    m_Options.AddOption(_("Disable optimizations"), _T("/Od"), _("Optimization")); //added no optimization
    m_Options.AddOption(_("Minimize space"), _T("/O1"), _("Optimization"));
    m_Options.AddOption(_("Maximize speed"), _T("/O2"), _("Optimization"));
    m_Options.AddOption(_("Favor code space"), _T("/Os"), _("Optimization"));
    m_Options.AddOption(_("Favor code speed"), _T("/Ot"), _("Optimization"));
    m_Options.AddOption(_("Enable intrinsic functions"), _T("/Oi"), _("Optimization"));
    m_Options.AddOption(_("Enable frame pointer omission"), _T("/Oy"), _("Optimization"));
    m_Options.AddOption(_("Inline expansion"), _T("/Ob"), _("Optimization"));

    //Code generation
    m_Options.AddOption(_("Enable read-only string pooling"), _T("/GF"), _("Code generation"));
    m_Options.AddOption(_("Separate functions for linker"), _T("/Gy"), _("Code generation"));
    m_Options.AddOption(_("Enable security checks"), _T("/GS"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ RTTI"), _T("/GR"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ exception handling (no SEH)"), _T("/EHs"), _("Code generation"));
    m_Options.AddOption(_("Enable C++ exception handling (w/ SEH)"), _T("/EHa"), _("Code generation"));
    m_Options.AddOption(_("extern \"C\" defaults to nothrow"), _T("/EHc"), _("Code generation"));
    m_Options.AddOption(_("Consider floating-point exceptions when generating code"), _T("/fp:except"), _("Code generation"));
    m_Options.AddOption(_("Do not consider floating-point exceptions when generating code"), _T("/fp:except-"), _("Code generation"));
    m_Options.AddOption(_("\"fast\" floating-point model; results are less predictable"), _T("/fp:fast"), _("Code generation"));
    m_Options.AddOption(_("\"precise\" floating-point model; results are predictable"), _T("/fp:precise"), _("Code generation"));
    m_Options.AddOption(_("\"strict\" floating-point model (implies /fp:except)"), _T("/fp:strict"), _("Code generation"));
    m_Options.AddOption(_("Enable minimal rebuild"), _T("/Gm"), _("Code generation"));
    m_Options.AddOption(_("Enable link-time code generation"), _T("/GL"), _("Code generation"), _T(""), true, _T("/Zi /ZI"), _("Link-time code generation is incompatible with debugging info"));
    m_Options.AddOption(_("Optimize for windows application"), _T("/GA"), _("Code generation"));
    //m_Options.AddOption(_("Force stack checking for all funcs"), _T("/Ge"), _("Code generation")); // Deprecated in MSVC 8
    // /Gs[num] control stack checking calls
    m_Options.AddOption(_("Enable _penter function call"), _T("/Gh"), _("Code generation"));
    m_Options.AddOption(_("Enable _pexit function call"), _T("/GH"), _("Code generation"));
    m_Options.AddOption(_("Generate fiber-safe TLS accesses"), _T("/GT"), _("Code generation"));
    m_Options.AddOption(_("Enable fast checks (/RTCsu)"), _T("/RTC1"), _("Code generation"));
    m_Options.AddOption(_("Convert to smaller type checks"), _T("/RTCc"), _("Code generation"));
    m_Options.AddOption(_("Stack Frame runtime checking"), _T("/RTCs"), _("Code generation"));
    m_Options.AddOption(_("Uninitialized local usage checks"), _T("/RTCu"), _("Code generation"));
    // /clr[:option] compile for common language runtime, where option is:
    //  pure - produce IL-only output file (no native executable code)
    //  safe - produce IL-only verifiable output file
    //  oldSyntax - accept the Managed Extensions syntax from Visual C++ 2002/2003
    //  initialAppDomain - enable initial AppDomain behavior of Visual C++ 2002
    //  noAssembly - do not produce an assembly
    m_Options.AddOption(_("__cdecl calling convention"), _T("/Gd"), _("Code generation"));
    m_Options.AddOption(_("__fastcall calling convention"), _T("/Gr"), _("Code generation"));
    m_Options.AddOption(_("__stdcall calling convention"), _T("/Gz"), _("Code generation"));
    //m_Options.AddOption(_("use FIST instead of ftol("), _T("/QIfist"), _("Code generation")); // Deprecated in MSVC 8
    //m_Options.AddOption(_("Ensure function padding for hotpatchable images"), _T("/hotpatch"), _("Code generation"));
    m_Options.AddOption(_("Enable SSE instruction set"), _T("/arch:SSE"), _("Code generation"));
    m_Options.AddOption(_("Enable SSE2 instruction set"), _T("/arch:SSE2"), _("Code generation"));

    //Misc
    m_Options.AddOption(_("Default char type is unsigned"), _T("/J"), _("Other"));
    m_Options.AddOption(_("Compile all files as .c"), _T("/TC"), _("Other"));
    m_Options.AddOption(_("Compile all files as .cpp"), _T("/TP"), _("Other"));

    // Added Runtime options for cl.exe, that is the runtime library selection
    m_Options.AddOption(_("Multi-threaded Runtime Library"), _T("/MT"), _("Runtime"), _T(""), true);
    m_Options.AddOption(_("Multi-threaded Debug Runtime Library"), _T("/MTd"), _("Runtime"));
    m_Options.AddOption(_("Multi-threaded DLL Runtime Library"), _T("/MD"), _("Runtime"));
    m_Options.AddOption(_("Multi-threaded DLL Debug Runtime Library"), _T("/MDd"), _("Runtime"));


    m_Commands[(int)ctCompileObjectCmd].push_back( CompilerTool(_T("$compiler /nologo $options $includes /c $file /Fo$object")) );
    m_Commands[(int)ctCompileResourceCmd].push_back( CompilerTool(_T("$rescomp $res_includes -fo$resource_output $file")) );
    m_Commands[(int)ctLinkExeCmd].push_back( CompilerTool(_T("$linker /nologo /subsystem:windows $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkConsoleExeCmd].push_back( CompilerTool(_T("$linker /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkDynamicCmd].push_back( CompilerTool(_T("$linker /dll /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkStaticCmd].push_back( CompilerTool(_T("$lib_linker /lib /nologo $libdirs /out:$static_output $libs $link_objects $link_resobjects $link_options")) );
    m_Commands[(int)ctLinkNativeCmd].push_back( CompilerTool(_T("$linker /nologo /subsystem:native $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")) );

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
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, _T("(") + FilePathWithSpaces + _T(")\\(([0-9]+)\\) :[ \t]([Ww]arning[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, _T("(") + FilePathWithSpaces + _T(")\\(([0-9]+)\\) :[ \t](.*[Ee]rror[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltWarning, _T("(") + FilePathWithSpaces + _T(")[ \t]+:[ \t]+(.*warning LNK[0-9]+.*)"), 2, 1, 0));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, _T("(") + FilePathWithSpaces + _T(")[ \t]+:[ \t]+(.*error LNK[0-9]+.*)"), 2, 1, 0));
}

#endif // __WXMSW__
