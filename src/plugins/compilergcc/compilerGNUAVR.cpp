#include <sdk.h>
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
#endif
#include "compilerGNUAVR.h"

CompilerGNUAVR::CompilerGNUAVR()
    : Compiler(_("GNU AVR GCC Compiler"),wxT_2("avr-gcc"))
{
    Reset();
}

CompilerGNUAVR::~CompilerGNUAVR()
{
    //dtor
}

Compiler * CompilerGNUAVR::CreateCopy()
{
    Compiler* c = new CompilerGNUAVR(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
} // end of CreateCopy

void CompilerGNUAVR::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("avr-gcc.exe");
        m_Programs.CPP = wxT_2("avr-g++.exe");
        m_Programs.LD = wxT_2("avr-g++.exe");
        m_Programs.DBG = wxT_2("avr-gdb.exe");
        m_Programs.LIB = wxT_2("avr-ar.exe");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("avr-gcc");
        m_Programs.CPP = wxT_2("avr-g++");
        m_Programs.LD = wxT_2("avr-g++");
        m_Programs.DBG = wxT_2("avr-gdb");
        m_Programs.LIB = wxT_2("avr-ar");
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
    category = _("AVR CPU architecture specific");
    m_Options.AddOption(_("AVR 1 architecture (only assembler)"), wxT_2("-mmcu=avr1"), category);
    m_Options.AddOption(_("AT90S1200 (only assembler)"), wxT_2("-mmcu=at90s1200"), category);
    m_Options.AddOption(_("ATtiny11 (only assembler)"), wxT_2("-mmcu=attiny11"), category);
    m_Options.AddOption(_("ATtiny12 (only assembler)"), wxT_2("-mmcu=attiny12"), category);
    m_Options.AddOption(_("ATtiny15 (only assembler)"), wxT_2("-mmcu=attiny15"), category);
    m_Options.AddOption(_("ATtiny28 (only assembler)"), wxT_2("-mmcu=attiny28"), category);
    m_Options.AddOption(_("AVR 2 archtecture"), wxT_2("-mmcu=avr2"), category);
    m_Options.AddOption(_("AT90S2313"), wxT_2("-mmcu=at90s2313"), category);
    m_Options.AddOption(_("AT90S2323"), wxT_2("-mmcu=at90s2323"), category);
    m_Options.AddOption(_("AT90S2333"), wxT_2("-mmcu=at90s2333"), category);
    m_Options.AddOption(_("AT90S2343"), wxT_2("-mmcu=at90s2343"), category);
    m_Options.AddOption(_("ATtiny22"), wxT_2("-mmcu=attiny22"), category);
    m_Options.AddOption(_("ATtiny26"), wxT_2("-mmcu=attiny26"), category);
    m_Options.AddOption(_("AT90S4414"), wxT_2("-mmcu=at90s4414"), category);
    m_Options.AddOption(_("AT90S4433"), wxT_2("-mmcu=at90s4433"), category);
    m_Options.AddOption(_("AT90S4434"), wxT_2("-mmcu=at90s4434"), category);
    m_Options.AddOption(_("AT90S8515"), wxT_2("-mmcu=at90s8515"), category);
    m_Options.AddOption(_("AT90C8534"), wxT_2("-mmcu=at90c8534"), category);
    m_Options.AddOption(_("AT90s8535"), wxT_2("-mmcu=at90s8535"), category);
    m_Options.AddOption(_("AVR 2.5 architecture"), wxT_2("-mmcu=avr25"), category);
    m_Options.AddOption(_("ATtiny13"), wxT_2("-mmcu=attiny13"), category);
    m_Options.AddOption(_("ATtiny2313"), wxT_2("-mmcu=attiny2313"), category);
    m_Options.AddOption(_("ATtiny24"), wxT_2("-mmcu=attiny24"), category);
    m_Options.AddOption(_("ATtiny44"), wxT_2("-mmcu=attiny44"), category);
    m_Options.AddOption(_("ATtiny84"), wxT_2("-mmcu=attiny84"), category);
    m_Options.AddOption(_("ATtiny25"), wxT_2("-mmcu=attiny25"), category);
    m_Options.AddOption(_("ATtiny45"), wxT_2("-mmcu=attiny45"), category);
    m_Options.AddOption(_("ATtiny85"), wxT_2("-mmcu=attiny85"), category);
    m_Options.AddOption(_("ATtiny261"), wxT_2("-mmcu=attiny261"), category);
    m_Options.AddOption(_("ATtiny461"), wxT_2("-mmcu=attiny461"), category);
    m_Options.AddOption(_("ATtiny861"), wxT_2("-mmcu=attiny861"), category);
    m_Options.AddOption(_("AT86RF401"), wxT_2("-mmcu=at86rf401"), category);
    m_Options.AddOption(_("AVR 3 architecture"), wxT_2("-mmcu=avr3"), category);
    m_Options.AddOption(_("ATmega103"), wxT_2("-mmcu=atmega103"), category);
    m_Options.AddOption(_("ATmega603"), wxT_2("-mmcu=atmega603"), category);
    m_Options.AddOption(_("AT43USB320"), wxT_2("-mmcu=at43usb320"), category);
    m_Options.AddOption(_("AT43USB355"), wxT_2("-mmcu=at43usb355"), category);
    m_Options.AddOption(_("AT76C711"), wxT_2("-mmcu=at76c711"), category);
    m_Options.AddOption(_("AVR 4 architecture"), wxT_2("-mmcu=avr4"), category);
    m_Options.AddOption(_("ATmega8"), wxT_2("-mmcu=atmega8"), category);
    m_Options.AddOption(_("ATmega48"), wxT_2("-mmcu=atmega48"), category);
    m_Options.AddOption(_("ATmega88"), wxT_2("-mmcu=atmega88"), category);
    m_Options.AddOption(_("ATmega8515"), wxT_2("-mmcu=atmega8515"), category);
    m_Options.AddOption(_("ATmega8535"), wxT_2("-mmcu=atmega8535"), category);
    m_Options.AddOption(_("ATmega8HVA"), wxT_2("-mmcu=atmega8hva"), category);
    m_Options.AddOption(_("AT90PWM1"), wxT_2("-mmcu=at90pwm1"), category);
    m_Options.AddOption(_("AT90PWM2"), wxT_2("-mmcu=at90pwm2"), category);
    m_Options.AddOption(_("AT90PWM3"), wxT_2("-mmcu=at90pwm3"), category);
    m_Options.AddOption(_("AVR 5 architecture"), wxT_2("-mmcu=avr5"), category);
    m_Options.AddOption(_("ATmega16"), wxT_2("-mmcu=atmega16"), category);
    m_Options.AddOption(_("ATmega161"), wxT_2("-mmcu=atmega161"), category);
    m_Options.AddOption(_("ATmega163"), wxT_2("-mmcu=atmega163"), category);
    m_Options.AddOption(_("ATmega164P"), wxT_2("-mmcu=atmega164p"), category);
    m_Options.AddOption(_("ATmega165"), wxT_2("-mmcu=atmega165"), category);
    m_Options.AddOption(_("ATmega165P"), wxT_2("-mmcu=atmega165p"), category);
    m_Options.AddOption(_("ATmega168"), wxT_2("-mmcu=atmega168"), category);
    m_Options.AddOption(_("ATmega169"), wxT_2("-mmcu=atmega169"), category);
    m_Options.AddOption(_("ATmega169P"), wxT_2("-mmcu=atmega169p"), category);
    m_Options.AddOption(_("ATmega32"), wxT_2("-mmcu=atmega32"), category);
    m_Options.AddOption(_("ATmega323"), wxT_2("-mmcu=atmega323"), category);
    m_Options.AddOption(_("ATmega324P"), wxT_2("-mmcu=atmega324p"), category);
    m_Options.AddOption(_("ATmega325"), wxT_2("-mmcu=atmega325"), category);
    m_Options.AddOption(_("ATmega325P"), wxT_2("-mmcu=atmega325p"), category);
    m_Options.AddOption(_("ATmega3250"), wxT_2("-mmcu=atmega3250"), category);
    m_Options.AddOption(_("ATmega3250P"), wxT_2("-mmcu=atmega3250p"), category);
    m_Options.AddOption(_("ATmega329"), wxT_2("-mmcu=atmega329"), category);
    m_Options.AddOption(_("ATmega329P"), wxT_2("-mmcu=atmega329p"), category);
    m_Options.AddOption(_("ATmega3290"), wxT_2("-mmcu=atmega3290"), category);
    m_Options.AddOption(_("ATmega3290P"), wxT_2("-mmcu=atmega3290p"), category);
    m_Options.AddOption(_("ATmega406"), wxT_2("-mmcu=atmega406"), category);
    m_Options.AddOption(_("ATmega64"), wxT_2("-mmcu=atmega64"), category);
    m_Options.AddOption(_("ATmega640"), wxT_2("-mmcu=atmega640"), category);
    m_Options.AddOption(_("ATmega644"), wxT_2("-mmcu=atmega644"), category);
    m_Options.AddOption(_("ATmega644P"), wxT_2("-mmcu=atmega644p"), category);
    m_Options.AddOption(_("ATmega645"), wxT_2("-mmcu=atmega645"), category);
    m_Options.AddOption(_("ATmega6450"), wxT_2("-mmcu=atmega6450"), category);
    m_Options.AddOption(_("ATmega649"), wxT_2("-mmcu=atmega649"), category);
    m_Options.AddOption(_("ATmega6490"), wxT_2("-mmcu=atmega6490"), category);
    m_Options.AddOption(_("ATmega128"), wxT_2("-mmcu=atmega128"), category);
    m_Options.AddOption(_("ATmega1280"), wxT_2("-mmcu=atmega1280"), category);
    m_Options.AddOption(_("ATmega1281"), wxT_2("-mmcu=atmega1281"), category);
    m_Options.AddOption(_("ATmega16HVA"), wxT_2("-mmcu=atmega16hva"), category);
    m_Options.AddOption(_("AT90CAN32"), wxT_2("-mmcu=at90can32"), category);
    m_Options.AddOption(_("AT90CAN64"), wxT_2("-mmcu=at90can64"), category);
    m_Options.AddOption(_("AT90CAN128"), wxT_2("-mmcu=at90can128"), category);
    m_Options.AddOption(_("AT90USB82"), wxT_2("-mmcu=at90usb82"), category);
    m_Options.AddOption(_("AT90USB162"), wxT_2("-mmcu=at90usb162"), category);
    m_Options.AddOption(_("AT90USB646"), wxT_2("-mmcu=at90usb646"), category);
    m_Options.AddOption(_("AT90USB647"), wxT_2("-mmcu=at90usb647"), category);
    m_Options.AddOption(_("AT90USB1286"), wxT_2("-mmcu=at90usb1286"), category);
    m_Options.AddOption(_("AT90USB1287"), wxT_2("-mmcu=at90usb1287"), category);
    m_Options.AddOption(_("AT94K"), wxT_2("-mmcu=at94k"), category);
    m_Options.AddOption(_("Output instruction sizes to the asm file"), wxT_2("-msize"), category);
    m_Options.AddOption(_("Initial stack address"), wxT_2("-minit-stack=N"), category);
    m_Options.AddOption(_("Disable interrupts"), wxT_2("-mno-interrupts"), category);
    m_Options.AddOption(_("Expand functions prologues/epilogues"), wxT_2("-mcall-prologues"), category);
    m_Options.AddOption(_("Disable tablejump instructions"), wxT_2("-mno-tablejump"), category);
    m_Options.AddOption(_("8 bits stack pointer"), wxT_2("-mtiny-stack"), category);
    m_Options.AddOption(_("int as 8 bit integer"), wxT_2("-mint8"), category);

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

void CompilerGNUAVR::LoadDefaultRegExArray()
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
} // end of LoadDefaultRegExArray

AutoDetectResult CompilerGNUAVR::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
        m_MasterPath = wxT_2("C:\\WinAVR");
    else
        m_MasterPath = wxT_2("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        if (platform::windows)
        {
            AddIncludeDir(m_MasterPath + sep + wxT_2("avr\\include"));
            AddLibDir(m_MasterPath + sep + wxT_2("avr\\lib"));
            m_ExtraPaths.Add(m_MasterPath + sep + wxT_2("utils") + sep + wxT_2("bin")); // for make
        }
        else
        {
            AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
            AddLibDir(m_MasterPath + sep + wxT_2("lib"));
        }
    }
    return ret;
} // end of AutoDetectInstallationDir
