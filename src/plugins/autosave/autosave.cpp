/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id $
* $HeadURL$
*/

#include "sdk.h"
#ifndef CB_PRECOMP
    #include <wx/checkbox.h>
    #include <wx/choice.h>
    #include <wx/filefn.h>
    #include <wx/filename.h>
    #include <wx/textctrl.h>
    #include <wx/timer.h>
    #include <wx/xrc/xmlres.h>
    #include "cbeditor.h"
    #include "cbproject.h"
    #include "configmanager.h"
    #include "editormanager.h"
    #include "globals.h"
    #include "pluginmanager.h"
    #include "projectmanager.h"
    #include "manager.h"
    #include "sdk_events.h"
#endif
#include "cbstyledtextctrl.h"

#include "projectloader.h"
#include "autosave.h"

// this auto-registers the plugin
namespace
{
    PluginRegistrant<Autosave> reg(wxT_2("Autosave"));
}

BEGIN_EVENT_TABLE(Autosave, cbPlugin)
EVT_TIMER(-1, Autosave::OnTimer)
END_EVENT_TABLE()

Autosave::Autosave()
{
    //ctor
}

Autosave::~Autosave()
{
}

void Autosave::OnAttach()
{
    if(!Manager::LoadResource(wxT_2("autosave.zip")))
    {
        NotifyMissingFile(wxT_2("autosave.zip"));
    }

    timer1 = new wxTimer(this, 10000);
    timer2 = new wxTimer(this, 20000);

    Start();
}

void Autosave::Start()
{
    ConfigManager *cfg = Manager::Get()->GetConfigManager(wxT_2("autosave"));
    if(cfg->ReadBool(wxT_2("do_project")))
        timer1->Start(60 * 1000 * cfg->ReadInt(wxT_2("project_mins")));
    else
        timer1->Stop();

    if(cfg->ReadBool(wxT_2("do_sources")))
        timer2->Start(60 * 1000 * cfg->ReadInt(wxT_2("source_mins")));
    else
        timer2->Stop();
}

void Autosave::OnRelease(bool appShutDown)
{
    delete timer1;
    delete timer2;
    timer1 = 0;
    timer2 = 0;
}

void Autosave::OnTimer(wxTimerEvent& e)
{
    if(e.GetId() == 10000)
    {
        PluginManager *plm = Manager::Get()->GetPluginManager();
        int method = Manager::Get()->GetConfigManager(wxT_2("autosave"))->ReadInt(wxT_2("method"));
        ProjectManager *pm = Manager::Get()->GetProjectManager();
        if(pm && pm->GetActiveProject())
        {
            if(cbProject * p = pm->GetActiveProject())
            {
                switch(method)
                {
                    case 0:
                    {
                        if(p->GetModified())
                        {
                            if(::wxRenameFile(p->GetFilename(), p->GetFilename() + wxT_2(".bak")))
                                if(p->Save())
                                {
                                    CodeBlocksEvent e(cbEVT_PROJECT_SAVE);
                                    plm->NotifyPlugins(e);
                                }
                        }
                        wxFileName file = p->GetFilename();
                        file.SetExt(wxT_2("layout"));
                        wxString filename = file.GetFullPath();
                        if(::wxRenameFile(filename, filename + wxT_2(".bak")))
                            p->SaveLayout();
                        break;
                    }
                    case 1:
                    {
                        if(p->GetModified() && p->Save())
                        {
                            CodeBlocksEvent e(cbEVT_PROJECT_SAVE);
                            plm->NotifyPlugins(e);
                        }
                        p->SaveLayout();
                        break;
                    }
                    case 2:
                    {
                        if (p->IsLoaded() == false)
                            return;
                        if(p->GetModified())
                        {
                            ProjectLoader loader(p);
                            if(loader.Save(p->GetFilename() + wxT_2(".save")))
                            {
                                CodeBlocksEvent e(cbEVT_PROJECT_SAVE);
                                plm->NotifyPlugins(e);
                            }
                            p->SetModified(); // the actual project file is still not updated!
                        }
                        wxFileName file = wxFileName(p->GetFilename());
                        file.SetExt(wxT_2("layout"));
                        wxString filename = file.GetFullPath();
                        wxString temp = filename + wxT_2(".temp");
                        wxString save = filename + wxT_2(".save");
                        if(::wxFileExists(filename) && ::wxCopyFile(filename, temp))
                        {
                            p->SaveLayout();
                            ::wxRenameFile(filename, save);
                            ::wxRenameFile(temp, filename);
                        }
                        break;
                    }
                }
            }
        }
    }
    else if(e.GetId() == 20000)
    {
        int method = Manager::Get()->GetConfigManager(wxT_2("autosave"))->ReadInt(wxT_2("method"));
        EditorManager* em = Manager::Get()->GetEditorManager();

        if(em)
        {
            for(int i = 0; i < em->GetEditorsCount(); ++i)
            {
                cbEditor* ed = em->GetBuiltinEditor(em->GetEditor(i));
                if(ed && ed->GetModified())
                {
                    wxFileName fn(ed->GetFilename());
                    switch(method)
                    {
                        case 0:
                        {
                            if(::wxRenameFile(fn.GetFullPath(), fn.GetFullPath() + wxT_2(".bak")))
                                cbSaveToFile(fn.GetFullPath(), ed->GetControl()->GetText(), ed->GetEncoding(), ed->GetUseBom());
                            break;
                        }
                        case 1:
                        {
                            cbSaveToFile(fn.GetFullPath(), ed->GetControl()->GetText(), ed->GetEncoding(), ed->GetUseBom());
                            break;
                        }
                        case 2:
                        {
                            cbSaveToFile(fn.GetFullPath() + wxT_2(".save"), ed->GetControl()->GetText(), ed->GetEncoding(), ed->GetUseBom());
                            ed->SetModified(); // the "real" file has not been saved!
                            break;
                        }
                    }
                }

            }
        }
    }

}

