/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision: 3167 $
* $Id: compilerTcc.cpp 3167 2006-11-02 09:07:52Z killerbot $
* $HeadURL: svn+ssh://killerbot@svn.berlios.de/svnroot/repos/codeblocks/trunk/src/plugins/compilergcc/compilerTcc.cpp $
*/

#include "ca/sdk.h"
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/string.h>
#endif
#include <wx/filefn.h> // wxFileExists
#include "compilerTcc.h"

CompilerTcc::CompilerTcc()
    : Compiler(_("Tiny C Compiler"), wxT_2("tcc"))
{
    Reset();
}

CompilerTcc::~CompilerTcc()
{
    //dtor
}

Compiler * CompilerTcc::CreateCopy()
{
    Compiler* c = new CompilerTcc(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
} // end of CreateCopy

void CompilerTcc::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("tcc.exe");
        m_Programs.CPP = wxT_2("");
        m_Programs.LD = wxT_2("tcc.exe");
        m_Programs.DBG = wxT_2("gdb.exe");
        m_Programs.LIB = wxT_2("tcc.exe");
        m_Programs.WINDRES = wxT_2("windres.exe");
        m_Programs.MAKE = wxT_2("mingw32-make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("tcc");
        m_Programs.CPP = wxT_2("");
        m_Programs.LD = wxT_2("tcc");
        m_Programs.DBG = wxT_2("gdb");
        m_Programs.LIB = wxT_2("tcc");
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
    m_Switches.supportsPCH = false;
    m_Switches.PCHExtension = wxT_2("");

    // Summary of tcc options

    m_Options.ClearOptions();
    m_Options.AddOption(_("Produce debugging symbols"),
                wxT_2("-g"),
                _("Debugging"));

    wxString category = _("Warnings");

    // warnings
    m_Options.AddOption(_("Enable all compiler warnings (overrides every other setting)"), wxT_2("-Wall"), category);
    m_Options.AddOption(_("Disable all warnings"), wxT_2("-w"), category);
    m_Options.AddOption(_("Abort compilation if warnings are issued"), wxT_2("-Werror"), category);
    m_Options.AddOption(_("Inhibit all warning messages"), wxT_2("-w"), category);


    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(wxT_2("")));
    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("")));
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
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -r -static -o $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerTcc::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, wxT_2("windres.exe:[ \t](") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, wxT_2("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + wxT_2("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, wxT_2(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, wxT_2("([Ww]arning:[ \t].*)"), 1));
}

AutoDetectResult CompilerTcc::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
    if (platform::windows)
        m_MasterPath = wxT_2("C:\\tcc");
    else
        m_MasterPath = wxT_2("/usr");
    wxString BinPath = m_MasterPath + sep + wxT_2("tcc");
    AutoDetectResult ret = wxFileExists(BinPath + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
        AddIncludeDir(m_MasterPath + sep + wxT_2("include"));
        AddLibDir(m_MasterPath + sep + wxT_2("lib"));
        m_ExtraPaths.Add(BinPath);
    }
    return ret;
} // end of AutoDetectInstallationDir
