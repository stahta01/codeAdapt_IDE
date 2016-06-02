///////////////////////////////////////////////////////////////////////////////
// Name:        pdflayer.cpp
// Purpose:     
// Author:      Ulrich Telle
// Modified by:
// Created:     2009-07-01
// RCS-ID:      $$
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdflayer.cpp Implementation of the layer classes

// For compilers that support precompilation, includes <wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// includes

#include "wx/pdflayer.h"
#include "wx/pdfobjects.h"

// --- OCG

wxPdfOcg::wxPdfOcg()
  : m_type(wxPDF_OCG_TYPE_UNKNOWN), m_index(0), m_n(0)
{
}

wxPdfOcg::~wxPdfOcg()
{
}

bool
wxPdfOcg::IsOk() const
{
  return m_type != wxPDF_OCG_TYPE_UNKNOWN;
}

// --- Layer

wxPdfLayer::wxPdfLayer(const wxString& name)
  : wxPdfOcg()
{
  m_type = wxPDF_OCG_TYPE_LAYER;
  m_name = name;
  m_intent = wxPDF_OCG_INTENT_DEFAULT;
  m_on = true;
  m_onPanel = true;
  m_parent = NULL;
  m_usage = NULL;
}


wxPdfLayer::~wxPdfLayer()
{
  if (m_usage != NULL)
  {
    delete m_usage;
  }
}

bool
wxPdfLayer::AddChild(wxPdfLayer* child)
{
  bool ok = child != NULL;
  if (ok)
  {
    if (child->GetParent() == NULL)
    {
      child->SetParent(this);
      m_children.Add(child);
    }
    else
    {
      wxLogDebug(wxString(wxT_2("wxPdfLayer::AddChild: ")) +
                 wxString::Format(_("The layer '%s' already has a parent."), child->GetName().c_str()));
      ok = false;
    }
  }
  return ok;
}

bool
wxPdfLayer::SetParent(wxPdfLayer* parent)
{
  m_parent = parent;
  return true;
}

wxPdfLayer*
wxPdfLayer::CreateTitle(const wxString& title)
{
  wxPdfLayer* layer = new wxPdfLayer(wxEmptyString);
  layer->m_type = wxPDF_OCG_TYPE_TITLE;
  layer->m_title = title;
  return layer;
}

wxPdfDictionary*
wxPdfLayer::AllocateUsage()
{
  if (m_usage == NULL)
  {
    m_usage = new wxPdfDictionary();
  }
  return m_usage;
}

void
wxPdfLayer::SetCreatorInfo(const wxString& creator, const wxString& subtype)
{
  wxPdfDictionary* usage = AllocateUsage();
  if (usage->Get(wxT_2("CreatorInfo")) == NULL)
  {
    wxPdfDictionary* dic = new wxPdfDictionary();
    dic->Put(wxT_2("Creator"), new wxPdfString(creator));
    dic->Put(wxT_2("Subtype"), new wxPdfName(subtype));
    usage->Put(wxT_2("CreatorInfo"), dic);
  }
  else
  {
    wxLogDebug(wxString(wxT_2("wxPdfLayer::SetPrint: ")) +
               wxString(_("Usage entry 'CreatorInfo' already defined.")));
  }
}
    
void
wxPdfLayer::SetLanguage(const wxString& lang, bool preferred)
{
  wxPdfDictionary* usage = AllocateUsage();
  if (usage->Get(wxT_2("Language")) == NULL)
  {
    wxPdfDictionary* dic = new wxPdfDictionary();
    dic->Put(wxT_2("Lang"), new wxPdfString(lang));
    if (preferred)
    {
      dic->Put(wxT_2("Preferred"), new wxPdfName(wxT_2("ON")));
    }
    usage->Put(wxT_2("Language"), dic);
  }
  else
  {
    wxLogDebug(wxString(wxT_2("wxPdfLayer::SetPrint: ")) +
               wxString(_("Usage entry 'Language' already defined.")));
  }
}
    
void
wxPdfLayer::SetExport(bool exportState)
{
  wxPdfDictionary* usage = AllocateUsage();
  if (usage->Get(wxT_2("Export")) == NULL)
  {
    wxPdfDictionary* dic = new wxPdfDictionary();
    dic->Put(wxT_2("ExportState"), exportState ? new wxPdfName(wxT_2("ON")) : new wxPdfName(wxT_2("OFF")));
    usage->Put(wxT_2("Export"), dic);
  }
  else
  {
    wxLogDebug(wxString(wxT_2("wxPdfLayer::SetPrint: ")) +
               wxString(_("Usage entry 'Export' already defined.")));
  }
}
    
