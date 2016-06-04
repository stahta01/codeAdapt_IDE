//////////////////////////////////////////////////////////////////////////////
// File:        prefs.cpp
// Purpose:     wxScintilla test preferences
// Maintainer:  Otto Wyss
// Created:     2003-09-01
// RCS-ID:      $Id: prefs.cpp 106 2007-12-01 18:31:55Z Pecan $
// Copyright:   (c) 2004 wxCode
// Licence:     wxWindows
//////////////////////////////////////////////////////////////////////////////
/*
	This file is part of Code Snippets, a plugin for Code::Blocks
	Copyright (C) 2006 Arto Jonsson
	Copyright (C) 2007 Pecan Heber

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
// RCS-ID: $Id: prefs.cpp 106 2007-12-01 18:31:55Z Pecan $

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWindows headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//! wxWindows headers

//! wxWindows/contrib headers

//! application headers
#include "defsext.h"     // Additional definitions
#include "prefs.h"       // Preferences


//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! language types
const CommonInfo g_CommonPrefs = {
    // editor functionality prefs
    true,  // syntaxEnable
    false, // foldEnable
    true,  // indentEnable
    // display defaults prefs
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    false, // longLineOnEnable
    false, // whiteSpaceEnable
};

//----------------------------------------------------------------------------
// keywordlists
// C++
const wxChar* CppWordlist1 =
    wxT("asm auto bool break case catch char class const const_cast \
       continue default delete do double dynamic_cast else enum explicit \
       export extern false float for friend goto if inline int long \
       mutable namespace new operator private protected public register \
       reinterpret_cast return short signed sizeof static static_cast \
       struct switch template this throw true try typedef typeid \
       typename union unsigned using virtual void volatile wchar_t \
       while");
const wxChar* CppWordlist2 =
    wxT("file");
const wxChar* CppWordlist3 =
    wxT("a addindex addtogroup anchor arg attention author b brief bug c \
       class code date def defgroup deprecated dontinclude e em endcode \
       endhtmlonly endif endlatexonly endlink endverbatim enum example \
       exception f$ f[ f] file fn hideinitializer htmlinclude \
       htmlonly if image include ingroup internal invariant interface \
       latexonly li line link mainpage name namespace nosubgrouping note \
       overload p page par param post pre ref relates remarks return \
       retval sa section see showinitializer since skip skipline struct \
       subsection test throw todo typedef union until var verbatim \
       verbinclude version warning weakgroup $ @ \" & < > # { }");

// Python
const wxChar* PythonWordlist1 =
    wxT("and assert break class continue def del elif else except exec \
       finally for from global if import in is lambda None not or pass \
       print raise return try while yield");
const wxChar* PythonWordlist2 =
    wxT("ACCELERATORS ALT AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON BEGIN \
       BITMAP BLOCK BUTTON CAPTION CHARACTERISTICS CHECKBOX CLASS \
       COMBOBOX CONTROL CTEXT CURSOR DEFPUSHBUTTON DIALOG DIALOGEX \
       DISCARDABLE EDITTEXT END EXSTYLE FONT GROUPBOX ICON LANGUAGE \
       LISTBOX LTEXT MENU MENUEX MENUITEM MESSAGETABLE POPUP PUSHBUTTON \
       RADIOBUTTON RCDATA RTEXT SCROLLBAR SEPARATOR SHIFT STATE3 \
       STRINGTABLE STYLE TEXTINCLUDE VALUE VERSION VERSIONINFO VIRTKEY");


//----------------------------------------------------------------------------
//! languages
const LanguageInfo g_LanguagePrefs [] = {
    // C++
    {wxT_2("C++"),
     wxT_2("*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma"),
     wxSCI_LEX_CPP,
     {{TOKEN_DEFAULT, NULL},
      {TOKEN_COMMENT, NULL},
      {TOKEN_COMMENT_LINE, NULL},
      {TOKEN_COMMENT_DOC, NULL},
      {TOKEN_NUMBER, NULL},
      {TOKEN_WORD1, CppWordlist1}, // KEYWORDS
      {TOKEN_STRING, NULL},
      {TOKEN_CHARACTER, NULL},
      {TOKEN_UUID, NULL},
      {TOKEN_PREPROCESSOR, NULL},
      {TOKEN_OPERATOR, NULL},
      {TOKEN_IDENTIFIER, NULL},
      {TOKEN_STRING_EOL, NULL},
      {TOKEN_DEFAULT, NULL}, // VERBATIM
      {TOKEN_REGEX, NULL},
      {TOKEN_COMMENT_SPECIAL, NULL}, // DOXY
      {TOKEN_WORD2, CppWordlist2}, // EXTRA WORDS
      {TOKEN_WORD3, CppWordlist3}, // DOXY KEYWORDS
      {TOKEN_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     FOLD_TYPE_COMMENT | FOLD_TYPE_COMPACT | FOLD_TYPE_PREPROC},
    // Python
    {wxT_2("Python"),
     wxT_2("*.py;*.pyw"),
     wxSCI_LEX_PYTHON,
     {{TOKEN_DEFAULT, NULL},
      {TOKEN_COMMENT_LINE, NULL},
      {TOKEN_NUMBER, NULL},
      {TOKEN_STRING, NULL},
      {TOKEN_CHARACTER, NULL},
      {TOKEN_WORD1, PythonWordlist1}, // KEYWORDS
      {TOKEN_DEFAULT, NULL}, // TRIPLE
      {TOKEN_DEFAULT, NULL}, // TRIPLEDOUBLE
      {TOKEN_DEFAULT, NULL}, // CLASSNAME
      {TOKEN_DEFAULT, PythonWordlist2}, // DEFNAME
      {TOKEN_OPERATOR, NULL},
      {TOKEN_IDENTIFIER, NULL},
      {TOKEN_DEFAULT, NULL}, // COMMENT_BLOCK
      {TOKEN_STRING_EOL, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     FOLD_TYPE_COMMENTPY | FOLD_TYPE_QUOTESPY},
    // * (any)
    {(wxChar *)DEFAULT_LANGUAGE,
     wxT("*.*"),
     wxSCI_LEX_PROPERTIES,
     {{TOKEN_DEFAULT, NULL},
      {TOKEN_DEFAULT, NULL},
      {TOKEN_DEFAULT, NULL},
      {TOKEN_DEFAULT, NULL},
      {TOKEN_DEFAULT, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     0},
    };

const int g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

//----------------------------------------------------------------------------
//! style types
const StyleInfo g_StylePrefs [] = {
    // TOKEN_DEFAULT
    {wxT("Default"),
     wxT("BLACK"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_WORD1
    {wxT("Keyword1"),
     wxT("BLUE"), wxT("WHITE"),
     wxT(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_WORD2
    {wxT("Keyword2"),
     wxT("DARK BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_WORD3
    {wxT("Keyword3"),
     wxT("CORNFLOWER BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_WORD4
    {wxT("Keyword4"),
     wxT("CYAN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_WORD5
    {wxT("Keyword5"),
     wxT("DARK GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_WORD6
    {wxT("Keyword6"),
     wxT("GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_COMMENT
    {wxT("Comment"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_COMMENT_DOC
    {wxT("Comment (Doc)"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_COMMENT_LINE
    {wxT("Comment line"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_COMMENT_SPECIAL
    {wxT("Special comment"),
     wxT("FOREST GREEN"), wxT("WHITE"),
     wxT(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_CHARACTER
    {wxT("Character"),
     wxT("KHAKI"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_CHARACTER_EOL
    {wxT("Character (EOL)"),
     wxT("KHAKI"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_STRING
    {wxT("String"),
     wxT("BROWN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_STRING_EOL
    {wxT("String (EOL)"),
     wxT("BROWN"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_DELIMITER
    {wxT("Delimiter"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_PUNCTUATION
    {wxT("Punctuation"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_OPERATOR
    {wxT("Operator"),
     wxT("BLACK"), wxT("WHITE"),
     wxT(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_BRACE
    {wxT("Label"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_COMMAND
    {wxT("Command"),
     wxT("BLUE"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_IDENTIFIER
    {wxT("Identifier"),
     wxT("BLACK"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_LABEL
    {wxT("Label"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_NUMBER
    {wxT("Number"),
     wxT("SIENNA"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_PARAMETER
    {wxT("Parameter"),
     wxT("VIOLET"), wxT("WHITE"),
     wxT(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_REGEX
    {wxT("Regular expression"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_UUID
    {wxT("UUID"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_VALUE
    {wxT("Value"),
     wxT("ORCHID"), wxT("WHITE"),
     wxT(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_PREPROCESSOR
    {wxT("Preprocessor"),
     wxT("GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_SCRIPT
    {wxT("Script"),
     wxT("DARK GREY"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_ERROR
    {wxT("Error"),
     wxT("RED"), wxT("WHITE"),
     wxT(""), 10, 0, 0},

    // TOKEN_UNDEFINED
    {wxT("Undefined"),
     wxT("ORANGE"), wxT("WHITE"),
     wxT(""), 10, 0, 0}

    };

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);

