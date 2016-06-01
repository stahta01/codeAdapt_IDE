/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is distributed under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include <sdk.h>
#ifndef CB_PRECOMP
    #include <wx/radiobox.h>
    #include <wx/xrc/xmlres.h>
#endif
#include "associations.h"
#include "appglobals.h"
#include <manager.h>
#include <configmanager.h>
#include <filefilters.h>
#include <wx/checklst.h>

const Associations::Assoc knownTypes[] =
{
/*
    { Extension (wxString), Description (wxString), IconIndex (int) }
      Note: "index" is the index of the icon resource in "resources.rc"
            Keep all indices in sync with icon indices in "resources.rc"!
*/
    { FileFilters::CODEBLOCKS_EXT,      wxT_2("project file"),                  1 },
    { FileFilters::WORKSPACE_EXT,       wxT_2("workspace file"),               11 },

    { FileFilters::C_EXT,               wxT_2("C source file"),                 3 },

    { FileFilters::CC_EXT,              wxT_2("C++ source file"),               4 },
    { FileFilters::CPP_EXT,             wxT_2("C++ source file"),               4 },
    { FileFilters::CXX_EXT,             wxT_2("C++ source file"),               4 },

    { FileFilters::H_EXT,               wxT_2("Header file"),                   5 },
    { FileFilters::HH_EXT,              wxT_2("Header file"),                   5 },
    { FileFilters::HPP_EXT,             wxT_2("Header file"),                   5 },
    { FileFilters::HXX_EXT,             wxT_2("Header file"),                   5 },

    { FileFilters::JAVA_EXT,            wxT_2("Java source file"),              6 },
    { wxT_2("cg"),                         wxT_2("cg source file"),                7 },
    { FileFilters::D_EXT,               wxT_2("D source file"),                 8 },
    { FileFilters::RESOURCE_EXT,        wxT_2("resource file"),                10 },
    { FileFilters::XRCRESOURCE_EXT,     wxT_2("XRC resource file"),            10 },

    { FileFilters::ASM_EXT,             wxT_2("ASM source file"),               2 },
    { FileFilters::S_EXT,               wxT_2("ASM source file"),               2 },
    { FileFilters::SS_EXT,              wxT_2("ASM source file"),               2 },
    { FileFilters::S62_EXT,             wxT_2("ASM source file"),               2 },

    { FileFilters::F_EXT,               wxT_2("Fortran source file"),           9 },
    { FileFilters::F77_EXT,             wxT_2("Fortran source file"),           9 },
    { FileFilters::F90_EXT,             wxT_2("Fortran source file"),           9 },
    { FileFilters::F95_EXT,             wxT_2("Fortran source file"),           9 },

    { FileFilters::DEVCPP_EXT,          wxT_2("Dev-CPP project file"),         21 },
    { FileFilters::MSVC6_EXT,           wxT_2("MS Visual C++ project file"),   22 },
    { FileFilters::MSVC6_WORKSPACE_EXT, wxT_2("MS Visual C++ workspace file"), 23 }
    //{ wxT_2("proj"),                       wxT_2("XCODE Project file"),           24 }
};

inline void DoSetAssociation(const wxString& executable, int index)
{
    Associations::DoSetAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

inline bool DoCheckAssociation(const wxString& executable, int index)
{
    return Associations::DoCheckAssociation(knownTypes[index].ext, knownTypes[index].descr, executable, knownTypes[index].index);
};

unsigned int Associations::CountAssocs()
{
    return sizeof(knownTypes)/sizeof(Associations::Assoc);
}

void Associations::SetBatchBuildOnly()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    ::DoSetAssociation(name, 0);
    ::DoSetAssociation(name, 1);

    UpdateChanges();
}

void Associations::UpdateChanges()
{
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0L, 0L);
}

