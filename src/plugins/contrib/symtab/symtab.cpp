#include "sdk.h"

#ifndef CB_PRECOMP
  #include <wx/intl.h>
  #include <wx/string.h>
  #include "globals.h"
  #include "manager.h"
  #include "configmanager.h"
#endif

//#define TRACE_SYMTAB
#ifdef TRACE_SYMTAB
  #ifndef CB_PRECOMP
    #include "logmanager.h"
  #endif
#endif

#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include "symtab.h"
#include "symtabconfig.h"
#include "symtabexec.h"

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

// Register the plugin
namespace
{
  PluginRegistrant<SymTab> reg(wxT_2("SymTab"));
};

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

SymTab::SymTab() : CfgDlg(0L), ExeDlg(0L)
{
  //ctor
  if(!Manager::LoadResource(wxT_2("SymTab.zip")))
  {
    NotifyMissingFile(wxT_2("SymTab.zip"));
  }
}// SymTab

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

SymTab::~SymTab()
{
  //dtor
}// ~SymTab

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTab::OnAttach()
{
  // do whatever initialization you need for your plugin
  // NOTE: after this function, the inherited member variable
  // IsAttached() will be TRUE...
  // You should check for it in other functions, because if it
  // is FALSE, it means that the application did *not* "load"
  // (see: does not need) this plugin...
}// OnAttach

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

void SymTab::OnRelease(bool appShutDown)
{
  // do de-initialization for your plugin
  // if appShutDown is false, the plugin is unloaded because Code::Blocks is being shut down,
  // which means you must not use any of the SDK Managers
  // NOTE: after this function, the inherited member variable
  // IsAttached() will be FALSE...
  if (CfgDlg) {CfgDlg->Destroy(); CfgDlg = 0L;}
  if (ExeDlg) {ExeDlg->Destroy(); ExeDlg = 0L;}
}// OnRelease

/* ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- */

int SymTab::Execute()
{
  // if not attached, exit
  if (!IsAttached())
    return -1;

#ifdef TRACE_SYMTAB
	Manager::Get()->GetLogManager()->DebugLog(F(wxT_2("SymTab::Execute")));
#endif

  if (!CfgDlg)
    CfgDlg = new SymTabConfigDlg(Manager::Get()->GetAppWindow());

  if ((!CfgDlg) || (CfgDlg->Execute() != wxID_OK))
    return -1;

  // Load the config settings
  ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("symtab"));

  // Loading configuration
  struct_config config;
  config.choWhatToDo    = cfg->ReadInt (wxT_2("/what_to_do"),   0);

  config.txtLibraryPath = cfg->Read    (wxT_2("/library_path"), wxEmptyString);
  config.chkIncludeA    = cfg->ReadBool(wxT_2("/include_a"),    true);
  config.chkIncludeLib  = cfg->ReadBool(wxT_2("/include_lib"),  true);
  config.chkIncludeO    = cfg->ReadBool(wxT_2("/include_o"),    false);
  config.chkIncludeObj  = cfg->ReadBool(wxT_2("/include_obj"),  false);
  config.chkIncludeDll  = cfg->ReadBool(wxT_2("/include_dll"),  false);

  config.txtLibrary     = cfg->Read    (wxT_2("/library"),      wxEmptyString);

  config.txtSymbol      = cfg->Read    (wxT_2("/symbol"),       wxEmptyString);

  config.txtNM          = cfg->Read    (wxT_2("/nm"),           wxEmptyString);

  config.chkDebug       = cfg->ReadBool(wxT_2("/debug"),        false);
  config.chkDefined     = cfg->ReadBool(wxT_2("/defined"),      false);
  config.chkDemangle    = cfg->ReadBool(wxT_2("/demangle"),     false);
  config.chkExtern      = cfg->ReadBool(wxT_2("/extern"),       false);
  config.chkSpecial     = cfg->ReadBool(wxT_2("/special"),      false);
  config.chkSynthetic   = cfg->ReadBool(wxT_2("/synthetic"),    false);
  config.chkUndefined   = cfg->ReadBool(wxT_2("/undefined"),    false);

  // If we got this far, all is left is to call nm
  if (!ExeDlg)
    ExeDlg = new SymTabExecDlg(Manager::Get()->GetAppWindow());

  // Do we need to show the dialog (process successful)?
  if ((!ExeDlg) || (ExeDlg->Execute(config) != wxID_OK))
    return -1;

  return 0;
}// Execute
