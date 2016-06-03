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

#include "sdk.h"

#ifndef CB_PRECOMP
    #include <wx/confbase.h>
    #include <wx/fileconf.h>
    #include <wx/msgdlg.h>
    #include <wx/intl.h>
    #include <wx/filename.h>
    #include <wx/txtstrm.h>
    #include <wx/dynarray.h>
    #include <wx/wfstream.h>

    #include "manager.h"
    #include "projectmanager.h"
    #include "logmanager.h"
    #include "cbproject.h"
    #include "globals.h"
    #include "compilerfactory.h"
    #include "compiler.h"
#endif

#include <wx/stream.h>

#include "prep.h"
#include "importers_globals.h"
#include "msvcloader.h"
#include "multiselectdlg.h"

/* NOTE:- Replacing all wxString::Remove(size_t, size_t) with wxString::Mid()
 * and Truncate() functions. This function has been marked as a wx-1.xx
 * compatibility function in wxWidgets-2.8. So in future it will be dropped.
 */

MSVCLoader::MSVCLoader(cbProject* project)
    : m_pProject(project),
    m_ConvertSwitches(true)
{
    //ctor
}

MSVCLoader::~MSVCLoader()
{
    //dtor
}

bool MSVCLoader::Open(const wxString& filename)
{
    /* NOTE (mandrav#1#): not necessary to ask for switches conversion... */
    m_ConvertSwitches = m_pProject->GetCompilerID().IsSameAs(wxT_2("gcc"));

    m_Filename = filename;
    if (!ReadConfigurations())
        return false;

    // the file is read, now process it
    Manager::Get()->GetLogManager()->DebugLog(wxT_2("Importing MSVC project: ") + filename);

    // delete all targets of the project (we 'll create new ones from the imported configurations)
    while (m_pProject->GetBuildTargetsCount())
        m_pProject->RemoveBuildTarget(0);

    wxArrayInt selected_indices;
    if (ImportersGlobals::ImportAllTargets)
    {
        // don't ask; just fill selected_indices with all indices
        for (size_t i = 0; i < m_Configurations.GetCount(); ++i)
            selected_indices.Add(i);
    }
    else
    {
        // ask the user to select a configuration - multiple choice ;)
        MultiSelectDlg dlg(0, m_Configurations, true, _("Select configurations to import:"), m_Filename.GetName());
        PlaceWindow(&dlg);
        if (dlg.ShowModal() == wxID_CANCEL)
        {
            Manager::Get()->GetLogManager()->DebugLog(wxT_2("Canceled..."));
            return false;
        }
        selected_indices = dlg.GetSelectedIndices();
    }

    // create all selected targets
    for (size_t i = 0; i < selected_indices.GetCount(); ++i)
    {
        if (!ParseConfiguration(selected_indices[i]))
            return false;
    }

    m_pProject->SetTitle(m_Filename.GetName());
    return ParseSourceFiles();
}

bool MSVCLoader::Save(const wxString& filename)
{
    // no support to save MSVC projects
    return false;
}

