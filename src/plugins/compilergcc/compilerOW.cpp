#ifdef __WXMSW__
// this compiler is valid only in windows

#include "ca/sdk.h"
#include "compilerOW.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/utils.h>
#include <wx/msw/registry.h>

#include <logmanager.h>
#include <manager.h>
#include "compilerOWgenerator.h"

#include <wx/utils.h>
#include <wx/filefn.h>

CompilerOW::CompilerOW()
    : Compiler(wxT_2("OpenWatcom (W32) Compiler"), wxT_2("ow"))
{
    Reset();
}

CompilerOW::~CompilerOW()
{
	//dtor
}

Compiler * CompilerOW::CreateCopy()
{
    Compiler* c = new CompilerOW(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

CompilerCommandGenerator* CompilerOW::GetCommandGenerator()
{
    return new CompilerOWGenerator;
}

void CompilerOW::Reset()
{
    /*
     *  Define compiler suite programs. I have chosen to use wcl386 for all
     *  to provide as consistent a set of options as possible.
     */

	m_Programs.C                = wxT_2("wcl386.exe");
	m_Programs.CPP              = wxT_2("wcl386.exe");
	m_Programs.LD               = wxT_2("wlink.exe");
	m_Programs.LIB              = wxT_2("wlib.exe");
	m_Programs.WINDRES          = wxT_2("wrc.exe");
	m_Programs.MAKE             = wxT_2("wmake.exe");

	m_Switches.includeDirs      = wxT_2("-i");
	m_Switches.libDirs          = wxT_2("LIBP ");
	m_Switches.linkLibs         = wxT_2("");
	m_Switches.libPrefix        = wxT_2("");
	m_Switches.libExtension     = wxT_2("lib");
	m_Switches.defines          = wxT_2("-d");
	m_Switches.genericSwitch    = wxT_2("-");
	m_Switches.objectExtension  = wxT_2("obj");

	m_Switches.needDependencies = false;
	m_Switches.forceCompilerUseQuotes = false;
	m_Switches.forceLinkerUseQuotes = false;
	m_Switches.logging = clogSimple;
	m_Switches.linkerNeedsLibPrefix = false;
	m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();
//	m_Options.AddOption(_("Produce debugging symbols"),
//				"-g",
//				_("Debugging"),
//				"",
//				true,
//				"-o -o+space",
//				_("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));


    wxString   category = wxT_2("General");

    m_Options.AddOption(wxT_2("treat source files as C code"), wxT_2("-cc"), category);
    m_Options.AddOption(wxT_2("treat source files as C++ code"), wxT_2("-cc++"), category);
    m_Options.AddOption(wxT_2("ignore the WCL386 environment variable"), wxT_2("-y"), category);


    category = wxT_2("Processor options");
    m_Options.AddOption(wxT_2("386 register calling conventions"), wxT_2("-3r"), category);
    m_Options.AddOption(wxT_2("386 stack calling conventions"), wxT_2("-3s"), category);
    m_Options.AddOption(wxT_2("486 register calling conventions"), wxT_2("-4r"), category);
    m_Options.AddOption(wxT_2("486 stack calling conventions"), wxT_2("-4s"), category);
    m_Options.AddOption(wxT_2("Pentium register calling conventions"), wxT_2("-5r"), category);
    m_Options.AddOption(wxT_2("Pentium stack calling conventions"), wxT_2("-5s"), category);
    m_Options.AddOption(wxT_2("Pentium Pro register call conventions"), wxT_2("-6r"), category);
    m_Options.AddOption(wxT_2("Pentium Pro stack call conventions"), wxT_2("-6s"), category);

    category = wxT_2("Floating-point processor options");

    m_Options.AddOption(wxT_2("calls to floating-point library"), wxT_2("-fpc"), category);
    m_Options.AddOption(wxT_2("enable Pentium FDIV check"), wxT_2("-fpd"), category);
    m_Options.AddOption(wxT_2("inline 80x87 with emulation"), wxT_2("-fpi"), category);
    m_Options.AddOption(wxT_2("inline 80x87"), wxT_2("-fpi87"), category);
    m_Options.AddOption(wxT_2("use old floating-point conventions"), wxT_2("-fpr"), category);
    m_Options.AddOption(wxT_2("generate 287 floating-point code"), wxT_2("-fp2"), category);
    m_Options.AddOption(wxT_2("generate 387 floating-point code"), wxT_2("-fp3"), category);
    m_Options.AddOption(wxT_2("optimize floating-point for Pentium"), wxT_2("-fp5"), category);
    m_Options.AddOption(wxT_2("optimize floating-point for Pentium Pro"), wxT_2("-fp6"), category);

    category = wxT_2("Compiler options");

    m_Options.AddOption(wxT_2("compile and link for DOS"), wxT_2("-bcl=dos"), category);
    m_Options.AddOption(wxT_2("compile and link for Linux"), wxT_2("-bcl=linux"), category);
    m_Options.AddOption(wxT_2("compile and link for NT (includes Win32)"), wxT_2("-bcl=nt"), category);
    m_Options.AddOption(wxT_2("compile and link for OS/2"), wxT_2("-bcl=os2"), category);
    m_Options.AddOption(wxT_2("compile and link for QNX"), wxT_2("-bcl=qnx"), category);
    m_Options.AddOption(wxT_2("compile and link for Windows"), wxT_2("-bcl=windows"), category);

    m_Options.AddOption(wxT_2("compile for DOS"), wxT_2("-bt=dos"), category);
    m_Options.AddOption(wxT_2("compile for Linux"), wxT_2("-bt=linux"), category);
    m_Options.AddOption(wxT_2("compile for NetWare"), wxT_2("-bt=netware"), category);
    m_Options.AddOption(wxT_2("compile for NetWare 5 and later"), wxT_2("-bt=netware5"), category);
    m_Options.AddOption(wxT_2("compile for NT (includes Win32)"), wxT_2("-bt=nt"), category);
    m_Options.AddOption(wxT_2("compile for OS/2"), wxT_2("-bt=os2"), category);
    m_Options.AddOption(wxT_2("compile for QNX"), wxT_2("-bt=qnx"), category);
    m_Options.AddOption(wxT_2("compile for Windows"), wxT_2("-bt=windows"), category);


    m_Options.AddOption(wxT_2("generate browsing information"), wxT_2("-db"), category);
    m_Options.AddOption(wxT_2("set error limit number (set ERROR_LIMIT in custom variables)"), wxT_2("-e=$(ERROR_LIMIT)"), category);
    m_Options.AddOption(wxT_2("call epilogue hook routine"), wxT_2("-ee"), category);
    m_Options.AddOption(wxT_2("full paths in messages"), wxT_2("-ef"), category);
    m_Options.AddOption(wxT_2("force enums to be type int"), wxT_2("-ei"), category);
    m_Options.AddOption(wxT_2("minimum base type for enum is int"), wxT_2("-em"), category);
    m_Options.AddOption(wxT_2("emit routine names in the code"), wxT_2("-en"), category);
    m_Options.AddOption(wxT_2("call prologue hook routine"), wxT_2("-ep"), category);
    m_Options.AddOption(wxT_2("call prologue hook routine with n bytes of stack (set PROLOGUE_STACK in custom variables)"), wxT_2("-ep=$(PROLOGUE_STACK)"), category);
    m_Options.AddOption(wxT_2("do not display error messages"), wxT_2("-eq"), category);
    m_Options.AddOption(wxT_2("P5 profiling"), wxT_2("-et"), category);
    m_Options.AddOption(wxT_2("generate PharLap EZ-OMF object"), wxT_2("-ez"), category);
    m_Options.AddOption(wxT_2("generate pre-compiled header (set PCH_FILE in custom variables)"), wxT_2("-fh=$(PCH_FILE)"), category);
    m_Options.AddOption(wxT_2("generate pre-compiled header (Quiet) (set PCH_FILE in custom variables)"), wxT_2("-fhq=$(PCH_FILE)"), category);
    m_Options.AddOption(wxT_2("(C++) only read PCH"), wxT_2("-fhr"), category);
    m_Options.AddOption(wxT_2("(C++) only write PCH"), wxT_2("-fhw"), category);
    m_Options.AddOption(wxT_2("(C++) don't count PCH warnings"), wxT_2("-fhwe"), category);

    // This should be a multiple option. We can define multiple force includes
    m_Options.AddOption(wxT_2("force include of file (define FORCE_INCLUDE in custom variables)"), wxT_2("-fi=$(FORCE_INCLUDE)"), category);
    // This one is mandatory in the ctCompileObjectCmd
    //m_Options.AddOption(wxT_2("set object file name"), wxT_2("-fo=<file>"), category);
    m_Options.AddOption(wxT_2("set error file name (define ERROR_FILE in custom variables)"), wxT_2("-fr=$(ERROR_FILE)"), category);
    m_Options.AddOption(wxT_2("(C++) check for 8.3 file names"), wxT_2("-ft"), category);
    m_Options.AddOption(wxT_2("(C++) no check for 8.3 file names"), wxT_2("-fx"), category);
    m_Options.AddOption(wxT_2("set code group name (define CODEGROUP in custom variables)"), wxT_2("-g=$(CODEGROUP)"), category);
    m_Options.AddOption(wxT_2("codeview debug format"), wxT_2("-hc"), category);
    m_Options.AddOption(wxT_2("dwarf debug format"), wxT_2("-hd"), category);
    m_Options.AddOption(wxT_2("watcom debug format"), wxT_2("-hw"), category);
    m_Options.AddOption(wxT_2("change char default to signed"), wxT_2("-j"), category);
    m_Options.AddOption(wxT_2("memory model flat"), wxT_2("-mf"), category);
    m_Options.AddOption(wxT_2("memory model small"), wxT_2("-ms"), category);
    m_Options.AddOption(wxT_2("memory model medium"), wxT_2("-mm"), category);
    m_Options.AddOption(wxT_2("memory model compact"), wxT_2("-mc"), category);
    m_Options.AddOption(wxT_2("memory model large"), wxT_2("-ml"), category);
    m_Options.AddOption(wxT_2("memory model huge"), wxT_2("-mh"), category);
    m_Options.AddOption(wxT_2("set CODE class name (define CODECLASS in custom variables)"), wxT_2("-nc=$(CODECLASS)"), category);
    m_Options.AddOption(wxT_2("set data segment name (define DATANAME in custom variables)"), wxT_2("-nd=$(DATANAME)"), category);
    m_Options.AddOption(wxT_2("set module name (define MODULENAME in custom variables)"), wxT_2("-nm=$(MODULENAME)"), category);
    m_Options.AddOption(wxT_2("set text segment name (define TEXTNAME in custom variables)"), wxT_2("-nt=$(TEXTNAME)"), category);
    m_Options.AddOption(wxT_2("save/restore segregs across calls"), wxT_2("-r"), category);
    m_Options.AddOption(wxT_2("promote function args/rets to int"), wxT_2("-ri"), category);
    m_Options.AddOption(wxT_2("remove stack overflow checks"), wxT_2("-s"), category);
    m_Options.AddOption(wxT_2("generate calls to grow the stack"), wxT_2("-sg"), category);
    m_Options.AddOption(wxT_2("touch stack through SS first"), wxT_2("-st"), category);
    m_Options.AddOption(wxT_2("output func declarations to .def"), wxT_2("-v"), category);
    m_Options.AddOption(wxT_2("VC++ compat: alloca allowed in arg lists"), wxT_2("-vcap"), category);
    m_Options.AddOption(wxT_2("set warning level to 0 (suppress warnings)"), wxT_2("=w=0"), category);
    m_Options.AddOption(wxT_2("set warning level to 1"), wxT_2("-w=1"), category);
    m_Options.AddOption(wxT_2("set warning level to 2"), wxT_2("-w=2"), category);
    m_Options.AddOption(wxT_2("set warning level to 3"), wxT_2("-w=3"), category);
    m_Options.AddOption(wxT_2("disable warning message (define DIS_WARN in custom variables)"), wxT_2("-wcd=$(DIS_WARN)"), category);
    m_Options.AddOption(wxT_2("enable warning message (define ENA_WARN in custom variables)"), wxT_2("-wce=$(ENA_WARN)"), category);
    m_Options.AddOption(wxT_2("treat all warnings as errors"), wxT_2("-we"), category);
    m_Options.AddOption(wxT_2("set warning level to max"), wxT_2("-wx"), category);
    m_Options.AddOption(wxT_2("(C++) enable RTTI"), wxT_2("-xr"), category);
    m_Options.AddOption(wxT_2("disable language extensions (ANSI/ISO compliance)"), wxT_2("-za"), category);
    m_Options.AddOption(wxT_2("enable language extensions"), wxT_2("-ze"), category);
    m_Options.AddOption(wxT_2("place strings in CODE segment"), wxT_2("-zc"), category);
    m_Options.AddOption(wxT_2("DS not pegged to DGROUP"), wxT_2("-zdf"), category);
    m_Options.AddOption(wxT_2("DS pegged to DGROUP"), wxT_2("-zdp"), category);
    m_Options.AddOption(wxT_2("load DS directly from DGROUP"), wxT_2("-zdl"), category);
    m_Options.AddOption(wxT_2("Allow code-generator to use FS"), wxT_2("-zff"), category);
    m_Options.AddOption(wxT_2("Do not allow code-generator to use FS"), wxT_2("-zfp"), category);
    m_Options.AddOption(wxT_2("Allow code-generator to use GS"), wxT_2("-zgf"), category);
    m_Options.AddOption(wxT_2("Do not allow code-generator to use GS"), wxT_2("-zgp"), category);
    m_Options.AddOption(wxT_2("Allow arithmetic on void derived type"), wxT_2("-zev"), category);
    m_Options.AddOption(wxT_2("function prototype using base type"), wxT_2("-zg"), category);
    // Duplicate to -zk0
    //m_Options.AddOption(wxT_2("Double byte chars in strings (Japanese DBCS)"), wxT_2("-zk"), category);
    m_Options.AddOption(wxT_2("Double byte chars in strings (Japanese DBCS)"), wxT_2("-zk0"), category);
    m_Options.AddOption(wxT_2("Double byte chars in strings (Japanese DBCS - translate to Unicode)"), wxT_2("-zk0u"), category);
    m_Options.AddOption(wxT_2("Double byte chars in strings (Trad Chinese or Taiwanese DBCS)"), wxT_2("-zk1"), category);
    m_Options.AddOption(wxT_2("Double byte chars in strings (Korean Hangeul) DBCS)"), wxT_2("-zk2"), category);
    m_Options.AddOption(wxT_2("Double byte chars in strings (Use current code page)"), wxT_2("-zkl"), category);
    m_Options.AddOption(wxT_2("Translate characters to Unicode (specify UNI_CP in custom variables)"), wxT_2("-zku=$(UNI_CP)"), category);
    m_Options.AddOption(wxT_2("remove default library information"), wxT_2("-zl"), category);
    m_Options.AddOption(wxT_2("remove file dependency information"), wxT_2("-zld"), category);
    m_Options.AddOption(wxT_2("place functions in separate segments"), wxT_2("-zm"), category);
    m_Options.AddOption(wxT_2("(C++) zm with near calls allowed"), wxT_2("-zmf"), category);
    m_Options.AddOption(wxT_2("struct packing align 1 byte"), wxT_2("-zp1"), category);
    m_Options.AddOption(wxT_2("struct packing align 2 byte"), wxT_2("-zp2"), category);
    m_Options.AddOption(wxT_2("struct packing align 4 byte"), wxT_2("-zp4"), category);
    m_Options.AddOption(wxT_2("struct packing align 8 byte"), wxT_2("-zp8"), category);
    m_Options.AddOption(wxT_2("struct packing align 16 byte"), wxT_2("-zp16"), category);
    m_Options.AddOption(wxT_2("warning when padding a struct"), wxT_2("-zpw"), category);
    m_Options.AddOption(wxT_2("operate quietly"), wxT_2("-zq"), category);
    m_Options.AddOption(wxT_2("check syntax only"), wxT_2("-zs"), category);
    m_Options.AddOption(wxT_2("set data threshold (set DATA_THRESHOLD in custom variables)"), wxT_2("-zt=$(DATA_THRESHOLD)"), category);
    m_Options.AddOption(wxT_2("Do not assume SS contains DGROUP"), wxT_2("-zu"), category);
    m_Options.AddOption(wxT_2("(C++) enable virt. fun. removal opt"), wxT_2("-zv"), category);
    m_Options.AddOption(wxT_2("generate code for MS Windows"), wxT_2("-zw"), category);
    m_Options.AddOption(wxT_2("remove @size from __stdcall func."), wxT_2("-zz"), category);

    category = wxT_2("Debugging options");

    m_Options.AddOption(wxT_2("no debugging information"), wxT_2("-d0"), category);
    m_Options.AddOption(wxT_2("line number debugging information"), wxT_2("-d1"), category);
    m_Options.AddOption(wxT_2("(C) line number debugging information plus typing information for global symbols and local structs and arrays"), wxT_2("-d1+"), category);
    m_Options.AddOption(wxT_2("full symbolic debugging information"), wxT_2("-d2"), category);
    m_Options.AddOption(wxT_2("(C++) d2 and debug inlines; emit inlines as external out-of-line functions"), wxT_2("-d2i"), category);
    m_Options.AddOption(wxT_2("(C++) d2 and debug inlines; emit inlines as static out-of-line functions"), wxT_2("-d2s"), category);
    m_Options.AddOption(wxT_2("(C++) d2 but without type names"), wxT_2("-d2t"), category);
    m_Options.AddOption(wxT_2("full symbolic debugging with unreferenced type names"), wxT_2("-d3"), category);
    m_Options.AddOption(wxT_2("(C++) d3 plus debug inlines; emit inlines as external out-of-line functions"), wxT_2("-d3i"), category);
    m_Options.AddOption(wxT_2("(C++) d3 plus debug inlines; emit inlines as static out-of-line functions"), wxT_2("-d3s"), category);

    category = wxT_2("Optimization options");

    m_Options.AddOption(wxT_2("relax alias checking"), wxT_2("-oa"), category);
    m_Options.AddOption(wxT_2("branch prediction"), wxT_2("-ob"), category);
    m_Options.AddOption(wxT_2("disable call/ret optimization"), wxT_2("-oc"), category);
    m_Options.AddOption(wxT_2("disable optimizations"), wxT_2("-od"), category);
    m_Options.AddOption(wxT_2("expand functions inline (specify INLINE_NUM in custom variables)"), wxT_2("-oe=$(INLINE_NUM)"), category);
    m_Options.AddOption(wxT_2("generate traceable stack frames"), wxT_2("-of"), category);
    m_Options.AddOption(wxT_2("always generate traceable stack frames"), wxT_2("-of+"), category);
    m_Options.AddOption(wxT_2("enable repeated optimizations"), wxT_2("-oh"), category);
    m_Options.AddOption(wxT_2("inline intrinsic functions"), wxT_2("-oi"), category);
    m_Options.AddOption(wxT_2("(C++) oi with max inlining depth"), wxT_2("-oi+"), category);
    m_Options.AddOption(wxT_2("control flow entry/exit sequence"), wxT_2("-ok"), category);
    m_Options.AddOption(wxT_2("perform loop optimizations"), wxT_2("-ol"), category);
    m_Options.AddOption(wxT_2("ol with loop unrolling"), wxT_2("-ol+"), category);
    m_Options.AddOption(wxT_2("generate inline math functions"), wxT_2("-om"), category);
    m_Options.AddOption(wxT_2("numerically unstable floating-point"), wxT_2("-on"), category);
    m_Options.AddOption(wxT_2("continue compile when low on memory"), wxT_2("-oo"), category);
    m_Options.AddOption(wxT_2("improve floating-point consistency"), wxT_2("-op"), category);
    m_Options.AddOption(wxT_2("re-order instructions to avoid stalls"), wxT_2("-or"), category);
    m_Options.AddOption(wxT_2("optimize for space"), wxT_2("-os"), category);
    m_Options.AddOption(wxT_2("optimize for time"), wxT_2("-ot"), category);
    m_Options.AddOption(wxT_2("ensure unique addresses for functions"), wxT_2("-ou"), category);
    m_Options.AddOption(wxT_2("maximum optimization (-obmiler -s)"), wxT_2("-ox"), category);

    category = wxT_2("C++ exception handling options");

    m_Options.AddOption(wxT_2("no exception handling"), wxT_2("-xd"), category);
    m_Options.AddOption(wxT_2("no exception handling: space"), wxT_2("-xds"), category);
    // duplicate to -xd
    //m_Options.AddOption(wxT_2("no exception handling"), wxT_2("-xdt"), category);
    m_Options.AddOption(wxT_2("exception handling: balanced"), wxT_2("-xs"), category);
    m_Options.AddOption(wxT_2("exception handling: space"), wxT_2("-xss"), category);
    m_Options.AddOption(wxT_2("exception handling: time"), wxT_2("-xst"), category);

    category = wxT_2("Preprocessor options");

    //  defined in m_Switches.defines
    //  m_Options.AddOption(wxT_2("Define macro"), wxT_2("-d"), category);
    // difficult to support
    //  m_Options.AddOption(wxT_2("Extend -d syntax"), wxT_2(""), category);
    //  This one is mandatory in the ctCompileObjectCmd
    //  m_Options.AddOption(wxT_2("set object file name"), wxT_2("-fo=<file>"), category);
    //  Specified by m_Switches.includeDirs
    //  m_Options.AddOption(wxT_2("include directory"), wxT_2("-i"), category);
    m_Options.AddOption(wxT_2("number of spaces in tab stop (set TAB_STOP in custom variables)"), wxT_2("-t=$(TAB_STOP)"), category);
    // multi-option
    //m_Options.AddOption(wxT_2("set #pragma on"), wxT_2("-tp=$(PRAGMA_NAMES)"), category);
    // multi-option
    //m_Options.AddOption(wxT_2("undefine macro name"), wxT_2("-u"), category);
    /*
     *  options are -pcelw=n
     */
    m_Options.AddOption(wxT_2("preprocess source file"), wxT_2("-p"), category);
    m_Options.AddOption(wxT_2("preprocess source file (preserve comments)"), wxT_2("-pc"), category);
    m_Options.AddOption(wxT_2("preprocess source file (insert #line directives)"), wxT_2("-pl"), category);
    m_Options.AddOption(wxT_2("(C++) preprocess file (encrypt identifiers)"), wxT_2("-pe"), category);
    //

    category = wxT_2("Linker options");

    m_Options.AddOption(wxT_2("build Dynamic link library"), wxT_2("-bd"), category);
    m_Options.AddOption(wxT_2("build Multi-thread application"), wxT_2("-bm"), category);
    m_Options.AddOption(wxT_2("build with dll run-time library"), wxT_2("-br"), category);
    m_Options.AddOption(wxT_2("build default Windowing appllication"), wxT_2("-bw"), category);
    m_Options.AddOption(wxT_2("write directives"), wxT_2("-fd"), category);
    m_Options.AddOption(wxT_2("write directives (define DIRECTIVE_FILE in custom variables)"), wxT_2("-fd=$(DIRECTIVE_FILE)"), category);
    // mandatory in link commands
    //m_Options.AddOption(wxT_2("name executable file"), wxT_2("-fe=<file>"), category);
    m_Options.AddOption(wxT_2("generate map file"), wxT_2("-fm"), category);
    m_Options.AddOption(wxT_2("generate map file (define MAP_FILE in custom variables)"), wxT_2("-fm=$(MAP_FILE)"), category);
    m_Options.AddOption(wxT_2("set stack size (define STACK_SIZE in custom variables)"), wxT_2("-k$(STACK_SIZE)"), category);
    m_Options.AddOption(wxT_2("link for the specified OS (define TARGET_OS in custom variables)"), wxT_2("-l=$(TARGET_OS)"), category);
    m_Options.AddOption(wxT_2("make names case sensitive"), wxT_2("-x"), category);
    m_Options.AddOption(wxT_2("additional directive file (specify LINK_DIRECTIVES in custom variables)"), wxT_2("@$(LINK_VARIABLES)"), category);
    // ????
    // m_Options.AddOption(wxT_2("following parameters are linker options"), wxT_2("-"), category);


    m_Commands[(int)ctCompileObjectCmd]
       .push_back( CompilerTool(wxT_2("$compiler -q -c $options $includes -fo=$object $file")) );
    m_Commands[(int)ctCompileResourceCmd]
       .push_back( CompilerTool(wxT_2("$rescomp -q -r -fo=$resource_output $res_includes $file")) );
    m_Commands[(int)ctLinkExeCmd]
       .push_back( CompilerTool(wxT_2("$linker option quiet $link_options $libdirs $link_objects name $exe_output $libs $link_resobjects")) );
    m_Commands[(int)ctLinkConsoleExeCmd]
       .push_back( CompilerTool(wxT_2("$linker option quiet $link_options $libdirs $link_objects name $exe_output $libs $link_resobjects")) );
    m_Commands[(int)ctLinkDynamicCmd]
       .push_back( CompilerTool(wxT_2("$linker option quiet $link_options $libdirs name $exe_output $libs $link_objects")) );
    m_Commands[(int)ctLinkStaticCmd]
       .push_back( CompilerTool(wxT_2("$lib_linker -q $static_output $link_objects")) );
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerOW::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Note"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\): Note! (.+)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\): Error! (.+)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\): Warning! (.+)"), 3, 1, 2));
}

