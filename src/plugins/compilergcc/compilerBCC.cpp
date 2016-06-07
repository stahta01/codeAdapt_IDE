#ifdef __WXMSW__
// this compiler is valid only in windows

#include "ca/sdk.h"
#include "compilerBCC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/msw/registry.h>

CompilerBCC::CompilerBCC()
    : Compiler(_("Borland C++ Compiler (5.5, 5.82)"), wxT_2("bcc"))
{
    Reset();
}

CompilerBCC::~CompilerBCC()
{
	//dtor
}

Compiler * CompilerBCC::CreateCopy()
{
    Compiler* c = new CompilerBCC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerBCC::Reset()
{
    m_Programs.C = wxT_2("bcc32.exe");
    m_Programs.CPP = wxT_2("bcc32.exe");
    m_Programs.LD = wxT_2("ilink32.exe");
    m_Programs.LIB = wxT_2("tlib.exe");
    m_Programs.WINDRES = wxT_2("brcc32.exe"); // platform SDK is needed for this
    m_Programs.MAKE = wxT_2("make.exe");

    m_Switches.includeDirs = wxT_2("-I");
    m_Switches.libDirs = wxT_2("-L");
    m_Switches.linkLibs = wxT_2("");
    m_Switches.defines = wxT_2("-D");
    m_Switches.genericSwitch = wxT_2("-");
    m_Switches.objectExtension = wxT_2("obj");
    m_Switches.needDependencies = false;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = true;
    m_Switches.logging = clogSimple;
    m_Switches.libPrefix = wxT_2("");
    m_Switches.libExtension = wxT_2("lib");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = true;

    m_Options.ClearOptions();

    wxString category = _("Optimization");
    m_Options.AddOption(_("Optimizations for size"), wxT_2("-O1"), category);
    m_Options.AddOption(_("Optimizations for speed"), wxT_2("-O2"), category);
    m_Options.AddOption(_("Optimize jumps"), wxT_2("-O"), category);
    m_Options.AddOption(_("Eliminate duplicate expressions"), wxT_2("-Oc"), category);
    m_Options.AddOption(_("Disable optimizations"), wxT_2("-Od"), category);
    m_Options.AddOption(_("Expand intrinsic functions"), wxT_2("-Oi"), category);
    m_Options.AddOption(_("Enable instruction scheduling for Pentium"), wxT_2("-OS"), category);
    m_Options.AddOption(_("Disable instruction scheduling"), wxT_2("-O-S"), category);
    m_Options.AddOption(_("Enable loop induction variable and strength reduction"), wxT_2("-Ov"), category);
    m_Options.AddOption(_("Disable register variables"), wxT_2("-r-"), category);
    m_Options.AddOption(_("Merge duplicate strings"), wxT_2("-d"), category);
    m_Options.AddOption(_("Function stack frame optimization"), wxT_2("-k-"), category);


    category = _("C++ Features");
    m_Options.AddOption(_("Disable runtime type information"), wxT_2("-RT-"), category);
    m_Options.AddOption(_("Disable exception handling"), wxT_2("-x-"), category);
    m_Options.AddOption(_("Enable destructor cleanup"), wxT_2("-xd"), category);
    m_Options.AddOption(_("Use global destructor count"), wxT_2("-xdg"), category);
    m_Options.AddOption(_("Enable fast exception prologs"), wxT_2("-xf"), category);
    m_Options.AddOption(_("Enable exception location information"), wxT_2("-xp"), category);
    m_Options.AddOption(_("Enable slow exception epilogues"), wxT_2("-xs"), category);
    m_Options.AddOption(_("Zero length empty class members"), wxT_2("-Vx"), category);
    m_Options.AddOption(_("Zero-length empty base classes"), wxT_2("-Ve"), category);


    category = _("C Features");
    m_Options.AddOption(_("Emulate floating point"), wxT_2("-f"), category);
    m_Options.AddOption(_("Disable floating point"), wxT_2("-f-"), category);
    m_Options.AddOption(_("Fast floating point"), wxT_2("-ff"), category);
    m_Options.AddOption(_("Pentium FDIV workaround"), wxT_2("-fp"), category);
    m_Options.AddOption(_("Pascal calling convention"), wxT_2("-p"), category);
    m_Options.AddOption(_("C calling convention"), wxT_2("-pc"), category);
    m_Options.AddOption(_("__msfastcall calling convention"), wxT_2("-pm"), category);
    m_Options.AddOption(_("fastcall calling convention"), wxT_2("-pr"), category);
    m_Options.AddOption(_("stdcall calling convention"), wxT_2("-ps"), category);
    m_Options.AddOption(_("Align data by byte"), wxT_2("-a1"), category);
    m_Options.AddOption(_("Align data by word (2 bytes)"), wxT_2("-a2"), category);
    m_Options.AddOption(_("Align data by double word (4 bytes)"), wxT_2("-a4"), category);
    m_Options.AddOption(_("Align data by quad word (8 bytes)"), wxT_2("-a8"), category);
    m_Options.AddOption(_("Align data by paragraph (16 bytes)"), wxT_2("-a16"), category);
    m_Options.AddOption(_("Use minimum sized enums"), wxT_2("-b-"), category);
    m_Options.AddOption(_("Use unsigned char"), wxT_2("-K"), category);


    category = _("Language");
    m_Options.AddOption(_("ANSI keywords and extensions"), wxT_2("-A"), category);
    m_Options.AddOption(_("Kernighan and Ritchie keywords and extensions"), wxT_2("-AK"), category);
    m_Options.AddOption(_("Borland C++ keywords and extensions"), wxT_2("-AT"), category);
    m_Options.AddOption(_("UNIX V keywords and extensions"), wxT_2("-AU"), category);
    m_Options.AddOption(_("Allow nested comments"), wxT_2("-C"), category);


    category = _("Debugging");
    m_Options.AddOption(_("debugging on, inline expansion off"), wxT_2("-v"), category);
    m_Options.AddOption(_("debugging off, inline expansion on"), wxT_2("-v-"), category);
    m_Options.AddOption(_("inline function expansion on"), wxT_2("-vi"), category);
    m_Options.AddOption(_("inline function expansion off"), wxT_2("-vi-"), category);
    m_Options.AddOption(_("Include line numbers"), wxT_2("-y"), category);


    category = _("Architecture");
    m_Options.AddOption(_("Optimize for 80386"), wxT_2("-3"), category);
    m_Options.AddOption(_("Optimize for 80486"), wxT_2("-4"), category);
    m_Options.AddOption(_("Optimize for Pentium"), wxT_2("-5"), category);
    m_Options.AddOption(_("Optimize for Pentium Pro, Pentium II, Pentium III"), wxT_2("-6"), category);


    category = _("Target");
    m_Options.AddOption(_("Windows application"), wxT_2("-tW"), category);
    m_Options.AddOption(_("Console application"), wxT_2("-tWC"), category);
    m_Options.AddOption(_(".DLL executable"), wxT_2("-tWD"), category);
    m_Options.AddOption(_("32-bit multi-threaded"), wxT_2("-tWM"), category);
    m_Options.AddOption(_("Target uses the dynamic RTL"), wxT_2("-tWR"), category);
    m_Options.AddOption(_("Target uses the VCL"), wxT_2("-tWV"), category);


    category = _("Warnings");
    m_Options.AddOption(_("Display all warnings"), wxT_2("-w"), category);


    m_Commands[(int)ctCompileObjectCmd].push_back( CompilerTool(wxT_2("$compiler -q $options $includes -o$object -c $file")) );
    m_Commands[(int)ctCompileResourceCmd].push_back( CompilerTool(wxT_2("$rescomp -32 -fo$resource_output $res_includes $file")) );
    m_Commands[(int)ctLinkExeCmd].push_back( CompilerTool(wxT_2("$linker -q -aa  $link_options $libdirs c0w32 $link_objects,$exe_output,,$libs,,$link_resobjects")) );
    m_Commands[(int)ctLinkConsoleExeCmd].push_back( CompilerTool(wxT_2("$linker -q -ap  $link_options $libdirs c0x32 $link_objects,$exe_output,,$libs,,$link_resobjects")) );
    m_Commands[(int)ctLinkDynamicCmd].push_back( CompilerTool(wxT_2("$linker -q $libdirs -Tpd $link_options $link_objects,$exe_output,,$libs,,$link_resobjects")) );
    m_Commands[(int)ctLinkStaticCmd].push_back( CompilerTool(wxT_2("$lib_linker /C $static_output $+-link_objects,$def_output")) );
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerBCC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(^Warning[ \t]W[0-9]+)[ \t](") + FilePathWithSpaces + wxT_2(")[ \t]([0-9]+)(:[ \t].*)"), 1, 2, 3, 4));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(^Error[ \t]E[0-9]+)[ \t](") + FilePathWithSpaces + wxT_2(")[ \t]([0-9]+)(:[ \t].*)"), 1, 2, 3, 4));
    m_RegExes.Add(RegExStruct(_("Unknown error"), cltError, wxT_2("(^Error[ \t]+E[0-9]+:.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("Fatal:[ \t]+(.*)"), 1));
}

AutoDetectResult CompilerBCC::AutoDetectInstallationDir()
{
    wxArrayString l_MasterPath_Arr, l_RegKey_Arr;
    // Fill assumed masterpaths and reg keys
    l_MasterPath_Arr.Add(wxT_2("C:\\Program Files\\Borland\\BDS\\4.0"));
    l_MasterPath_Arr.Add(wxT_2("C:\\Borland\\BCC55"));
    l_RegKey_Arr.Add(wxT_2("HKEY_LOCAL_MACHINE\\SOFTWARE\\Borland\\BDS\\4.0"));
    l_RegKey_Arr.Add(wxT_2("HKEY_LOCAL_MACHINE\\SOFTWARE\\Borland\\C++Builder\\5.0"));

    // try to detect Installation dir
    int match = -1;
    for (int i = 0; i < 2; ++i)
    {
        wxRegKey tkey(l_RegKey_Arr[i]);
        if (tkey.Exists() || wxDirExists(l_MasterPath_Arr[i]))
        {
            match = i;
            break;
        }
    }
    if (match < 0)
        match = 0;
    m_MasterPath = l_MasterPath_Arr[match];
    wxRegKey key(l_RegKey_Arr[match]);
    if(key.Exists() && key.Open(wxRegKey::Read))
    {
        wxString dir;
        key.QueryValue(wxT_2("RootDir"), dir);
        if (!dir.IsEmpty() && wxDirExists(dir))
        {
            m_MasterPath = dir;
        }
        key.Close();
    }

    wxString sep = wxString(wxFileName::GetPathSeparator());
    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib") + sep + wxT_2("psdk"));
    }

    return wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // __WXMSW__
