/*
* This file is part of Code::Blocks Studio, an open-source cross-platform IDE
* Copyright (C) 2003  Yiannis An. Mandravellos
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact e-mail: Yiannis An. Mandravellos <mandrav@codeblocks.org>
* Program URL   : http://www.codeblocks.org
*
* $Revision$
* $Id$
* $HeadURL$
*/

#include "sdk_precomp.h"

#ifndef CB_PRECOMP
    #include "projectbuildtarget.h" // class's header file
    #include "cbproject.h"
    #include "manager.h"
    #include "projectmanager.h"
    #include "macrosmanager.h"
    #include "globals.h"
#endif



// class constructor
caProjectBuildTarget::caProjectBuildTarget(caProject* parentProject) : m_Project(parentProject)
{
    m_BuildWithAll = false;
    m_CreateStaticLib = true;
    m_CreateDefFile = true;
    m_UseConsoleRunner = true;
}

// class destructor
caProjectBuildTarget::~caProjectBuildTarget()
{
}

caProject* caProjectBuildTarget::GetParentProject()
{
    return m_Project;
}

wxString caProjectBuildTarget::GetFullTitle() const
{
    return m_Project->GetTitle() + _T(" - ") + GetTitle();
}

const wxString & caProjectBuildTarget::GetExternalDeps() const
{
    return m_ExternalDeps;
}

void caProjectBuildTarget::SetExternalDeps(const wxString& deps)
{
    if (m_ExternalDeps != deps)
    {
        m_ExternalDeps = deps;
        SetModified(true);
    }
}

const wxString & caProjectBuildTarget::GetAdditionalOutputFiles() const
{
    return m_AdditionalOutputFiles;
}

void caProjectBuildTarget::SetAdditionalOutputFiles(const wxString& files)
{
    if (m_AdditionalOutputFiles != files)
    {
        m_AdditionalOutputFiles = files;
        SetModified(true);
    }
}

bool caProjectBuildTarget::GetIncludeInTargetAll() const
{
	return m_BuildWithAll;
}

void caProjectBuildTarget::SetIncludeInTargetAll(bool buildIt)
{
	if (m_BuildWithAll != buildIt)
	{
        m_BuildWithAll = buildIt;
        SetModified(true);
	}
}

bool caProjectBuildTarget::GetCreateDefFile() const
{
    return m_CreateDefFile;
}

void caProjectBuildTarget::SetCreateDefFile(bool createIt)
{
    if (m_CreateDefFile != createIt)
    {
        m_CreateDefFile = createIt;
        SetModified(true);
    }
}

bool caProjectBuildTarget::GetCreateStaticLib()
{
    return m_CreateStaticLib;
}

void caProjectBuildTarget::SetCreateStaticLib(bool createIt)
{
    if (m_CreateStaticLib != createIt)
    {
        m_CreateStaticLib = createIt;
        SetModified(true);
    }
}

bool caProjectBuildTarget::GetUseConsoleRunner() const
{
    return GetTargetType() == ttConsoleOnly ? m_UseConsoleRunner : false;
}

void caProjectBuildTarget::SetUseConsoleRunner(bool useIt)
{
    if (GetTargetType() == ttConsoleOnly && useIt != m_UseConsoleRunner)
    {
        m_UseConsoleRunner = useIt;
        SetModified(true);
    }
}

void caProjectBuildTarget::SetTargetType(const TargetType& pt)
{
	TargetType ttold = GetTargetType();
	caCompileTargetBase::SetTargetType(pt);
	if (ttold != GetTargetType() && GetTargetType() == ttConsoleOnly)
        SetUseConsoleRunner(true); // by default, use console runner
}

// target dependencies: targets to be compiled (if necessary) before this one
void caProjectBuildTarget::AddTargetDep(caProjectBuildTarget* target) {
	m_TargetDeps.Add(target);
}

// get the list of dependency targets of this target
BuildTargets& caProjectBuildTarget::GetTargetDeps() {
	return m_TargetDeps;
}

