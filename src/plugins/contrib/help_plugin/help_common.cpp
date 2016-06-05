#include "help_common.h"
#include <configmanager.h>
#include <wx/intl.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>


using std::find;
using std::make_pair;

int HelpCommon::m_DefaultHelpIndex = -1;
int HelpCommon::m_NumReadFromIni = 0;

void HelpCommon::LoadHelpFilesVector(HelpCommon::HelpFilesVector &vect)
{
  vect.clear();
  HelpCommon::setNumReadFromIni(0);
  ConfigManager* conf = Manager::Get()->GetConfigManager(wxT_2("help_plugin"));
  m_DefaultHelpIndex = conf->ReadInt(wxT_2("/default"), -1);
  wxArrayString list = conf->EnumerateSubPaths(wxT_2("/"));

  for (unsigned int i = 0; i < list.GetCount(); ++i)
  {
      HelpFileAttrib hfa;
      wxString name = conf->Read(list[i] + wxT_2("/name"), wxEmptyString);
      hfa.name = conf->Read(list[i] + wxT_2("/file"), wxEmptyString);
      conf->Read(list[i] + wxT_2("/isexec"), &hfa.isExecutable);
      conf->Read(list[i] + wxT_2("/embeddedviewer"), &hfa.openEmbeddedViewer);
      // Patch by Yorgos Pagles: Read new attributes from settings
      int keyWordCase=0;
      (conf->Read(list[i] + wxT_2("/keywordcase"), &keyWordCase));
      hfa.keywordCase = static_cast<HelpCommon::StringCase>(keyWordCase);
      hfa.defaultKeyword = conf->Read(list[i] + wxT_2("/defaultkeyword"), wxEmptyString);

      if (!name.IsEmpty() && !hfa.name.IsEmpty())
      {
        vect.push_back(make_pair(name, hfa));
      }
  }

  wxString docspath = ConfigManager::GetFolder(sdBase)+_("/share/codeblocks/docs");
  wxString iniFileName =  docspath + wxFileName::GetPathSeparator() + wxT_2("index.ini");

  if ((wxFileName::DirExists(docspath)) && (wxFileName::FileExists(iniFileName)))
  {
    wxTextFile hFile(iniFileName);
    hFile.Open();
    unsigned int cnt = hFile.GetLineCount();

    for(unsigned int i = 0; i < cnt; i++)
    {
      wxString line = hFile.GetLine(i);

      if (!line.IsEmpty())
      {
        wxString item = line.BeforeLast('=').Strip();
        wxString file = line.AfterLast('=').Strip();
        file = docspath + wxFileName::GetPathSeparator() + file;

        if (!item.IsEmpty() && !file.IsEmpty())
        {
            HelpFileAttrib hfa;
            hfa.name = file;
            hfa.isExecutable = false;
            hfa.openEmbeddedViewer = false;
            hfa.readFromIni = true;
            hfa.keywordCase = static_cast<HelpCommon::StringCase> (0);
            hfa.defaultKeyword = wxEmptyString;

            if (!hfa.name.IsEmpty())
            {
              vect.push_back(make_pair(item, hfa));
              ++HelpCommon::m_NumReadFromIni;
            }
        }
      }
    }

    hFile.Close();
  }
}

void HelpCommon::SaveHelpFilesVector(HelpCommon::HelpFilesVector &vect)
{
  ConfigManager* conf = Manager::Get()->GetConfigManager(wxT_2("help_plugin"));
  wxArrayString list = conf->EnumerateSubPaths(wxT_2("/"));

  for (unsigned int i = 0; i < list.GetCount(); ++i)
  {
    conf->DeleteSubPath(list[i]);
  }

  HelpFilesVector::iterator it;

  int count = 0;

  for (it = vect.begin(); it != vect.end(); ++it)
  {
    HelpFileAttrib hfa;
    wxString name = it->first;
    hfa = it->second;

    if (!name.IsEmpty() && !hfa.name.IsEmpty() && !hfa.readFromIni)
    {
      wxString key = wxString::Format(wxT_2("/help%d/"), count++);
      conf->Write(key + wxT_2("name"), name);
      conf->Write(key + wxT_2("file"), hfa.name);
      conf->Write(key + wxT_2("isexec"), hfa.isExecutable);
      conf->Write(key + wxT_2("embeddedviewer"), hfa.openEmbeddedViewer);
      // Patch by Yorgos Pagles: Write new attributes in settings
      conf->Write(key + wxT_2("keywordcase"), static_cast<int>(hfa.keywordCase));
      conf->Write(key + wxT_2("defaultkeyword"), hfa.defaultKeyword);
    }
  }

  conf->Write(wxT_2("/default"), m_DefaultHelpIndex);
}
