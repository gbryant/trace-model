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



#define LOG_ERROR \
  wxFileName fileName(wxT(__FILE__),wxPATH_NATIVE);\
  wxString str = fileName.GetFullName();\
  wxLogMessage(wxT("Error: file: %s line: %d"),str.c_str(),__LINE__);




class TM_App : public wxApp
{
	public:
		virtual bool OnInit();
		virtual int OnExit();
                
};