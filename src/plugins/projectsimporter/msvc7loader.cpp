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
    #include <wx/intl.h>
    #include <wx/msgdlg.h>

    #include "manager.h"
    #include "logmanager.h"
    #include "cbproject.h"
    #include "globals.h"
    #include "compilerfactory.h"
    #include "compiler.h"
#endif

#include <wx/choicdlg.h>

#include "prep.h"
#include "msvc7loader.h"
#include "multiselectdlg.h"
#include "importers_globals.h"


MSVC7Loader::MSVC7Loader(cbProject* project)
    : m_pProject(project),
    m_ConvertSwitches(false),
    m_Version(0)
{
    //ctor
    if (platform::windows)
        m_PlatformName = wxT_2("Win32");
    else if (platform::linux)
        m_PlatformName = wxT_2("Linux");
    else if (platform::macosx)
        m_PlatformName = wxT_2("MacOSX");
    else
        m_PlatformName = wxT_2("Unknown");
}

MSVC7Loader::~MSVC7Loader()
{
    //dtor
}

wxString MSVC7Loader::ReplaceMSVCMacros(const wxString& str)
{
    wxString ret = str;
    ret.Replace(wxT_2("$(OutDir)"), m_OutDir);
    ret.Replace(wxT_2("$(IntDir)"), m_IntDir);
    ret.Replace(wxT_2("$(INTDIR)"), m_IntDir);
    ret.Replace(wxT_2("$(ConfigurationName)"), m_ConfigurationName);
    ret.Replace(wxT_2("$(PlatformName)"), m_PlatformName);
    ret.Replace(wxT_2("$(ProjectName)"), m_ProjectName);
    ret.Replace(wxT_2("$(ProjectDir)"), m_pProject->GetBasePath());
    ret.Replace(wxT_2("$(TargetPath)"), m_TargetPath);
    ret.Replace(wxT_2("$(TargetFileName)"), m_TargetFilename);
    ret.Replace(wxT_2("\""), wxT_2(""));
    //ret.Replace(wxT_2("&quot;"), wxT_2("\""));

    // env. vars substitution removed because C::B recognizes them
    // during use ;)

    return ret;
}

bool MSVC7Loader::Open(const wxString& filename)
{
    LogManager* pMsg = Manager::Get()->GetLogManager();
    if (!pMsg)
        return false;

    /* NOTE (mandrav#1#): not necessary to ask for switches conversion... */
    m_ConvertSwitches = m_pProject->GetCompilerID().IsSameAs(wxT_2("gcc"));
    m_ProjectName = wxFileName(filename).GetName();

    pMsg->DebugLog(F(wxT("Importing MSVC 7.xx project: %s"), filename.c_str()));

    TiXmlDocument doc(filename.mb_str());
    if (!doc.LoadFile())
        return false;

    pMsg->DebugLog(wxT_2("Parsing project file..."));
    TiXmlElement* root;

    root = doc.FirstChildElement("VisualStudioProject");
    if (!root)
    {
        pMsg->DebugLog(wxT_2("Not a valid MS Visual Studio project file..."));
        return false;
    }
    if (strcmp(root->Attribute("ProjectType"), "Visual C++") != 0)
    {
        pMsg->DebugLog(wxT_2("Project is not Visual C++..."));
        return false;
    }

    wxString ver = cbC2U(root->Attribute("Version"));
    if (ver.IsSameAs(wxT_2("7.0")) || ver.IsSameAs(wxT_2("7.00"))) m_Version = 70;
    if (ver.IsSameAs(wxT_2("7.1")) || ver.IsSameAs(wxT_2("7.10"))) m_Version = 71;
    if (ver.IsSameAs(wxT_2("8.0")) || ver.IsSameAs(wxT_2("8.00"))) m_Version = 80;
    if ((m_Version!=70) && (m_Version!=71))
    {
        // seems to work with visual 8 too ;)
        pMsg->DebugLog(F(wxT("Project version is '%s'. Although this loader was designed for version 7.xx, will try to import..."), ver.c_str()));
    }

    m_pProject->ClearAllProperties();
    m_pProject->SetModified(true);
    m_pProject->SetTitle(cbC2U(root->Attribute("Name")));

    // delete all targets of the project (we 'll create new ones from the imported configurations)
    while (m_pProject->GetBuildTargetsCount())
        m_pProject->RemoveBuildTarget(0);

    return DoSelectConfiguration(root);
}

