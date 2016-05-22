/***************************************************************
 * Name:      astyle.h
 * Purpose:   Code::Blocks plugin
 * Author:    Yiannis Mandravellos<mandrav@codeblocks.org>
 * Created:   05/25/04 10:06:39
 * Copyright: (c) Yiannis Mandravellos
 * License:   GPL
 **************************************************************/

#ifndef ASTYLEPLUGIN_H
#define ASTYLEPLUGIN_H


#include <api/plugin.h> // the base class we 're inheriting
#include <settings.h> // needed to use the Code::Blocks SDK

class AStylePlugin : public caToolPlugin
{
  public:
    AStylePlugin();
    ~AStylePlugin();
    int Configure();
    int GetConfigurationGroup() const { return cgEditor; }
    caConfigurationPanel* GetConfigurationPanel(wxWindow* parent);
    int Execute();
    void OnAttach(); // fires when the plugin is attached to the application
    void OnRelease(bool appShutDown); // fires when the plugin is released from the application
};

#endif // ASTYLEPLUGIN_H
