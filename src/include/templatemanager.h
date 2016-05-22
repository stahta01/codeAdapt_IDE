#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H

#include "settings.h"
#include "manager.h"
#include <wx/event.h>

#ifndef CB_PRECOMP
    #include "globals.h"
#endif

//forward decls
class wxMenuBar;
class wxMenu;
class caProject;
class NewFromTemplateDlg;

class DLLIMPORT TemplateManager : public Mgr<TemplateManager>, public wxEvtHandler
{
        friend class Mgr<TemplateManager>;
	public:
		void CreateMenu(wxMenuBar* menuBar);
		void ReleaseMenu(wxMenuBar* menuBar);
		void BuildToolsMenu(wxMenu* menu);

		caProject* New(TemplateOutputType initial = totProject, wxString* pFilename = 0);
		wxString GetLastCreatedFilename() const;
		void SaveUserTemplate(caProject* prj);
	protected:
		void LoadTemplates();
		void LoadUserTemplates();
		caProject* NewFromTemplate(NewFromTemplateDlg& dlg, wxString* pFilename = 0);
		caProject* NewProjectFromUserTemplate(NewFromTemplateDlg& dlg, wxString* pFilename = 0);
		wxArrayString m_UserTemplates;
	private:
		TemplateManager();
		virtual ~TemplateManager();
};

#endif // TEMPLATEMANAGER_H
