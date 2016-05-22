#ifndef COMPILERCOMMANDGENERATOR_H
#define COMPILERCOMMANDGENERATOR_H

#include <map>

#include <wx/string.h>
#include <wx/dynarray.h>
#include "settings.h"
#include "compiletargetbase.h"

class caProject;
class caProjectBuildTarget;
class ProjectFile;
class Compiler;

typedef std::map<caProjectBuildTarget*, wxString> OptionsMap;
typedef std::map<caProjectBuildTarget*, wxArrayString> SearchDirsMap;
typedef std::map<wxString, wxString> BackticksMap;

/** Generate command-lines needed to produce a build.
  * This pre-generates everything when Init() is called.
  *
  * This is used by compilers to generate the needed
  * command lines for a build.
  */
class DLLIMPORT CompilerCommandGenerator
{
    public:
        CompilerCommandGenerator();
        virtual ~CompilerCommandGenerator();

        /** Initialize for use with the specified @c project. */
        virtual void Init(caProject* project);

        /** Get the command line to compile/link the specific file. */
        virtual void GenerateCommandLine(wxString& macro,
                                        caProjectBuildTarget* target,
                                        ProjectFile* pf,
                                        const wxString& file,
                                        const wxString& object,
                                        const wxString& FlatObject,
                                        const wxString& deps);

		/** @brief Get the full include dirs used in the actual command line.
		  *
		  * These are the actual include dirs that will be used for building
		  * and might be different than target->GetIncludeDirs(). This is
		  * because it's the sum of target include dirs + project include dirs +
		  * build-script include dirs.
		  * @note This is only valid after Init() has been called.
		  */
		virtual const wxArrayString& GetCompilerSearchDirs(caProjectBuildTarget* target);

		/** @brief Get the full linker dirs used in the actual command line.
		  *
		  * These are the actual linker dirs that will be used for building
		  * and might be different than target->GetLibDirs(). This is
		  * because it's the sum of target linker dirs + project linker dirs +
		  * build-script linker dirs.
		  * @note This is only valid after Init() has been called.
		  */
		virtual const wxArrayString& GetLinkerSearchDirs(caProjectBuildTarget* target);
    protected:
        virtual void DoBuildScripts(caProject* project, caCompileTargetBase* target, const wxString& funcName);
        virtual wxString GetOrderedOptions(const caProjectBuildTarget* target, OptionsRelationType rel, const wxString& project_options, const wxString& target_options);
        virtual wxArrayString GetOrderedOptions(const caProjectBuildTarget* target, OptionsRelationType rel, const wxArrayString& project_options, const wxArrayString& target_options);
        virtual wxString SetupOutputFilenames(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupIncludeDirs(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupLibrariesDirs(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupResourceIncludeDirs(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupCompilerOptions(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupLinkerOptions(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupLinkLibraries(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString SetupResourceCompilerOptions(Compiler* compiler, caProjectBuildTarget* target);
        virtual wxString FixupLinkLibraries(Compiler* compiler, const wxString& lib);
        virtual void FixPathSeparators(Compiler* compiler, wxString& inAndOut);

        OptionsMap m_Output; ///< output filenames, per-target
        OptionsMap m_StaticOutput; ///< static output filenames, per-target
        OptionsMap m_DefOutput; ///< def output filenames, per-target
        OptionsMap m_Inc; ///< compiler 'include' dirs, per-target
        OptionsMap m_Lib; ///< linker 'include' dirs, per-target
        OptionsMap m_RC; ///< resource compiler 'include' dirs, per-target
        OptionsMap m_CFlags; ///< compiler flags, per-target
        OptionsMap m_LDFlags; ///< linker flags, per-target
        OptionsMap m_LDAdd; ///< link libraries, per-target
        OptionsMap m_RCFlags; ///< resource compiler flags, per-target

        wxString m_PrjIncPath; ///< directive to add the project's top-level path in compiler search dirs (ready for the command line)

		SearchDirsMap m_CompilerSearchDirs; ///< array of final compiler search dirs, per-target
		SearchDirsMap m_LinkerSearchDirs; ///< array of final linker search dirs, per-target
    private:
        wxString ExpandBackticks(wxString& str);
		void SearchDirsFromBackticks(Compiler* compiler, caProjectBuildTarget* target, const wxString& btOutput);
        BackticksMap m_Backticks;
        wxArrayString m_NotLoadedScripts;
        wxArrayString m_ScriptsWithErrors;
};

#endif // COMPILERCOMMANDGENERATOR_H
