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

#include "ca/sdk.h"
#include <prep.h>
#include "compilerMINGW.h"
#include <wx/intl.h>
#include <wx/regex.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include "manager.h"
#include "logmanager.h"
#include "compilerMINGWgenerator.h"
#include "ca/filename.h"

#include <configmanager.h>

#ifdef __WXMSW__
    #include <wx/msw/registry.h>
#endif

CompilerMINGW::CompilerMINGW(const wxString& name, const wxString& ID)
    : Compiler(name, ID)
{
    Reset();
}

CompilerMINGW::~CompilerMINGW()
{
    //dtor
}

Compiler * CompilerMINGW::CreateCopy()
{
    Compiler* c = new CompilerMINGW(*this);
    c->SetExtraPaths(m_ExtraPaths); // wxArrayString doesn't seem to be copied with the default copy ctor...
    return c;
}

CompilerCommandGenerator* CompilerMINGW::GetCommandGenerator()
{
    return new CompilerMINGWGenerator;
}

void CompilerMINGW::Reset()
{
    if (platform::windows)
    {
        m_Programs.C = wxT_2("mingw32-gcc.exe");
        m_Programs.CPP = wxT_2("mingw32-g++.exe");
        m_Programs.LD = wxT_2("mingw32-g++.exe");
        m_Programs.DBG = wxT_2("gdb.exe");
        m_Programs.LIB = wxT_2("ar.exe");
        m_Programs.WINDRES = wxT_2("windres.exe");
        m_Programs.MAKE = wxT_2("mingw32-make.exe");
    }
    else
    {
        m_Programs.C = wxT_2("gcc");
        m_Programs.CPP = wxT_2("g++");
        m_Programs.LD = wxT_2("g++");
        m_Programs.DBG = wxT_2("gdb");
        m_Programs.LIB = wxT_2("ar");
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
    m_Switches.supportsPCH = true;
    m_Switches.PCHExtension = wxT_2("h.gch");
    m_Switches.UseFullSourcePaths = true; // use the GDB workaround !!!!!!!!

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
    category = _("CPU architecture tuning (choose none, or only one of these)");
    m_Options.AddOption(_("i386"), wxT_2("-march=i386"), category);
    m_Options.AddOption(_("i486"), wxT_2("-march=i486"), category);
    m_Options.AddOption(_("Intel Pentium"), wxT_2("-march=i586"), category);
    m_Options.AddOption(_("Intel Pentium (MMX)"), wxT_2("-march=pentium-mmx"), category);
    m_Options.AddOption(_("Intel Pentium PRO"), wxT_2("-march=i686"), category);
    m_Options.AddOption(_("Intel Pentium 2 (MMX)"), wxT_2("-march=pentium2"), category);
    m_Options.AddOption(_("Intel Pentium 3 (MMX, SSE)"), wxT_2("-march=pentium3"), category);
    m_Options.AddOption(_("Intel Pentium 4 (MMX, SSE, SSE2)"), wxT_2("-march=pentium4"), category);
    m_Options.AddOption(_("Intel Pentium 4 Prescott (MMX, SSE, SSE2, SSE3)"), wxT_2("-march=prescott"), category);
    m_Options.AddOption(_("Intel Pentium 4 Nocona (MMX, SSE, SSE2, SSE3, 64bit extensions)"), wxT_2("-march=nocona"), category);
    m_Options.AddOption(_("Intel Pentium M (MMX, SSE, SSE2)"), wxT_2("-march=pentium-m"), category);
    m_Options.AddOption(_("AMD K6 (MMX)"), wxT_2("-march=k6"), category);
    m_Options.AddOption(_("AMD K6-2 (MMX, 3DNow!)"), wxT_2("-march=k6-2"), category);
    m_Options.AddOption(_("AMD K6-3 (MMX, 3DNow!)"), wxT_2("-march=k6-3"), category);
    m_Options.AddOption(_("AMD Athlon (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), wxT_2("-march=athlon"), category);
    m_Options.AddOption(_("AMD Athlon Thunderbird (MMX, 3DNow!, enhanced 3DNow!, SSE prefetch)"), wxT_2("-march=athlon-tbird"), category);
    m_Options.AddOption(_("AMD Athlon 4 (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), wxT_2("-march=athlon-4"), category);
    m_Options.AddOption(_("AMD Athlon XP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), wxT_2("-march=athlon-xp"), category);
    m_Options.AddOption(_("AMD Athlon MP (MMX, 3DNow!, enhanced 3DNow!, full SSE)"), wxT_2("-march=athlon-mp"), category);
    m_Options.AddOption(_("AMD K8 core (x86-64 instruction set)"), wxT_2("-march=k8"), category);
    m_Options.AddOption(_("AMD Opteron (x86-64 instruction set)"), wxT_2("-march=opteron"), category);
    m_Options.AddOption(_("AMD Athlon64 (x86-64 instruction set)"), wxT_2("-march=athlon64"), category);
    m_Options.AddOption(_("AMD Athlon-FX (x86-64 instruction set)"), wxT_2("-march=athlon-fx"), category);

    m_Commands[(int)ctCompileObjectCmd].push_back(CompilerTool(wxT_2("$compiler $options $includes -c $file -o $object")));
    m_Commands[(int)ctGenDependenciesCmd].push_back(CompilerTool(wxT_2("$compiler -MM $options -MF $dep_object -MT $object $includes $file")));
    m_Commands[(int)ctCompileResourceCmd].push_back(CompilerTool(wxT_2("$rescomp -i $file -J rc -o $resource_output -O coff $res_includes")));
    m_Commands[(int)ctLinkConsoleExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs")));
    if (platform::windows)
    {
        m_Commands[(int)ctLinkNativeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs --subsystem,native")));
        m_Commands[(int)ctLinkExeCmd].push_back(CompilerTool(wxT_2("$linker $libdirs -o $exe_output $link_objects $link_resobjects $link_options $libs -mwindows")));
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker -shared -Wl,--output-def=$def_output -Wl,--out-implib=$static_output -Wl,--dll $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    else
    {
        m_Commands[(int)ctLinkExeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkNativeCmd] = m_Commands[(int)ctLinkConsoleExeCmd]; // no -mwindows
        m_Commands[(int)ctLinkDynamicCmd].push_back(CompilerTool(wxT_2("$linker -shared $libdirs $link_objects $link_resobjects -o $exe_output $link_options $libs")));
    }
    m_Commands[(int)ctLinkStaticCmd].push_back(CompilerTool(wxT_2("$lib_linker -r -s $static_output $link_objects")));

    LoadDefaultRegExArray();

    m_CompilerOptions.Clear();
    m_LinkerOptions.Clear();
    m_LinkLibs.Clear();
    m_CmdsBefore.Clear();
    m_CmdsAfter.Clear();
    SetVersionString();
}

void CompilerMINGW::LoadDefaultRegExArray()
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

AutoDetectResult CompilerMINGW::AutoDetectInstallationDir()
{
    // try to find MinGW in environment variable PATH first
    wxPathList list;
    list.AddEnvList(wxT_2("PATH"));
    wxString path = list.FindAbsoluteValidPath(m_Programs.C);
    if (!path.IsEmpty())
    {
        wxFileName fname(path);
        fname.RemoveLastDir();
        m_MasterPath = fname.GetPath(wxPATH_GET_VOLUME);
        return adrDetected;
    }

    wxString sep = caFileName::GetPathSeparator();
    if (platform::windows)
    {
        // look first if MinGW was installed with Code::Blocks (new in beta6)
        m_MasterPath = ConfigManager::GetExecutableFolder();
        if (!wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C))
            // if that didn#t do it, look under C::B\MinGW, too (new in 08.02)
            m_MasterPath += sep + wxT_2("MinGW");
        if (!wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C))
        {
            // no... search for MinGW installation dir
            wxString windir = wxGetOSDirectory();
            wxFileConfig ini(wxT_2(""), wxT_2(""), windir + wxT_2("/MinGW.ini"), wxT_2(""), wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
            m_MasterPath = ini.Read(wxT_2("/InstallSettings/InstallPath"), wxT_2("C:\\MinGW"));
            if (!wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C))
            {
#ifdef __WXMSW__ // for wxRegKey
                // not found...
                // look for dev-cpp installation
                wxRegKey key; // defaults to HKCR
                key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\Software\\Dev-C++"));
                if (key.Exists() && key.Open(wxRegKey::Read))
                {
                    // found; read it
                    key.QueryValue(wxT_2("Install_Dir"), m_MasterPath);
                }
                else
                {
                    // installed by inno-setup
                    // HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Minimalist GNU for Windows 4.1_is1
                    wxString name;
                    long index;
                    key.SetName(wxT_2("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));
                    //key.SetName("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
                    bool ok = key.GetFirstKey(name, index);
                    while (ok && !name.StartsWith(wxT_2("Minimalist GNU for Windows")))
                    {
                        ok = key.GetNextKey(name, index);
                    }
                    if (ok)
                    {
                        name = key.GetName() + wxT_2("\\") + name;
                        key.SetName(name);
                        if (key.Exists() && key.Open(wxRegKey::Read))
                            key.QueryValue(wxT_2("InstallLocation"), m_MasterPath);
                    }
                }
#endif
            }
        }
        else
            m_Programs.MAKE = wxT_2("make.exe"); // we distribute "make" not "mingw32-make"
    }
    else
        m_MasterPath = wxT_2("/usr");

    AutoDetectResult ret = wxFileExists(m_MasterPath + sep + wxT_2("bin") + sep + m_Programs.C) ? adrDetected : adrGuessed;
    // don't add lib/include dirs. GCC knows where its files are located

    SetVersionString();
    return ret;
}

void CompilerMINGW::SetVersionString()
{
    wxArrayString output, errors;
    wxString sep = caFileName::GetPathSeparator();
    wxString masterpath = m_MasterPath;

    /* We should read the master path from the configuration manager as
     * the m_MasterPath is empty if AutoDetectInstallationDir() is not
     * called
     */
    ConfigManager* cmgr = Manager::Get()->GetConfigManager(wxT_2("compiler"));
    if (cmgr)
        cmgr->Read(wxT_2("/sets/gcc/master_path"), &masterpath);

    if (masterpath.IsEmpty())
    {
        if (platform::windows)
            masterpath = wxT_2("C:\\MinGW");
        else
            masterpath = wxT_2("/usr");
    }
    wxString gcc_command = masterpath + sep + wxT_2("bin") + sep + m_Programs.C;
    if (!wxFileExists(gcc_command))
        return;
    long result = wxExecute(gcc_command + wxT_2(" --version"), output, errors, wxEXEC_NODISABLE);
    if (result > 0)
    {
        Manager::Get()->GetLogManager()->DebugLog(wxT_2("Error in executing ") + gcc_command);
    }
    else
    {
        if (output.GetCount() > 0)
        {
            wxRegEx reg_exp;
            if (reg_exp.Compile(wxT_2("[0-9][.][0-9][.][0-9]")) && reg_exp.Matches(output[0]))
                m_VersionString = reg_exp.GetMatch(output[0]);
            else
            {
                m_VersionString = output[0].Mid(10);
                m_VersionString = m_VersionString.Left(5);
                m_VersionString.Trim(false);
            }
        }
    }
}
