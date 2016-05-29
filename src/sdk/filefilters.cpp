#include "sdk_precomp.h"
#include <map>
#include <wx/arrstr.h>
#include "filefilters.h"
#include "globals.h"

typedef std::map<wxString, wxString> FileFiltersMap;
static FileFiltersMap s_Filters;

static size_t s_LastFilterAllIndex = 0;

// Let's add some default extensions.
// The rest will be added by editor lexers ;)
void FileFilters::AddDefaultFileFilters()
{
    if (s_Filters.size() != 0)
        return;
    Add(_("Code::Blocks workspace files"),         wxT_2("*.workspace"));
    Add(_("Code::Blocks project files"),           wxT_2("*.cbp"));
    Add(_("Bloodshed Dev-C++ project files"),      wxT_2("*.dev"));
    Add(_("MS Visual C++ 6.0 project files"),      wxT_2("*.dsp"));
    Add(_("MS Visual Studio 7.0+ project files"),  wxT_2("*.vcproj"));
    Add(_("MS Visual C++ 6.0 workspace files"),    wxT_2("*.dsw"));
    Add(_("MS Visual Studio 7.0+ solution files"), wxT_2("*.sln"));
    Add(_("Apple Xcode 1.x project bundles"),      wxT_2("*.xcode"));
    Add(_("Apple Xcode 2.x project bundles"),      wxT_2("*.xcodeproj"));
}

bool FileFilters::Add(const wxString& name, const wxString& mask)
{
    if (name.IsEmpty() || mask.IsEmpty())
        return false; // both must be valid
    if (mask.Index(wxT_2(',')) != wxString::npos)
    {
        // replace commas with semicolons
        wxString tmp = mask;
        while (tmp.Replace(wxT_2(","), wxT_2(";")))
            ;
        s_Filters[name] = tmp;
    }
    else
        s_Filters[name] = mask;
    return true;
}

wxString FileFilters::GetFilterString(const wxString& ext)
{
    size_t count = 0;
    wxString ret;
    for (FileFiltersMap::iterator it = s_Filters.begin(); it != s_Filters.end(); ++it)
    {
        if (!ext.IsEmpty())
        {
            // filter based on parameter
            bool match = false;
            wxArrayString array = GetArrayFromString(it->second, wxT_2(";"), true);
            for (size_t i = 0; i < array.GetCount(); ++i)
            {
                if (ext.Matches(array[i]))
                {
                    match = true;
                    break;
                }
            }
            if (!match)
                continue; // filtered
        }
        ++count;
        if (!ret.IsEmpty())
            ret << wxT_2('|');
        ret << it->first << wxT_2('|') << it->second;
    }

    // last filter is always "All"
    if (!ret.IsEmpty())
        ret << wxT_2('|');
    ret << GetFilterAll();

    s_LastFilterAllIndex = count;

    return ret;
}

wxString FileFilters::GetFilterAll()
{
    s_LastFilterAllIndex = 0;
    if(platform::windows)
        return _("All files (*.*)|*.*");
    else
        return _("All files (*)|*");
}

size_t FileFilters::GetIndexForFilterAll()
{
    return s_LastFilterAllIndex;
}

bool FileFilters::GetFilterIndexFromName(const wxString& FiltersList, const wxString& FilterName, int& Index)
{
    bool bFound = false;
    // the List will contain 2 entries per type (description, mask)
    wxArrayString List = GetArrayFromString(FiltersList, wxT_2("|"), true);
    int LoopEnd = static_cast<int>(List.GetCount());
    for(int idxList = 0; idxList < LoopEnd; idxList+=2)
    {
        if(List[idxList] == FilterName)
        {
            Index = idxList/2;
            bFound = true;
        }
    } // end for : idx : idxList
    return bFound;
} // end of GetFilterIndexFromName

bool FileFilters::GetFilterNameFromIndex(const wxString& FiltersList, int Index, wxString& FilterName)
{    // we return the name (not the mask)
    bool bFound = false;
    // the List will contain 2 entries per type (description, mask)
    wxArrayString List = GetArrayFromString(FiltersList, wxT_2("|"), true);
    int LoopEnd = static_cast<int>(List.GetCount());
    if(2*Index < LoopEnd)
    {
        FilterName = List[2*Index];
        bFound = true;
    }
    return bFound;
} // end of GetFilterStringFromIndex

// define some constants used throughout C::B

