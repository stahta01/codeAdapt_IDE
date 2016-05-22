#ifndef TODOLIST_H
#define TODOLIST_H


#include <wx/arrstr.h>
#include "api/plugin.h" // the base class we 're inheriting
#include "globals.h"

class wxMenuBar;
class wxMenu;
class wxToolBar;
class caFileTreeData;
class wxCommandEvent;
class wxUpdateUIEvent;
class CodeBlocksEvent;
class ToDoListView;
class caFileTreeData;

class ToDoList : public caPlugin
{
    public:
        ToDoList();
        ~ToDoList();
        virtual caConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
        int Configure();
        void BuildMenu(wxMenuBar* menuBar);
        void BuildModuleMenu(const ModuleType type, wxMenu* menu, const caFileTreeData* data = 0);
        bool BuildToolBar(wxToolBar* toolBar);
        void OnAttach(); // fires when the plugin is attached to the application
        void OnRelease(bool appShutDown); // fires when the plugin is released from the application
    private:
        void OnAppDoneStartup(CodeBlocksEvent& event);
        void OnViewList(wxCommandEvent& event);
        void OnAddItem(wxCommandEvent& event);
        void OnReparse(CodeBlocksEvent& event);
        void OnReparseCurrent(CodeBlocksEvent& event);
        void OnUpdateUI(wxUpdateUIEvent& event);
        void OnStartParsingProjects(wxTimerEvent& event);
        void LoadTypes();
        void SaveTypes();
        void ParseCurrent(bool forced = false);
        void Parse();

        ToDoListView* m_pListLog;
        int m_ListPageIndex;
        bool m_AutoRefresh;
        bool m_InitDone;
        bool m_ParsePending;
        bool m_StandAlone;
        wxArrayString m_Types;
        wxTimer m_timer;

        DECLARE_EVENT_TABLE()
};

#endif // TODOLIST_H

