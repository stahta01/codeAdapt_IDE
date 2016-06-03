/***************************************************************
 * Name:      codestatconfig.cpp
 * Purpose:   Code::Blocks CodeStat plugin: configuration window
 * Author:    Zlika
 * Created:   11/09/2005
 * Copyright: (c) Zlika
 * License:   GPL
 **************************************************************/
#include "sdk.h"
#ifndef CB_PRECOMP
#include <wx/combobox.h>
#include <wx/event.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>
#include "configmanager.h"
#include "globals.h"
#include "manager.h"
#endif
#include <wx/textdlg.h>
#include <wx/tokenzr.h>
#include "codestatconfig.h"

int LoadDefaultSettings(LanguageDef languages[NB_FILETYPES_MAX]);


BEGIN_EVENT_TABLE (CodeStatConfigDlg, wxPanel)
EVT_COMBOBOX(XRCID("combo_Names"), CodeStatConfigDlg::ComboBoxEvent)
EVT_BUTTON(XRCID("btn_Add"), CodeStatConfigDlg::Add)
EVT_BUTTON(XRCID("btn_Remove"), CodeStatConfigDlg::Remove)
EVT_BUTTON(XRCID("btn_Default"), CodeStatConfigDlg::RestoreDefault)
END_EVENT_TABLE ()

/** Load the language settings and display the configuration dialog.
 */
CodeStatConfigDlg::CodeStatConfigDlg(wxWindow* parent)
{
    wxXmlResource::Get()->LoadPanel(this, parent, _("dlgCodeStatConfig"));

    // Load the languages parameters
    nb_languages = LoadSettings(languages);

    ReInitDialog();
}

/** Re-init the combobox and text fields of the configuration dialog.
 */
void CodeStatConfigDlg::ReInitDialog()
{
    // Clear text fields and combobox
    wxTextCtrl* txt_FileTypes = XRCCTRL(*this, "txt_FileTypes", wxTextCtrl);
    txt_FileTypes->SetValue(wxT_2(""));
    wxTextCtrl* txt_SingleComment = XRCCTRL(*this, "txt_SingleComment", wxTextCtrl);
    txt_SingleComment->SetValue(wxT_2(""));
    wxTextCtrl* txt_MultiLineCommentBegin = XRCCTRL(*this, "txt_MultiLineCommentBegin", wxTextCtrl);
    txt_MultiLineCommentBegin->SetValue(wxT_2(""));
    wxTextCtrl* txt_MultiLineCommentEnd = XRCCTRL(*this, "txt_MultiLineCommentEnd", wxTextCtrl);
    txt_MultiLineCommentEnd->SetValue(wxT_2(""));
    wxComboBox* combo_Names = XRCCTRL(*this, "combo_Names", wxComboBox);
    combo_Names->Clear();

    // Write languages names in the combo-box
    for (int i=0; i<nb_languages; i++)
    {
       combo_Names->Append(languages[i].name);
    }

    // Select first one
    selected_language = -1;
    if (nb_languages > 0)
    {
       combo_Names->SetSelection(0);
       selected_language = 0;
       PrintLanguageInfo(0);
    }
}

CodeStatConfigDlg::~CodeStatConfigDlg()
{
}

/** Save all the languages settings.
 */
void CodeStatConfigDlg::SaveSettings()
{
    // Delete the old keys
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("codestat"));
    cfg->Delete();

    // Save current language settings
    SaveCurrentLanguage();

    // Save settings
    cfg = Manager::Get()->GetConfigManager(wxT_2("codestat"));
    cfg->Write(wxT_2("/nb_languages"), nb_languages);
    for (int i=0; i<nb_languages; ++i)
    {
        wxString extensions;
        cfg->Write(wxString::Format(wxT_2("/l%d/name"),i), languages[i].name);
        for (unsigned int j=0; j<languages[i].ext.Count(); ++j)
        {
           extensions = extensions + languages[i].ext[j] + wxT_2(" ");
        }
        cfg->Write(wxString::Format(wxT_2("/l%d/ext"),i), extensions);
        cfg->Write(wxString::Format(wxT_2("/l%d/single_line_comment"),i), languages[i].single_line_comment);
        cfg->Write(wxString::Format(wxT_2("/l%d/multiple_line_comment_begin"),i), languages[i].multiple_line_comment[0]);
        cfg->Write(wxString::Format(wxT_2("/l%d/multiple_line_comment_end"),i), languages[i].multiple_line_comment[1]);
    }
}

/** Save the current language settings and print the caracteristics for the language selected.
 */
