/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include <wx/button.h>
    #include <wx/checkbox.h>
    #include <wx/filename.h>
    #include <wx/intl.h>
    #include <wx/listctrl.h>
    #include <wx/string.h>
    #include <wx/utils.h>
    #include <wx/xrc/xmlres.h>
    #include "manager.h"
    #include "configmanager.h"
    #include "pluginmanager.h"
    #include "cbplugin.h" // IsAttached
#endif

#include "annoyingdialog.h"
#include "prep.h"
#include <wx/settings.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/progdlg.h>
#include <wx/html/htmlwin.h>

#include "pluginsconfigurationdlg.h" // class's header file

int wxCALLBACK sortByTitle(long item1, long item2, long sortData)
{
    const PluginElement* elem1 = (const PluginElement*)item1;
    const PluginElement* elem2 = (const PluginElement*)item2;

    return elem1->info.title.CompareTo(elem2->info.title);
}

BEGIN_EVENT_TABLE(PluginsConfigurationDlg, wxDialog)
    EVT_BUTTON(XRCID("btnEnable"), PluginsConfigurationDlg::OnToggle)
    EVT_BUTTON(XRCID("btnDisable"), PluginsConfigurationDlg::OnToggle)
    EVT_BUTTON(XRCID("btnInstall"), PluginsConfigurationDlg::OnInstall)
    EVT_BUTTON(XRCID("btnUninstall"), PluginsConfigurationDlg::OnUninstall)
    EVT_BUTTON(XRCID("btnExport"), PluginsConfigurationDlg::OnExport)
    EVT_LIST_ITEM_SELECTED(XRCID("lstPlugins"), PluginsConfigurationDlg::OnSelect)

    EVT_UPDATE_UI(-1, PluginsConfigurationDlg::OnUpdateUI)
END_EVENT_TABLE()

// class constructor
PluginsConfigurationDlg::PluginsConfigurationDlg(wxWindow* parent)
{
	wxXmlResource::Get()->LoadDialog(this, parent, wxT_2("dlgConfigurePlugins"));
    FillList();

    // install options
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("plugins"));
    bool globalInstall = cfg->ReadBool(wxT_2("/install_globally"), true);
    bool confirmation = cfg->ReadBool(wxT_2("/install_confirmation"), true);

    // verify user can install globally
    DirAccessCheck access = cbDirAccessCheck(ConfigManager::GetFolder(sdPluginsGlobal));
    if (access != dacReadWrite)
    {
        globalInstall = false;
        // disable checkbox
        XRCCTRL(*this, "chkInstallGlobally", wxCheckBox)->Enable(false);
    }
    XRCCTRL(*this, "chkInstallGlobally", wxCheckBox)->SetValue(globalInstall);
    XRCCTRL(*this, "chkInstallConfirmation", wxCheckBox)->SetValue(confirmation);

    // Set default font size based on system default font size
#ifdef __linux__
    /* NOTE (mandrav#1#): wxWidgets documentation on wxHtmlWindow::SetFonts(),
    states that the sizes array accepts values from -2 to +4.
    My tests (under linux at least) have showed that it actually
    expects real point sizes. */

	wxFont systemFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	int sizes[7] = {};
	for (int i = 0; i < 7; ++i)
        sizes[i] = systemFont.GetPointSize();
	XRCCTRL(*this, "htmlInfo", wxHtmlWindow)->SetFonts(wxEmptyString, wxEmptyString, &sizes[0]);
#endif

    wxString initialInfo;
    initialInfo << wxT_2("<html><body><font color=\"#0000AA\">");
    initialInfo << _("Tip: The above list allows for multiple selections.");
    initialInfo << wxT_2("</font><br /><br /><b><font color=\"red\">");
    initialInfo << _("Have you saved your work first?");
    initialInfo << wxT_2("</font></b><br /><i><font color=\"black\">\n");
    initialInfo << _("If a plugin is not well-written, it could cause Code::Blocks to crash ");
    initialInfo << wxT_2("when performing any operation on it...");

	if (PluginManager::GetSafeMode())
	{
		initialInfo << wxT_2("</font></i><br /><br /><b><font color=\"red\">");
		initialInfo << _("Code::Blocks started up in \"safe-mode\"");
		initialInfo << wxT_2("</font></b><br /><i><font color=\"black\">\n");
		initialInfo << _("All plugins were disabled on startup so that you can troubleshoot ");
		initialInfo << wxT_2("problematic plugins. Enable plugins at will now...");
	}

    initialInfo << wxT_2("</font></i><br /></body></html>\n");

    XRCCTRL(*this, "htmlInfo", wxHtmlWindow)->SetPage(initialInfo);
}

