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

#include "sdk.h"


#ifndef CB_PRECOMP
    #include <wx/confbase.h>
    #include <wx/intl.h>
    #include <wx/filename.h>

    #include "manager.h"
    #include "projectmanager.h"
    #include "logmanager.h"
    #include "cbproject.h"
    #include "globals.h"
#endif

#include "devcpploader.h"

#include <wx/fileconf.h>


DevCppLoader::DevCppLoader(cbProject* project)
    : m_pProject(project)
{
	//ctor
}

DevCppLoader::~DevCppLoader()
{
	//dtor
}

bool DevCppLoader::Open(const wxString& filename)
{
    m_pProject->ClearAllProperties();

    wxFileConfig* dev = new wxFileConfig(wxT_2(""), wxT_2(""), filename, wxT_2(""), wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
    dev->SetPath(wxT_2("/Project"));
    int unitCount;
    dev->Read(wxT_2("UnitCount"), &unitCount, 0);

    wxString path, tmp, title, output, out_path, obj_path;
    wxArrayString array;
    int typ;

    // read project options
    dev->Read(wxT_2("Name"), &title, wxT_2(""));
    m_pProject->SetTitle(title);

    dev->Read(wxT_2("CppCompiler"), &tmp, wxT_2(""));
    if (tmp.IsEmpty())
        dev->Read(wxT_2("Compiler"), &tmp, wxT_2(""));
    array = GetArrayFromString(tmp, wxT_2("_@@_"));
    m_pProject->SetCompilerOptions(array);

    dev->Read(wxT_2("Linker"), &tmp, wxT_2(""));
    // some .dev I got my hands on, had the following in the linker options
    // remove them
    tmp.Replace(wxT_2("-o$@"), wxT_2(""));
    tmp.Replace(wxT_2("-o $@"), wxT_2(""));
    // read the list of linker options
    array = GetArrayFromString(tmp, wxT_2("_@@_"));
    // but separate the libs
    size_t i = 0;
    while (i < array.GetCount())
    {
        if (array[i].StartsWith(wxT_2("-l")))
        {
            wxString tmplib = array[i].Right(array[i].Length() - 2);
            // there might be multiple libs defined in a single line, like:
            // -lmingw32 -lscrnsave -lcomctl32 -lpng -lz -mwindows
            // we got to split by "-l" too...
            if (tmplib.Find(wxT_2(' ')) != wxNOT_FOUND)
            {
                wxArrayString tmparr = GetArrayFromString(array[i], wxT_2(" "));
                while (tmparr.GetCount())
                {
                    if (tmparr[0].StartsWith(wxT_2("-l")))
                        m_pProject->AddLinkLib(tmparr[0].Right(tmparr[0].Length() - 2));
                    else
                        array.Add(tmparr[0]);
                    tmparr.RemoveAt(0, 1);
                }
            }
            else
                m_pProject->AddLinkLib(tmplib);
            array.RemoveAt(i, 1);
        }
        else
            ++i;
    }
    // the remaining are linker options
    m_pProject->SetLinkerOptions(array);

    // read compiler's dirs
    dev->Read(wxT_2("Includes"), &tmp, wxT_2(""));
    array = GetArrayFromString(tmp, wxT_2(";"));
    m_pProject->SetIncludeDirs(array);

    // read linker's dirs
    dev->Read(wxT_2("Libs"), &tmp, wxT_2(""));
    array = GetArrayFromString(tmp, wxT_2(";"));
    m_pProject->SetLibDirs(array);

    // read resource files
    dev->Read(wxT_2("Resources"), &tmp, wxT_2(""));
    array = GetArrayFromString(tmp, wxT_2(",")); // make sure that this is comma-separated
    for (unsigned int i = 0; i < array.GetCount(); ++i)
    {
        if (array[i].IsEmpty())
            continue;
        tmp = array[i];
        m_pProject->AddFile(0, tmp, true, true);
    }

    // read project units
    for (int x = 0; x < unitCount; ++x)
    {
        path.Printf(wxT_2("/Unit%d"), x + 1);
        dev->SetPath(path);
        tmp.Clear();
        dev->Read(wxT_2("FileName"), &tmp, wxT_2(""));
        if (tmp.IsEmpty())
            continue;

        bool compile, compileCpp, link;
        dev->Read(wxT_2("Compile"), &compile, false);
        dev->Read(wxT_2("CompileCpp"), &compileCpp, true);
        dev->Read(wxT_2("Link"), &link, true);

        // .dev files set Link=0 for resources which is plain wrong for C::B.
        // correct this...
        if (!link && FileTypeOf(tmp) == ftResource)
            link = true;

        ProjectFile* pf = m_pProject->AddFile(0, tmp, compile || compileCpp, link);
        if (pf)
            pf->compilerVar = compileCpp ? wxT_2("CPP") : wxT_2("CC");
    }
    dev->SetPath(wxT_2("/Project"));

    // set the target type
    ProjectBuildTarget* target = m_pProject->GetBuildTarget(0);
    dev->Read(wxT_2("Type"), &typ, 0);
    target->SetTargetType(TargetType(typ));

    // decide on the output filename
    if (dev->Read(wxT_2("OverrideOutput"), (long)0) == 1)
        dev->Read(wxT_2("OverrideOutputName"), &output, wxT_2(""));
    if (output.IsEmpty())
        output = target->SuggestOutputFilename();
    dev->Read(wxT_2("ExeOutput"), &out_path, wxT_2(""));
    if (!out_path.IsEmpty())
        output = out_path + wxT_2("\\") + output;
    target->SetOutputFilename(output);

    // set the object output
    dev->Read(wxT_2("ObjectOutput"), &obj_path, wxT_2(""));
    if (!obj_path.IsEmpty())
        target->SetObjectOutput(obj_path);

    // all done
    delete dev;

    m_pProject->SetModified(true);
    return true;
}

bool DevCppLoader::Save(const wxString& filename)
{
    // no support to save DevCpp projects
    return false;
}
