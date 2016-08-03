#ifndef PRINTDLG_H
#define PRINTDLG_H

#include <wx/dialog.h>
#include "printing_types.h"

#if wxUSE_PRINTING_ARCHITECTURE
class PrintDialog : public wxDialog
{
	public:
		PrintDialog(wxWindow* parent);
		virtual ~PrintDialog();

        virtual void EndModal(int retCode);

		PrintScope GetPrintScope() const;
		PrintColourMode GetPrintColourMode() const;
		bool GetPrintLineNumbers() const;
};

#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // PRINTDLG_H
