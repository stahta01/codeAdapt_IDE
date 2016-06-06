#include "sdk.h"
#ifndef CB_PRECOMP
  #include <wx/button.h>
  #include <wx/checkbox.h>
  #include <wx/choice.h>
  #include <wx/event.h>
  #include <wx/intl.h>
  #include <wx/string.h>
  #include <wx/textctrl.h>
  #include <wx/xrc/xmlres.h>
  #include "globals.h" // cbMessageBox
  #include "manager.h"
  #include "configmanager.h"
#endif

//#define TRACE_SYMTAB_CFG
#ifdef TRACE_SYMTAB_CFG
  #ifndef CB_PRECOMP
    #include "logmanager.h"
  #endif
#endif

#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include "symtabconfig.h"
#include "prep.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

BEGIN_EVENT_TABLE(SymTabConfigDlg, wxDialog)
  EVT_BUTTON(XRCID("btnSearch"),      SymTabConfigDlg::OnSearch)
  EVT_BUTTON(XRCID("btnClose"),       SymTabConfigDlg::OnClose)
  EVT_CHOICE(XRCID("choWhatToDo"),    SymTabConfigDlg::OnWhatToDo)
  EVT_BUTTON(XRCID("btnLibraryPath"), SymTabConfigDlg::OnLibraryPath)
  EVT_BUTTON(XRCID("btnLibrary"),     SymTabConfigDlg::OnLibrary)
  EVT_BUTTON(XRCID("btnNM"),          SymTabConfigDlg::OnNM)
END_EVENT_TABLE()

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

SymTabConfigDlg::~SymTabConfigDlg()
{
  //dtor
}// ~SymTabConfigDlg

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

int SymTabConfigDlg::Execute()
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::Execute")));
#endif

  // Avoid multiple load of resources
  if (!SymTabConfigDlgLoaded)
  {
    // Instantiate and initialise dialog
    SymTabConfigDlgLoaded =
      wxXmlResource::Get()->LoadDialog(this, parent, wxT_2("dlgSymTabConfig"));
  }

  LoadSettings();
  return wxDialog::ShowModal();
}// Execute

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- -----PRIVATE----- ----- ----- ----- ----- ----- */
/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::EndModal(int retCode)
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::EndModal")));
#endif

  wxDialog::EndModal(retCode);
}// EndModal

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnWhatToDo(wxCommandEvent& event)
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnWhatToDo")));
#endif

  int choice = event.GetInt();
  ToggleWidgets(choice);
}// OnWhatToDo

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnSearch(wxCommandEvent& WXUNUSED(event))
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnSearch")));
#endif

  // user pressed Search; save settings
  SaveSettings();

  ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("symtab"));
  int choice = cfg->ReadInt(wxT_2("/what_to_do"), 0);

  // Search for a symbol given a library path
  if      (choice==0)
  {
    wxString library_path = (cfg->Read(wxT_2("/library_path"))).Trim();
    if (library_path.IsEmpty())
    {
      cbMessageBox(_("No library path provided."), _("Error"), wxICON_ERROR | wxOK,
                   (wxWindow*)Manager::Get()->GetAppWindow());
      return;
    }

    if ( !(   cfg->ReadBool(wxT_2("/include_a"),   true)
           || cfg->ReadBool(wxT_2("/include_lib"), true)
           || cfg->ReadBool(wxT_2("/include_o"),   false)
           || cfg->ReadBool(wxT_2("/include_obj"), false)
           || cfg->ReadBool(wxT_2("/include_dll"), false)) )
    {
      cbMessageBox(_("No file type (include) provided."), _("Error"), wxICON_ERROR | wxOK,
                   (wxWindow*)Manager::Get()->GetAppWindow());
      return;
    }

    wxString symbol = (cfg->Read(wxT_2("/symbol"))).Trim();
    if (symbol.IsEmpty())
    {
      wxString msg;
      msg << _("Warning: You did not select a symbol to search for in a path.\n")
          << _("You may operate on many files - this can be a lengthy operation.\n")
          << _("Are you really sure that you want to do this?");
      if (cbMessageBox(msg, _("Warning"), wxICON_WARNING | wxYES_NO,
                       (wxWindow*)Manager::Get()->GetAppWindow()) == wxID_NO)
        return;
    }
  }
  // Search for all symbols in a given library
  else if (choice==1)
  {
    wxString library = (cfg->Read(wxT_2("/library"))).Trim();
    if (library.IsEmpty())
    {
      cbMessageBox(_("No library provided."), _("Error"), wxICON_ERROR | wxOK,
                   (wxWindow*)Manager::Get()->GetAppWindow());
      return;
    }
  }

  wxDialog::EndModal(wxID_OK);
}// OnSearch

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnClose(wxCommandEvent& WXUNUSED(event))
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnClose")));
#endif

  wxDialog::EndModal(wxID_CANCEL);
}// OnClose

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnLibraryPath(wxCommandEvent& WXUNUSED(event))
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnLibraryPath")));
#endif

  wxDirDialog dd(parent, _("Select directory for search"));
  if (dd.ShowModal() == wxID_OK)
  {
    wxString path = dd.GetPath();
    XRCCTRL(*this, "txtLibraryPath", wxTextCtrl)->SetValue(path);
  }
}// OnLibraryPath

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnLibrary(wxCommandEvent& WXUNUSED(event))
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnLibrary")));
#endif

  wxString caption  = wxT_2("Choose a (library) file");
  wxString wildcard;
           wildcard << wxT_2("Library files (*.a)|*.a|")
                    << wxT_2("Library files (*.lib)|*.lib|")
                    << wxT_2("Object files (*.o)|*.o|")
                    << wxT_2("Object files (*.obj)|*.obj|")