bool MSVC7Loader::Save(const wxString& filename)
{
    // no support to save MSVC7 projects
    return false;
}

bool MSVC7Loader::DoSelectConfiguration(TiXmlElement* root)
{
    TiXmlElement* config = root->FirstChildElement("Configurations");
    if (!config)
    {
        Manager::Get()->GetLogManager()->DebugLog(wxT_2("No 'Configurations' node..."));
        return false;
    }

    TiXmlElement* confs = config->FirstChildElement("Configuration");
    if (!confs)
    {
        Manager::Get()->GetLogManager()->DebugLog(wxT_2("No 'Configuration' node..."));
        return false;
    }

    // build an array of all configurations
    wxArrayString configurations;
    wxString ConfigName;
    while (confs)
    {
        /*Replace all '|' with '_' so that compilation does not fail.
        * This is vital as object directory names will be derived from target names
        */
        ConfigName = cbC2U(confs->Attribute("Name"));
        ConfigName.Replace(wxT_2("|"), wxT_2(" "), true);
        configurations.Add(ConfigName);
        confs = confs->NextSiblingElement();
    }

    wxArrayInt selected_indices;
    if (ImportersGlobals::ImportAllTargets)
    {
        // don't ask; just fill selected_indices with all indices
        for (size_t i = 0; i < configurations.GetCount(); ++i)
            selected_indices.Add(i);
    }
    else
    {
        // ask the user to select a configuration - multiple choice ;)
        MultiSelectDlg dlg(0, configurations, true, _("Select configurations to import:"), m_pProject->GetTitle());
        PlaceWindow(&dlg);
        if (dlg.ShowModal() == wxID_CANCEL)
        {
            Manager::Get()->GetLogManager()->DebugLog(wxT_2("Cancelled..."));
            return false;
        }
        selected_indices = dlg.GetSelectedIndices();
    }

    confs = config->FirstChildElement("Configuration");
    int current_sel = 0;
    bool success = true;
    for (size_t i = 0; i < selected_indices.GetCount(); ++i)
    {
        // re-iterate configurations to find each selected one
        while (confs && current_sel++ < selected_indices[i])
            confs = confs->NextSiblingElement();
        if (!confs)
        {
            Manager::Get()->GetLogManager()->DebugLog(F(wxT("Cannot find configuration nr %d..."), selected_indices[i]));
            success = false;
            break;
        }

        Manager::Get()->GetLogManager()->DebugLog(wxT_2("Importing configuration: ") + configurations[selected_indices[i]]);

        // prepare the configuration name
        m_ConfigurationName = configurations[selected_indices[i]];

        // do not change the configuration name. we might have duplicates then...

//        int pos = m_ConfigurationName.Find(wxT_2('|'));
//        if (pos != wxNOT_FOUND)
//            m_ConfigurationName.Remove(pos);

        // parse the selected configuration
        success = success && DoImport(confs);
        confs = confs->NextSiblingElement();
    }
    return success && DoImportFiles(root, selected_indices.GetCount());
}

