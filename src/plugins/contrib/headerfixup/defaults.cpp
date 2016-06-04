/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision: 10596 $
 * $Id: defaults.cpp 10596 2015-11-21 21:44:20Z mortenmacfly $
 * $HeadURL: http://svn.code.sf.net/p/codeblocks/code/branches/release-16.xx/src/plugins/contrib/headerfixup/defaults.cpp $
 */

#include "bindings.h"
#include "globals.h"

// ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----

void Bindings::SetDefaults()
{
  SetDefaultsCodeBlocks();
  SetDefaultsWxWidgets();
  SetDefaultsSTL();
  SetDefaultsCLibrary();
}

void Bindings::SetDefaultsCodeBlocks()
{
    wxString strCodeBlocks = wxT_2(
    "AbstractJob;backgroundthread.h|"
    "AddBuildTarget;projectbuildtarget.h|"
    "AddFile;projectfile.h|"
    "Agony;backgroundthread.h|"
    "AnnoyingDialog;annoyingdialog.h|"
    "AppendArray;globals.h|"
    "AutoDetectCompilers;autodetectcompilers.h|"
    "BackgroundThread;backgroundthread.h|"
    "BackgroundThreadPool;backgroundthread.h|"
    "BlkAllc;blockallocated.h|"
    "BlockAllocated;blockallocated.h|"
    "BlockAllocator;blockallocated.h|"
    "cbAssert;cbexception.h|"
    "cbC2U;globals.h|"
    "cbCodeCompletionPlugin;cbplugin.h|"
    "cbCompilerPlugin;cbplugin.h|"
    "cbConfigurationDialog;configurationpanel.h|"
    "cbConfigurationPanel;configurationpanel.h|"
    "cbDebuggerPlugin;cbplugin.h|"
    "cbDirAccessCheck;globals.h|"
    "cbEditor;cbeditor.h|"
    "cbEditorPrintout;cbeditorprintout.h|"
    "cbEventFunctor;cbfunctor.h|"
    "cbException;cbexception.h|"
    "cbExecuteProcess;cbexecute.h|"
    "cbLoadBitmap;globals.h|"
    "cbMessageBox;globals.h|"
    "cbMimePlugin;cbplugin.h|"
    "cbPlugin;cbplugin.h|"
    "cbProject;cbproject.h|"
    "cbRead;globals.h|"
    "cbReadFileContents;globals.h|"
    "cbSaveTinyXMLDocument;globals.h|"
    "cbSaveToFile;globals.h|"
    "cbStyledTextCtrl;cbeditor.h|"
    "cbSyncExecute;cbexecute.h|"
    "cbThreadedTask;cbthreadtask.h|"
    "cbThreadPool;cbthreadpool.h|"
    "cbThrow;cbexception.h|"
    "cbTool;cbtool.h|"
    "cbToolPlugin;cbplugin.h|"
    "cbU2C;globals.h|"
    "cbWizardPlugin;cbplugin.h|"
    "cbWorkerThread;cbthreadpool_extras.h|"
    "cbWorkspace;cbworkspace.h|"
    "cbWrite;globals.h|"
    "CfgMgrBldr;configmanager.h|"
    "cgCompiler;cbplugin.h|"
    "cgContribPlugin;cbplugin.h|"
    "cgCorePlugin;cbplugin.h|"
    "cgEditor;cbplugin.h|"
    "cgUnknown;cbplugin.h|"
    "ChooseDirectory;globals.h|"
    "clogFull;compiler.h|"
    "clogNone;compiler.h|"
    "clogSimple;compiler.h|"
    "cltError;compiler.h|"
    "cltInfo;compiler.h|"
    "cltNormal;compiler.h|"
    "cltWarning;compiler.h|"
    "CodeBlocksDockEvent;sdk_events.h|"
    "CodeBlocksEvent;sdk_events.h|"
    "CodeBlocksLayoutEvent;sdk_events.h|"
    "CodeBlocksLogEvent;sdk_events.h|"
    "CompileOptionsBase;compileoptionsbase.h|"
    "Compiler;compiler.h|"
    "CompilerCommandGenerator;compilercommandgenerator.h|"
    "CompilerFactory;compilerfactory.h|"
    "CompilerOptions;compileroptions.h|"
    "CompilerPrograms;compiler.h|"
    "CompilerSwitches;compiler.h|"
    "CompilerTool;compiler.h|"
    "CompilerToolsVector;compiler.h|"
    "CompileTargetBase;compiletargetbase.h|"
    "CompOption;compileroptions.h|"
    "ConfigManager;configmanager.h|"
    "ConfigureToolsDlg;configuretoolsdlg.h|"
    "ConfigManagerContainer;configmanager.h|"
    "ConfirmReplaceDlg;confirmreplacedlg.h|"
    "CreateDir;globals.h|"
    "CreateDirRecursively;globals.h|"
    "CSS;loggers.h|"
    "Death;backgroundthread.h|"
    "DelayedDelete;filemanager.h|"
    "DetectEncodingAndConvert;globals.h|"
    "DuplicateBuildTarget;projectbuildtarget.h|"
    "EditArrayFileDlg;editarrayfiledlg.h|"
    "EditArrayOrderDlg;editarrayorderdlg.h|"
    "EditArrayStringDlg;editarraystringdlg.h|"
    "EditKeywordsDlg;editkeywordsdlg.h|"
    "EditorBase;editorbase.h|"
    "EditorColourSet;editorcolourset.h|"
    "EditorConfigurationDlg;editorconfigurationdlg.h|"
    "EditorHooks;editor_hooks.h|"
    "EditorLexerLoader;editorlexerloader.h|"
    "EditorManager;editormanager.h|"
    "EditPairDlg;editpairdlg.h|"
    "EditPathDlg;editpathdlg.h|"
    "EditToolDlg;edittooldlg.h|"
    "EncodingDetector;encodingdetector.h|"
    "ExternalDepsDlg;externaldepsdlg.h|"
    "FileGroups;filegroupsandmasks.h|"
    "FileLoader;filemanager.h|"
    "FileLogger;loggers.h|"
    "FileManager;filemanager.h|"
    "FileSet;projecttemplateloader.h|"
    "FileSetFile;projecttemplateloader.h|"
    "FilesGroupsAndMasks;filegroupsandmasks.h|"
    "FileTreeData;cbproject.h|"
    "FileType;globals.h|"
    "FileTypeOf;globals.h|"
    "FindDlg;finddlg.h|"
    "FindReplaceBase;findreplacebase.h|"
    "GenericMultiLineNotesDlg;genericmultilinenotesdlg.h|"
    "GetActiveEditor;editorbase.h|"
    "GetActiveProject;cbproject.h|"
    "GetArrayFromString;globals.h|"
    "GetBuiltinActiveEditor;cbeditor.h|"
    "GetBuiltinEditor;cbeditor.h|"
    "GetBuildTarget;projectbuildtarget.h|"
    "GetColourSet;editorcolourset.h|"
    "GetConfigManager;configmanager.h|"
    "GetConfigurationPanel;configurationpanel.h|"
    "GetCurrentlyCompilingTarget;projectbuildtarget.h|"
    "GetEditor;editorbase.h|"
    "GetEditorManager;editormanager.h|"
    "GetFile;projectfile.h|"
    "GetFileByFilename;projectfile.h|"
    "GetFileManager;filemanager.h|"
    "GetLogManager;logmanager.h|"
    "GetMacrosManager;macrosmanager.h|"
    "GetMessageManager;messagemanager.h|"
    "GetNotebook;wx/wxFlatNotebook/wxFlatNotebook.h|"
    "GetParentProject;cbproject.h|"
    "GetPersonalityManager;personalitymanager.h|"
    "GetPlatformsFromString;globals.h|"
    "GetPluginManager;pluginmanager.h|"
    "GetProjectConfigurationPanel;configurationpanel.h|"
    "GetProjectFile;projectfile.h|"
    "GetProjectManager;projectmanager.h|"
    "GetProjects;cbproject.h|"
    "GetScriptingManager;scriptingmanager.h|"
    "GetStringFromArray;globals.h|"
    "GetStringFromPlatforms;globals.h|"
    "GetToolsManager;toolsmanager.h|"
    "GetTopEditor;gettopeditor.h|"
    "GetUserVariableManager;uservarmanager.h|"
    "GetWorkspace;cbworkspace.h|"
    "HTMLFileLogger;loggers.h|"
    "IBaseLoader;ibaseloader.h|"
    "IBaseWorkspaceLoader;ibaseworkspaceloader.h|"
    "ID;id.h|"
    "IEventFunctorBase;cbfunctor.h|"
    "IFunctorBase;cbfunctor.h|"
    "ImportersGlobals;importer_globals.h|"
    "IncrementalSelectListDlg;incrementalselectlistdlg.h|"
    "InfoWindow;infowindow.h|"
    "IsBuiltinOpen;cbeditor.h|"
    "ISerializable;configmanager.h|"
    "IsOpen;cbproject.h|"
    "IsWindowReallyShown;globals.h|"
    "JobQueue;backgroundthread.h|"
    "FileLoader;filemanager.h|"
    "FileTreeData;cbproject.h|"
    "ListCtrlLogger;loggers.h|"
    "LoaderBase;filemanager.h|"
    "LoadPNGWindows2000Hack;globals.h|"
    "LoadProject;cbproject.h|"
    "Logger;logger.h|"
    "LogManager;logmanager.h|"
    "LogSlot;logmanager.h|"
    "MacrosManager;macrosmanager.h|"
    "MakeCommand;compiletargetbase.h|"
    "ManagedThread;managerthread.h|"
    "Manager;manager.h|"
    "MenuItemsManager;menuitemsmanager.h|"
    "MessageManager;messagemanager.h|"
    "Mgr;manager.h|"
    "MiscTreeItemData;misctreeitemdata.h|"
    "MultiSelectDlg;multiselectdlg.h|"
    "NewFromTemplateDlg;newfromtemplatedlg.h|"
    "NewProject;cbproject.h|"
    "NormalizePath;globals.h|"
    "NotifyMissingFile;globals.h|"
    "NullLoader;filemanager.h|"
    "NullLogger;logger.h|"
    "OptionColour;editorcolourset.h|"
    "OptionSet;editorcolourset.h|"
    "PCHMode;cbproject.h|"
    "pchObjectDir;cbproject.h|"
    "pchSourceDir;cbproject.h|"
    "pchSourceFile;cbproject.h|"
    "PlaceWindow;globals.h|"
    "PersonalityManager;personalitymanager.h|"
    "pfCustomBuild;projectfile.h|"
    "pfDetails;projectfile.h|"
    "PipedProcess;pipedprocess.h|"
    "PluginElement;pluginmanager.h|"
    "PluginInfo;pluginmanager.h|"
    "PluginManager;pluginmanager.h|"
    "PluginRegistrant;cbplugin.h|"
    "PluginsConfigurationDlg;pluginsconfigurationdlg.h|"
    "PluginType;globals.h|"
    "ProjectBuildTarget;projectbuildtarget.h|"
    "ProjectDepsDlg;projectdepsdlg.h|"
    "ProjectFile;projectfile.h|"
    "ProjectFileOptionsDlg;projectfileoptionsdlg.h|"
    "ProjectFilesVector;projectfile.h|"
    "ProjectLayoutLoader;projectlayoutloader.h|"
    "ProjectLoader;projectloader.h|"
    "ProjectLoaderHooks;projectloader_hooks.h|"
    "ProjectManager;projectmanager.h|"
    "ProjectOptionsDlg;projectoptionsdlg.h|"
    "ProjectsFileMasksDlg;projectsfilemasksdlg.h|"
    "ProjectTemplateLoader;projecttemplateloader.h|"
    "ptCodeCompletion;globals.h|"
    "ptCompiler;globals.h|"
    "ptDebugger;globals.h|"
    "ptMime;globals.h|"
    "ptNone;globals.h|"
    "ptOther;globals.h|"
    "ptTool;globals.h|"
    "ptWizard;globals.h|"
    "QuoteStringIfNeeded;globals.h|"
    "RegExStruct;compiler.h|"
    "ReplaceDlg;replacedlg.h|"
    "RestoreTreeState;globals.h|"
    "SaveTreeState;globals.h|"
    "ScriptingManager;scriptingmanager.h|"
    "ScriptSecurityWarningDlg;scriptsecuritywarningdlg.h|"
    "sdAllGlobal;configmanager.h|"
    "sdAllKnown;configmanager.h|"
    "sdAllUser;configmanager.h|"
    "sdBase;configmanager.h|"
    "sdConfig;configmanager.h|"
    "sdCurrent;configmanager.h|"
    "sdDataGlobal;configmanager.h|"
    "sdDataUser;configmanager.h|"
    "sdHome;configmanager.h|"
    "sdPath;configmanager.h|"
    "sdPluginsGlobal;configmanager.h|"
    "sdPluginsUser;configmanager.h|"
    "sdScriptsGlobal;configmanager.h|"
    "sdScriptsUser;configmanager.h|"
    "sdTemp;configmanager.h|"
    "SearchDirs;configmanager.h|"
    "SearchResultsLog;searchresultslog.h|"
    "SelectTargetDlg;selecttargetdlg.h|"
    "SeqDelete;safedelete.h|"
    "Stacker;infowindow.h|"
    "StdoutLogger;loggers.h|"
    "TemplateManager;templatemanager.h|"
    "TemplateOption;projecttemplateloader.h|"
    "TextCtrlLogger;loggers.h|"
    "TimestampTextCtrlLogger;loggers.h|"
    "ToolsManager;toolsmanager.h|"
    "ttCommandsOnly;compiletargetbase.h|"
    "ttConsoleOnly;compiletargetbase.h|"
    "ttDynamicLib;compiletargetbase.h|"
    "ttExecutable;compiletargetbase.h|"
    "ttNative;compiletargetbase.h|"
    "ttStaticLib;compiletargetbase.h|"
    "UnixFilename;globals.h|"
    "URLEncode;globals.h|"
    "URLLoader;filemanager.h|"
    "UsesCommonControls6;globals.h|"
    "UserVariableManager;uservarmanager.h|"
    "VirtualBuildTargetsDlg;virtualbuildtargetsdlg.h|"
    "WorkspaceLoader;workspaceloader.h|"
    "wxToolBarAddOnXmlHandler;xtra_res.h|"
    "wxBase64;base64.h|"
    "wxCrc32;crc32.h");

    const wxArrayString arCodeBlocks = GetArrayFromString(strCodeBlocks, wxT_2("|"));
    for(std::size_t i = 0; i < arCodeBlocks.GetCount(); ++i)
    {
        const wxArrayString arTmp = GetArrayFromString(arCodeBlocks.Item(i), wxT_2(";"));
        AddBinding(wxT_2("CodeBlocks"), arTmp.Item(0), arTmp.Item(1) );
    }
}// SetDefaultsCodeBlocks

