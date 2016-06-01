/********************************************************************
    filename:        compilerICC.cpp
    created by:    Yorgos Pagles (yop [at] protiamail.gr)
    description:    Support of Intel's ICC compiler for CodeBlocks IDE
********************************************************************/

#include <sdk.h>
#include <prep.h>
#include "compilerICC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include "manager.h"
#include "logmanager.h"

CompilerICC::CompilerICC()
    : Compiler(_("Intel C/C++ Compiler"), wxT_2("icc"))
{
    Reset();
}

CompilerICC::~CompilerICC()
{
    //dtor
}

Compiler * CompilerICC::CreateCopy()
{
    Compiler* c = new CompilerICC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerICC::Reset()
{
    if (platform::windows)
    {
        // Looks alot like the msvc compiler. Needs sdk as the msvc does
        m_Programs.C = wxT_2("icl.exe");
        m_Programs.CPP = wxT_2("icl.exe");
        m_Programs.LD = wxT_2("xilink.exe"); //Runs Microsoft's link.exe
        m_Programs.DBG = wxT_2("idb.exe");
        m_Programs.LIB = wxT_2("xilink.exe");
        m_Programs.WINDRES = wxT_2("rc.exe"); // platform SDK is needed for this
        m_Programs.MAKE = wxT_2("mingw32-make.exe");//it works with nmake as well but cb doesn't

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

        // Intel Compiler Options from the supplied with the compiler documentation (man pages and docs)
        // NOTE(yop): I have been as descriptive as possible, producing very long descriptions for
        //            each option. Should this change to something more compact but less informative?
        //            These are not the most common options but they are from my point of view some
        //            of the most usefull ones. Some of them should be removed to conform with the
        //            rest of the supported compilers layout of options (that are significantly less).
        m_Options.ClearOptions();
        // Debug and Profile options
        wxString category = _("Output and Debug");
        m_Options.AddOption(_("Trap uninitialized variables"), wxT_2("/Qtrapuv"), category);
        // This is a tricky one. I see precompiled headers enabled by default in RC2 but each compiler
        // takes different flags to generate them. Shouldn't the functionality of precompiled headers
        // move here? This option produces a precompiled header during first compilation and uses the
        // produced one for the next ones.
        m_Options.AddOption(_("Enable automatic precompiled header file creation/usage"), wxT_2("/YX"), category);
        m_Options.AddOption(_("Perform syntax and semantic checking only (no object file produced)"), wxT_2("/Zs"), category);
        m_Options.AddOption(_("Produce symbolic debug information in object files (you should avoid using any optimizations)"), wxT_2("/Zi"), category);

        // Compiler Diagnostics
        category = _("Compiler Diagnostics (some options overide each other)");
        m_Options.AddOption(_("Only display errors"), wxT_2("-W0"), category);
        m_Options.AddOption(_("Enable more strict diagnostics"), wxT_2("-Wcheck"), category);
        m_Options.AddOption(_("Force warnings to be reported as errors"), wxT_2("-WX"), category);
        m_Options.AddOption(_("Print brief one-line diagnostics"), wxT_2("-WL"), category);
        m_Options.AddOption(_("Enable all compiler diagnostics"), wxT_2("-Wall"), category);
        m_Options.AddOption(_("Issue portability diagnostics"), wxT_2("-Wport"), category);
        m_Options.AddOption(_("Print diagnostics for 64-bit porting"), wxT_2("-Wp64"), category);

        // Performance
        category = _("Performance");
        m_Options.AddOption(_("Disable optimizations"), wxT_2("/Od"), category);
        m_Options.AddOption(_("Optimize for maximum speed, but disable some optimizations which increase code size for a small speed benefit."), wxT_2("/O1"), category);
        m_Options.AddOption(_("Enable optimizations"), wxT_2("/O2"), category);
        m_Options.AddOption(_("Enable -O2 plus more aggressive optimizations that may not improve performance for all programs"), wxT_2("/O3"), category);
        m_Options.AddOption(_("Enable speed optimizations, but disable some optimizations which increase code size for small speed benefit"), wxT_2("/Os"), category);
        m_Options.AddOption(_("Enable -xP -O3 -ipo -no-prec-div -static"), wxT_2("/fast"), category);
        m_Options.AddOption(_("Disable inlining"), wxT_2("/Ob0"), category);
        m_Options.AddOption(_("Inline functions declared with __inline, and perform C++ inlining"), wxT_2("/Ob1"), category);
        m_Options.AddOption(_("Inline any function, at the compiler's discretion"), wxT_2("/Ob2"), category);
        m_Options.AddOption(_("Assume no aliasing in program"), wxT_2("/Oa"), category);
        m_Options.AddOption(_("Assume no aliasing within functions, but assume aliasing across calls"), wxT_2("/Ow"), category);
        m_Options.AddOption(_("Maintain floating point precision (disables some optimizations)"), wxT_2("/Op"), category);
        m_Options.AddOption(_("Improve floating-point precision (speed impact is less than -mp)"), wxT_2("/Qprec"), category);
        m_Options.AddOption(_("Disable using EBP as general purpose register"), wxT_2("-Oy"), category);
        m_Options.AddOption(_("Improve precision of floating-point divides (some speed impact)"), wxT_2("/Qprec-div"), category);
        m_Options.AddOption(_("Determine if certain square root optimizations are enabled"), wxT_2("/Qprec-sqrt"), category);
        m_Options.AddOption(_("Round fp results at assignments & casts (some speed impact)"), wxT_2("/Qfp-port"), category);
        m_Options.AddOption(_("Enable fp stack checking after every function/procedure call"), wxT_2("/Qfpstkchk"), category);
        m_Options.AddOption(_("Rounding mode to enable fast float-to-int conversions"), wxT_2("/Qrcd"), category);
        m_Options.AddOption(_("Optimize specificly for Pentium processor"), wxT_2("/G5"), category);
        m_Options.AddOption(_("Optimize specificly for Pentium Pro, Pentium II and Pentium III processors"), wxT_2("/G6"), category);
        m_Options.AddOption(_("Code is optimized for Intel Pentium III"), wxT_2("/QxK"), category);
        m_Options.AddOption(_("Code is optimized for Intel Pentium 4"), wxT_2("/QxW"), category);
        m_Options.AddOption(_("Code is optimized for Intel Pentium 4 and enables new optimizations"), wxT_2("/QxN"), category);
        m_Options.AddOption(_("Code is optimized for Intel Pentium M"), wxT_2("/QxB"), category);
        m_Options.AddOption(_("Code is optimized for Intel Pentium 4 with SSE3 support"), wxT_2("/QxP"), category);

        // Language
        category = _("Language");
        m_Options.AddOption(_("Enable the 'restrict' keyword for disambiguating pointers"), wxT_2("/Qrestrict"), category);
        m_Options.AddOption(_("Strict ANSI conformance dialects"), wxT_2("/Za"), category);
        m_Options.AddOption(_("Compile all source or unrecognized file types as C++ source files"), wxT_2("/Qc++"), category);
        m_Options.AddOption(_("Disable RTTI support"), wxT_2("/GR-"), category);
        m_Options.AddOption(wxT_2("Process OpenMP directives"), wxT_2("/Qopenmp"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 1"), wxT_2("/Zp1"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 2"), wxT_2("/Zp2"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 4"), wxT_2("/Zp4"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 8"), wxT_2("/Zp8"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 16"), wxT_2("/Zp16"), category);
        m_Options.AddOption(_("Change default char type to unsigned"), wxT_2("/J"), category);

        m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler /nologo $options $includes /c $file /Fo$object")));
        //The rest are part of the microsoft sdk. The xilink.exe calls link.exe eventually.
        m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("$rescomp $res_includes -fo$resource_output $file")));
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker /nologo /subsystem:windows $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")));
        m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")));
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker /dll /nologo $libdirs /out:$exe_output $libs $link_objects $link_resobjects $link_options")));
        m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker /lib /nologo $libdirs /out:$static_output $libs $link_objects $link_resobjects $link_options")));
        m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently
    }
    else
    {
        m_Programs.C = wxT_2("icc");
        m_Programs.CPP = wxT_2("icpc");
        m_Programs.LD = wxT_2("icpc");
        m_Programs.DBG = wxT_2("idb");
        m_Programs.LIB = wxT_2("ar");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make");

        m_Switches.includeDirs = wxT_2("-I");
        m_Switches.libDirs = wxT_2("-L");
        m_Switches.linkLibs = wxT_2("-l");
        m_Switches.defines = wxT_2("-D");
        m_Switches.genericSwitch = wxT_2("-");
        m_Switches.objectExtension = wxT_2("o");
        m_Switches.needDependencies = true;
        m_Switches.forceCompilerUseQuotes = false;
        m_Switches.forceLinkerUseQuotes = false;
        m_Switches.logging = clogSimple;
        m_Switches.libPrefix = wxT_2("lib");
        m_Switches.libExtension = wxT_2("a");
        m_Switches.linkerNeedsLibPrefix = false;
        m_Switches.linkerNeedsLibExtension = false;

        m_Options.ClearOptions();
        // Debug and Profile options
        wxString category = _("Output, Debug and Profile");
        m_Options.AddOption(_("Trap uninitialized variables"), wxT_2("-ftrapuv"), category);
        m_Options.AddOption(_("Enable automatic precompiled header file creation/usage"), wxT_2("-pch"), category);
        m_Options.AddOption(_("Perform syntax and semantic checking only (no object file produced)"), wxT_2("-fsyntax-only"), category);
        m_Options.AddOption(_("Produce symbolic debug information in object files (you should avoid using any optimizations)"), wxT_2("-g"), category);
        m_Options.AddOption(_("Compile and link for function profiling with UNIX gprof tool"), wxT_2("-p"), category);

        // Compiler Diagnostics
        category = _("Compiler Diagnostics (some options overide each other)");
        m_Options.AddOption(_("Only display errors"), wxT_2("-w0"), category);
        m_Options.AddOption(_("Display remarks, warnings, and errors"), wxT_2("-w2"), category);
        m_Options.AddOption(_("Enable more strict diagnostics"), wxT_2("-Wcheck"), category);
        m_Options.AddOption(_("Force warnings to be reported as errors"), wxT_2("-Werror"), category);
        m_Options.AddOption(_("Print brief one-line diagnostics"), wxT_2("-Wbrief"), category);
        m_Options.AddOption(_("Enable all compiler diagnostics"), wxT_2("-Wall"), category);
        m_Options.AddOption(_("Warn for missing prototypes"), wxT_2("-Wmissing-prototypes"), category);
        m_Options.AddOption(_("Warn for questionable pointer arithmetic"), wxT_2("-Wpointer-arith"), category);
        m_Options.AddOption(_("Warn if a variable is used before being initialized"), wxT_2("-Wuninitialized"), category);
        m_Options.AddOption(_("Enable inline diagnostics"), wxT_2("-Winline"), category);
        m_Options.AddOption(_("Print warnings related to deprecated features"), wxT_2("-Wdeprecated"), category);
        m_Options.AddOption(_("Warn if declared function is not used"), wxT_2("-Wunused-function"), category);
        m_Options.AddOption(_("Don't warn if an unknown #pragma directive is used"), wxT_2("-Wno-unknown-pragmas"), category);
        m_Options.AddOption(_("Warn if return type of main is not expected"), wxT_2("-Wmain"), category);
        m_Options.AddOption(_("Warn when /* appears in the middle of a /* */ comment"), wxT_2("-Wcomment"), category);
        m_Options.AddOption(_("Warn when a function uses the default int return type and warn when a return statement is used in a void function"), wxT_2("-Wreturn-type"), category);
        m_Options.AddOption(_("Print diagnostics for 64-bit porting"), wxT_2("-Wp64"), category);

        // Performance
        category = _("Performance");
        m_Options.AddOption(_("Disable optimizations"), wxT_2("-O0"), category);
        m_Options.AddOption(_("Optimize for maximum speed, but disable some optimizations which increase code size for a small speed benefit."), wxT_2("-O1"), category);
        m_Options.AddOption(_("Enable optimizations"), wxT_2("-O2"), category);
        m_Options.AddOption(_("Enable -O2 plus more aggressive optimizations that may not improve performance for all programs"), wxT_2("-O3"), category);
        m_Options.AddOption(_("Enable speed optimizations, but disable some optimizations which increase code size for small speed benefit"), wxT_2("-Os"), category);
        m_Options.AddOption(_("Enable -xP -O3 -ipo -no-prec-div -static"), wxT_2("-fast"), category);
        m_Options.AddOption(_("Disable inlining"), wxT_2("-Ob0"), category);
        m_Options.AddOption(_("Inline functions declared with __inline, and perform C++ inlining"), wxT_2("-Ob1"), category);
        m_Options.AddOption(_("Inline any function, at the compiler's discretion"), wxT_2("-Ob2"), category);
        m_Options.AddOption(_("Assume no aliasing in program"), wxT_2("-fno-alias"), category);
        m_Options.AddOption(_("Assume no aliasing within functions, but assume aliasing across calls"), wxT_2("-fno-fnalias"), category);
        m_Options.AddOption(_("Maintain floating point precision (disables some optimizations)"), wxT_2("-mp"), category);
        m_Options.AddOption(_("Improve floating-point precision (speed impact is less than -mp)"), wxT_2("-mp1"), category);
        m_Options.AddOption(_("Disable using EBP as general purpose register"), wxT_2("-fp"), category);
        m_Options.AddOption(_("Improve precision of floating-point divides (some speed impact)"), wxT_2("-prec-div"), category);
        m_Options.AddOption(_("Determine if certain square root optimizations are enabled"), wxT_2("-prec-sqrt"), category);
        m_Options.AddOption(_("Round fp results at assignments & casts (some speed impact)"), wxT_2("-fp-port"), category);
        m_Options.AddOption(_("Enable fp stack checking after every function/procedure call"), wxT_2("-fpstkchk"), category);
        m_Options.AddOption(_("Rounding mode to enable fast float-to-int conversions"), wxT_2("-rcd"), category);
        m_Options.AddOption(_("Optimize specificly for Pentium processor"), wxT_2("-mtune=pentium"), category);
        m_Options.AddOption(_("Optimize specificly for Pentium Pro, Pentium II and Pentium III processors"), wxT_2("-mtune=pentiumpro"), category);
        m_Options.AddOption(_("Generate code excusively for Pentium Pro and Pentium II processor instructions"), wxT_2("-march=pentiumpro"), category);
        m_Options.AddOption(_("Generate code excusively for MMX instructions"), wxT_2("-march=pentiumii"), category);
        m_Options.AddOption(_("Generate code excusively for streaming SIMD extensions"), wxT_2("-march=pentiumiii"), category);
        m_Options.AddOption(_("Generate code excusively for Pentium 4 New Instructions"), wxT_2("-march=pentium4"), category);
        m_Options.AddOption(_("Generate code for Intel Pentium III and compatible Intel processors"), wxT_2("-msse"), category);
        m_Options.AddOption(_("Generate code for Intel Pentium 4 and compatible Intel processors"), wxT_2("-msse2"), category);
        m_Options.AddOption(_("Generate code for Intel Pentium 4 processors with SSE3 extensions"), wxT_2("-msse3"), category);

        // Language
        category = _("Language");
        m_Options.AddOption(_("Enable the 'restrict' keyword for disambiguating pointers"), wxT_2("-restrict"), category);
        m_Options.AddOption(_("Equivalent to GNU -ansi"), wxT_2("-ansi"), category);
        m_Options.AddOption(_("Strict ANSI conformance dialects"), wxT_2("-strict-ansi"), category);
        m_Options.AddOption(_("Compile all source or unrecognized file types as C++ source files"), wxT_2("-Kc++"), category);
        m_Options.AddOption(_("Disable RTTI support"), wxT_2("-fno-rtti"), category);
        m_Options.AddOption(wxT_2("Process OpenMP directives"), wxT_2("-openmp"), category);
        m_Options.AddOption(_("Analyze and reorder memory layout for variables and arrays"), wxT_2("-align"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 1"), wxT_2("-Zp1"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 2"), wxT_2("-Zp2"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 4"), wxT_2("-Zp4"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 8"), wxT_2("-Zp8"), category);
        m_Options.AddOption(_("Specify alignment constraint for structures to 16"), wxT_2("-Zp16"), category);
        m_Options.AddOption(_("Allocate as many bytes as needed for enumerated types"), wxT_2("-fshort-enums"), category);
        m_Options.AddOption(_("Change default char type to unsigned"), wxT_2("-funsigned-char"), category);
        m_Options.AddOption(_("Change default bitfield type to unsigned"), wxT_2("-funsigned-bitfields"), category);
        m_Options.AddOption(_("Disable support for operator name keywords"), wxT_2("-fno-operator-names"), category);
        m_Options.AddOption(_("Do not recognize 'typeof' as a keyword"), wxT_2("-fno-gnu-keywords"), category);
        m_Options.AddOption(_("Allow for non-conformant code"), wxT_2("-fpermissive"), category);

        m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -o $object")));
        m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(wxT_2("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
        m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
        m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs")));
        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd];
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
        m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -r $static_output $link_objects\n\tranlib $exe_output")));
        m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently
    }

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerICC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Compilation remark"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+).:[ \t]([Rr]emark[ \t]#[0-9]+:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("OpenMP remark"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\):[ \\t]\\(col. ([0-9]+)\\)[ \\t]([Rr]emark:[ \\t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compilation warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+).:[ \t]([Ww]arning[ \t]#[0-9]+:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compilation error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+).:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, wxT_2("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("General error"), cltError, wxT_2("([Ee]rror:[ \t].*)"), 1));
}

