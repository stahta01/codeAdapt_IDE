#include "ca/sdk.h"
#include "compilerMINGWgenerator.h"
#include <wx/intl.h>
#include "cbexception.h"
#include "cbproject.h"
#include "compilerfactory.h"
#include "compiler.h"
#include "manager.h"
#include "configmanager.h"
#include "logmanager.h"
#include "macrosmanager.h"
#include "scriptingmanager.h"

CompilerMINGWGenerator::CompilerMINGWGenerator()
{
    //ctor
    m_VerStr = wxEmptyString;
}

CompilerMINGWGenerator::~CompilerMINGWGenerator()
{
    //dtor
}

wxString CompilerMINGWGenerator::SetupIncludeDirs(Compiler* compiler, ProjectBuildTarget* target)
{
    wxString result = CompilerCommandGenerator::SetupIncludeDirs(compiler, target);
#if 0
    m_VerStr = compiler->GetVersionString();
    wxString pch_prepend;
    bool IsGcc4 = m_VerStr.Left(1).IsSameAs(wxT_2("4"));

    // for PCH to work, the very first include dir *must* be the object output dir
    // *only* if PCH is generated in the object output dir
    if (target &&
        target->GetParentProject()->GetModeForPCH() == pchObjectDir)
    {
        wxArrayString includedDirs; // avoid adding duplicate dirs...
        wxString sep = wxFILE_SEP_PATH;
        // find all PCH in project
        int count = target->GetParentProject()->GetFilesCount();
        for (int i = 0; i < count; ++i)
        {
            ProjectFile* f = target->GetParentProject()->GetFile(i);
            if (FileTypeOf(f->relativeFilename) == ftHeader &&
                f->compile)
            {
                // it is a PCH; add it's object dir to includes
                wxString dir = wxFileName(target->GetObjectOutput() + sep + f->GetObjName()).GetPath();
                if (includedDirs.Index(dir) == wxNOT_FOUND)
                {
                    includedDirs.Add(dir);
                    QuoteStringIfNeeded(dir);
                    if (!IsGcc4)
                        pch_prepend << compiler->GetSwitches().includeDirs << dir << wxT_2(' ');
                    else
                        pch_prepend << wxT_2("-iquote") << dir << wxT_2(' ');
                }
            }
        }
        // for gcc-4.0+, use the following:
        // pch_prepend << wxT_2("-iquote") << dir << wxT_2(' ');
        // for earlier versions, -I- must be used
        if (!IsGcc4)
            pch_prepend << wxT_2("-I- ");
        count = (int)includedDirs.GetCount();
        for (int i = 0; i < count; ++i)
        {
            QuoteStringIfNeeded(includedDirs[i]);
            pch_prepend << compiler->GetSwitches().includeDirs << includedDirs[i] << wxT_2(' ');
        }
        pch_prepend << wxT_2("-I. ");
        result.Prepend(pch_prepend);
    }
#endif // #if 0

    // add in array
    return result;
}