void Bindings::SetDefaultsWxWidgets()
{
    /////////////
    // v 2.6.4 //
    /////////////

    // All macros
    wxString strWxWidgets_2_6_4 = wxT_2(
    "DECLARE_APP;wx/app.h|"
    "DECLARE_CLASS;wx/object.h|"
    "DECLARE_ABSTRACT_CLASS;wx/object.h|"
    "DECLARE_DYNAMIC_CLASS;wx/object.h|"
    "DECLARE_EVENT_TYPE;wx/event.h|"
    "DECLARE_EVENT_MACRO;wx/event.h|"
    "DECLARE_EVENT_TABLE_ENTRY;wx/event.h|"
    "IMPLEMENT_APP;wx/app.h|"
    "IMPLEMENT_ABSTRACT_CLASS;wx/object.h|"
    "IMPLEMENT_ABSTRACT_CLASS2;wx/object.h|"
    "IMPLEMENT_CLASS;wx/object.h|"
    "IMPLEMENT_CLASS2;wx/object.h|"
    "IMPLEMENT_DYNAMIC_CLASS;wx/object.h|"
    "IMPLEMENT_DYNAMIC_CLASS2;wx/object.h|"
    "DEFINE_EVENT_TYPE;wx/event.h|"
    "BEGIN_EVENT_TABLE;wx/event.h|"
    "END_EVENT_TABLE;wx/event.h|"
    "EVT_CUSTOM;wx/event.h|"
    "EVT_CUSTOM_RANGE;wx/event.h|"
    "EVT_COMMAND;wx/event.h|"
    "EVT_COMMAND_RANGE;wx/event.h|"
    "EVT_NOTIFY;wx/event.h|"
    "EVT_NOTIFY_RANGE;wx/event.h|"
    "EVT_BUTTON;wx/button.h|"
    "EVT_CHECKBOX;wx/checkbox.h|"
    "EVT_CHOICE;wx/choice.h|"
    "EVT_CHOICE;wx/choice.h|"
    "EVT_COMBOBOX;wx/combobox.h|"
    "EVT_LISTBOX;wx/listbox.h|"
    "EVT_LISTBOX_DCLICK;wx/listbox.h|"
    "EVT_RADIOBOX;wx/radiobox.h|"
    "EVT_RADIOBUTTON;wx/radiobut.h|"
    "EVT_SCROLLBAR;wx/scrolbar.h|"
    "EVT_SLIDER;wx/slider.h|"
    "EVT_TOGGLEBUTTON;wx/tglbtn.h|"
    "WX_APPEND_ARRAY;wx/dynarray.h|"
    "WX_CLEAR_ARRAY;wx/dynarray.h|"
    "WX_DECLARE_OBJARRAY;wx/dynarray.h|"
    "WX_DEFINE_ARRAY;wx/dynarray.h|"
    "WX_DEFINE_OBJARRAY;wx/dynarray.h|"
    "WX_DEFINE_SORTED_ARRAY;wx/dynarray.h|"
    "WX_DECLARE_STRING_HASH_MAP;wx/hashmap.h|"
    "WX_DECLARE_HASH_MAP;wx/hashmap.h|"
    "wxASSERT;wx/debug.h|"
    "wxASSERT_MIN_BITSIZE;wx/debug.h|"
    "wxASSERT_MSG;wx/debug.h|"
    "wxBITMAP;wx/gdicmn.h|"
    "wxCOMPILE_TIME_ASSERT;wx/debug.h|"
    "wxCOMPILE_TIME_ASSERT2;wx/debug.h|"
    "wxCRIT_SECT_DECLARE;wx/thread.h|"
    "wxCRIT_SECT_DECLARE_MEMBER;wx/thread.h|"
    "wxCRIT_SECT_LOCKER;wx/thread.h|"
    "wxDYNLIB_FUNCTION;wx/dynlib.h|"
    "wxENTER_CRIT_SECT;wx/thread.h|"
    "wxFAIL;wx/debug.h|"
    "wxFAIL_MSG;wx/debug.h|"
    "wxICON;wx/gdicmn.h|"
    "wxLEAVE_CRIT_SECT;wx/thread.h|"
    "wxLL;wx/longlong.h|"
    "wxTRANSLATE;wx/intl.h|"
    "wxULL;wx/longlong.h|"

    // All ::wx methods
    "wxBeginBusyCursor;wx/utils.h|"
    "wxBell;wx/utils.h|"
    "wxClientDisplayRect;wx/gdicmn.h|"
    "wxClipboardOpen;wx/clipbrd.h|"
    "wxCloseClipboard;wx/clipbrd.h|"
    "wxColourDisplay;wx/gdicmn.h|"
    "wxConcatFiles;wx/filefn.h|"
    "wxCopyFile;wx/filefn.h|"
    "wxCreateDynamicObject;wx/object.h|"
    "wxCreateFileTipProvider;wx/tipdlg.h|"
    "wxDDECleanUp;wx/dde.h|"
    "wxDDEInitialize;wx/dde.h|"
    "wxDebugMsg;wx/utils.h|"
    "wxDirExists;wx/filefn.h|"
    "wxDirSelector;wx/dirdlg.h|"
    "wxDisplayDepth;wx/gdicmn.h|"
    "wxDisplaySize;wx/gdicmn.h|"
    "wxDisplaySizeMM;wx/gdicmn.h|"
    "wxDos2UnixFilename;wx/filefn.h|"
    "wxDROP_ICON;wx/dnd.h|"
    "wxEmptyClipboard;wx/clipbrd.h|"
    "wxEnableTopLevelWindows;wx/utils.h|"
    "wxEndBusyCursor;wx/utils.h|"
    "wxEntry;wx/app.h|"
    "wxEnumClipboardFormats;wx/clipbrd.h|"
    "wxError;wx/utils.h|"
    "wxExecute;wx/utils.h|"
    "wxExit;wx/app.h|"
    "wxFatalError;wx/utils.h|"
    "wxFileExists;wx/filefn.h|"
    "wxFileModificationTime;wx/filefn.h|"
    "wxFileNameFromPath;wx/filefn.h|"
    "wxFileSelector;wx/filedlg.h|"
    "wxFindFirstFile;wx/filefn.h|"
    "wxFindMenuItemId;wx/utils.h|"
    "wxFindNextFile;wx/filefn.h|"
    "wxFindWindowAtPoint;wx/utils.h|"
    "wxFindWindowAtPointer;wx/windows.h|"
    "wxFindWindowByLabel;wx/utils.h|"
    "wxFindWindowByName;wx/utils.h|"
    "wxGetActiveWindow;wx/windows.h|"
    "wxGetApp;wx/app.h|"
    "wxGetBatteryState;wx/utils.h|"
    "wxGetClipboardData;wx/clipbrd.h|"
    "wxGetClipboardFormatName;wx/clipbrd.h|"
    "wxGetColourFromUser;wx/colordlg.h|"
    "wxGetCwd;wx/filefn.h|"
    "wxGetDiskSpace;wx/filefn.h|"
    "wxGetDisplayName;wx/utils.h|"
    "wxGetElapsedTime;wx/timer.h|"
    "wxGetEmailAddress;wx/utils.h|"
    "wxGetFileKind;wx/filefn.h|"
    "wxGetFontFromUser;wx/fontdlg.h|"
    "wxGetFreeMemory;wx/utils.h|"
    "wxGetFullHostName;wx/utils.h|"
    "wxGetHomeDir;wx/utils.h|"
    "wxGetHostName;wx/utils.h|"
    "wxGetKeyState;wx/utils.h|"
    "wxGetLocalTime;wx/timer.h|"
    "wxGetLocalTimeMillis;wx/timer.h|"
    "wxGetMousePosition;wx/utils.h|"
    "wxGetMouseState;wx/utils.h|"
    "wxGetMultipleChoice;wx/choicdlg.h|"
    "wxGetMultipleChoices;wx/choicdlg.h|"
    "wxGetNumberFromUser;wx/numdlg.h|"
    "wxGetOsDescription;wx/utils.h|"
    "wxGetOSDirectory;wx/filefn.h|"
    "wxGetOsVersion;wx/utils.h|"
    "wxGetPasswordFromUser;wx/textdlg.h|"
    "wxGetPowerType;wx/utils.h|"
    "wxGetPrinterCommand;wx/dcps.h|"
    "wxGetPrinterFile;wx/dcps.h|"
    "wxGetPrinterMode;wx/dcps.h|"
    "wxGetPrinterOptions;wx/dcps.h|"
    "wxGetPrinterOrientation;wx/dcps.h|"
    "wxGetPrinterPreviewCommand;wx/dcps.h|"
    "wxGetPrinterScaling;wx/dcps.h|"
    "wxGetPrinterTranslation;wx/dcps.h|"
    "wxGetProcessId;wx/utils.h|"
    "wxGetResource;wx/utils.h|"
    "wxGetSingleChoice;wx/choicdlg.h|"
    "wxGetSingleChoiceData;wx/choicdlg.h|"
    "wxGetSingleChoiceIndex;wx/choicdlg.h|"
    "wxGetStockLabel;wx/stockitem.h|"
    "wxGetTempFileName;wx/filefn.h|"
    "wxGetTextFromUser;wx/textdlg.h|"
    "wxGetTopLevelParent;wx/window.h|"
    "wxGetTranslation;wx/intl.h|"
    "wxGetUserHome;wx/utils.h|"
    "wxGetUserId;wx/utils.h|"
    "wxGetUserName;wx/utils.h|"
    "wxGetUTCTime;wx/timer.h|"
    "wxGetWorkingDirectory;wx/filefn.h|"
    "wxHandleFatalExceptions;wx/app.h|"
    "wxInitAllImageHandlers;wx/image.h|"
    "wxInitialize;wx/app.h|"
    "wxIsAbsolutePath;wx/filefn.h|"
    "wxIsBusy;wx/utils.h|"
    "wxIsClipboardFormatAvailable;wx/clipbrd.h|"
    "wxIsDebuggerRunning;wx/debug.h|"
    "wxIsEmpty;wx/wxchar.h|"
    "wxIsMainThread;wx/thread.h|"
    "wxIsWild;wx/filefn.h|"
    "wxKill;wx/app.h|"
    "wxLaunchDefaultBrowser;wx/utils.h|"
    "wxLoadUserResource;wx/utils.h|"
    "wxLogDebug;wx/log.h|"
    "wxLogError;wx/log.h|"
    "wxLogFatalError;wx/log.h|"
    "wxLogMessage;wx/log.h|"
    "wxLogStatus;wx/log.h|"
    "wxLogSysError;wx/log.h|"
    "wxLogTrace;wx/log.h|"
    "wxLogVerbose;wx/log.h|"
    "wxLogWarning;wx/log.h|"
    "wxMakeMetafilePlaceable;wx/gdicmn.h|"
    "wxMatchWild;wx/filefn.h|"
    "wxMessageBox;wx/msgdlg.h|"
    "wxMicroSleep;wx/utils.h|"
    "wxMilliSleep;wx/utils.h|"
    "wxMkdir;wx/filefn.h|"
    "wxMutexGuiEnter;wx/thread.h|"
    "wxMutexGuiLeave;wx/thread.h|"
    "wxNewId;wx/utils.h|"
    "wxNow;wx/utils.h|"
    "wxOnAssert;wx/debug.h|"
    "wxOpenClipboard;wx/clipbrd.h|"
    "wxParseCommonDialogsFilter;wx/filefn.h|"
    "wxPathOnly;wx/filefn.h|"
    "wxPostDelete;wx/utils.h|"
    "wxPostEvent;wx/app.h|"
    "wxRegisterClipboardFormat;wx/clipbrd.h|"
    "wxRegisterId;wx/utils.h|"
    "wxRemoveFile;wx/filefn.h|"
    "wxRenameFile;wx/filefn.h|"
    "wxRmdir;wx/filefn.h|"
    "wxSafeShowMessage;wx/log.h|"
    "wxSafeYield;wx/utils.h|"
    "wxSetClipboardData;wx/clipbrd.h|"
    "wxSetCursor;wx/gdicmn.h|"
    "wxSetDisplayName;wx/utils.h|"
    "wxSetPrinterCommand;wx/dcps.h|"
    "wxSetPrinterFile;wx/dcps.h|"
    "wxSetPrinterMode;wx/dcps.h|"
    "wxSetPrinterOptions;wx/dcps.h|"
    "wxSetPrinterOrientation;wx/dcps.h|"
    "wxSetPrinterPreviewCommand;wx/dcps.h|"
    "wxSetPrinterScaling;wx/dcps.h|"
    "wxSetPrinterTranslation;wx/dcps.h|"
    "wxSetWorkingDirectory;wx/filefn.h|"
    "wxShell;wx/utils.h|"
    "wxShowTip;wx/tipdlg.h|"
    "wxShutdown;wx/utils.h|"
    "wxSleep;wx/utils.h|"
    "wxSnprintf;wx/wxchar.h|"
    "wxSplitPath;wx/filefn.h|"
    "wxStartTimer;wx/timer.h|"
    "wxStrcmp;wx/wxchar.h|"
    "wxStricmp;wx/wxchar.h|"
    "wxStringEq;wx/utils.h|"
    "wxStripMenuCodes;wx/utils.h|"
    "wxStrlen;wx/wxchar.h|"
    "wxSysErrorCode;wx/log.h|"
    "wxSysErrorMsg;wx/log.h|"
    "wxTrace;wx/memory.h|"
    "wxTraceLevel;wx/memory.h|"
    "wxTransferFileToStream;wx/docview.h|"
    "wxTransferStreamToFile;wx/docview.h|"
    "wxTrap;wx/debug.h|"
    "wxUninitialize;wx/app.h|"
    "wxUnix2DosFilename;wx/filefn.h|"
    "wxUsleep;wx/utils.h|"
    "wxVsnprintf;wx/wxchar.h|"
    "wxWakeUpIdle;wx/app.h|"
    "wxWriteResource;wx/utils.h|"
    "wxYield;wx/app.h|"

    // All ::wx classes
    "wxAcceleratorEntry;wx/accel.h|"
    "wxAcceleratorTable;wx/accel.h|"
    "wxAccessible;wx/access.h|"
    "wxActivateEvent;wx/event.h|"
    "wxApp;wx/app.h|"
    "wxArchiveClassFactory;wx/archive.h|"
    "wxArchiveEntry;wx/archive.h|"
    "wxArchiveInputStream;wx/archive.h|"
    "wxArchiveIterator;wx/archive.h|"
    "wxArchiveNotifier;wx/archive.h|"
    "wxArchiveOutputStream;wx/archive.h|"
    "wxArray;wx/dynarray.h|"
    "wxArrayString;wx/arrstr.h|"
    "wxArtProvider;wx/artprov.h|"
    "wxAutomationObject;wx/msw/ole/automtn.h|"
    "wxBitmap;wx/bitmap.h|"
    "wxBitmapButton;wx/bmpbuttn.h|"
    "wxBitmapDataObject;wx/dataobj.h|"
    "wxBitmapHandler;wx/bitmap.h|"
    "wxBoxSizer;wx/sizer.h|"
    "wxBrush;wx/brush.h|"
    "wxBrushList;wx/gdicmn.h|"
    "wxBufferedDC;wx/dcbuffer.h|"
    "wxBufferedInputStream;wx/stream.h|"
    "wxBufferedOutputStream;wx/stream.h|"
    "wxBufferedPaintDC;wx/dcbuffer.h|"
    "wxBusyCursor;wx/utils.h|"
    "wxBusyInfo;wx/busyinfo.h|"
    "wxButton;wx/button.h|"
    "wxCalculateLayoutEvent;wx/laywin.h|"
    "wxCalendarCtrl;wx/calctrl.h|"
    "wxCalendarDateAttr;wx/calctrl.h|"
    "wxCalendarEvent;wx/calctrl.h|"
    "wxCaret;wx/caret.h|"
    "wxCheckBox;wx/checkbox.h|"
    "wxCheckListBox;wx/checklst.h|"
    "wxChoice;wx/choice.h|"
    "wxChoicebook;wx/choicebk.h|"
    "wxClassInfo;wx/object.h|"
    "wxClient;wx/ipc.h|"
    "wxClientData;wx/clntdata.h|"
    "wxClientDataContainer;wx/clntdata.h|"
    "wxClientDC;wx/dcclient.h|"
    "wxClipboard;wx/clipbrd.h|"
    "wxCloseEvent;wx/event.h|"
    "wxCmdLineParser;wx/cmdline.h|"
    "wxColour;wx/colour.h|"
    "wxColourData;wx/cmndata.h|"
    "wxColourDatabase;wx/gdicmn.h|"
    "wxColourDialog;wx/colordlg.h|"
    "wxComboBox;wx/combobox.h|"
    "wxCommand;wx/cmdproc.h|"
    "wxCommandEvent;wx/event.h|"
    "wxCommandProcessor;wx/cmdproc.h|"
    "wxCondition;wx/thread.h|"
    "wxConfigBase;wx/config.h|"
    "wxConnection;wx/ipc.h|"
    "wxContextHelp;wx/cshelp.h|"
    "wxContextHelpButton;wx/cshelp.h|"
    "wxContextMenuEvent;wx/event.h|"
    "wxControl;wx/control.h|"
    "wxControlWithItems;wx/ctrlsub.h|"
    "wxCountingOutputStream;wx/stream.h|"
    "wxCriticalSection;wx/thread.h|"
    "wxCriticalSectionLocker;wx/thread.h|"
    "wxCSConv;wx/strconv.h|"
    "wxCurrentTipProvider;wx/tipdlg.h|"
    "wxCursor;wx/cursor.h|"
    "wxCustomDataObject;wx/dataobj.h|"
    "wxDataFormat;wx/dataobj.h|"
    "wxDataInputStream;wx/datstrm.h|"
    "wxDataObject;wx/dataobj.h|"
    "wxDataObjectComposite;wx/dataobj.h|"
    "wxDataObjectSimple;wx/dataobj.h|"
    "wxDataOutputStream;wx/datstrm.h|"
    "wxDateEvent;wx/dateevt.h|"
    "wxDatePickerCtrl;wx/datectrl.h|"
    "wxDateSpan;wx/datetime.h|"
    "wxDateTime;wx/datetime.h|"
    "wxDb;wx/db.h|"
    "wxDbColDataPtr;wx/db.h|"
    "wxDbColDef;wx/db.h|"
    "wxDbColFor;wx/db.h|"
    "wxDbColInf;wx/db.h|"
    "wxDbConnectInf;wx/db.h|"
    "wxDbGridColInfo;wx/dbgrid.h|"
    "wxDbGridTableBase;wx/dbgrid.h|"
    "wxDbIdxDef;wx/db.h|"
    "wxDbInf;wx/db.h|"
    "wxDbTable;wx/dbtable.h|"
    "wxDbTableInf;wx/db.h|"
    "wxDC;wx/dc.h|"
    "wxDCClipper;wx/dc.h|"
    "wxDDEClient;wx/dde.h|"
    "wxDDEConnection;wx/dde.h|"
    "wxDDEServer;wx/dde.h|"
    "wxDebugContext;wx/memory.h|"
    "wxDebugReport;wx/debugrpt.h|"
    "wxDebugReportCompress;wx/debugrpt.h|"
    "wxDebugReportPreview;wx/debugrpt.h|"
    "wxDebugReportUpload;wx/debugrpt.h|"
    "wxDebugStreamBuf;wx/memory.h|"
    "wxDelegateRendererNative;wx/renderer.h|"
    "wxDialog;wx/dialog.h|"
    "wxDialUpEvent;wx/dialup.h|"
    "wxDialUpManager;wx/dialup.h|"
    "wxDir;wx/dir.h|"
    "wxDirDialog;wx/dirdlg.h|"
    "wxDirTraverser;wx/dir.h|"
    "wxDisplay;wx/display.h|"
    "wxDllLoader;wx/dynlib.h|"
    "wxDocChildFrame;wx/docview.h|"
    "wxDocManager;wx/docview.h|"
    "wxDocMDIChildFrame;wx/docmdi.h|"
    "wxDocMDIParentFrame;wx/docmdi.h|"
    "wxDocParentFrame;wx/docview.h|"
    "wxDocTemplate;wx/docview.h|"
    "wxDocument;wx/docview.h|"
    "wxDragImage;wx/dragimag.h|"
    "wxDragResult;wx/dnd.h|"
    "wxDropFilesEvent;wx/event.h|"
    "wxDropSource;wx/dnd.h|"
    "wxDropTarget;wx/dnd.h|"
    "wxDynamicLibrary;wx/dynlib.h|"
    "wxDynamicLibraryDetails;wx/dynlib.h|"
    "wxEncodingConverter;wx/encconv.h|"
    "wxEraseEvent;wx/event.h|"
    "wxEvent;wx/event.h|"
    "wxEvtHandler;wx/event.h|"
    "wxFFile;wx/ffile.h|"
    "wxFFileInputStream;wx/wfstream.h|"
    "wxFFileOutputStream;wx/wfstream.h|"
    "wxFFileStream;wx/wfstream.h|"
    "wxFile;wx/file.h|"
    "wxFileConfig;wx/fileconf.h|"
    "wxFileDataObject;wx/dataobj.h|"
    "wxFileDialog;wx/filedlg.h|"
    "wxFileDropTarget;wx/dnd.h|"
    "wxFileHistory;wx/docview.h|"
    "wxFileInputStream;wx/wfstream.h|"
    "wxFileName;wx/filename.h|"
    "wxFileOutputStream;wx/wfstream.h|"
    "wxFileStream;wx/wfstream.h|"
    "wxFileSystem;wx/filesys.h|"
    "wxFileSystemHandler;wx/filesys.h|"
    "wxFileType;wx/mimetype.h|"
    "wxFilterInputStream;wx/stream.h|"
    "wxFilterOutputStream;wx/stream.h|"
    "wxFindDialogEvent;wx/fdrepdlg.h|"
    "wxFindReplaceData;wx/fdrepdlg.h|"
    "wxFindReplaceDialog;wx/fdrepdlg.h|"
    "wxFinite;wx/math.h|"
    "wxFlexGridSizer;wx/sizer.h|"
    "wxFocusEvent;wx/event.h|"
    "wxFont;wx/font.h|"
    "wxFontData;wx/cmndata.h|"
    "wxFontDialog;wx/fontdlg.h|"
    "wxFontEnumerator;wx/fontenum.h|"
    "wxFontList;wx/gdicmn.h|"
    "wxFontMapper;wx/fontmap.h|"
    "wxFrame;wx/frame.h|"
    "wxFSFile;wx/filesys.h|"
    "wxFTP;wx/protocol/ftp.h|"
    "wxGauge;wx/gauge.h|"
    "wxGBPosition;wx/gbsizer.h|"
    "wxGBSizerItem;wx/gbsizer.h|"
    "wxGBSpan;wx/gbsizer.h|"
    "wxGDIObject;wx/gdiobj.h|"
    "wxGenericDirCtrl;wx/dirctrl.h|"
    "wxGenericValidator;wx/valgen.h|"
    "wxGetenv;wx/utils.h|"
    "wxGetVariantCast;wx/variant.h|"
    "wxGLCanvas;wx/glcanvas.h|"
    "wxGLContext;wx/glcanvas.h|"
    "wxGrid;wx/grid.h|"
    "wxGridBagSizer;wx/gbsizer.h|"
    "wxGridCellAttr;wx/grid.h|"
    "wxGridCellBoolEditor;wx/grid.h|"
    "wxGridCellBoolRenderer;wx/grid.h|"
    "wxGridCellChoiceEditor;wx/grid.h|"
    "wxGridCellEditor;wx/grid.h|"
    "wxGridCellFloatEditor;wx/grid.h|"
    "wxGridCellFloatRenderer;wx/grid.h|"
    "wxGridCellNumberEditor;wx/grid.h|"
    "wxGridCellNumberRenderer;wx/grid.h|"
    "wxGridCellRenderer;wx/grid.h|"
    "wxGridCellStringRenderer;wx/grid.h|"
    "wxGridCellTextEditor;wx/grid.h|"
    "wxGridEditorCreatedEvent;wx/grid.h|"
    "wxGridEvent;wx/grid.h|"
    "wxGridRangeSelectEvent;wx/grid.h|"
    "wxGridSizeEvent;wx/grid.h|"
    "wxGridSizer;wx/sizer.h|"
    "wxGridTableBase;wx/grid.h|"
    "wxHashMap;wx/hashmap.h|"
    "wxHashSet;wx/hashset.h|"
    "wxHashTable;wx/hash.h|"
    "wxHelpController;wx/help.h|"
    "wxHelpControllerHelpProvider;wx/cshelp.h|"
    "wxHelpEvent;wx/event.h|"
    "wxHelpProvider;wx/cshelp.h|"
    "wxHtmlCell;wx/html/htmlcell.h|"
    "wxHtmlColourCell;wx/html/htmlcell.h|"
    "wxHtmlContainerCell;wx/html/htmlcell.h|"
    "wxHtmlDCRenderer;wx/html/htmprint.h|"
    "wxHtmlEasyPrinting;wx/html/htmprint.h|"
    "wxHtmlFilter;wx/html/htmlfilt.h|"
    "wxHtmlHelpController;wx/html/helpctrl.h|"
    "wxHtmlHelpData;wx/html/helpdata.h|"
    "wxHtmlHelpFrame;wx/html/helpfrm.h|"
    "wxHtmlLinkInfo;wx/html/htmlcell.h|"
    "wxHtmlListBox;wx/htmllbox.h|"
    "wxHtmlParser;wx/html/htmlpars.h|"
    "wxHtmlPrintout;wx/html/htmprint.h|"
    "wxHtmlTag;wx/html/htmltag.h|"
    "wxHtmlTagHandler;wx/html/htmlpars.h|"
    "wxHtmlTagsModule;wx/html/winpars.h|"
    "wxHtmlWidgetCell;wx/html/htmlcell.h|"
    "wxHtmlWindow;wx/html/htmlwin.h|"
    "wxHtmlWinParser;wx/html/winpars.h|"
    "wxHtmlWinTagHandler;wx/html/winpars.h|"
    "wxHTTP;wx/protocol/http.h|"
    "wxIcon;wx/icon.h|"
    "wxIconBundle;wx/iconbndl.h|"
    "wxIconizeEvent;wx/event.h|"
    "wxIconLocation;wx/iconloc.h|"
    "wxIdleEvent;wx/event.h|"
    "wxImage;wx/image.h|"
    "wxImageHandler;wx/image.h|"
    "wxImageList;wx/imaglist.h|"
    "wxIndividualLayoutConstraint;wx/layout.h|"
    "wxInitDialogEvent;wx/event.h|"
    "wxInputStream;wx/stream.h|"
    "wxIPaddress;wx/socket.h|"
    "wxIPV4address;wx/socket.h|"
    "wxIsNaN;wx/math.h|"
    "wxJoystick;wx/joystick.h|"
    "wxJoystickEvent;wx/event.h|"
    "wxKeyEvent;wx/event.h|"
    "wxLayoutAlgorithm;wx/laywin.h|"
    "wxLayoutConstraints;wx/layout.h|"
    "wxList;wx/list.h|"
    "wxListbook;wx/listbook.h|"
    "wxListCtrl;wx/listctrl.h|"
    "wxListEvent;wx/listctrl.h|"
    "wxListItem;wx/listctrl.h|"
    "wxListItemAttr;wx/listctrl.h|"
    "wxListView;wx/listctrl.h|"
    "wxLocale;wx/intl.h|"
    "wxLog;wx/log.h|"
    "wxLogChain;wx/log.h|"
    "wxLogGui;wx/log.h|"
    "wxLogNull;wx/log.h|"
    "wxLogPassThrough;wx/log.h|"
    "wxLogStderr;wx/log.h|"
    "wxLogStream;wx/log.h|"
    "wxLogTextCtrl;wx/log.h|"
    "wxLogWindow;wx/log.h|"
    "wxLongLong;wx/longlong.h|"
    "wxLongLongFmtSpec;wx/longlong.h|"
    "wxMask;wx/bitmap.h|"
    "wxMaximizeEvent;wx/event.h|"
    "wxMBConv;wx/strconv.h|"
    "wxMBConvFile;wx/strconv.h|"
    "wxMBConvUTF16;wx/strconv.h|"
    "wxMBConvUTF32;wx/strconv.h|"
    "wxMBConvUTF7;wx/strconv.h|"
    "wxMBConvUTF8;wx/strconv.h|"
    "wxMDIChildFrame;wx/mdi.h|"
    "wxMDIClientWindow;wx/mdi.h|"
    "wxMDIParentFrame;wx/mdi.h|"
    "wxMediaCtrl;wx/mediactrl.h|"
    "wxMediaEvent;wx/mediactrl.h|"
    "wxMemoryBuffer;wx/buffer.h|"
    "wxMemoryDC;wx/dcmemory.h|"
    "wxMemoryFSHandler;wx/fs_mem.h|"
    "wxMemoryInputStream;wx/mstream.h|"
    "wxMemoryOutputStream;wx/mstream.h|"
    "wxMenu;wx/menu.h|"
    "wxMenuBar;wx/menu.h|"
    "wxMenuEvent;wx/event.h|"
    "wxMenuItem;wx/menuitem.h|"
    "wxMessageDialog;wx/msgdlg.h|"
    "wxMetafile;wx/metafile.h|"
    "wxMetafileDC;wx/metafile.h|"
    "wxMimeTypesManager;wx/mimetype.h|"
    "wxMiniFrame;wx/minifram.h|"
    "wxMirrorDC;wx/dcmirror.h|"
    "wxModule;wx/module.h|"
    "wxMouseCaptureChangedEvent;wx/event.h|"
    "wxMouseEvent;wx/event.h|"
    "wxMoveEvent;wx/event.h|"
    "wxMultiChoiceDialog;wx/choicdlg.h|"
    "wxMutex;wx/thread.h|"
    "wxMutexLocker;wx/thread.h|"
    "wxNode;wx/list.h|"
    "wxNotebook;wx/notebook.h|"
    "wxNotebookEvent;wx/notebook.h|"
    "wxNotebookSizer;wx/sizer.h|"
    "wxNotifyEvent;wx/event.h|"
    "wxObjArray;wx/dynarray.h|"
    "wxObject;wx/object.h|"
    "wxObjectRefData;wx/object.h|"
    "wxOpenErrorTraverser;wx/dir.h|"
    "wxOutputStream;wx/stream.h|"
    "wxPageSetupDialog;wx/printdlg.h|"
    "wxPageSetupDialogData;wx/cmndata.h|"
    "wxPaintDC;wx/dcclient.h|"
    "wxPaintEvent;wx/event.h|"
    "wxPalette;wx/palette.h|"
    "wxPanel;wx/panel.h|"
    "wxPaperSize;wx/cmndata.h|"
    "wxPasswordEntryDialog;wx/textdlg.h|"
    "wxPathList;wx/filefn.h|"
    "wxPen;wx/pen.h|"
    "wxPenList;wx/gdicmn.h|"
    "wxPoint;wx/gdicmn.h|"
    "wxPostScriptDC;wx/dcps.h|"
    "wxPreviewCanvas;wx/print.h|"
    "wxPreviewControlBar;wx/print.h|"
    "wxPreviewFrame;wx/print.h|"
    "wxPrintData;wx/cmndata.h|"
    "wxPrintDialog;wx/printdlg.h|"
    "wxPrintDialogData;wx/cmndata.h|"
    "wxPrinter;wx/print.h|"
    "wxPrinterDC;wx/dcprint.h|"
    "wxPrintout;wx/print.h|"
    "wxPrintPreview;wx/print.h|"
    "wxProcess;wx/process.h|"
    "wxProgressDialog;wx/progdlg.h|"
    "wxPropertySheetDialog;wx/propdlg.h|"
    "wxProtocol;wx/protocol/protocol.h|"
    "wxQuantize;wx/quantize.h|"
    "wxQueryLayoutInfoEvent;wx/laywin.h|"
    "wxRadioBox;wx/radiobox.h|"
    "wxRadioButton;wx/radiobut.h|"
    "wxRealPoint;wx/gdicmn.h|"
    "wxRect;wx/gdicmn.h|"
    "wxRecursionGuard;wx/recguard.h|"
    "wxRecursionGuardFlag;wx/recguard.h|"
    "wxRegEx;wx/regex.h|"
    "wxRegion;wx/region.h|"
    "wxRegionIterator;wx/region.h|"
    "wxRegKey;wx/msw/registry.h|"
    "wxRendererNative;wx/renderer.h|"
    "wxRendererVersion;wx/renderer.h|"
    "wxSashEvent;wx/sashwin.h|"
    "wxSashLayoutWindow;wx/laywin.h|"
    "wxSashWindow;wx/sashwin.h|"
    "wxScopedArray;wx/ptr_scpd.h|"
    "wxScopedPtr;wx/ptr_scpd.h|"
    "wxScopedTiedPtr;wx/ptr_scpd.h|"
    "wxScreenDC;wx/dcscreen.h|"
    "wxScrollBar;wx/scrolbar.h|"
    "wxScrolledWindow;wx/scrolwin.h|"
    "wxScrollEvent;wx/event.h|"
    "wxScrollWinEvent;wx/event.h|"
    "wxSemaphore;wx/thread.h|"
    "wxServer;wx/ipc.h|"
    "wxSetCursorEvent;wx/event.h|"
    "wxSetEnv;wx/utils.h|"
    "wxSimpleHelpProvider;wx/cshelp.h|"
    "wxSingleChoiceDialog;wx/choicdlg.h|"
    "wxSingleInstanceChecker;wx/snglinst.h|"
    "wxSize;wx/gdicmn.h|"
    "wxSizeEvent;wx/event.h|"
    "wxSizer;wx/sizer.h|"
    "wxSizerFlags;wx/sizer.h|"
    "wxSizerItem;wx/sizer.h|"
    "wxSlider;wx/slider.h|"
    "wxSockAddress;wx/socket.h|"
    "wxSocketBase;wx/socket.h|"
    "wxSocketClient;wx/socket.h|"
    "wxSocketEvent;wx/socket.h|"
    "wxSocketInputStream;wx/sckstrm.h|"
    "wxSocketOutputStream;wx/sckstrm.h|"
    "wxSocketServer;wx/socket.h|"
    "wxSound;wx/sound.h|"
    "wxSpinButton;wx/spinbutt.h|"
    "wxSpinCtrl;wx/spinctrl.h|"
    "wxSpinEvent;wx/spinctrl.h|"
    "wxSplashScreen;wx/splash.h|"
    "wxSplitterEvent;wx/splitter.h|"
    "wxSplitterRenderParams;wx/renderer.h|"
    "wxSplitterWindow;wx/splitter.h|"
    "wxStackFrame;wx/stackwalk.h|"
    "wxStackWalker;wx/stackwalk.h|"
    "wxStandardPaths;wx/stdpaths.h|"
    "wxStaticBitmap;wx/statbmp.h|"
    "wxStaticBox;wx/statbox.h|"
    "wxStaticLine;wx/statline.h|"
    "wxStaticText;wx/stattext.h|"
    "wxStatusBar;wx/statusbr.h|"
    "wxStdDialogButtonSizer;wx/sizer.h|"
    "wxStopWatch;wx/stopwatch.h|"
    "wxStreamBase;wx/stream.h|"
    "wxStreamBuffer;wx/stream.h|"
    "wxStreamToTextRedirector;wx/textctrl.h|"
    "wxString;wx/string.h|"
    "wxStringBuffer;wx/string.h|"
    "wxStringBufferLength;wx/string.h|"
    "wxStringClientData;clntdata.h|"
    "wxStringInputStream;wx/sstream.h|"
    "wxStringOutputStream;wx/sstream.h|"
    "wxStringTokenizer;wx/tokenzr.h|"
    "wxSystemOptions;wx/sysopt.h|"
    "wxSystemSettings;wx/settings.h|"
    "wxTaskBarIcon;wx/taskbar.h|"
    "wxTCPClient;wx/sckipc.h|"
    "wxTCPServer;wx/sckipc.h|"
    "wxTempFile;wx/file.h|"
    "wxTempFileOutputStream;wx/wfstream.h|"
    "wxTextAttr;wx/textctrl.h|"
    "wxTextCtrl;wx/textctrl.h|"
    "wxTextDataObject;wx/dataobj.h|"
    "wxTextDropTarget;wx/dnd.h|"
    "wxTextEntryDialog;wx/textdlg.h|"
    "wxTextFile;wx/textfile.h|"
    "wxTextInputStream;wx/txtstrm.h|"
    "wxTextOutputStream;wx/txtstrm.h|"
    "wxTextValidator;wx/valtext.h|"
    "wxTheClipboard;wx/clipbrd.h|"
    "wxThread;wx/thread.h|"
    "wxThreadHelper;wx/thread.h|"
    "wxTimer;wx/timer.h|"
    "wxTimerEvent;wx/timer.h|"
    "wxTimeSpan;wx/datetime.h|"
    "wxTipProvider;wx/tipdlg.h|"
    "wxTipWindow;wx/tipwin.h|"
    "wxToggleButton;wx/tglbtn.h|"
    "wxToolBar;wx/toolbar.h|"
    "wxToolTip;wx/tooltip.h|"
    "wxTopLevelWindow;wx/toplevel.h|"
    "wxTreeCtrl;wx/treectrl.h|"
    "wxTreeEvent;wx/treectrl.h|"
    "wxTreeItemData;wx/treectrl.h|"
    "wxUnsetEnv;wx/utils.h|"
    "wxUpdateUIEvent;wx/event.h|"
    "wxURI;wx/uri.h|"
    "wxURL;wx/url.h|"
    "wxVaCopy;wx/defs.h|"
    "wxValidator;wx/validate.h|"
    "wxVariant;wx/variant.h|"
    "wxVariantData;wx/variant.h|"
    "wxView;wx/docview.h|"
    "wxVListBox;wx/vlbox.h|"
    "wxVScrolledWindow;wx/vscroll.h|"
    "wxWindow;wx/window.h|"
    "wxWizard;wx/wizard.h|"
    "wxWizardEvent;wx/wizard.h|"
    "wxWizardPage;wx/wizard.h|"
    "wxWizardPageSimple;wx/wizard.h|"
    "wxXmlResource;wx/xrc/xmlres.h|"
    "wxXmlResourceHandler;wx/xrc/xmlres.h|"
    "wxZipClassFactory;wx/zipstrm.h|"
    "wxZipEntry;wx/zipstrm.h|"
    "wxZipInputStream;wx/zipstrm.h|"
    "wxZipNotifier;wx/zipstrm.h|"
    "wxZipOutputStream;wx/zipstrm.h|"
    "wxZlibInputStream;wx/zstream.h|"
    "wxZlibOutputStream;wx/zstream.h");

    const wxArrayString arWxWidgets_2_6_4 = GetArrayFromString(strWxWidgets_2_6_4, wxT_2("|"));
    for(std::size_t i = 0; i < arWxWidgets_2_6_4.GetCount(); ++i)
    {
        const wxArrayString arTmp = GetArrayFromString(arWxWidgets_2_6_4.Item(i), wxT_2(";"));
        AddBinding(wxT_2("wxWidgets_2_6_4"), arTmp.Item(0), arTmp.Item(1) );
    }

    /////////////
    // v 2.8.8 //
    /////////////

    // All macros
    wxString strWxWidgets_2_8_8 = wxT_2(
    "DECLARE_APP;wx/app.h|"
    "DECLARE_ABSTRACT_CLASS;wx/object.h|"
    "DECLARE_CLASS;wx/object.h|"
    "DECLARE_DYNAMIC_CLASS;wx/object.h|"
    "IMPLEMENT_APP;wx/app.h|"
    "IMPLEMENT_ABSTRACT_CLASS;wx/object.h|"
    "IMPLEMENT_ABSTRACT_CLASS2;wx/object.h|"
    "IMPLEMENT_CLASS;wx/object.h|"
    "IMPLEMENT_CLASS2;wx/object.h|"
    "IMPLEMENT_DYNAMIC_CLASS;wx/object.h|"
    "IMPLEMENT_DYNAMIC_CLASS2;wx/object.h|"
    "DECLARE_EVENT_TYPE;wx/event.h|"
    "DECLARE_EVENT_MACRO;wx/event.h|"
    "DECLARE_EVENT_TABLE_ENTRY;wx/event.h|"
    "DEFINE_EVENT_TYPE;wx/event.h|"
    "BEGIN_EVENT_TABLE;wx/event.h|"
    "END_EVENT_TABLE;wx/event.h|"
    "EVT_CUSTOM;wx/event.h|"
    "EVT_CUSTOM_RANGE;wx/event.h|"
    "EVT_COMMAND;wx/event.h|"
    "EVT_COMMAND_RANGE;wx/event.h|"
    "EVT_NOTIFY;wx/event.h|"
    "EVT_NOTIFY_RANGE;wx/event.h|"
    "EVT_BUTTON;wx/button.h|"
    "EVT_CHECKBOX;wx/checkbox.h|"
    "EVT_CHOICE;wx/choice.h|"
    "EVT_CHOICE;wx/choice.h|"
    "EVT_COMBOBOX;wx/combobox.h|"
    "EVT_LISTBOX;wx/listbox.h|"
    "EVT_LISTBOX_DCLICK;wx/listbox.h|"
    "EVT_RADIOBOX;wx/radiobox.h|"
    "EVT_RADIOBUTTON;wx/radiobut.h|"
    "EVT_SCROLLBAR;wx/scrolbar.h|"
    "EVT_SLIDER;wx/slider.h|"
    "EVT_TOGGLEBUTTON;wx/tglbtn.h|"
    "WX_APPEND_ARRAY;wx/dynarray.h|"
    "WX_PREPEND_ARRAY;wx/dynarray.h|"
    "WX_CLEAR_ARRAY;wx/dynarray.h|"
    "WX_DECLARE_OBJARRAY;wx/dynarray.h|"
    "WX_DEFINE_ARRAY;wx/dynarray.h|"
    "WX_DEFINE_OBJARRAY;wx/dynarray.h|"
    "WX_DEFINE_SORTED_ARRAY;wx/dynarray.h|"
    "WX_DECLARE_STRING_HASH_MAP;wx/hashmap.h|"
    "WX_DECLARE_HASH_MAP;wx/hashmap.h|"
    "wxASSERT;wx/debug.h|"
    "wxASSERT_MIN_BITSIZE;wx/debug.h|"
    "wxASSERT_MSG;wx/debug.h|"
    "wxBITMAP;wx/gdicmn.h|"
    "wxCOMPILE_TIME_ASSERT;wx/debug.h|"
    "wxCOMPILE_TIME_ASSERT2;wx/debug.h|"
    "wxCRIT_SECT_DECLARE;wx/thread.h|"
    "wxCRIT_SECT_DECLARE_MEMBER;wx/thread.h|"
    "wxCRIT_SECT_LOCKER;wx/thread.h|"
    "wxDYNLIB_FUNCTION;wx/dynlib.h|"
    "wxENTER_CRIT_SECT;wx/thread.h|"
    "wxFAIL;wx/debug.h|"
    "wxFAIL_MSG;wx/debug.h|"
    "wxICON;wx/gdicmn.h|"
    "wxLEAVE_CRIT_SECT;wx/thread.h|"
    "wxLL;wx/longlong.h|"
    "wxTRANSLATE;wx/intl.h|"
    "wxULL;wx/longlong.h|"

    // All ::wx methods
    "wxAboutBox;wx/aboutdlg.h|"
    "wxBeginBusyCursor;wx/utils.h|"
    "wxBell;wx/utils.h|"
    "wxClientDisplayRect;wx/gdicmn.h|"
    "wxClipboardOpen;wx/clipbrd.h|"
    "wxCloseClipboard;wx/clipbrd.h|"
    "wxColourDisplay;wx/gdicmn.h|"
    "wxConcatFiles;wx/filefn.h|"
    "wxCopyFile;wx/filefn.h|"
    "wxCreateDynamicObject;wx/object.h|"
    "wxCreateFileTipProvider;wx/tipdlg.h|"
    "wxDDECleanUp;wx/dde.h|"
    "wxDDEInitialize;wx/dde.h|"
    "wxDebugMsg;wx/utils.h|"
    "wxDirExists;wx/filefn.h|"
    "wxDirSelector;wx/dirdlg.h|"
    "wxDisplayDepth;wx/gdicmn.h|"
    "wxDisplaySize;wx/gdicmn.h|"
    "wxDisplaySizeMM;wx/gdicmn.h|"
    "wxDos2UnixFilename;wx/filefn.h|"
    "wxDROP_ICON;wx/dnd.h|"
    "wxEmptyClipboard;wx/clipbrd.h|"
    "wxEnableTopLevelWindows;wx/utils.h|"
    "wxEndBusyCursor;wx/utils.h|"
    "wxEntry;wx/app.h|"
    "wxEntryCleanup;wx/init.h|"
    "wxEntryStart;wx/init.h|"
    "wxEnumClipboardFormats;wx/clipbrd.h|"
    "wxError;wx/utils.h|"
    "wxExecute;wx/utils.h|"
    "wxExit;wx/app.h|"
    "wxFatalError;wx/utils.h|"
    "wxFileExists;wx/filefn.h|"
    "wxFileModificationTime;wx/filefn.h|"
    "wxFileNameFromPath;wx/filefn.h|"
    "wxFileSelector;wx/filedlg.h|"
    "wxFindFirstFile;wx/filefn.h|"
    "wxFindMenuItemId;wx/utils.h|"
    "wxFindNextFile;wx/filefn.h|"
    "wxFindWindowAtPoint;wx/utils.h|"
    "wxFindWindowAtPointer;wx/windows.h|"
    "wxFindWindowByLabel;wx/utils.h|"
    "wxFindWindowByName;wx/utils.h|"
    "wxGenericAboutBox;wx/aboutdlg.h\nwx/generic/aboutdlgg.h|"
    "wxGetActiveWindow;wx/windows.h|"
    "wxGetApp;wx/app.h|"
    "wxGetBatteryState;wx/utils.h|"
    "wxGetClipboardData;wx/clipbrd.h|"
    "wxGetClipboardFormatName;wx/clipbrd.h|"
    "wxGetColourFromUser;wx/colordlg.h|"
    "wxGetCwd;wx/filefn.h|"
    "wxGetDiskSpace;wx/filefn.h|"
    "wxGetDisplayName;wx/utils.h|"
    "wxGetElapsedTime;wx/timer.h|"
    "wxGetEmailAddress;wx/utils.h|"
    "wxGetFileKind;wx/filefn.h|"
    "wxGetFontFromUser;wx/fontdlg.h|"
    "wxGetFreeMemory;wx/utils.h|"
    "wxGetFullHostName;wx/utils.h|"
    "wxGetHomeDir;wx/utils.h|"
    "wxGetHostName;wx/utils.h|"
    "wxGetKeyState;wx/utils.h|"
    "wxGetLocalTime;wx/timer.h|"
    "wxGetLocalTimeMillis;wx/timer.h|"
    "wxGetMousePosition;wx/utils.h|"
    "wxGetMouseState;wx/utils.h|"
    "wxGetMultipleChoice;wx/choicdlg.h|"
    "wxGetMultipleChoices;wx/choicdlg.h|"
    "wxGetNumberFromUser;wx/numdlg.h|"
    "wxGetOsDescription;wx/utils.h|"
    "wxGetOSDirectory;wx/filefn.h|"
    "wxGetOsVersion;wx/utils.h|"
    "wxGetPasswordFromUser;wx/textdlg.h|"
    "wxGetPowerType;wx/utils.h|"
    "wxGetPrinterCommand;wx/dcps.h|"
    "wxGetPrinterFile;wx/dcps.h|"
    "wxGetPrinterMode;wx/dcps.h|"
    "wxGetPrinterOptions;wx/dcps.h|"
    "wxGetPrinterOrientation;wx/dcps.h|"
    "wxGetPrinterPreviewCommand;wx/dcps.h|"
    "wxGetPrinterScaling;wx/dcps.h|"
    "wxGetPrinterTranslation;wx/dcps.h|"
    "wxGetProcessId;wx/utils.h|"
    "wxGetResource;wx/utils.h|"
    "wxGetSingleChoice;wx/choicdlg.h|"
    "wxGetSingleChoiceData;wx/choicdlg.h|"
    "wxGetSingleChoiceIndex;wx/choicdlg.h|"
    "wxGetStockLabel;wx/stockitem.h|"
    "wxGetTempFileName;wx/filefn.h|"
    "wxGetTextFromUser;wx/textdlg.h|"
    "wxGetTopLevelParent;wx/window.h|"
    "wxGetTranslation;wx/intl.h|"
    "wxGetUserHome;wx/utils.h|"
    "wxGetUserId;wx/utils.h|"
    "wxGetUserName;wx/utils.h|"
    "wxGetUTCTime;wx/timer.h|"
    "wxGetWorkingDirectory;wx/filefn.h|"
    "wxHandleFatalExceptions;wx/app.h|"
    "wxInitAllImageHandlers;wx/image.h|"
    "wxInitialize;wx/app.h|"
    "wxIsAbsolutePath;wx/filefn.h|"
    "wxIsBusy;wx/utils.h|"
    "wxIsClipboardFormatAvailable;wx/clipbrd.h|"
    "wxIsDebuggerRunning;wx/debug.h|"
    "wxIsEmpty;wx/wxchar.h|"
    "wxIsMainThread;wx/thread.h|"
    "wxIsPlatform64Bit;wx/utils.h|"
    "wxIsPlatformLittleEndian;wx/utils.h|"
    "wxIsWild;wx/filefn.h|"
    "wxKill;wx/app.h|"
    "wxLaunchDefaultBrowser;wx/utils.h|"
    "wxLoadUserResource;wx/utils.h|"
    "wxLogDebug;wx/log.h|"
    "wxLogError;wx/log.h|"
    "wxLogFatalError;wx/log.h|"
    "wxLogMessage;wx/log.h|"
    "wxLogStatus;wx/log.h|"
    "wxLogSysError;wx/log.h|"
    "wxLogTrace;wx/log.h|"
    "wxLogVerbose;wx/log.h|"
    "wxLogWarning;wx/log.h|"
    "wxMakeMetafilePlaceable;wx/gdicmn.h|"
    "wxMatchWild;wx/filefn.h|"
    "wxMessageBox;wx/msgdlg.h|"
    "wxMicroSleep;wx/utils.h|"
    "wxMilliSleep;wx/utils.h|"
    "wxMkdir;wx/filefn.h|"
    "wxMutexGuiEnter;wx/thread.h|"
    "wxMutexGuiLeave;wx/thread.h|"
    "wxNewId;wx/utils.h|"
    "wxNow;wx/utils.h|"
    "wxOnAssert;wx/debug.h|"
    "wxOpenClipboard;wx/clipbrd.h|"
    "wxParseCommonDialogsFilter;wx/filefn.h|"
    "wxPathOnly;wx/filefn.h|"
    "wxPostDelete;wx/utils.h|"
    "wxPostEvent;wx/app.h|"
    "wxRegisterClipboardFormat;wx/clipbrd.h|"
    "wxRegisterId;wx/utils.h|"
    "wxRemoveFile;wx/filefn.h|"
    "wxRenameFile;wx/filefn.h|"
    "wxRmdir;wx/filefn.h|"
    "wxSafeShowMessage;wx/log.h|"
    "wxSafeYield;wx/utils.h|"
    "wxSetClipboardData;wx/clipbrd.h|"
    "wxSetCursor;wx/gdicmn.h|"
    "wxSetDisplayName;wx/utils.h|"
    "wxSetPrinterCommand;wx/dcps.h|"
    "wxSetPrinterFile;wx/dcps.h|"
    "wxSetPrinterMode;wx/dcps.h|"
    "wxSetPrinterOptions;wx/dcps.h|"
    "wxSetPrinterOrientation;wx/dcps.h|"
    "wxSetPrinterPreviewCommand;wx/dcps.h|"
    "wxSetPrinterScaling;wx/dcps.h|"
    "wxSetPrinterTranslation;wx/dcps.h|"
    "wxSetWorkingDirectory;wx/filefn.h|"
    "wxShell;wx/utils.h|"
    "wxShowTip;wx/tipdlg.h|"
    "wxShutdown;wx/utils.h|"
    "wxSleep;wx/utils.h|"
    "wxSnprintf;wx/wxchar.h|"
    "wxSplitPath;wx/filefn.h|"
    "wxStartTimer;wx/timer.h|"
    "wxStrcmp;wx/wxchar.h|"
    "wxStricmp;wx/wxchar.h|"
    "wxStringEq;wx/string.h|"
    "wxStringMatch;wx/string.h|"
    "wxStringTokenize;wx/string.h|"
    "wxStripMenuCodes;wx/utils.h|"
    "wxStrlen;wx/wxchar.h|"
    "wxSysErrorCode;wx/log.h|"
    "wxSysErrorMsg;wx/log.h|"
    "wxTrace;wx/memory.h|"
    "wxTraceLevel;wx/memory.h|"
    "wxTransferFileToStream;wx/docview.h|"
    "wxTransferStreamToFile;wx/docview.h|"
    "wxTrap;wx/debug.h|"
    "wxUninitialize;wx/app.h|"
    "wxUnix2DosFilename;wx/filefn.h|"
    "wxUsleep;wx/utils.h|"
    "wxVsnprintf;wx/wxchar.h|"
    "wxWakeUpIdle;wx/app.h|"
    "wxWriteResource;wx/utils.h|"
    "wxYield;wx/app.h|"

    // All ::wx classes
    "wxAboutDialogInfo;wx/aboutdlg.h|"
    "wxAcceleratorEntry;wx/accel.h|"
    "wxAcceleratorTable;wx/accel.h|"
    "wxAccessible;wx/access.h|"
    "wxActivateEvent;wx/event.h|"
    "wxActiveXContainer;wx/msw/ole/activex.h|"
    "wxActiveXEvent;wx/msw/ole/activex.h|"
    "wxAnimation;wx/animate.h|"
    "wxAnimationCtrl;wx/animate.h|"
    "wxApp;wx/app.h|"
    "wxAppTraits;wx/apptrait.h|"
    "wxArchiveClassFactory;wx/archive.h|"
    "wxArchiveEntry;wx/archive.h|"
    "wxArchiveInputStream;wx/archive.h|"
    "wxArchiveIterator;wx/archive.h|"
    "wxArchiveNotifier;wx/archive.h|"
    "wxArchiveOutputStream;wx/archive.h|"
    "wxArray;wx/dynarray.h|"
    "wxArrayString;wx/arrstr.h|"
    "wxArtProvider;wx/artprov.h|"
    "wxAuiDockArt;wx/aui/dockart.h|"
    "wxAuiTabArt;wx/aui/auibook.h|"
    "wxAuiManager;wx/aui/aui.h|"
    "wxAuiNotebook;wx/aui/auibook.h|"
    "wxAuiPaneInfo;wx/aui/aui.h|"
    "wxAutomationObject;wx/msw/ole/automtn.h|"
    "wxBitmap;wx/bitmap.h|"
    "wxBitmapButton;wx/bmpbuttn.h|"
    "wxBitmapComboBox;wx/bmpcbox.h|"
    "wxBitmapDataObject;wx/dataobj.h|"
    "wxBitmapHandler;wx/bitmap.h|"
    "wxBoxSizer;wx/sizer.h|"
    "wxBrush;wx/brush.h|"
    "wxBrushList;wx/gdicmn.h|"
    "wxBufferedDC;wx/dcbuffer.h|"
    "wxBufferedInputStream;wx/stream.h|"
    "wxBufferedOutputStream;wx/stream.h|"
    "wxBufferedPaintDC;wx/dcbuffer.h|"
    "wxBusyCursor;wx/utils.h|"
    "wxBusyInfo;wx/busyinfo.h|"
    "wxButton;wx/button.h|"
    "wxCalculateLayoutEvent;wx/laywin.h|"
    "wxCalendarCtrl;wx/calctrl.h|"
    "wxCalendarDateAttr;wx/calctrl.h|"
    "wxCalendarEvent;wx/calctrl.h|"
    "wxCaret;wx/caret.h|"
    "wxCheckBox;wx/checkbox.h|"
    "wxCheckListBox;wx/checklst.h|"
    "wxChildFocusEvent;wx/event.h|"
    "wxChoice;wx/choice.h|"
    "wxChoicebook;wx/choicebk.h|"
    "wxClassInfo;wx/object.h|"
    "wxClient;wx/ipc.h|"
    "wxClientData;wx/clntdata.h|"
    "wxClientDataContainer;wx/clntdata.h|"
    "wxClientDC;wx/dcclient.h|"
    "wxClipboard;wx/clipbrd.h|"
    "wxClipboardTextEvent;wx/event.h|"
    "wxCloseEvent;wx/event.h|"
    "wxCmdLineParser;wx/cmdline.h|"
    "wxCollapsiblePane;wx/collpane.h|"
    "wxCollapsiblePaneEvent;wx/collpane.h|"
    "wxColour;wx/colour.h|"
    "wxColourData;wx/cmndata.h|"
    "wxColourDatabase;wx/gdicmn.h|"
    "wxColourDialog;wx/colordlg.h|"
    "wxColourPickerCtrl;wx/clrpicker.h|"
    "wxColourPickerEvent;wx/clrpicker.h|"
    "wxComboBox;wx/combobox.h|"
    "wxComboCtrl;wx/combo.h|"
    "wxComboPopup;wx/combo.h|"
    "wxCommand;wx/cmdproc.h|"
    "wxCommandEvent;wx/event.h|"
    "wxCommandProcessor;wx/cmdproc.h|"
    "wxCondition;wx/thread.h|"
    "wxConfigBase;wx/config.h|"
    "wxConnection;wx/ipc.h|"
    "wxContextHelp;wx/cshelp.h|"
    "wxContextHelpButton;wx/cshelp.h|"
    "wxContextMenuEvent;wx/event.h|"
    "wxControl;wx/control.h|"
    "wxControlWithItems;wx/ctrlsub.h|"
    "wxCountingOutputStream;wx/stream.h|"
    "wxCriticalSection;wx/thread.h|"
    "wxCriticalSectionLocker;wx/thread.h|"
    "wxCSConv;wx/strconv.h|"
    "wxCursor;wx/cursor.h|"
    "wxCustomDataObject;wx/dataobj.h|"
    "wxDataFormat;wx/dataobj.h|"
    "wxDatagramSocket;wx/socket.h|"
    "wxDataInputStream;wx/datstrm.h|"
    "wxDataObject;wx/dataobj.h|"
    "wxDataObjectComposite;wx/dataobj.h|"
    "wxDataObjectSimple;wx/dataobj.h|"
    "wxDataOutputStream;wx/datstrm.h|"
    "wxDataViewColumn;wx/dataview.h|"
    "wxDataViewCtrl;wx/dataview.h|"
    "wxDataViewEvent;wx/dataview.h|"
    "wxDataViewListModelNotifier;wx/dataview.h|"
    "wxDataViewModel;wx/dataview.h|"
    "wxDataViewListModel;wx/dataview.h|"
    "wxDataViewSortedListModel;wx/dataview.h|"
    "wxDataViewRenderer;wx/dataview.h|"
    "wxDataViewTextRenderer;wx/dataview.h|"
    "wxDataViewProgressRenderer;wx/dataview.h|"
    "wxDataViewToggleRenderer;wx/dataview.h|"
    "wxDataViewBitmapRenderer;wx/dataview.h|"
    "wxDataViewDateRenderer;wx/dataview.h|"
    "wxDataViewCustomRenderer;wx/dataview.h|"
    "wxDateEvent;wx/dateevt.h|"
    "wxDatePickerCtrl;wx/datectrl.h|"
    "wxDateSpan;wx/datetime.h|"
    "wxDateTime;wx/datetime.h|"
    "wxDb;wx/db.h|"
    "wxDbColDataPtr;wx/db.h|"
    "wxDbColDef;wx/db.h|"
    "wxDbColFor;wx/db.h|"
    "wxDbColInf;wx/db.h|"
    "wxDbConnectInf;wx/db.h|"
    "wxDbGridColInfo;wx/dbgrid.h|"
    "wxDbGridTableBase;wx/dbgrid.h|"
    "wxDbIdxDef;wx/db.h|"
    "wxDbInf;wx/db.h|"
    "wxDbTable;wx/dbtable.h|"
    "wxDbTableInf;wx/db.h|"
    "wxDC;wx/dc.h|"
    "wxDCClipper;wx/dc.h|"
    "wxDDEClient;wx/dde.h|"
    "wxDDEConnection;wx/dde.h|"
    "wxDDEServer;wx/dde.h|"
    "wxDebugContext;wx/memory.h|"
    "wxDebugReport;wx/debugrpt.h|"
    "wxDebugReportCompress;wx/debugrpt.h|"
    "wxDebugReportPreview;wx/debugrpt.h|"
    "wxDebugReportPreviewStd;wx/debugrpt.h|"
    "wxDebugReportUpload;wx/debugrpt.h|"
    "wxDebugStreamBuf;wx/memory.h|"
    "wxDelegateRendererNative;wx/renderer.h|"
    "wxDialog;wx/dialog.h|"
    "wxDialUpEvent;wx/dialup.h|"
    "wxDialUpManager;wx/dialup.h|"
    "wxDir;wx/dir.h|"
    "wxDirDialog;wx/dirdlg.h|"
    "wxDirPickerCtrl;wx/filepicker.h|"
    "wxDirTraverser;wx/dir.h|"
    "wxDisplay;wx/display.h|"
    "wxDllLoader;wx/dynlib.h|"
    "wxDocChildFrame;wx/docview.h|"
    "wxDocManager;wx/docview.h|"
    "wxDocMDIChildFrame;wx/docmdi.h|"
    "wxDocMDIParentFrame;wx/docmdi.h|"
    "wxDocParentFrame;wx/docview.h|"
    "wxDocTemplate;wx/docview.h|"
    "wxDocument;wx/docview.h|"
    "wxDragImage;wx/dragimag.h|"
    "wxDragResult;wx/dnd.h|"
    "wxDropFilesEvent;wx/event.h|"
    "wxDropSource;wx/dnd.h|"
    "wxDropTarget;wx/dnd.h|"
    "wxDynamicLibrary;wx/dynlib.h|"
    "wxDynamicLibraryDetails;wx/dynlib.h|"
    "wxEncodingConverter;wx/encconv.h|"
    "wxEraseEvent;wx/event.h|"
    "wxEvent;wx/event.h|"
    "wxEvtHandler;wx/event.h|"
    "wxFFile;wx/ffile.h|"
    "wxFFileInputStream;wx/wfstream.h|"
    "wxFFileOutputStream;wx/wfstream.h|"
    "wxFFileStream;wx/wfstream.h|"
    "wxFile;wx/file.h|"
    "wxFileConfig;wx/fileconf.h|"
    "wxFileDataObject;wx/dataobj.h|"
    "wxFileDialog;wx/filedlg.h|"
    "wxFileDropTarget;wx/dnd.h|"
    "wxFileHistory;wx/docview.h|"
    "wxFileInputStream;wx/wfstream.h|"
    "wxFileName;wx/filename.h|"
    "wxFileOutputStream;wx/wfstream.h|"
    "wxFilePickerCtrl;wx/filepicker.h|"
    "wxFileDirPickerEvent;wx/filepicker.h|"
    "wxFileStream;wx/wfstream.h|"
    "wxFileSystem;wx/filesys.h|"
    "wxFileSystemHandler;wx/filesys.h|"
    "wxFileType;wx/mimetype.h|"
    "wxFilterClassFactory;wx/stream.h|"
    "wxFilterInputStream;wx/stream.h|"
    "wxFilterOutputStream;wx/stream.h|"
    "wxFindDialogEvent;wx/fdrepdlg.h|"
    "wxFindReplaceData;wx/fdrepdlg.h|"
    "wxFindReplaceDialog;wx/fdrepdlg.h|"
    "wxFinite;wx/math.h|"
    "wxFlexGridSizer;wx/sizer.h|"
    "wxFocusEvent;wx/event.h|"
    "wxFont;wx/font.h|"
    "wxFontData;wx/cmndata.h|"
    "wxFontDialog;wx/fontdlg.h|"
    "wxFontEnumerator;wx/fontenum.h|"
    "wxFontList;wx/gdicmn.h|"
    "wxFontMapper;wx/fontmap.h|"
    "wxFontPickerCtrl;wx/fontpicker.h|"
    "wxFontPickerEvent;wx/fontpicker.h|"
    "wxFrame;wx/frame.h|"
    "wxFSFile;wx/filesys.h|"
    "wxFTP;wx/protocol/ftp.h|"
    "wxGauge;wx/gauge.h|"
    "wxGBPosition;wx/gbsizer.h|"
    "wxGBSizerItem;wx/gbsizer.h|"
    "wxGBSpan;wx/gbsizer.h|"
    "wxGDIObject;wx/gdiobj.h|"
    "wxGenericDirCtrl;wx/dirctrl.h|"
    "wxGenericValidator;wx/valgen.h|"
    "wxGetenv;wx/utils.h|"
    "wxGetVariantCast;wx/variant.h|"
    "wxGLCanvas;wx/glcanvas.h|"
    "wxGLContext;wx/glcanvas.h|"
    "wxGraphicsBrush;wx/graphics.h|"
    "wxGraphicsContext;wx/graphics.h|"
    "wxGraphicsFont;wx/graphics.h|"
    "wxGraphicsMatrix;wx/graphics.h|"
    "wxGraphicsObject;wx/graphics.h|"
    "wxGraphicsPath;wx/graphics.h|"
    "wxGraphicsPen;wx/graphics.h|"
    "wxGraphicsRenderer;wx/graphics.h|"
    "wxGrid;wx/grid.h|"
    "wxGridBagSizer;wx/gbsizer.h|"
    "wxGridCellAttr;wx/grid.h|"
    "wxGridCellBoolEditor;wx/grid.h|"
    "wxGridCellBoolRenderer;wx/grid.h|"
    "wxGridCellChoiceEditor;wx/grid.h|"
    "wxGridCellEditor;wx/grid.h|"
    "wxGridCellRenderer;wx/grid.h|"
    "wxGridCellFloatEditor;wx/grid.h|"
    "wxGridCellFloatRenderer;wx/grid.h|"
    "wxGridCellNumberEditor;wx/grid.h|"
    "wxGridCellNumberRenderer;wx/grid.h|"
    "wxGridCellStringRenderer;wx/grid.h|"
    "wxGridCellTextEditor;wx/grid.h|"
    "wxGridEditorCreatedEvent;wx/grid.h|"
    "wxGridEvent;wx/grid.h|"
    "wxGridRangeSelectEvent;wx/grid.h|"
    "wxGridSizeEvent;wx/grid.h|"
    "wxGridSizer;wx/sizer.h|"
    "wxGridTableBase;wx/grid.h|"
    "wxHashMap;wx/hashmap.h|"
    "wxHashSet;wx/hashset.h|"
    "wxHashTable;wx/hash.h|"
    "wxHelpController;wx/help.h|"
    "wxHelpControllerHelpProvider;wx/cshelp.h|"
    "wxHelpEvent;wx/event.h|"
    "wxHelpProvider;wx/cshelp.h|"
    "wxHtmlCell;wx/html/htmlcell.h|"
    "wxHtmlCellEvent;wx/html/htmlwin.h|"
    "wxHtmlColourCell;wx/html/htmlcell.h|"
    "wxHtmlContainerCell;wx/html/htmlcell.h|"
    "wxHtmlDCRenderer;wx/html/htmprint.h|"
    "wxHtmlEasyPrinting;wx/html/htmprint.h|"
    "wxHtmlFilter;wx/html/htmlfilt.h|"
    "wxHtmlHelpController;wx/html/helpctrl.h|"
    "wxHtmlHelpData;wx/html/helpdata.h|"
    "wxHtmlHelpDialog;wx/html/helpdlg.h|"
    "wxHtmlHelpFrame;wx/html/helpfrm.h|"
    "wxHtmlHelpWindow;wx/html/helpwnd.h|"
    "wxHtmlModalHelp;wx/html/helpctrl.h|"
    "wxHtmlLinkInfo;wx/html/htmlcell.h|"
    "wxHtmlLinkEvent;wx/html/htmlwin.h|"
    "wxHtmlListBox;wx/htmllbox.h|"
    "wxHtmlParser;wx/html/htmlpars.h|"
    "wxHtmlPrintout;wx/html/htmprint.h|"
    "wxHtmlTag;wx/html/htmltag.h|"
    "wxHtmlTagHandler;wx/html/htmlpars.h|"
    "wxHtmlTagsModule;wx/html/winpars.h|"
    "wxHtmlWidgetCell;wx/html/htmlcell.h|"
    "wxHtmlWindow;wx/html/htmlwin.h|"
    "wxHtmlWinParser;wx/html/winpars.h|"
    "wxHtmlWinTagHandler;wx/html/winpars.h|"
    "wxHTTP;wx/protocol/http.h|"
    "wxHyperlinkCtrl;wx/hyperlink.h|"
    "wxHyperlinkEvent;wx/hyperlink.h|"
    "wxIcon;wx/icon.h|"
    "wxIconBundle;wx/iconbndl.h|"
    "wxIconizeEvent;wx/event.h|"
    "wxIconLocation;wx/iconloc.h|"
    "wxIdleEvent;wx/event.h|"
    "wxImage;wx/image.h|"
    "wxImageHandler;wx/image.h|"
    "wxImageList;wx/imaglist.h|"
    "wxIndividualLayoutConstraint;wx/layout.h|"
    "wxInitDialogEvent;wx/event.h|"
    "wxInputStream;wx/stream.h|"
    "wxIPaddress;wx/socket.h|"
    "wxIPV4address;wx/socket.h|"
    "wxIsNaN;wx/math.h|"
    "wxJoystick;wx/joystick.h|"
    "wxJoystickEvent;wx/event.h|"
    "wxKeyEvent;wx/event.h|"
    "wxLayoutAlgorithm;wx/laywin.h|"
    "wxLayoutConstraints;wx/layout.h|"
    "wxList;wx/list.h|"
    "wxListbook;wx/listbook.h|"
    "wxListBox;wx/listbox.h|"
    "wxListCtrl;wx/listctrl.h|"
    "wxListEvent;wx/listctrl.h|"
    "wxListItem;wx/listctrl.h|"
    "wxListItemAttr;wx/listctrl.h|"
    "wxListView;wx/listctrl.h|"
    "wxLocale;wx/intl.h|"
    "wxLog;wx/log.h|"
    "wxLogChain;wx/log.h|"
    "wxLogGui;wx/log.h|"
    "wxLogNull;wx/log.h|"
    "wxLogPassThrough;wx/log.h|"
    "wxLogStderr;wx/log.h|"
    "wxLogStream;wx/log.h|"
    "wxLogTextCtrl;wx/log.h|"
    "wxLogWindow;wx/log.h|"
    "wxLongLong;wx/longlong.h|"
    "wxLongLongFmtSpec;wx/longlong.h|"
    "wxMask;wx/bitmap.h|"
    "wxMaximizeEvent;wx/event.h|"
    "wxMBConv;wx/strconv.h|"
    "wxMBConvFile;wx/strconv.h|"
    "wxMBConvUTF16;wx/strconv.h|"
    "wxMBConvUTF32;wx/strconv.h|"
    "wxMBConvUTF7;wx/strconv.h|"
    "wxMBConvUTF8;wx/strconv.h|"
    "wxMDIChildFrame;wx/mdi.h|"
    "wxMDIClientWindow;wx/mdi.h|"
    "wxMDIParentFrame;wx/mdi.h|"
    "wxMediaCtrl;wx/mediactrl.h|"
    "wxMediaEvent;wx/mediactrl.h|"
    "wxMemoryBuffer;wx/buffer.h|"
    "wxMemoryDC;wx/dcmemory.h|"
    "wxMemoryFSHandler;wx/fs_mem.h|"
    "wxMemoryInputStream;wx/mstream.h|"
    "wxMemoryOutputStream;wx/mstream.h|"
    "wxMenu;wx/menu.h|"
    "wxMenuBar;wx/menu.h|"
    "wxMenuEvent;wx/event.h|"
    "wxMenuItem;wx/menuitem.h|"
    "wxMessageDialog;wx/msgdlg.h|"
    "wxMetafile;wx/metafile.h|"
    "wxMetafileDC;wx/metafile.h|"
    "wxMimeTypesManager;wx/mimetype.h|"
    "wxMiniFrame;wx/minifram.h|"
    "wxMirrorDC;wx/dcmirror.h|"
    "wxModule;wx/module.h|"
    "wxMouseCaptureChangedEvent;wx/event.h|"
    "wxMouseCaptureLostEvent;wx/event.h|"
    "wxMouseEvent;wx/event.h|"
    "wxMoveEvent;wx/event.h|"
    "wxMultiChoiceDialog;wx/choicdlg.h|"
    "wxMutex;wx/thread.h|"
    "wxMutexLocker;wx/thread.h|"
    "wxNode;wx/list.h|"
    "wxNotebook;wx/notebook.h|"
    "wxNotebookEvent;wx/notebook.h|"
    "wxNotebookSizer;wx/sizer.h|"
    "wxNotifyEvent;wx/event.h|"
    "wxObjArray;wx/dynarray.h|"
    "wxObject;wx/object.h|"
    "wxObjectRefData;wx/object.h|"
    "wxOpenErrorTraverser;wx/dir.h|"
    "wxOutputStream;wx/stream.h|"
    "wxOwnerDrawnComboBox;wx/odcombo.h|"
    "wxPageSetupDialog;wx/printdlg.h|"
    "wxPageSetupDialogData;wx/cmndata.h|"
    "wxPaintDC;wx/dcclient.h|"
    "wxPaintEvent;wx/event.h|"
    "wxPalette;wx/palette.h|"
    "wxPanel;wx/panel.h|"
    "wxPaperSize;wx/cmndata.h|"
    "wxPasswordEntryDialog;wx/textdlg.h|"
    "wxPathList;wx/filefn.h|"
    "wxPen;wx/pen.h|"
    "wxPenList;wx/gdicmn.h|"
    "wxPickerBase;wx/pickerbase.h|"
    "wxPlatformInfo;wx/platinfo.h|"
    "wxPoint;wx/gdicmn.h|"
    "wxPostScriptDC;wx/dcps.h|"
    "wxPowerEvent;wx/power.h|"
    "wxPreviewCanvas;wx/print.h|"
    "wxPreviewControlBar;wx/print.h|"
    "wxPreviewFrame;wx/print.h|"
    "wxPrintData;wx/cmndata.h|"
    "wxPrintDialog;wx/printdlg.h|"
    "wxPrintDialogData;wx/cmndata.h|"
    "wxPrinter;wx/print.h|"
    "wxPrinterDC;wx/dcprint.h|"
    "wxPrintout;wx/print.h|"
    "wxPrintPreview;wx/print.h|"
    "wxProcess;wx/process.h|"
    "wxProcessEvent;wx/process.h|"
    "wxProgressDialog;wx/progdlg.h|"
    "wxPropertySheetDialog;wx/propdlg.h|"
    "wxProtocol;wx/protocol/protocol.h|"
    "wxQuantize;wx/quantize.h|"
    "wxQueryLayoutInfoEvent;wx/laywin.h|"
    "wxRadioBox;wx/radiobox.h|"
    "wxRadioButton;wx/radiobut.h|"
    "wxRealPoint;wx/gdicmn.h|"
    "wxRect;wx/gdicmn.h|"
    "wxRecursionGuard;wx/recguard.h|"
    "wxRecursionGuardFlag;wx/recguard.h|"
    "wxRegEx;wx/regex.h|"
    "wxRegion;wx/region.h|"
    "wxRegionIterator;wx/region.h|"
    "wxRegKey;wx/msw/registry.h|"
    "wxRendererNative;wx/renderer.h|"
    "wxRendererVersion;wx/renderer.h|"
    "wxRichTextAttr;wx/richtext/richtextbuffer.h|"
    "wxRichTextBuffer;wx/richtext/richtextbuffer.h|"
    "wxRichTextCharacterStyleDefinition;wx/richtext/richtextstyles.h|"
    "wxRichTextCtrl;wx/richtext/richtextctrl.h|"
    "wxRichTextEvent;wx/richtext/richtextctrl.h|"
    "wxRichTextFileHandler;wx/richtext/richtextbuffer.h|"
    "wxRichTextFormattingDialog;wx/richtext/richtextformatdlg.h|"
    "wxRichTextFormattingDialogFactory;wx/richtext/richtextformatdlg.h|"
    "wxRichTextHeaderFooterData;wx/richtext/richtextprint.h|"
    "wxRichTextHTMLHandler;wx/richtext/richtexthtml.h|"
    "wxRichTextListStyleDefinition;wx/richtext/richtextstyles.h|"
    "wxRichTextParagraphStyleDefinition;wx/richtext/richtextstyles.h|"
    "wxRichTextPrinting;wx/richtext/richtextprint.h|"
    "wxRichTextPrintout;wx/richtext/richtextprint.h|"
    "wxRichTextRange;wx/richtext/richtextbuffer.h|"
    "wxRichTextStyleDefinition;wx/richtext/richtextstyles.h|"
    "wxRichTextStyleComboCtrl;wx/richtext/richtextstyles.h|"
    "wxRichTextStyleListBox;wx/richtext/richtextstyles.h|"
    "wxRichTextStyleListCtrl;wx/richtext/richtextstyles.h|"
    "wxRichTextStyleOrganiserDialog;wx/richtext/richtextstyledlg.h|"
    "wxRichTextStyleSheet;wx/richtext/richtextstyles.h|"
    "wxRichTextXMLHandler;wx/richtext/richtextxml.h|"
    "wxSashEvent;wx/sashwin.h|"
    "wxSashLayoutWindow;wx/laywin.h|"
    "wxSashWindow;wx/sashwin.h|"
    "wxScopedArray;wx/ptr_scpd.h|"
    "wxScopedPtr;wx/ptr_scpd.h|"
    "wxScopedTiedPtr;wx/ptr_scpd.h|"
    "wxScreenDC;wx/dcscreen.h|"
    "wxScrollBar;wx/scrolbar.h|"
    "wxScrolledWindow;wx/scrolwin.h|"
    "wxScrollEvent;wx/event.h|"
    "wxScrollWinEvent;wx/event.h|"
    "wxSearchCtrl;wx/srchctrl.h|"
    "wxSemaphore;wx/thread.h|"
    "wxServer;wx/ipc.h|"
    "wxSetCursorEvent;wx/event.h|"
    "wxSetEnv;wx/utils.h|"
    "wxSimpleHelpProvider;wx/cshelp.h|"
    "wxSimpleHtmlListBox;wx/htmllbox.h|"
    "wxSingleChoiceDialog;wx/choicdlg.h|"
    "wxSingleInstanceChecker;wx/snglinst.h|"
    "wxSize;wx/gdicmn.h|"
    "wxSizeEvent;wx/event.h|"
    "wxSizer;wx/sizer.h|"
    "wxSizerFlags;wx/sizer.h|"
    "wxSizerItem;wx/sizer.h|"
    "wxSlider;wx/slider.h|"
    "wxSockAddress;wx/socket.h|"
    "wxSocketBase;wx/socket.h|"
    "wxSocketClient;wx/socket.h|"
    "wxSocketEvent;wx/socket.h|"
    "wxSocketInputStream;wx/sckstrm.h|"
    "wxSocketOutputStream;wx/sckstrm.h|"
    "wxSocketServer;wx/socket.h|"
    "wxSound;wx/sound.h|"
    "wxSpinButton;wx/spinbutt.h|"
    "wxSpinCtrl;wx/spinctrl.h|"
    "wxSpinEvent;wx/spinctrl.h|"
    "wxSplashScreen;wx/splash.h|"
    "wxSplitterEvent;wx/splitter.h|"
    "wxSplitterRenderParams;wx/renderer.h|"
    "wxSplitterWindow;wx/splitter.h|"
    "wxStackFrame;wx/stackwalk.h|"
    "wxStackWalker;wx/stackwalk.h|"
    "wxStandardPaths;wx/stdpaths.h|"
    "wxStaticBitmap;wx/statbmp.h|"
    "wxStaticBox;wx/statbox.h|"
    "wxStaticBoxSizer;wx/sizer.h|"
    "wxStaticLine;wx/statline.h|"
    "wxStaticText;wx/stattext.h|"
    "wxStatusBar;wx/statusbr.h|"
    "wxStdDialogButtonSizer;wx/sizer.h|"
    "wxStopWatch;wx/stopwatch.h|"
    "wxStreamBase;wx/stream.h|"
    "wxStreamBuffer;wx/stream.h|"
    "wxStreamToTextRedirector;wx/textctrl.h|"
    "wxString;wx/string.h|"
    "wxStringBuffer;wx/string.h|"
    "wxStringBufferLength;wx/string.h|"
    "wxStringClientData;clntdata.h|"
    "wxStringInputStream;wx/sstream.h|"
    "wxStringOutputStream;wx/sstream.h|"
    "wxStringTokenizer;wx/tokenzr.h|"
    "wxSymbolPickerDialog;wx/richtext/richtextsymboldlg.h|"
    "wxSysColourChangedEvent;wx/event.h|"
    "wxSystemOptions;wx/sysopt.h|"
    "wxSystemSettings;wx/settings.h|"
    "wxTarClassFactory;wx/tarstrm.h|"
    "wxTarEntry;wx/tarstrm.h|"
    "wxTarInputStream;wx/tarstrm.h|"
    "wxTarOutputStream;wx/tarstrm.h|"
    "wxTaskBarIcon;wx/taskbar.h|"
    "wxTCPClient;wx/sckipc.h|"
    "wxTCPServer;wx/sckipc.h|"
    "wxTempFile;wx/file.h|"
    "wxTempFileOutputStream;wx/wfstream.h|"
    "wxTextAttr;wx/textctrl.h|"
    "wxTextAttrEx;wx/richtext/richtextbuffer.h|"
    "wxTextCtrl;wx/textctrl.h|"
    "wxTextDataObject;wx/dataobj.h|"
    "wxTextDropTarget;wx/dnd.h|"
    "wxTextEntryDialog;wx/textdlg.h|"
    "wxTextFile;wx/textfile.h|"
    "wxTextInputStream;wx/txtstrm.h|"
    "wxTextOutputStream;wx/txtstrm.h|"
    "wxTextValidator;wx/valtext.h|"
    "wxTheClipboard;wx/clipbrd.h|"
    "wxThread;wx/thread.h|"
    "wxThreadHelper;wx/thread.h|"
    "wxTimer;wx/timer.h|"
    "wxTimerEvent;wx/timer.h|"
    "wxTimeSpan;wx/datetime.h|"
    "wxTipProvider;wx/tipdlg.h|"
    "wxTipWindow;wx/tipwin.h|"
    "wxToggleButton;wx/tglbtn.h|"
    "wxToolBar;wx/toolbar.h|"
    "wxToolbook;wx/toolbook.h|"
    "wxToolTip;wx/tooltip.h|"
    "wxTopLevelWindow;wx/toplevel.h|"
    "wxTreebook;wx/treebook.h|"
    "wxTreebookEvent;wx/treebook.h|"
    "wxTreeCtrl;wx/treectrl.h|"
    "wxTreeEvent;wx/treectrl.h|"
    "wxTreeItemData;wx/treectrl.h|"
    "wxTreeItemId;wx/treebase.h|"
    "wxUnsetEnv;wx/utils.h|"
    "wxUpdateUIEvent;wx/event.h|"
    "wxURI;wx/uri.h|"
    "wxURL;wx/url.h|"
    "wxURLDataObject;wx/dataobj.h|"
    "wxVaCopy;wx/defs.h|"
    "wxValidator;wx/validate.h|"
    "wxVariant;wx/variant.h|"
    "wxVariantData;wx/variant.h|"
    "wxView;wx/docview.h|"
    "wxVListBox;wx/vlbox.h|"
    "wxVScrolledWindow;wx/vscroll.h|"
    "wxWindow;wx/window.h|"
    "wxWindowUpdateLocker;wx/wupdlock.h|"
    "wxWindowCreateEvent;wx/event.h|"
    "wxWindowDC;wx/dcclient.h|"
    "wxWindowDestroyEvent;wx/event.h|"
    "wxWindowDisabler;wx/utils.h|"
    "wxWizard;wx/wizard.h|"
    "wxWizardEvent;wx/wizard.h|"
    "wxWizardPage;wx/wizard.h|"
    "wxWizardPageSimple;wx/wizard.h|"
    "wxXmlDocument;wx/xml/xml.h|"
    "wxXmlNode;wx/xml/xml.h|"
    "wxXmlProperty;wx/xml/xml.h|"
    "wxXmlResource;wx/xrc/xmlres.h|"
    "wxXmlResourceHandler;wx/xrc/xmlres.h|"
    "wxZipClassFactory;wx/zipstrm.h|"
    "wxZipEntry;wx/zipstrm.h|"
    "wxZipInputStream;wx/zipstrm.h|"
    "wxZipNotifier;wx/zipstrm.h|"
    "wxZipOutputStream;wx/zipstrm.h|"
    "wxZlibInputStream;wx/zstream.h|"
    "wxZlibOutputStream;wx/zstream.h");

    const wxArrayString arWxWidgets_2_8_8 = GetArrayFromString(strWxWidgets_2_8_8, wxT_2("|"));
    for(std::size_t i = 0; i < arWxWidgets_2_8_8.GetCount(); ++i)
    {
        const wxArrayString arTmp = GetArrayFromString(arWxWidgets_2_8_8.Item(i), wxT_2(";"));
        AddBinding(wxT_2("wxWidgets_2_8_8"), arTmp.Item(0), arTmp.Item(1) );
    }
}// SetDefaultsWxWidgets

