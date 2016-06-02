///////////////////////////////////////////////////////////////////////////////
// Name:        pdfcffdecoder.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2008-08-01
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfcffdecoder.cpp Implementation of Type1 and Type2 CFF decoder class

/*
 * This Class decodes a Type1 or Type2 CFF string. The code is based on code and ideas from the iText project.
 */

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdfarraytypes.h"
#include "wx/pdfcffindex.h"
#include "wx/pdfcffdecoder.h"

#include "wxmemdbg.h"

// --- Implementation of CFF Decoder for charstring types 1 and 2

// CFF Dict Operators
// If the high byte is 0 the command is encoded with a single byte.

const int BASEFONTNAME_OP = 0x0c16;
const int CHARSET_OP      = 0x000f;
const int CHARSTRINGS_OP  = 0x0011;
const int CIDCOUNT_OP     = 0x0c22;
const int COPYRIGHT_OP    = 0x0c00;
const int ENCODING_OP     = 0x0010;
const int FAMILYNAME_OP   = 0x0003;
const int FDARRAY_OP      = 0x0c24;
const int FDSELECT_OP     = 0x0c25;
const int FONTBBOX_OP     = 0x0005;
const int FONTNAME_OP     = 0x0c26;
const int FULLNAME_OP     = 0x0002;
const int LOCAL_SUB_OP    = 0x0013;
const int NOTICE_OP       = 0x0001;
const int POSTSCRIPT_OP   = 0x0c15;
const int PRIVATE_OP      = 0x0012;
const int ROS_OP          = 0x0c1e;
const int UNIQUEID_OP     = 0x000d;
const int VERSION_OP      = 0x0000;
const int WEIGHT_OP       = 0x0004;
const int XUID_OP         = 0x000e;

const int NUM_STD_STRINGS = 391;

const char SUBR_RETURN_OP = 11;

