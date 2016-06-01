/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 10270 $
 * $Id: infopanel.cpp 10270 2015-05-15 10:57:08Z jenslody $
 * $HeadURL: http://svn.code.sf.net/p/codeblocks/code/branches/release-16.xx/src/plugins/scriptedwizard/infopanel.cpp $
 */

#include <sdk.h>
#ifndef CB_PRECOMP
    //(*InternalHeadersPCH(InfoPanel)
    #include <wx/string.h>
    #include <wx/intl.h>
    //*)
#endif // CB_PRECOMP

#include "infopanel.h"


//(*IdInit(InfoPanel)
const long InfoPanel::ID_STATICTEXT1 = wxNewId();
const long InfoPanel::ID_CHECKBOX1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(InfoPanel,wxPanel)
	//(*EventTable(InfoPanel)
	//*)
END_EVENT_TABLE()

InfoPanel::InfoPanel(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(InfoPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT_2("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	lblIntro = new wxStaticText(this, ID_STATICTEXT1, _("Welcome to the new console application wizard!\n\n\n\n\n\n\n\n\n\n\n\n\n\n"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, wxT_2("ID_STATICTEXT1"));
	BoxSizer1->Add(lblIntro, 1, wxALL|wxEXPAND, 8);
	chkSkip = new wxCheckBox(this, ID_CHECKBOX1, _("Skip this page next time"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, wxT_2("ID_CHECKBOX1"));
	chkSkip->SetValue(false);
	BoxSizer1->Add(chkSkip, 0, wxALL|wxEXPAND, 8);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	//*)
}

InfoPanel::~InfoPanel()
{
}

