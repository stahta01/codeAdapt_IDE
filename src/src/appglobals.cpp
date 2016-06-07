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

#include "ca/sdk.h"
#ifndef CB_PRECOMP
#include <wx/utils.h>
#include <wx/intl.h>
#include "configmanager.h"
#endif
#include "appglobals.h"

namespace appglobals
{
    const wxString AppVendor              = wxT_2("Code::Blocks");
    const wxString AppName			    = wxT_2("Code::Blocks");

    #if SVN_BUILD
        const wxString AppVersion				= wxT_2("svn build");
        const wxString AppActualVersionVerb	= wxT_2("svn build  rev ") +  ConfigManager::GetRevisionString();
        const wxString AppActualVersion		= wxT_2("svn-r") +  ConfigManager::GetRevisionString();
    #else
        const wxString AppVersion				= wxT_2(RELEASE);
        const wxString AppActualVersionVerb	= wxT_2("Release " RELEASE "  rev ") + ConfigManager::GetRevisionString();
        const wxString AppActualVersion		= wxT_2(RELEASE "-r") + ConfigManager::GetRevisionString();
    #endif

    const wxString AppUrl					= wxT_2("http://www.codeblocks.org");
    const wxString AppContactEmail		= wxT_2("info@codeblocks.org");

    #if defined(__WXMSW__)
      const wxString AppPlatform = wxT_2("Windows");
    #elif defined(__WXOS2__)
      const wxString AppPlatform = wxT_2("OS/2");
    #elif defined(__WXMAC__) || defined(__WXCOCOA__)
      const wxString AppPlatform = wxT_2("Mac OS X");
    #elif defined(__APPLE__)
      const wxString AppPlatform = wxT_2("Darwin");
    #elif defined(__FreeBSD__)
      const wxString AppPlatform = wxT_2("FreeBSD");
    #elif defined(__UNIX__)
      const wxString AppPlatform = wxT_2("Linux");
    #else
      const wxString AppPlatform = wxT_2("Unknown");
    #endif

    const wxString AppWXAnsiUnicode = platform::unicode ? wxT_2("unicode") : wxT_2("ANSI");

    const wxString AppBuildTimestamp = (wxString(wxT_2(__DATE__)) + wxT_2(", ") + wxT_2(__TIME__) + wxT_2(" - wx") + wxString(wxT_2(wxVERSION_NUM_DOT_STRING)) + wxT_2(" (") + AppPlatform + wxT_2(", ") + AppWXAnsiUnicode + wxT_2(")") );

    const wxString DefaultBatchBuildArgs = wxT_2("-na -nd -ns --batch-build-notify");
};



