#include "avSvnRevision.h"
#include <wx/intl.h>
#include <wx/utils.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <tinyxml/tinyxml.h>
#include <globals.h>

bool QuerySvn(const wxString& workingDir, wxString& revision, wxString& date)
{
    revision = wxT_2("0");
    date = wxT_2("unknown date");
    wxString svncmd = wxT_2("svn info --xml --non-interactive ");
    svncmd.Append(wxT_2("\"") + workingDir + wxT_2("\""));
    wxArrayString xmlOutput;

    if (wxExecute(svncmd, xmlOutput) != -1)
    {

        wxString buf = wxT_2("");

        for(unsigned int i=0; i<xmlOutput.GetCount(); ++i){
            buf << xmlOutput[i];
        }

        TiXmlDocument doc;
        doc.Parse(cbU2C(buf));

        if (doc.Error())
            return 0;

        TiXmlHandle hCommit(&doc);
        hCommit = hCommit.FirstChildElement("info").FirstChildElement("entry").FirstChildElement("commit");
		if(const TiXmlElement* e = hCommit.ToElement())
        {
            revision = e->Attribute("revision") ? cbC2U(e->Attribute("revision")) : wxT_2("");
            const TiXmlElement* d = e->FirstChildElement("date");
            if(d && d->GetText())
                date = cbC2U(d->GetText());

            return 1;
        }
    }
    return 0;
}
