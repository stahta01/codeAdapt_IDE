#include <sdk.h>
#include "formattersettings.h"
#include <configmanager.h>

FormatterSettings::FormatterSettings()
{
  //ctor
}

FormatterSettings::~FormatterSettings()
{
  //dtor
}

void FormatterSettings::ApplyTo(astyle::ASFormatter& formatter)
{
  ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("astyle"));

  int style = cfg->ReadInt(wxT_2("/style"), 0);

  switch (style)
  {
    case 0: // ansi
      formatter.setBracketIndent(false);
      formatter.setTabIndentation(4);
      formatter.setSpaceIndentation(4);
      formatter.setBracketFormatMode(astyle::BREAK_MODE);
      formatter.setClassIndent(false);
      formatter.setSwitchIndent(false);
      formatter.setNamespaceIndent(true);
      formatter.setBlockIndent(false);
      formatter.setBreakClosingHeaderBracketsMode(false);
      formatter.setBreakBlocksMode(false);
      formatter.setBreakElseIfsMode(false);
      formatter.setOperatorPaddingMode(false);
      formatter.setParensOutsidePaddingMode(false);
      formatter.setParensInsidePaddingMode(false);
      formatter.setParensUnPaddingMode(false);
      formatter.setSingleStatementsMode(true);
      formatter.setBreakOneLineBlocksMode(true);
      break;

    case 1: // K&R
      formatter.setBracketIndent(false);
      formatter.setTabIndentation(4);
      formatter.setSpaceIndentation(4);
      formatter.setBracketFormatMode(astyle::ATTACH_MODE);
      formatter.setClassIndent(false);
      formatter.setSwitchIndent(false);
      formatter.setNamespaceIndent(true);
      formatter.setBlockIndent(false);
      formatter.setBreakClosingHeaderBracketsMode(false);
      formatter.setBreakBlocksMode(false);
      formatter.setBreakElseIfsMode(false);
      formatter.setOperatorPaddingMode(false);
      formatter.setParensInsidePaddingMode(false);
      formatter.setParensOutsidePaddingMode(false);
      formatter.setParensUnPaddingMode(false);
      formatter.setSingleStatementsMode(true);
      formatter.setBreakOneLineBlocksMode(true);
      break;

    case 2: // Linux
      formatter.setBracketIndent(false);
      formatter.setTabIndentation(8);
      formatter.setSpaceIndentation(8);
      formatter.setBracketFormatMode(astyle::BDAC_MODE);
      formatter.setClassIndent(false);
      formatter.setSwitchIndent(false);
      formatter.setNamespaceIndent(true);
      formatter.setBlockIndent(false);
      formatter.setBreakClosingHeaderBracketsMode(false);
      formatter.setBreakBlocksMode(false);
      formatter.setBreakElseIfsMode(false);
      formatter.setOperatorPaddingMode(false);
      formatter.setParensOutsidePaddingMode(false);
      formatter.setParensInsidePaddingMode(false);
      formatter.setParensUnPaddingMode(false);
      formatter.setSingleStatementsMode(true);
      formatter.setBreakOneLineBlocksMode(true);
      break;

    case 3: // GNU
      formatter.setBracketIndent(false);
      formatter.setTabIndentation(2);
      formatter.setSpaceIndentation(2);
      formatter.setBracketFormatMode(astyle::BREAK_MODE);
      formatter.setClassIndent(false);
      formatter.setSwitchIndent(false);
      formatter.setNamespaceIndent(true);
      formatter.setBlockIndent(true);
      formatter.setBreakClosingHeaderBracketsMode(false);
      formatter.setBreakBlocksMode(false);
      formatter.setBreakElseIfsMode(false);
      formatter.setOperatorPaddingMode(false);
      formatter.setParensOutsidePaddingMode(false);
      formatter.setParensInsidePaddingMode(false);
      formatter.setParensUnPaddingMode(false);
      formatter.setSingleStatementsMode(true);
      formatter.setBreakOneLineBlocksMode(true);
      break;

    case 4: // Java
      formatter.setJavaStyle();
      //formatter.modeSetManually = true;
      formatter.setBracketIndent(false);
      formatter.setTabIndentation(4);
      formatter.setSpaceIndentation(4);
      formatter.setBracketFormatMode(astyle::ATTACH_MODE);
      //formatter.setClassIndent(false);
      formatter.setSwitchIndent(false);
      //formatter.setNamespaceIndent(true);
      formatter.setBlockIndent(false);
      formatter.setBreakClosingHeaderBracketsMode(false);
      formatter.setBreakBlocksMode(false);
      formatter.setBreakElseIfsMode(false);
      formatter.setOperatorPaddingMode(false);
      formatter.setParensOutsidePaddingMode(false);
      formatter.setParensInsidePaddingMode(false);
      formatter.setParensUnPaddingMode(false);
      formatter.setSingleStatementsMode(true);
      formatter.setBreakOneLineBlocksMode(true);
      break;

    default: // Custom
    {
      bool value = cfg->ReadBool(wxT_2("/force_tabs"));
      int spaceNum = cfg->ReadInt(wxT_2("/indentation"), 4);

      if (cfg->ReadBool(wxT_2("/use_tabs")))
      {
        formatter.setTabIndentation(spaceNum, value);
      }
      else
      {
        formatter.setSpaceIndentation(spaceNum);
      }

      formatter.setClassIndent(cfg->ReadBool(wxT_2("/indent_classes")));
      formatter.setSwitchIndent(cfg->ReadBool(wxT_2("/indent_switches")));
      formatter.setCaseIndent(cfg->ReadBool(wxT_2("/indent_case")));
      formatter.setBracketIndent(cfg->ReadBool(wxT_2("/indent_brackets")));
      formatter.setBlockIndent(cfg->ReadBool(wxT_2("/indent_blocks")));
      formatter.setNamespaceIndent(cfg->ReadBool(wxT_2("/indent_namespaces")));
      formatter.setLabelIndent(cfg->ReadBool(wxT_2("/indent_labels")));
      formatter.setPreprocessorIndent(cfg->ReadBool(wxT_2("/indent_preprocessor")));

      wxString breakType = cfg->Read(wxT_2("/break_type"));

      if (breakType == wxT_2("Break"))
      {
        formatter.setBracketFormatMode(astyle::BREAK_MODE);
      }
      else if (breakType == wxT_2("Attach"))
      {
        formatter.setBracketFormatMode(astyle::ATTACH_MODE);
      }
      else if (breakType == wxT_2("Linux"))
      {
        formatter.setBracketFormatMode(astyle::BDAC_MODE);
      }
      else
      {
        formatter.setBracketFormatMode(astyle::NONE_MODE);
      }

      formatter.setBreakClosingHeaderBracketsMode(cfg->ReadBool(wxT_2("/break_closing")));
      formatter.setBreakBlocksMode(cfg->ReadBool(wxT_2("/break_blocks")));
      formatter.setBreakElseIfsMode(cfg->ReadBool(wxT_2("/break_elseifs")));
      formatter.setOperatorPaddingMode(cfg->ReadBool(wxT_2("/pad_operators")));
      formatter.setParensOutsidePaddingMode(cfg->ReadBool(wxT_2("/pad_parentheses_out")));
      formatter.setParensInsidePaddingMode(cfg->ReadBool(wxT_2("/pad_parentheses_in")));
      formatter.setParensUnPaddingMode(cfg->ReadBool(wxT_2("/unpad_parentheses")));
      formatter.setSingleStatementsMode(!cfg->ReadBool(wxT_2("/keep_complex")));
      formatter.setBreakOneLineBlocksMode(!cfg->ReadBool(wxT_2("/keep_blocks")));
      formatter.setTabSpaceConversionMode(cfg->ReadBool(wxT_2("/convert_tabs")));
      formatter.setEmptyLineFill(cfg->ReadBool(wxT_2("/fill_empty_lines")));
      break;
    }
  }
}
