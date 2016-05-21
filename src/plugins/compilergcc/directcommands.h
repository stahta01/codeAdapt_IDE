#ifndef DIRECTCOMMANDS_H
#define DIRECTCOMMANDS_H

#include <wx/string.h>
#include <wx/hashmap.h>

#define COMPILER_SIMPLE_LOG 	_T("SLOG:")
#define COMPILER_TARGET_CHANGE  _T("TGT:")
#define COMPILER_WAIT			_T("WAIT")
#define COMPILER_WAIT_LINK		_T("LINK")

// forward decls
class CompilerGCC;
class Compiler;
class cbProject;
class caProjectBuildTarget;
class ProjectFile;
class pfDetails;

WX_DEFINE_ARRAY(ProjectFile*, MyFilesArray); // keep our own copy, to sort it by file weight (priority)

class DirectCommands
{
    public:
        DirectCommands(CompilerGCC* compilerPlugin,
                        Compiler* compiler,
                        cbProject* project,
                        int logPageIndex = 0);
        ~DirectCommands();

        wxArrayString GetPreBuildCommands(caProjectBuildTarget* target);
        wxArrayString GetPostBuildCommands(caProjectBuildTarget* target);
        wxArrayString CompileFile(caProjectBuildTarget* target, ProjectFile* pf, bool force = false);
        wxArrayString GetCompileFileCommand(caProjectBuildTarget* target, ProjectFile* pf);
        wxArrayString GetCompileSingleFileCommand(const wxString& filename);
        wxArrayString GetCompileCommands(caProjectBuildTarget* target, bool force = false);
        wxArrayString GetTargetCompileCommands(caProjectBuildTarget* target, bool force = false);
        wxArrayString GetLinkCommands(caProjectBuildTarget* target, bool force = false);
        wxArrayString GetTargetLinkCommands(caProjectBuildTarget* target, bool force = false);
        wxArrayString GetCleanCommands(caProjectBuildTarget* target, bool distclean = false);
        wxArrayString GetCleanSingleFileCommand(const wxString& filename);
        wxArrayString GetTargetCleanCommands(caProjectBuildTarget* target, bool distclean = false);
        bool m_doYield;
    protected:
        bool AreExternalDepsOutdated(const wxString& buildOutput, const wxString& additionalFiles, const wxString& externalDeps);
        bool IsObjectOutdated(caProjectBuildTarget* target, const pfDetails& pfd, wxString* errorStr = 0);
        void DepsSearchStart(caProjectBuildTarget* target);
        MyFilesArray GetProjectFilesSortedByWeight(caProjectBuildTarget* target, bool compile, bool link);
        void AddCommandsToArray(const wxString& cmds, wxArrayString& array, bool isWaitCmd = false, bool isLinkCmd = false);

        int m_PageIndex;
        CompilerGCC* m_pCompilerPlugin;
        Compiler* m_pCompiler;
        cbProject* m_pProject;
        caProjectBuildTarget* m_pCurrTarget; // temp
    private:
};

#endif // DIRECTCOMMANDS_H
