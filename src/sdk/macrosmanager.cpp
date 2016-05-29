/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
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
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/menu.h>

    #include "projectmanager.h"
    #include "editormanager.h"
    #include "logmanager.h"
    #include "macrosmanager.h"
    #include "scriptingmanager.h"
    #include "manager.h"
    #include "cbproject.h"
    #include "cbeditor.h"
    #include "uservarmanager.h"
    #include "configmanager.h"
    #include "globals.h"
    #include "compilerfactory.h"
    #include "compiler.h"
#endif

#include <cstdlib>

#include "scripting/sqplus/sqplus.h"
#include "scripting/bindings/scriptbindings.h"
#include "prep.h"

using namespace std;

template<> MacrosManager* Mgr<MacrosManager>::instance = 0;
template<> bool  Mgr<MacrosManager>::isShutdown = false;

static const wxString const_COIN(wxT_2("COIN"));
static const wxString const_RANDOM(wxT_2("RANDOM"));

MacrosManager::MacrosManager()
{
    Reset();
}

MacrosManager::~MacrosManager()
{
}

void MacrosManager::CreateMenu(wxMenuBar* menuBar)
{
}

void MacrosManager::ReleaseMenu(wxMenuBar* menuBar)
{
}

wxString MacrosManager::ReplaceMacros(const wxString& buffer, ProjectBuildTarget* target)
{
    wxString tmp = buffer;
    ReplaceMacros(tmp, target);
    return tmp;
}

void MacrosManager::Reset()
{
    m_lastProject = 0;
    m_lastTarget = 0;
    m_lastEditor = 0;

    m_AppPath = UnixFilename(ConfigManager::GetExecutableFolder());
    m_Plugins = UnixFilename(ConfigManager::GetPluginsFolder());
    m_DataPath = UnixFilename(ConfigManager::GetDataFolder());
    ClearProjectKeys();
    m_re_unx.Compile(wxT_2("([^$]|^)(\\$[({]?(#?[A-Za-z_0-9.]+)[)} /\\]?)"), wxRE_EXTENDED | wxRE_NEWLINE);
    m_re_dos.Compile(wxT_2("([^%]|^)(%(#?[A-Za-z_0-9.]+)%)"), wxRE_EXTENDED | wxRE_NEWLINE);
    m_re_if.Compile(wxT_2("\\$if\\((.*)\\)[ ]*\\{([^}]*)\\}{1}([ ]*else[ ]*\\{([^}]*)\\})?"), wxRE_EXTENDED | wxRE_NEWLINE);
    m_re_ifsp.Compile(wxT_2("[^=!<>]+|(([^=!<>]+)[ ]*(=|==|!=|>|<|>=|<=)[ ]*([^=!<>]+))"), wxRE_EXTENDED | wxRE_NEWLINE);
    m_re_script.Compile(wxT_2("(\\[\\[(.*)\\]\\])"), wxRE_EXTENDED | wxRE_NEWLINE);
    m_uVarMan = Manager::Get()->GetUserVariableManager();
    srand(time(0));
    assert(m_re_unx.IsValid());
    assert(m_re_dos.IsValid());
}

