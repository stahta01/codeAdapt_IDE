#ifndef PRINTING_TYPES_H
#define PRINTING_TYPES_H

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#include "settings.h"
#include <wx/print.h>

// Global printer
extern wxPrinter* g_printer;

// Global page setup data
extern wxPageSetupData* g_pageSetupData;

// printing scope for print dialog
enum PrintScope
{
    psSelection,
    psActiveEditor,
    psAllOpenEditors
};

// printing colour mode
enum PrintColourMode
{
    pcmBlackAndWhite,
    pcmColourOnWhite,
    pcmInvertColours,
    pcmAsIs
};

#ifdef __cplusplus
extern "C" {
#endif
extern DLLIMPORT void InitPrinting();
extern DLLIMPORT void DeInitPrinting();
#ifdef __cplusplus
}
#endif

#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // PRINTING_TYPES_H