void
wxPdfLayer::SetZoom(double minZoom, double maxZoom)
{
  if (minZoom > 0 || maxZoom >= 0)
  {
    wxPdfDictionary* usage = AllocateUsage();
    if (usage->Get(wxT_2("Zoom")) == NULL)
    {
      wxPdfDictionary* dic = new wxPdfDictionary();
      if (minZoom > 0)
      {
        dic->Put(wxT_2("min"), new wxPdfNumber(minZoom));
      }
      if (maxZoom >= 0)
      {
        dic->Put(wxT_2("max"), new wxPdfNumber(maxZoom));
      }
      usage->Put(wxT_2("Zoom"), dic);
    }
    else
    {
      wxLogDebug(wxString(wxT_2("wxPdfLayer::SetPrint: ")) +
                 wxString(_("Usage entry 'Zoom' already defined.")));
    }
  }
}

void
wxPdfLayer::SetPrint(const wxString& subtype, bool printState)
{
  wxPdfDictionary* usage = AllocateUsage();
  if (usage->Get(wxT_2("Print")) == NULL)
  {
    wxPdfDictionary* dic = new wxPdfDictionary();
    dic->Put(wxT_2("Subtype"), new wxPdfName(subtype));
    dic->Put(wxT_2("PrintState"), printState ? new wxPdfName(wxT_2("ON")) : new wxPdfName(wxT_2("OFF")));
    usage->Put(wxT_2("Print"), dic);
  }
  else
  {
    wxLogDebug(wxString(wxT_2("wxPdfLayer::SetPrint: ")) +
               wxString(_("Usage entry 'Print' already defined.")));
  }
}

void
wxPdfLayer::SetView(bool viewState)
{
  wxPdfDictionary* usage = AllocateUsage();
  if (usage->Get(wxT_2("View")) == NULL)
  {
    wxPdfDictionary* dic = new wxPdfDictionary();
    dic->Put(wxT_2("ViewState"), viewState ? new wxPdfName(wxT_2("ON")) : new wxPdfName(wxT_2("OFF")));
    usage->Put(wxT_2("View"), dic);
  }
  else
  {
    wxLogDebug(wxString(wxT_2("wxPdfLayer::SetView: ")) +
               wxString(_("Usage entry 'View' already defined.")));
  }
}

// --- Layer membership

wxPdfLayerMembership::wxPdfLayerMembership()
  : wxPdfOcg()
{
  m_type = wxPDF_OCG_TYPE_MEMBERSHIP;
  m_policy = wxPDF_OCG_POLICY_ANYON;
}

wxPdfLayerMembership::~wxPdfLayerMembership()
{
}

bool
wxPdfLayerMembership::AddMember(wxPdfLayer* layer)
{
  bool ok = m_layers.Index(layer) == wxNOT_FOUND;
  if (ok)
  {
    m_layers.Add(layer);
  }
  return ok;
}

wxPdfArrayLayer
wxPdfLayerMembership::GetMembers() const
{
  return m_layers;
}

void
wxPdfLayerMembership::SetVisibilityPolicy(wxPdfOcgPolicy policy)
{
  m_policy = policy;
}

wxPdfOcgPolicy
wxPdfLayerMembership::GetVisibilityPolicy() const
{
  return m_policy;
}

wxPdfLayerGroup::wxPdfLayerGroup()
{
}

wxPdfLayerGroup::~wxPdfLayerGroup()
{
}

wxPdfLayerGroup::wxPdfLayerGroup(const wxPdfLayerGroup& layer)
{
  m_layers = layer.m_layers;
}

wxPdfLayerGroup&
wxPdfLayerGroup::operator=(const wxPdfLayerGroup& layer)
{
  m_layers = layer.m_layers;
  return *this;
}

bool
wxPdfLayerGroup::Add(wxPdfLayer* layer)
{
  bool ok = (layer != NULL) && 
            (layer->GetType() == wxPDF_OCG_TYPE_LAYER) && 
            (m_layers.Index(layer) == wxNOT_FOUND);
  if (ok)
  {
    m_layers.Add(layer);
  }
  return ok;
}

wxPdfArrayLayer
wxPdfLayerGroup::GetGroup() const
{
  return m_layers;
}