void MacrosManager::ClearProjectKeys()
{
//    Manager::Get()->GetLogManager()->DebugLog(wxT_2("clear"));
    macros.clear();

    macros[wxT_2("AMP")]   = wxT_2("&");
    macros[wxT_2("CODEBLOCKS")] = m_AppPath;
    macros[wxT_2("APP_PATH")]  = m_AppPath;
    macros[wxT_2("APP-PATH")]  = m_AppPath;
    macros[wxT_2("APPPATH")]  = m_AppPath;
    macros[wxT_2("DATA_PATH")]  = m_DataPath;
    macros[wxT_2("DATA-PATH")]  = m_DataPath;
    macros[wxT_2("DATAPATH")]  = m_DataPath;
    macros[wxT_2("PLUGINS")]  = m_Plugins;
    macros[wxT_2("LANGUAGE")]  = wxLocale::GetLanguageName(wxLocale::GetSystemLanguage());
    macros[wxT_2("ENCODING")]  = wxLocale::GetSystemEncodingName();

    if (platform::windows)
    {
        const wxString cmd(wxT_2("cmd /c "));
        macros[wxT_2("CMD_CP")]  = cmd + wxT_2("copy");
        macros[wxT_2("CMD_RM")]  = cmd + wxT_2("del");
        macros[wxT_2("CMD_MV")]  = cmd + wxT_2("move");
        macros[wxT_2("CMD_NULL")]  = cmd + wxT_2("NUL");
        macros[wxT_2("CMD_MKDIR")] = cmd + wxT_2("md");
        macros[wxT_2("CMD_RMDIR")] = cmd + wxT_2("rd");
    }
    else
    {
        macros[wxT_2("CMD_CP")]  = wxT_2("cp --preserve=timestamps");
        macros[wxT_2("CMD_RM")]  = wxT_2("rm");
        macros[wxT_2("CMD_MV")]  = wxT_2("mv");
        macros[wxT_2("CMD_NULL")]  = wxT_2("/dev/null");
        macros[wxT_2("CMD_MKDIR")]  = wxT_2("mkdir -p");
        macros[wxT_2("CMD_RMDIR")]  = wxT_2("rmdir");
    }
}

