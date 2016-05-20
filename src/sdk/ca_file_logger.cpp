//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2014 Eran Ifrah
// file name            : file_logger.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "ca_file_logger.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <sys/time.h>
#include <wx/log.h>
#include <wx/crt.h>
#include "cl/cl_standard_paths.h"

static caFileLogger theLogger;
static bool initialized = false;

caFileLogger::caFileLogger()
    : m_verbosity(caFileLogger::Error)
    , _requestedLogLevel(caFileLogger::Developer)
    , m_fp(NULL)
{
}

caFileLogger::~caFileLogger()
{
    if(m_fp) {
        fclose(m_fp);
        m_fp = NULL;
    }
}

void caFileLogger::AddLogLine(const wxString& msg, int verbosity)
{
    if(msg.IsEmpty()) return;
    if((m_verbosity >= verbosity) && m_fp) {
        wxString formattedMsg = Prefix(verbosity);
        formattedMsg << " " << msg;
        formattedMsg.Trim().Trim(false);
        formattedMsg << wxT("\n");
        wxFprintf(m_fp, wxT("%s"), formattedMsg.c_str());
        fflush(m_fp);
    }
}

caFileLogger& caFileLogger::Get() { return theLogger; }

void caFileLogger::SetVerbosity(int level)
{
    if(level > caFileLogger::Warning) {
        clSYSTEM() << "Log verbosity is now set to:" << caFileLogger::GetVerbosityAsString(level) << clEndl;
    }
    m_verbosity = level;
}

int caFileLogger::GetVerbosityAsNumber(const wxString& verbosity)
{
    if(verbosity == wxT("Debug")) {
        return caFileLogger::Dbg;

    } else if(verbosity == wxT("Error")) {
        return caFileLogger::Error;

    } else if(verbosity == wxT("Warning")) {
        return caFileLogger::Warning;

    } else if(verbosity == wxT("System")) {
        return caFileLogger::System;

    } else if(verbosity == wxT("Developer")) {
        return caFileLogger::Developer;

    } else {
        return caFileLogger::Error;
    }
}

wxString caFileLogger::GetVerbosityAsString(int verbosity)
{
    switch(verbosity) {
    case caFileLogger::Dbg:
        return wxT("Debug");

    case caFileLogger::Error:
        return wxT("Error");

    case caFileLogger::Warning:
        return wxT("Warning");

    case caFileLogger::Developer:
        return wxT("Developer");

    default:
        return wxT("Error");
    }
}

void caFileLogger::SetVerbosity(const wxString& verbosity) { SetVerbosity(caFileLogger::GetVerbosityAsNumber(verbosity)); }

void caFileLogger::OpenLog(const wxString& fullName, int verbosity)
{
    if(!initialized) {
        wxString filename;
        filename << clStandardPaths::Get().GetUserDataDir() << wxFileName::GetPathSeparator() << fullName;
        theLogger.m_fp = wxFopen(filename, wxT("a+"));
        theLogger.m_verbosity = verbosity;
        initialized = true;
    }
}

void caFileLogger::AddLogLine(const wxArrayString& arr, int verbosity)
{
    for(size_t i = 0; i < arr.GetCount(); ++i) {
        AddLogLine(arr.Item(i), verbosity);
    }
}

void caFileLogger::Flush()
{
    if(m_buffer.IsEmpty()) {
        return;
    }
    wxFprintf(m_fp, "%s\n", m_buffer);
    fflush(m_fp);
    m_buffer.Clear();
}

wxString caFileLogger::Prefix(int verbosity) const
{
    wxString prefix;

    timeval tim;
    gettimeofday(&tim, NULL);
    int ms = (int)tim.tv_usec / 1000.0;

    wxString msStr = wxString::Format(wxT("%03d"), ms);

    prefix << wxT("[") << wxDateTime::Now().FormatISOTime() << wxT(":") << msStr;

    switch(verbosity) {
    case System:
        prefix << wxT(" SYS]");
        break;

    case Error:
        prefix << wxT(" ERR]");
        break;

    case Warning:
        prefix << wxT(" WRN]");
        break;

    case Dbg:
        prefix << wxT(" DBG]");
        break;

    case Developer:
        prefix << wxT(" DVL]");
        break;
    }
    return prefix;
}