void CodeStatConfigDlg::ComboBoxEvent(wxCommandEvent & event)
{
    SaveCurrentLanguage();
	PrintLanguageInfo(event.GetSelection());
}

/** Save the current language settings.
 */
void CodeStatConfigDlg::SaveCurrentLanguage()
{
    if (selected_language >= 0)
    {
       wxString extensions;
       extensions = XRCCTRL(*this, "txt_FileTypes", wxTextCtrl)->GetValue();
       extensions.Trim(true); extensions.Trim(false);
       languages[selected_language].ext.Clear();
       wxStringTokenizer tkz(extensions);
       while (tkz.HasMoreTokens())
       {
           languages[selected_language].ext.Add(tkz.GetNextToken());
       }
       languages[selected_language].single_line_comment = XRCCTRL(*this, "txt_SingleComment", wxTextCtrl)->GetValue();
       languages[selected_language].multiple_line_comment[0] = XRCCTRL(*this, "txt_MultiLineCommentBegin", wxTextCtrl)->GetValue();
       languages[selected_language].multiple_line_comment[1] = XRCCTRL(*this, "txt_MultiLineCommentEnd", wxTextCtrl)->GetValue();
    }
}

/** Save the current language settings and print the caracteristics for the language number "id".
 *  @param id Number of the language to display
 */
void CodeStatConfigDlg::PrintLanguageInfo(int id)
{
    selected_language = id;
	wxTextCtrl* txt_FileTypes = XRCCTRL(*this, "txt_FileTypes", wxTextCtrl);
	wxString ext_string = wxT_2("");
	for (unsigned int i=0; i<languages[id].ext.GetCount(); ++i)
	{
	   ext_string = ext_string + wxT_2(" ") + languages[id].ext[i];
	}
	txt_FileTypes->SetValue(ext_string);
	wxTextCtrl* txt_SingleComment = XRCCTRL(*this, "txt_SingleComment", wxTextCtrl);
	txt_SingleComment->SetValue(languages[id].single_line_comment);
	wxTextCtrl* txt_MultiLineCommentBegin = XRCCTRL(*this, "txt_MultiLineCommentBegin", wxTextCtrl);
	txt_MultiLineCommentBegin->SetValue(languages[id].multiple_line_comment[0]);
	wxTextCtrl* txt_MultiLineCommentEnd = XRCCTRL(*this, "txt_MultiLineCommentEnd", wxTextCtrl);
	txt_MultiLineCommentEnd->SetValue(languages[id].multiple_line_comment[1]);
}

/** Add configuration for a new language.
 */
void CodeStatConfigDlg::Add(wxCommandEvent& event)
{
   wxTextEntryDialog dialog(this, _("Enter name of the new language:"), _("New language"), wxT_2(""), wxOK|wxCANCEL);
   if (dialog.ShowModal() == wxID_OK)
   {
       if (nb_languages < NB_FILETYPES_MAX)
       {
           wxString name = dialog.GetValue();
           name.Trim(true); name.Trim(false);
           if (!name.IsEmpty())
           {
               languages[nb_languages].name = name;
               languages[nb_languages].ext.Clear();
               languages[nb_languages].single_line_comment = wxT_2("");
               languages[nb_languages].multiple_line_comment[0] = wxT_2("");
               languages[nb_languages].multiple_line_comment[1] = wxT_2("");
               nb_languages++;
               wxComboBox* combo_Names = XRCCTRL(*this, "combo_Names", wxComboBox);
               combo_Names->Append(name);
               combo_Names->SetSelection(nb_languages-1);
               PrintLanguageInfo(nb_languages-1);
           }
       }
       else cbMessageBox(_("Language list is full!"), _("Error"), wxOK, Manager::Get()->GetAppWindow());
   }
}

/** Remove the selected language from the list.
 */
void CodeStatConfigDlg::Remove(wxCommandEvent& event)
{
    if (nb_languages > 0)
    {
       for (int i=selected_language; i<nb_languages-1; ++i)
       {
          languages[i].name = languages[i+1].name;
          languages[i].ext = languages[i+1].ext;
          languages[i].single_line_comment = languages[i+1].single_line_comment;
          languages[i].multiple_line_comment[0] = languages[i+1].multiple_line_comment[0];
          languages[i].multiple_line_comment[1] = languages[i+1].multiple_line_comment[1];
       }
       --nb_languages;
       ReInitDialog();
    }
}

/** Restore the default settings.
 */
void CodeStatConfigDlg::RestoreDefault(wxCommandEvent& event)
{
    nb_languages = LoadDefaultSettings(languages);
    ReInitDialog();
}