const wxString FileFilters::WORKSPACE_EXT                = wxT_2("workspace");
const wxString FileFilters::CODEBLOCKS_EXT               = wxT_2("cbp");
const wxString FileFilters::DEVCPP_EXT                   = wxT_2("dev");
const wxString FileFilters::MSVC6_EXT                    = wxT_2("dsp");
const wxString FileFilters::MSVC7_EXT                    = wxT_2("vcproj");
const wxString FileFilters::MSVC6_WORKSPACE_EXT          = wxT_2("dsw");
const wxString FileFilters::MSVC7_WORKSPACE_EXT          = wxT_2("sln");
const wxString FileFilters::XCODE1_EXT                   = wxT_2("xcode");
const wxString FileFilters::XCODE2_EXT                   = wxT_2("xcodeproj");
const wxString FileFilters::ASM_EXT                      = wxT_2("asm");
const wxString FileFilters::D_EXT                        = wxT_2("d");
const wxString FileFilters::F_EXT                        = wxT_2("f");
const wxString FileFilters::F77_EXT                      = wxT_2("f77");
const wxString FileFilters::F90_EXT                      = wxT_2("f90");
const wxString FileFilters::F95_EXT                      = wxT_2("f95");
const wxString FileFilters::JAVA_EXT                     = wxT_2("java");
const wxString FileFilters::C_EXT                        = wxT_2("c");
const wxString FileFilters::CC_EXT                       = wxT_2("cc");
const wxString FileFilters::CPP_EXT                      = wxT_2("cpp");
const wxString FileFilters::CXX_EXT                      = wxT_2("cxx");
const wxString FileFilters::H_EXT                        = wxT_2("h");
const wxString FileFilters::HH_EXT                       = wxT_2("hh");
const wxString FileFilters::HPP_EXT                      = wxT_2("hpp");
const wxString FileFilters::HXX_EXT                      = wxT_2("hxx");
const wxString FileFilters::S_EXT                        = wxT_2("s");
const wxString FileFilters::SS_EXT                       = wxT_2("ss");
const wxString FileFilters::S62_EXT                      = wxT_2("s62");
const wxString FileFilters::OBJECT_EXT                   = wxT_2("o");
const wxString FileFilters::XRCRESOURCE_EXT              = wxT_2("xrc");
const wxString FileFilters::STATICLIB_EXT                = wxT_2("a");
const wxString FileFilters::RESOURCE_EXT                 = wxT_2("rc");
const wxString FileFilters::RESOURCEBIN_EXT              = wxT_2("res");
const wxString FileFilters::XML_EXT                      = wxT_2("xml");
const wxString FileFilters::SCRIPT_EXT                   = wxT_2("script");
#ifdef __WXMSW__
    const wxString FileFilters::DYNAMICLIB_EXT           = wxT_2("dll");
    const wxString FileFilters::EXECUTABLE_EXT           = wxT_2("exe");
    const wxString FileFilters::NATIVE_EXT               = wxT_2("sys");
#elif __WXMAC__
    const wxString FileFilters::DYNAMICLIB_EXT           = wxT_2("dylib");
    const wxString FileFilters::EXECUTABLE_EXT           = wxT_2("");
    const wxString FileFilters::NATIVE_EXT               = wxT_2("");
#else
    const wxString FileFilters::DYNAMICLIB_EXT           = wxT_2("so");
    const wxString FileFilters::EXECUTABLE_EXT           = wxT_2("");
    const wxString FileFilters::NATIVE_EXT               = wxT_2("");
#endif

