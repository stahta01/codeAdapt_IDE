/////////////////////////////////////////////////////////////////////////////
// Name:        ca/filename.h
// Purpose:     caFileName - encapsulates a file path
// Author:      Tim S.
// Modified by:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef   CA_FILENAME_H
#define   CA_FILENAME_H

#include <wx/filename.h>

class WXDLLIMPEXP_BASE caFileName: protected wxFileName
{
public:
    // constructors and assignment

        // the usual stuff
    caFileName() { Clear(); }
    caFileName(const caFileName& filepath) { Assign(filepath); }

        // from a full filename: if it terminates with a '/', a directory path
        // is contructed (the name will be empty), otherwise a file name and
        // extension are extracted from it
    caFileName( const wxString& fullpath, wxPathFormat format = wxPATH_NATIVE )
        { Assign( fullpath, format ); }

        // from a directory name and a file name
    caFileName(const wxString& path,
               const wxString& name,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(path, name, format); }

        // from a volume, directory name, file base name and extension
    caFileName(const wxString& volume,
               const wxString& path,
               const wxString& name,
               const wxString& ext,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(volume, path, name, ext, format); }

        // from a directory name, file base name and extension
    caFileName(const wxString& path,
               const wxString& name,
               const wxString& ext,
               wxPathFormat format = wxPATH_NATIVE)
        { Assign(path, name, ext, format); }

        // the same for delayed initialization

    void Assign(const caFileName& filepath);

    void Assign(const wxString& fullpath,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& volume,
                const wxString& path,
                const wxString& name,
                const wxString& ext,
                bool hasExt,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& volume,
                const wxString& path,
                const wxString& name,
                const wxString& ext,
                wxPathFormat format = wxPATH_NATIVE)
        { Assign(volume, path, name, ext, !ext.empty(), format); }

    void Assign(const wxString& path,
                const wxString& name,
                wxPathFormat format = wxPATH_NATIVE);

    void Assign(const wxString& path,
                const wxString& name,
                const wxString& ext,
                wxPathFormat format = wxPATH_NATIVE);


//  Below are methods that are different in operation from wxFileName

    // always return '/' as path separator
    static wxChar GetPathSeparator(wxPathFormat format = wxPATH_NATIVE)
        { return '/'; }

};

#endif // CA_FILENAME_H
