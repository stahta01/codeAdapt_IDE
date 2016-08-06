#ifndef XTRA_RES_H
#define XTRA_RES_H

#ifndef CA_BUILD_WITHOUT_TOOLBARS

#include <wx/xrc/xmlres.h>
#include <wx/toolbar.h>


class wxToolBarAddOnXmlHandler : public wxXmlResourceHandler
{
    public:
        wxToolBarAddOnXmlHandler();
        virtual wxObject *DoCreateResource();
        virtual bool CanHandle(wxXmlNode *node);

    protected:
        bool m_isInside;
        bool m_isAddon;
        wxToolBar *m_toolbar;

        wxBitmap GetCenteredBitmap(const wxString& param = wxT("bitmap"),
            const wxArtClient& defaultArtClient = wxART_OTHER,
            wxSize size = wxDefaultSize);
};

#endif // CA_BUILD_WITHOUT_TOOLBARS

#endif
