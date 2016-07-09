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

BEGIN_EVENT_TABLE( TM_MainFrame, wxFrame )
    EVT_MENU( Menu_File_ShowLog, TM_MainFrame::OnShowLog )
    EVT_MENU( Menu_File_Open, TM_MainFrame::OnOpen )
    EVT_MENU( Menu_File_Save, TM_MainFrame::OnSave )
    EVT_MENU( Menu_File_Quit, TM_MainFrame::OnQuit )
    EVT_MENU( Menu_Image_Load, TM_MainFrame::OnImageLoad )
    EVT_MENU( Menu_Help_About, TM_MainFrame::OnAbout )
    EVT_MENU( Menu_View_ZoomIn, TM_MainFrame::OnViewZoomIn )
    EVT_MENU( Menu_View_ZoomOut, TM_MainFrame::OnViewZoomOut )
    EVT_MENU( Menu_View_Image, TM_MainFrame::OnViewImage )
    EVT_CLOSE(TM_MainFrame::OnCloseWindow)
    EVT_PAINT( TM_MainFrame::OnPaint )
    //EVT_UPDATE_UI(Menu_View_Selection, TM_MainFrame::ViewSelectionUpdate)
END_EVENT_TABLE()


TM_MainFrame::TM_MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size ): wxFrame((wxFrame *)NULL, -1, title, pos, size)
{

	wxMenu *menuFile  = new wxMenu;
  wxMenu *menuEdit  = new wxMenu;
  wxMenu *menuImage = new wxMenu;
  wxMenu *menuView  = new wxMenu;
  wxMenu *menuHelp  = new wxMenu;

  
  
  menuFile->Append( Menu_File_ShowLog, wxT( "Show &Log" ) );
  menuFile->AppendSeparator();
  menuFile->Append( Menu_File_Open, wxT( "&Open.." ) );
  menuFile->Append( Menu_File_Save, wxT( "&Save" ) );
  menuFile->Enable(Menu_File_Save,false);
  menuFile->Append( Menu_File_SaveAs, wxT( "&Save As..." ) );
  menuFile->AppendSeparator();
  menuFile->Append( Menu_File_Quit, wxT( "&Quit\tCtrl+Q" ) );
  
  menuImage->Append( Menu_Image_Load, wxT( "&Load\tCtrl+O" ) );

  menuView->Append( Menu_View_ZoomIn, wxT( "Zoom &In\tCtrl+0" ) );
  menuView->Append( Menu_View_ZoomOut, wxT( "Zoom &Out\tCtrl+9" ) );
  menuView->AppendSeparator();
  menuView->Append(Menu_View_Image,wxT("Hide I&mage\tCtrl+K"));

  menuHelp->Append( Menu_Help_About, wxT( "&About...\tCtrl+/" ) );
	
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile,wxT("&File"));
  menuBar->Append(menuEdit,wxT("&Edit"));
  menuBar->Append(menuImage,wxT("&Image"));
  menuBar->Append(menuView,wxT("&View"));
	menuBar->Append(menuHelp,wxT("&Help"));
	
	SetMenuBar( menuBar );
	



	//CreateStatusBar(1,wxST_SIZEGRIP,-1,wxT("mainStatusBar"));
	//SetStatusText( wxT( "Placement Mode" ) );
}

void TM_MainFrame::OnQuit(wxCommandEvent &event)
{
  Destroy();
	wxTheApp->ExitMainLoop();
	//Close(TRUE);
}

void TM_MainFrame::OnViewZoomIn(wxCommandEvent &event)
{
  imagePanel->ZoomIn();
}

void TM_MainFrame::OnViewZoomOut(wxCommandEvent &event)
{
  imagePanel->ZoomOut();
}

void TM_MainFrame::OnViewImage(wxCommandEvent &event)
{

  wxMenuBar	*menuBar;
  int		id;

  menuBar = GetMenuBar();


  if(mainFrame->imagePanel->show_image)
  {
    menuBar->SetLabel(Menu_View_Image,wxT("Show I&mage\tCtrl+K"));
    mainFrame->imagePanel->show_image=false;
    mainFrame->imagePanel->Refresh();
  }
  else
  {
    menuBar->SetLabel(Menu_View_Image,wxT("Hide I&mage\tCtrl+K"));
    mainFrame->imagePanel->show_image=true;
    mainFrame->imagePanel->Refresh();
  }


}

void TM_MainFrame::OnOpen( wxCommandEvent &event)
{

  wxFileDialog *fileDialog = new wxFileDialog(this,wxT("hello :)"),wxT(""),wxT(""),wxT("*.xml"),wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,wxT(""));
  int           result;

  result = fileDialog->ShowModal();

  if(result==wxID_OK)
  {
    wxString str = fileDialog->GetPath();
    wxLogMessage(wxT("Path: %s"),str.c_str());
    result = objectTree->LoadProject(str);
  }

}

void TM_MainFrame::OnSave( wxCommandEvent &event)
{
}

void TM_MainFrame::OnSaveAs( wxCommandEvent &event)
{
}


void TM_MainFrame::OnShowLog( wxCommandEvent &event)
{
wxMenuBar	*menuBar;
int		id;

menuBar = GetMenuBar();
if(menuBar)
{

  if(!((gLogWin->GetFrame())->IsShown()))
  {
    menuBar->SetLabel(Menu_File_ShowLog,wxT("Hide &Log"));
    gLogWin->Show(true);
  }
  else
  {
    menuBar->SetLabel(Menu_File_ShowLog,wxT("Show &Log"));
    gLogWin->Show(false);
  }
}
else
{
  LOG_ERROR
}

}


void TM_MainFrame::OnPaint(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  //DrawMyDocument(dc);
}



void TM_MainFrame::OnImageLoad(wxCommandEvent &event)
{
  wxFileDialog *fileDialog = new wxFileDialog(this,wxT("hello :)"),wxT(""),wxT(""),wxT("*.*"),wxFD_DEFAULT_STYLE,wxDefaultPosition,wxDefaultSize,wxT(""));
  int           result;

  result = fileDialog->ShowModal();

  if(result==wxID_OK)
  {
    wxString str = fileDialog->GetPath();
    wxLogMessage(wxT("Path: %s"),str.c_str());
    result = imagePanel->LoadImage(str);
  }








}

void TM_MainFrame::OnAbout(wxCommandEvent &event)
{
 wxMessageBox( wxT( "TraceModel Pre-Alpha 3" ), wxT( "About Trace Model" ), wxOK | wxICON_INFORMATION, this );
}

void TM_MainFrame::OnCloseWindow(wxCloseEvent &event)
{
	Destroy();
	wxTheApp->ExitMainLoop();
	//Close(TRUE);
}
