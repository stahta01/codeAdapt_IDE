/***************************************************************
 * Name:      devpakupdater.h
 * Purpose:   Code::Blocks plugin
 * Author:    Yiannis Mandravellos<mandrav@codeblocks.org>
 * Created:   05/12/05 15:44:32
 * Copyright: (c) Yiannis Mandravellos
 * License:   GPL
 **************************************************************/

#ifndef DEVPAKUPDATER_H
#define DEVPAKUPDATER_H


#include <api/plugin.h> // the base class we 're inheriting
#include <settings.h> // needed to use the Code::Blocks SDK

class DevPakUpdater : public cbToolPlugin
{
	public:
		DevPakUpdater();
		~DevPakUpdater();
		int Configure();
		int Execute();
		void OnAttach(); // fires when the plugin is attached to the application
		void OnRelease(bool appShutDown); // fires when the plugin is released from the application
	protected:
        bool ConfigurationValid();
	private:
};

#endif // DEVPAKUPDATER_H