bool MSVCLoader::ReadConfigurations()
{
    m_Configurations.Clear();
    m_ConfigurationsLineIndex.Clear();
    m_BeginTargetLine = -1;

    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    wxArrayString comps;
    wxTextInputStream input(file);

    int currentLine = 0;
    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        ++currentLine;
        line.Trim(true);
        line.Trim(false);
        int size = -1;
        if (line.StartsWith(wxT_2("# TARGTYPE")))
        {
            // # TARGTYPE "Win32 (x86) Application" 0x0103
            int idx = line.Find(' ', true);
            if (idx != -1)
            {
                TargetType type;
                wxString targtype = line.Mid(12, idx-1-12);
                wxString projcode = line.Mid(idx+3, 4);
                if      (projcode.Matches(wxT_2("0101"))) type = ttExecutable;
                else if (projcode.Matches(wxT_2("0102"))) type = ttDynamicLib;
                else if (projcode.Matches(wxT_2("0103"))) type = ttConsoleOnly;
                else if (projcode.Matches(wxT_2("0104"))) type = ttStaticLib;
                else if (projcode.Matches(wxT_2("010a"))) type = ttCommandsOnly;
                else
                {
                    type = ttCommandsOnly;
                    Manager::Get()->GetLogManager()->DebugLog(wxT_2("unrecognized target type"));
                }

                //Manager::Get()->GetLogManager()->DebugLog(wxT_2("TargType '%s' is %d"), targtype.c_str(), type);
                m_TargType[targtype] = type;
            }
            continue;
        }
        else if (line.StartsWith(wxT_2("!MESSAGE \"")))
        {
            //  !MESSAGE "anothertest - Win32 Release" (based on "Win32 (x86) Application")
            int pos;
            pos = line.Find('\"');
            line = line.Mid(pos + 1);
            pos = line.Find('\"');
            wxArrayString projectTarget = GetArrayFromString(line.Left(pos), wxT_2("-"));
            wxString target = projectTarget[1];
            if (projectTarget.GetCount() != 2)
            {
                Manager::Get()->GetLogManager()->DebugLog(wxT_2("ERROR: bad target format"));
                return false;
            }
            line = line.Mid(pos + 1);
            pos = line.Find('\"');
            line = line.Mid(pos + 1);
            pos = line.Find('\"');
            wxString basedon = line.Left(pos);
            TargetType type = ttCommandsOnly;
            HashTargetType::iterator it = m_TargType.find(basedon);
            if (it != m_TargType.end())
                type = it->second;
            else
            {
                Manager::Get()->GetLogManager()->DebugLog(wxT_2("ERROR: target type not found"));
                return false;
            }
            m_TargetBasedOn[target] = type;
            //Manager::Get()->GetLogManager()->DebugLog(wxT_2("Target '%s' type %d"), target.c_str(), type);
        }
        else if (line.StartsWith(wxT_2("!IF  \"$(CFG)\" ==")))
            size = 16;
        else if (line.StartsWith(wxT_2("!ELSEIF  \"$(CFG)\" ==")))
            size = 20;
        else if (line == wxT_2("# Begin Target"))
        {
            // done
            m_BeginTargetLine = currentLine;
            break;
        }
        if (size != -1)
        {
            // read configuration name
            line = line.Mid(size);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            // remove the project name part, i.e "anothertest - "
            int idx = tmp.Find('-');
            if (idx != -1)
            {
                tmp = tmp.Mid(idx + 1);
                tmp.Trim(false);
            }
            if (m_Configurations.Index(tmp) == wxNOT_FOUND)
            {
                m_Configurations.Add(tmp);
                m_ConfigurationsLineIndex.Add(currentLine);
                Manager::Get()->GetLogManager()->DebugLog(F(wxT("Detected configuration '%s' at line %d"), tmp.c_str(), currentLine));
            }
        }
    }
    return true;
}