bool MSVC7Loader::DoImport(TiXmlElement* conf)
{
    ProjectBuildTarget* bt = m_pProject->GetBuildTarget(m_ConfigurationName);
    if (!bt)
        bt = m_pProject->AddBuildTarget(m_ConfigurationName);
    bt->SetCompilerID(m_pProject->GetCompilerID());

    // See http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vcext/html/vxlrfvcprojectenginelibraryruntimelibraryoption.asp


    m_OutDir = ReplaceMSVCMacros(cbC2U(conf->Attribute("OutputDirectory")));
    m_IntDir = ReplaceMSVCMacros(cbC2U(conf->Attribute("IntermediateDirectory")));
    if (m_IntDir.StartsWith(wxT_2(".\\"))) m_IntDir.Remove(0,2);
    bt->SetObjectOutput(m_IntDir);

    // see MSDN: ConfigurationTypes Enumeration
    wxString conftype = cbC2U(conf->Attribute("ConfigurationType"));
    if (conftype.IsSameAs(wxT_2("1"))) // typeApplication 1, no difference between console or gui here, we must check the subsystem property of the linker
        bt->SetTargetType(ttExecutable);
    else if (conftype.IsSameAs(wxT_2("2"))) // typeDynamicLibrary 2
        bt->SetTargetType(ttDynamicLib);
    else if (conftype.IsSameAs(wxT_2("4"))) // typeStaticLibrary 4
        bt->SetTargetType(ttStaticLib);
    else if (conftype.IsSameAs(wxT_2("-1"))) // typeNative -1, check subsystem property of the linker to make sure
        bt->SetTargetType(ttNative);
    else if (conftype.IsSameAs(wxT_2("10"))) // typeGeneric 10
        bt->SetTargetType(ttCommandsOnly);
    else { // typeUnknown 0
        bt->SetTargetType(ttCommandsOnly);
        Manager::Get()->GetLogManager()->DebugLog(wxT_2("unrecognized project type"));
    }

    TiXmlElement* tool = conf->FirstChildElement("Tool");
    if (!tool)
    {
        Manager::Get()->GetLogManager()->DebugLog(wxT_2("No 'Tool' node..."));
        return false;
    }

    while (tool)
    {
        if (strcmp(tool->Attribute("Name"), "VCLinkerTool") == 0 ||
            strcmp(tool->Attribute("Name"), "VCLibrarianTool") == 0)
        {
            // linker
            wxString tmp;

            if ((bt->GetTargetType()==ttExecutable) || (bt->GetTargetType()==ttNative)) {
                tmp = cbC2U(tool->Attribute("SubSystem"));
                //subSystemNotSet 0
                //subSystemConsole 1
                //subSystemWindows 2
                if (tmp.IsSameAs(wxT_2("1"))) {
                    bt->SetTargetType(ttConsoleOnly);
                    //bt->AddLinkerOption("/SUBSYSTEM:CONSOLE"); // don't know if it is necessary
                }
                else if (tmp.IsSameAs(wxT_2("3"))) {
                    bt->SetTargetType(ttNative);
                }
            } // else we keep executable

            tmp = ReplaceMSVCMacros(cbC2U(tool->Attribute("OutputFile")));
            tmp = UnixFilename(tmp);
            if (tmp.Last() == wxT_2('.')) tmp.RemoveLast();
            if (bt->GetTargetType() == ttStaticLib) {
                // convert the lib name
                Compiler* compiler = CompilerFactory::GetCompiler(m_pProject->GetCompilerID());
                wxString prefix = compiler->GetSwitches().libPrefix;
                wxString suffix = compiler->GetSwitches().libExtension;
                wxFileName fname = tmp;
                if (!fname.GetName().StartsWith(prefix))
                    fname.SetName(prefix + fname.GetName());
                fname.SetExt(suffix);
                tmp = fname.GetFullPath();
            }
            bt->SetOutputFilename(tmp);
            m_TargetPath = wxFileName(tmp).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
            m_TargetFilename = wxFileName(tmp).GetFullName();

            tmp = cbC2U(tool->Attribute("AdditionalLibraryDirectories"));
            wxArrayString arr;
            ParseInputString(tmp, arr);
            for (unsigned int i = 0; i < arr.GetCount(); ++i)
            {
                bt->AddLibDir(ReplaceMSVCMacros(arr[i]));
            }

            if (!m_ConvertSwitches) // no point importing this option, if converting to GCC
            {
                tmp = cbC2U(tool->Attribute("IgnoreDefaultLibraryNames"));
                arr = GetArrayFromString(tmp, wxT_2(";"));
                if (arr.GetCount()==1) arr = GetArrayFromString(tmp, wxT_2(","));
                for (unsigned int i = 0; i < arr.GetCount(); ++i)
                {
                    bt->AddLinkerOption(wxString(wxT_2("/NODEFAULTLIB:")) + arr[i]);
                }
            }

#if 0
            // no need since "/nologo" appear on the invocation commands of compilers/linkers
            if (!m_ConvertSwitches)
            {
                tmp = tool->Attribute("SuppressStartupBanner");
                if (tmp.IsSameAs("TRUE"))
                    bt->AddLinkerOption("/nologo");
            }
#endif

            tmp = cbC2U(tool->Attribute("GenerateDebugInformation"));
            if (tmp.IsSameAs(wxT_2("TRUE")))
            {
                //bt->AddCompilerOption(m_ConvertSwitches ? "-g" : "/Zi"); // no !
                if (!m_ConvertSwitches)
                    bt->AddLinkerOption(wxT_2("/debug"));
            }

            // other options: /MACHINE:I386, /INCREMENTAL:YES, /STACK:10000000
            if (!m_ConvertSwitches) {
                arr = GetArrayFromString(ReplaceMSVCMacros(cbC2U(tool->Attribute("AdditionalOptions"))), wxT_2(" "));
                for (unsigned int i = 0; i < arr.GetCount(); ++i) bt->AddLinkerOption(arr[i]);
            }
            // else ignore all options

            tmp = ReplaceMSVCMacros(cbC2U(tool->Attribute("AdditionalDependencies")));
            arr = GetArrayFromString(tmp, wxT_2(" "));
            for (unsigned int i = 0; i < arr.GetCount(); ++i)
            {
                tmp = arr[i];
                if (tmp.Right(4).CmpNoCase(wxT_2(".lib")) == 0)
                    tmp.Remove(tmp.Length() - 4);
                bt->AddLinkLib(tmp);
            }

            if (!m_ConvertSwitches)
            {
                tmp = cbC2U(tool->Attribute("LinkIncremental"));
                if (tmp.IsSameAs(wxT_2("1"))) // 1 -> no, default is yes
                    bt->AddLinkerOption(wxT_2("/INCREMENTAL:NO"));
            }

            if (!m_ConvertSwitches)
            {
                tmp = ReplaceMSVCMacros(cbC2U(tool->Attribute("ProgramDatabaseFile")));
                if (!tmp.IsEmpty())
                    bt->AddLinkerOption(wxString(wxT_2("/pdb:")) + UnixFilename(tmp));
            }

            if (!m_ConvertSwitches)
            {
                tmp = ReplaceMSVCMacros(cbC2U(tool->Attribute("ModuleDefinitionFile")));
                if (!tmp.IsEmpty())
                    bt->AddLinkerOption(wxT_2("/DEF:\"") + tmp + wxT_2("\""));
            }
        }
        else if (strcmp(tool->Attribute("Name"), "VCCLCompilerTool") == 0)
        {
            unsigned int i;
            wxString tmp;
            wxArrayString arr;

            // compiler
            tmp = cbC2U(tool->Attribute("AdditionalIncludeDirectories"));
            // vc70 uses ";" while vc71 uses "," separators
            // NOTE (mandrav#1#): No, that is *not* the case (what were they thinking at MS?)
            // try with comma (,) which is the newest I believe
            ParseInputString(tmp, arr); // This will parse recursively
            for (i = 0; i < arr.GetCount(); ++i)
            {
                bt->AddIncludeDir(ReplaceMSVCMacros(arr[i]));
                bt->AddResourceIncludeDir(ReplaceMSVCMacros(arr[i]));
            }

            tmp = cbC2U(tool->Attribute("PreprocessorDefinitions"));
            arr = GetArrayFromString(tmp, wxT_2(","));
            if (arr.GetCount() == 1) // if it fails, try with semicolon
                arr = GetArrayFromString(tmp, wxT_2(";"));
            for (unsigned int i = 0; i < arr.GetCount(); ++i)
            {
                if (m_ConvertSwitches)
                    bt->AddCompilerOption(wxString(wxT_2("-D")) + arr[i]);
                else
                    bt->AddCompilerOption(wxString(wxT_2("/D")) + arr[i]);
            }

            tmp = cbC2U(tool->Attribute("WarningLevel"));
            if (m_ConvertSwitches)
            {
                if (tmp.IsSameAs(wxT_2("0")))
                    bt->AddCompilerOption(wxT_2("-w"));
                else if (tmp.IsSameAs(wxT_2("1")) || tmp.IsSameAs(wxT_2("2")) || tmp.IsSameAs(wxT_2("3")))
                    bt->AddCompilerOption(wxT_2("-W"));
                else if (tmp.IsSameAs(wxT_2("4")))
                    bt->AddCompilerOption(wxT_2("-Wall"));
            }
            else
            {
                bt->AddCompilerOption(wxString(wxT_2("/W")) + tmp);
            }

/* For more details on "DebugInformationFormat", please visit
   http://msdn2.microsoft.com/en-us/library/aa652260(VS.71).aspx
   http://msdn2.microsoft.com/en-us/library/microsoft.visualstudio.vcprojectengine.debugoption(VS.80).aspx */
            tmp = cbC2U(tool->Attribute("DebugInformationFormat"));
            if (tmp.IsSameAs(wxT_2("3")))
                bt->AddCompilerOption(m_ConvertSwitches ? wxT_2("") : wxT_2("/Zi"));
            else if (tmp.IsSameAs(wxT_2("4")))
                bt->AddCompilerOption(m_ConvertSwitches ? wxT_2("-g") : wxT_2("/ZI"));


            tmp = cbC2U(tool->Attribute("InlineFunctionExpansion"));
            if (!m_ConvertSwitches && tmp.IsSameAs(wxT_2("1"))) bt->AddCompilerOption(wxT_2("/Ob1"));

            /* Optimization :
            optimizeDisabled 0
            optimizeMinSpace 1
            optimizeMaxSpeed 2
            optimizeFull 3
            optimizeCustom 4
            */
            tmp = cbC2U(tool->Attribute("Optimization"));
            if (m_ConvertSwitches)
            {
                if      (tmp.IsSameAs(wxT_2("0"))) bt->AddCompilerOption(wxT_2("-O0"));
                else if (tmp.IsSameAs(wxT_2("1"))) bt->AddCompilerOption(wxT_2("-O1"));
                else if (tmp.IsSameAs(wxT_2("2"))) bt->AddCompilerOption(wxT_2("-O2"));
                else if (tmp.IsSameAs(wxT_2("3"))) bt->AddCompilerOption(wxT_2("-O3"));
                //else if (tmp.IsSameAs("4")) bt->AddCompilerOption("-O1"); // nothing to do ?
            }
            else
            {
                if      (tmp.IsSameAs(wxT_2("0"))) bt->AddCompilerOption(wxT_2("/Od"));
                else if (tmp.IsSameAs(wxT_2("1"))) bt->AddCompilerOption(wxT_2("/O1"));
                else if (tmp.IsSameAs(wxT_2("2"))) bt->AddCompilerOption(wxT_2("/O2"));
                else if (tmp.IsSameAs(wxT_2("3"))) bt->AddCompilerOption(wxT_2("/Ox"));
                //else if (tmp.IsSameAs("4")) bt->AddCompilerOption("/O1"); // nothing to do ?
            }

            if (!m_ConvertSwitches)
            {
                tmp = cbC2U(tool->Attribute("Detect64BitPortabilityProblems"));
                if (tmp.IsSameAs(wxT_2("TRUE")))
                    bt->AddCompilerOption(wxT_2("/Wp64"));

                tmp = cbC2U(tool->Attribute("MinimalRebuild"));
                if (tmp.IsSameAs(wxT_2("TRUE")))
                    bt->AddCompilerOption(wxT_2("/Gm"));
/*
                RuntimeLibrary :
                rtMultiThreaded          0 --> /MT
                rtMultiThreadedDebug     1 --> /MTd
                rtMultiThreadedDLL       2 --> /MD
                rtMultiThreadedDebugDLL  3 --> /MDd
                rtSingleThreaded         4 --> /ML
                rtSingleThreadedDebug    5 --> /MLd
*/
                tmp = cbC2U(tool->Attribute("RuntimeLibrary"));
                if      (tmp.IsSameAs(wxT_2("0"))) bt->AddCompilerOption(wxT_2("/MT"));
                else if (tmp.IsSameAs(wxT_2("1"))) bt->AddCompilerOption(wxT_2("/MTd"));
                else if (tmp.IsSameAs(wxT_2("2"))) bt->AddCompilerOption(wxT_2("/MD"));
                else if (tmp.IsSameAs(wxT_2("3"))) bt->AddCompilerOption(wxT_2("/MDd"));
                else if (tmp.IsSameAs(wxT_2("4"))) bt->AddCompilerOption(wxT_2("/ML"));
                else if (tmp.IsSameAs(wxT_2("5"))) bt->AddCompilerOption(wxT_2("/MLd"));

#if 0
                tmp = cbC2U(tool->Attribute("SuppressStartupBanner"));
                if (tmp.IsSameAs(wxT_2("TRUE"))) bt->AddCompilerOption("/nologo");
#endif
/*
                runtimeBasicCheckNone 0
                runtimeCheckStackFrame 1  --> /RTCs or /GZ
                runtimeCheckUninitVariables 2
                runtimeBasicCheckAll 3
*/
                tmp = cbC2U(tool->Attribute("BasicRuntimeChecks"));
                if (tmp.IsSameAs(wxT_2("1")))
                    bt->AddCompilerOption(wxT_2("/GZ"));

                tmp = cbC2U(tool->Attribute("ExceptionHandling"));
                if (tmp.IsSameAs(wxT_2("TRUE"))) bt->AddCompilerOption(wxT_2("EHsc")); // add C++ exception handling

            }

            tmp = cbC2U(tool->Attribute("RuntimeTypeInfo"));
            if (tmp.IsSameAs(wxT_2("TRUE")))
                bt->AddCompilerOption(m_ConvertSwitches ? wxT_2("-frtti") : wxT_2("/GR"));

/*
            AdditionalOptions=" /Zm1000 /GR  -DCMAKE_INTDIR=\&quot;Debug\&quot;"
            ObjectFile="Debug\"
            /Zm<n> max memory alloc (% of default)
*/
            tmp = cbC2U(tool->Attribute("AdditionalOptions"));
            //tmp = ReplaceMSVCMacros(tmp);
            arr = GetArrayFromString(tmp, wxT_2(" "));
            for (i=0; i<arr.GetCount(); ++i)
            {
                if (arr[i].IsSameAs(wxT_2("/D")) || arr[i].IsSameAs(wxT_2("-D")))
                {
                    bt->AddCompilerOption((m_ConvertSwitches? wxT_2("-D"):wxT_2("/D")) + arr[i+1]);
                    ++i;
                }
                else if (arr[i].StartsWith(wxT_2("/D")) || arr[i].StartsWith(wxT_2("-D")))
                    bt->AddCompilerOption((m_ConvertSwitches? wxT_2("-D"):wxT_2("/D")) + arr[i].Mid(2));
                else if (arr[i].IsSameAs(wxT_2("/Zi")))
                    bt->AddCompilerOption(m_ConvertSwitches? wxT_2("-g"):wxT_2("/Zi"));
                else if (!m_ConvertSwitches)
                    bt->AddCompilerOption(arr[i]);
            }

            tmp = cbC2U(tool->Attribute("ForcedIncludeFiles"));
            if (!tmp.IsEmpty())
            {
                wxArrayString FIfiles;
                ParseInputString(tmp, FIfiles);
                for (size_t i = 0; i < FIfiles.GetCount(); ++i)
                    bt->AddCompilerOption(m_ConvertSwitches? wxT_2("-include ") + ReplaceMSVCMacros(FIfiles[i])
                                          : wxT_2("/FI ") + ReplaceMSVCMacros(FIfiles[i]));
            }

        }
        else if (strcmp(tool->Attribute("Name"), "VCPreBuildEventTool") == 0)
        {
            // pre-build step
            wxString cmd = ReplaceMSVCMacros(cbC2U(tool->Attribute("CommandLine")));
            if (!cmd.IsEmpty())
                bt->AddCommandsBeforeBuild(cmd);
        }
        else if (strcmp(tool->Attribute("Name"), "VCPostBuildEventTool") == 0)
        {
            // post-build step
            wxString cmd = ReplaceMSVCMacros(cbC2U(tool->Attribute("CommandLine")));
            if (!cmd.IsEmpty())
                bt->AddCommandsAfterBuild(cmd);
        }
        tool = tool->NextSiblingElement();
    }
    return true;
}