#if 0
static const wxChar* gs_standardStrings[] = {
    // Generated from Appendix A of the CFF specification. Size should be 391.
    wxT_2(".notdef"),             wxT_2("space"),              wxT_2("exclam"),           wxT_2("quotedbl"),       wxT_2("numbersign"),
    wxT_2("dollar"),              wxT_2("percent"),            wxT_2("ampersand"),        wxT_2("quoteright"),     wxT_2("parenleft"),
    wxT_2("parenright"),          wxT_2("asterisk"),           wxT_2("plus"),             wxT_2("comma"),          wxT_2("hyphen"), 
    wxT_2("period"),              wxT_2("slash"),              wxT_2("zero"),             wxT_2("one"),            wxT_2("two"),
    wxT_2("three"),               wxT_2("four"),               wxT_2("five"),             wxT_2("six"),            wxT_2("seven"),
    wxT_2("eight"),               wxT_2("nine"),               wxT_2("colon"),            wxT_2("semicolon"),      wxT_2("less"),
    wxT_2("equal"),               wxT_2("greater"),            wxT_2("question"),         wxT_2("at"),             wxT_2("A"),
    wxT_2("B"),                   wxT_2("C"),                  wxT_2("D"),                wxT_2("E"),              wxT_2("F"),
    wxT_2("G"),                   wxT_2("H"),                  wxT_2("I"),                wxT_2("J"),              wxT_2("K"),
    wxT_2("L"),                   wxT_2("M"),                  wxT_2("N"),                wxT_2("O"),              wxT_2("P"),
    wxT_2("Q"),                   wxT_2("R"),                  wxT_2("S"),                wxT_2("T"),              wxT_2("U"),
    wxT_2("V"),                   wxT_2("W"),                  wxT_2("X"),                wxT_2("Y"),              wxT_2("Z"),
    wxT_2("bracketleft"),         wxT_2("backslash"),          wxT_2("bracketright"),     wxT_2("asciicircum"),    wxT_2("underscore"),
    wxT_2("quoteleft"),           wxT_2("a"),                  wxT_2("b"),                wxT_2("c"),              wxT_2("d"),
    wxT_2("e"),                   wxT_2("f"),                  wxT_2("g"),                wxT_2("h"),              wxT_2("i"),
    wxT_2("j"),                   wxT_2("k"),                  wxT_2("l"),                wxT_2("m"),              wxT_2("n"),
    wxT_2("o"),                   wxT_2("p"),                  wxT_2("q"),                wxT_2("r"),              wxT_2("s"),
    wxT_2("t"),                   wxT_2("u"),                  wxT_2("v"),                wxT_2("w"),              wxT_2("x"),
    wxT_2("y"),                   wxT_2("z"),                  wxT_2("braceleft"),        wxT_2("bar"),            wxT_2("braceright"),
    wxT_2("asciitilde"),          wxT_2("exclamdown"),         wxT_2("cent"),             wxT_2("sterling"),       wxT_2("fraction"),
    wxT_2("yen"),                 wxT_2("florin"),             wxT_2("section"),          wxT_2("currency"),       wxT_2("quotesingle"),
    wxT_2("quotedblleft"),        wxT_2("guillemotleft"),      wxT_2("guilsinglleft"),    wxT_2("guilsinglright"), wxT_2("fi"),
    wxT_2("fl"),                  wxT_2("endash"),             wxT_2("dagger"),           wxT_2("daggerdbl"),      wxT_2("periodcentered"),
    wxT_2("paragraph"),           wxT_2("bullet"),             wxT_2("quotesinglbase"),   wxT_2("quotedblbase"),   wxT_2("quotedblright"),
    wxT_2("guillemotright"),      wxT_2("ellipsis"),           wxT_2("perthousand"),      wxT_2("questiondown"),   wxT_2("grave"),
    wxT_2("acute"),               wxT_2("circumflex"),         wxT_2("tilde"),            wxT_2("macron"),         wxT_2("breve"),
    wxT_2("dotaccent"),           wxT_2("dieresis"),           wxT_2("ring"),             wxT_2("cedilla"),        wxT_2("hungarumlaut"),
    wxT_2("ogonek"),              wxT_2("caron"),              wxT_2("emdash"),           wxT_2("AE"),             wxT_2("ordfeminine"),
    wxT_2("Lslash"),              wxT_2("Oslash"),             wxT_2("OE"),               wxT_2("ordmasculine"),   wxT_2("ae"),
    wxT_2("dotlessi"),            wxT_2("lslash"),             wxT_2("oslash"),           wxT_2("oe"),             wxT_2("germandbls"),
    wxT_2("onesuperior"),         wxT_2("logicalnot"),         wxT_2("mu"),               wxT_2("trademark"),      wxT_2("Eth"),
    wxT_2("onehalf"),             wxT_2("plusminus"),          wxT_2("Thorn"),            wxT_2("onequarter"),     wxT_2("divide"),
    wxT_2("brokenbar"),           wxT_2("degree"),             wxT_2("thorn"),            wxT_2("threequarters"),  wxT_2("twosuperior"),
    wxT_2("registered"),          wxT_2("minus"),              wxT_2("eth"),              wxT_2("multiply"),       wxT_2("threesuperior"),
    wxT_2("copyright"),           wxT_2("Aacute"),             wxT_2("Acircumflex"),      wxT_2("Adieresis"),      wxT_2("Agrave"),
    wxT_2("Aring"),               wxT_2("Atilde"),             wxT_2("Ccedilla"),         wxT_2("Eacute"),         wxT_2("Ecircumflex"),
    wxT_2("Edieresis"),           wxT_2("Egrave"),             wxT_2("Iacute"),           wxT_2("Icircumflex"),    wxT_2("Idieresis"),
    wxT_2("Igrave"),              wxT_2("Ntilde"),             wxT_2("Oacute"),           wxT_2("Ocircumflex"),    wxT_2("Odieresis"),
    wxT_2("Ograve"),              wxT_2("Otilde"),             wxT_2("Scaron"),           wxT_2("Uacute"),         wxT_2("Ucircumflex"),
    wxT_2("Udieresis"),           wxT_2("Ugrave"),             wxT_2("Yacute"),           wxT_2("Ydieresis"),      wxT_2("Zcaron"),
    wxT_2("aacute"),              wxT_2("acircumflex"),        wxT_2("adieresis"),        wxT_2("agrave"),         wxT_2("aring"),
    wxT_2("atilde"),              wxT_2("ccedilla"),           wxT_2("eacute"),           wxT_2("ecircumflex"),    wxT_2("edieresis"),
    wxT_2("egrave"),              wxT_2("iacute"),             wxT_2("icircumflex"),      wxT_2("idieresis"),      wxT_2("igrave"),
    wxT_2("ntilde"),              wxT_2("oacute"),             wxT_2("ocircumflex"),      wxT_2("odieresis"),      wxT_2("ograve"),
    wxT_2("otilde"),              wxT_2("scaron"),             wxT_2("uacute"),           wxT_2("ucircumflex"),    wxT_2("udieresis"),
    wxT_2("ugrave"),              wxT_2("yacute"),             wxT_2("ydieresis"),        wxT_2("zcaron"),         wxT_2("exclamsmall"),
    wxT_2("Hungarumlautsmall"),   wxT_2("dollaroldstyle"),     wxT_2("dollarsuperior"),   wxT_2("ampersandsmall"), wxT_2("Acutesmall"),
    wxT_2("parenleftsuperior"),   wxT_2("parenrightsuperior"), wxT_2("twodotenleader"),   wxT_2("onedotenleader"), wxT_2("zerooldstyle"),
    wxT_2("oneoldstyle"),         wxT_2("twooldstyle"),        wxT_2("threeoldstyle"),    wxT_2("fouroldstyle"),   wxT_2("fiveoldstyle"),
    wxT_2("sixoldstyle"),         wxT_2("sevenoldstyle"),      wxT_2("eightoldstyle"),    wxT_2("nineoldstyle"),   wxT_2("commasuperior"),
    wxT_2("threequartersemdash"), wxT_2("periodsuperior"),     wxT_2("questionsmall"),    wxT_2("asuperior"),      wxT_2("bsuperior"),
    wxT_2("centsuperior"),        wxT_2("dsuperior"),          wxT_2("esuperior"),        wxT_2("isuperior"),      wxT_2("lsuperior"),
    wxT_2("msuperior"),           wxT_2("nsuperior"),          wxT_2("osuperior"),        wxT_2("rsuperior"),      wxT_2("ssuperior"),
    wxT_2("tsuperior"),           wxT_2("ff"),                 wxT_2("ffi"),              wxT_2("ffl"),            wxT_2("parenleftinferior"),
    wxT_2("parenrightinferior"),  wxT_2("Circumflexsmall"),    wxT_2("hyphensuperior"),   wxT_2("Gravesmall"),     wxT_2("Asmall"),
    wxT_2("Bsmall"),              wxT_2("Csmall"),             wxT_2("Dsmall"),           wxT_2("Esmall"),         wxT_2("Fsmall"),
    wxT_2("Gsmall"),              wxT_2("Hsmall"),             wxT_2("Ismall"),           wxT_2("Jsmall"),         wxT_2("Ksmall"),
    wxT_2("Lsmall"),              wxT_2("Msmall"),             wxT_2("Nsmall"),           wxT_2("Osmall"),         wxT_2("Psmall"),
    wxT_2("Qsmall"),              wxT_2("Rsmall"),             wxT_2("Ssmall"),           wxT_2("Tsmall"),         wxT_2("Usmall"),
    wxT_2("Vsmall"),              wxT_2("Wsmall"),             wxT_2("Xsmall"),           wxT_2("Ysmall"),         wxT_2("Zsmall"),
    wxT_2("colonmonetary"),       wxT_2("onefitted"),          wxT_2("rupiah"),           wxT_2("Tildesmall"),     wxT_2("exclamdownsmall"),
    wxT_2("centoldstyle"),        wxT_2("Lslashsmall"),        wxT_2("Scaronsmall"),      wxT_2("Zcaronsmall"),    wxT_2("Dieresissmall"),
    wxT_2("Brevesmall"),          wxT_2("Caronsmall"),         wxT_2("Dotaccentsmall"),   wxT_2("Macronsmall"),    wxT_2("figuredash"),
    wxT_2("hypheninferior"),      wxT_2("Ogoneksmall"),        wxT_2("Ringsmall"),        wxT_2("Cedillasmall"),   wxT_2("questiondownsmall"),
    wxT_2("oneeighth"),           wxT_2("threeeighths"),       wxT_2("fiveeighths"),      wxT_2("seveneighths"),   wxT_2("onethird"),
    wxT_2("twothirds"),           wxT_2("zerosuperior"),       wxT_2("foursuperior"),     wxT_2("fivesuperior"),   wxT_2("sixsuperior"),
    wxT_2("sevensuperior"),       wxT_2("eightsuperior"),      wxT_2("ninesuperior"),     wxT_2("zeroinferior"),   wxT_2("oneinferior"),
    wxT_2("twoinferior"),         wxT_2("threeinferior"),      wxT_2("fourinferior"),     wxT_2("fiveinferior"),   wxT_2("sixinferior"),
    wxT_2("seveninferior"),       wxT_2("eightinferior"),      wxT_2("nineinferior"),     wxT_2("centinferior"),   wxT_2("dollarinferior"),
    wxT_2("periodinferior"),      wxT_2("commainferior"),      wxT_2("Agravesmall"),      wxT_2("Aacutesmall"),    wxT_2("Acircumflexsmall"),
    wxT_2("Atildesmall"),         wxT_2("Adieresissmall"),     wxT_2("Aringsmall"),       wxT_2("AEsmall"),        wxT_2("Ccedillasmall"),
    wxT_2("Egravesmall"),         wxT_2("Eacutesmall"),        wxT_2("Ecircumflexsmall"), wxT_2("Edieresissmall"), wxT_2("Igravesmall"),
    wxT_2("Iacutesmall"),         wxT_2("Icircumflexsmall"),   wxT_2("Idieresissmall"),   wxT_2("Ethsmall"),       wxT_2("Ntildesmall"),
    wxT_2("Ogravesmall"),         wxT_2("Oacutesmall"),        wxT_2("Ocircumflexsmall"), wxT_2("Otildesmall"),    wxT_2("Odieresissmall"),
    wxT_2("OEsmall"),             wxT_2("Oslashsmall"),        wxT_2("Ugravesmall"),      wxT_2("Uacutesmall"),    wxT_2("Ucircumflexsmall"),
    wxT_2("Udieresissmall"),      wxT_2("Yacutesmall"),        wxT_2("Thornsmall"),       wxT_2("Ydieresissmall"), wxT_2("001.000"),
    wxT_2("001.001"),             wxT_2("001.002"),            wxT_2("001.003"),          wxT_2("Black"),          wxT_2("Bold"),
    wxT_2("Book"),                wxT_2("Light"),              wxT_2("Medium"),           wxT_2("Regular"),        wxT_2("Roman"),
    wxT_2("Semibold")
  };
