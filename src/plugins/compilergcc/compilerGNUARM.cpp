/* compilerMINGW.cpp
 * $Id: compilerMINGW.cpp 1429 2005-12-02 23:25:50Z mandrav $
 */

#include "ca/sdk.h"
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/utils.h> // wxGetOSDirectory, wxGetEnv
#endif
#include <wx/filefn.h> // wxFileExists
#include <wx/fileconf.h> // wxFileConfig
#include "compilerGNUARM.h"

CompilerGNUARM::CompilerGNUARM()
    : Compiler(_("GNU ARM GCC Compiler"),wxT_2("arm-elf-gcc"))
{
    Reset();
}

CompilerGNUARM::~CompilerGNUARM()
{
    //dtor
}

Compiler * CompilerGNUARM::CreateCopy()
{
    Compiler* c = new CompilerGNUARM(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGNUARM::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("arm-elf-gcc.exe");
        m_Programs.CPP = wxT_2("arm-elf-g++.exe");
        m_Programs.LD = wxT_2("arm-elf-g++.exe");
        m_Programs.DBG = wxT_2("arm-elf-gdb.exe");
        m_Programs.LIB = wxT_2("arm-elf-ar.exe");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("arm-elf-gcc");
        m_Programs.CPP = wxT_2("arm-elf-g++");
        m_Programs.LD = wxT_2("arm-elf-g++");
        m_Programs.DBG = wxT_2("arm-elf-gdb");
        m_Programs.LIB = wxT_2("arm-elf-ar");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make");
    }
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

    // Summary of GCC options: http://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

    m_Options.ClearOptions();
    m_Options.AddOption(_("Produce debugging symbols"),
                wxT_2("-g"),
                _("Debugging"),
                wxT_2(""),
                true,
                wxT_2("-O -O1 -O2 -O3 -Os"),
                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));

    wxString gprof_link = wxT_2("-pg");
    if (platform::windows)
        gprof_link = wxT_2("-pg -lgmon");
    m_Options.AddOption(_("Profile code when executed"), wxT_2("-pg"), _("Profiling"), gprof_link);

    wxString category = _("Warnings");

    // warnings
    m_Options.AddOption(_("In C mode, support all ISO C90 programs. In C++ mode, remove GNU extensions that conflict with ISO C++"), wxT_2("-ansi"), category);
    m_Options.AddOption(_("Enable all compiler warnings (overrides every other setting)"), wxT_2("-Wall"), category);
    m_Options.AddOption(_("Enable standard compiler warnings"), wxT_2("-W"), category);
    m_Options.AddOption(_("Stop compiling after first error"), wxT_2("-Wfatal-errors"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), wxT_2("-w"), category);
    m_Options.AddOption(_("Enable warnings demanded by strict ISO C and ISO C++"), wxT_2("-pedantic"), category);
    m_Options.AddOption(_("Treat as errors the warnings demanded by strict ISO C and ISO C++"), wxT_2("-pedantic-errors"), category);
    m_Options.AddOption(_("Warn if main() is not conformant"), wxT_2("-Wmain"), category);
    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), wxT_2(""), category, wxT_2("-s"), true, wxT_2("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), wxT_2("-O"), category);
    m_Options.AddOption(_("Optimize more (for speed)"), wxT_2("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), wxT_2("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), wxT_2("-O3"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), wxT_2("-Os"), category);
    m_Options.AddOption(_("Expensive optimizations"), wxT_2("-fexpensive-optimizations"), category);
    // machine dependent options - cpu arch
    category = _("ARM CPU architecture specific");
    m_Options.AddOption(_("-mapcs-frame"), wxT_2("-mapcs-frame"), category);
    m_Options.AddOption(_("-mno-apcs-frame"), wxT_2("-mno-apcs-frame"), category);
    m_Options.AddOption(_("-mabi=NAME"), wxT_2("-mabi=NAME"), category);
    m_Options.AddOption(_("-mapcs-stack-check"), wxT_2("-mapcs-stack-check"), category);
    m_Options.AddOption(_("-mno-apcs-stack-check"), wxT_2("-mno-apcs-stack-check"), category);
    m_Options.AddOption(_("-mapcs-float"), wxT_2("-mapcs-float"), category);
    m_Options.AddOption(_("-mno-apcs-float"), wxT_2("-mno-apcs-float"), category);
    m_Options.AddOption(_("-mapcs-reentrant"), wxT_2("-mapcs-reentrant"), category);
    m_Options.AddOption(_("-mno-apcs-reentrant"), wxT_2("-mno-apcs-reentrant"), category);
    m_Options.AddOption(_("-msched-prolog"), wxT_2("-msched-prolog"), category);
    m_Options.AddOption(_("-mno-sched-prolog"), wxT_2("-mno-sched-prolog"), category);
    m_Options.AddOption(_("-mlittle-endian"), wxT_2("-mlittle-endian"), category);
    m_Options.AddOption(_("-mbig-endian"), wxT_2("-mbig-endian"), category);
    m_Options.AddOption(_("-mwords-little-endian"), wxT_2("-mwords-little-endian"), category);
    m_Options.AddOption(_("-mfloat-abi=NAME"), wxT_2("-mfloat-abi=NAME"), category);
    m_Options.AddOption(_("-msoft-float"), wxT_2("-msoft-float"), category);
    m_Options.AddOption(_("-mhard-float"), wxT_2("-mhard-float"), category);
    m_Options.AddOption(_("-mfpe"), wxT_2("-mfpe"), category);
    m_Options.AddOption(_("-mthumb-interwork"), wxT_2("-mthumb-interwork"), category);
    m_Options.AddOption(_("-mno-thumb-interwork"), wxT_2("-mno-thumb-interwork"), category);
    m_Options.AddOption(_("-mcpu=NAME"), wxT_2("-mcpu=NAME"), category);
    m_Options.AddOption(_("-march=NAME"), wxT_2("-march=NAME"), category);
    m_Options.AddOption(_("-mfpu=NAME"), wxT_2("-mfpu=NAME"), category);
    m_Options.AddOption(_("-mstructure-size-boundary=N"), wxT_2("-mstructure-size-boundary=N"), category);
    m_Options.AddOption(_("-mabort-on-noreturn"), wxT_2("-mabort-on-noreturn"), category);
    m_Options.AddOption(_("-mlong-calls"), wxT_2("-mlong-calls"), category);
    m_Options.AddOption(_("-mno-long-calls"), wxT_2("-mno-long-calls"), category);
    m_Options.AddOption(_("-msingle-pic-base"), wxT_2("-msingle-pic-base"), category);
    m_Options.AddOption(_("-mno-single-pic-base"), wxT_2("-mno-single-pic-base"), category);
    m_Options.AddOption(_("-mpic-register=REG"), wxT_2("-mpic-register=REG"), category);
    m_Options.AddOption(_("-mnop-fun-dllimport"), wxT_2("-mnop-fun-dllimport"), category);
    m_Options.AddOption(_("-mcirrus-fix-invalid-insns"), wxT_2("-mcirrus-fix-invalid-insns"), category);
    m_Options.AddOption(_("-mno-cirrus-fix-invalid-insns"), wxT_2("-mno-cirrus-fix-invalid-insns"), category);
    m_Options.AddOption(_("-mpoke-function-name"), wxT_2("-mpoke-function-name"), category);
    m_Options.AddOption(_("-mthumb"), wxT_2("-mthumb"), category);
    m_Options.AddOption(_("-marm"), wxT_2("-marm"), category);
    m_Options.AddOption(_("-mtpcs-frame"), wxT_2("-mtpcs-frame"), category);
    m_Options.AddOption(_("-mtpcs-leaf-frame"), wxT_2("-mtpcs-leaf-frame"), category);
    m_Options.AddOption(_("-mcaller-super-interworking"), wxT_2("-mcaller-super-interworking"), category);
    m_Options.AddOption(_("-mcallee-super-interworking"), wxT_2("-mcallee-super-interworking"), category);

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(wxT_2("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs")));
    if (platform::windows)
    {
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -mwindows")));
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    else
    {
        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -r -s $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
} // end of Reset

void CompilerGNUARM::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'In function...' info"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):[ \t]+") + wxT_2("([iI]n ([cC]lass|[cC]onstructor|[dD]estructor|[fF]unction|[mM]ember [fF]unction).*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from' info"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]+([iI]nstantiated from .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, wxT_2("windres.exe:[ \t](") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, wxT_2("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler note"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Nn]ote:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General note"), cltInfo, wxT_2("([Nn]ote:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + wxT_2("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, wxT_2(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, wxT_2("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Auto-import info"), cltInfo, wxT_2("([Ii]nfo:[ \t].*)\\(auto-import\\)"), 1));
}

AutoDetectResult CompilerGNUARM::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
    {
        // Search for GNUARM installation dir
        wxString windir = wxGetOSDirectory();
        wxFileConfig ini(wxT_2(""), wxT_2(""), windir + wxT_2("/GnuARM.ini"), wxT_2(""), wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
        // need it as const , so correct overloaded method will be selected
        wxString Programs = wxT_2("C:\\Program Files");
        // what's the "Program Files" location
        // TO DO : support 64 bit ->    32 bit apps are in "ProgramFiles(x86)"
        //                              64 bit apps are in "ProgramFiles"
        wxGetEnv(wxT_2("ProgramFiles"), &Programs);
        // need it as const , so correct overloaded method will be selected
        const wxString ProgramsConst = Programs + wxT_2("\\GNUARM");
        m_MasterPath = ini.Read(wxT_2("/InstallSettings/InstallPath"), ProgramsConst);

        if (wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C))
        {
            m_Programs.MAKE = wxT_2("make.exe"); // we distribute "make" not "mingw32-make"
        }
    }
    else
        m_MasterPath = wxT_2("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));
    }
    return ret;
} // end of AutoDetectInstallationDir
