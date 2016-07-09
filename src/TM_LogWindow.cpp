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

bool TM_LogWindow::OnFrameClose(wxFrame *frame)
    {
      wxWindow  *topWindow;
      wxCommandEvent test;
      topWindow = wxTheApp->GetTopWindow();
      ((TM_MainFrame*)topWindow)->OnShowLog(test);
      //wxTheApp->ExitMainLoop();
      return true;
    }

TM_LogWindow::TM_LogWindow(wxFrame *parent, const wxChar *title, bool show, bool passToOld):wxLogWindow(parent, title, show, passToOld)
{}