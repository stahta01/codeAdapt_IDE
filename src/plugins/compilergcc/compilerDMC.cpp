#ifdef __WXMSW__
// this compiler is valid only in windows

#include "ca/sdk.h"
#include "compilerDMC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>

CompilerDMC::CompilerDMC()
    : Compiler(_("Digital Mars Compiler"), wxT_2("dmc"))
{
    Reset();
}

CompilerDMC::~CompilerDMC()
{
	//dtor
}

Compiler * CompilerDMC::CreateCopy()
{
    Compiler* c = new CompilerDMC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerDMC::Reset()
{
	m_Programs.C = wxT_2("dmc.exe");
	m_Programs.CPP = wxT_2("dmc.exe");
	m_Programs.LD = wxT_2("link.exe");
	m_Programs.LIB = wxT_2("lib.exe");
	m_Programs.WINDRES = wxT_2("rcc.exe");
	m_Programs.MAKE = wxT_2("mingw32-make.exe");

	m_Switches.includeDirs = wxT_2("-I");
	m_Switches.libDirs = wxT_2("");
	m_Switches.linkLibs = wxT_2("");
	m_Switches.libPrefix = wxT_2("");
	m_Switches.libExtension = wxT_2("lib");
	m_Switches.defines = wxT_2("-D");
	m_Switches.genericSwitch = wxT_2("-");
	m_Switches.objectExtension = wxT_2("obj");
	m_Switches.needDependencies = false;
	m_Switches.forceCompilerUseQuotes = false;
	m_Switches.forceLinkerUseQuotes = true;
	m_Switches.logging = clogSimple;
	m_Switches.linkerNeedsLibPrefix = false;
	m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();
	m_Options.AddOption(_("Produce debugging symbols"),
				wxT_2("-g"),
				_("Debugging"),
				wxT_2(""),
				true,
				wxT_2("-o -o+space"),
				_("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));

    //
    // TODO (hd#3#): should be simplified
    //

	//. m_Options.AddOption(_("Alignment of struct members"), "-a[1|2|4|8]", _("Architecture"));
	m_Options.AddOption(_("Enforce strict compliance with ANSI C/C++"), wxT_2("-A"), _("C++ Features"));
	m_Options.AddOption(_("Enable new[] and delete[] overloading"), wxT_2("-Aa"), _("C++ Features"));
	m_Options.AddOption(_("Enable bool"), wxT_2("-Ab"), _("C++ Features"));
	m_Options.AddOption(_("Enable exception handling"), wxT_2("-Ae"), _("C++ Features"));
	m_Options.AddOption(_("Enable RTTI"), wxT_2("-Ar"), _("C++ Features"));
	//. m_Options.AddOption(_("Message language: English,French,German,Japanese"), wxT_2("-B[e|f|g|j]"), _("Others"));
	//. m_Options.AddOption(_("Skip the link, do compile only"), wxT_2("-c"), _("Others"));
	m_Options.AddOption(_("Compile all source files as C++"), wxT_2("-cpp"), _("C++ Features"));
	m_Options.AddOption(_("Generate .cod (assemply) file"), wxT_2("-cod"), _("Others"));
	m_Options.AddOption(_("No inline function expansion"), wxT_2("-C"), _("Debugging"));
	m_Options.AddOption(_("Generate .dep (make dependency) file"), wxT_2("-d"), _("Others"));
	m_Options.AddOption(wxT_2("#define DEBUG 1"), wxT_2("-D"), _("Debugging"));
	m_Options.AddOption(_("Show results of preprocessor"), wxT_2("-e"), _("Others"));
	m_Options.AddOption(_("Do not elide comments"), wxT_2("-EC"), _("Others"));
	m_Options.AddOption(_("#line directives not output"), wxT_2("-EL"), _("Others"));
	m_Options.AddOption(_("IEEE 754 inline 8087 code"), wxT_2("-f"), _("Others"));
	m_Options.AddOption(_("Work around FDIV problem"), wxT_2("-fd"), _("Others"));
	m_Options.AddOption(_("Fast inline 8087 code"), wxT_2("-ff"), _("Optimization"));
	m_Options.AddOption(_("Disable debug info optimization"), wxT_2("-gf"), _("Optimization"));
	m_Options.AddOption(_("Make static functions global"), wxT_2("-gg"), _("Optimization"));
	m_Options.AddOption(_("Symbol info for globals"), wxT_2("-gh"), _("C++ Features"));
	m_Options.AddOption(_("Debug line numbers only"), wxT_2("-gl"), _("Debugging"));
	m_Options.AddOption(_("Generate pointer validations"), wxT_2("-gp"), _("Others"));
	m_Options.AddOption(_("Debug symbol info only"), wxT_2("-gs"), _("Debugging"));
	m_Options.AddOption(_("Generate trace prolog/epilog"), wxT_2("-gt"), _("Others"));
	//. m_Options.AddOption(_("Set data threshold to nnnn"), wxT_2("-GTnnnn"), _("Others"));
	m_Options.AddOption(_("Use precompiled headers (ph)"), wxT_2("-H"), _("Others"));
	//. m_Options.AddOption(_("Use ph from directory"), wxT_2("-HDdirectory"), _("Others"));
	//. m_Options.AddOption(_("Generate ph to filename"), wxT_2("-HF[filename]"), _("Others"));
	//. m_Options.AddOption(_("#include \"filename\""), wxT_2("-HIfilename"), _("Others"));
	m_Options.AddOption(_("Include files only once"), wxT_2("-HO"), _("Others"));
	m_Options.AddOption(_("Only search -I directories"), wxT_2("-HS"), _("Others"));
	//. m_Options.AddOption(_("#include file search path"), wxT_2("-Ipath"), _("Others"));
	//. m_Options.AddOption(_("Asian language characters (Japanese)"), wxT_2("-j0"), _("Others"));
	//. m_Options.AddOption(_("Asian language characters (Taiwanese/Chinese)"), wxT_2("-j1"), _("Others"));
	//. m_Options.AddOption(_("Asian language characters (Korean)"), wxT_2("-j2"), _("Others"));
	m_Options.AddOption(_("Relaxed type checking"), wxT_2("-Jm"), _("Others"));
	m_Options.AddOption(wxT_2("char==unsigned"), wxT_2("-Ju"), _("Others"));
	m_Options.AddOption(_("No empty base class optimization"), wxT_2("-Jb"), _("Optimization"));
	m_Options.AddOption(_("chars are unsigned"), wxT_2("-J"), _("Others"));
	//. m_Options.AddOption(_("Generate list file"), wxT_2("-l[listfile]"), _("Others"));
	//. m_Options.AddOption(_("Using non-Digital Mars linker"), wxT_2("-L"), _("Others"));
	//. m_Options.AddOption(_("Specify linker to use"), wxT_2("-Llink"), _("Others"));
	//. m_Options.AddOption(_("Pass /switch to linker"), wxT_2("-L/switch"), _("Others"));
	//. m_Options.AddOption(_("Specify assembler to use"), wxT_2("-Masm"), _("Others"));
	//. m_Options.AddOption(_("Pass /switch to assembler"), wxT_2("-M/switch"), _("Others"));
	//. m_Options.AddOption(_("Set memory model (-mn: Windows)"), wxT_2("-m[tsmclvfnrpxz][do][w][u]"), _("Architecture"));
	m_Options.AddOption(_("Perform function level linking"), wxT_2("-Nc"), _("Optimization"));
	m_Options.AddOption(_("No default library"), wxT_2("-NL"), _("Optimization"));
	m_Options.AddOption(_("Place expr strings in code seg"), wxT_2("-Ns"), _("Optimization"));
	m_Options.AddOption(_("New code seg for each function"), wxT_2("-NS"), _("Optimization"));
	//. m_Options.AddOption(_("Set code segment name"), wxT_2("-NTname"), _("Others"));
	m_Options.AddOption(_("vtables in far data"), wxT_2("-NV"), _("Others"));
	//. m_Options.AddOption(_("Run optimizer with flag"), wxT_2("-o[-+flag]"), _("Optimization"));
	m_Options.AddOption(_("Minimize space"), wxT_2("-o+space"), _("Optimization"));
	m_Options.AddOption(_("Maximize speed"), wxT_2("-o"), _("Optimization"));
	//. m_Options.AddOption(_("Output filename"), wxT_2("-ooutput"), _("Others"));
	m_Options.AddOption(_("Turn off function auto-prototyping"), wxT_2("-p"), _("Others"));
	m_Options.AddOption(_("Make Pascal linkage the default"), wxT_2("-P"), _("Linkage"));
	m_Options.AddOption(_("Make stdcall linkage the default"), wxT_2("-Pz"), _("Linkage"));
	m_Options.AddOption(_("Require strict function prototyping"), wxT_2("-r"), _("Others"));
	m_Options.AddOption(_("Put switch tables in code seg"), wxT_2("-R"), _("Others"));
	m_Options.AddOption(_("Stack overflow checking"), wxT_2("-s"), _("Others"));
	m_Options.AddOption(_("Always generate stack frame"), wxT_2("-S"), _("Others"));
	m_Options.AddOption(_("Suppress non-ANSI predefined macros"), wxT_2("-u"), _("C++ Features"));
	//. m_Options.AddOption(_("Verbose compile"), wxT_2("-v[0|1|2]"), _("Warnings"));
	m_Options.AddOption(_("Suppress all warning messages"), wxT_2("-w"), _("Warnings"));
	m_Options.AddOption(_("Warn on C style casts"), wxT_2("-wc"), _("Warnings"));
	//. m_Options.AddOption(_("Suppress warning number n"), wxT_2("-wn"), _("Warnings"));
	m_Options.AddOption(_("Treat warnings as errors"), wxT_2("-wx"), _("Warnings"));
	//. m_Options.AddOption(_("Windows prolog/epilog (-WA exe -WD dll)"), wxT_2("-W{0123ADabdefmrstuvwx-+}"), _("Architecture"));
	m_Options.AddOption(_("Windows prolog/epilog : Win32 Exe"), wxT_2("-WA"), _("Architecture"));
	m_Options.AddOption(_("Windows prolog/epilog : Win32 Dll"), wxT_2("-WD"), _("Architecture"));
	m_Options.AddOption(_("Turn off error maximum"), wxT_2("-x"), _("Warnings"));
	m_Options.AddOption(_("Instantiate templates"), wxT_2("-XD"), _("C++ Features"));
	//. m_Options.AddOption(_("Instantiate template class temp<type>"), wxT_2("-XItemp<type>"), _("C++ Features"));
	//. m_Options.AddOption(_("Instantiate template function func(type)"), wxT_2("-XIfunc(type)"), _("C++ Features"));
	//. m_Options.AddOption(_("8088/286/386/486/Pentium/P6 code"), wxT_2("-[0|2|3|4|5|6]"), _("Architecture"));
	m_Options.AddOption(_("Optimize for 80386"), wxT_2("-3"), _("Architecture"));
	m_Options.AddOption(_("Optimize for 80486"), wxT_2("-4"), _("Architecture"));
	m_Options.AddOption(_("Optimize for Pentium"), wxT_2("-5"), _("Architecture"));
	m_Options.AddOption(_("Optimize for Pentium Pro, Pentium II, Pentium III"), wxT_2("-6"), _("Architecture"));

    // FIXME (hd#1#): should be work on: we need $res_options
    m_Commands[(int)ctCompileObjectCmd].push_back( CompilerTool(wxT_2("$compiler -mn -c $options $includes -o$object $file")) );
    m_Commands[(int)ctCompileResourceCmd].push_back( CompilerTool(wxT_2("$rescomp -32 -I$res_includes -o$resource_output $file")) );
    m_Commands[(int)ctLinkExeCmd].push_back( CompilerTool(wxT_2("$linker /NOLOGO /subsystem:windows $link_objects, $exe_output, , $libs $link_options, , $link_resobjects")) );
    m_Commands[(int)ctLinkConsoleExeCmd].push_back( CompilerTool(wxT_2("$linker /NOLOGO $link_objects, $exe_output, , $libs $link_options")) );
    m_Commands[(int)ctLinkDynamicCmd].push_back( CompilerTool(wxT_2("$linker /NOLOGO /subsystem:windows $link_objects, $exe_output, , $libs $link_options, , $link_resobjects")) );
    m_Commands[(int)ctLinkStaticCmd].push_back( CompilerTool(wxT_2("$lib_linker -c $link_options $static_output $link_objects")) );
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerDMC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")[ \t]+:[ \t]+(.*error LNK[0-9]+.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\)[ \t]*:[ \t]*[Ww][Aa][Rr][Nn][Ii][Nn][Gg][ \t]*(.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\)[ \t]*:[ \t]*(.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("Fatal error:[ \t](.*)"), 1));
}

AutoDetectResult CompilerDMC::AutoDetectInstallationDir()
{
    // just a guess; the default installation dir
	m_MasterPath = wxT_2("C:\\dm");
    wxString sep = wxFileName::GetPathSeparator();

    // NOTE (hd#1#): dmc uses sc.ini for compiler's master directories
    // NOTE (mandrav#1#): which doesn't seem to exist if you don't have the CD version ;)
    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + sep + wxT_2("stlport") + sep + wxT_2("stlport"));
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));
    }

    return wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
