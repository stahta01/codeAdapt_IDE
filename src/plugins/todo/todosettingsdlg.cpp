#include "sdk.h"
#ifndef CB_PRECOMP
  #include <wx/checkbox.h>
  #include <wx/intl.h>
  #include <wx/string.h>
  #include <wx/xrc/xmlres.h>
  #include "configmanager.h"
  #include "manager.h"
#endif
#include "todosettingsdlg.h"

ToDoSettingsDlg::ToDoSettingsDlg(wxWindow* parent)
{
    //ctor
    wxXmlResource::Get()->LoadPanel(this, parent, wxT_2("ToDoSettingsDlg"));
    bool checked = Manager::Get()->GetConfigManager(wxT_2("todo_list"))->ReadBool(wxT_2("auto_refresh"), true);
    bool standalone = Manager::Get()->GetConfigManager(wxT_2("todo_list"))->ReadBool(wxT_2("stand_alone"), true);
    XRCCTRL(*this, "chkAutoRefresh", wxCheckBox)->SetValue(checked);
    XRCCTRL(*this, "chkMessagesPane", wxCheckBox)->SetValue(!standalone);
}

ToDoSettingsDlg::~ToDoSettingsDlg()
{
    //dtor
}

void ToDoSettingsDlg::OnApply()
{
    bool checked = XRCCTRL(*this, "chkAutoRefresh", wxCheckBox)->GetValue();
    bool standalone = !(XRCCTRL(*this, "chkMessagesPane", wxCheckBox)->GetValue());
    Manager::Get()->GetConfigManager(wxT_2("todo_list"))->Write(wxT_2("auto_refresh"), checked);
    Manager::Get()->GetConfigManager(wxT_2("todo_list"))->Write(wxT_2("stand_alone"), standalone);
}