cbConfigurationPanel* Autosave::GetConfigurationPanel(wxWindow* parent)
{
    AutosaveConfigDlg* dlg = new AutosaveConfigDlg(parent, this);
    // deleted by the caller

    return dlg;
}


int Autosave::Configure()
{
    return 0;
}



AutosaveConfigDlg::AutosaveConfigDlg(wxWindow* parent, Autosave* plug) : plugin(plug)
{
    wxXmlResource::Get()->LoadPanel(this, parent, wxT_2("dlgAutosave"));

    LoadSettings();
}

void AutosaveConfigDlg::LoadSettings()
{
    ConfigManager *cfg = Manager::Get()->GetConfigManager(wxT_2("autosave"));

    XRCCTRL(*this, "do_project", wxCheckBox)->SetValue(cfg->ReadBool(wxT_2("do_project")));
    XRCCTRL(*this, "do_sources", wxCheckBox)->SetValue(cfg->ReadBool(wxT_2("do_sources")));
    XRCCTRL(*this, "project_mins", wxTextCtrl)->SetValue(wxString::Format(wxT_2("%d"), cfg->ReadInt(wxT_2("project_mins"))));
    XRCCTRL(*this, "source_mins", wxTextCtrl)->SetValue(wxString::Format(wxT_2("%d"), cfg->ReadInt(wxT_2("source_mins"))));

    XRCCTRL(*this, "method", wxChoice)->SetSelection(cfg->ReadInt(wxT_2("method"), 2));
}

void AutosaveConfigDlg::SaveSettings()
{
    ConfigManager *cfg = Manager::Get()->GetConfigManager(wxT_2("autosave"));

    cfg->Write(wxT_2("do_project"), (bool) XRCCTRL(*this, "do_project", wxCheckBox)->GetValue());
    cfg->Write(wxT_2("do_sources"), (bool) XRCCTRL(*this, "do_sources", wxCheckBox)->GetValue());

    long pm, sm;

    XRCCTRL(*this, "project_mins", wxTextCtrl)->GetValue().ToLong(&pm);
    XRCCTRL(*this, "source_mins", wxTextCtrl)->GetValue().ToLong(&sm);

    cfg->Write(wxT_2("project_mins"), (int) pm);
    cfg->Write(wxT_2("source_mins"), (int) sm);

    cfg->Write(wxT_2("method"), XRCCTRL(*this, "method", wxChoice)->GetSelection());

    plugin->Start();
}