static int gs_standardStringsCount = sizeof(gs_standardStrings) / sizeof(wxChar*);
#endif

// The Strings in this array represent Type1/Type2 operator names
static const wxChar* gs_subrsFunctions[] = {
    wxT("RESERVED_0"),  wxT("hstem"),       wxT("RESERVED_2"),  wxT("vstem"),          wxT("vmoveto"),
    wxT("rlineto"),     wxT("hlineto"),     wxT("vlineto"),     wxT("rrcurveto"),      wxT("RESERVED_9"),
    wxT("callsubr"),    wxT("return"),      wxT("escape"),      wxT("hsbw"),/*RES_13*/ wxT("endchar"),
    wxT("RESERVED_15"), wxT("RESERVED_16"), wxT("RESERVED_17"), wxT("hstemhm"),        wxT("hintmask"),
    wxT("cntrmask"),    wxT("rmoveto"),     wxT("hmoveto"),     wxT("vstemhm"),        wxT("rcurveline"),
    wxT("rlinecurve"),  wxT("vvcurveto"),   wxT("hhcurveto"),   wxT("shortint"),       wxT("callgsubr"),
    wxT("vhcurveto"),   wxT("hvcurveto")
  };
#if 0
static int gs_subrsFunctionsCount = sizeof(gs_subrsFunctions) / sizeof(wxChar*);
#endif

