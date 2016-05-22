#ifndef MSVCLOADER_H
#define MSVCLOADER_H

#include <wx/dynarray.h>
#include <wx/filename.h>
#include <wx/hashmap.h>
#include "ibaseloader.h"
#include "compiletargetbase.h" // for target type

// forward decls
class caProject;
class caProjectBuildTarget;

class MSVCLoader : public IBaseLoader
{
	public:
		MSVCLoader(caProject* project);
		virtual ~MSVCLoader();

		bool Open(const wxString& filename);
		bool Save(const wxString& filename);
	protected:
        bool ReadConfigurations();
        bool ParseConfiguration(int index);
        bool ParseSourceFiles();
        bool ParseResponseFile(const wxString filename, wxArrayString& output);
        void ProcessCompilerOptions(caProjectBuildTarget* target, const wxString& opts);
        void ProcessLinkerOptions(caProjectBuildTarget* target, const wxString& opts);
        void ProcessResourceCompilerOptions(caProjectBuildTarget* target, const wxString& opts);
        wxArrayString OptStringTokeniser(const wxString& opts);
        wxString RemoveQuotes(const wxString& src);

        caProject* m_pProject;
        bool m_ConvertSwitches;
        wxArrayString m_Configurations;
        wxArrayInt m_ConfigurationsLineIndex;
        wxFileName m_Filename;
        TargetType m_Type;
        WX_DECLARE_STRING_HASH_MAP(TargetType, HashTargetType);
        HashTargetType m_TargType;
        HashTargetType m_TargetBasedOn;
        int m_BeginTargetLine;
	private:
};

#endif // MSVCLOADER_H

