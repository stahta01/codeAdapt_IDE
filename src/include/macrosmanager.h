#ifndef MACROSMANAGER_H
#define MACROSMANAGER_H

#include "settings.h"
#include "manager.h"
#include <wx/regex.h>
#include <wx/filename.h>

// forward decls;
class wxMenuBar;
class caProject;
class caProjectBuildTarget;
class EditorBase;
class UserVariableManager;

WX_DECLARE_STRING_HASH_MAP( wxString, MacrosMap );

class DLLIMPORT MacrosManager : public Mgr<MacrosManager>
{
public:
    friend class Manager;
    friend class Mgr<MacrosManager>;
    void CreateMenu(wxMenuBar* menuBar);
    void ReleaseMenu(wxMenuBar* menuBar);
    void ReplaceMacros(wxString& buffer, caProjectBuildTarget* target = 0, bool subrequest = false);
    wxString ReplaceMacros(const wxString& buffer, caProjectBuildTarget* target = 0);
    void ReplaceEnvVars(wxString& buffer){ ReplaceMacros(buffer); }  /* misnomer, should be ReplaceVariables */;
    void RecalcVars(caProject* project,EditorBase* editor,caProjectBuildTarget* target);
    void ClearProjectKeys();
protected:
    caProjectBuildTarget* m_lastTarget;
    caProject* m_lastProject;
    EditorBase* m_lastEditor;
    wxFileName m_prjname;
    wxString m_AppPath, m_DataPath, m_Plugins, m_ActiveEditorFilename,
    m_ProjectFilename, m_ProjectName, m_ProjectDir, m_ProjectTopDir,
    m_ProjectFiles, m_Makefile, m_TargetOutputDir, m_TargetName,
    m_TargetOutputBaseName, m_TargetFilename;
	MacrosMap macros;
    wxRegEx m_re_unx;
    wxRegEx m_re_dos;
    wxRegEx m_re_if;
    wxRegEx m_re_ifsp;
    wxRegEx m_re_script;
    UserVariableManager *m_uVarMan;
public:
    void Reset();
private:
    MacrosManager();
    ~MacrosManager();
    wxString EvalCondition(const wxString& cond, const wxString& true_clause, const wxString& false_clause, caProjectBuildTarget* target);
};

#endif // MACROSMANAGER_H