// The Strings in this array represent Type1/Type2 escape operator names
static const wxChar* gs_subrsEscapeFuncs[] = {
    wxT("RESERVED_0"),  wxT("RESERVED_1"),    wxT("RESERVED_2"),   wxT("and"),           wxT("or"), 
    wxT("not"),         wxT("seac"),/*RES_6*/ wxT("sbw"),/*RES_7*/ wxT("RESERVED_8"),    wxT("abs"),
    wxT("add"),         wxT("sub"),           wxT("div"),          wxT("RESERVED_13"),   wxT("neg"),
    wxT("eq"),          wxT("RESERVED_16"),   wxT("RESERVED_17"),  wxT("drop"),          wxT("RESERVED_19"),
    wxT("put"),         wxT("get"),           wxT("ifelse"),       wxT("random"),        wxT("mul"),
    wxT("RESERVED_25"), wxT("sqrt"),          wxT("dup"),          wxT("exch"),          wxT("index"), 
    wxT("roll"),        wxT("RESERVED_31"),   wxT("RESERVED_32"),  wxT("RESERVED_33"),   wxT("hflex"),
    wxT("flex"),        wxT("hflex1"),        wxT("flex1"),        wxT("RESERVED_REST")
  };
static int gs_subrsEscapeFuncsCount = sizeof(gs_subrsEscapeFuncs) / sizeof(wxChar*);

