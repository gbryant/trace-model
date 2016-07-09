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

class TM_ImagePanel : public wxPanel
{
public:

  wxImage           *loadedImage;
  wxBitmap          *loadedImageBitmap;
  wxMemoryDC        *loadedImageDC;

  bool              show_image;
  bool              ld_motion;
  bool              ld_control;
  bool              clear_on_no_drag;
  bool              clear_drag_rectangle;
  wxPoint           selectionStart;
  wxPoint           selectionPrev;
  wxPoint           selectionCurr;

  int               drag_point;

  int      scaleFactor;

   TM_ImagePanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name);
   void OnPaint(wxPaintEvent& event);
   void OnSize(wxSizeEvent& event);
   void OnMotion(wxMouseEvent& event);
   void OnKeyDown(wxKeyEvent& event);
   void OnKeyUp(wxKeyEvent& event);
   void OnChar(wxKeyEvent &event);
   void OnLeftDown(wxMouseEvent& event);
   void OnLeftUp(wxMouseEvent& event);
   void OnScroll(wxScrollWinEvent& event);
   void AdjustScrollBars(void);
   void CenterScrollBars(void);
   int LoadImage(wxString str);
   void ZoomIn(void);
   void ZoomOut(void);
  ~TM_ImagePanel(void);
   private:
     bool     spaceDown;
     wxPoint  spacePoint;
    DECLARE_EVENT_TABLE()
};