bool MSVC7Loader::DoImportFiles(TiXmlElement* root, int numConfigurations)
{
    if (!root)
        return false;

    TiXmlElement* files = root->FirstChildElement("Files");
    if (!files)
        files = root; // might not have "Files" section
    while (files)
    {
        TiXmlElement* file = files->FirstChildElement("File");
        while (file)
        {
            wxString fname = ReplaceMSVCMacros(cbC2U(file->Attribute("RelativePath")));
            if ((!fname.IsEmpty()) && (fname != wxT_2(".\\")))
            {
                if (fname.StartsWith(wxT_2(".\\")))
                    fname.erase(0, 2);

                if (!platform::windows)
                    fname.Replace(wxT_2("\\"), wxT_2("/"), true);

                ProjectFile* pf = m_pProject->AddFile(0, fname);
                if (pf)
                {
                    // add it to all configurations, not just the first
                    for (int i = 1; i < numConfigurations; ++i)
                    {
                        pf->AddBuildTarget(m_pProject->GetBuildTarget(i)->GetTitle());
                        HandleFileConfiguration(file, pf); // We need to do this for all files
                    }
                }
            }
            file = file->NextSiblingElement("File");
        }

        // recurse for nested filters
        TiXmlElement* nested = files->FirstChildElement("Filter");
        while (nested)
        {
            DoImportFiles(nested, numConfigurations);
            nested = nested->NextSiblingElement("Filter");
        }

        files = files->NextSiblingElement("Files");
    }

    // recurse for nested filters
    TiXmlElement* nested = root->FirstChildElement("Filter");
    while (nested)
    {
        DoImportFiles(nested, numConfigurations);
        nested = nested->NextSiblingElement("Filter");
    }

    return true;
}

