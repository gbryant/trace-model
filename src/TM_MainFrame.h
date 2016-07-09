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

class TM_MainFrame : public wxFrame
{
	public:
    wxAuiManager  *auiMan;
    TM_ImagePanel *imagePanel;
    TM_ObjectTree *objectTree;
    TM_ToolPalette *toolPalette;

		TM_MainFrame( const wxString& title, const wxPoint& pos, const wxSize& size );
                void OnShowLog( wxCommandEvent &event );
                void OnOpen( wxCommandEvent &event );
                void OnSave( wxCommandEvent &event );
                void OnSaveAs( wxCommandEvent &event );
		            void OnQuit( wxCommandEvent &event );
                void OnImageLoad( wxCommandEvent &event );
                void OnViewZoomIn( wxCommandEvent &event );
                void OnViewZoomOut( wxCommandEvent &event );
                void OnViewImage(wxCommandEvent &event);
                void OnAbout( wxCommandEvent &event );
		            void OnCloseWindow( wxCloseEvent &event );
                void OnPaint(wxPaintEvent &event);
              

	private:
		DECLARE_EVENT_TABLE()
};

enum
{
Menu_File_Quit = 100,
Menu_File_ShowLog,
Menu_File_Open,
Menu_File_Save,
Menu_File_SaveAs,
Menu_Image_Load,
Menu_View_ZoomIn,
Menu_View_ZoomOut,
Menu_View_Image,
Menu_Help_About
};
