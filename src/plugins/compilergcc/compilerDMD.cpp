#if defined(_WIN32) || defined(__linux__)
// this compiler is valid only in windows and linux

#include <sdk.h>
#include <prep.h>
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include "compilerDMD.h"

CompilerDMD::CompilerDMD()
    : Compiler(_("Digital Mars D Compiler"), wxT_2("dmd"))
{
    Reset();
}

CompilerDMD::~CompilerDMD()
{
    //dtor
}

Compiler * CompilerDMD::CreateCopy()
{
    Compiler* c = new CompilerDMD(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerDMD::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("dmd.exe");
        m_Programs.CPP = wxT_2("dmd.exe");
        m_Programs.LD = wxT_2("dmd.exe");
        m_Programs.LIB = wxT_2("lib.exe");
        m_Programs.DBG = wxT_2("windbg.exe");
        m_Programs.WINDRES = wxT_2("rcc.exe");
        m_Programs.MAKE = wxT_2("make.exe");

        m_Switches.includeDirs = wxT_2("-I");
        m_Switches.libDirs = wxT_2("");
        m_Switches.linkLibs = wxT_2("");
        m_Switches.libPrefix = wxT_2("");
        m_Switches.libExtension = wxT_2("lib");
        m_Switches.defines = wxT_2("-version=");
        m_Switches.genericSwitch = wxT_2("-");
        m_Switches.objectExtension = wxT_2("obj");
        m_Switches.needDependencies = false;
        m_Switches.forceCompilerUseQuotes = false;
        m_Switches.forceLinkerUseQuotes = true;
        m_Switches.logging = clogSimple;
        m_Switches.linkerNeedsLibPrefix = false;
        m_Switches.linkerNeedsLibExtension = true;

        // FIXME (hd#1#): should be work on: we need $res_options
        m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("$rescomp -o$resource_output $res_includes $file -32 -r")));
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker $exe_output $link_options $link_objects $link_resobjects $libs")));
        m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker $exe_output $link_options $link_objects $link_resobjects $libs")));
    }
    else
    {
        m_Programs.C = wxT_2("dmd");
        m_Programs.CPP = wxT_2("dmd");
        m_Programs.LD = wxT_2("gcc");
        m_Programs.LIB = wxT_2("ar");
        m_Programs.DBG = wxT_2("gdb");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make");

        m_Switches.includeDirs = wxT_2("-I");
        m_Switches.libDirs = wxT_2("-L");
        m_Switches.linkLibs = wxT_2("-l");
        m_Switches.libPrefix = wxT_2("lib");
        m_Switches.libExtension = wxT_2("a");
        m_Switches.defines = wxT_2("-version=");
        m_Switches.genericSwitch = wxT_2("-");
        m_Switches.objectExtension = wxT_2("o");
        m_Switches.needDependencies = false;
        m_Switches.forceCompilerUseQuotes = false;
        m_Switches.forceLinkerUseQuotes = false;
        m_Switches.logging = clogSimple;
        m_Switches.linkerNeedsLibPrefix = false;
        m_Switches.linkerNeedsLibExtension = false;

        m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("")));
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker -o $exe_output $link_options $link_objects $libs")));
        m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker -o $exe_output $link_options $link_objects $libs")));
    }

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -of$object")));
    m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker $exe_output $link_options $link_objects $libs $link_resobjects")));
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker $link_options $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    m_Options.ClearOptions();

    //. m_Options.AddOption(_("Alignment of struct members"), "-a[1|2|4|8]", _("Architecture"));
    //m_Options.AddOption(_("compile only, do not link"), wxT_2("-c"), _("D Features"));
    m_Options.AddOption(_("instrument for code coverage analysis"), wxT_2("-cov"), _("D Features"));
    m_Options.AddOption(_("generate documentation from source"), wxT_2("-D"), _("D Features"));
    m_Options.AddOption(_("allow deprecated features"), wxT_2("-d"), _("D Features"));
    m_Options.AddOption(_("compile in debug code"), wxT_2("-debug"), _("D Features"));
    m_Options.AddOption(_("add symbolic debug info"), wxT_2("-g"), _("D Features"));
    m_Options.AddOption(_("generate D interface file"), wxT_2("-H"), _("Others"));
    m_Options.AddOption(_("inline expand functions"), wxT_2("-inline"), _("Optimize"));
    m_Options.AddOption(_("optimize"), wxT_2("-O"), _("D Features"));
    m_Options.AddOption(_("suppress generation of object file"), wxT_2("-o-"), _("D Features"));
    m_Options.AddOption(_("do not strip path from .d source files for object files"), wxT_2("-op"), _("D Features"));
    m_Options.AddOption(_("profile the runtime performance of the generated code"), wxT_2("-profile"), _("Debugging"));
    m_Options.AddOption(_("suppress non-essential compiler messages"), wxT_2("-quiet"), _("Others"));
    m_Options.AddOption(_("compile release version, which means not generating code for contracts and asserts"), wxT_2("-release"), _("D Features"));
    m_Options.AddOption(_("compile in unittest code, also turns on asserts"), wxT_2("-unittest"), _("Debugging"));
    m_Options.AddOption(_("verbose"), wxT_2("-v"), _("Others"));
    m_Options.AddOption(_("enable warnings"), wxT_2("-w"), _("Others"));

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    if (!platform::windows)
    {
      m_LinkLibs.Add(_("pthread"));
      m_LinkLibs.Add(_("m"));
    }
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerDMD::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltError, wxT_2("warning - (") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\):[ \\t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2(")\\(([0-9]+)\\):[ \\t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("Error ([0-9]+):[\\s]*(.*)"), 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltError, wxT_2("Error ([0-9]+):[\\s]*(.*)"), 2));
}

AutoDetectResult CompilerDMD::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();

    // NOTE (hd#1#): dmc uses sc.ini for compiler's master directories
    // NOTE (mandrav#1#): which doesn't seem to exist if you don't have the CD version ;)

    // just a guess; the default installation dir
    wxString incPath;
    wxString libPath;
    wxString libName;
    if (platform::windows)
    {
        m_MasterPath = wxT_2("C:\\dmd");
        incPath = m_MasterPath + sep + wxT_2("src") + sep + wxT_2("phobos");
        libPath = m_MasterPath + sep + wxT_2("lib");
        libName = wxT_2("phobos.lib");
        m_ExtraPaths.Add(wxT_2("C:\\dm\\bin"));
    }
    else
    {
      m_MasterPath = wxFileExists(wxT_2("/usr/local/bin/dmd")) ? wxT_2("/usr/local") : wxT_2("/usr");
      incPath = m_MasterPath + sep + wxT_2("lib") + sep + wxT_2("phobos");
      libPath = m_MasterPath + sep + wxT_2("lib");
      libName = wxT_2("phobos");
    }

    if (!m_MasterPath.IsEmpty())
    {
        AddIncludeDir(incPath);
        AddLibDir(libPath);
    }
    AddLinkLib(libName);

    return wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
}

#endif // _WIN32 || linux
