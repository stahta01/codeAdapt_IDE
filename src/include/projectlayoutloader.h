#ifndef PROJECTLAYOUTLOADER_H
#define PROJECTLAYOUTLOADER_H

#include <wx/string.h>

class caProject;

class DLLIMPORT ProjectLayoutLoader
{
	public:
		ProjectLayoutLoader(caProject* project);
		virtual ~ProjectLayoutLoader();

        bool Open(const wxString& filename);
        bool Save(const wxString& filename);

        ProjectFile* GetTopProjectFile(){ return m_TopProjectFile; }
	protected:
	private:
        caProject* m_pProject;
        ProjectFile* m_TopProjectFile;
};

#endif // PROJECTLAYOUTLOADER_H

