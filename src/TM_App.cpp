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

TM_LogWindow     *gLogWin;
TM_MainFrame     *mainFrame;

IMPLEMENT_APP(TM_App)
	
		
bool TM_App::OnInit()
{
  
  int       width,height,tempint;
  
  
  
  wxInitAllImageHandlers();
  
  wxRect clientRect = wxGetClientDisplayRect();
  mainFrame = new TM_MainFrame( wxT( "Trace Model" ), wxPoint(0,0), wxSize(clientRect.width,clientRect.height) );
  
  gLogWin = new TM_LogWindow(mainFrame,wxT("Log Window"),false,false);

  mainFrame->imagePanel = new TM_ImagePanel(mainFrame,-1,wxPoint(-1,-1),wxSize(100,100),wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL|wxALWAYS_SHOW_SB,wxT("ImagePanel"));

  mainFrame->objectTree = new TM_ObjectTree(mainFrame,wxT("TM_ObjectTree"));

  mainFrame->toolPalette = new TM_ToolPalette(mainFrame,-1,wxPoint(-1,-1),wxSize(77,100),wxDEFAULT_FRAME_STYLE,wxT("ToolPalette"));

  mainFrame->auiMan = new wxAuiManager(mainFrame,wxAUI_MGR_DEFAULT);

  wxAuiPaneInfo ipPaneInfo;
  
  
//  frame->auiMan->AddPane(gLogWin->GetFrame(),wxRIGHT,wxT("Log"));
//  frame->auiMan->GetPane(gLogWin->GetFrame()).Float();
  
  //frame->auiMan->AddPane( new TM_ImagePanel(frame,-1,wxPoint(-1,-1),wxSize(100,100),wxDEFAULT_FRAME_STYLE/*|wxVSCROLL|wxHSCROLL*/,wxT("ImagePanel")),wxAuiPaneInfo().Name(wxT("test2")).Caption(wxT("Pane Caption")).Right());

  ipPaneInfo.Caption(wxT("ImagePanel"));
  ipPaneInfo.Name(wxT("image_panel"));
  ipPaneInfo.CenterPane();
  mainFrame->auiMan->AddPane(mainFrame->imagePanel,ipPaneInfo);

  ipPaneInfo.Caption(wxT("ObjectTree"));
  ipPaneInfo.Name(wxT("object_tree"));
  ipPaneInfo.DefaultPane();
  ipPaneInfo.Right();
  mainFrame->auiMan->AddPane(mainFrame->objectTree,ipPaneInfo);

  ipPaneInfo.Caption(wxT("Tools"));
  ipPaneInfo.Name(wxT("tool_palette"));
  ipPaneInfo.DefaultPane();
  ipPaneInfo.Left();
  mainFrame->auiMan->AddPane(mainFrame->toolPalette,ipPaneInfo);
  
  mainFrame->auiMan->Update();
  mainFrame->Show(TRUE);

  return TRUE;
}

int TM_App::OnExit()
{
return 0;
}