void MacrosManager::RecalcVars(cbProject* project,EditorBase* editor,ProjectBuildTarget* target)
{
    if(!editor)
    {
        m_ActiveEditorFilename = wxEmptyString;
        m_lastEditor = 0;
    }
    else if(editor != m_lastEditor)
    {
        m_ActiveEditorFilename = UnixFilename(editor->GetFilename());
        m_lastEditor = editor;
    }
    if(!project)
    {
//        Manager::Get()->GetLogManager()->DebugLog("project == 0");
        m_ProjectFilename = wxEmptyString;
        m_ProjectName = wxEmptyString;
        m_ProjectDir = wxEmptyString;
        m_ProjectFiles = wxEmptyString;
        m_Makefile = wxEmptyString;
        m_lastProject = 0;
        ClearProjectKeys();
        macros[wxT_2("PROJECT_FILE")]  = wxEmptyString;
        macros[wxT_2("PROJECT_FILENAME")] = wxEmptyString;
        macros[wxT_2("PROJECT_FILE_NAME")] = wxEmptyString;
        macros[wxT_2("PROJECTFILE")]  = wxEmptyString;
        macros[wxT_2("PROJECTFILENAME")] = wxEmptyString;
        macros[wxT_2("PROJECT_NAME")]  = wxEmptyString;
        macros[wxT_2("PROJECTNAME")]  = wxEmptyString;
        macros[wxT_2("PROJECT_DIR")]  = wxEmptyString;
        macros[wxT_2("PROJECT_DIRECTORY")] = wxEmptyString;
        macros[wxT_2("PROJECTDIR")]  = wxEmptyString;
        macros[wxT_2("PROJECTDIRECTORY")] = wxEmptyString;
        macros[wxT_2("PROJECT_TOPDIR")]  = wxEmptyString;
        macros[wxT_2("PROJECT_TOPDIRECTORY")] = wxEmptyString;
        macros[wxT_2("PROJECTTOPDIR")]  = wxEmptyString;
        macros[wxT_2("PROJECTTOPDIRECTORY")] = wxEmptyString;
        macros[wxT_2("MAKEFILE")]   = wxEmptyString;
        macros[wxT_2("ALL_PROJECT_FILES")] = wxEmptyString;
    }
    else if(project != m_lastProject)
    {
//        Manager::Get()->GetLogManager()->DebugLog("project != m_lastProject");
        m_lastTarget = 0; // reset last target when project changes
        m_prjname.Assign(project->GetFilename());
        m_ProjectFilename = UnixFilename(m_prjname.GetFullName());
        m_ProjectName = project->GetTitle();
        m_ProjectDir = UnixFilename(project->GetBasePath());
        m_ProjectTopDir = UnixFilename(project->GetCommonTopLevelPath());
        m_Makefile = UnixFilename(project->GetMakefile());
        m_ProjectFiles = wxEmptyString;
        for (int i = 0; i < project->GetFilesCount(); ++i)
        {
            // quote filenames, if they contain spaces
            wxString out = UnixFilename(project->GetFile(i)->relativeFilename);
            QuoteStringIfNeeded(out);
            m_ProjectFiles << out << wxT_2(' ');
        }

        ClearProjectKeys();
        macros[wxT_2("PROJECT_FILE")]  = m_ProjectFilename;
        macros[wxT_2("PROJECT_FILENAME")] = m_ProjectFilename;
        macros[wxT_2("PROJECT_FILE_NAME")] = m_ProjectFilename;
        macros[wxT_2("PROJECTFILE")]  = m_ProjectFilename;
        macros[wxT_2("PROJECTFILENAME")] = m_ProjectFilename;
        macros[wxT_2("PROJECTNAME")]  = m_ProjectName;
        macros[wxT_2("PROJECT_NAME")]  = m_ProjectName;
        macros[wxT_2("PROJECT_DIR")]  = m_ProjectDir;
        macros[wxT_2("PROJECT_DIRECTORY")] = m_ProjectDir;
        macros[wxT_2("PROJECTDIR")]  = m_ProjectDir;
        macros[wxT_2("PROJECTDIRECTORY")] = m_ProjectDir;
        macros[wxT_2("PROJECT_TOPDIR")]  = m_ProjectTopDir;
        macros[wxT_2("PROJECT_TOPDIRECTORY")] = m_ProjectTopDir;
        macros[wxT_2("PROJECTTOPDIR")]  = m_ProjectTopDir;
        macros[wxT_2("PROJECTTOPDIRECTORY")] = m_ProjectTopDir;
        macros[wxT_2("MAKEFILE")]   = m_Makefile;
        macros[wxT_2("ALL_PROJECT_FILES")] = m_ProjectFiles;

        for (int i = 0; i < project->GetBuildTargetsCount(); ++i)
        {
            ProjectBuildTarget* target = project->GetBuildTarget(i);
            if (!target)
                continue;
            wxString title = target->GetTitle().Upper();
            while (title.Replace(wxT_2(" "), wxT_2("_")))
                ; // replace spaces with underscores (what about other invalid chars?)
            macros[title + wxT_2("_OUTPUT_FILE")] = UnixFilename(target->GetOutputFilename());
            macros[title + wxT_2("_OUTPUT_DIR")] = UnixFilename(target->GetBasePath());
            macros[title + wxT_2("_OUTPUT_BASENAME")] = wxFileName(target->GetOutputFilename()).GetName();
        }
        m_lastProject = project;
    }

    if(target)
    {
        const Compiler* compiler = CompilerFactory::GetCompiler(target->GetCompilerID());
        if(compiler)
        {
            const StringHash& v = compiler->GetAllVars();
            for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
            {
                macros[it->first.Upper()] = it->second;
            }
        }
    }

    if(project)
    {
        const StringHash& v = project->GetAllVars();
        for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            macros[it->first.Upper()] = it->second;
        }
    }

    if(!target)
    {
        m_TargetOutputDir = wxEmptyString;
        m_TargetName = wxEmptyString;
        m_TargetOutputBaseName = wxEmptyString;
        m_TargetFilename = wxEmptyString;
        m_lastTarget = 0;
    }
    else if(target != m_lastTarget)
    {
        wxFileName tod(target->GetOutputFilename());
        m_TargetOutputDir = UnixFilename(tod.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR));
        m_TargetName = UnixFilename(target->GetTitle());
        m_TargetOutputBaseName = wxFileName(target->GetOutputFilename()).GetName();
        m_TargetFilename = UnixFilename(target->GetOutputFilename());
        m_lastTarget = target;
    }

    if(target)
    {
        const StringHash& v = target->GetAllVars();
        for (StringHash::const_iterator it = v.begin(); it != v.end(); ++it)
        {
            macros[it->first.Upper()] = it->second;
        }

        if(Compiler* c = CompilerFactory::GetCompiler(target->GetCompilerID()))
        {
            macros[wxT_2("TARGET_CC")]   = c->GetPrograms().C;
            macros[wxT_2("TARGET_CPP")]   = c->GetPrograms().CPP;
            macros[wxT_2("TARGET_LD")]   = c->GetPrograms().LD;
            macros[wxT_2("TARGET_LIB")]   = c->GetPrograms().LIB;
            wxFileName aFilePath;
            aFilePath.SetPath(c->GetMasterPath(), wxPATH_NATIVE);
            wxString aPathStr = aFilePath.GetPathWithSep(wxPATH_NATIVE);
            aPathStr.Replace(wxT_2("\\"), wxT_2("/"));
            macros[wxT_2("TARGET_COMPILER_DIR")] = aPathStr;
        }
        macros[wxT_2("TARGET_OBJECT_DIR")]   = target->GetObjectOutput();
    }

    macros[wxT_2("TARGET_OUTPUT_DIR")]   = m_TargetOutputDir;
    macros[wxT_2("TARGET_NAME")]    = m_TargetName;
    macros[wxT_2("TARGET_OUTPUT_BASENAME")]    = m_TargetOutputBaseName;
    macros[wxT_2("TARGET_OUTPUT_FILE")]    = m_TargetFilename;
    macros[wxT_2("ACTIVE_EDITOR_FILENAME")] = m_ActiveEditorFilename;
    wxFileName fn(m_ActiveEditorFilename);
    macros[wxT_2("ACTIVE_EDITOR_DIRNAME")]  = fn.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    macros[wxT_2("ACTIVE_EDITOR_STEM")]  = fn.GetName();
    macros[wxT_2("ACTIVE_EDITOR_EXT")]  = fn.GetExt();
    wxDateTime now(wxDateTime::Now());
    wxDateTime nowGMT(now.ToGMT());

    macros[wxT_2("TDAY")]   = now.Format(wxT_2("%Y%m%d"));
    macros[wxT_2("TODAY")]   = now.Format(wxT_2("%Y-%m-%d"));
    macros[wxT_2("NOW")]   = now.Format(wxT_2("%Y-%m-%d-%H.%M"));
    macros[wxT_2("NOW_L")]   = now.Format(wxT_2("%Y-%m-%d-%H.%M.%S"));
    macros[wxT_2("WEEKDAY")]  = now.Format(wxT_2("%A"));
    macros[wxT_2("TDAY_UTC")]  = nowGMT.Format(wxT_2("%Y%m%d"));
    macros[wxT_2("TODAY_UTC")]  = nowGMT.Format(wxT_2("%Y-%m-%d"));
    macros[wxT_2("NOW_UTC")]  = nowGMT.Format(wxT_2("%Y-%m-%d-%H.%M"));
    macros[wxT_2("NOW_L_UTC")]  = nowGMT.Format(wxT_2("%Y-%m-%d-%H.%M.%S"));
    macros[wxT_2("WEEKDAY_UTC")] = nowGMT.Format(wxT_2("%A"));
}


