/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#include <prep.h>
#include "compilerSDCC.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerSDCC::CompilerSDCC()
    : Compiler(_("SDCC Compiler"), wxT_2("sdcc"))
{
    Reset();
}

CompilerSDCC::~CompilerSDCC()
{
    //dtor
}

Compiler * CompilerSDCC::CreateCopy()
{
    Compiler* c = new CompilerSDCC(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

void CompilerSDCC::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("sdcc.exe");
        m_Programs.CPP = wxT_2("sdcc.exe");
        m_Programs.LD = wxT_2("sdcc.exe");
        m_Programs.DBG = wxT_2("sdcdb.exe");
        m_Programs.LIB = wxT_2("sdcclib.exe");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("sdcc");
        m_Programs.CPP = wxT_2("sdcc");
        m_Programs.LD = wxT_2("sdcc");
        m_Programs.DBG = wxT_2("sdcdb");
        m_Programs.LIB = wxT_2("sdcclib");
        m_Programs.WINDRES = wxT_2("");
        m_Programs.MAKE = wxT_2("make");
    }
    m_Switches.includeDirs = wxT_2("-I");
    m_Switches.libDirs = wxT_2("-L");
    m_Switches.linkLibs = wxT_2("-l");
    m_Switches.defines = wxT_2("-D");
    m_Switches.genericSwitch = wxT_2("-");
    m_Switches.objectExtension = wxT_2("rel");

    m_Switches.needDependencies = true;
    m_Switches.forceCompilerUseQuotes = false;
    m_Switches.forceLinkerUseQuotes = false;
    m_Switches.logging = clogSimple; // clogFull;
    m_Switches.libPrefix = wxT_2("lib");
    m_Switches.libExtension = wxT_2("lib");
    m_Switches.linkerNeedsLibPrefix = false;
    m_Switches.linkerNeedsLibExtension = false;

    // Summary of SDCC options: http://sdcc.sourceforge.net

    m_Options.ClearOptions();

    wxString category = _("General");
    m_Options.AddOption(_("Produce debugging symbols"), wxT_2("--debug"), category, wxT_2(""),
                true,
                wxT_2("--opt-code-speed --opt-code-size"),
                _("You have optimizations enabled. This is Not A Good Thing(tm) when producing debugging symbols..."));

    m_Options.AddOption(_("All functions will be compiled as reentrant"), wxT_2("--stack-auto"), category);
    m_Options.AddOption(_("Be verbose"), wxT_2("--verbose"), category);
    m_Options.AddOption(_("Generate extra profiling information"), wxT_2("--profile"), category);
    m_Options.AddOption(_("Callee will always save registers used"), wxT_2("--all-callee-saves"), category);
    m_Options.AddOption(_("Leave out the frame pointer"), wxT_2("--fommit-frame-pointer"), category);
    m_Options.AddOption(_("[MCS51/DS390] - use Bank1 for parameter passing"), wxT_2("--parms-in-bank1"), category);

    // optimization
    category = _("Optimization");
    m_Options.AddOption(_("Optimize generated code (for speed)"), wxT_2("--opt-code-speed"), category);
    m_Options.AddOption(_("Optimize generated code (for size)"), wxT_2("--opt-code-size"), category);

    // machine dependent options - cpu arch
    category = _("CPU architecture (choose none, or only one of these)");
    m_Options.AddOption(_("[CPU] Intel MCS51 (default)"), wxT_2("-mmcs51"), category);
    m_Options.AddOption(_("[CPU] Dallas DS80C390"), wxT_2("-mds390"), category);
    m_Options.AddOption(_("[CPU] Dallas DS80C400"), wxT_2("-mds400"), category);
    m_Options.AddOption(_("[CPU] Freescale/Motorola HC08"), wxT_2("-mhc08"), category);
    m_Options.AddOption(_("[CPU] Zilog Z80"), wxT_2("-mz80"), category);
    m_Options.AddOption(_("[CPU] GameBoy Z80 (Not actively maintained)."), wxT_2("-mgbz80"), category);
    m_Options.AddOption(_("[CPU] Atmel AVR (In development, not complete)"), wxT_2("-mavr"), category);
    m_Options.AddOption(_("[CPU] Microchip PIC 14-bit (p16f84 and variants. In development, not complete)"), wxT_2("-mpic14"), category);
    m_Options.AddOption(_("[CPU] PIC 16-bit (p18f452 and variants. In development, not complete)"), wxT_2("-mpic16"), category);

    // MCS51 dependent options
    category = _("MCS51 Options");
    m_Options.AddOption(_("[MCS51] Large model programs (default is Small)"), wxT_2("--model-large"), category);
    m_Options.AddOption(_("[MCS51] Use a pseudo stack in the first 256 bytes in the external ram"), wxT_2("--xstack"), category);
    m_Options.AddOption(_("[MCS51] Linker use old style for allocating memory areas."), wxT_2("--no-pack-iram"), category);

    // DS390 / DS400 Options
    category = _("DS390 / DS400 Options");
    m_Options.AddOption(_("[DS390 / DS400] Generate 24-bit flat mode code"), wxT_2("--model-flat24"), category);
    m_Options.AddOption(_("[DS390 / DS400] Disable interrupts during ESP:SP updates"), wxT_2("--protect-sp-update"), category);
    m_Options.AddOption(_("[DS390 / DS400] Insert call to function __stack_probe at each function prologue"), wxT_2("--stack-probe"), category);
    m_Options.AddOption(_("[DS390 / DS400] Generate code for DS390 Arithmetic Accelerator"), wxT_2("--use-accelerator"), category);

    // Z80 Options
    category = _("Z80 Options");
    m_Options.AddOption(_("[Z80] Force a called function to always save BC"), wxT_2("--callee-saves-bc"), category);
    m_Options.AddOption(_("[Z80] When linking, skip the standard crt0.o object file"), wxT_2("--no-std-crt0"), category);

    // Linker output format options
    category = _("Linker output format (choose none, or only one of these)");
    m_Options.AddOption(_("Output Intel Hex (default)"), wxT_2("--out-fmt-ihx"), category);
    m_Options.AddOption(_("Output Motorola S19"), wxT_2("--out-fmt-s19"), category);
    m_Options.AddOption(_("Output ELF (Currently only supported for the HC08 processors)"), wxT_2("--out-fmt-elf"), category);

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(wxT_2("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
    m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $options $link_options $libs $link_objects")));
    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $options $link_options $libs $link_objects")));
    //m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -r $static_output $link_objects\n\tranlib $exe_output")));
    m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // unsupported currently

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
}

void CompilerSDCC::LoadDefaultRegExArray()
{
    m_RegExes.Clear();
    m_RegExes.Add(RegExStruct(_("Fatal error"), cltError, wxT_2("FATAL:[ \t]*(.*)"), 1));
    m_RegExes.Add(RegExStruct(_("Compiler warning (.h)"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9:]+[ \t]([Ww]arning[: \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler warning"), cltWarning, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t]([Ww]arning[: \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[ \t](.*[Ee]rror[: \t].*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Compiler error (2)"), cltError, wxT_2("(") + FilePathWithSpaces + wxT_2("):([0-9]+):[0-9:]+ (.*: No such .*)"), 3, 1, 2));
    m_RegExes.Add(RegExStruct(_("Linker warning"), cltWarning, wxT_2("(ASlink-Warning-.*)"), 1));
}

AutoDetectResult CompilerSDCC::AutoDetectInstallationDir()
{
    if (platform::windows)
    {
#ifdef __WXMSW__ // for wxRegKey
        wxRegKey key;   // defaults to HKCR
        key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\SDCC"));
        if (key.Exists() && key.Open(wxRegKey::Read)) // found; read it
            key.QueryValue(wxT_2("UninstallString"), m_MasterPath);
#endif

        if (m_MasterPath.IsEmpty())
            // just a guess; the default installation dir
            m_MasterPath = wxT_2("C:\\sdcc");
        else {
            wxFileName fn(m_MasterPath);
            m_MasterPath = fn.GetPath();
        }

        if (!m_MasterPath.IsEmpty())
        {
            AddIncludeDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("include"));
            AddLibDir(m_MasterPath + wxFILE_SEP_PATH + wxT_2("lib"));
            m_ExtraPaths.Add(m_MasterPath + wxFILE_SEP_PATH + wxT_2("bin"));
        }
    }
    else
        m_MasterPath=wxT_2("/usr/local/bin"); // default

    return wxFileExists(m_MasterPath + wxFILE_SEP_PATH + wxT_2("bin") + wxFILE_SEP_PATH + m_Programs.C) ? adrDetected : adrGuessed;
}
