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

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/dcbuffer.h>
#include <wx/treectrl.h>
#include <wx/aui/aui.h>

#include "tinyxml/tinyxml.h"


enum
{
  tool_pointer = 1,
  tool_hand,
  tool_ellipse,
  tool_createpoint
};


extern class TM_MainFrame *mainFrame;

#include "TM_App.h"
#include "TM_ImagePanel.h"
#include "TM_ObjectTree.h"
#include "TM_LogWindow.h"
#include "TM_ToolPalette.h"

#include "TM_MainFrame.h"



#define LOG(X) wxLogMessage(wxT(X));
#define LOGV(X,Y) wxLogMessage(wxT(X),Y);

extern  TM_LogWindow     *gLogWin;