#ifdef __WXMSW__
                    << wxT_2("Object files (*.dll)|*.dll|")
                    << wxT_2("All files (*.*)|*.*");
#else
                    << wxT_2("All files (*)|*");
#endif
  wxString es       = wxEmptyString;

  wxFileDialog fd(parent, caption, es, es, wildcard, wxFD_OPEN|compatibility::wxHideReadonly);
  if (fd.ShowModal() == wxID_OK)
  {
    wxString path = fd.GetPath();
    //int filterIndex = fd.GetFilterIndex();
    XRCCTRL(*this, "txtLibrary", wxTextCtrl)->SetValue(path);
  }
}// OnLibrary

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::OnNM(wxCommandEvent& WXUNUSED(event))
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::OnNM")));
#endif

  wxString caption  = wxT_2("Choose NM application");
  wxString wildcard;
  if (platform::windows)
    wildcard = wxT_2("All files (*.*)|*.*");
  else
    wildcard = wxT_2("All files (*)|*");

  wxString es = wxEmptyString;

  wxFileDialog fd(parent, caption, es, es, wildcard, wxFD_OPEN|compatibility::wxHideReadonly);
  if (fd.ShowModal() == wxID_OK)
  {
    wxString path = fd.GetPath();
    //int filterIndex = fd.GetFilterIndex();
    XRCCTRL(*this, "txtNM", wxTextCtrl)->SetValue(path);
  }
}// OnNM

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::ToggleWidgets(int choice)
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::ToggleWidgets")));
#endif

  // Strategy: Disable all widgets, enable required.
  XRCCTRL(*this, "txtLibraryPath", wxTextCtrl)->Enable(false);
  XRCCTRL(*this, "btnLibraryPath", wxButton)->Enable(false);
  XRCCTRL(*this, "chkIncludeA",    wxCheckBox)->Enable(false);
  XRCCTRL(*this, "chkIncludeLib",  wxCheckBox)->Enable(false);
  XRCCTRL(*this, "chkIncludeO",    wxCheckBox)->Enable(false);
  XRCCTRL(*this, "chkIncludeObj",  wxCheckBox)->Enable(false);
  XRCCTRL(*this, "chkIncludeDll",  wxCheckBox)->Enable(false);

  XRCCTRL(*this, "txtLibrary",     wxTextCtrl)->Enable(false);
  XRCCTRL(*this, "btnLibrary",     wxButton)->Enable(false);

  // Search for a symbol given a library path
  if      (choice==0)
  {
    XRCCTRL(*this, "txtLibraryPath", wxTextCtrl)->Enable(true);
    XRCCTRL(*this, "btnLibraryPath", wxButton)->Enable(true);
    XRCCTRL(*this, "chkIncludeA",    wxCheckBox)->Enable(true);
    XRCCTRL(*this, "chkIncludeLib",  wxCheckBox)->Enable(true);
    XRCCTRL(*this, "chkIncludeO",    wxCheckBox)->Enable(true);
    XRCCTRL(*this, "chkIncludeObj",  wxCheckBox)->Enable(true);
    XRCCTRL(*this, "chkIncludeDll",  wxCheckBox)->Enable(true);
  }
  // Search for all symbols in a given library
  else if (choice==1)
  {
    XRCCTRL(*this, "txtLibrary",     wxTextCtrl)->Enable(true);
    XRCCTRL(*this, "btnLibrary",     wxButton)->Enable(true);
  }
}// ToggleWidgets

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::LoadSettings()
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::LoadSettings")));
#endif

  ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("symtab"));

  // What to do options
  int choice = cfg->ReadInt(wxT_2("/what_to_do"), 0);
  XRCCTRL(*this, "choWhatToDo",     wxChoice)->SetSelection(choice);
  ToggleWidgets(choice); // Event won't fire if not changed,
                         // do manual to ensure proper initialisation
  XRCCTRL(*this, "txtLibraryPath",    wxTextCtrl)->SetValue(
    cfg->Read(wxT_2("/library_path"),    wxT_2("")));
  XRCCTRL(*this, "chkIncludeA",       wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/include_a"),   true));
  XRCCTRL(*this, "chkIncludeLib",     wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/include_lib"), true));
  XRCCTRL(*this, "chkIncludeO",       wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/include_o"),   false));
  XRCCTRL(*this, "chkIncludeObj",     wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/include_obj"), false));
  XRCCTRL(*this, "chkIncludeDll",     wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/include_dll"), false));

  XRCCTRL(*this, "txtLibrary",        wxTextCtrl)->SetValue(
    cfg->Read(wxT_2("/library"),         wxT_2("")));

  XRCCTRL(*this, "txtSymbol",         wxTextCtrl)->SetValue(
    cfg->Read(wxT_2("/symbol"),          wxT_2("")));

  // Options
  XRCCTRL(*this, "txtNM",             wxTextCtrl)->SetValue(
    cfg->Read(wxT_2("/nm"),              wxT_2("")));

  XRCCTRL(*this, "chkDebug",          wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/debug"),       false));
  XRCCTRL(*this, "chkDefined",        wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/defined"),     false));
  XRCCTRL(*this, "chkDemangle",       wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/demangle"),    false));
  XRCCTRL(*this, "chkExtern",         wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/extern"),      false));
  XRCCTRL(*this, "chkSpecial",        wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/special"),     false));
  XRCCTRL(*this, "chkSynthetic",      wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/synthetic"),   false));
  XRCCTRL(*this, "chkUndefined",      wxCheckBox)->SetValue(
    cfg->ReadBool(wxT_2("/undefined"),   false));
}// LoadSettings

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTabConfigDlg::SaveSettings()
{
#ifdef TRACE_SYMTAB_CFG
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTabConfigDlg::SaveSettings")));
#endif

  ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("symtab"));

  // What to do options
  cfg->Write(wxT_2("/what_to_do"),
    XRCCTRL(*this, "choWhatToDo",    wxChoice  )->GetSelection());

  cfg->Write(wxT_2("/library_path"),
    XRCCTRL(*this, "txtLibraryPath", wxTextCtrl)->GetValue().Trim());
  cfg->Write(wxT_2("/include_a"),
    XRCCTRL(*this, "chkIncludeA",    wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/include_lib"),
    XRCCTRL(*this, "chkIncludeLib",  wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/include_o"),
    XRCCTRL(*this, "chkIncludeO",    wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/include_obj"),
    XRCCTRL(*this, "chkIncludeObj",  wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/include_dll"),
    XRCCTRL(*this, "chkIncludeDll",  wxCheckBox)->GetValue());

  cfg->Write(wxT_2("/library"),
    XRCCTRL(*this, "txtLibrary",     wxTextCtrl)->GetValue().Trim());

  cfg->Write(wxT_2("/symbol"),
    XRCCTRL(*this, "txtSymbol",      wxTextCtrl)->GetValue().Trim());

  // Options
  cfg->Write(wxT_2("/nm"),
    XRCCTRL(*this, "txtNM",          wxTextCtrl)->GetValue().Trim());

  cfg->Write(wxT_2("/debug"),
    XRCCTRL(*this, "chkDebug",       wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/defined"),
    XRCCTRL(*this, "chkDefined",     wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/demangle"),
    XRCCTRL(*this, "chkDemangle",    wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/extern"),
    XRCCTRL(*this, "chkExtern",      wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/special"),
    XRCCTRL(*this, "chkSpecial",     wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/synthetic"),
    XRCCTRL(*this, "chkSynthetic",   wxCheckBox)->GetValue());
  cfg->Write(wxT_2("/undefined"),
    XRCCTRL(*this, "chkUndefined",   wxCheckBox)->GetValue());
}// SaveSettings
