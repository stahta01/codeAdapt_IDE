/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2007  Mario Cupelli (HighTec EDV-Systeme GmbH)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Mario Cupelli <codeblocks@hightec-rt.com>
* Program URL   : http://www.codeblocks.org
*
* $Revision: 1.17 $
* $Id: compilerGNUTRICORE.cpp,v 1.17 2007/09/06 10:16:33 mario Exp $
* $HeadURL: $
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
#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif // __WXMSW__
#include "compilerGNUTRICORE.h"

CompilerGNUTRICORE::CompilerGNUTRICORE()
        : Compiler(_("GNU GCC Compiler for TriCore"),wxT_2("tricore-gcc"))
{
    Reset();
}

CompilerGNUTRICORE::~CompilerGNUTRICORE()
{
    //dtor
}

Compiler * CompilerGNUTRICORE::CreateCopy()
{
    Compiler* c = new CompilerGNUTRICORE(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGNUTRICORE::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("tricore-gcc.exe");
        m_Programs.CPP = wxT_2("tricore-g++.exe");
        m_Programs.LD = wxT_2("tricore-g++.exe");
        m_Programs.DBG = wxT_2("tricore-insight.exe");
        m_Programs.LIB = wxT_2("tricore-ar.exe");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("tricore-gcc");
        m_Programs.CPP = wxT_2("tricore-g++");
        m_Programs.LD = wxT_2("tricore-g++");
        m_Programs.DBG = wxT_2("tricore-insight");
        m_Programs.LIB = wxT_2("tricore-ar");
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
    m_Switches.logging = clogFull;
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
                        _("You have optimizations enabled. This will make debugging difficult because variables may be optimized away etc."));
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

    category = _("General Options");

    // general options
    m_Options.AddOption(_("Output an error if same variable is declared without extern in different modules"), wxT_2("-fno-common"), category);
    m_Options.AddOption(_("Do not allocate to an enum type only as many bytes as it needs for the declared range of possible values"), wxT_2("-fno-short-enums"), category);
    m_Options.AddOption(_("Save intermediate files in the build directory"), wxT_2("-save-temps"), category);

    // Startup options
    category = _("Linker and startup code");
    m_Options.AddOption(_("do not link against the default crt0.o, so you can add your own startup code (TriCore specific)"), wxT_2("-nocrt0"), category);
    m_Options.AddOption(_("do not link against standard system startup files"), wxT_2("-nostartfiles"), category);
    m_Options.AddOption(_("only search library directories explicitly specified on the command line"), wxT_2("-nostdlib"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Strip all symbols from binary (minimizes size)"), wxT_2(""), category, wxT_2("-s"), true, wxT_2("-g -ggdb"), _("Stripping the binary will strip debugging symbols as well!"));
    m_Options.AddOption(_("Optimize generated code (for speed)"), wxT_2("-O"), category);
    m_Options.AddOption(_("Optimize more (for speed)"), wxT_2("-O1"), category);
    m_Options.AddOption(_("Optimize even more (for speed)"), wxT_2("-O2"), category);
    m_Options.AddOption(_("Optimize fully (for speed)"), wxT_2("-O3"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), wxT_2("-Os"), category);
    m_Options.AddOption(_("Expensive optimizations"), wxT_2("-fexpensive-optimizations"), category);
    m_Options.AddOption(_("No instruction scheduling before reload"), wxT_2("-fno-schedule-insns"), category);
    m_Options.AddOption(_("No instruction scheduling after reload"), wxT_2("-fno-schedule-insns2"), category);

    // machine dependent options
    category = _("TriCore achitecture specific");

    m_Options.AddOption(_("jump tables in text section"), wxT_2("-mjumptable-in-textsection"), category);
    m_Options.AddOption(_("include C source lines into assembler output"), wxT_2("-masm-source-lines"), category);
    m_Options.AddOption(_("base relative addresses for small data objects"), wxT_2("-msmall-pid"), category);
    m_Options.AddOption(_("allow _bit in structures and unions"), wxT_2("-mbits-struct-unions"), category);
    m_Options.AddOption(_("allow callee and caller in different sections"), wxT_2("-mnocallerrors"), category);
    m_Options.AddOption(_("allocate data in aligned section"), wxT_2("-maligned-data-sections"), category);
    m_Options.AddOption(_("enable attribute alignedaccess"), wxT_2("-maligned-access"), category);
    m_Options.AddOption(_("use optimized single float emulation"), wxT_2("-moptfp"), category);
    m_Options.AddOption(_("layout bitfields EABI conform"), wxT_2("-meabi-bitfields"), category);
    m_Options.AddOption(_("EABI conform"), wxT_2("-meabi"), category);

    // machine dependent options
    category = _("TriCore MCU derivatives");

    m_Options.AddOption(_("TriCore TC1130"), wxT_2("-mcpu=tc1130"), category);
    m_Options.AddOption(_("TriCore TC1130 BA step"), wxT_2("-mcpu=tc1130ba"), category);
    m_Options.AddOption(_("TriCore TC1161"), wxT_2("-mcpu=tc1161"), category);
    m_Options.AddOption(_("TriCore TC1162"), wxT_2("-mcpu=tc1162"), category);
    m_Options.AddOption(_("TriCore TC1762"), wxT_2("-mcpu=tc1762"), category);
    m_Options.AddOption(_("TriCore TC1765"), wxT_2("-mcpu=tc1765"), category);
    m_Options.AddOption(_("TriCore TC1766"), wxT_2("-mcpu=tc1766"), category);
    m_Options.AddOption(_("TriCore TC1775"), wxT_2("-mcpu=tc1775"), category);
    m_Options.AddOption(_("TriCore TC1792"), wxT_2("-mcpu=tc1792"), category);
    m_Options.AddOption(_("TriCore TC1796"), wxT_2("-mcpu=tc1796"), category);
    m_Options.AddOption(_("TriCore TC1920"), wxT_2("-mcpu=tc1920"), category);

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
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -rs $static_output $link_objects")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerGNUTRICORE::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("'In function...' info"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):[ \t]+") + wxT_2("([iI]n ([cC]lass|[cC]onstructor|[dD]estructor|[fF]unction|[mM]ember [fF]unction).*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("'Instantiated from' info"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]+([iI]nstantiated from .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error"), cltError, wxT_2("windres.exe:[ \t](") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Resource compiler error (2)"), cltError, wxT_2("windres.exe:[ \t](.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Preprocessor warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 4, 1, 2));
    m_RegExes.Add(RegExStruct(_("Preprocessor error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler note"), cltInfo, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Nn]ote:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("General note"), cltInfo, wxT_2("([Nn]ote:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Ww]arning:[ \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9]+:[ \t](.*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker error (2)"), cltError, FilePathWithSpaces + wxT_2("\\(.text\\+[0-9A-Za-z]+\\):([ \tA-Za-z0-9_:+/\\.-]+):[ \t](.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Linker error (lib not found)"), cltError, wxT_2(".*(ld.*):[ \t](cannot find.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("Undefined reference"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):[ \t](undefined reference.*)"), 2, 1));
    m_RegExes.Add(RegExStruct(_("General warning"), cltWarning, wxT_2("([Ww]arning:[ \t].*)"), 1));
    m_RegExes.Add(RegExStruct(_("Auto-import info"), cltInfo, wxT_2("([Ii]nfo:[ \t].*)\\(auto-import\\)"), 1));
}

AutoDetectResult CompilerGNUTRICORE::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
#ifdef __WXMSW__
    m_MasterPath = wxT_2("C:\\HighTec\\TriCore"); // just a guess

    //    wxLogNull ln;
    wxRegKey key; // defaults to HKCR
    key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\HighTec EDV-Systeme\\TriCore\\"));
    if (key.Exists() && key.Open(wxRegKey::Read))
    {
     // found; read it
    	if (key.HasValue(wxT_2("InstallPath")))
	{
		key.QueryValue(wxT_2("InstallPath"), m_MasterPath);
        }
     }
#else
        m_MasterPath = wxT_2("/usr/local/tricore");
#endif // __WXMSW__
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
          AddIncludeDir(m_MasterPath + sep + wxT_2("tricore") + sep + wxT_2("include"));
    }
    return ret;
}