void MacrosManager::ReplaceMacros(wxString& buffer, ProjectBuildTarget* target, bool subrequest)
{
    if (buffer.IsEmpty())
        return;

    static const wxString delim(wxT_2("$%["));
    if( buffer.find_first_of(delim) == wxString::npos )
        return;

    cbProject* project = target
                        ? target->GetParentProject()
                        : Manager::Get()->GetProjectManager()->GetActiveProject();
    EditorBase* editor = Manager::Get()->GetEditorManager()->GetActiveEditor();

    if(!target)
    {
        if (project)
        {
            // use the currently compiling target
            target = project->GetCurrentlyCompilingTarget();
            // if none,
            if (!target)
                // use the last known active target
                target = project->GetBuildTarget(project->GetActiveBuildTarget());
        }
    }

    if(project != m_lastProject || target != m_lastTarget || editor != m_lastEditor)
        RecalcVars(project, editor, target);

    wxString search;
    wxString replace;

    if(buffer.find(wxT_2("$if")) != wxString::npos)
    while(m_re_if.Matches(buffer))
    {
        search = m_re_if.GetMatch(buffer, 0);
        replace = EvalCondition(m_re_if.GetMatch(buffer, 1), m_re_if.GetMatch(buffer, 2), m_re_if.GetMatch(buffer, 4), target);
        buffer.Replace(search, replace, false);
    }

    while(m_re_script.Matches(buffer))
    {
        search = m_re_script.GetMatch(buffer, 1);
        replace = Manager::Get()->GetScriptingManager()->LoadBufferRedirectOutput(m_re_script.GetMatch(buffer, 2));
        buffer.Replace(search, replace, false);
    }

    while(m_re_unx.Matches(buffer))
    {
        replace.Empty();

        wxString search = m_re_unx.GetMatch(buffer, 2);
        wxString var = m_re_unx.GetMatch(buffer, 3).Upper();

        if (var.GetChar(0) == wxT_2('#'))
        {
            replace = UnixFilename(m_uVarMan->Replace(var));
        }
        else
        {
            if(var.compare(const_COIN) == 0)
                replace.assign(1u, rand() & 1 ? wxT_2('1') : wxT_2('0'));
            else if(var.compare(const_RANDOM) == 0)
                replace = wxString::Format(wxT_2("%d"), rand() & 0xffff);
            else
            {
                MacrosMap::iterator it;
                if((it = macros.find(var)) != macros.end())
                    replace = it->second;
            }
        }

        const wxChar l = search.Last(); // make non-braced variables work
        if(l == wxT_2('/') || l == wxT_2('\\') || l == wxT_2('$') || l == wxT_2(' '))
            replace.append(l);

        if (replace.IsEmpty())
            wxGetEnv(var, &replace);

        buffer.Replace(search, replace, false);
    }

    while(m_re_dos.Matches(buffer))
    {
        replace.Empty();

        wxString search = m_re_dos.GetMatch(buffer, 2);
        wxString var = m_re_dos.GetMatch(buffer, 3).Upper();

        if (var.GetChar(0) == wxT_2('#'))
        {
            replace = UnixFilename(m_uVarMan->Replace(var));
        }
        else
        {
            if(var.compare(const_COIN) == 0)
                replace.assign(1u, rand() & 1 ? wxT_2('1') : wxT_2('0'));
            else if(var.compare(const_RANDOM) == 0)
                replace = wxString::Format(wxT_2("%d"), rand() & 0xffff);
            else
            {
                MacrosMap::iterator it;
                if((it = macros.find(var)) != macros.end())
                    replace = it->second;
            }
        }

        if (replace.IsEmpty())
            wxGetEnv(var, &replace);

        buffer.Replace(search, replace, false);
    }


    if(!subrequest)
    {
        buffer.Replace(wxT_2("%%"), wxT_2("%"));
        buffer.Replace(wxT_2("$$"), wxT_2("$"));
    }
}

