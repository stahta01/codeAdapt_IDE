///////////////////////////////////////////////////////////////////////////////
// Name:        makefont.cpp
// Purpose:     Utility for creating font metrics file for use with wxPdfDocument
// Author:      Ulrich Telle
// Modified by:
// Created:     2005-09-05
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// #include "wx/wx.h"
#endif

#include <wx/cmdline.h>
#include <wx/log.h>
#include <wx/dynarray.h>
#include <wx/tokenzr.h>
#include <wx/mstream.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/xml/xml.h>
#include <wx/zstream.h>

#include "wx/pdfdoc.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontparsertruetype.h"
#include "wx/pdffontdata.h"
#include "wx/pdffontdataopentype.h"
#include "wx/pdffontdatatruetype.h"
#include "wx/pdffontdatatype1.h"
#include "wx/pdffontmanager.h"

static int
CompareInts(int n1, int n2)
{
  return n1 - n2;
}

class GlyphListEntry
{
public:
  GlyphListEntry() {};
  ~GlyphListEntry() {};
  int m_gid;
  int m_uid;
};

WX_DEFINE_SORTED_ARRAY(GlyphListEntry*, GlyphList);

static int
CompareGlyphListEntries(GlyphListEntry* item1, GlyphListEntry* item2)
{
  return item1->m_gid - item2->m_gid;
}

WX_DECLARE_HASH_MAP(long, wxString, wxIntegerHash, wxIntegerEqual, CTGMap);
WX_DECLARE_STRING_HASH_MAP(wxString, FixGlyphMap);

class MakeFont : public wxAppConsole
{
public:
  bool OnInit();
  int OnRun();
  int OnExit();

protected:
  bool MakeFontAFM(const wxString& fontFileName, const wxString& afmFileName,
                   const wxString& encoding = wxT_2("cp1252"),
                   const wxString& type = wxT_2("TrueType"),
                   const wxString& patch = wxEmptyString);
#if wxUSE_UNICODE
  bool MakeFontUFM(const wxString& fontFileName,
                   const wxString& ufmFileName,
                   const wxString& type = wxT_2("TrueType"),
                   const wxString& volt = wxT_2(""));
  bool MakeFontImmediate(const wxString& fontFileName);

  void WriteStreamBuffer(wxOutputStream& stream, const char* buffer);
  void WriteToUnicode(GlyphList& glyphs, wxMemoryOutputStream& toUnicode);
#endif
  bool ReadMap(const wxString& enc, CTGMap& cc2gn);
  short ReadShort(wxInputStream* stream);
  int ReadInt(wxInputStream* stream);
  void CheckTTF(const wxString& fileName, 
                bool& embeddingAllowed, bool& subsettingAllowed, 
                int& cffOffset, int& cffLength);
  void CreateFontMetricsFile(const wxString& xmlFileName, wxPdfFontData& font, bool includeGlyphInfo);

private:
  wxString m_version;
  bool     m_unicode;
  bool     m_immediate;
  wxString m_afmFile;
  wxString m_ufmFile;
  wxString m_fontFile;
  wxString m_encoding;
  wxString m_patchFile;
  wxString m_fontType;
  wxString m_volt;
  wxString m_outputDir;
};

/// Fast string search (KMP method): initialization
static int*
makeFail(const char* target, int tlen)
{
  int t = 0;
  int s, m;
  m = tlen;
  int* f = new int[m+1];
  f[1] = 0;
  for (s = 1; s < m; s++)
  {
    while ((t > 0) && (target[s] != target[t]))
    {
      t = f[t];
    }
    if (target[t] == target[s])
    {
      t++;
      f[s+1] = t;
    }
    else
    {
      f[s+1] = 0;
    }
  }
  return f;
}

/// Fast string search (KMP method)
static int
findString(const char* src, int slen, const char* target, int tlen, int* f)
{
  int s = 0;
  int i;
  int m = tlen;
  for (i = 0; i < slen; i++)
  {
    while ( (s > 0) && (src[i] != target[s]))
    {
      s = f[s];
    }
    if (src[i] == target[s]) s++;
    if (s == m) return (i-m+1);
  }
  return -1;
}

/// Read encoding map
bool
MakeFont::ReadMap(const wxString& enc, CTGMap& cc2gn)
{
  //Read a map file
  wxString mapFileName = enc.Lower() + wxT_2(".map");
  wxFileInputStream mapFile(mapFileName);
  if (!mapFile.Ok())
  {
    wxLogMessage(wxT_2("Error: Unable to read map file '") + mapFileName + wxT_2("'."));
    return false;
  }

  wxTextInputStream text(mapFile);
  wxString charcode, unicode, glyphname;

  cc2gn.clear();
  while (!mapFile.Eof())
  {
    text >> charcode >> unicode >> glyphname;
    if (!mapFile.Eof() && charcode.Length() > 0)
    {
      if (charcode[0] == wxT_2('!'))
      {
        int cc = wxHexToDec(charcode.Mid(1));
        cc2gn[cc] = glyphname;
      }
    }
  }

  int i;
  for (i = 0; i <= 255; i++)
  {
    CTGMap::iterator iter = cc2gn.find(i);
    if (iter == cc2gn.end())
    {
      cc2gn[i] = wxT_2(".notdef");
    }
  }
  return true;
}

/// Read a 2-byte integer from file (big endian)
short
MakeFont::ReadShort(wxInputStream* stream)
{
  short i16;
  stream->Read(&i16, 2);
  return wxINT16_SWAP_ON_LE(i16);
}

/// Read a 4-byte integer from file (big endian)
int
MakeFont::ReadInt(wxInputStream* stream)
{
  int i32;
  stream->Read(&i32, 4);
  return wxINT32_SWAP_ON_LE(i32);
}

/// Check TrueType font file whether font license allows embedding
void
MakeFont::CheckTTF(const wxString& fileName, bool& embeddingAllowed, bool& subsettingAllowed, int& cffOffset, int& cffLength)
{
  embeddingAllowed = false;
  subsettingAllowed = false;
  cffOffset = -1;
  cffLength = 0;
  if (fileName.Length() == 0)
  {
    return;
  }
  wxFileInputStream ttfFile(fileName);
  if (!ttfFile.Ok())
  {
    // Can't open file
    wxLogMessage(wxT_2("Error: Unable to read font file '") + fileName + wxT_2("'."));
    return;
  }
  // Extract number of tables
  ttfFile.SeekI(0, wxFromCurrent);
  int id = ReadInt(&ttfFile);
  if (id != 0x00010000 && id != 0x4f54544f)
  {
    wxLogError(wxT_2("Error: File '") + fileName + wxT_2("' is not a valid font file."));
    return;
  }
  short nb = ReadShort(&ttfFile);
  ttfFile.SeekI(6, wxFromCurrent);
  // Seek OS/2 table
  bool found = false;
  int offset = 0;
  int i;
  for (i = 0; i < nb; i++)
  {
    char buffer[4];
    ttfFile.Read(buffer,4);
    if (strncmp(buffer,"OS/2",4) == 0)
    {
      found = true;
      ttfFile.SeekI(4, wxFromCurrent);
      offset = ReadInt(&ttfFile);
      ttfFile.SeekI(4, wxFromCurrent);
    }
    else if (strncmp(buffer,"CFF ",4) == 0)
    {
      ttfFile.SeekI(4, wxFromCurrent);
      cffOffset = ReadInt(&ttfFile);
      cffLength = ReadInt(&ttfFile);
    }
    else
    {
      ttfFile.SeekI(12, wxFromCurrent);
    }
  }
  if (!found)
  {
    return;
  }
  ttfFile.SeekI(offset, wxFromStart);
  // Extract fsType flags
  ttfFile.SeekI(8, wxFromCurrent);
  short fsType = ReadShort(&ttfFile);
  bool rl = (fsType & 0x02) != 0;
  bool pp = (fsType & 0x04) != 0;
  bool e  = (fsType & 0x08) != 0;
  bool ns = (fsType & 0x0100) != 0;
  bool eb = (fsType & 0x0200) != 0;
  embeddingAllowed = !(rl && !pp && !e) && !eb;
  subsettingAllowed = !ns;
}

