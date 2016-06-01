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
* $Id: compilerGNUMSP430.cpp,v 1.17 2007/09/06 10:16:33 mario Exp $
* $HeadURL: $
*/

#include <sdk.h>
#include <prep.h>
#ifndef CB_PRECOMP
    #include <wx/intl.h>
    #include <wx/regex.h>
    #include <wx/utils.h> // wxGetOSDirectory, wxGetEnv
#endif
#include <wx/filefn.h> // wxFileExists
#include <wx/fileconf.h> // wxFileConfig
#include <wx/msgdlg.h>
#ifdef __WXMSW__
#include <wx/msw/registry.h>
#endif // __WXMSW__
#include "compilerGNUMSP430.h"

CompilerGNUMSP430::CompilerGNUMSP430()
        : Compiler(_("GNU GCC Compiler for MSP430"),wxT_2("msp430-gcc"))
{
    Reset();
}

CompilerGNUMSP430::~CompilerGNUMSP430()
{
    //dtor
}

Compiler * CompilerGNUMSP430::CreateCopy()
{
    Compiler* c = new CompilerGNUMSP430(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerGNUMSP430::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("msp430-gcc.exe");
        m_Programs.CPP = wxT_2("msp430-g++.exe");
        m_Programs.LD = wxT_2("msp430-g++.exe");
        m_Programs.DBG = wxT_2("msp430-insight.exe");
        m_Programs.LIB = wxT_2("msp430-ar.exe");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("msp430-gcc");
        m_Programs.CPP = wxT_2("msp430-g++");
        m_Programs.LD = wxT_2("msp430-g++");
        m_Programs.DBG = wxT_2("msp430-insight");
        m_Programs.LIB = wxT_2("msp430-ar");
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
    m_Options.AddOption(_("Save intermediate files in the build directory"), wxT_2("-save-temps"), category);

    // Startup options
    category = _("Linker and startup code");
    m_Options.AddOption(_("do not link against the default crt0.o, so you can add your own startup code (MSP430 specific)"), wxT_2("-nocrt0"), category);
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
    category = _("MSP430 achitecture specific");

    m_Options.AddOption(_("Use subroutine call for function prologue/epilogue when possible"), wxT_2("-msave-prologue"), category);
    m_Options.AddOption(_("Do not perform volatile workaround for bitwise operations"), wxT_2("-mno-volatile-workaround"), category);
    m_Options.AddOption(_("No stack init in main()"), wxT_2("-mno-stack-init"), category);
    m_Options.AddOption(_("Produce IAR assembler syntax"), wxT_2("-mIAR"), category);
    m_Options.AddOption(_("Assume interrupt routine does not do hardware multiply"), wxT_2("-mnoint-hwmul"), category);
    m_Options.AddOption(_("Issue inline multiplication code for 32-bit integers"), wxT_2("-minline-hwmul"), category);
    m_Options.AddOption(_("Disable hardware multiplier"), wxT_2("-mdisable-hwmul"), category);
    m_Options.AddOption(_("Force hardware multiplier"), wxT_2("-mforce-hwmul"), category);
    m_Options.AddOption(_("Strict alignment for all structures"), wxT_2("-mstrict-align"), category);
    m_Options.AddOption(_("Add stack information to profiler"), wxT_2("-mpgr"), category);
    m_Options.AddOption(_("Add library profile information"), wxT_2("-mpgl"), category);
    m_Options.AddOption(_("Add ordinary profile information"), wxT_2("-mpgs"), category);
    m_Options.AddOption(_("Jump to specified routine at the end of main()"), wxT_2("-mendup-at="), category);
    m_Options.AddOption(_("Specify the initial stack address"), wxT_2("-minit-stack="), category);
    m_Options.AddOption(_("enable relaxation at assembly time"), wxT_2("-mQ"), category);
    m_Options.AddOption(_("enable polymorph instructions"), wxT_2("-mP"), category);

    // machine dependent options
    category = _("MSP430 MCU derivatives");

    m_Options.AddOption(_("MSP430 MSP1"), wxT_2("-mmcu=msp1"), category);
    m_Options.AddOption(_("MSP430 MSP2"), wxT_2("-mmcu=msp2"), category);
    m_Options.AddOption(_("MSP430 110"), wxT_2("-mmcu=msp430x110"), category);
    m_Options.AddOption(_("MSP430 112"), wxT_2("-mmcu=msp430x112"), category);
    m_Options.AddOption(_("MSP430 1101"), wxT_2("-mmcu=msp430x1101"), category);
    m_Options.AddOption(_("MSP430 1111"), wxT_2("-mmcu=msp430x1111"), category);
    m_Options.AddOption(_("MSP430 1121"), wxT_2("-mmcu=msp430x1121"), category);
    m_Options.AddOption(_("MSP430 1122"), wxT_2("-mmcu=msp430x1122"), category);
    m_Options.AddOption(_("MSP430 1132"), wxT_2("-mmcu=msp430x1132"), category);
    m_Options.AddOption(_("MSP430 122"), wxT_2("-mmcu=msp430x122"), category);
    m_Options.AddOption(_("MSP430 123"), wxT_2("-mmcu=msp430x123"), category);
    m_Options.AddOption(_("MSP430 1222"), wxT_2("-mmcu=msp430x1222"), category);
    m_Options.AddOption(_("MSP430 1232"), wxT_2("-mmcu=msp430x1232"), category);
    m_Options.AddOption(_("MSP430 133"), wxT_2("-mmcu=msp430x133"), category);
    m_Options.AddOption(_("MSP430 135"), wxT_2("-mmcu=msp430x135"), category);
    m_Options.AddOption(_("MSP430 1331"), wxT_2("-mmcu=msp430x1331"), category);
    m_Options.AddOption(_("MSP430 1351"), wxT_2("-mmcu=msp430x1351"), category);
    m_Options.AddOption(_("MSP430 147"), wxT_2("-mmcu=msp430x147"), category);
    m_Options.AddOption(_("MSP430 148"), wxT_2("-mmcu=msp430x148"), category);
    m_Options.AddOption(_("MSP430 149"), wxT_2("-mmcu=msp430x149"), category);
    m_Options.AddOption(_("MSP430 1471"), wxT_2("-mmcu=msp430x1471"), category);
    m_Options.AddOption(_("MSP430 1481"), wxT_2("-mmcu=msp430x1481"), category);
    m_Options.AddOption(_("MSP430 1491"), wxT_2("-mmcu=msp430x1491"), category);
    m_Options.AddOption(_("MSP430 155"), wxT_2("-mmcu=msp430x155"), category);
    m_Options.AddOption(_("MSP430 156"), wxT_2("-mmcu=msp430x156"), category);
    m_Options.AddOption(_("MSP430 157"), wxT_2("-mmcu=msp430x157"), category);
    m_Options.AddOption(_("MSP430 167"), wxT_2("-mmcu=msp430x167"), category);
    m_Options.AddOption(_("MSP430 168"), wxT_2("-mmcu=msp430x168"), category);
    m_Options.AddOption(_("MSP430 169"), wxT_2("-mmcu=msp430x169"), category);
    m_Options.AddOption(_("MSP430 1610"), wxT_2("-mmcu=msp430x1610"), category);
    m_Options.AddOption(_("MSP430 1611"), wxT_2("-mmcu=msp430x1611"), category);
    m_Options.AddOption(_("MSP430 1612"), wxT_2("-mmcu=msp430x1612"), category);
    m_Options.AddOption(_("MSP430 2001"), wxT_2("-mmcu=msp430x2001"), category);
    m_Options.AddOption(_("MSP430 2011"), wxT_2("-mmcu=msp430x2011"), category);
    m_Options.AddOption(_("MSP430 2002"), wxT_2("-mmcu=msp430x2002"), category);
    m_Options.AddOption(_("MSP430 2012"), wxT_2("-mmcu=msp430x2012"), category);
    m_Options.AddOption(_("MSP430 2003"), wxT_2("-mmcu=msp430x2003"), category);
    m_Options.AddOption(_("MSP430 2013"), wxT_2("-mmcu=msp430x2013"), category);
    m_Options.AddOption(_("MSP430 2101"), wxT_2("-mmcu=msp430x2101"), category);
    m_Options.AddOption(_("MSP430 2111"), wxT_2("-mmcu=msp430x2111"), category);
    m_Options.AddOption(_("MSP430 2121"), wxT_2("-mmcu=msp430x2121"), category);
    m_Options.AddOption(_("MSP430 2131"), wxT_2("-mmcu=msp430x2131"), category);
    m_Options.AddOption(_("MSP430 2234"), wxT_2("-mmcu=msp430x2234"), category);
    m_Options.AddOption(_("MSP430 2254"), wxT_2("-mmcu=msp430x2254"), category);
    m_Options.AddOption(_("MSP430 2274"), wxT_2("-mmcu=msp430x2274"), category);
    m_Options.AddOption(_("MSP430 311"), wxT_2("-mmcu=msp430x311"), category);
    m_Options.AddOption(_("MSP430 312"), wxT_2("-mmcu=msp430x312"), category);
    m_Options.AddOption(_("MSP430 313"), wxT_2("-mmcu=msp430x313"), category);
    m_Options.AddOption(_("MSP430 314"), wxT_2("-mmcu=msp430x314"), category);
    m_Options.AddOption(_("MSP430 315"), wxT_2("-mmcu=msp430x315"), category);
    m_Options.AddOption(_("MSP430 323"), wxT_2("-mmcu=msp430x323"), category);
    m_Options.AddOption(_("MSP430 325"), wxT_2("-mmcu=msp430x325"), category);
    m_Options.AddOption(_("MSP430 336"), wxT_2("-mmcu=msp430x336"), category);
    m_Options.AddOption(_("MSP430 337"), wxT_2("-mmcu=msp430x337"), category);
    m_Options.AddOption(_("MSP430 412"), wxT_2("-mmcu=msp430x412"), category);
    m_Options.AddOption(_("MSP430 413"), wxT_2("-mmcu=msp430x413"), category);
    m_Options.AddOption(_("MSP430 415"), wxT_2("-mmcu=msp430x415"), category);
    m_Options.AddOption(_("MSP430 417"), wxT_2("-mmcu=msp430x417"), category);
    m_Options.AddOption(_("MSP430 423"), wxT_2("-mmcu=msp430x423"), category);
    m_Options.AddOption(_("MSP430 425"), wxT_2("-mmcu=msp430x425"), category);
    m_Options.AddOption(_("MSP430 427"), wxT_2("-mmcu=msp430x427"), category);
    m_Options.AddOption(_("MSP430 4250"), wxT_2("-mmcu=msp430x4250"), category);
    m_Options.AddOption(_("MSP430 4260"), wxT_2("-mmcu=msp430x4260"), category);
    m_Options.AddOption(_("MSP430 4270"), wxT_2("-mmcu=msp430x4270"), category);
    m_Options.AddOption(_("MSP430 E423"), wxT_2("-mmcu=msp430xE423"), category);
    m_Options.AddOption(_("MSP430 E425"), wxT_2("-mmcu=msp430xE425"), category);
    m_Options.AddOption(_("MSP430 E427"), wxT_2("-mmcu=msp430xE427"), category);
    m_Options.AddOption(_("MSP430 W423"), wxT_2("-mmcu=msp430xW423"), category);
    m_Options.AddOption(_("MSP430 W425"), wxT_2("-mmcu=msp430xW425"), category);
    m_Options.AddOption(_("MSP430 W427"), wxT_2("-mmcu=msp430xW427"), category);
    m_Options.AddOption(_("MSP430 G437"), wxT_2("-mmcu=msp430xG437"), category);
    m_Options.AddOption(_("MSP430 G438"), wxT_2("-mmcu=msp430xG438"), category);
    m_Options.AddOption(_("MSP430 G439"), wxT_2("-mmcu=msp430xG439"), category);
    m_Options.AddOption(_("MSP430 435"), wxT_2("-mmcu=msp430x435"), category);
    m_Options.AddOption(_("MSP430 436"), wxT_2("-mmcu=msp430x436"), category);
    m_Options.AddOption(_("MSP430 437"), wxT_2("-mmcu=msp430x437"), category);
    m_Options.AddOption(_("MSP430 447"), wxT_2("-mmcu=msp430x447"), category);
    m_Options.AddOption(_("MSP430 448"), wxT_2("-mmcu=msp430x448"), category);
    m_Options.AddOption(_("MSP430 449"), wxT_2("-mmcu=msp430x449"), category);
    m_Options.AddOption(_("MSP430 4616"), wxT_2("-mmcu=msp430xG4616"), category);
    m_Options.AddOption(_("MSP430 4617"), wxT_2("-mmcu=msp430xG4617"), category);
    m_Options.AddOption(_("MSP430 4618"), wxT_2("-mmcu=msp430xG4618"), category);
    m_Options.AddOption(_("MSP430 4619"), wxT_2("-mmcu=msp430xG4619"), category);

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

void CompilerGNUMSP430::LoadDefaultRegExArray()
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

AutoDetectResult CompilerGNUMSP430::AutoDetectInstallationDir()
{
    wxString sep = wxFileName::GetPathSeparator();
#ifdef __WXMSW__
	m_MasterPath = wxT_2("C:\\HighTec\\Msp430"); // just a guess

    //    wxLogNull ln;
        wxRegKey key; // defaults to HKCR
        key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\HighTec EDV-Systeme\\Msp430\\"));
        if (key.Exists() && key.Open(wxRegKey::Read))
        {
            // found; read it
            if (key.HasValue(wxT_2("InstallPath")))
            {
                key.QueryValue(wxT_2("InstallPath"), m_MasterPath);
            }
        }
#else
        m_MasterPath = wxT_2("/usr/local/msp430");
#endif // __WXMSW__
    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    if (ret == adrDetected)
    {
          AddIncludeDir(m_MasterPath + sep + wxT_2("msp430") + sep + wxT_2("include"));
    }
    return ret;
}
