/***************************************************************
 * Name:      avHeader.cpp
 * Purpose:   Code of the class to parse the version values from version.h
 * Author:    JGM (jgmdev@gmail.com)
 * Created:   2007-10-20
 * Copyright: JGM
 * License: GPL
 **************************************************************/

//{Headers
#include "avHeader.h"
#include <wx/ffile.h>
//}


//{Constructor
avHeader::avHeader(const wxString& text )
{
    m_text = text;
}
//}


//{Setter functions
void avHeader::SetString(const wxString& text )
{
    m_text = text;
}

bool avHeader::LoadFile(const wxString& fileName)
{
    if (!fileName.IsEmpty())
    {
        wxFFile file( fileName, wxT_2("r") );
        if (file.IsOpened())
        {
            file.ReadAll(&m_text);
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}
//}


//{Getter functions
long avHeader::GetValue(const wxString& nameOfVariable ) const
{
    wxString strExpression;
    strExpression << wxT_2("(") << nameOfVariable << wxT_2(")");
    strExpression << wxT_2("([ \t\n\r\f\v])*([=])([ \t\n\r\f\v])*([0-9]+)([ \t\n\r\f\v])*([;])+");
    wxRegEx expression;

    if (!expression.Compile(strExpression))
    {
        return 0;
    }

    if (expression.Matches(m_text))
    {
        wxString strResult;
        strResult = expression.GetMatch(m_text);
        expression.ReplaceAll(&strResult, wxT_2("\\5"));
        long value;
        strResult.ToLong(&value);
        return value;
    }

    return 0;
}

wxString avHeader::GetString(const wxString& nameOfVariable ) const
{
    wxString strExpression;
    strExpression << wxT_2("(") << nameOfVariable << wxT_2(")");
    strExpression << wxT_2("([\\[\\]]+)([ \t\n\r\f\v])*([=])([ \t\n\r\f\v])*([\\\"])+([0-9A-Za-z \\-]+)([\\\"])+([ \t\n\r\f\v])*([;])+");
    wxRegEx expression;

    if (!expression.Compile(strExpression))
    {
        return wxT_2("");
    }

    if (expression.Matches(m_text))
    {
        wxString strResult;
        strResult = expression.GetMatch(m_text);
        expression.ReplaceAll(&strResult, wxT_2("\\7"));
        return strResult;
    }

    return wxT_2("");
}
//}