void Associations::SetCore()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(int i = 0; i <= 12; ++i)        // beware, the number 12 is hardcoded ;)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::SetAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(unsigned int i = 0; i < CountAssocs(); ++i)
        ::DoSetAssociation(name, i);

    UpdateChanges();
}

void Associations::ClearAll()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(unsigned int i = 0; i < CountAssocs(); ++i)
    {
        DoClearAssociation(knownTypes[i].ext);
    };

    UpdateChanges();
}

bool Associations::Check()
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    bool result = true;

    for(int i = 0; i <= 12; ++i)        // beware, the number 12 is hardcoded ;)
        result &= ::DoCheckAssociation(name, i);

    return  result;
}

void Associations::DoSetAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(wxT_2("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = wxT_2("HKEY_CLASSES_ROOT\\");

    wxString node(wxT_2("CodeBlocks.") + ext);

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + wxT_2(".") + ext);
    key.Create();
    key = wxT_2("CodeBlocks.") + ext;

    key.SetName(BaseKeyName + node);
    key.Create();
    key = descr;

    key.SetName(BaseKeyName + node + wxT_2("\\DefaultIcon"));
    key.Create();
    key = exe + wxString::Format(wxT_2(",%d"), icoNum);

    key.SetName(BaseKeyName + node + wxT_2("\\shell\\open\\command"));
    key.Create();
    key = wxT_2("\"") + exe + wxT_2("\" \"%1\"");

    key.SetName(BaseKeyName + node + wxT_2("\\shell\\open\\ddeexec"));
    key.Create();
    key = wxT_2("[Open(\"%1\")]");

    key.SetName(BaseKeyName + node + wxT_2("\\shell\\open\\ddeexec\\application"));
    key.Create();
    key = DDE_SERVICE;

    key.SetName(BaseKeyName + node + wxT_2("\\shell\\open\\ddeexec\\ifexec"));
    key.Create();
    key = wxT_2("[IfExec_Open(\"%1\")]");;

    key.SetName(BaseKeyName + node + wxT_2("\\shell\\open\\ddeexec\\topic"));
    key.Create();
    key = DDE_TOPIC;

    if(ext.IsSameAs(FileFilters::CODEBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(wxT_2("app"))->Read(wxT_2("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + node + wxT_2("\\shell\\Build\\command"));
        key.Create();
        key = wxT_2("\"") + exe + wxT_2("\" ") + batchbuildargs + wxT_2(" --build \"%1\"");

        key.SetName(BaseKeyName + node + wxT_2("\\shell\\Rebuild (clean)\\command"));
        key.Create();
        key = wxT_2("\"") + exe + wxT_2("\" ") + batchbuildargs + wxT_2(" --rebuild \"%1\"");
    }
}

void Associations::DoClearAssociation(const wxString& ext)
{
    wxString BaseKeyName(wxT_2("HKEY_CURRENT_USER\\Software\\Classes\\"));
    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = wxT_2("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + wxT_2(".") + ext);
    if(key.Exists())
    {
        wxString s;
        if(key.QueryValue(wxEmptyString, s) && s.StartsWith(wxT_2("CodeBlocks")))
            key.DeleteSelf();
    }

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext);
    if(key.Exists())
        key.DeleteSelf();
}

bool Associations::DoCheckAssociation(const wxString& ext, const wxString& descr, const wxString& exe, int icoNum)
{
    wxString BaseKeyName(wxT_2("HKEY_CURRENT_USER\\Software\\Classes\\"));

    if(platform::WindowsVersion() == platform::winver_Windows9598ME)
        BaseKeyName = wxT_2("HKEY_CLASSES_ROOT\\");

    wxRegKey key; // defaults to HKCR
    key.SetName(BaseKeyName + wxT_2(".") + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext);
    if (!key.Exists())
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\DefaultIcon"));
    if (!key.Exists())
        return false;
    wxString strVal;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(wxT_2("%s,%d"), exe.c_str(), icoNum))
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\open\\command"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxString::Format(wxT_2("\"%s\" \"%%1\""), exe.c_str()))
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\open\\ddeexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxT_2("[Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\open\\ddeexec\\application"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_SERVICE)
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\open\\ddeexec\\ifexec"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != wxT_2("[IfExec_Open(\"%1\")]"))
        return false;

    key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\open\\ddeexec\\topic"));
    if (!key.Open())
        return false;
    if (!key.QueryValue(wxEmptyString, strVal))
        return false;
    if (strVal != DDE_TOPIC)
        return false;

    if(ext.IsSameAs(FileFilters::CODEBLOCKS_EXT) || ext.IsSameAs(FileFilters::WORKSPACE_EXT))
    {
        wxString batchbuildargs = Manager::Get()->GetConfigManager(wxT_2("app"))->Read(wxT_2("/batch_build_args"), appglobals::DefaultBatchBuildArgs);
        key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\Build\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != wxT_2("\"") + exe + wxT_2("\" ") + batchbuildargs + wxT_2(" --build \"%1\""))
            return false;

        key.SetName(BaseKeyName + wxT_2("CodeBlocks.") + ext + wxT_2("\\shell\\Rebuild (clean)\\command"));
        if (!key.Open())
            return false;
        if (!key.QueryValue(wxEmptyString, strVal))
            return false;
        if (strVal != wxT_2("\"") + exe + wxT_2("\" ") + batchbuildargs + wxT_2(" --rebuild \"%1\""))
            return false;
    }

    return true;
}