#if 0
static wxChar* gs_operatorNames[] = {
    wxT_2("version"),           wxT_2("Notice"),             wxT_2("FullName"),      wxT_2("FamilyName"),     wxT_2("Weight"),
    wxT_2("FontBBox"),          wxT_2("BlueValues"),         wxT_2("OtherBlues"),    wxT_2("FamilyBlues"),    wxT_2("FamilyOtherBlues"),
    wxT_2("StdHW"),             wxT_2("StdVW"),              wxT_2("UNKNOWN_12"),    wxT_2("UniqueID"),       wxT_2("XUID"),
    wxT_2("charset"),           wxT_2("Encoding"),           wxT_2("CharStrings"),   wxT_2("Private"),        wxT_2("Subrs"),
    wxT_2("defaultWidthX"),     wxT_2("nominalWidthX"),      wxT_2("UNKNOWN_22"),    wxT_2("UNKNOWN_23"),     wxT_2("UNKNOWN_24"),
    wxT_2("UNKNOWN_25"),        wxT_2("UNKNOWN_26"),         wxT_2("UNKNOWN_27"),    wxT_2("UNKNOWN_28"),     wxT_2("UNKNOWN_29"),
    wxT_2("UNKNOWN_30"),        wxT_2("UNKNOWN_31"),         wxT_2("Copyright"),     wxT_2("isFixedPitch"),   wxT_2("ItalicAngle"),
    wxT_2("UnderlinePosition"), wxT_2("UnderlineThickness"), wxT_2("PaintType"),     wxT_2("CharstringType"), wxT_2("FontMatrix"),
    wxT_2("StrokeWidth"),       wxT_2("BlueScale"),          wxT_2("BlueShift"),     wxT_2("BlueFuzz"),       wxT_2("StemSnapH"),
    wxT_2("StemSnapV"),         wxT_2("ForceBold"),          wxT_2("UNKNOWN_12_15"), wxT_2("UNKNOWN_12_16"),  wxT_2("LanguageGroup"),
    wxT_2("ExpansionFactor"),   wxT_2("initialRandomSeed"),  wxT_2("SyntheticBase"), wxT_2("PostScript"),     wxT_2("BaseFontName"),
    wxT_2("BaseFontBlend"),     wxT_2("UNKNOWN_12_24"),      wxT_2("UNKNOWN_12_25"), wxT_2("UNKNOWN_12_26"),  wxT_2("UNKNOWN_12_27"),
    wxT_2("UNKNOWN_12_28"),     wxT_2("UNKNOWN_12_29"),      wxT_2("ROS"),           wxT_2("CIDFontVersion"), wxT_2("CIDFontRevision"),
    wxT_2("CIDFontType"),       wxT_2("CIDCount"),           wxT_2("UIDBase"),       wxT_2("FDArray"),        wxT_2("FDSelect"),
    wxT_2("FontName")
  };
static int gs_operatorNamesCount = sizeof(gs_operatorNames) / sizeof(wxChar*);
#endif

class wxPdfCffFontObject
{
public:
  wxPdfCffFontObject() {}
  int      m_type;
  int      m_intValue;
  wxString m_strValue;
};

wxPdfCffDecoder::wxPdfCffDecoder()
{
  m_charstringType = 1;
  
  m_globalSubrIndex  = NULL;
  m_hGlobalSubrsUsed = NULL;
  m_lGlobalSubrsUsed = NULL;

  m_args = new wxPdfCffFontObject[48];
  m_argCount = 0;
}

wxPdfCffDecoder::wxPdfCffDecoder(wxPdfCffIndexArray* globalSubrIndex,
                                 wxPdfSortedArrayInt* hGlobalSubrsUsed,
                                 wxArrayInt* lGlobalSubrsUsed)
{
  m_charstringType = 2;
  
  m_globalSubrIndex  = globalSubrIndex;
  m_hGlobalSubrsUsed = hGlobalSubrsUsed;
  m_lGlobalSubrsUsed = lGlobalSubrsUsed;

  m_args = new wxPdfCffFontObject[48];
  m_argCount = 0;
}

