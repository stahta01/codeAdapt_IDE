#include <sdk_precomp.h>

#ifndef CB_PRECOMP
    #include <globals.h>
    #include <cbexception.h>
    #include <wx/string.h>
#endif

#include <wx/progdlg.h>

#include "sc_base_types.h"

#if wxUSE_PROGRESSDLG
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
            cbThrow(_T("ProgressDialog copy constructor should never be called!"));
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
#endif // wxUSE_PROGRESSDLG