bool MSVCLoader::ParseConfiguration(int index)
{
    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    // create new target
    ProjectBuildTarget* bt = m_pProject->AddBuildTarget(m_Configurations[index]);
    if (!bt)
        return false;
    bt->SetCompilerID(m_pProject->GetCompilerID());
    m_Type = ttCommandsOnly;
    HashTargetType::iterator it = m_TargetBasedOn.find(m_Configurations[index]);
    if (it != m_TargetBasedOn.end()) m_Type = it->second;
    else Manager::Get()->GetLogManager()->DebugLog(wxT_2("ERROR: could not find the target type of ") + m_Configurations[index]);
    bt->SetTargetType(m_Type);
    bt->SetOutputFilename(bt->SuggestOutputFilename());

    wxTextInputStream input(file);

    // go to the configuration's line
    int currentLine = 0;
    while (!file.Eof() && currentLine <= m_ConfigurationsLineIndex[index])
    {
        input.ReadLine();
        ++currentLine;
    }

    // start parsing the configuration
    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        line.Trim(true);
        line.Trim(false);

        // we want empty lines (skipped) or lines starting with #
        // if we encounter a line starting with !, we break out of here
        if (line.GetChar(0) == '!')
            break;
        if (line.IsEmpty() || line.GetChar(0) != '#')
            continue;

//        if (line.StartsWith("# PROP BASE Output_Dir "))
        if (line.StartsWith(wxT_2("# PROP Output_Dir ")))
        {
            line = line.Mid(18);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            if (!line.IsEmpty())
            {
                wxFileName out = bt->GetOutputFilename();
                out.SetPath(tmp); // out could be a full path name and not only a relative one !
                if (out.IsRelative())
                    out.MakeAbsolute(m_Filename.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
                bt->SetOutputFilename(out.GetFullPath());
            }
        }
//        else if (line.StartsWith("# PROP BASE Intermediate_Dir "))
        else if (line.StartsWith(wxT_2("# PROP Intermediate_Dir ")))
        {
            line = line.Mid(24);
            line.Trim(true);
            line.Trim(false);
            wxString tmp = RemoveQuotes(line);
            if (!line.IsEmpty())
            {
                bt->SetObjectOutput(tmp);
            }
        }
        else if (line.StartsWith(wxT_2("# ADD BASE CPP ")))
        {
            line = line.Mid(15);
            line.Trim(true);
            line.Trim(false);
            ProcessCompilerOptions(bt, line);
        }
        else if (line.StartsWith(wxT_2("# ADD CPP ")))
        {
            line = line.Mid(10);
            line.Trim(true);
            line.Trim(false);
            ProcessCompilerOptions(bt, line);
        }
        else if (line.StartsWith(wxT_2("# ADD BASE LINK32 ")))
        {
            line = line.Mid(18);
            line.Trim(true);
            line.Trim(false);
            ProcessLinkerOptions(bt, line);
        }
        else if (line.StartsWith(wxT_2("# ADD LINK32 ")))
        {
            line = line.Mid(13);
            line.Trim(true);
            line.Trim(false);
            ProcessLinkerOptions(bt, line);
        }
        else if (line.StartsWith(wxT_2("# ADD BASE RSC "))) // To import resource compiler options
        {
            line = line.Mid(16);
            line.Trim(true);
            line.Trim(false);
            ProcessResourceCompilerOptions(bt, line);
        }
        else if (line.StartsWith(wxT_2("# ADD RSC ")))
        {
            line = line.Mid(11);
            line.Trim(true);
            line.Trim(false);
            ProcessResourceCompilerOptions(bt, line);
        }
    }
    return true;
}

