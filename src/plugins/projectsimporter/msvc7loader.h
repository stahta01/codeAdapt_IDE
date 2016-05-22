#ifndef MSVC7LOADER_H
#define MSVC7LOADER_H

#include "ibaseloader.h"

// forward decls
class caProject;
class ProjectFile;

class MSVC7Loader : public IBaseLoader
{
    public:
        MSVC7Loader(caProject* project);
        virtual ~MSVC7Loader();

        bool Open(const wxString& filename);
        bool Save(const wxString& filename);
    protected:
        caProject* m_pProject;
        bool m_ConvertSwitches;
        // macros used in Visual Studio projects
        wxString m_ConfigurationName;
        wxString m_ProjectName;
        wxString m_TargetFilename;
        wxString m_TargetPath;
        wxString m_OutDir;
        wxString m_IntDir;
        int m_Version;
    private:
        wxString m_PlatformName;

        wxString ReplaceMSVCMacros(const wxString& str);
        void HandleFileConfiguration(TiXmlElement* file, ProjectFile* pf);
        bool DoSelectConfiguration(TiXmlElement* root);
        bool DoImport(TiXmlElement* conf);
        bool DoImportFiles(TiXmlElement* root, int numConfigurations);
        bool ParseInputString(const wxString& Input, wxArrayString& Output);
};

#endif // MSVC7LOADER_H
