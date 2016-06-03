#include "shellproperties.h"

#include <configmanager.h>

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(ShellCommandVec);

namespace
{
wxString istr0(int i)
{
    return wxString::Format(wxT_2("%i"), i);
}
}


bool CommandCollection::WriteConfig()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("ShellExtensions"));
    //cfg->Clear();
    const int len = interps.GetCount();
    cfg->Write(wxT_2("ShellCmds/numcmds"), len);
    for(int i = 0; i < len; ++i)
    {
        const wxString istr = istr0(i);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/name"), interps[i].name);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/command"), interps[i].command);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/wdir"), interps[i].wdir);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/wildcards"), interps[i].wildcards);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/menu"), interps[i].menu);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/menupriority"), interps[i].menupriority);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/cmenu"), interps[i].cmenu);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/cmenupriority"), interps[i].cmenupriority);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/envvarset"), interps[i].envvarset);
        cfg->Write(wxT_2("ShellCmds/I")+istr+wxT_2("/mode"), interps[i].mode);
    }
    return true;
}

bool CommandCollection::ReadConfig()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("ShellExtensions"));
    int len = 0;
    if(!cfg->Read(wxT_2("ShellCmds/numcmds"), &len))
    {
//        cbMessageBox(wxT_2("Warning: couldn't read interpreter config data"));
        return false;
    }
    for(int i = 0; i < len; ++i)
    {
        ShellCommand interp;
        const wxString istr = istr0(i);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/name"), &interp.name);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/command"), &interp.command);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/wdir"), &interp.wdir);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/wildcards"), &interp.wildcards);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/menu"), &interp.menu);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/menupriority"), &interp.menupriority);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/cmenu"), &interp.cmenu);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/cmenupriority"), &interp.cmenupriority);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/envvarset"), &interp.envvarset);
        cfg->Read(wxT_2("ShellCmds/I")+istr+wxT_2("/mode"), &interp.mode);
        interps.Add(interp);
    }
    return true;
}

bool CommandCollection::ExportConfig(const wxString &filename)
{
    wxFile file(filename, wxFile::write);
    if(!file.IsOpened())
        return false;
    file.Write(wxT_2("##Tools Plus Plugin (v0.6) Command Export##\n"));
    const int len=interps.GetCount();
    for(int i = 0; i < len; ++i)
    {
        file.Write(wxT_2("COMMAND#####################################\n"));
        file.Write(wxT_2("name:")+interps[i].name+wxT_2("\n"));
        file.Write(wxT_2("command line:")+interps[i].command+wxT_2("\n"));
        file.Write(wxT_2("workdir:")+interps[i].wdir+wxT_2("\n"));
        file.Write(wxT_2("wildcards:")+interps[i].wildcards+wxT_2("\n"));
        file.Write(wxT_2("menu string:")+interps[i].menu+wxT_2("\n"));
        file.Write(wxString::Format(wxT_2("menu priority:%i\n"), interps[i].menupriority));
        file.Write(wxT_2("context menu string:")+interps[i].cmenu+wxT_2("\n"));
        file.Write(wxString::Format(wxT_2("context menu priority:%i\n"), interps[i].cmenupriority));
        file.Write(wxT_2("envvarset:")+interps[i].envvarset+wxT_2("\n"));
        file.Write(wxT_2("mode (W,C,):")+interps[i].mode+wxT_2("\n"));
    }
    return true;
}

namespace
{
wxString readconfigdata(wxString &configstr)
{
    configstr = configstr.AfterFirst(':');
    const wxString data = configstr.BeforeFirst('\n');
    configstr = configstr.AfterFirst('\n');
    return data;
}
}

bool CommandCollection::ImportConfig(const wxString &filename)
{
    wxFile file(filename, wxFile::read);
    if(!file.IsOpened())
        return false;
    wxString import = cbReadFileContents(file);
    import.Replace(wxT_2("\r\n"),wxT_2("\n"));
    import.Replace(wxT_2("\r"),wxT_2("\n"));
    import = import.AfterFirst('\n');
    while(!import.IsEmpty())
    {
        ShellCommand s;
        import = import.AfterFirst('\n');
        s.name = readconfigdata(import);
        s.command = readconfigdata(import);
        s.wdir = readconfigdata(import);
        s.wildcards = readconfigdata(import);
        s.menu = readconfigdata(import);
        long i;
        readconfigdata(import).ToLong(&i);
        s.menupriority = i;
        s.cmenu = readconfigdata(import);
        readconfigdata(import).ToLong(&i);
        s.cmenupriority = i;
        s.envvarset = readconfigdata(import);
        s.mode = readconfigdata(import);
        interps.Add(s);
    }
    return true;
}


bool CommandCollection::ImportLegacyConfig()
{
    ConfigManager* cfg = Manager::Get()->GetConfigManager(wxT_2("InterpretedLangs"));
    int len = 0;
    if(!cfg->Read(wxT_2("InterpProps/numinterps"), &len))
    {
        return false;
    }
    for(int i = 0; i < len; ++i)
    {
        const wxString istr = istr0(i);
        wxString name,exec,extensions;
        cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/name"), &name);
        cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/exec"), &exec);
        cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/ext"), &extensions);
        int lenact = 0;
        cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/numactions"), &lenact);
        for(int j = 0; j < lenact; ++j)
        {
            ShellCommand interp;
            const wxString jstr = istr0(j);
            wxString aname,command,mode,wdir,envvarset;
            cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/actions/A")+jstr+wxT_2("/name"), &aname);
            cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/actions/A")+jstr+wxT_2("/command"), &command);
            cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/actions/A")+jstr+wxT_2("/mode"), &mode);
            cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/actions/A")+jstr+wxT_2("/workingdir"), &wdir);
            cfg->Read(wxT_2("InterpProps/I")+istr+wxT_2("/actions/A")+jstr+wxT_2("/envvarset"), &envvarset);
            interp.name = name+wxT_2(" ")+aname;
            interp.wildcards = extensions;
            interp.command = command;
            interp.command.Replace(wxT_2("$interpreter"),exec);
            interp.wdir = wdir;
            interp.menu = name+wxT_2("/")+aname;
            interp.cmenu = name+wxT_2("/")+aname;
            interp.cmenupriority = 0;
            interp.menupriority = 0;
            interp.envvarset = envvarset;
            interp.mode = mode;
            interps.Add(interp);
        }
    }
    cfg->Clear();
    WriteConfig();
    return true;
}

