#ifndef CONFIRMREPLACEDLG_H
#define CONFIRMREPLACEDLG_H

#if !defined(CA_BUILD_WITHOUT_WXSCINTILLA) 

#include <wx/dialog.h>
#include <wx/intl.h>
#include "settings.h"

class cbStyledTextCtrl;

#endif // #if !defined(CA_BUILD_WITHOUT_WXSCINTILLA) 

enum ConfirmResponse
{
	crYes = 0,
	crNo,
	crAllInFile,
	crSkipFile,
	crAll,
	crCancel
};

#if !defined(CA_BUILD_WITHOUT_WXSCINTILLA) 

class ConfirmReplaceDlg : public wxDialog
{
	public:
		ConfirmReplaceDlg(wxWindow* parent, bool replaceInFiles = false,
            const wxString& label = _("Replace this occurrence?"));
		~ConfirmReplaceDlg();
		void OnYes(wxCommandEvent& event);
		void OnNo(wxCommandEvent& event);
		void OnAllInFile(wxCommandEvent& event);
		void OnSkipFile(wxCommandEvent& event);
		void OnAll(wxCommandEvent& event);
		void OnCancel(wxCommandEvent& event);
		void CalcPosition(cbStyledTextCtrl* ed);

	private:
		DECLARE_EVENT_TABLE()
};

#endif // #if !defined(CA_BUILD_WITHOUT_WXSCINTILLA) 

#endif // CONFIRMREPLACEDLG_H
