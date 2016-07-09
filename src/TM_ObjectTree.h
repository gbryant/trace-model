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

class TM_ObjectTree : public wxTreeCtrl
{
public:
  TM_ObjectTree(wxWindow *parent, const wxString &name);
  ~TM_ObjectTree(void);
  void OnItemMenu(wxTreeEvent& event);
  void OnSM_NewObject(wxCommandEvent &event);
  void OnOM_NewSubObject(wxCommandEvent &event);
  void OnOM_Delete(wxCommandEvent &event);
  void OnSOM_Delete(wxCommandEvent &event);
  void OnSelectionChanged(wxTreeEvent &event);
  int GetPointID(int x, int y);
  int TogglePoint(int x, int y);
  int SelectPoint(int x, int y);
  int SelectPoint(int id);
  int SelectPoints(int x, int y, int width, int height);
  void DeSelectPoints(void);
  void MoveSelectedPoints(int xOff, int yOff);
  void MoveSelectedPointsTo(int pID, int xCord, int yCord);
  bool IsPointSelected(int x, int y);
  bool LoadProject(wxString str);

  wxTreeItemId  treeItemID;
  wxPoint       contextPoint;
private:
  wxTreeCtrl        *ObjectTreeCtrl;
  DECLARE_EVENT_TABLE()
};


enum
{
  TM_ObjectTreeControlID = 1,
  SceneItem,
  ObjectItem,
  SubObjectItem,
  OM_NewSubObject,
  OM_Delete,
  SM_NewObject,
  SOM_Delete
};


class MyTreeItemData : public wxTreeItemData
{
public:
  MyTreeItemData(void);
  int       itemType;
};

struct PointList
{
  int     x,y;
  int     selected;
  int     id;
  struct  PointList *next;
};
typedef struct PointList PointList;

class SubObjectItemData : public MyTreeItemData
{
public:
  int         count;
  int         selected_point;
  PointList   points;
  PointList   *currPoint;
  SubObjectItemData   *next;
  SubObjectItemData(void);
  ~SubObjectItemData(void);
  void Add(int x, int y);
  void SelectionIncrement(void);
  void SelectionDecrement(void);
};

class ObjectItemData : public MyTreeItemData
{
public:
  SubObjectItemData   *subObjects;
  SubObjectItemData   *currSubObject;
  ObjectItemData      *next;
  ObjectItemData(void);
  ~ObjectItemData(void);
};

class SceneItemData : public MyTreeItemData
{
public:
  ObjectItemData  *objects;
  ObjectItemData  *currObject;
  SceneItemData(void);
};



