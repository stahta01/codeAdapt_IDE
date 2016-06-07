#ifdef USE_PCH
    #ifndef WX_PCH_H_INCLUDED
        #include "wx_pch.h"
    #endif
#else
    #include <wx/defs.h> // Keep as first wx include.

    #include <wx/string.h>
#endif // USE_PCH


#include <globals.h>
#include <cbexception.h>


#include <wx/progdlg.h>

#include "sc_base_types.h"

class ProgressDialog : public wxProgressDialog
{
    public:
        ProgressDialog()
            : wxProgressDialog(_("Progress"),
                                _("Please wait while operation is in progress..."),
                                100, 0,
                                wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT)
        {
        }

        ~ProgressDialog()
        {
        }

        ProgressDialog& operator=(const ProgressDialog&)
        {
            cbThrow(wxT_2("ProgressDialog copy constructor should never be called!"));
        }

        bool Update(int val, const wxString& msg)
        {
            return wxProgressDialog::Update(val, msg, 0);
        }
};

DECLARE_INSTANCE_TYPE(ProgressDialog);

namespace ScriptBindings
{
    void Register_ProgressDialog()
    {
        SqPlus::SQClassDef<ProgressDialog>("ProgressDialog").
                emptyCtor().
                func(&ProgressDialog::Update, "Update");
    }
} // namespace ScriptBindings