wxPdfCffDecoder::~wxPdfCffDecoder()
{
  delete [] m_args;
}

// --- Read original CFF stream

unsigned char
wxPdfCffDecoder::ReadByte(wxInputStream* stream)
{
  unsigned char card8;
  stream->Read(&card8, 1);
  return card8;
}

short
wxPdfCffDecoder::ReadShort(wxInputStream* stream)
{
  // Read a 2-byte integer from file (big endian)
  short i16;
  stream->Read(&i16, 2);
  return wxINT16_SWAP_ON_LE(i16);
}

int
wxPdfCffDecoder::ReadInt(wxInputStream* stream)
{
  // Read a 4-byte integer from file (big endian)
  int i32;
  stream->Read(&i32, 4);
  return wxINT32_SWAP_ON_LE(i32);
}

// -- Subset global and local subroutines

int
wxPdfCffDecoder::CalcBias(int nSubrs)
{
  int bias;
  // If type == 1 then bias = 0, else calc according to the count
  if (m_charstringType == 1)
  {
    bias = 0;
  }
  else if (nSubrs < 1240)
  {
    bias = 107;
  }
  else if (nSubrs < 33900)
  {
    bias = 1131;
  }
  else
  {
    bias = 32768;
  }
  return bias;
}

bool
wxPdfCffDecoder::GetCharWidthAndComposite(wxPdfCffIndexElement& charstring, int& width, bool& isComposite, int& bchar, int& achar)
{
  bool ok = false;
  width = -1;
  isComposite = false;
  bchar = -1;
  achar = -1;

  wxInputStream* stream = charstring.GetBuffer();
  int begin = charstring.GetOffset();
  int end   = begin + charstring.GetLength();

  // Clear the stack
  EmptyStack();
  m_numHints = 0;

  stream->SeekI(begin);
  ReadCommand(stream);
  wxPdfCffFontObject* element = NULL;
  int numArgs = m_argCount;
  HandleStack();
  if (m_key == wxT_2("hsbw"))
  {
    if (numArgs == 2)
    {
      ok = true;
      element = &m_args[1]; // 2nd argument is width
      width = element->m_intValue;
    }
  }
  else if (m_key == wxT_2("sbw"))
  {
    if (numArgs == 4)
    {
      ok = true;
      element = &m_args[2]; // 3rd argument is width
      width = element->m_intValue;
    }
  }
  if (ok && (stream->TellI() < end))
  {
    ReadCommand(stream);
    numArgs = m_argCount;
    // Check the modification needed on the Argument Stack according to key;
    HandleStack();
    if (m_key == wxT_2("seac"))
    {
      if (numArgs == 5)
      {
        isComposite = true;
        // third argument
        element = &m_args[3];
        bchar = element->m_intValue;
        element = &m_args[4];
        achar = element->m_intValue;
      }
    }
  }
  return ok;
}

