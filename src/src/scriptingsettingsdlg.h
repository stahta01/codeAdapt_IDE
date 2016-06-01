#ifndef SCRIPTINGSETTINGSDLG_H
#define SCRIPTINGSETTINGSDLG_H

#include <vector>
#include <wx/listctrl.h> // wxListEvent
#include <wx/dialog.h>

class ScriptEntry// : public ISerializable (<-- can't make it work)
{
public:
    wxString SerializeOut() const
    {
        return wxString::Format(wxT_2("%s;%d;%d;%s"), script.c_str(), enabled ? 1 : 0, registered ? 1 : 0, menu.c_str());
    }
    void SerializeIn(const wxString& s)
    {
        wxString tmp = s;
        script = tmp.BeforeFirst(wxT_2(';'));
        tmp = tmp.AfterFirst(wxT_2(';'));
        enabled = tmp.BeforeFirst(wxT_2(';')) == wxT_2("1") ? true : false;
        tmp = tmp.AfterFirst(wxT_2(';'));
        registered = tmp.BeforeFirst(wxT_2(';')) == wxT_2("1") ? true : false;
        tmp = tmp.AfterFirst(wxT_2(';'));
        menu = tmp.BeforeFirst(wxT_2(';'));
    }

    wxString script;
    bool enabled;
    bool registered;
    wxString menu;
};
typedef std::vector<ScriptEntry> ScriptsVector;

class ScriptingSettingsDlg : public wxDialog
{
    public:
        ScriptingSettingsDlg(wxWindow* parent);
        ~ScriptingSettingsDlg();

        void EndModal(int retCode);
    protected:
        void FillScripts();
        void UpdateState();
        void FillTrusts();
        void UpdateTrustsState();
        void LoadItem(long item);
        void SaveItem(long item);

        void OnListSelection(wxListEvent& event);
        void OnListDeselection(wxListEvent& event);
        void OnAddScript(wxCommandEvent& event);
        void OnRemoveScript(wxCommandEvent& event);
        void OnScriptChanged(wxCommandEvent& event);
        void OnScriptMenuChanged(wxCommandEvent& event);
        void OnEnable(wxCommandEvent& event);
        void OnRegister(wxCommandEvent& event);
        void OnBrowse(wxCommandEvent& event);

        void OnTrustSelection(wxListEvent& event);
        void OnDeleteTrust(wxCommandEvent& event);
        void OnValidateTrusts(wxCommandEvent& event);

        ScriptsVector m_ScriptsVector;
        bool m_IgnoreTextEvents;
    private:
        DECLARE_EVENT_TABLE()
};



#endif // SCRIPTINGSETTINGSDLG_H