/** Load the default languages settings.
 */
int LoadDefaultSettings(LanguageDef languages[NB_FILETYPES_MAX])
{
    int nb_languages = 7;
    // C/C++ style comments
    languages[0].name = wxT_2("C/C++");
    languages[0].ext.Clear();
    languages[0].ext.Add(wxT_2("c"));
    languages[0].ext.Add(wxT_2("cpp"));
    languages[0].ext.Add(wxT_2("h"));
    languages[0].ext.Add(wxT_2("hpp"));
    languages[0].single_line_comment = wxT_2("//");
    languages[0].multiple_line_comment[0] = wxT_2("/*");
    languages[0].multiple_line_comment[1] = wxT_2("*/");

    // Java style comments
    languages[1].name = wxT_2("Java");
    languages[1].ext.Clear();
    languages[1].ext.Add(wxT_2("java"));
    languages[1].single_line_comment = wxT_2("//");
    languages[1].multiple_line_comment[0] = wxT_2("/*");
    languages[1].multiple_line_comment[1] = wxT_2("*/");

    // Python style comments
    languages[2].name = wxT_2("Python");
    languages[2].ext.Clear();
    languages[2].ext.Add(wxT_2("py"));
    languages[2].single_line_comment = wxT_2("#");
    languages[2].multiple_line_comment[0] = wxT_2("");
    languages[2].multiple_line_comment[1] = wxT_2("");

    // Perl style comments
    languages[3].name = wxT_2("Perl");
    languages[3].ext.Clear();
    languages[3].ext.Add(wxT_2("pl"));
    languages[3].single_line_comment = wxT_2("#");
    languages[3].multiple_line_comment[0] = wxT_2("");
    languages[3].multiple_line_comment[1] = wxT_2("");

    // ASM style comments
    languages[4].name = wxT_2("ASM");
    languages[4].ext.Clear();
    languages[4].ext.Add(wxT_2("asm"));
    languages[4].single_line_comment = wxT_2(";");
    languages[4].multiple_line_comment[0] = wxT_2("");
    languages[4].multiple_line_comment[1] = wxT_2("");

    // Pascal style comments
    languages[5].name = wxT_2("Pascal");
    languages[5].ext.Clear();
    languages[5].ext.Add(wxT_2("pas"));
    languages[5].single_line_comment = wxT_2("");
    languages[5].multiple_line_comment[0] = wxT_2("{");
    languages[5].multiple_line_comment[1] = wxT_2("}");

    // Matlab style comments
    languages[6].name = wxT_2("Matlab");
    languages[6].ext.Clear();
    languages[6].ext.Add(wxT_2("m"));
    languages[6].single_line_comment = wxT_2("%");
    languages[6].multiple_line_comment[0] = wxT_2("");
    languages[6].multiple_line_comment[1] = wxT_2("");

    return nb_languages;
}

/** Load the definition of the comments for each language.
 *  @param languages Array of languages.
 *  @see LanguageDef
 */
int LoadSettings(LanguageDef languages[NB_FILETYPES_MAX])
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("codestat"));
    int nb_languages = cfg->ReadInt(wxT_2("/nb_languages"), 0);

    // If no comment styles exist, use and save default ones
    if (nb_languages == 0)
    {
       nb_languages = LoadDefaultSettings(languages);
    }
    // else, load the user settings
    else
    {
        if (nb_languages > NB_FILETYPES_MAX)
           nb_languages = NB_FILETYPES_MAX;
        for (int i=0; i<nb_languages; ++i)
        {
            wxString extensions;
            languages[i].name = cfg->Read(wxString::Format(wxT_2("/l%d/name"),i), wxT_2(""));
            extensions = cfg->Read(wxString::Format(wxT_2("/l%d/ext"),i), wxT_2(""));
            languages[i].ext.Clear();
            wxStringTokenizer tkz(extensions);
            while (tkz.HasMoreTokens())
               languages[i].ext.Add(tkz.GetNextToken());
            languages[i].single_line_comment = cfg->Read(wxString::Format(wxT_2("/l%d/single_line_comment"),i), wxT_2(""));
            languages[i].multiple_line_comment[0] = cfg->Read(wxString::Format(wxT_2("/l%d/multiple_line_comment_begin"),i), wxT_2(""));
            languages[i].multiple_line_comment[1] = cfg->Read(wxString::Format(wxT_2("/l%d/multiple_line_comment_end"),i), wxT_2(""));
        }
    }
    return nb_languages;
}