AutoDetectResult CompilerOW::AutoDetectInstallationDir()
{
    /* Following code is Not necessary as OpenWatcom does not write to
       Registry anymore */
    /*wxRegKey key; // defaults to HKCR
    key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\Open Watcom\\c_1.0"));
    if (key.Open())
        // found; read it
        key.QueryValue(wxT_2("Install Location"), m_MasterPath);*/

    if (m_MasterPath.IsEmpty())
        // just a guess; the default installation dir
        m_MasterPath = wxT_2("C:\\watcom");

    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("h"));
        AddIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("h") + wxFILE_SEP_PATH + wxT_2("nt"));
        AddLibDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("lib386"));
        AddLibDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("lib386") + wxFILE_SEP_PATH + wxT_2("nt"));
        AddResourceIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("h"));
        AddResourceIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("h") + wxFILE_SEP_PATH + wxT_2("nt"));
        m_ExtraPaths.Add(m_MasterPath + wxFILE_SEP_PATH + wxT_2("binnt"));
        m_ExtraPaths.Add(m_MasterPath + wxFILE_SEP_PATH + wxT_2("binw"));
    }
    wxSetEnv(wxT_2("WATCOM"), m_MasterPath);

    return wxFileExists(m_MasterPath + wxFILE_SEP_PATH + wxT_2("binnt") + wxFILE_SEP_PATH + m_Programs.C) ? adrDetected : adrGuessed;
}

void CompilerOW::LoadSettings(const wxString& baseKey)
{
    Compiler::LoadSettings(baseKey);
    wxSetEnv(wxT_2("WATCOM"), m_MasterPath);
}

void CompilerOW::SetMasterPath(const wxString& path)
{
    Compiler::SetMasterPath(path);
    wxSetEnv(wxT_2("WATCOM"), m_MasterPath);
}

#endif // __WXMSW__
