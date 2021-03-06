#ifndef EMBEDDEDHTMLPANEL_H
#define EMBEDDEDHTMLPANEL_H

#if !defined(CA_BUILD_WITHOUT_wxSMITH)

//(*HeadersPCH(EmbeddedHtmlPanel)
#include <wx/panel.h>
class wxStaticText;
class wxBitmapButton;
class wxBoxSizer;
//*)

//(*Headers(EmbeddedHtmlPanel)
class wxHtmlWindow;
//*)

class wxHtmlLinkEvent;

class EmbeddedHtmlPanel: public wxPanel
{
	public:

		EmbeddedHtmlPanel(wxWindow* parent);
		virtual ~EmbeddedHtmlPanel();

		void Open(const wxString& url);

		//(*Declarations(EmbeddedHtmlPanel)
		wxBitmapButton* btnBack;
		wxPanel* Panel1;
		wxHtmlWindow* winHtml;
		wxStaticText* lblStatus;
		wxBitmapButton* btnForward;
		//*)

	protected:

		//(*Identifiers(EmbeddedHtmlPanel)
		static const long ID_BITMAPBUTTON2;
		static const long ID_BITMAPBUTTON3;
		static const long ID_STATICTEXT1;
		static const long ID_PANEL1;
		static const long ID_HTMLWINDOW1;
		//*)

	private:
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnLinkClicked(wxHtmlLinkEvent &event);

		//(*Handlers(EmbeddedHtmlPanel)
		void OnbtnBackClick(wxCommandEvent& event);
		void OnbtnForwardClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif // #if !defined(CA_BUILD_WITHOUT_wxSMITH)

#endif