/// Create wxPdfDocument font metrics file
void
MakeFont::CreateFontMetricsFile(const wxString& xmlFileName, wxPdfFontData& font, bool includeGlyphInfo)
{
  wxXmlDocument fontMetrics;
  const wxPdfFontDescription& fd = font.GetDescription();

  wxXmlNode* rootNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("wxpdfdoc-font-metrics"));
#if wxCHECK_VERSION(2,9,0)
  rootNode->AddAttribute(wxT_2("type"), font.GetType() /*wxT_2("TrueTypeUnicode")*/);
#else
  rootNode->AddProperty(wxT_2("type"), font.GetType() /*wxT_2("TrueTypeUnicode")*/);
#endif
  wxXmlNode* node;
  wxXmlNode* textNode;

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("font-name"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), font.GetName());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("encoding"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), font.GetEncoding());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  wxXmlNode* descNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("description"));

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("ascent"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetAscent()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("descent"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetDescent()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("cap-height"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetCapHeight()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("flags"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetFlags()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("font-bbox"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), fd.GetFontBBox());
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("italic-angle"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetItalicAngle()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("stem-v"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetStemV()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("missing-width"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetMissingWidth()));
  node->AddChild(textNode);
  descNode->AddChild(node);
  
  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("x-height"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetXHeight()));
  node->AddChild(textNode);
  descNode->AddChild(node);
  
  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("underline-position"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetUnderlinePosition()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("underline-thickness"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), wxString::Format(wxT_2("%d"), fd.GetUnderlineThickness()));
  node->AddChild(textNode);
  descNode->AddChild(node);

  rootNode->AddChild(descNode);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("diff"));
  textNode = new wxXmlNode(wxXML_TEXT_NODE, wxT_2("text"), font.GetDiffs());
  node->AddChild(textNode);
  rootNode->AddChild(node);

  node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("file"));
#if wxCHECK_VERSION(2,9,0)
  node->AddAttribute(wxT_2("name"), font.GetFontFile());
#else
  node->AddProperty(wxT_2("name"), font.GetFontFile());
#endif
  wxString fontType = font.GetType();
  if (fontType == wxT_2("TrueTypeUnicode") || fontType == wxT_2("OpenTypeUnicode"))
  {
    // TrueTypeUnicode (name,ctg,originalsize)
#if wxCHECK_VERSION(2,9,0)
    node->AddAttribute(wxT_2("ctg"), font.GetCtgFile());
    node->AddAttribute(wxT_2("originalsize"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#else
    node->AddProperty(wxT_2("ctg"), font.GetCtgFile());
    node->AddProperty(wxT_2("originalsize"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#endif
  }
  else if (fontType == wxT_2("TrueType"))
  {
    // Truetype (name, originalsize)
#if wxCHECK_VERSION(2,9,0)
    node->AddAttribute(wxT_2("originalsize"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#else
    node->AddProperty(wxT_2("originalsize"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#endif
  }
  else if  (fontType == wxT_2("Type1"))
  {
    // Type1 (name, size1, size2)
#if wxCHECK_VERSION(2,9,0)
    node->AddAttribute(wxT_2("size1"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#else
    node->AddProperty(wxT_2("size1"), wxString::Format(wxT_2("%d"), font.GetSize1()));
#endif
    if (font.HasSize2())
    {
#if wxCHECK_VERSION(2,9,0)
      node->AddAttribute(wxT_2("size2"), wxString::Format(wxT_2("%d"), font.GetSize2()));
#else
      node->AddProperty(wxT_2("size2"), wxString::Format(wxT_2("%d"), font.GetSize2()));
#endif
    }
  }
  rootNode->AddChild(node);

  wxXmlNode* widthsNode = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("widths"));
  if (includeGlyphInfo)
  {
#if wxCHECK_VERSION(2,9,0)
    widthsNode->AddAttribute(wxT_2("subsetting"), wxT_2("enabled"));
#else
    widthsNode->AddProperty(wxT_2("subsetting"), wxT_2("enabled"));
#endif
  }

  wxPdfGlyphWidthMap* widths = (wxPdfGlyphWidthMap*) font.GetGlyphWidthMap();
  wxPdfChar2GlyphMap* glyphs = (wxPdfChar2GlyphMap*) font.GetChar2GlyphMap();
  wxPdfSortedArrayInt idList(CompareInts);
  wxPdfGlyphWidthMap::iterator iter = widths->begin();
  for (iter = widths->begin(); iter != widths->end(); iter++)
  {
    idList.Add(iter->first);
  }
  size_t j;
  for (j = 0; j < idList.GetCount(); j++)
  {
    node = new wxXmlNode(wxXML_ELEMENT_NODE, wxT_2("char"));
#if wxCHECK_VERSION(2,9,0)
    node->AddAttribute(wxT_2("id"), wxString::Format(wxT_2("%d"), idList[j]));
#else
    node->AddProperty(wxT_2("id"), wxString::Format(wxT_2("%d"), idList[j]));
#endif
    if (includeGlyphInfo)
    {
      int glyph = (*glyphs)[idList[j]];
#if wxCHECK_VERSION(2,9,0)
      node->AddAttribute(wxT_2("gn"), wxString::Format(wxT_2("%d"), glyph));
#else
      node->AddProperty(wxT_2("gn"), wxString::Format(wxT_2("%d"), glyph));
#endif
    }
#if wxCHECK_VERSION(2,9,0)
    node->AddAttribute(wxT_2("width"), wxString::Format(wxT_2("%d"), (*widths)[idList[j]]));
#else
    node->AddProperty(wxT_2("width"), wxString::Format(wxT_2("%d"), (*widths)[idList[j]]));
#endif
    widthsNode->AddChild(node);
  }
  rootNode->AddChild(widthsNode);

  wxXmlDocument voltData;
  wxXmlNode* voltRoot = NULL;
  if (!m_volt.IsEmpty())
  {
    wxFileSystem fs;
    // Open volt data XML file
    wxFSFile* xmlVoltData = fs.OpenFile(wxFileSystem::FileNameToURL(m_volt));
    if (xmlVoltData != NULL)
    {
      // Load the XML file
      bool loaded = voltData.Load(*(xmlVoltData->GetStream()));
      delete xmlVoltData;
      if (loaded)
      {
        if (voltData.IsOk() && voltData.GetRoot()->GetName().IsSameAs(wxT_2("volt")))
        {
          voltRoot = voltData.GetRoot();
          rootNode->AddChild(voltRoot);
        }
      }
    }
  }

  fontMetrics.SetRoot(rootNode);
  fontMetrics.Save(xmlFileName);
  if (voltRoot != NULL)
  {
    rootNode->RemoveChild(voltRoot);
  }
}

/// Make wxPdfDocument font metrics file based on AFM file
bool
MakeFont::MakeFontAFM(const wxString& fontFileName, const wxString& afmFileName,
                      const wxString& encoding, const wxString& type, const wxString& patch)
{
  bool valid = false;
  wxString fontType = type;
  wxFileName fileName(fontFileName);
  // Find font type
  if (fontFileName.Length() > 0)
  {
    wxString ext = fileName.GetExt();
    if (ext == wxT_2("ttf"))
    {
      fontType = wxT_2("TrueType");
    }
    else if (ext == wxT_2("pfb"))
    {
      fontType = wxT_2("Type1");
    }
    else
    {
      wxLogMessage(wxT_2("Warning: Unrecognized font file extension (") + ext + wxT_2("), ") + fontType + wxT_2(" font assumed."));
    }
  }

  bool embeddingAllowed = true;
  bool subsettingAllowed = true;
  if (fontFileName.Length() > 0)
  {
    int cffOffset, cffLength;
    if (fontType == wxT_2("TrueType"))
    {
      CheckTTF(fontFileName, embeddingAllowed, subsettingAllowed, cffOffset, cffLength);
    }
  }

  wxPdfFontData* afmFont;
  if (fontType == wxT_2("TrueType"))
  {
    afmFont = new wxPdfFontDataTrueType();
  }
  else
  {
    afmFont = new wxPdfFontDataType1();
  }
  // Initialize font description
  wxPdfFontDescription fd;
  fd.SetAscent(1000);
  fd.SetDescent(-200);
  fd.SetItalicAngle(0);
  fd.SetStemV(70);
  fd.SetMissingWidth(600);
  fd.SetUnderlinePosition(-100);
  fd.SetUnderlineThickness(50);
  
  bool hasCapHeight = false;
  bool hasXCapHeight = false;
  bool hasXHeight = false;
  bool hasFontBBox = false;
  bool hasStemV = false;
  bool hasMissingWidth = false;
  int flags = 0;

  CTGMap cc2gn;
  CTGMap cc2gnPatch;
  if (encoding.Length() > 0)
  {
    afmFont->SetEncoding(encoding);
    valid = ReadMap(encoding, cc2gn);
    if (!valid)
    {
      delete afmFont;
      return false;
    }
    if (patch.Length() > 0)
    {
      valid = ReadMap(patch, cc2gnPatch);
      if (valid)
      {
        CTGMap::iterator patchIter;
        for (patchIter = cc2gnPatch.begin(); patchIter != cc2gnPatch.end(); patchIter++)
        {
          cc2gn[patchIter->first] = patchIter->second;
        }
      }
      else
      {
        wxLogMessage(wxT_2("Warning: Unable to read patch file '") + patch + wxT_2("'."));
      }
    }
  }

  // Read a font metric file
  wxFileInputStream afmFile(afmFileName);
  if (!afmFile.Ok())
  {
    wxLogMessage(wxT_2("Error: Unable to read AFM file '") + afmFileName + wxT_2("'."));
    delete afmFont;
    return false;
  }
  wxTextInputStream text(afmFile);

  FixGlyphMap fix;

  fix[wxT_2("Edot")] = wxT_2("Edotaccent");
  fix[wxT_2("edot")] = wxT_2("edotaccent");
  fix[wxT_2("Idot")] = wxT_2("Idotaccent");
  fix[wxT_2("Zdot")] = wxT_2("Zdotaccent");
  fix[wxT_2("zdot")] = wxT_2("zdotaccent");
  fix[wxT_2("Odblacute")] = wxT_2("Ohungarumlaut");
  fix[wxT_2("odblacute")] = wxT_2("ohungarumlaut");
  fix[wxT_2("Udblacute")] = wxT_2("Uhungarumlaut");
  fix[wxT_2("udblacute")] = wxT_2("uhungarumlaut");
  fix[wxT_2("Gcedilla")] = wxT_2("Gcommaaccent");
  fix[wxT_2("gcedilla")] = wxT_2("gcommaaccent");
  fix[wxT_2("Kcedilla")] = wxT_2("Kcommaaccent");
  fix[wxT_2("kcedilla")] = wxT_2("kcommaaccent");
  fix[wxT_2("Lcedilla")] = wxT_2("Lcommaaccent");
  fix[wxT_2("lcedilla")] = wxT_2("lcommaaccent");
  fix[wxT_2("Ncedilla")] = wxT_2("Ncommaaccent");
  fix[wxT_2("ncedilla")] = wxT_2("ncommaaccent");
  fix[wxT_2("Rcedilla")] = wxT_2("Rcommaaccent");
  fix[wxT_2("rcedilla")] = wxT_2("rcommaaccent");
  fix[wxT_2("Scedilla")] = wxT_2("Scommaaccent");
  fix[wxT_2("scedilla")] = wxT_2("scommaaccent");
  fix[wxT_2("Tcedilla")] = wxT_2("Tcommaaccent");
  fix[wxT_2("tcedilla")] = wxT_2("tcommaaccent");
  fix[wxT_2("Dslash")] = wxT_2("Dcroat");
  fix[wxT_2("dslash")] = wxT_2("dcroat");
  fix[wxT_2("Dmacron")] = wxT_2("Dcroat");
  fix[wxT_2("dmacron")] = wxT_2("dcroat");
  fix[wxT_2("combininggraveaccent")] = wxT_2("gravecomb");
  fix[wxT_2("combininghookabove")] = wxT_2("hookabovecomb");
  fix[wxT_2("combiningtildeaccent")] = wxT_2("tildecomb");
  fix[wxT_2("combiningacuteaccent")] = wxT_2("acutecomb");
  fix[wxT_2("combiningdotbelow")] = wxT_2("dotbelowcomb");
  fix[wxT_2("dongsign")] = wxT_2("dong");

  // Read the AFM font metric file
  wxPdfGlyphWidthMap* widths = new wxPdfGlyphWidthMap();
  wxPdfChar2GlyphMap* glyphs = new wxPdfChar2GlyphMap();
  long cc;
  if (!cc2gn.empty())
  {
    for (cc = 0; cc <= 255; cc++)
    {
      (*widths)[cc] = 0xFFFF;
      (*glyphs)[cc] = 0;
    }
  }

  wxString line;
  wxString charcode, glyphname;
  wxString code, param, dummy, glyph;
  wxString token, tokenBoxHeight;
  long nParam, incWidth = -1, incGlyph = 0;
  long width, boxHeight, glyphNumber;
  wxString weight;
  bool hasGlyphNumbers = false;
  while (!afmFile.Eof())
  {
    line = text.ReadLine();
    line.Trim();

    wxStringTokenizer tkz(line, wxT_2(" "));
    int count = (int) tkz.CountTokens();
    if (count < 2) continue;
    code  = tkz.GetNextToken(); // 0
    param = tkz.GetNextToken(); // 1
    if (code == wxT_2("C"))
    {
      width = -1;
      glyphNumber = 0;
      boxHeight = 0;
      tokenBoxHeight = wxEmptyString;
      // Character metrics
      param.ToLong(&cc);
      dummy = tkz.GetNextToken(); // 2
      while (tkz.HasMoreTokens())
      {
        token = tkz.GetNextToken();
        if (token == wxT_2("WX")) // Character width
        {
          param = tkz.GetNextToken(); // Width
          param.ToLong(&width);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("N")) // Glyph name
        {
          glyphname = tkz.GetNextToken(); // Glyph name
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("G")) // Glyph number
        {
          hasGlyphNumbers = true;
          param = tkz.GetNextToken(); // Number
          param.ToLong(&glyphNumber);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("B")) // Character bounding box
        {
          dummy = tkz.GetNextToken(); // x left
          dummy = tkz.GetNextToken(); // y bottom
          dummy = tkz.GetNextToken(); // x right
          tokenBoxHeight = tkz.GetNextToken(); // y top
          tokenBoxHeight.ToLong(&boxHeight);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else
        {
          while (tkz.HasMoreTokens() && tkz.GetNextToken() != wxT_2(";"));
        }
      }

      if (glyphname.Right(4) == wxT_2("20AC"))
      {
        glyphname = wxT_2("Euro");
      }
      if (glyphname == wxT_2("increment"))
      {
        incWidth = width;
        if (hasGlyphNumbers)
        {
          incGlyph = glyphNumber;
        }
      }
      if (fix.find(glyphname) != fix.end())
      {
        //Fix incorrect glyph name
        for (int i = 0; i <= 255; i++)
        {
          if (cc2gn[i] == fix[glyphname])
          {
            cc2gn[i] = glyphname;
          }
        }
      }
      if (cc2gn.empty())
      {
        // Symbolic font: use built-in encoding
        (*widths)[cc] = width;
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = glyphNumber;
        }
        if (!hasCapHeight && !hasXCapHeight && cc == 'X' && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && cc == 'x' && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }
      }
      else
      {
        // Search glyphname in cc2gn
        bool found = false;
        for (int i = 0; i <= 255; i++)
        {
          if (cc2gn[i] == glyphname)
          {
            found = true;
            (*widths)[i] = width;
            if (hasGlyphNumbers)
            {
              (*glyphs)[i] = glyphNumber;
            }
          }
        }
        if (!found)
        {
          wxLogMessage(wxT_2("Warning: character '") + glyphname + wxT_2("' is undefined."));
        }
        if (!hasCapHeight && !hasXCapHeight && glyphname == wxT_2("X") && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && glyphname == wxT_2("x") && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }
      }
      if (!hasMissingWidth && glyphname == wxT_2(".notdef"))
      {
        hasMissingWidth = true;
        fd.SetMissingWidth(width);
      }
    }
    else if (code == wxT_2("FontName"))
    {
      afmFont->SetName(param);
    }
    else if (code == wxT_2("Weight"))
    {
      wxString weight = param.Lower();
      if (!hasStemV && (weight == wxT_2("black") || weight == wxT_2("bold")))
      {
        fd.SetStemV(120);
      }
    }
    else if (code == wxT_2("ItalicAngle"))
    {
      double italic;
      param.ToDouble(&italic);
      int italicAngle = int(italic);
      fd.SetItalicAngle(italicAngle);
      if (italicAngle > 0)
      {
        flags += 1 << 6;
      }
    }
    else if (code == wxT_2("Ascender"))
    {
      long ascent;
      param.ToLong(&ascent);
      fd.SetAscent(ascent);
    }
    else if (code == wxT_2("Descender"))
    {
      param.ToLong(&nParam);
      fd.SetDescent(nParam);
    }
    else if (code == wxT_2("UnderlineThickness"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlineThickness(nParam);
    }
    else if (code == wxT_2("UnderlinePosition"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlinePosition(nParam);
    }
    else if (code == wxT_2("IsFixedPitch"))
    {
      if (param == wxT_2("true"))
      {
        flags += 1 << 0;
      }
    }
    else if (code == wxT_2("FontBBox"))
    {
      hasFontBBox = true;
      wxString bbox2 = tkz.GetNextToken();
      wxString bbox3 = tkz.GetNextToken();
      wxString bbox4 = tkz.GetNextToken();
      wxString bBox = wxT_2("[") + param + wxT_2(" ") + bbox2 + wxT_2(" ") + bbox3 + wxT_2(" ") + bbox4 + wxT_2("]");
      fd.SetFontBBox(bBox);
    }
    else if (code == wxT_2("CapHeight"))
    {
      hasCapHeight = true;
      long capHeight;
      param.ToLong(&capHeight);
      fd.SetCapHeight(capHeight);
    }
    else if (code == wxT_2("StdVW"))
    {
      hasStemV = true;
      long stemV;
      param.ToLong(&stemV);
      fd.SetStemV(stemV);
    }
  }

  for (cc = 0; cc <= 255; cc++)
  {
    if ((*widths)[cc] == 0xFFFF)
    {
      if (cc2gn[cc] == wxT_2("Delta") && incWidth >= 0)
      {
        (*widths)[cc] = incWidth;
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = incGlyph;
        }
      }
      else
      {
        (*widths)[cc] = fd.GetMissingWidth();
        if (hasGlyphNumbers)
        {
          (*glyphs)[cc] = 0;
        }
        wxLogMessage(wxT_2("Warning: character '") + cc2gn[cc] + wxT_2("' is missing."));
      }
    }
  }

  wxString diffs;
  if (encoding.Length() > 0)
  {
    // Build differences from reference encoding
    CTGMap cc2gnRef;
    valid = ReadMap(wxT_2("cp1252"), cc2gnRef);
    diffs = wxT_2("");
    int last = 0;
    for (int i = 32; i <= 255; i++)
    {
      if (cc2gn[i] != cc2gnRef[i])
      {
        if (i != last+1)
        {
          diffs += wxString::Format(wxT_2("%d "), i);
        }
        last = i;
        diffs = diffs + wxT_2("/") + cc2gn[i] + wxT_2(" ");
      }
    }
  }
  afmFont->SetDiffs(diffs);  
  
  if (cc2gn.empty())
  {
    flags += 1 << 2;
  }
  else
  {
    flags += 1 << 5;
  }
  fd.SetFlags(flags);
  if (!hasCapHeight && !hasXCapHeight)
  {
    fd.SetCapHeight(fd.GetAscent());
  }
  if (!hasFontBBox)
  {
    wxString fbb = wxString::Format(wxT_2("[0 %d 1000 %d]"), fd.GetDescent()-100, fd.GetAscent()+100);
    fd.SetFontBBox(fbb);
  }

  afmFont->SetGlyphWidthMap(widths);
  afmFont->SetChar2GlyphMap(glyphs);

  afmFont->SetDescription(fd);

  wxString baseName = fileName.GetName();
  if (embeddingAllowed)
  {
    if (fontType == wxT_2("TrueType"))
    {
      wxFileInputStream ttfFile(fontFileName);
      if (!ttfFile.Ok())
      {
        wxLogMessage(wxT_2("Error: Unable to read font file '") + fontFileName + wxT_2("'."));
        delete afmFont;
        return false;
      }
      size_t len = ttfFile.GetLength();
      wxFileOutputStream outFontFile(m_outputDir + baseName + wxT_2(".z"));
      wxZlibOutputStream zOutFontFile(outFontFile);
      zOutFontFile.Write(ttfFile);
      zOutFontFile.Close();
      afmFont->SetFontFile(baseName + wxT_2(".z"));
      afmFont->SetSize1(len);
      wxLogMessage(wxT_2("Font file compressed (") + baseName + wxT_2(".z)."));
    }
    else
    {
      wxFileInputStream pfbFile(fontFileName);
      if (!pfbFile.Ok())
      {
        wxLogMessage(wxT_2("Error: Unable to read font file '") + fontFileName + wxT_2("'."));
        delete afmFont;
        return false;
      }
      size_t len = pfbFile.GetLength();
      // Find first two sections and discard third one
      unsigned char* buffer = new unsigned char[len];
      unsigned char* buf1 = buffer;
      unsigned char* buf2;
      pfbFile.Read(buffer, len);
      unsigned char first = buffer[0];
      if (first == 128)
      {
        // Strip first binary header
        buf1 += 6;
        len -= 6;
      }
      int* f = makeFail("eexec",5);
      int size1 = (int) findString((char*) buf1, len, "eexec", 5, f);
      delete [] f;

      int size2 = -1;
      if (size1 >= 0)
      {
        size1 += 6;
        unsigned char second = buf1[size1];
        buf2 = buf1 + size1;
        len -= size1;
        if (first == 128 && second == 128)
        {
          // Strip second binary header
          buf2 += 6;
          len -= 6;
        }
        f = makeFail("00000000",8);
        size2 = (int) findString((char*) buf2, len, "00000000", 8, f);
        delete [] f;
        if (size2 >= 0)
        {
          wxFileOutputStream outFontFile(m_outputDir + baseName + wxT_2(".z"));
          wxZlibOutputStream zOutFontFile(outFontFile);
          zOutFontFile.Write(buf1, size1);
          zOutFontFile.Write(buf2, size2);
          afmFont->SetFontFile(baseName + wxT_2(".z"));
          afmFont->SetSize1(size1);
          afmFont->SetSize2(size2);
        }
      }
      delete [] buffer;
      if (size1 < 0 || size2 < 0)
      {
        wxLogMessage(wxT_2("Warning: Font file does not seem to be valid Type1, font embedding not possible."));
      }
    }
  }
  else
  {
    if (fontFileName.Length() > 0)
    {
      wxLogMessage(wxT_2("Warning: Font license does not allow embedding."));
    }
    else
    {
      wxLogMessage(wxT_2("Warning: Font file name missing, font embedding not possible."));
    }
  }

  // Create XML file
  CreateFontMetricsFile(m_outputDir + baseName + wxT_2(".xml"), *afmFont, hasGlyphNumbers);
  wxLogMessage(wxT_2("Font definition file generated (") + baseName + wxT_2(".xml)."));

  //delete widths;
  delete afmFont;
  return true;
}

#if wxUSE_UNICODE

void
MakeFont::WriteStreamBuffer(wxOutputStream& stream, const char* buffer)
{
  size_t buflen = strlen(buffer);
  stream.Write(buffer, buflen);
}

void
MakeFont::WriteToUnicode(GlyphList& glyphs, wxMemoryOutputStream& toUnicode)
{
  WriteStreamBuffer(toUnicode, "/CIDInit /ProcSet findresource begin\n");
  WriteStreamBuffer(toUnicode, "12 dict begin\n");
  WriteStreamBuffer(toUnicode, "begincmap\n");
  WriteStreamBuffer(toUnicode, "/CIDSystemInfo\n");
  WriteStreamBuffer(toUnicode, "<< /Registry (Adobe)\n");
  WriteStreamBuffer(toUnicode, "/Ordering (UCS)\n");
  WriteStreamBuffer(toUnicode, "/Supplement 0\n");
  WriteStreamBuffer(toUnicode, ">> def\n");
  WriteStreamBuffer(toUnicode, "/CMapName /Adobe-Identity-UCS def\n");
  WriteStreamBuffer(toUnicode, "/CMapType 2 def\n");
  WriteStreamBuffer(toUnicode, "1 begincodespacerange\n");
  WriteStreamBuffer(toUnicode, "<0000><FFFF>\n");
  WriteStreamBuffer(toUnicode, "endcodespacerange\n");
  int size = 0;
  size_t k;
  size_t numGlyphs = glyphs.GetCount();
  for (k = 0; k < numGlyphs; ++k)
  {
    if (size == 0)
    {
      if (k != 0)
      {
        WriteStreamBuffer(toUnicode, "endbfrange\n");
      }
      size = (numGlyphs-k > 100) ? 100 : numGlyphs - k;
      wxString sizeStr = wxString::Format(wxT_2("%d"), size);
      WriteStreamBuffer(toUnicode, sizeStr.ToAscii());
      WriteStreamBuffer(toUnicode, " beginbfrange\n");
    }
    size--;
    GlyphListEntry* entry = glyphs[k];
    wxString fromTo = wxString::Format(wxT_2("<%04x>"), entry->m_gid);
    wxString uniChr = wxString::Format(wxT_2("<%04x>"), entry->m_uid);
    WriteStreamBuffer(toUnicode, fromTo.ToAscii());
    WriteStreamBuffer(toUnicode, fromTo.ToAscii());
    WriteStreamBuffer(toUnicode, uniChr.ToAscii());
    WriteStreamBuffer(toUnicode, "\n");
  }
  WriteStreamBuffer(toUnicode, "endbfrange\n");
  WriteStreamBuffer(toUnicode, "endcmap\n");
  WriteStreamBuffer(toUnicode, "CMapName currentdict /CMap defineresource pop\n");
  WriteStreamBuffer(toUnicode, "end end\n");
}

/// Make wxPdfDocument font metrics file based on UFM file
bool
MakeFont::MakeFontUFM(const wxString& fontFileName,
                      const wxString& ufmFileName,
                      const wxString& type,
                      const wxString& volt)
{
  bool hasVolt = !volt.IsEmpty();
  int paBase = 0xe000;
  bool cff = false;
  GlyphList glyphList(CompareGlyphListEntries);
  static size_t CC2GNSIZE = 131072; // 2*64kB
  wxFileName fileName(ufmFileName);
  bool embeddingAllowed = false;
  bool subsettingAllowed = false;
  int cffOffset = -1;
  int cffLength = 0;
  if (fontFileName.Length() > 0)
  {
    CheckTTF(fontFileName, embeddingAllowed, subsettingAllowed, cffOffset, cffLength);
    cff = cffOffset > 0;
  }
  else
  {
    cff = (type == wxT_2("OpenType"));
  }

  wxPdfFontData* ufmFont;
  if (cff)
  {
    ufmFont = new wxPdfFontDataOpenTypeUnicode();
  }
  else
  {
    ufmFont = new wxPdfFontDataTrueTypeUnicode();
  }

  // Initialize font description
  wxPdfFontDescription fd;
  fd.SetAscent(1000);
  fd.SetDescent(-200);
  fd.SetItalicAngle(0);
  fd.SetStemV(70);
  fd.SetMissingWidth(600);
  bool hasCapHeight = false;
  bool hasXCapHeight = false;
  bool hasXHeight = false;
  bool hasFontBBox = false;
  bool hasStemV = false;
  bool hasMissingWidth = false;
  int flags = 0;

  wxFileInputStream ufmFile(ufmFileName);
  if (!ufmFile.Ok())
  {
    wxLogMessage(wxT_2("Error: Unable to read UFM file '") + ufmFileName + wxT_2("'."));
    return false;
  }
  wxTextInputStream text(ufmFile);

  // Prepare empty CIDToGIDMap
  unsigned char* cc2gn = new unsigned char[131072];
  size_t j;
  for (j = 0; j < CC2GNSIZE; j++)
  {
    cc2gn[j] = '\0';
  }
  
  // Read the UFM font metric file
  wxPdfGlyphWidthMap* widths = new wxPdfGlyphWidthMap();
  wxPdfChar2GlyphMap* glyphs = new wxPdfChar2GlyphMap();

  wxString line;
  wxString charcode, glyphname;
  wxString code, param, dummy, glyph;
  wxString token, tokenBoxHeight;
  long nParam, width, glyphNumber, boxHeight;
  wxString weight;
  while (!ufmFile.Eof())
  {
    line = text.ReadLine();
    line.Trim();

    wxStringTokenizer tkz(line, wxT_2(" "));
    int count = (int) tkz.CountTokens();
    if (count < 2) continue;
    code  = tkz.GetNextToken(); // 0
    param = tkz.GetNextToken(); // 1
    if (code == wxT_2("U"))
    {
      long cc;
      param.ToLong(&cc); // Character code
      width = -1;
      glyphNumber = 0;
      boxHeight = 0;
      tokenBoxHeight = wxEmptyString;
      dummy = tkz.GetNextToken(); // Semicolon
      while (tkz.HasMoreTokens())
      {
        token = tkz.GetNextToken();
        if (token == wxT_2("WX")) // Character width
        {
          param = tkz.GetNextToken(); // Width
          param.ToLong(&width);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("N")) // Glyph name
        {
          glyphname = tkz.GetNextToken(); // Glyph name
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("G")) // Glyph number
        {
          param = tkz.GetNextToken(); // Number
          param.ToLong(&glyphNumber);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else if (token == wxT_2("B")) // Character bounding box
        {
          dummy = tkz.GetNextToken(); // x left
          dummy = tkz.GetNextToken(); // y bottom
          dummy = tkz.GetNextToken(); // x right
          tokenBoxHeight = tkz.GetNextToken(); // y top
          tokenBoxHeight.ToLong(&boxHeight);
          dummy = tkz.GetNextToken(); // Semicolon
        }
        else
        {
          while (tkz.HasMoreTokens() && tkz.GetNextToken() != wxT_2(";"));
        }
      }
      if (cc != -1 || (hasVolt && glyphNumber > 0))
      {
        if (!hasCapHeight && !hasXCapHeight && cc == 'X' && tokenBoxHeight != wxEmptyString)
        {
          hasXCapHeight = true;
          fd.SetCapHeight(boxHeight);
        }
        if (!hasXHeight && cc == 'x' && tokenBoxHeight != wxEmptyString)
        {
          hasXHeight = true;
          fd.SetXHeight(boxHeight);
        }

        if (cc == -1)
        {
          cc = paBase++;
        }
        (*widths)[cc] = width;
        (*glyphs)[cc] = glyphNumber;
        // Set GID
        if (cc >= 0 && cc < 0xFFFF)
        {
          cc2gn[2*cc]   = (glyphNumber >> 8) & 0xFF;
          cc2gn[2*cc+1] =  glyphNumber       & 0xFF;
        }
        if (cff)
        {
          GlyphListEntry* glEntry = new GlyphListEntry();
          glEntry->m_gid = glyphNumber;
          glEntry->m_uid = cc;
          glyphList.Add(glEntry);
        }
      }
      if (!hasMissingWidth && glyphname == wxT_2(".notdef"))
      {
        hasMissingWidth = true;
        fd.SetMissingWidth(width);
      }
    }
    else if (code == wxT_2("FontName"))
    {
      ufmFont->SetName(param);
    }
    else if (code == wxT_2("Weight"))
    {
      wxString weight = param.Lower();
      if (!hasStemV && (weight == wxT_2("black") || weight == wxT_2("bold")))
      {
        fd.SetStemV(120);
      }
    }
    else if (code == wxT_2("ItalicAngle"))
    {
      double italic;
      param.ToDouble(&italic);
      int italicAngle = int(italic);
      fd.SetItalicAngle(italicAngle);
      if (italicAngle > 0)
      {
        flags += 1 << 6;
      }
    }
    else if (code == wxT_2("Ascender"))
    {
      long ascent;
      param.ToLong(&ascent);
      fd.SetAscent(ascent);
    }
    else if (code == wxT_2("Descender"))
    {
      param.ToLong(&nParam);
      fd.SetDescent(nParam);
    }
    else if (code == wxT_2("UnderlineThickness"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlineThickness(nParam);
    }
    else if (code == wxT_2("UnderlinePosition"))
    {
      param.ToLong(&nParam);
      fd.SetUnderlinePosition(nParam);
    }
    else if (code == wxT_2("IsFixedPitch"))
    {
      if (param == wxT_2("true"))
      {
        flags += 1 << 0;
      }
    }
    else if (code == wxT_2("FontBBox"))
    {
      hasFontBBox = true;
      wxString bbox2 = tkz.GetNextToken();
      wxString bbox3 = tkz.GetNextToken();
      wxString bbox4 = tkz.GetNextToken();
      wxString bBox = wxT_2("[") + param + wxT_2(" ") + bbox2 + wxT_2(" ") + bbox3 + wxT_2(" ") + bbox4 + wxT_2("]");
      fd.SetFontBBox(bBox);
    }
    else if (code == wxT_2("CapHeight"))
    {
      hasCapHeight = true;
      long capHeight;
      param.ToLong(&capHeight);
      fd.SetCapHeight(capHeight);
    }
    else if (code == wxT_2("StdVW"))
    {
      hasStemV = true;
      long stemV;
      param.ToLong(&stemV);
      fd.SetStemV(stemV);
    }
  }
  flags += 1 << 5;
  fd.SetFlags(flags);
  if (!hasCapHeight && !hasXCapHeight)
  {
    fd.SetCapHeight(fd.GetAscent());
  }
  if (!hasFontBBox)
  {
    hasFontBBox = true;
    wxString fbb = wxString::Format(wxT_2("[0 %d 1000 %d]"), fd.GetDescent()-100, fd.GetAscent()+100);
    fd.SetFontBBox(fbb);
  }

  ufmFont->SetGlyphWidthMap(widths);
  ufmFont->SetChar2GlyphMap(glyphs);

  ufmFont->SetDescription(fd);

  wxString baseName = fileName.GetName();
  if (embeddingAllowed)
  {
    wxFileInputStream fontFile(fontFileName);
    if (!fontFile.Ok())
    {
      wxLogMessage(wxT_2("Error: Unable to read font file '") + fontFileName + wxT_2("'."));
      delete [] cc2gn;
      return false;
    }
    size_t len = fontFile.GetLength();
    wxFileOutputStream outFontFile(m_outputDir + baseName + wxT_2(".z"));
    wxZlibOutputStream zOutFontFile(outFontFile);
    if (cff)
    {
      char* buffer = new char[cffLength];
      fontFile.SeekI(cffOffset);
      fontFile.Read(buffer, cffLength);
      zOutFontFile.Write(buffer, cffLength);
      delete [] buffer;
    }
    else
    {
      zOutFontFile.Write(fontFile);
    }
    zOutFontFile.Close();
    ufmFont->SetFontFile(baseName + wxT_2(".z"));
    ufmFont->SetSize1(len);
    wxLogMessage(wxT_2("Font file compressed (") + baseName + wxT_2(".z)."));
  }
  else
  {
    if (fontFileName.Length() > 0)
    {
      wxLogMessage(wxT_2("Warning: Font license does not allow embedding."));
    }
    else
    {
      wxLogMessage(wxT_2("Warning: Font file name missing, font embedding not possible."));
    }
  }

  if (embeddingAllowed)
  {
    // Create CID to GID map file
    wxFileOutputStream outCtgFile(m_outputDir + baseName + wxT_2(".ctg.z"));
    wxZlibOutputStream zOutCtgFile(outCtgFile);
    if (cff)
    {
      wxMemoryOutputStream toUnicode;
      WriteToUnicode(glyphList, toUnicode);
      wxMemoryInputStream inUnicode(toUnicode);
      zOutCtgFile.Write(inUnicode);
    }
    else
    {
#if 0
// Debug
      size_t j;
      for (j = 0; j < CC2GNSIZE; j+=2)
      {
        wxLogMessage(wxT_2("C %ld G %ld"), j/2, cc2gn[j] * 256 + cc2gn[j+1]);
      }
#endif
      zOutCtgFile.Write(cc2gn, CC2GNSIZE);
    }
    zOutCtgFile.Close();
    ufmFont->SetCtgFile(baseName + wxT_2(".ctg.z"));
    wxLogMessage(wxT_2("CIDToGIDMap created and compressed (") + baseName + wxT_2(".ctg.z)"));
  }

  // Create XML file
  CreateFontMetricsFile(m_outputDir + baseName + wxT_2(".xml"), *ufmFont, cff);
  wxLogMessage(wxT_2("Font definition file generated (") + baseName + wxT_2(".xml)."));

  if (cff)
  {
    WX_CLEAR_ARRAY(glyphList);
  }
  //delete widths;
  delete [] cc2gn;
  delete ufmFont;

  return true;
}

/// Make wxPdfDocument font metrics file immediately from font file
bool
MakeFont::MakeFontImmediate(const wxString& fontFileName)
{
  wxFileName fileName(fontFileName);

  wxFileInputStream fontStream(fontFileName);
  if (!fontStream.Ok())
  {
    wxLogMessage(wxT_2("Error: Unable to read font file '") + fontFileName + wxT_2("'."));
    return false;
  }
  size_t len = fontStream.GetLength();

  int cffOffset, cffLength;
  bool embeddingAllowed = false; 
  bool subsettingAllowed = false;
  CheckTTF(fontFileName, embeddingAllowed, subsettingAllowed, cffOffset, cffLength);
  bool cff = cffOffset > 0;

  wxPdfFontParserTrueType fontParser;
  wxPdfFontData* loadedFont = fontParser.IdentifyFont(fileName.GetFullName(), 0);
  if (loadedFont != NULL && fontParser.LoadFontData(loadedFont))
  {
    wxString baseName = fileName.GetName();
    if (loadedFont->EmbedSupported())
    {
      wxFileOutputStream outFontFile(m_outputDir + baseName + wxT_2(".z"));
      loadedFont->WriteFontData(&outFontFile);
      outFontFile.Close();
      wxLogMessage(wxT_2("Font file compressed (") + baseName + wxT_2(".z)"));
      wxFileOutputStream outCtgFile(m_outputDir + baseName + wxT_2(".ctg.z"));
      if (!cff)
      {
        static size_t CC2GNSIZE = 131072; // 2*64kB
        unsigned char* cc2gn = new unsigned char[131072];
        size_t j;
        for (j = 0; j < CC2GNSIZE; j++)
        {
          cc2gn[j] = '\0';
        }
        const wxPdfChar2GlyphMap* ctgMap = loadedFont->GetChar2GlyphMap();
        wxPdfChar2GlyphMap::const_iterator ccIter;
        for (ccIter = ctgMap->begin(); ccIter != ctgMap->end(); ccIter++)
        {
          wxUint32 cc = ccIter->first;
          wxUint32 gn = ccIter->second;
          if (cc >= 0 && cc < 0xFFFF)
          {
            cc2gn[2*cc]   = (gn >> 8) & 0xFF;
            cc2gn[2*cc+1] =  gn       & 0xFF;
          }
        }
        wxZlibOutputStream zOutCtgFile(outCtgFile);
        zOutCtgFile.Write(cc2gn, CC2GNSIZE);
      }
      else
      {
        loadedFont->WriteUnicodeMap(&outCtgFile);
      }
      outCtgFile.Close();
      wxLogMessage(wxT_2("CIDToGIDMap file compressed (") + baseName + wxT_2(".ctg.z)"));
      loadedFont->SetFontFile(baseName + wxT_2(".z"));
      loadedFont->SetSize1(len);
      loadedFont->SetCtgFile(baseName + wxT_2(".ctg.z"));
    }
    else
    {
      loadedFont->SetFontFile(wxT_2(""));
      loadedFont->SetCtgFile(wxT_2(""));
      wxLogMessage(wxT_2("Warning: Font license does not allow embedding."));
    }

    // Create XML file
    CreateFontMetricsFile(m_outputDir + baseName + wxT_2(".xml"), *loadedFont, cff);
    wxLogMessage(wxT_2("Font definition file generated (") + baseName + wxT_2(".xml)."));
    delete loadedFont;
  }

  return true;
}

#endif // wxUSE_UNICODE

static const wxCmdLineEntryDesc cmdLineDesc[] =
{
#if wxCHECK_VERSION(2,9,0)
  { wxCMD_LINE_OPTION, "a", "afm",       "Adobe Font Metric file (AFM)",                    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#if wxUSE_UNICODE
  { wxCMD_LINE_OPTION, "u", "ufm",       "Unicode Font Metric file (UFM)",                  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, "i", "immediate", "Extract font metrics from ttf/otf font file",     wxCMD_LINE_VAL_NONE,   wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "f", "font",      "Font file (ttf, otf or pfb)",                     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "v", "volt",      "Visual order layout table",                       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#else
  { wxCMD_LINE_OPTION, "f", "font",      "Font file (ttf or pfb)",                          wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
  { wxCMD_LINE_OPTION, "e", "enc",       "Character encoding of the font (for AFM only)",   wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "p", "patch",     "Patch file (for AFM only)",                       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "t", "type",      "Font type (type = otf, ttf or t1, default: ttf)", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, "o", "output",    "Path to output directory",                        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#else
  { wxCMD_LINE_OPTION, wxT_2("a"), wxT_2("afm"),       wxT_2("Adobe Font Metric file (AFM)"),                    wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#if wxUSE_UNICODE
  { wxCMD_LINE_OPTION, wxT_2("u"), wxT_2("ufm"),       wxT_2("Unicode Font Metric file (UFM)"),                  wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_SWITCH, wxT_2("i"), wxT_2("immediate"), wxT_2("Extract font metrics from ttf/otf font file"),     wxCMD_LINE_VAL_NONE,   wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxT_2("f"), wxT_2("font"),      wxT_2("Font file (ttf, otf or pfb)"),                     wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxT_2("v"), wxT_2("volt"),      wxT_2("Visual order layout table"),                       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#else
  { wxCMD_LINE_OPTION, wxT_2("f"), wxT_2("font"),      wxT_2("Font file (ttf or pfb)"),                          wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
  { wxCMD_LINE_OPTION, wxT_2("e"), wxT_2("enc"),       wxT_2("Character encoding of the font (for AFM only)"),   wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxT_2("p"), wxT_2("patch"),     wxT_2("Patch file (for AFM only)"),                       wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxT_2("t"), wxT_2("type"),      wxT_2("Font type (type = otf, ttf or t1, default: ttf)"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
  { wxCMD_LINE_OPTION, wxT_2("o"), wxT_2("output"),    wxT_2("Path to output directory"),                        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
#endif
  { wxCMD_LINE_NONE }
};

bool
MakeFont::OnInit()
{
  // Set the font path and working directory
  wxFileName exePath = wxStandardPaths::Get().GetExecutablePath();
  wxString fontPath = exePath.GetPathWithSep() + wxT_2("../lib/fonts");
  wxString cwdPath  = exePath.GetPath();
  wxPdfFontManager::GetFontManager()->AddSearchPath(fontPath);
  wxSetWorkingDirectory(cwdPath);

  m_version = wxT_2("1.6.1 (March 2013)");
  bool valid = false;
  //gets the parameters from cmd line
  wxCmdLineParser parser (cmdLineDesc, argc, argv);
  wxString logo = wxT_2("wxPdfDocument MakeFont Utility Version ") + m_version + 
                  wxT_2("\n\nCreate wxPdfDocument font support files.\n") +
                  wxT_2("Please specify file extensions in ALL file name parameters.\n") +
                  wxT_2("Select exactly one method to provide font metrics.\n");
  parser.SetLogo(logo);
  if (parser.Parse() == 0)
  {
    bool hasAfm       = parser.Found(wxT_2("afm"),    &m_afmFile);
    bool hasUfm       = parser.Found(wxT_2("ufm"),    &m_ufmFile);
    bool hasImmediate = parser.Found(wxT_2("immediate"));
    bool hasFont      = parser.Found(wxT_2("font"),   &m_fontFile);
    bool hasEnc       = parser.Found(wxT_2("enc"),    &m_encoding);
    bool hasPatch     = parser.Found(wxT_2("patch"),  &m_patchFile);
    bool hasType      = parser.Found(wxT_2("type"),   &m_fontType);
    bool hasVolt      = parser.Found(wxT_2("volt"),   &m_volt);
    bool hasOutput    = parser.Found(wxT_2("output"), &m_outputDir);

#if wxUSE_UNICODE
    m_unicode = hasUfm || hasImmediate;
    m_immediate = hasImmediate;
#else
    hasUfm = false;
    m_unicode = false;
    m_immediate = false;
#endif
    if (!hasOutput)
    {
      m_outputDir = wxEmptyString;
    }
    valid = ( hasImmediate && !hasAfm && !hasUfm && !hasVolt) ||
            (!hasImmediate &&  hasAfm && !hasUfm && !hasVolt) ||
            (!hasImmediate && !hasAfm &&  hasUfm);
    if (valid)
    {
      if (hasAfm)
      {
        m_fontType = (hasType && m_fontType == wxT_2("t1")) ? wxT_2("Type1") : wxT_2("TrueType");
      }
      else if (hasUfm)
      {
        m_fontType = (hasType && m_fontType == wxT_2("otf")) ? wxT_2("OpenType") : wxT_2("TrueType");
      }
    }
    else
    {
      parser.Usage();
    }
  }
  return valid;
}

int
MakeFont::OnExit()
{
  return 0;
}

int
MakeFont::OnRun()
{
  bool valid;
  wxLogMessage(wxT_2("wxPdfDocument MakeFont Utility Version ") + m_version);
  wxLogMessage(wxT_2("*** Starting to create font support files for ..."));
  if (!m_outputDir.IsEmpty())
  {
    wxFileName outputDirName(m_outputDir, wxEmptyString);
    if (outputDirName.DirExists())
    {
      m_outputDir = outputDirName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    }
    else
    {
      wxLogMessage(wxT_2("*** Warning: Output directory doesn't exist: ") + m_outputDir);
      m_outputDir = wxEmptyString;
    }
  }
  if (!m_outputDir.IsEmpty())
  {
    wxLogMessage(wxT_2("  OutputDir: ") + m_outputDir);
  }
  else
  {
    wxLogMessage(wxT_2("  OutputDir: (current working directory)"));
  }
#if wxUSE_UNICODE
  if (m_unicode)
  {
    if (m_immediate)
    {
      wxLogMessage(wxT_2("  Metrics  : Immediately from font file"));
      wxLogMessage(wxT_2("  Font file: ") + m_fontFile);
      valid = MakeFontImmediate(m_fontFile);
    }
    else
    {
      wxLogMessage(wxT_2("  UFM file : ") + m_ufmFile);
      wxLogMessage(wxT_2("  Font file: ") + m_fontFile);
      if (!m_volt.IsEmpty())
      {
        wxLogMessage(wxT_2("  VOLT file: ") + m_volt);
      }
      valid = MakeFontUFM(m_fontFile, m_ufmFile, m_fontType, m_volt);
    }
  }
  else
#endif
  {
    wxLogMessage(wxT_2("  AFM file : ") + m_afmFile);
    wxLogMessage(wxT_2("  Font file: ") + m_fontFile);
    wxLogMessage(wxT_2("  Encoding : ") + m_encoding);
    wxLogMessage(wxT_2("  Font type: ") + m_fontType);
    valid = MakeFontAFM(m_fontFile, m_afmFile, m_encoding, m_patchFile);
  }
  wxLogMessage(wxT_2("*** wxPdfDocument MakeFont finished."));

  return 0;
}

IMPLEMENT_APP_CONSOLE(MakeFont)