void Bindings::SetDefaultsSTL()
{
  AddBinding(wxT_2("STL"),wxT_2("adjacent_find"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("all_of"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("any_of"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("binary_search"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_copy"),                       wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_copy_backward"),              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_fill_n"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_generate_n"),                 wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_merge"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_remove_copy"),                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_remove_copy_if"),             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_replace_copy"),               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_replace_copy_if"),            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_reverse_copy"),               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_rotate_copy"),                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_set_difference"),             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_set_intersection"),           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_set_symmetric_difference"),   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_set_union"),                  wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("checked_unique_copy"),                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("copy"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("copy_backward"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("copy_if"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("copy_n"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("count"),                              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("count_if"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("equal"),                              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("equal_range"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("fill"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("fill_n"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("find"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("find_end"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("find_first_of"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("find_if"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("find_if_not"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("for_each"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("generate"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("generate_n"),                         wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("includes"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("inplace_merge"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_heap"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_heap_until"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_partitioned"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_permutation"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_sorted"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("is_sorted_until"),                    wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("iter_swap"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("lexicographical_compare"),            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("lower_bound"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("make_heap"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("max"),                                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("max_element"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("merge"),                              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("min"),                                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("minmax"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("minmax_element"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("min_element"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("mismatch"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("move"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("move_backward"),                      wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("next_permutation"),                   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("none_of"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("nth_element"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("partial_sort"),                       wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("partial_sort_copy"),                  wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("partition"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("partition_copy"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("partition_point"),                    wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("pop_heap"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("prev_permutation"),                   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("push_heap"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("random_shuffle"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("remove"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("remove_copy"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("remove_copy_if"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("remove_if"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("replace"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("replace_copy"),                       wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("replace_copy_if"),                    wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("replace_if"),                         wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("reverse"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("reverse_copy"),                       wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("rotate"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("rotate_copy"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("search"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("search_n"),                           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("set_difference"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("set_intersection"),                   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("set_symmetric_difference"),           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("set_union"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("shuffle"),                            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("sort"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("sort_heap"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("stable_partition"),                   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("stable_sort"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("swap"),                               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("swap_ranges"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("transform"),                          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_copy"),                     wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_copy_backward"),            wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_fill_n"),                   wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_generate_n"),               wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_merge"),                    wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_remove_copy"),              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_remove_copy_if"),           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_replace_copy"),             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_replace_copy_if"),          wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_reverse_copy"),             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_rotate_copy"),              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_set_difference"),           wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_set_intersection"),         wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_set_symmetric_difference"), wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_set_union"),                wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_unique_copy"),              wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unique"),                             wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("unique_copy"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("upper_bound"),                        wxT_2("algorithm"));
  AddBinding(wxT_2("STL"),wxT_2("array"),                              wxT_2("array"));
  AddBinding(wxT_2("STL"),wxT_2("bitset"),                             wxT_2("bitset"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_mode"),                       wxT_2("codecvt"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_utf16"),                      wxT_2("codecvt"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_utf8"),                       wxT_2("codecvt"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_utf8_utf16"),                 wxT_2("codecvt"));
  AddBinding(wxT_2("STL"),wxT_2("arg"),                                wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("complex"),                            wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("conj"),                               wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("imag"),                               wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("norm"),                               wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("polar"),                              wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("proj"),                               wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("real"),                               wxT_2("complex"));
  AddBinding(wxT_2("STL"),wxT_2("deque"),                              wxT_2("deque"));
  AddBinding(wxT_2("STL"),wxT_2("bad_exception"),                      wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("current_exception"),                  wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("exception"),                          wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("exception_ptr"),                      wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("get_terminate"),                      wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("get_unexpected"),                     wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("make_exception_ptr"),                 wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("nested_exception"),                   wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("rethrow_exception"),                  wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("rethrow_if_nested"),                  wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("set_terminate"),                      wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("set_unexpected"),                     wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("terminate"),                          wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("terminate_handler"),                  wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("throw_with_nested"),                  wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("uncaught_exception"),                 wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("unexpected"),                         wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("unexpected_handler"),                 wxT_2("exception"));
  AddBinding(wxT_2("STL"),wxT_2("forward_list"),                       wxT_2("forward_list"));
  AddBinding(wxT_2("STL"),wxT_2("basic_filebuf"),                      wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_fstream"),                      wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_ifstream"),                     wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_ofstream"),                     wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("filebuf"),                            wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("fstream"),                            wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("ifstream"),                           wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("ofstream"),                           wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("wfilebuf"),                           wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("wfstream"),                           wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("wifstream"),                          wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("wofstream"),                          wxT_2("fstream"));
  AddBinding(wxT_2("STL"),wxT_2("bad_function_call"),                  wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("binary_function"),                    wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("binary_negate"),                      wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bind"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bind1st"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bind2nd"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("binder1st"),                          wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("binder2nd"),                          wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bit_and"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bit_not"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bit_or"),                             wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("bit_xor"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("cref"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("const_mem_fun_ref_t"),                wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("const_mem_fun_t"),                    wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("const_mem_fun1_ref_t"),               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("const_mem_fun1_t"),                   wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("divides"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("equal_to"),                           wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("function"),                           wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("greater"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("greater_equal"),                      wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("hash"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("is_bind_expression"),                 wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("is_placeholder"),                     wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("less"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("less_equal"),                         wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("logical_and"),                        wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("logical_not"),                        wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("logical_or"),                         wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fn"),                             wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun_ref"),                        wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun_ref_t"),                      wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun_t"),                          wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun1_ref_t"),                     wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("mem_fun1_t"),                         wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("minus"),                              wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("modulus"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("multiplies"),                         wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("negate"),                             wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("not_equal_to"),                       wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("not1"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("not2"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("plus"),                               wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("pointer_to_binary_function"),         wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("pointer_to_unary_function"),          wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("ptr_fun"),                            wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("ref"),                                wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("reference_wrapper"),                  wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("unary_function"),                     wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("unary_negate"),                       wxT_2("functional"));
  AddBinding(wxT_2("STL"),wxT_2("hash_compare"),                       wxT_2("hash_map"));
  AddBinding(wxT_2("STL"),wxT_2("hash_map"),                           wxT_2("hash_map"));
  AddBinding(wxT_2("STL"),wxT_2("hash_multimap"),                      wxT_2("hash_map"));
  AddBinding(wxT_2("STL"),wxT_2("value_compare"),                      wxT_2("hash_map"));
  AddBinding(wxT_2("STL"),wxT_2("hash_multiset"),                      wxT_2("hash_set"));
  AddBinding(wxT_2("STL"),wxT_2("hash_set"),                           wxT_2("hash_set"));
  AddBinding(wxT_2("STL"),wxT_2("boolalpha"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("dec"),                                wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("defaultfloat"),                       wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("get_money"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("get_time"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("fixed"),                              wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("hex"),                                wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("hexfloat"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("internal"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("left"),                               wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("noboolalpha"),                        wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("noshowbase"),                         wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("noshowpoint"),                        wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("noshowpos"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("noskipws"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("nounitbuf"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("nouppercase"),                        wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("oct"),                                wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("put_money"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("put_time"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("quoted"),                             wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("resetiosflags"),                      wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("right"),                              wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("scientific"),                         wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("setbase"),                            wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("setfill"),                            wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("setiosflags"),                        wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("setprecision"),                       wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("setw"),                               wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("showbase"),                           wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("showpoint"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("showpos"),                            wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("skipws"),                             wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("unitbuf"),                            wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("uppercase"),                          wxT_2("iomanip"));
  AddBinding(wxT_2("STL"),wxT_2("initializer_list"),                   wxT_2("initializer_list"));
  AddBinding(wxT_2("STL"),wxT_2("cerr"),                               wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("cin"),                                wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("clog"),                               wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("cout"),                               wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("wcerr"),                              wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("wcin"),                               wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("wclog"),                              wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("wcout"),                              wxT_2("iostream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_istream"),                      wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_iostream"),                     wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("istream"),                            wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("iostream"),                           wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("wistream"),                           wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("wiostream"),                          wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("ws"),                                 wxT_2("istream"));
  AddBinding(wxT_2("STL"),wxT_2("advance"),                            wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("back_inserter"),                      wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("back_insert_iterator"),               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("begin"),                              wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("bidirectional_iterator_tag"),         wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("cbegin"),                             wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("cend"),                               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("checked_array_iterator"),             wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("crbegin"),                            wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("crend"),                              wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("distance"),                           wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("end"),                                wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("forward_iterator_tag"),               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("front_inserter"),                     wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("front_insert_iterator"),              wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("input_iterator_tag"),                 wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("inserter"),                           wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("insert_iterator"),                    wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("istream_iterator"),                   wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("istreambuf_iterator"),                wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("iterator"),                           wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("iterator_traits"),                    wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("make_move_iterator"),                 wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("make_reverse_iterator"),              wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("move_iterator"),                      wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("next"),                               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("ostream_iterator"),                   wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("ostreambuf_iterator"),                wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("output_iterator_tag"),                wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("prev"),                               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("random_access_iterator_tag"),         wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("rbegin"),                             wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("rend"),                               wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("reverse_iterator"),                   wxT_2("iterator"));
  AddBinding(wxT_2("STL"),wxT_2("float_denorm_style"),                 wxT_2("limits"));
  AddBinding(wxT_2("STL"),wxT_2("float_round_style"),                  wxT_2("limits"));
  AddBinding(wxT_2("STL"),wxT_2("numeric_limits"),                     wxT_2("limits"));
  AddBinding(wxT_2("STL"),wxT_2("list"),                               wxT_2("list"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt"),                            wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_base"),                       wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("codecvt_byname"),                     wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("collate"),                            wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("collate_byname"),                     wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("ctype"),                              wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("ctype_base"),                         wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("ctype_byname"),                       wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("has_facet"),                          wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("locale"),                             wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("messages"),                           wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("messages_base"),                      wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("messages_byname"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("money_base"),                         wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("money_get"),                          wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("money_put"),                          wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("moneypunct"),                         wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("moneypunct_byname"),                  wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("num_get"),                            wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("num_put"),                            wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("numpunct"),                           wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("numpunct_byname"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("time_base"),                          wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("time_get"),                           wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("time_get_byname"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("time_put"),                           wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("time_put_byname"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("use_facet"),                          wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("wbuffer_convert"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("wstring_convert"),                    wxT_2("locale"));
  AddBinding(wxT_2("STL"),wxT_2("map"),                                wxT_2("map"));
  AddBinding(wxT_2("STL"),wxT_2("multimap"),                           wxT_2("map"));
  AddBinding(wxT_2("STL"),wxT_2("value_compare"),                      wxT_2("map"));
  AddBinding(wxT_2("STL"),wxT_2("addressof"),                          wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("align"),                              wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("allocate_shared"),                    wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("allocator"),                          wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("allocator_arg"),                      wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("allocator_arg_t"),                    wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("allocator_traits"),                   wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("auto_ptr"),                           wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("bad_weak_ptr"),                       wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("const_pointer_cast"),                 wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("checked_uninitialized_fill_n"),       wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("declare_no_pointers"),                wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("declare_reachable"),                  wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("default_delete"),                     wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("dynamic_pointer_cast"),               wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("enable_shared_from_this"),            wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("get_deleter"),                        wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("get_pointer_safety"),                 wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("get_temporary_buffer"),               wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("make_unique"),                        wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("make_shared"),                        wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("owner_less"),                         wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("pointer_safety"),                     wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("pointer_traits"),                     wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("raw_storage_iterator"),               wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("return_temporary_buffer"),            wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("shared_ptr"),                         wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("static_pointer_cast"),                wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_uninitialized_fill_n"),     wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("undeclare_no_pointers"),              wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("undeclare_reachable"),                wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("uninitialized_copy"),                 wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("uninitialized_copy_n"),               wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("uninitialized_fill"),                 wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("uninitialized_fill_n"),               wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("unique_ptr"),                         wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("uses_allocator"),                     wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("weak_ptr"),                           wxT_2("memory"));
  AddBinding(wxT_2("STL"),wxT_2("bad_alloc"),                          wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("bad_array_new_length"),               wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("new_handler"),                        wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("nothrow"),                            wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("nothrow_t"),                          wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("get_new_handler"),                    wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("set_new_handler"),                    wxT_2("new"));
  AddBinding(wxT_2("STL"),wxT_2("accumulate"),                         wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("adjacent_difference"),                wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("checked_adjacent_difference"),        wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("checked_partial_sum"),                wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("inner_product"),                      wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("iota"),                               wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("partial_sum"),                        wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_adjacent_difference"),      wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("unchecked_partial_sum"),              wxT_2("numeric"));
  AddBinding(wxT_2("STL"),wxT_2("basic_ostream"),                      wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("endl"),                               wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("ends"),                               wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("flush"),                              wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("ostream"),                            wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("wostream"),                           wxT_2("ostream"));
  AddBinding(wxT_2("STL"),wxT_2("priority_queue "),                    wxT_2("queue"));
  AddBinding(wxT_2("STL"),wxT_2("queue"),                              wxT_2("queue"));
  AddBinding(wxT_2("STL"),wxT_2("bernouilli_distribution"),            wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("binomial_distribution"),              wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("cauchy_distribution"),                wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("chi_squared_distribution"),           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("default_random_engine"),              wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("discard_block_engine"),               wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("discrete_distribution"),              wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("exponential_distribution"),           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("extreme_value_distribution"),         wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("fisher_f_distribution"),              wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("gamma_distribution"),                 wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("generate_canonical"),                 wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("geometric_distribution"),             wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("independent_bits_engine"),            wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("knuth_b"),                            wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("linear_congruential_engine"),         wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("lognormal_distribution"),             wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("mersenne_twister_engine"),            wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("minstd_rand"),                        wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("minstd_rand0"),                       wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("mt19937"),                            wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("mt19937_64"),                         wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("negative_binomial_distribution"),     wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("normal_distribution"),                wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("piecewise_constant_distribution"),    wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("piecewise_linear_distribution"),      wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("poisson_distribution"),               wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("ranlux24"),                           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("ranlux48"),                           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("ranlux24_base"),                      wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("ranlux48_base"),                      wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("seed_seq"),                           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("student_t_distribution"),             wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("shuffle_order_engine"),               wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("subtract_with_carry_engine"),         wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("uniform_int_distribution"),           wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("uniform_real_distribution"),          wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("weibull_distribution"),               wxT_2("random"));
  AddBinding(wxT_2("STL"),wxT_2("centi"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("deca"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("deci"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("atto"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("hecto"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("exa"),                                wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("femto"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("giga"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("kilo"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("mega"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("micro"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("milli"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("nano"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("peta"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("pico"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_add"),                          wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_divide"),                       wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_equal"),                        wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_greater"),                      wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_greater_equal"),                wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_less"),                         wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_less_equal"),                   wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_multiply"),                     wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_not_equal"),                    wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("ratio_subtract"),                     wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("tera"),                               wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("yocto"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("yotta"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("zepto"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("zetta"),                              wxT_2("ratio"));
  AddBinding(wxT_2("STL"),wxT_2("scoped_allocator_adaptor"),           wxT_2("scoped_allocator_adaptor"));
  AddBinding(wxT_2("STL"),wxT_2("multiset"),                           wxT_2("set"));
  AddBinding(wxT_2("STL"),wxT_2("set"),                                wxT_2("set"));
  AddBinding(wxT_2("STL"),wxT_2("basic_istringstream"),                wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_ostringstream"),                wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_stringbuf"),                    wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("basic_stringstream"),                 wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("istringstream"),                      wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("ostringstream"),                      wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("stringbuf"),                          wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("stringstream"),                       wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("wistringstream"),                     wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("wostringstream"),                     wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("wstringbuf"),                         wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("wstringstream"),                      wxT_2("sstream"));
  AddBinding(wxT_2("STL"),wxT_2("stack"),                              wxT_2("stack"));
  AddBinding(wxT_2("STL"),wxT_2("domain_error"),                       wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("invalid_argument "),                  wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("length_error"),                       wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("logic_error"),                        wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("out_of_range"),                       wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("overflow_error"),                     wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("range_error"),                        wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("runtime_error "),                     wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("underflow_error "),                   wxT_2("stdexcept"));
  AddBinding(wxT_2("STL"),wxT_2("basic_streambuf"),                    wxT_2("streambuf"));
  AddBinding(wxT_2("STL"),wxT_2("streambuf"),                          wxT_2("streambuf"));
  AddBinding(wxT_2("STL"),wxT_2("wstreambuf"),                         wxT_2("streambuf"));
  AddBinding(wxT_2("STL"),wxT_2("basic_string"),                       wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("char_traits"),                        wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("getline"),                            wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("u16string"),                          wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("u32string"),                          wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stod"),                               wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stof"),                               wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stoi"),                               wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stol"),                               wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stold"),                              wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stoll"),                              wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stoul"),                              wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("stoull"),                             wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("string"),                             wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("to_string"),                          wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("to_wstring"),                         wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("wstring"),                            wxT_2("string"));
  AddBinding(wxT_2("STL"),wxT_2("istrstream"),                         wxT_2("strstream"));
  AddBinding(wxT_2("STL"),wxT_2("ostrstream"),                         wxT_2("strstream"));
  AddBinding(wxT_2("STL"),wxT_2("strstream"),                          wxT_2("strstream"));
  AddBinding(wxT_2("STL"),wxT_2("strstreambuf"),                       wxT_2("strstream"));
  AddBinding(wxT_2("STL"),wxT_2("forward_as_tuple"),                   wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("ignore"),                             wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("make_tuple"),                         wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("tie"),                                wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("tuple_cat"),                          wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("tuple"),                              wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("tuple_element"),                      wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("tuple_size"),                         wxT_2("tuple"));
  AddBinding(wxT_2("STL"),wxT_2("type_index"),                         wxT_2("typeindex"));
  AddBinding(wxT_2("STL"),wxT_2("bad_cast"),                           wxT_2("typeinfo"));
  AddBinding(wxT_2("STL"),wxT_2("bad_typeid"),                         wxT_2("typeinfo"));
  AddBinding(wxT_2("STL"),wxT_2("type_info"),                          wxT_2("typeinfo"));
  AddBinding(wxT_2("STL"),wxT_2("unordered_map"),                      wxT_2("unordered_map"));
  AddBinding(wxT_2("STL"),wxT_2("unordered_multimap"),                 wxT_2("unordered_map"));
  AddBinding(wxT_2("STL"),wxT_2("unordered_set"),                      wxT_2("unordered_set"));
  AddBinding(wxT_2("STL"),wxT_2("unordered_multiset"),                 wxT_2("unordered_set"));
  AddBinding(wxT_2("STL"),wxT_2("declval"),                            wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("exchange"),                           wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("forward"),                            wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("index_sequence"),                     wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("index_sequence_for"),                 wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("integer_sequence"),                   wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("make_index_sequence"),                wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("make_integer_sequence"),              wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("move_if_noexcept"),                   wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("pair"),                               wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("piecewise_construct"),                wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("piecewise_construct_t"),              wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("make_pair"),                          wxT_2("utility"));
  AddBinding(wxT_2("STL"),wxT_2("gslice"),                             wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("gslice_array"),                       wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("indirect_array"),                     wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("mask_array"),                         wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("slice"),                              wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("slice_array"),                        wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("valarray"),                           wxT_2("valarray"));
  AddBinding(wxT_2("STL"),wxT_2("vector"),                             wxT_2("vector"));
}// SetDefaultsSTL

void Bindings::SetDefaultsCLibrary()
{
  AddBinding(wxT_2("C_Library"),wxT_2("assert"),     wxT_2("cassert"));
  AddBinding(wxT_2("C_Library"),wxT_2("isalnum"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isalpha"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isblank"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("iscntrl"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isdigit"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isgraph"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("islower"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isprint"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("ispunct"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isspace"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isupper"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("isxdigit"),   wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("tolower"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("toupper"),    wxT_2("cctype"));
  AddBinding(wxT_2("C_Library"),wxT_2("setlocale"),  wxT_2("clocale"));
  AddBinding(wxT_2("C_Library"),wxT_2("localeconv"), wxT_2("clocale"));
  AddBinding(wxT_2("C_Library"),wxT_2("fclose"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fopen"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fflush"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("freopen"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("setbuf"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("setvbuf"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fprintf"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fscan"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("printf"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("scanf"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("sprintf"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("sscanf"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("vfprintf"),   wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("vprintf"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("vsprintf"),   wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fgetc"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fgets"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fputc"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fputs"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("getc"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("getchar"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("gets"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("putc"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("putchar"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("puts"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("ungetc"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fread"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fwrite"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fgetpos"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fseek"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("fsetpos"),    wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("ftell"),      wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("rewind"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("clearerr"),   wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("feof"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("ferror"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("perrer"),     wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("FILE"),       wxT_2("cstdio"));
  AddBinding(wxT_2("C_Library"),wxT_2("abort"),      wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("abs"),        wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("atexit"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("atof"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("atoi"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("atol"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("bsearch"),    wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("calloc"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("div"),        wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("exit"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("free"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("getenv"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("labs"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("ldiv"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("malloc"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("mblen"),      wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("mbstowcs"),   wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("mbtowc"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("qsort"),      wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("rand"),       wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("realloc"),    wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("srand"),      wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("strtod"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("strtol"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("strtoul"),    wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("system"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("wcstombs"),   wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("wctomb"),     wxT_2("cstdlib"));
  AddBinding(wxT_2("C_Library"),wxT_2("memchr"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("memcmp"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("memcpy"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("memmove"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("memset"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strcat"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strchr"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strcmp"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strcoll"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strcpy"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strcspn"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strerror"),   wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strlen"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strncat"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strncmp"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strncpy"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strpbrk"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strrchr"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strspn"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strstr"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strtok"),     wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("strxfrm"),    wxT_2("cstring"));
  AddBinding(wxT_2("C_Library"),wxT_2("asctime"),    wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("clock"),      wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("ctime"),      wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("difftime"),   wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("gmtime"),     wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("localtime"),  wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("mktime"),     wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("strftime"),   wxT_2("ctime"));
  AddBinding(wxT_2("C_Library"),wxT_2("time"),       wxT_2("ctime"));
}// SetDefaultsCLibrary