wxString MacrosManager::EvalCondition(const wxString& in_cond, const wxString& true_clause, const wxString& false_clause, ProjectBuildTarget* target)
{
    enum condition_codes {EQ = 1, LT = 2, GT = 4, NE = 8};

    wxString cond(in_cond);
    wxString result;

    ReplaceMacros(cond, target, true);

    if(!m_re_ifsp.Matches(in_cond))
        return false_clause;


    wxString cmpToken(m_re_ifsp.GetMatch(in_cond, 3).Strip(wxString::both));
    wxString left(m_re_ifsp.GetMatch(in_cond, 2).Strip(wxString::both));
    wxString right(m_re_ifsp.GetMatch(in_cond, 4).Strip(wxString::both));


    int compare = left.Cmp(right);
    if(compare == 0)
        compare = EQ;
    else if(compare < 0)
        compare = LT | NE;
    else if(compare > 0)
        compare = GT | NE;


    if(cmpToken.IsEmpty())
        {
        wxString s(m_re_ifsp.GetMatch(in_cond, 0));
        if(s.IsEmpty() || s.IsSameAs(wxT_2("0")) || s.IsSameAs(wxT_2("false")))
            return false_clause;
        return true_clause;
        }

    int condCode = 0;

    if(cmpToken.IsSameAs(wxT_2("==")) || cmpToken.IsSameAs(wxT_2("=")))
        condCode = EQ;
    if(cmpToken.IsSameAs(wxT_2("<")))
        condCode = LT;
    if(cmpToken.IsSameAs(wxT_2(">")))
        condCode = GT;
    if(cmpToken.IsSameAs(wxT_2("<=")))
        condCode = EQ | LT;
    if(cmpToken.IsSameAs(wxT_2(">=")))
        condCode = EQ | GT;
    if(cmpToken.IsSameAs(wxT_2("!=")))
        condCode = NE;

    return condCode & compare ? true_clause : false_clause;
}
