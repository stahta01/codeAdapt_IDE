#ifndef TODOSETTINGSDLG_H
#define TODOSETTINGSDLG_H

// #include <wx/dialog.h>
#include <wx/intl.h>
#include "configurationpanel.h"
#include <settings.h>

class ToDoSettingsDlg : public cbConfigurationPanel
{
	public:
		ToDoSettingsDlg(wxWindow* parent);
		~ToDoSettingsDlg();

        virtual wxString GetTitle() const { return _("To-do list"); }
        virtual wxString GetBitmapBaseName() const { return _T("todo"); }
        virtual void OnApply();
        virtual void OnCancel(){}
};

#endif // TODOSETTINGSDLG_H

