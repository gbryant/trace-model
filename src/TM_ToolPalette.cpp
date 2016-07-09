/*
    Copyright 2008 Gregory Bryant

    This file is part of TraceModel.

    TraceModel is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TraceModel is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TraceModel.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TraceModelHeaders.h"

#define RESTORE_ICON \
    wxString wname = selectedButton->GetName();\
    wname = wxT("./data/") + wname + wxT(".png");\
    bitmap = new wxBitmap(wname,wxBITMAP_TYPE_PNG);\
    selectedButton->SetBitmapLabel(*bitmap);\
    delete bitmap;

BEGIN_EVENT_TABLE( TM_ToolPalette, wxPanel )
    EVT_BUTTON(tool_pointer, TM_ToolPalette::OnPointerClick)
    EVT_BUTTON(tool_hand, TM_ToolPalette::OnHandClick)
    EVT_BUTTON(tool_ellipse, TM_ToolPalette::OnEllipseClick)
    EVT_BUTTON(tool_createpoint, TM_ToolPalette::OnCreatePointClick)
END_EVENT_TABLE()

TM_ToolPalette::TM_ToolPalette(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name): wxPanel(parent, id, pos, size, style, name)
{
  wxBitmap *bitmap = new wxBitmap(wxT("./data/pointerclick.png"),wxBITMAP_TYPE_PNG);
  pointerButton = new wxBitmapButton(this,tool_pointer,*bitmap,wxPoint(5,0),wxSize(33,33),0,wxDefaultValidator,wxT("pointer"));
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/pointerclick.png"),wxBITMAP_TYPE_PNG);
  pointerButton->SetBitmapSelected(*bitmap);
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/pointerdisable.png"),wxBITMAP_TYPE_PNG);
  pointerButton->SetBitmapDisabled(*bitmap);
  delete bitmap;

  selectedButton = pointerButton;
  selectedTool = tool_pointer;

  bitmap = new wxBitmap(wxT("./data/hand.png"),wxBITMAP_TYPE_PNG);
  handButton = new wxBitmapButton(this,tool_hand,*bitmap,wxPoint(39,0),wxSize(33,33),0,wxDefaultValidator,wxT("hand"));
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/handclick.png"),wxBITMAP_TYPE_PNG);
  handButton->SetBitmapSelected(*bitmap);
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/handdisable.png"),wxBITMAP_TYPE_PNG);
  handButton->SetBitmapDisabled(*bitmap);
  delete bitmap;

  bitmap = new wxBitmap(wxT("./data/ellipse.png"),wxBITMAP_TYPE_PNG);
  ellipseButton = new wxBitmapButton(this,tool_ellipse,*bitmap,wxPoint(5,34),wxSize(33,33),0,wxDefaultValidator,wxT("ellipse"));
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/ellipseclick.png"),wxBITMAP_TYPE_PNG);
  ellipseButton->SetBitmapSelected(*bitmap);
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/ellipsedisable.png"),wxBITMAP_TYPE_PNG);
  ellipseButton->SetBitmapDisabled(*bitmap);
  delete bitmap;
  ellipseButton->Disable();

  bitmap = new wxBitmap(wxT("./data/createpoint.png"),wxBITMAP_TYPE_PNG);
  createpointButton = new wxBitmapButton(this,tool_createpoint,*bitmap,wxPoint(39,34),wxSize(33,33),0,wxDefaultValidator,wxT("createpoint"));
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/createpointclick.png"),wxBITMAP_TYPE_PNG);
  createpointButton->SetBitmapSelected(*bitmap);
  delete bitmap;
  bitmap = new wxBitmap(wxT("./data/createpointdisable.png"),wxBITMAP_TYPE_PNG);
  createpointButton->SetBitmapDisabled(*bitmap);
  delete bitmap;
  createpointButton->Disable();
  

}

TM_ToolPalette::~TM_ToolPalette(void)
{
}

int TM_ToolPalette::GetSelectedTool(void)
{
  return selectedTool;
}

void TM_ToolPalette::SetSelectedTool(int tool)
{
  wxCommandEvent *wxCmdEvt = new wxCommandEvent();
  switch(tool)
  {
    case tool_pointer:      {OnPointerClick(*wxCmdEvt);break;}
    case tool_hand:         {OnHandClick(*wxCmdEvt);break;}
    case tool_ellipse:      {OnEllipseClick(*wxCmdEvt);break;}
    case tool_createpoint:  {OnCreatePointClick(*wxCmdEvt);break;}
  }
  delete wxCmdEvt;
}


void TM_ToolPalette::EnableTool(int tool)
{
  switch(tool)
  {
  case tool_pointer:      {pointerButton->Enable();break;}
  case tool_hand:         {handButton->Enable();break;}
  case tool_ellipse:      {ellipseButton->Enable();break;}
  case tool_createpoint:  {createpointButton->Enable();break;}
  }
}

void TM_ToolPalette::DisableTool(int tool)
{
  switch(tool)
  {
  case tool_pointer:      {pointerButton->Disable();break;}
  case tool_hand:         {handButton->Disable();break;}
  case tool_ellipse:      {ellipseButton->Disable();break;}
  case tool_createpoint:  {createpointButton->Disable();break;}
  }
}



void TM_ToolPalette::OnButtonClick(wxCommandEvent &event)
{
  LOG("click");
  mainFrame->imagePanel->SetFocus();
}



void TM_ToolPalette::OnPointerClick(wxCommandEvent &event)
{
  wxBitmap  *bitmap;
  mainFrame->imagePanel->SetFocus();
 
  if(selectedButton!=pointerButton)
  {
    RESTORE_ICON
    bitmap = new wxBitmap(wxT("./data/pointerclick.png"),wxBITMAP_TYPE_PNG);
    pointerButton->SetBitmapLabel(*bitmap);
    delete bitmap;
    selectedButton = pointerButton;
    selectedTool=tool_pointer;
  }
}

void TM_ToolPalette::OnHandClick(wxCommandEvent &event)
{
  wxBitmap  *bitmap;
  mainFrame->imagePanel->SetFocus();
  
  if(selectedButton!=handButton)
  {
    RESTORE_ICON
    bitmap = new wxBitmap(wxT("./data/handclick.png"),wxBITMAP_TYPE_PNG);
    handButton->SetBitmapLabel(*bitmap);
    delete bitmap;
    selectedButton = handButton;
    selectedTool=tool_hand;
  }
}

void TM_ToolPalette::OnEllipseClick(wxCommandEvent &event)
{
  wxBitmap  *bitmap;
  mainFrame->imagePanel->SetFocus();
  
  if(selectedButton!=ellipseButton)
  {
    RESTORE_ICON
    bitmap = new wxBitmap(wxT("./data/ellipseclick.png"),wxBITMAP_TYPE_PNG);
    ellipseButton->SetBitmapLabel(*bitmap);
    delete bitmap;
    selectedButton = ellipseButton;
    selectedTool=tool_ellipse;
  }
}

void TM_ToolPalette::OnCreatePointClick(wxCommandEvent &event)
{
  wxBitmap  *bitmap;
  mainFrame->imagePanel->SetFocus();
  
  if(selectedButton!=createpointButton)
  {
    RESTORE_ICON
    bitmap = new wxBitmap(wxT("./data/createpointclick.png"),wxBITMAP_TYPE_PNG);
    createpointButton->SetBitmapLabel(*bitmap);
    delete bitmap;
    selectedButton = createpointButton;
    selectedTool=tool_createpoint;
  }
}