bool MSVCLoader::ParseSourceFiles()
{
    wxFileInputStream file(m_Filename.GetFullPath());
    if (!file.Ok())
        return false; // error opening file???

    wxTextInputStream input(file);
    wxString LastProcessedFile = wxEmptyString;
    wxString CurCFG;
    bool FoundIf = false;
    size_t size;

    // go to the begining of source files
    int currentLine = 0;
    while (!file.Eof() && currentLine < m_BeginTargetLine)
    {
        input.ReadLine();
        ++currentLine;
    }

    while (!file.Eof())
    {
        wxString line = input.ReadLine();
        line.Trim(true);
        line.Trim(false);

        if (line.StartsWith(wxT_2("SOURCE=")))
        {
            line = line.Mid(7);
            line.Trim(true);
            line.Trim(false);

            wxString fname (RemoveQuotes(line));

            if ((!fname.IsEmpty()) && (fname != wxT_2(".\\")))
            {
                if (fname.StartsWith(wxT_2(".\\")))
                    fname.erase(0, 2);

                if (!platform::windows)
                    fname.Replace(wxT_2("\\"), wxT_2("/"), true);

                ProjectFile* pf = m_pProject->AddFile(0, fname);
                if (pf)
                {
                    LastProcessedFile = fname;
                    // add it to all configurations, not just the first
                    for (int i = 1; i < m_pProject->GetBuildTargetsCount(); ++i)
                        pf->AddBuildTarget(m_pProject->GetBuildTarget(i)->GetTitle());
                }
            }
        }
        else if (line.StartsWith(wxT_2("!")))
        {
            FoundIf = true;
            if (line.StartsWith(wxT_2("!IF  \"$(CFG)\" ==")))
                size = 16;
            else if (line.StartsWith(wxT_2("!ELSEIF  \"$(CFG)\" ==")))
                size = 20;
            else
            {
                size = 0;
                FoundIf = false;
            }
            if (size > 0)
            {
                CurCFG = line.Mid(size);
                CurCFG = RemoveQuotes(CurCFG.Trim(false).Trim(true));
                CurCFG = CurCFG.Mid(CurCFG.Find(wxT_2("-")) + 1).Trim(true).Trim(false);
            }
            if (line.StartsWith(wxT_2("!ENDIF")))
            {
                FoundIf = false;
                CurCFG = wxEmptyString;
                LastProcessedFile = wxEmptyString;
            }
        }
        else if (line.StartsWith(wxT_2("#")))
        {
            if (FoundIf && line.StartsWith(wxT_2("# PROP Exclude_From_Build ")))
            {
                line.Trim(true);
                if (line.Right(1).IsSameAs(wxT_2("1")))
                {
                    ProjectFile* pf = m_pProject->GetFileByFilename(LastProcessedFile);
                    if (pf)
                    {
                        for (int j = 0; j < m_pProject->GetBuildTargetsCount(); ++j)
                        {
                            if (m_pProject->GetBuildTarget(j)->GetTitle().IsSameAs(CurCFG))
                            {
                                pf->RemoveBuildTarget(CurCFG);
                                Manager::Get()->GetLogManager()->DebugLog(wxString::Format(wxT_2("Buid target %s has been excluded from %s"),
																		CurCFG.c_str(), LastProcessedFile.c_str()));
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

void MSVCLoader::ProcessCompilerOptions(ProjectBuildTarget* target, const wxString& opts)
{
    wxArrayString array;
    array = OptStringTokeniser(opts);

    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        wxString opt = array[i];
        opt.Trim();

        if (m_ConvertSwitches)
        {
            if (opt.Matches(wxT_2("/D")))
                target->AddCompilerOption(wxT_2("-D") + RemoveQuotes(array[++i]));
            else if (opt.Matches(wxT_2("/U")))
                target->AddCompilerOption(wxT_2("-U") + RemoveQuotes(array[++i]));
            else if (opt.Matches(wxT_2("/Zi")) || opt.Matches(wxT_2("/ZI")))
                target->AddCompilerOption(wxT_2("-g"));
            else if (opt.Matches(wxT_2("/I")))
                target->AddIncludeDir(RemoveQuotes(array[++i]));
            else if (opt.Matches(wxT_2("/W0")))
                target->AddCompilerOption(wxT_2("-w"));
            else if (opt.Matches(wxT_2("/O1")) ||
                     opt.Matches(wxT_2("/O2")) ||
                     opt.Matches(wxT_2("/O3")))
                target->AddCompilerOption(wxT_2("-O2"));
            else if (opt.Matches(wxT_2("/W1")) ||
                     opt.Matches(wxT_2("/W2")) ||
                     opt.Matches(wxT_2("/W3")))
                target->AddCompilerOption(wxT_2("-W"));
            else if (opt.Matches(wxT_2("/W4")))
                target->AddCompilerOption(wxT_2("-Wall"));
            else if (opt.Matches(wxT_2("/WX")))
                target->AddCompilerOption(wxT_2("-Werror"));
            else if (opt.Matches(wxT_2("/GX")))
                target->AddCompilerOption(wxT_2("-fexceptions"));
            else if (opt.Matches(wxT_2("/Ob0")))
                target->AddCompilerOption(wxT_2("-fno-inline"));
            else if (opt.Matches(wxT_2("/Ob2")))
                target->AddCompilerOption(wxT_2("-finline-functions"));
            else if (opt.Matches(wxT_2("/Oy")))
                target->AddCompilerOption(wxT_2("-fomit-frame-pointer"));
            else if (opt.Matches(wxT_2("/GB")))
                target->AddCompilerOption(wxT_2("-mcpu=pentiumpro -D_M_IX86=500"));
            else if (opt.Matches(wxT_2("/G6")))
                target->AddCompilerOption(wxT_2("-mcpu=pentiumpro -D_M_IX86=600"));
            else if (opt.Matches(wxT_2("/G5")))
                target->AddCompilerOption(wxT_2("-mcpu=pentium -D_M_IX86=500"));
            else if (opt.Matches(wxT_2("/G4")))
                target->AddCompilerOption(wxT_2("-mcpu=i486 -D_M_IX86=400"));
            else if (opt.Matches(wxT_2("/G3")))
                target->AddCompilerOption(wxT_2("-mcpu=i386 -D_M_IX86=300"));
            else if (opt.Matches(wxT_2("/Za")))
                target->AddCompilerOption(wxT_2("-ansi"));
            else if (opt.Matches(wxT_2("/Zp1")))
                target->AddCompilerOption(wxT_2("-fpack-struct"));
            else if (opt.Matches(wxT_2("/nologo")))
            {
                // do nothing (ignore silently)
            }
            else if (opt.Matches(wxT_2("/c")))
            {
                // do nothing (ignore silently)
            }
            else if (opt.StartsWith(wxT_2("@")))
            {
                wxArrayString options;
                if (ParseResponseFile(m_pProject->GetBasePath() + opt.Mid(1), options))
                {
                    for (size_t i = 0; i < options.GetCount(); ++i)
                        ProcessCompilerOptions(target, options[i]);
                }
                else
                { // Fallback: Remember GCC will process Pre-processor macros only
                    Manager::Get()->GetLogManager()->DebugLog(wxT_2("Can't open ") + m_pProject->GetBasePath() + opt.Mid(1) + wxT_2(" for parsing"));
                    target->AddCompilerOption(wxT_2("-imacros ") + opt.Mid(1));
                }
            }
            //else Manager::Get()->GetLogManager()->DebugLog("Unhandled compiler option: " + opt);
        }
        else // !m_ConvertSwitches
        {
            // only differentiate includes and definitions
            if (opt.Matches(wxT_2("/I")))
                target->AddIncludeDir(RemoveQuotes(array[++i]));
            else if (opt.Matches(wxT_2("/D")))
                target->AddCompilerOption(wxT_2("/D") + RemoveQuotes(array[++i]));
            else if (opt.Matches(wxT_2("/U")))
                target->AddCompilerOption(wxT_2("/U") + RemoveQuotes(array[++i]));
            else if (opt.StartsWith(wxT_2("/Yu")))
                Manager::Get()->GetLogManager()->DebugLog(wxT_2("Ignoring precompiled headers option (/Yu)"));
            else if (opt.Matches(wxT_2("/c")) || opt.Matches(wxT_2("/nologo")))
            {
                // do nothing (ignore silently)
            }
            else
                target->AddCompilerOption(opt);
        }
    }
}

void MSVCLoader::ProcessLinkerOptions(ProjectBuildTarget* target, const wxString& opts)
{
    wxArrayString array;
    array = OptStringTokeniser(opts);

    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        wxString opt = array[i];
        opt.Trim();

        if (m_ConvertSwitches)
        {
            if (opt.StartsWith(wxT_2("/libpath:")))
            {
                opt = opt.Mid(9);
                target->AddLibDir(RemoveQuotes(opt));
            }
            else if (opt.StartsWith(wxT_2("/base:")))
            {
                opt = opt.Mid(6);
                target->AddLinkerOption(wxT_2("--image-base ") + RemoveQuotes(opt));
            }
            else if (opt.StartsWith(wxT_2("/implib:")))
            {
                opt = opt.Mid(8);
                target->AddLinkerOption(wxT_2("--implib ") + RemoveQuotes(opt));
            }
            else if (opt.StartsWith(wxT_2("/map:")))
            {
                opt = opt.Mid(5);
                target->AddLinkerOption(wxT_2("-Map ") + RemoveQuotes(opt) + wxT_2(".map"));
            }
            else if (opt.Matches(wxT_2("/nologo")))
            {
                // do nothing (ignore silently)
            }
            else if (opt.StartsWith(wxT_2("/out:")))
            {
                // do nothing; it is handled below, in common options
            }
            else if (opt.StartsWith(wxT_2("@")))
            {
                wxArrayString options;
                if (ParseResponseFile(m_pProject->GetBasePath() + opt.Mid(1), options))
                {
                    for (size_t i = 0; i < options.GetCount(); ++i)
                        ProcessLinkerOptions(target, options[i]);
                } // else ignore
            }
            else if (opt.Find(wxT_2(".lib")) == -1) // don't add linking lib (added below, in common options)
                Manager::Get()->GetLogManager()->DebugLog(wxT_2("Unknown linker option: " + opt));
        }
        else // !m_ConvertSwitches
        {
            if (opt.StartsWith(wxT_2("/libpath:")))
            {
                opt = opt.Mid(9);
                target->AddLibDir(RemoveQuotes(opt));
            }
            else if (opt.Matches(wxT_2("/nologo"))) {} // ignore silently
            else if (opt.StartsWith(wxT_2("@")))
                target->AddLinkerOption(opt);
            else
            {
                // don't add linking lib (added below, in common options)
                int idx = opt.Find(wxT_2(".lib"));
                if (idx == -1)
                    target->AddLinkerOption(opt);
            }
        }

        // common options
        if (!opt.StartsWith(wxT_2("/")))
        {
            // probably linking lib
            int idx = opt.Find(wxT_2(".lib"));
            if (idx != -1)
            {
                opt.Truncate(idx);
                target->AddLinkLib(opt);
            }
        }
        else if (opt.StartsWith(wxT_2("/out:")))
        {
            opt = opt.Mid(5);
            opt = RemoveQuotes(opt);
            if (m_Type == ttStaticLib)
            {
                // convert lib filename based on compiler
                /* NOTE (mandrav#1#): I think I should move this code somewhere more accessible...
                I need it here and there... */
                wxFileName orig = target->GetOutputFilename();
                wxFileName newf = opt;
                if (newf.IsRelative())
                    newf.MakeAbsolute(m_pProject->GetBasePath());
                Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());
                newf.SetExt(compiler->GetSwitches().libExtension);
                wxString name = newf.GetName();
                wxString prefix = compiler->GetSwitches().libPrefix;
                if (!prefix.IsEmpty() && !name.StartsWith(prefix))
                    newf.SetName(prefix + name);
                target->SetOutputFilename(newf.GetFullPath());
            }
            else
                target->SetOutputFilename(opt);
        }
    }
}

void MSVCLoader::ProcessResourceCompilerOptions(ProjectBuildTarget* target, const wxString& opts)
{
    wxArrayString array;
    array = OptStringTokeniser(opts);

    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        wxString opt = array[i];
        opt.Trim();

        if (opt.StartsWith(wxT_2("/")))
        {
            if (opt.StartsWith(wxT_2("/i"))) // Only include dir is imported
                target->AddResourceIncludeDir(RemoveQuotes(array[++i]));
        }
    }
}

wxArrayString MSVCLoader::OptStringTokeniser(const wxString& opts)
{
    // tokenise string like:
    // wsock32.lib /nologo /machine:I386 /libpath:"lib" /libpath:"C:\My Folder"

    wxArrayString out;

    wxString search = opts;
    search.Trim(true).Trim(false);

    // trivial case: string is empty or consists of blanks only
    if (search.IsEmpty())
        return out;

    wxString token;
    bool     inside_quot = false;
    size_t   pos         = 0;
    while (pos < search.Length())
    {
        wxString current_char = search.GetChar(pos);

        // for e.g. /libpath:"C:\My Folder"
        if (current_char.CompareTo(wxT("\""))==0) // equality
            inside_quot = !inside_quot;

        if ((current_char.CompareTo(wxT(" "))==0) && (!inside_quot))
        {
            if (!token.IsEmpty())
            {
                out.Add(token);
                token.Clear();
            }
        }
        else
        {
            token.Append(current_char);
        }

        pos++;
        // Append final token
        if ((pos==search.Length()) && (!inside_quot) && (!token.IsEmpty()))
            out.Add(token);
    }

    return out;
}

wxString MSVCLoader::RemoveQuotes(const wxString& src)
{
    wxString res = src;
    if (res.StartsWith(wxT_2("\"")))
    {
        res = res.Mid(1);
        res.Truncate(res.Length() - 1);
    }
//    Manager::Get()->GetLogManager()->DebugLog(wxT_2("Removing quotes: %s --> %s"), src.c_str(), res.c_str());
    return res;
}

bool MSVCLoader::ParseResponseFile(const wxString filename, wxArrayString& output)
{
    /* Note: MSDN says user cannot call another response file
     * from a response file. Thus it's quite safe to parse the file. */
    bool success = false;
    wxFileInputStream inp_file(filename);
    if (inp_file.Ok())
    {
        wxTextInputStream inp_txt(inp_file);
        success = true;
        while (!inp_file.Eof())
            output.Add(inp_txt.ReadLine());
    }
    else
        success = false;
    return success;
}
