#ifndef CA_FILETREEDATA_H
#define CA_FILETREEDATA_H

#include "settings.h"
#include "misctreeitemdata.h"

// forward decl
class caProject;
class ProjectFile;

class DLLIMPORT caFileTreeData : public MiscTreeItemData
{
    public:
        /// The kind of tree node
        enum FileTreeDataKind
        {
            ftdkUndefined = 0,
            ftdkProject,
            ftdkFolder,
            ftdkFile,
            ftdkVirtualGroup, // wilcard matching
            ftdkVirtualFolder
        };

        caFileTreeData(caProject* project, FileTreeDataKind kind = ftdkUndefined)
            : m_Index(-1),
            m_Project(project),
            m_file(0),
            m_kind(kind)
        {}

        FileTreeDataKind GetKind() const { return m_kind; }
        caProject* GetProject() const { return m_Project; }
        int GetFileIndex() const { return m_Index; }
        ProjectFile* GetProjectFile() const { return m_file; }
        const wxString& GetFolder() const { return m_folder; }

        void SetKind(FileTreeDataKind kind){ m_kind = kind; }
        void SetProject(caProject* project){ m_Project = project; }
        // only valid for file selections
        void SetFileIndex(int index){ m_Index = index; }
        void SetProjectFile(ProjectFile* file){ m_file = file; }
        // only valid for folder selections
        void SetFolder(const wxString& folder){ m_folder = folder; }
    private:
        int m_Index;
        caProject* m_Project;
        ProjectFile* m_file;
        wxString m_folder;
        FileTreeDataKind m_kind;
};

#endif // CA_FILETREEDATA_H
