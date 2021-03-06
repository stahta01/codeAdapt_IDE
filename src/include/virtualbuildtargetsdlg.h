#ifndef VIRTUALBUILDTARGETSDLG_H
#define VIRTUALBUILDTARGETSDLG_H

#if !defined(CA_BUILD_WITHOUT_wxSMITH)

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(VirtualBuildTargetsDlg)
#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
//*)

class cbProject;

class VirtualBuildTargetsDlg: public wxDialog
{
	public:

		VirtualBuildTargetsDlg(wxWindow* parent,wxWindowID id, cbProject* project);
		virtual ~VirtualBuildTargetsDlg();

		//(*Identifiers(VirtualBuildTargetsDlg)
		//*)

	protected:

		//(*Handlers(VirtualBuildTargetsDlg)
		void OnAddClick(wxCommandEvent& event);
		void OnEditClick(wxCommandEvent& event);
		void OnRemoveClick(wxCommandEvent& event);
		void OnAliasesSelect(wxCommandEvent& event);
		void OnTargetsToggled(wxCommandEvent& event);
		//*)

		void OnUpdateUI(wxUpdateUIEvent& event);

		//(*Declarations(VirtualBuildTargetsDlg)
		wxListBox* lstAliases;
		wxButton* btnAdd;
		wxButton* btnEdit;
		wxButton* btnRemove;
		wxCheckListBox* lstTargets;
		//*)

        void SetVirtualTarget(const wxString& targetName);
        void CheckTargets();
		cbProject* m_pProject;

	private:

		DECLARE_EVENT_TABLE()
};

#endif // #if !defined(CA_BUILD_WITHOUT_wxSMITH)

#endif