void PluginsConfigurationDlg::FillList()
{
    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    if (list->GetColumnCount() == 0)
    {
        list->InsertColumn(0, wxT_2("Title"));
        list->InsertColumn(1, wxT_2("Version"));
        list->InsertColumn(2, wxT_2("Enabled"), wxLIST_FORMAT_CENTER);
        list->InsertColumn(3, wxT_2("Filename"));
    }

    PluginManager* man = Manager::Get()->GetPluginManager();
    const PluginElementsArray& plugins = man->GetPlugins();

    // populate Plugins checklist
    list->DeleteAllItems();
    for (unsigned int i = 0; i < plugins.GetCount(); ++i)
    {
        const PluginElement* elem = plugins[i];

        long idx = list->InsertItem(i, elem->info.title);
        list->SetItem(idx, 1, elem->info.version);
        list->SetItem(idx, 2, elem->plugin->IsAttached() ? _("Yes") : _("No"));
        list->SetItem(idx, 3, UnixFilename(elem->fileName));
        list->SetItemData(idx, (long)elem);

        if (!elem->plugin->IsAttached())
            list->SetItemTextColour(idx, wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
        else
            list->SetItemTextColour(idx, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    }

    list->SetColumnWidth(0, wxLIST_AUTOSIZE);
    list->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
    list->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);
    list->SetColumnWidth(3, wxLIST_AUTOSIZE);

    list->SortItems(sortByTitle, 0);
}

// class destructor
PluginsConfigurationDlg::~PluginsConfigurationDlg()
{
	// insert your code here
}

void PluginsConfigurationDlg::OnToggle(wxCommandEvent& event)
{
    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    if (list->GetSelectedItemCount() == 0)
        return;
    bool isEnable = event.GetId() == XRCID("btnEnable");

    wxBusyCursor busy;

    wxProgressDialog pd(wxString::Format(_("%s plugin(s)"), isEnable ? _("Enabling") : _("Disabling")),
                        wxT_2("A description wide enough for the dialog ;)"),
                        list->GetSelectedItemCount(),
                        this,
                        wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT);

    int count = 0;
    long sel = -1;
    bool skip = false;
    while (true)
    {
        sel = list->GetNextItem(sel, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (sel == -1)
            break;

        const PluginElement* elem = (const PluginElement*)list->GetItemData(sel);
        if (elem && elem->plugin)
        {
            pd.Update(++count,
                        wxString::Format(_("%s \"%s\"..."), isEnable ? _("Enabling") : _("Disabling"), elem->info.title.c_str()),
                        &skip);
            if (skip)
                break;

            if (!isEnable && elem->plugin->IsAttached())
                Manager::Get()->GetPluginManager()->DetachPlugin(elem->plugin);
            else if (isEnable && !elem->plugin->IsAttached())
                Manager::Get()->GetPluginManager()->AttachPlugin(elem->plugin, true); // ignore safe-mode here
            else
                continue;

            wxListItem item;
            item.SetId(sel);
            item.SetColumn(2);
            item.SetMask(wxLIST_MASK_TEXT);
            list->GetItem(item);

            list->SetItem(sel, 2, elem->plugin->IsAttached() ? _("Yes") : _("No"));
            if (!elem->plugin->IsAttached())
                list->SetItemTextColour(sel, wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
            else
                list->SetItemTextColour(sel, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));

            // update configuration
            wxString baseKey;
            baseKey << wxT_2("/") << elem->info.name;
            Manager::Get()->GetConfigManager(wxT_2("plugins"))->Write(baseKey, elem->plugin->IsAttached());
        }
    }
}

void PluginsConfigurationDlg::OnInstall(wxCommandEvent& event)
{
    wxFileDialog fd(this,
                        _("Select plugin to install"),
                        wxEmptyString, wxEmptyString,
                        wxT_2("Code::Blocks Plugins (*.cbplugin)|*.cbplugin"),
                        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE | compatibility::wxHideReadonly);
    if (fd.ShowModal() != wxID_OK)
        return;

    wxBusyCursor busy;

    wxArrayString paths;
    fd.GetPaths(paths);

    // install in global or user dirs?
    bool globalInstall = XRCCTRL(*this, "chkInstallGlobally", wxCheckBox)->GetValue();
    bool confirm = XRCCTRL(*this, "chkInstallConfirmation", wxCheckBox)->GetValue();

    wxString failure;
    for (size_t i = 0; i < paths.GetCount(); ++i)
    {
        if (!Manager::Get()->GetPluginManager()->InstallPlugin(paths[i], globalInstall, confirm))
            failure << paths[i] << wxT_2('\n');
    }

    FillList();
    if (!failure.IsEmpty())
        cbMessageBox(_("One or more plugins were not installed succesfully:\n\n") + failure, _("Warning"), wxICON_WARNING);
}

void PluginsConfigurationDlg::OnUninstall(wxCommandEvent& event)
{
    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    if (list->GetSelectedItemCount() == 0)
        return;

    wxBusyCursor busy;

    long sel = -1;
    wxString failure;
    while (true)
    {
        sel = list->GetNextItem(sel, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (sel == -1)
            break;

        const PluginElement* elem = (const PluginElement*)list->GetItemData(sel);
        if (elem && elem->plugin)
        {
            if (!Manager::Get()->GetPluginManager()->UninstallPlugin(elem->plugin))
                failure << elem->info.title << wxT_2('\n');
        }
    }

    FillList();
    if (!failure.IsEmpty())
        cbMessageBox(_("One or more plugins were not uninstalled succesfully:\n\n") + failure, _("Warning"), wxICON_WARNING);
}

void PluginsConfigurationDlg::OnExport(wxCommandEvent& event)
{
    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    if (list->GetSelectedItemCount() == 0)
        return;

    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("plugins_configuration"));
    wxDirDialog dd(this, _("Select directory to export plugin"), cfg->Read(wxT_2("/last_export_path")), wxDD_NEW_DIR_BUTTON);
    if (dd.ShowModal() != wxID_OK)
        return;
    cfg->Write(wxT_2("/last_export_path"), dd.GetPath());

    wxBusyCursor busy;
    wxProgressDialog pd(_("Exporting plugin(s)"),
                        wxT_2("A description wide enough for the dialog ;)"),
                        list->GetSelectedItemCount(),
                        this,
                        wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT |
                        wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);

    int count = 0;
    long sel = -1;
    bool skip = false;
    bool confirmed = false;
    wxString failure;
    wxArrayString files; // avoid exporting different plugins from the same file twice
    while (true)
    {
        sel = list->GetNextItem(sel, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (sel == -1)
            break;

        const PluginElement* elem = (const PluginElement*)list->GetItemData(sel);
        if (!elem && !elem->plugin)
        {
            failure << list->GetItemText(sel) << wxT_2('\n');
            continue;
        }

        // avoid duplicates
        if (files.Index(elem->fileName) != wxNOT_FOUND)
            continue;
        files.Add(elem->fileName);

        // normalize version
        wxString version = elem->info.version;
        version.Replace(wxT_2("/"), wxT_2("_"), true);
        version.Replace(wxT_2("\\"), wxT_2("_"), true);
        version.Replace(wxT_2("?"), wxT_2("_"), true);
        version.Replace(wxT_2("*"), wxT_2("_"), true);
        version.Replace(wxT_2(">"), wxT_2("_"), true);
        version.Replace(wxT_2("<"), wxT_2("_"), true);
        version.Replace(wxT_2(" "), wxT_2("_"), true);
        version.Replace(wxT_2("\t"), wxT_2("_"), true);
        version.Replace(wxT_2("|"), wxT_2("_"), true);

        wxFileName fname;
        fname.SetPath(dd.GetPath());
        fname.SetName(wxFileName(elem->fileName).GetName() + wxT_2('-') + version);
        fname.SetExt(wxT_2("cbplugin"));

        pd.Update(++count,
                    wxString::Format(_("Exporting \"%s\"..."), elem->info.title.c_str()),
                    &skip);
        if (skip)
            break;

        wxString filename = fname.GetFullPath();

        if (!confirmed && wxFileExists(filename))
        {
            AnnoyingDialog dlg(_("Confirmation"),
                                wxString::Format(_("%s already exists.\n"
                                "Are you sure you want to overwrite it?"), filename.c_str()),
                                wxART_QUESTION,
                                AnnoyingDialog::THREE_BUTTONS,
                                1,
                                true,
                                _("&Yes"), _("Yes to &all"), _("&No"));
            switch (dlg.ShowModal())
            {
                case 3:
                    continue;
                    break;

                case 2:
                    confirmed = true;
                    break;

                default:
                    break;
            }
        }

        if (!Manager::Get()->GetPluginManager()->ExportPlugin(elem->plugin, filename))
            failure << list->GetItemText(sel) << wxT_2('\n');
    }

    if (!failure.IsEmpty())
        cbMessageBox(_("Failed exporting one or more plugins:\n\n") + failure, _("Warning"), wxICON_WARNING);
}

void PluginsConfigurationDlg::OnSelect(wxListEvent& event)
{
    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    if (list->GetSelectedItemCount() != 1)
        return;

    long sel = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    const PluginElement* elem = (const PluginElement*)list->GetItemData(sel);
    if (!elem)
        return;

    wxString description(elem->info.description);
    description.Replace(wxT_2("\n"), wxT_2("<br />\n"));

    wxString info;
    info << wxT_2("<html><body>\n");
    info << wxT_2("<h3>") << elem->info.title << wxT_2(" ");
    info << wxT_2("<font color=\"#0000AA\">") << elem->info.version << wxT_2("</font></h3>");
    info << wxT_2("<i><font color=\"#808080\" size=\"-1\">") << UnixFilename(elem->fileName) << wxT_2("</font></i><br />\n");
    info << wxT_2("<br />\n");
    info << description << wxT_2("<br />\n");
    info << wxT_2("</body></html>\n");

    XRCCTRL(*this, "htmlInfo", wxHtmlWindow)->SetPage(info);
}

void PluginsConfigurationDlg::OnUpdateUI(wxUpdateUIEvent& event)
{
    static bool lastSelection = -2;
    static bool lastSelectionMultiple = false;
    event.Skip();

    wxListCtrl* list = XRCCTRL(*this, "lstPlugins", wxListCtrl);
    long sel = list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    // no need to overdraw all the time
    if (sel == lastSelection && (lastSelectionMultiple && list->GetSelectedItemCount() > 1))
        return;
    lastSelection = sel;
    lastSelectionMultiple = list->GetSelectedItemCount() > 1;

    bool en = sel != -1;
    const PluginElement* elem = en ? (const PluginElement*)list->GetItemData(sel) : 0;
    bool hasPlugin = elem && elem->plugin;
    bool isAttached = hasPlugin && elem->plugin->IsAttached();

    XRCCTRL(*this, "btnEnable", wxButton)->Enable(en && (lastSelectionMultiple || (hasPlugin && !isAttached)));
    XRCCTRL(*this, "btnDisable", wxButton)->Enable(en && (lastSelectionMultiple || (hasPlugin && isAttached)));
    XRCCTRL(*this, "btnUninstall", wxButton)->Enable(en);
    XRCCTRL(*this, "btnExport", wxButton)->Enable(en);
}

void PluginsConfigurationDlg::EndModal(int retCode)
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("plugins"));

    cfg->Write(wxT_2("/install_globally"), XRCCTRL(*this, "chkInstallGlobally", wxCheckBox)->GetValue());
    cfg->Write(wxT_2("/install_confirmation"), XRCCTRL(*this, "chkInstallConfirmation", wxCheckBox)->GetValue());

    wxDialog::EndModal(retCode);
}