void
wxPdfCffDecoder::ReadASubr(wxInputStream* stream, int begin, int end,
                           int globalBias, int localBias, 
                           wxPdfSortedArrayInt& hSubrsUsed, wxArrayInt& lSubrsUsed,
                           wxPdfCffIndexArray& localSubrIndex)
{
  int beginSubr, endSubr;
#if 0
  wxLogDebug(wxT_2("ReadAsubr %d %d %d %d"), begin, end, globalBias, localBias);
#endif
  // Clear the stack for the subrs
  EmptyStack();
  m_numHints = 0;
  // Goto begining of the subr
  stream->SeekI(begin);
  while (stream->TellI() < end)
  {
    // Read the next command
    ReadCommand(stream);
    int pos = stream->TellI();
    wxPdfCffFontObject* topElement = NULL;
    if (m_argCount > 0)
    {
      topElement = &m_args[m_argCount-1];
    }
    int numArgs = m_argCount;
    // Check the modification needed on the Argument Stack according to key;
    HandleStack();
    // a call to a Lsubr
    if (m_key == wxT_2("callsubr")) 
    {
      // Verify that arguments are passed 
      if (numArgs > 0)
      {
        // Calc the index of the Subrs
        int subr = topElement->m_intValue + localBias;
        // If the subr isn't in the HashMap -> Put in
        if (hSubrsUsed.Index(subr) == wxNOT_FOUND)
        {
#if 0
          wxLogDebug(wxT_2("Add hSubr: %s %d"), m_key.c_str(), subr);
#endif
          hSubrsUsed.Add(subr);
          lSubrsUsed.Add(subr);
        }
        wxPdfCffIndexElement& localSubr = localSubrIndex[subr];
        beginSubr = localSubr.GetOffset();
        endSubr = beginSubr + localSubr.GetLength();
        CalcHints(localSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }            
    }
    // a call to a Gsubr
    else if (m_key == wxT_2("callgsubr"))
    {
      // Verify that arguments are passed 
      if (numArgs > 0)
      {
        // Calc the index of the Subrs
        int subr = topElement->m_intValue + globalBias;
        // If the subr isn't in the HashMap -> Put in
        if (m_hGlobalSubrsUsed->Index(subr) == wxNOT_FOUND)
        {
#if 0
          wxLogDebug(wxT_2("Add hGSubr: %s %d"), m_key.c_str(), subr);
#endif
          m_hGlobalSubrsUsed->Add(subr);
          m_lGlobalSubrsUsed->Add(subr);
        }
        wxPdfCffIndexElement& globalSubr = (*m_globalSubrIndex)[subr];
        beginSubr = globalSubr.GetOffset();
        endSubr = beginSubr + globalSubr.GetLength();
        CalcHints(globalSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // A call to "stem"
    else if (m_key == wxT_2("hstem") || m_key == wxT_2("vstem") || m_key == wxT_2("hstemhm") || m_key == wxT_2("vstemhm"))
    {
      // Increment the NumOfHints by the number couples of of arguments
      m_numHints += numArgs / 2;
    }
          // A call to "mask"
    else if (m_key == wxT_2("hintmask") || m_key == wxT_2("cntrmask"))
    {
      // Compute the size of the mask
      int sizeOfMask = m_numHints / 8;
      if (m_numHints % 8 != 0 || sizeOfMask == 0)
      {
        sizeOfMask++;
      }
      // Continue the pointer in SizeOfMask steps
      int i;
      for (i = 0; i < sizeOfMask; i++)
      {
        ReadByte(stream);
      }
    }
  }
#if 0
  wxLogDebug(wxT_2("ReadASubr end"));
#endif
}

void
wxPdfCffDecoder::HandleStack()
{
  // Findout what the operator does to the stack
  int stackHandle = StackOpp();
  if (stackHandle < 2)
  {
    // The operators that enlarge the stack by one
    if (stackHandle == 1)
    {
      PushStack();
    }
    // The operators that pop the stack
    else
    {
      // Abs value for the for loop
      stackHandle *= -1;
      int i;
      for (i = 0; i < stackHandle; i++)
      {
        PopStack();
      }
    }
  }
  // All other flush the stack
  else
  {
    EmptyStack();
  }
}
  
int
wxPdfCffDecoder::StackOpp()
{
  int op;
  if (m_key == wxT_2("ifelse"))
  {
    op = -3;
  }
  else if (m_key == wxT_2("roll") || m_key == wxT_2("put"))
  {
    op = -2;
  }
  else if (m_key == wxT_2("callsubr") || m_key == wxT_2("callgsubr") || m_key == wxT_2("add")  ||
           m_key == wxT_2("sub")      || m_key == wxT_2("div")       || m_key == wxT_2("mul")  ||
           m_key == wxT_2("drop")     || m_key == wxT_2("and")       || m_key == wxT_2("or")   ||
           m_key == wxT_2("eq"))
  {
    op = -1;
  }
  else if (m_key == wxT_2("abs")  || m_key == wxT_2("neg")   || m_key == wxT_2("sqrt") ||
           m_key == wxT_2("exch") || m_key == wxT_2("index") || m_key == wxT_2("get")  ||
           m_key == wxT_2("not")  || m_key == wxT_2("return"))
  {
    op = 0;
  }
  else if (m_key == wxT_2("random") || m_key == wxT_2("dup"))
  {
    op = 1;
  }
  else
  {
    op = 2;
  }
  return op;
}
  
void
wxPdfCffDecoder::EmptyStack()
{
  m_argCount = 0;    
}
  
void
wxPdfCffDecoder::PopStack()
{
  if (m_argCount > 0)
  {
    m_argCount--;
  }
}
  
void
wxPdfCffDecoder::PushStack()
{
  m_argCount++;
}
  
void
wxPdfCffDecoder::ReadCommand(wxInputStream* stream)
{
  m_key = wxEmptyString;
  bool gotKey = false;
  // Until a key is found
  while (!gotKey)
  {
    // Read the first Char
    unsigned char b0 = ReadByte(stream);
    // decode according to the type1/type2 format
    if (b0 == 28) // the two next bytes represent a short int;
    {
      int first = ReadByte(stream) & 0xff;
      int second = ReadByte(stream) & 0xff;
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = first << 8 | second;
      m_argCount++;
      continue;
    }
    if (b0 >= 32 && b0 <= 246) // The byte read is the byte;
    {
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = (int) (b0 - 139);
      m_argCount++;
      continue;
    }
    if (b0 >= 247 && b0 <= 250) // The byte read and the next byte constetute a short int
    {
      unsigned char b1 = ReadByte(stream);
      short item = (short) ((b0-247)*256 + b1 + 108);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 >= 251 && b0 <= 254)// Same as above except negative
    {
      unsigned char b1 = ReadByte(stream);
      short item = (short) (-(b0-251)*256-b1-108);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 == 255)// The next for bytes represent a double.
    {
      int item = ReadInt(stream);
      m_args[m_argCount].m_type = 0;
      m_args[m_argCount].m_intValue = item;
      m_argCount++;
      continue;
    }
    if (b0 <= 31 && b0 != 28) // An operator was found.. Set Key.
    {
      gotKey = true;
      // 12 is an escape command therefor the next byte is a part
      // of this command
      if (b0 == 12)
      {
        unsigned char b1 = ReadByte(stream);
        if (b1 > gs_subrsEscapeFuncsCount-1)
        {
          b1 = gs_subrsEscapeFuncsCount-1;
        }
        m_key = gs_subrsEscapeFuncs[b1];
      }
      else
      {
        m_key = gs_subrsFunctions[b0];
      }
      continue;
    }
  }
}
  
int
wxPdfCffDecoder::CalcHints(wxInputStream* stream, int begin, int end, int globalBias, int localBias, wxPdfCffIndexArray& localSubrIndex)
{
  int beginSubr, endSubr;
  // Goto begining of the subr
  stream->SeekI(begin);
  while (stream->TellI() < end)
  {
    // Read the next command
    ReadCommand(stream);
    int pos = stream->TellI();
    wxPdfCffFontObject* topElement = NULL;
    if (m_argCount > 0)
    {
      topElement = &m_args[m_argCount-1];
    }
    int numArgs = m_argCount;
    //Check the modification needed on the Argument Stack according to key;
    HandleStack();
    // a call to a Lsubr
    if (m_key == wxT_2("callsubr")) 
    {
      if (numArgs > 0)
      {
        int subr = topElement->m_intValue + localBias;
        wxPdfCffIndexElement& localSubr = localSubrIndex[subr];
        beginSubr = localSubr.GetOffset();
        endSubr = beginSubr + localSubr.GetLength();
        CalcHints(localSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // a call to a Gsubr
    else if (m_key == wxT_2("callgsubr"))
    {
      if (numArgs > 0)
      {
        int subr = topElement->m_intValue + globalBias;
        wxPdfCffIndexElement& globalSubr = (*m_globalSubrIndex)[subr];
        beginSubr = globalSubr.GetOffset();
        endSubr = beginSubr + globalSubr.GetLength();
        CalcHints(globalSubr.GetBuffer(), beginSubr, endSubr, globalBias, localBias, localSubrIndex);
        stream->SeekI(pos);
      }
    }
    // A call to "stem"
    else if (m_key == wxT_2("hstem") || m_key == wxT_2("vstem") || m_key == wxT_2("hstemhm") || m_key == wxT_2("vstemhm"))
    {
      // Increment the NumOfHints by the number couples of of arguments
      m_numHints += numArgs / 2;
    }
    // A call to "mask"
    else if (m_key == wxT_2("hintmask") || m_key == wxT_2("cntrmask"))
    {
      // Compute the size of the mask
      int sizeOfMask = m_numHints / 8;
      if (m_numHints % 8 != 0 || sizeOfMask == 0)
      {
        sizeOfMask++;
      }
      // Continue the pointer in SizeOfMask steps
      int i;
      for (i = 0; i < sizeOfMask; i++)
      {
        ReadByte(stream);
      }
    }
  }
  return m_numHints;
}
