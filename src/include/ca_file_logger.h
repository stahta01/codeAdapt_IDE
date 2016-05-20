//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2014 Eran Ifrah
// old file name        : file_logger.h
// new file name        : ca_file_logger.h
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

#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <wx/ffile.h>
#include "cl/codelite_exports.h"
#include <wx/stopwatch.h>

// manipulator function
class caFileLogger;
typedef caFileLogger& (*FileLoggerFunction)(caFileLogger&);

class WXDLLIMPEXP_CL caFileLogger
{
public:
    enum { System = -1, Error = 0, Warning = 1, Dbg = 2, Developer = 3 };

protected:
    int m_verbosity;
    int _requestedLogLevel;
    FILE* m_fp;
    wxString m_buffer;

public:
    caFileLogger();
    ~caFileLogger();

    caFileLogger& SetRequestedLogLevel(int level)
    {
        _requestedLogLevel = level;
        return *this;
    }

    int GetRequestedLogLevel() const { return _requestedLogLevel; }

    /**
     * @brief create log entry prefix
     */
    wxString Prefix(int verbosity) const;

    /**
     * @brief open the log file
     */
    static void OpenLog(const wxString& fullName, int verbosity);

    static caFileLogger& Get();

    void AddLogLine(const wxString& msg, int verbosity);
    /**
     * @brief print array into the log file
     * @param arr
     * @param verbosity
     */
    void AddLogLine(const wxArrayString& arr, int verbosity);
    void SetVerbosity(int level);

    // Set the verbosity as string
    void SetVerbosity(const wxString& verbosity);

    // Various util methods
    static wxString GetVerbosityAsString(int verbosity);
    static int GetVerbosityAsNumber(const wxString& verbosity);

    inline caFileLogger& operator<<(FileLoggerFunction f)
    {
        Flush();
        return *this;
    }

    /**
     * @brief special wxArrayString printing
     */
    inline caFileLogger& operator<<(const wxArrayString& arr)
    {
        if(GetRequestedLogLevel() > m_verbosity) {
            return *this;
        }
        if(!m_buffer.IsEmpty()) {
            m_buffer << " ";
        }
        m_buffer << "[";
        if(!arr.IsEmpty()) {
            for(size_t i = 0; i < arr.size(); ++i) {
                m_buffer << arr.Item(i) << ", ";
            }
            m_buffer.RemoveLast(2);
        }
        m_buffer << "]";
        return *this;
    }

    /**
     * @brief append any type to the buffer, take log level into consideration
     */
    template <typename T> caFileLogger& Append(const T& elem, int level)
    {
        if(level > m_verbosity) {
            return *this;
        }
        if(!m_buffer.IsEmpty()) {
            m_buffer << " ";
        }
        m_buffer << elem;
        return *this;
    }

    /**
     * @brief flush the logger content
     */
    void Flush();
};

inline caFileLogger& clEndl(caFileLogger& d)
{
    d.Flush();
    return d;
}

template <typename T> caFileLogger& operator<<(caFileLogger& logger, const T& obj)
{
    logger.Append(obj, logger.GetRequestedLogLevel());
    return logger;
}

#define CL_SYSTEM(...) caFileLogger::Get().AddLogLine(wxString::Format(__VA_ARGS__), caFileLogger::System);
#define CL_ERROR(...) caFileLogger::Get().AddLogLine(wxString::Format(__VA_ARGS__), caFileLogger::Error);
#define CL_WARNING(...) caFileLogger::Get().AddLogLine(wxString::Format(__VA_ARGS__), caFileLogger::Warning);
#define CL_DEBUG(...) caFileLogger::Get().AddLogLine(wxString::Format(__VA_ARGS__), caFileLogger::Dbg);
#define CL_DEBUGS(s) caFileLogger::Get().AddLogLine(s, caFileLogger::Dbg);
#define CL_DEBUG1(...) caFileLogger::Get().AddLogLine(wxString::Format(__VA_ARGS__), caFileLogger::Developer);
#define CL_DEBUG_ARR(arr) caFileLogger::Get().AddLogLine(arr, caFileLogger::Dbg);
#define CL_DEBUG1_ARR(arr) caFileLogger::Get().AddLogLine(arr, caFileLogger::Developer);

// New API
#define clDEBUG()                                           \
    caFileLogger::Get().SetRequestedLogLevel(caFileLogger::Dbg) \
        << caFileLogger::Get().Prefix(caFileLogger::Get().GetRequestedLogLevel())

#define clDEBUG1()                                                \
    caFileLogger::Get().SetRequestedLogLevel(caFileLogger::Developer) \
        << caFileLogger::Get().Prefix(caFileLogger::Get().GetRequestedLogLevel())

#define clERROR()                                             \
    caFileLogger::Get().SetRequestedLogLevel(caFileLogger::Error) \
        << caFileLogger::Get().Prefix(caFileLogger::Get().GetRequestedLogLevel())

#define clWARNING()                                             \
    caFileLogger::Get().SetRequestedLogLevel(caFileLogger::Warning) \
        << caFileLogger::Get().Prefix(caFileLogger::Get().GetRequestedLogLevel())

#define clSYSTEM()                                             \
    caFileLogger::Get().SetRequestedLogLevel(caFileLogger::System) \
        << caFileLogger::Get().Prefix(caFileLogger::Get().GetRequestedLogLevel())

#endif // FILELOGGER_H