// dot.ext version
const wxString FileFilters::WORKSPACE_DOT_EXT                = wxT_2('.') + FileFilters::WORKSPACE_EXT;
const wxString FileFilters::CODEBLOCKS_DOT_EXT               = wxT_2('.') + FileFilters::CODEBLOCKS_EXT;
const wxString FileFilters::DEVCPP_DOT_EXT                   = wxT_2('.') + FileFilters::DEVCPP_EXT;
const wxString FileFilters::MSVC6_DOT_EXT                    = wxT_2('.') + FileFilters::MSVC6_EXT;
const wxString FileFilters::MSVC7_DOT_EXT                    = wxT_2('.') + FileFilters::MSVC7_EXT;
const wxString FileFilters::MSVC6_WORKSPACE_DOT_EXT          = wxT_2('.') + FileFilters::MSVC6_WORKSPACE_EXT;
const wxString FileFilters::MSVC7_WORKSPACE_DOT_EXT          = wxT_2('.') + FileFilters::MSVC7_WORKSPACE_EXT;
const wxString FileFilters::XCODE1_DOT_EXT                   = wxT_2('.') + FileFilters::XCODE1_EXT;
const wxString FileFilters::XCODE2_DOT_EXT                   = wxT_2('.') + FileFilters::XCODE2_EXT;
const wxString FileFilters::ASM_DOT_EXT                      = wxT_2('.') + FileFilters::ASM_EXT;
const wxString FileFilters::D_DOT_EXT                        = wxT_2('.') + FileFilters::D_EXT;
const wxString FileFilters::F_DOT_EXT                        = wxT_2('.') + FileFilters::F_EXT;
const wxString FileFilters::F77_DOT_EXT                      = wxT_2('.') + FileFilters::F77_EXT;
const wxString FileFilters::F90_DOT_EXT                      = wxT_2('.') + FileFilters::F90_EXT;
const wxString FileFilters::F95_DOT_EXT                      = wxT_2('.') + FileFilters::F95_EXT;
const wxString FileFilters::JAVA_DOT_EXT                     = wxT_2('.') + FileFilters::JAVA_EXT;
const wxString FileFilters::C_DOT_EXT                        = wxT_2('.') + FileFilters::C_EXT;
const wxString FileFilters::CC_DOT_EXT                       = wxT_2('.') + FileFilters::CC_EXT;
const wxString FileFilters::CPP_DOT_EXT                      = wxT_2('.') + FileFilters::CPP_EXT;
const wxString FileFilters::CXX_DOT_EXT                      = wxT_2('.') + FileFilters::CXX_EXT;
const wxString FileFilters::H_DOT_EXT                        = wxT_2('.') + FileFilters::H_EXT;
const wxString FileFilters::HH_DOT_EXT                       = wxT_2('.') + FileFilters::HH_EXT;
const wxString FileFilters::HPP_DOT_EXT                      = wxT_2('.') + FileFilters::HPP_EXT;
const wxString FileFilters::HXX_DOT_EXT                      = wxT_2('.') + FileFilters::HXX_EXT;
const wxString FileFilters::S_DOT_EXT                        = wxT_2('.') + FileFilters::S_EXT;
const wxString FileFilters::SS_DOT_EXT                       = wxT_2('.') + FileFilters::SS_EXT;
const wxString FileFilters::S62_DOT_EXT                      = wxT_2('.') + FileFilters::S62_EXT;
const wxString FileFilters::OBJECT_DOT_EXT                   = wxT_2('.') + FileFilters::OBJECT_EXT;
const wxString FileFilters::XRCRESOURCE_DOT_EXT              = wxT_2('.') + FileFilters::XRCRESOURCE_EXT;
const wxString FileFilters::STATICLIB_DOT_EXT                = wxT_2('.') + FileFilters::STATICLIB_EXT;
const wxString FileFilters::RESOURCE_DOT_EXT                 = wxT_2('.') + FileFilters::RESOURCE_EXT;
const wxString FileFilters::RESOURCEBIN_DOT_EXT              = wxT_2('.') + FileFilters::RESOURCEBIN_EXT;
const wxString FileFilters::XML_DOT_EXT                      = wxT_2('.') + FileFilters::XML_EXT;
const wxString FileFilters::SCRIPT_DOT_EXT                   = wxT_2('.') + FileFilters::SCRIPT_EXT;
#ifdef __WXMSW__
    const wxString FileFilters::DYNAMICLIB_DOT_EXT           = wxT_2('.') + FileFilters::DYNAMICLIB_EXT;
    const wxString FileFilters::EXECUTABLE_DOT_EXT           = wxT_2('.') + FileFilters::EXECUTABLE_EXT;
    const wxString FileFilters::NATIVE_DOT_EXT               = wxT_2('.') + FileFilters::NATIVE_EXT;
#else
    const wxString FileFilters::DYNAMICLIB_DOT_EXT           = wxT_2('.') + FileFilters::DYNAMICLIB_EXT;
    const wxString FileFilters::EXECUTABLE_DOT_EXT           = EXECUTABLE_EXT; // no dot, since no extension
    const wxString FileFilters::NATIVE_DOT_EXT               = NATIVE_EXT; // no dot, since no extension
#endif