AutoDetectResult CompilerICC::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    wxString extraDir = wxT_2("");
    if (platform::windows)
    {
        // Read the ICPP_COMPILER90 environment variable
        wxGetEnv(wxT_2("ICPP_COMPILER90"), &m_MasterPath);
        extraDir = sep + wxT_2("Ia32");// Intel also provides compiler for Itanium processors

        if (m_MasterPath.IsEmpty())
        {
            // just a guess the default installation dir
            wxString Programs = wxT_2("C:\\Program Files");
            // what's the "Program Files" location
            // TO DO : support 64 bit ->    32 bit apps are in "ProgramFiles(x86)"
            //                              64 bit apps are in "ProgramFiles"
            wxGetEnv(wxT_2("ProgramFiles"), &Programs);
            m_MasterPath = Programs + wxT_2("\\Intel\\Compiler\\C++\\9.0");
        }
    }
    else
    {
        m_MasterPath = wxT_2("/opt/intel/cc/9.0");
        if (wxDirExists(wxT_2("/opt/intel")))
        {
            wxDir icc_dir(wxT_2("/opt/intel/cc"));
            if (icc_dir.IsOpened())
            {
                wxArrayString dirs;
                wxIccDirTraverser IccDirTraverser(dirs);
                icc_dir.Traverse(IccDirTraverser);
                if (!dirs.IsEmpty())
                {
                    // Now sort the array in reverse order to get the latest version's path
                    dirs.Sort(true);
                    m_MasterPath = dirs[0];
                }
            }
        }
    }

    AutoDetectResult ret = wxFileExists(m_MasterPath + extraDir + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + extraDir + sep + wxT_2("Include"));
        AddLibDir(m_MasterPath + extraDir + sep + wxT_2("Lib"));
    }
    // Try to detect the debugger. If not detected succesfully the debugger plugin will
    // complain, so only the autodetection of compiler is considered in return value
    wxString path;
    if (platform::windows)
    {
        wxGetEnv(wxT_2("IDB_PATH"), &path);
        path += wxT_2("IDB\\9.0\\IA32");
    }
    else
    {
        path= wxT_2("/opt/intel/idb/9.0");
        if (wxDirExists(wxT_2("/opt/intel")))
        {
            wxDir icc_debug_dir(wxT_2("/opt/intel/idb"));
            if (icc_debug_dir.IsOpened())
            {
                wxArrayString debug_dirs;
                wxIccDirTraverser IccDebugDirTraverser(debug_dirs);
                icc_debug_dir.Traverse(IccDebugDirTraverser);
                if (!debug_dirs.IsEmpty())
                {
                    // Now sort the array in reverse order to get the latest version's path
                    debug_dirs.Sort(true);
                    path = debug_dirs[0];
                }
            }
        }
    }

    if (wxFileExists(path + sep + wxT_2("bin") + sep + m_Programs.DBG))
        m_ExtraPaths.Add(path);

    return ret;
}
