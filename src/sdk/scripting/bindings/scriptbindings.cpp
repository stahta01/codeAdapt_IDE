#include <sdk_precomp.h>
#ifndef CB_PRECOMP
    #include <settings.h>
    #include <manager.h>
    #include <logmanager.h>
    #include <configmanager.h>
    #include <editormanager.h>
    #include <projectmanager.h>
    #include <macrosmanager.h>
    #include <compilerfactory.h>
    #include <cbproject.h>
    #include <cbeditor.h>
    #include <globals.h>
#endif
#include "cbstyledtextctrl.h"

#include "scriptbindings.h"
#include <cbexception.h>
#include "sc_base_types.h"

namespace ScriptBindings
{
    extern void Register_Constants();
    extern void Register_Globals();
    extern void Register_wxTypes();
    extern void Register_Dialog();
    extern void Register_ProgressDialog();
    extern void Register_UtilDialogs();
    extern void Register_IO();
    extern void Register_ScriptPlugin();

    SQInteger ConfigManager_Read(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            if (sa.GetType(3) == OT_INTEGER)
                return sa.Return((SQInteger)Manager::Get()->GetConfigManager(_T("scripts"))->ReadInt(key, sa.GetInt(3)));
            else if (sa.GetType(3) == OT_BOOL)
                return sa.Return(Manager::Get()->GetConfigManager(_T("scripts"))->ReadBool(key, sa.GetBool(3)));
            else if (sa.GetType(3) == OT_FLOAT)
                return sa.Return((float)Manager::Get()->GetConfigManager(_T("scripts"))->ReadDouble(key, sa.GetFloat(3)));
            else
            {
                wxString val = *SqPlus::GetInstance<wxString,false>(v, 3);
                wxString ret = Manager::Get()->GetConfigManager(_T("scripts"))->Read(key, val);
                return SqPlus::ReturnCopy(v, ret);
            }
        }
        return sa.ThrowError("Invalid arguments to \"ConfigManager::Read\"");
    }
    SQInteger ConfigManager_Write(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            if (sa.GetType(3) == OT_INTEGER)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, (int)sa.GetInt(3));
                return SQ_OK;
            }
            else if (sa.GetType(3) == OT_BOOL)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, (bool)sa.GetBool(3));
                return SQ_OK;
            }
            else if (sa.GetType(3) == OT_FLOAT)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, sa.GetFloat(3));
                return SQ_OK;
            }
            else
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, *SqPlus::GetInstance<wxString,false>(v, 3));
                return SQ_OK;
            }
        }
        else if (paramCount == 4)
        {
            wxString key = *SqPlus::GetInstance<wxString,false>(v, 2);
            wxString val = *SqPlus::GetInstance<wxString,false>(v, 3);
            if (sa.GetType(4) == OT_BOOL)
            {
                Manager::Get()->GetConfigManager(_T("scripts"))->Write(key, val, sa.GetBool(4));
                return SQ_OK;
            }
        }
        return sa.ThrowError("Invalid arguments to \"ConfigManager::Write\"");
    }
    SQInteger EditorManager_GetBuiltinEditor(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caEditor* ed = 0;
            if (sa.GetType(2) == OT_INTEGER)
                ed = Manager::Get()->GetEditorManager()->GetBuiltinEditor(sa.GetInt(2));
            else
                ed = Manager::Get()->GetEditorManager()->GetBuiltinEditor(*SqPlus::GetInstance<wxString,false>(v, 2));
            SqPlus::Push(v, ed);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"EditorManager::GetBuiltinEditor\"");
    }
    SQInteger EditorManager_Open(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caEditor* ed = Manager::Get()->GetEditorManager()->Open(*SqPlus::GetInstance<wxString,false>(v, 2));
            SqPlus::Push(v, ed);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"EditorManager::Open\"");
    }
    SQInteger EditorManager_Close(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(Manager::Get()->GetEditorManager()->Close(sa.GetInt(2)));
            else
                return sa.Return(Manager::Get()->GetEditorManager()->Close(*SqPlus::GetInstance<wxString,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"EditorManager::Close\"");
    }
    SQInteger EditorManager_Save(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(Manager::Get()->GetEditorManager()->Save(sa.GetInt(2)));
            else
                return sa.Return(Manager::Get()->GetEditorManager()->Save(*SqPlus::GetInstance<wxString,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"EditorManager::Save\"");
    }
    SQInteger cbProject_RemoveFile(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(prj->RemoveFile(sa.GetInt(2)));
            else
                return sa.Return(prj->RemoveFile(SqPlus::GetInstance<ProjectFile,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"caProject::RemoveFile\"");
    }
    SQInteger cbProject_AddFile(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount >= 3)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            wxString str = *SqPlus::GetInstance<wxString,false>(v, 3);
            bool b1 = paramCount >= 4 ? sa.GetBool(4) : true;
            bool b2 = paramCount >= 5 ? sa.GetBool(5) : true;
            int i = paramCount == 6 ? sa.GetInt(6) : 50;
            ProjectFile* pf = 0;
            if (sa.GetType(2) == OT_INTEGER)
                pf = prj->AddFile(sa.GetInt(2), str, b1, b2, i);
            else
                pf = prj->AddFile(*SqPlus::GetInstance<wxString,false>(v, 2), str, b1, b2, i);
            SqPlus::Push(v, pf);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"caProject::AddFile\"");
    }
    SQInteger cbProject_GetBuildTarget(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            caProjectBuildTarget* bt = 0;
            if (sa.GetType(2) == OT_INTEGER)
                bt = prj->GetBuildTarget(sa.GetInt(2));
            else
                bt = prj->GetBuildTarget(*SqPlus::GetInstance<wxString,false>(v, 2));
            SqPlus::Push(v, bt);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"caProject::GetBuildTarget\"");
    }
    SQInteger cbProject_RenameBuildTarget(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(prj->RenameBuildTarget(sa.GetInt(2), *SqPlus::GetInstance<wxString,false>(v, 3)));
            else
                return sa.Return(prj->RenameBuildTarget(*SqPlus::GetInstance<wxString,false>(v, 2), *SqPlus::GetInstance<wxString,false>(v, 3)));
        }
        return sa.ThrowError("Invalid arguments to \"caProject::RenameBuildTarget\"");
    }
    SQInteger cbProject_DuplicateBuildTarget(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 3)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            caProjectBuildTarget* bt = 0;
            if (sa.GetType(2) == OT_INTEGER)
                bt = prj->DuplicateBuildTarget(sa.GetInt(2), *SqPlus::GetInstance<wxString,false>(v, 3));
            else
                bt = prj->DuplicateBuildTarget(*SqPlus::GetInstance<wxString,false>(v, 2), *SqPlus::GetInstance<wxString,false>(v, 3));
            SqPlus::Push(v, bt);
            return 1;
        }
        return sa.ThrowError("Invalid arguments to \"caProject::DuplicateBuildTarget\"");
    }
    SQInteger cbProject_RemoveBuildTarget(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(prj->RemoveBuildTarget(sa.GetInt(2)));
            else
                return sa.Return(prj->RemoveBuildTarget(*SqPlus::GetInstance<wxString,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"caProject::RemoveBuildTarget\"");
    }
    SQInteger cbProject_ExportTargetAsProject(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caProject* prj = SqPlus::GetInstance<caProject,false>(v, 1);
            if (sa.GetType(2) == OT_INTEGER)
                return sa.Return(prj->ExportTargetAsProject(sa.GetInt(2)));
            else
                return sa.Return(prj->ExportTargetAsProject(*SqPlus::GetInstance<wxString,false>(v, 2)));
        }
        return sa.ThrowError("Invalid arguments to \"caProject::ExportTargetAsProject\"");
    }
    SQInteger ProjectManager_AddFileToProject(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 4)
        {
            if (sa.GetType(4) == OT_INTEGER)
            {
                wxString fname = *SqPlus::GetInstance<wxString,false>(v, 2);
                caProject* prj = SqPlus::GetInstance<caProject,false>(v, 3);
                int idx = sa.GetInt(4);
                return sa.Return((SQInteger)Manager::Get()->GetProjectManager()->AddFileToProject(fname, prj, idx));
            }
        }
        return sa.ThrowError("Invalid arguments to \"ProjectManager::AddFileToProject\"");
    }
    SQInteger cbEditor_SetText(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
        {
            caEditor* self = SqPlus::GetInstance<caEditor,false>(v, 1);
            if (self)
            {
                self->GetControl()->SetText(*SqPlus::GetInstance<wxString,false>(v, 2));
                return sa.Return();
            }
            return sa.ThrowError("'this' is NULL!?! (type of caEditor*)");
        }
        return sa.ThrowError("Invalid arguments to \"caEditor::SetText\"");
    }
    SQInteger cbEditor_GetText(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 1)
        {
            caEditor* self = SqPlus::GetInstance<caEditor,false>(v, 1);
            if (self)
            {
                wxString str = self->GetControl()->GetText();
                return SqPlus::ReturnCopy(v, str);
            }
            return sa.ThrowError("'this' is NULL!?! (type of caEditor*)");
        }
        return sa.ThrowError("Invalid arguments to \"caEditor::GetText\"");
    }
    SQInteger CompilerFactory_GetCompilerIndex(HSQUIRRELVM v)
    {
        StackHandler sa(v);
        int paramCount = sa.GetParamCount();
        if (paramCount == 2)
            return sa.Return((SQInteger)CompilerFactory::GetCompilerIndex(*SqPlus::GetInstance<wxString,false>(v, 2)));
        return sa.ThrowError("Invalid arguments to \"CompilerFactory::GetCompilerIndex\"");
    }

    void RegisterBindings()
    {
        if (!SquirrelVM::GetVMPtr())
            cbThrow(_T("Scripting engine not initialized!?"));

        Register_wxTypes();
        Register_Constants();
        Register_Globals();
        Register_IO(); // IO is enabled, but just for harmless functions
        Register_Dialog();
        Register_ProgressDialog();
        Register_UtilDialogs();

        SqPlus::SQClassDef<ConfigManager>("ConfigManager").
                staticFuncVarArgs(&ConfigManager_Read, "Read", "*").
                staticFuncVarArgs(&ConfigManager_Write, "Write", "*");

        SqPlus::SQClassDef<ProjectFile>("ProjectFile").
                func(&ProjectFile::AddBuildTarget, "AddBuildTarget").
                func(&ProjectFile::RenameBuildTarget, "RenameBuildTarget").
                func(&ProjectFile::RemoveBuildTarget, "RemoveBuildTarget").
                func(&ProjectFile::GetBaseName, "GetBaseName").
                func(&ProjectFile::GetObjName, "GetObjName").
                func(&ProjectFile::SetObjName, "SetObjName").
                func(&ProjectFile::GetParentProject, "GetParentProject").
                func(&ProjectFile::SetUseCustomBuildCommand, "SetUseCustomBuildCommand").
                func(&ProjectFile::SetCustomBuildCommand, "SetCustomBuildCommand").
                func(&ProjectFile::GetUseCustomBuildCommand, "GetUseCustomBuildCommand").
                func(&ProjectFile::GetCustomBuildCommand, "GetCustomBuildCommand").
                var(&ProjectFile::file, "file").
                var(&ProjectFile::relativeFilename, "relativeFilename").
                var(&ProjectFile::relativeToCommonTopLevelPath, "relativeToCommonTopLevelPath").
                var(&ProjectFile::compile, "compile").
                var(&ProjectFile::link, "link").
                var(&ProjectFile::weight, "weight").
                var(&ProjectFile::compilerVar, "compilerVar");

        SqPlus::SQClassDef<CompileOptionsBase>("CompileOptionsBase").
                func(&CompileOptionsBase::AddPlatform, "AddPlatform").
                func(&CompileOptionsBase::RemovePlatform, "RemovePlatform").
                func(&CompileOptionsBase::SetPlatforms, "SetPlatforms").
                func(&CompileOptionsBase::GetPlatforms, "GetPlatforms").
                func(&CompileOptionsBase::SupportsCurrentPlatform, "SupportsCurrentPlatform").
                func(&CompileOptionsBase::SetLinkerOptions, "SetLinkerOptions").
                func(&CompileOptionsBase::SetLinkLibs, "SetLinkLibs").
                func(&CompileOptionsBase::SetCompilerOptions, "SetCompilerOptions").
                func(&CompileOptionsBase::SetIncludeDirs, "SetIncludeDirs").
                func(&CompileOptionsBase::SetResourceIncludeDirs, "SetResourceIncludeDirs").
                func(&CompileOptionsBase::SetLibDirs, "SetLibDirs").
                func(&CompileOptionsBase::SetCommandsBeforeBuild, "SetCommandsBeforeBuild").
                func(&CompileOptionsBase::SetCommandsAfterBuild, "SetCommandsAfterBuild").
                func(&CompileOptionsBase::GetLinkerOptions, "GetLinkerOptions").
                func(&CompileOptionsBase::GetLinkLibs, "GetLinkLibs").
                func(&CompileOptionsBase::GetCompilerOptions, "GetCompilerOptions").
                func(&CompileOptionsBase::GetIncludeDirs, "GetIncludeDirs").
                func(&CompileOptionsBase::GetResourceIncludeDirs, "GetResourceIncludeDirs").
                func(&CompileOptionsBase::GetLibDirs, "GetLibDirs").
                func(&CompileOptionsBase::GetCommandsBeforeBuild, "GetCommandsBeforeBuild").
                func(&CompileOptionsBase::GetCommandsAfterBuild, "GetCommandsAfterBuild").
                func(&CompileOptionsBase::GetModified, "GetModified").
                func(&CompileOptionsBase::SetModified, "SetModified").
                func(&CompileOptionsBase::AddLinkerOption, "AddLinkerOption").
                func(&CompileOptionsBase::AddLinkLib, "AddLinkLib").
                func(&CompileOptionsBase::AddCompilerOption, "AddCompilerOption").
                func(&CompileOptionsBase::AddIncludeDir, "AddIncludeDir").
                func(&CompileOptionsBase::AddResourceIncludeDir, "AddResourceIncludeDir").
                func(&CompileOptionsBase::AddLibDir, "AddLibDir").
                func(&CompileOptionsBase::AddCommandsBeforeBuild, "AddCommandsBeforeBuild").
                func(&CompileOptionsBase::AddCommandsAfterBuild, "AddCommandsAfterBuild").
                func(&CompileOptionsBase::RemoveLinkerOption, "RemoveLinkerOption").
                func(&CompileOptionsBase::RemoveLinkLib, "RemoveLinkLib").
                func(&CompileOptionsBase::RemoveCompilerOption, "RemoveCompilerOption").
                func(&CompileOptionsBase::RemoveIncludeDir, "RemoveIncludeDir").
                func(&CompileOptionsBase::RemoveResourceIncludeDir, "RemoveResourceIncludeDir").
                func(&CompileOptionsBase::RemoveLibDir, "RemoveLibDir").
                func(&CompileOptionsBase::RemoveCommandsBeforeBuild, "RemoveCommandsBeforeBuild").
                func(&CompileOptionsBase::RemoveCommandsAfterBuild, "RemoveCommandsAfterBuild").
                func(&CompileOptionsBase::GetAlwaysRunPostBuildSteps, "GetAlwaysRunPostBuildSteps").
                func(&CompileOptionsBase::SetAlwaysRunPostBuildSteps, "SetAlwaysRunPostBuildSteps").
                func(&CompileOptionsBase::SetBuildScripts, "SetBuildScripts").
                func(&CompileOptionsBase::GetBuildScripts, "GetBuildScripts").
                func(&CompileOptionsBase::AddBuildScript, "AddBuildScript").
                func(&CompileOptionsBase::RemoveBuildScript, "RemoveBuildScript").
                func(&CompileOptionsBase::SetVar, "SetVar").
                func(&CompileOptionsBase::GetVar, "GetVar").
                func(&CompileOptionsBase::UnsetVar, "UnsetVar").
                func(&CompileOptionsBase::UnsetAllVars, "UnsetAllVars");

        SqPlus::SQClassDef<caCompileTargetBase>("caCompileTargetBase", "CompileOptionsBase").
                func(&caCompileTargetBase::SetTargetFilenameGenerationPolicy, "SetTargetFilenameGenerationPolicy").
//                func(&caCompileTargetBase::GetTargetFilenameGenerationPolicy, "GetTargetFilenameGenerationPolicy"). // not exposed because its args are non-const references
                func(&caCompileTargetBase::GetFilename, "GetFilename").
                func(&caCompileTargetBase::GetTitle, "GetTitle").
                func(&caCompileTargetBase::SetTitle, "SetTitle").
                func(&caCompileTargetBase::SetOutputFilename, "SetOutputFilename").
                func(&caCompileTargetBase::SetWorkingDir, "SetWorkingDir").
                func(&caCompileTargetBase::SetObjectOutput, "SetObjectOutput").
                func(&caCompileTargetBase::SetDepsOutput, "SetDepsOutput").
                func(&caCompileTargetBase::GetOptionRelation, "GetOptionRelation").
                func(&caCompileTargetBase::SetOptionRelation, "SetOptionRelation").
                func(&caCompileTargetBase::GetWorkingDir, "GetWorkingDir").
                func(&caCompileTargetBase::GetObjectOutput, "GetObjectOutput").
                func(&caCompileTargetBase::GetDepsOutput, "GetDepsOutput").
                func(&caCompileTargetBase::GetOutputFilename, "GetOutputFilename").
                func(&caCompileTargetBase::SuggestOutputFilename, "SuggestOutputFilename").
                func(&caCompileTargetBase::GetExecutableFilename, "GetExecutableFilename").
                func(&caCompileTargetBase::GetDynamicLibFilename, "GetDynamicLibFilename").
                func(&caCompileTargetBase::GetDynamicLibDefFilename, "GetDynamicLibDefFilename").
                func(&caCompileTargetBase::GetStaticLibFilename, "GetStaticLibFilename").
                func(&caCompileTargetBase::GetBasePath, "GetBasePath").
                func(&caCompileTargetBase::SetTargetType, "SetTargetType").
                func(&caCompileTargetBase::GetTargetType, "GetTargetType").
                func(&caCompileTargetBase::GetExecutionParameters, "GetExecutionParameters").
                func(&caCompileTargetBase::SetExecutionParameters, "SetExecutionParameters").
                func(&caCompileTargetBase::GetHostApplication, "GetHostApplication").
                func(&caCompileTargetBase::SetHostApplication, "SetHostApplication").
                func(&caCompileTargetBase::SetCompilerID, "SetCompilerID").
                func(&caCompileTargetBase::GetCompilerID, "GetCompilerID").
                func(&caCompileTargetBase::GetMakeCommandFor, "GetMakeCommandFor").
                func(&caCompileTargetBase::SetMakeCommandFor, "SetMakeCommandFor").
                func(&caCompileTargetBase::MakeCommandsModified, "MakeCommandsModified");

        SqPlus::SQClassDef<caProjectBuildTarget>("caProjectBuildTarget", "caCompileTargetBase").
                func(&caProjectBuildTarget::GetParentProject, "GetParentProject").
                func(&caProjectBuildTarget::GetFullTitle, "GetFullTitle").
                func(&caProjectBuildTarget::GetExternalDeps, "GetExternalDeps").
                func(&caProjectBuildTarget::SetExternalDeps, "SetExternalDeps").
                func(&caProjectBuildTarget::SetAdditionalOutputFiles, "SetAdditionalOutputFiles").
                func(&caProjectBuildTarget::GetAdditionalOutputFiles, "GetAdditionalOutputFiles").
                func(&caProjectBuildTarget::GetIncludeInTargetAll, "GetIncludeInTargetAll").
                func(&caProjectBuildTarget::SetIncludeInTargetAll, "SetIncludeInTargetAll").
                func(&caProjectBuildTarget::GetCreateDefFile, "GetCreateDefFile").
                func(&caProjectBuildTarget::SetCreateDefFile, "SetCreateDefFile").
                func(&caProjectBuildTarget::GetCreateStaticLib, "GetCreateStaticLib").
                func(&caProjectBuildTarget::SetCreateStaticLib, "SetCreateStaticLib").
                func(&caProjectBuildTarget::GetUseConsoleRunner, "GetUseConsoleRunner").
                func(&caProjectBuildTarget::SetUseConsoleRunner, "SetUseConsoleRunner");

        SqPlus::SQClassDef<caProject>("caProject", "caCompileTargetBase").
                func(&caProject::GetModified, "GetModified").
                func(&caProject::SetModified, "SetModified").
                func(&caProject::GetMakefile, "GetMakefile").
                func(&caProject::SetMakefile, "SetMakefile").
                func(&caProject::IsMakefileCustom, "IsMakefileCustom").
                func(&caProject::SetMakefileCustom, "SetMakefileCustom").
                func(&caProject::CloseAllFiles, "CloseAllFiles").
                func(&caProject::SaveAllFiles, "SaveAllFiles").
                func(&caProject::Save, "Save").
//                func(&caProject::SaveAs, "SaveAs"). // *UNSAFE*
                func(&caProject::SaveLayout, "SaveLayout").
                func(&caProject::LoadLayout, "LoadLayout").
                func(&caProject::ShowOptions, "ShowOptions").
                func(&caProject::GetCommonTopLevelPath, "GetCommonTopLevelPath").
                func(&caProject::GetFilesCount, "GetFilesCount").
                func(&caProject::GetFile, "GetFile").
                func(&caProject::GetFileByFilename, "GetFileByFilename").
                staticFuncVarArgs(&cbProject_RemoveFile, "RemoveFile", "*").
                staticFuncVarArgs(&cbProject_AddFile, "AddFile", "*").
                func(&caProject::GetBuildTargetsCount, "GetBuildTargetsCount").
                staticFuncVarArgs(&cbProject_GetBuildTarget, "GetBuildTarget", "*").
                func(&caProject::AddBuildTarget, "AddBuildTarget").
                staticFuncVarArgs(&cbProject_RenameBuildTarget, "RenameBuildTarget", "*").
                staticFuncVarArgs(&cbProject_DuplicateBuildTarget, "DuplicateBuildTarget", "*").
                staticFuncVarArgs(&cbProject_RemoveBuildTarget, "RemoveBuildTarget", "*").
                staticFuncVarArgs(&cbProject_ExportTargetAsProject, "ExportTargetAsProject", "*").
                func(&caProject::BuildTargetValid, "BuildTargetValid").
                func(&caProject::GetFirstValidBuildTargetName, "GetFirstValidBuildTargetName").
                func(&caProject::SetDefaultExecuteTarget, "SetDefaultExecuteTarget").
                func(&caProject::GetDefaultExecuteTarget, "GetDefaultExecuteTarget").
                func(&caProject::SetActiveBuildTarget, "SetActiveBuildTarget").
                func(&caProject::GetActiveBuildTarget, "GetActiveBuildTarget").
                func(&caProject::SelectTarget, "SelectTarget").
                func(&caProject::GetCurrentlyCompilingTarget, "GetCurrentlyCompilingTarget").
                func(&caProject::SetCurrentlyCompilingTarget, "SetCurrentlyCompilingTarget").
                func(&caProject::GetModeForPCH, "GetModeForPCH").
                func(&caProject::SetModeForPCH, "SetModeForPCH").
                func(&caProject::SetExtendedObjectNamesGeneration, "SetExtendedObjectNamesGeneration").
                func(&caProject::GetExtendedObjectNamesGeneration, "GetExtendedObjectNamesGeneration").
                func(&caProject::SetNotes, "SetNotes").
                func(&caProject::GetNotes, "GetNotes").
                func(&caProject::SetShowNotesOnLoad, "SetShowNotesOnLoad").
                func(&caProject::GetShowNotesOnLoad, "GetShowNotesOnLoad").
                func(&caProject::ShowNotes, "ShowNotes").
                func(&caProject::AddToExtensions, "AddToExtensions").
                func(&caProject::DefineVirtualBuildTarget, "DefineVirtualBuildTarget").
                func(&caProject::HasVirtualBuildTarget, "HasVirtualBuildTarget").
                func(&caProject::RemoveVirtualBuildTarget, "RemoveVirtualBuildTarget").
                func(&caProject::GetVirtualBuildTargets, "GetVirtualBuildTargets").
                func(&caProject::GetVirtualBuildTargetGroup, "GetVirtualBuildTargetGroup").
                func(&caProject::GetExpandedVirtualBuildTargetGroup, "GetExpandedVirtualBuildTargetGroup").
                func(&caProject::CanAddToVirtualBuildTarget, "CanAddToVirtualBuildTarget").
                func(&caProject::SetTitle, "SetTitle");


        SqPlus::SQClassDef<ProjectManager>("ProjectManager").
                func(&ProjectManager::GetDefaultPath, "GetDefaultPath").
                func(&ProjectManager::SetDefaultPath, "SetDefaultPath").
                func(&ProjectManager::GetActiveProject, "GetActiveProject").
                func(&ProjectManager::SetProject, "SetProject").
                func(&ProjectManager::LoadWorkspace, "LoadWorkspace").
                func(&ProjectManager::SaveWorkspace, "SaveWorkspace").
                func(&ProjectManager::SaveWorkspaceAs, "SaveWorkspaceAs").
                func(&ProjectManager::CloseWorkspace, "CloseWorkspace").
                func(&ProjectManager::IsOpen, "IsOpen").
                func(&ProjectManager::LoadProject, "LoadProject").
                func(&ProjectManager::SaveProject, "SaveProject").
                func(&ProjectManager::SaveProjectAs, "SaveProjectAs").
                func(&ProjectManager::SaveActiveProject, "SaveActiveProject").
                func(&ProjectManager::SaveActiveProjectAs, "SaveActiveProjectAs").
                func(&ProjectManager::SaveAllProjects, "SaveAllProjects").
                func(&ProjectManager::CloseProject, "CloseProject").
                func(&ProjectManager::CloseActiveProject, "CloseActiveProject").
                func(&ProjectManager::CloseAllProjects, "CloseAllProjects").
                func(&ProjectManager::NewProject, "NewProject").
                staticFuncVarArgs(&ProjectManager_AddFileToProject, "AddFileToProject", "*").
                func(&ProjectManager::AskForBuildTargetIndex, "AskForBuildTargetIndex").
                func(&ProjectManager::RebuildTree, "RebuildTree").
                func(&ProjectManager::AddProjectDependency, "AddProjectDependency").
                func(&ProjectManager::RemoveProjectDependency, "RemoveProjectDependency").
                func(&ProjectManager::ClearProjectDependencies, "ClearProjectDependencies").
                func(&ProjectManager::RemoveProjectFromAllDependencies, "RemoveProjectFromAllDependencies").
                func(&ProjectManager::GetDependenciesForProject, "GetDependenciesForProject").
                func(&ProjectManager::ConfigureProjectDependencies, "ConfigureProjectDependencies");

        SqPlus::SQClassDef<EditorBase>("EditorBase").
                func(&EditorBase::GetFilename, "GetFilename").
                func(&EditorBase::SetFilename, "SetFilename").
                func(&EditorBase::GetShortName, "GetShortName").
                func(&EditorBase::GetModified, "GetModified").
                func(&EditorBase::SetModified, "SetModified").
                func(&EditorBase::GetTitle, "GetTitle").
                func(&EditorBase::SetTitle, "SetTitle").
                func(&EditorBase::Activate, "Activate").
                func(&EditorBase::Close, "Close").
                func(&EditorBase::Save, "Save").
                func(&EditorBase::IsBuiltinEditor, "IsBuiltinEditor").
                func(&EditorBase::ThereAreOthers, "ThereAreOthers").
                func(&EditorBase::GotoLine, "GotoLine").
                func(&EditorBase::ToggleBreakpoint, "ToggleBreakpoint").
                func(&EditorBase::HasBreakpoint, "HasBreakpoint").
                func(&EditorBase::GotoNextBreakpoint, "GotoNextBreakpoint").
                func(&EditorBase::GotoPreviousBreakpoint, "GotoPreviousBreakpoint").
                func(&EditorBase::ToggleBookmark, "ToggleBookmark").
                func(&EditorBase::HasBookmark, "HasBookmark").
                func(&EditorBase::GotoNextBookmark, "GotoNextBookmark").
                func(&EditorBase::GotoPreviousBookmark, "GotoPreviousBookmark").
                func(&EditorBase::Undo, "Undo").
                func(&EditorBase::Redo, "Redo").
                func(&EditorBase::Cut, "Cut").
                func(&EditorBase::Copy, "Copy").
                func(&EditorBase::Paste, "Paste").
                func(&EditorBase::CanUndo, "CanUndo").
                func(&EditorBase::CanRedo, "CanRedo").
                func(&EditorBase::CanPaste, "CanPaste").
                func(&EditorBase::IsReadOnly, "IsReadOnly").
                func(&EditorBase::HasSelection, "HasSelection");

        SqPlus::SQClassDef<caEditor>("caEditor", "EditorBase").
                func(&caEditor::SetEditorTitle, "SetEditorTitle").
                func(&caEditor::GetProjectFile, "GetProjectFile").
                func(&caEditor::Save, "Save").
                func(&caEditor::SaveAs, "SaveAs").
                func(&caEditor::FoldAll, "FoldAll").
                func(&caEditor::UnfoldAll, "UnfoldAll").
                func(&caEditor::ToggleAllFolds, "ToggleAllFolds").
                func(&caEditor::FoldBlockFromLine, "FoldBlockFromLine").
                func(&caEditor::UnfoldBlockFromLine, "UnfoldBlockFromLine").
                func(&caEditor::ToggleFoldBlockFromLine, "ToggleFoldBlockFromLine").
                func(&caEditor::GetLineIndentInSpaces, "GetLineIndentInSpaces").
                func(&caEditor::GetLineIndentString, "GetLineIndentString").
                func(&caEditor::Touch, "Touch").
                func(&caEditor::Reload, "Reload").
                func(&caEditor::Print, "Print").
                func(&caEditor::AutoComplete, "AutoComplete").
                func(&caEditor::AddBreakpoint, "AddBreakpoint").
                func(&caEditor::RemoveBreakpoint, "RemoveBreakpoint").
                // these are not present in caEditor; included to help scripts edit text
                staticFuncVarArgs(&cbEditor_SetText, "SetText", "*").
                staticFuncVarArgs(&cbEditor_GetText, "GetText", "*");

        SqPlus::SQClassDef<EditorManager>("EditorManager").
                func(&EditorManager::Configure, "Configure").
                func(&EditorManager::New, "New").
                staticFuncVarArgs(&EditorManager_Open, "Open").
                func(&EditorManager::IsBuiltinOpen, "IsBuiltinOpen").
                staticFuncVarArgs(&EditorManager_GetBuiltinEditor, "GetBuiltinEditor", "*").
                func(&EditorManager::GetBuiltinActiveEditor, "GetBuiltinActiveEditor").
                func(&EditorManager::GetActiveEditor, "GetActiveEditor").
                func(&EditorManager::ActivateNext, "ActivateNext").
                func(&EditorManager::ActivatePrevious, "ActivatePrevious").
                func(&EditorManager::SwapActiveHeaderSource, "SwapActiveHeaderSource").
                func(&EditorManager::CloseActive, "CloseActive").
                staticFuncVarArgs(&EditorManager_Close, "Close", "*").
                func(&EditorManager::CloseAll, "CloseAll").
                staticFuncVarArgs(&EditorManager_Save, "Save", "*").
                func(&EditorManager::SaveActive, "SaveActive").
                func(&EditorManager::SaveAs, "SaveAs").
                func(&EditorManager::SaveActiveAs, "SaveActiveAs").
                func(&EditorManager::SaveAll, "SaveAll").
                func(&EditorManager::ShowFindDialog, "ShowFindDialog");

        SqPlus::SQClassDef<UserVariableManager>("UserVariableManager").
                func(&UserVariableManager::Exists, "Exists");

        SqPlus::SQClassDef<ScriptingManager>("ScriptingManager").
                func(&ScriptingManager::RegisterScriptMenu, "RegisterScriptMenu");

        typedef bool(*CF_INHERITSFROM)(const wxString&, const wxString&); // CompilerInheritsFrom

        SqPlus::SQClassDef<CompilerFactory>("CompilerFactory").
                staticFunc(&CompilerFactory::IsValidCompilerID, "IsValidCompilerID").
                staticFuncVarArgs(&CompilerFactory_GetCompilerIndex, "GetCompilerIndex", "*").
                staticFunc(&CompilerFactory::GetDefaultCompilerID, "GetDefaultCompilerID").
                staticFunc(&CompilerFactory::GetCompilerVersionString, "GetCompilerVersionString").
                staticFunc<CF_INHERITSFROM>(&CompilerFactory::CompilerInheritsFrom, "CompilerInheritsFrom");

        SqPlus::SQClassDef<caPluginInfo>("caPluginInfo").
            emptyCtor().
            var(&caPluginInfo::name, "name").
            var(&caPluginInfo::title, "title").
            var(&caPluginInfo::version, "version").
            var(&caPluginInfo::description, "description").
            var(&caPluginInfo::author, "author").
            var(&caPluginInfo::authorEmail, "authorEmail").
            var(&caPluginInfo::authorWebsite, "authorWebsite").
            var(&caPluginInfo::thanksTo, "thanksTo").
            var(&caPluginInfo::license, "license");

        SqPlus::SQClassDef<caFileTreeData>("caFileTreeData").
            func(&caFileTreeData::GetKind, "GetKind").
            func(&caFileTreeData::GetProject, "GetProject").
            func(&caFileTreeData::GetFileIndex, "GetFileIndex").
            func(&caFileTreeData::GetProjectFile, "GetProjectFile").
            func(&caFileTreeData::GetFolder, "GetFolder").
            func(&caFileTreeData::SetKind, "SetKind").
            func(&caFileTreeData::SetProject, "SetProject").
            func(&caFileTreeData::SetFileIndex, "SetFileIndex").
            func(&caFileTreeData::SetProjectFile, "SetProjectFile").
            func(&caFileTreeData::SetFolder, "SetFolder");

        // called last because it needs a few previously registered types
        Register_ScriptPlugin();
    }
} // namespace ScriptBindings
