#ifndef DLGABOUTPLUGIN_H
#define DLGABOUTPLUGIN_H

#include <wx/dialog.h> // inheriting class's header file

struct caPluginInfo;
class wxWindow;
/*
 * No description
 */
class dlgAboutPlugin : public wxDialog
{
	public:
		// class constructor
		dlgAboutPlugin(wxWindow* parent, const caPluginInfo* pi);
		// class destructor
		~dlgAboutPlugin();
};

#endif // DLGABOUTPLUGIN_H