BEGIN_EVENT_TABLE(ManageAssocsDialog, wxDialog)
    EVT_BUTTON(XRCID("wxID_OK"), ManageAssocsDialog::OnApply)
    EVT_BUTTON(XRCID("wxID_CANCEL"), ManageAssocsDialog::OnCancel)
    EVT_BUTTON(XRCID("clearAll"), ManageAssocsDialog::OnClearAll)
END_EVENT_TABLE()



ManageAssocsDialog::ManageAssocsDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadDialog(this, parent, wxT_2("dlgManageAssocs"));

    list = XRCCTRL(*this, "checkList", wxCheckListBox);
    assert(list);

    wxString d(wxT_2("."));

    wxChar exe[MAX_PATH] = {0};
    GetModuleFileName(0L, exe, MAX_PATH);

    for(unsigned int i = 0; i < Associations::CountAssocs(); ++i)
    {
        list->Append(d + knownTypes[i].ext + wxT_2("  (") + knownTypes[i].descr + wxT_2(")"));
        list->Check(i, Associations::DoCheckAssociation(knownTypes[i].ext, knownTypes[i].descr, exe, knownTypes[i].index));
    }

    CentreOnParent();
}

void ManageAssocsDialog::OnApply(wxCommandEvent& event)
{
    wxChar name[MAX_PATH] = {0};
    GetModuleFileName(0L, name, MAX_PATH);

    for(int i = 0; i < (int)list->GetCount(); ++i)
    {
        if(list->IsChecked(i))
            ::DoSetAssociation(name, i);
        else
            Associations::DoClearAssociation(knownTypes[i].ext);
    }

    Associations::UpdateChanges();
    EndModal(0);
}

void ManageAssocsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(0);
}

void ManageAssocsDialog::OnClearAll(wxCommandEvent& event)
{
    Associations::ClearAll();
    Associations::UpdateChanges();
    EndModal(0);
}



BEGIN_EVENT_TABLE(AskAssocDialog, wxDialog)
    EVT_BUTTON(XRCID("wxID_OK"), AskAssocDialog::OnOK)
END_EVENT_TABLE()



AskAssocDialog::AskAssocDialog(wxWindow* parent)
{
    wxXmlResource::Get()->LoadDialog(this, parent, wxT_2("askAssoc"));
}

void AskAssocDialog::OnOK(wxCommandEvent& event)
{
    EndModal(XRCCTRL(*this, "choice", wxRadioBox)->GetSelection());
}
