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

#pragma once

class TM_ToolPalette : public wxPanel
{
public:
  TM_ToolPalette(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name);
  ~TM_ToolPalette(void);

  void OnButtonClick(wxCommandEvent &event);
  void OnPointerClick(wxCommandEvent &event);
  void OnHandClick(wxCommandEvent &event);
  void OnEllipseClick(wxCommandEvent &event);
  void OnCreatePointClick(wxCommandEvent &event);
  int GetSelectedTool(void);
  void SetSelectedTool(int tool);
  void DisableTool(int);
  void EnableTool(int);

  int selectedTool;

  wxBitmapButton    *pointerButton;
  wxBitmapButton    *handButton;
  wxBitmapButton    *ellipseButton;
  wxBitmapButton    *createpointButton;


  wxBitmapButton    *selectedButton; // this is so the state of the previously selected button can be changed back to normal
  
  DECLARE_EVENT_TABLE()
};