// function contributed by Tim Baker
void MSVC7Loader::HandleFileConfiguration(TiXmlElement* file, ProjectFile* pf)
{
    TiXmlElement* fconf = file->FirstChildElement("FileConfiguration");
    while (fconf)
    {
        if (const char* s = fconf->Attribute("ExcludedFromBuild"))
        {
            wxString exclude = cbC2U(s); // can you initialize wxString from NULL?
            exclude = exclude.MakeUpper();
            if (exclude.IsSameAs(wxT_2("TRUE")))
            {
                wxString name = cbC2U(fconf->Attribute("Name"));
                name.Replace(wxT_2("|"), wxT_2(" "), true); // Replace '|' to ensure proper check
                pf->RemoveBuildTarget(name);
                Manager::Get()->GetLogManager()->DebugLog(
                    F(_("removed %s from %s"),
                    pf->file.GetFullPath().c_str(), name.c_str()));
            }
        }
        fconf = fconf->NextSiblingElement("FileConfiguration");
    }
}

bool MSVC7Loader::ParseInputString(const wxString& Input, wxArrayString& Output)
{
    /* This function will parse an input string recursively
     * with separators (',' and ';') */
    wxArrayString Array1, Array2;
    if (Input.IsEmpty())
        return false;
    Array1 = GetArrayFromString(Input, wxT_2(","));
    for (size_t i = 0; i < Array1.GetCount(); ++i)
    {
        if (Array1[i].Find(wxT_2(";")) != -1)
        {
            Array2 = GetArrayFromString(Array1[i], wxT_2(";"));
            for (size_t j = 0; j < Array2.GetCount(); ++j)
                Output.Add(Array2[j]);
        }
        else
            Output.Add(Array1[i]);
    }
    return true;
}
