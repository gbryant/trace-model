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

int gID;



SceneItemData::SceneItemData(void) : MyTreeItemData()
{
  itemType = SceneItem;
  objects=0;
  currObject=0;
  gID=1;
}

ObjectItemData::ObjectItemData(void) : MyTreeItemData()
{
  itemType = ObjectItem;
  subObjects=0;
  currSubObject=0;
  next=0;
}

ObjectItemData::~ObjectItemData(void)
{
  SceneItemData   *sceneData;
  ObjectItemData  *currentObject;
  
  sceneData = (SceneItemData*)mainFrame->objectTree->GetItemData(mainFrame->objectTree->GetItemParent(GetId()));

  currentObject = sceneData->objects;
  if(this!=currentObject)
  {
    while(this!=currentObject->next)
    {
      currentObject = currentObject->next;
      LOG("iterate")
    }
    LOG("found")
    if(this==sceneData->currObject){sceneData->currObject = currentObject;LOG("reassignment")}
    currentObject->next = currentObject->next->next;
  }
  else
  {
    LOG("top")
    if(currentObject->next)
    {
      LOG("not only")
      sceneData->objects = sceneData->objects->next;
    }
    else
    {
      LOG("only")
      sceneData->objects=0;
      sceneData->currObject=0;
    }
  }

  
  LOG("~ObjectItemData")
}

void SubObjectItemData::SelectionIncrement(void)
{
  PointList   *point;
  int         i;

  selected_point++;
  if(selected_point>count){selected_point=1;}

  point = &points;
  for(i=1;i<=count;i++)
  {
    if(i==selected_point){point->selected=1;}
    else{point->selected=0;}
    point = point->next;
  }

}

void SubObjectItemData::SelectionDecrement(void)
{
  PointList   *point;
  int         i;

  selected_point--;
  if(selected_point<1){selected_point=count;}

  point = &points;
  for(i=1;i<=count;i++)
  {
    if(i==selected_point){point->selected=1;}
    else{point->selected=0;}
    point = point->next;
  }
}

SubObjectItemData::SubObjectItemData(void) : MyTreeItemData()
{
  itemType = SubObjectItem;
  next=0;
  currPoint = &points;
  currPoint->next=0;
  count=0;
  selected_point=0;
}

SubObjectItemData::~SubObjectItemData(void)
{
  ObjectItemData      *objectData;
  SubObjectItemData   *currentSubObject;
  PointList           *currentPoint,*toFree;

  objectData = (ObjectItemData*)mainFrame->objectTree->GetItemData(mainFrame->objectTree->GetItemParent(GetId()));
  
  currentSubObject = objectData->subObjects;
  if(this!=currentSubObject)
  {
    while(this!=currentSubObject->next)
    {
      currentSubObject = currentSubObject->next;
      LOG("iterate")
    }
    LOG("found")
    if(this==objectData->currSubObject){objectData->currSubObject = currentSubObject;LOG("reassignment")}
    currentSubObject->next = currentSubObject->next->next;
  }
  else
  {
    LOG("top")
    if(currentSubObject->next)
    {
      LOG("not only")
      objectData->subObjects = objectData->subObjects->next;
    }
    else
    {
      LOG("only")
      objectData->subObjects=0;
      objectData->currSubObject=0;
    }
  }
  
  currentPoint = points.next;
  if(currentPoint)
  {
    while(currentPoint->next)
    {
      toFree = currentPoint;
      currentPoint = currentPoint->next;
      free(toFree);
      LOG("point free")
    }
  }
  
  LOG("~SubObjectItemData")
}

void SubObjectItemData::Add(int x, int y)
{
  count++;
  currPoint->x=x;
  currPoint->y=y;
  currPoint->id=gID;gID++;
  currPoint->selected=0;
  currPoint->next = (PointList*)malloc(sizeof(PointList));
  currPoint = currPoint->next;
  currPoint->next=0;
}

MyTreeItemData::MyTreeItemData(void) : wxTreeItemData()
{
}



BEGIN_EVENT_TABLE( TM_ObjectTree, wxTreeCtrl )
  EVT_MENU( SM_NewObject, TM_ObjectTree::OnSM_NewObject )
  EVT_MENU( OM_NewSubObject, TM_ObjectTree::OnOM_NewSubObject )
  EVT_MENU( OM_Delete, TM_ObjectTree::OnOM_Delete )
  EVT_MENU( SOM_Delete, TM_ObjectTree::OnSOM_Delete )
  EVT_TREE_ITEM_MENU(TM_ObjectTreeControlID, TM_ObjectTree::OnItemMenu)
  EVT_TREE_SEL_CHANGED(TM_ObjectTreeControlID,TM_ObjectTree::OnSelectionChanged)
END_EVENT_TABLE()



TM_ObjectTree::TM_ObjectTree(wxWindow *parent, const wxString &name): wxTreeCtrl(parent, TM_ObjectTreeControlID, wxPoint(0,0), wxSize(200,-1), wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS,wxDefaultValidator, name)
{
  wxTreeItemId      parentID,itemID;

  parentID = this->AddRoot(wxT("Untitled Scene"),-1,-1,new SceneItemData());

}

TM_ObjectTree::~TM_ObjectTree(void)
{
}

bool TM_ObjectTree::LoadProject(wxString str)
{
  TiXmlDocument xmlDoc;
  if(xmlDoc.LoadFile(str)){wxLogMessage(wxT("XML load success!"));}
  else{wxLogMessage(wxT("XML load fail!"));}
  return true;
}

void TM_ObjectTree::OnItemMenu(wxTreeEvent& event)
{
  wxMenu  *menu=0;
  
  MyTreeItemData *data = (MyTreeItemData*)GetItemData(event.GetItem());
  if(data->itemType == SceneItem)
  {
    menu = new wxMenu(wxT("Scene Menu"));
    menu->Append(SM_NewObject, wxT("&New Object"));
    treeItemID = event.GetItem();
    contextPoint = event.GetPoint();
  }
  else if(data->itemType == ObjectItem)
  {
    menu = new wxMenu(wxT("Object Menu"));
    menu->Append(OM_NewSubObject, wxT("&New SubObject"));
    menu->Append(OM_Delete, wxT("&Delete"));
    treeItemID = event.GetItem();
    contextPoint = event.GetPoint();
  }
  else if(data->itemType == SubObjectItem)
  {
    menu = new wxMenu(wxT("SubObject Menu"));
    menu->Append(SOM_Delete, wxT("&Delete"));
    treeItemID = event.GetItem();
    contextPoint = event.GetPoint();
  }
  if(menu)
  {
  wxPoint clientpt = event.GetPoint();
  PopupMenu(menu, clientpt);
  delete menu;
  }
}


void TM_ObjectTree::OnSM_NewObject(wxCommandEvent &event)
{
  SceneItemData *sceneData;
  
  ObjectItemData *data = new ObjectItemData();
  data->next=0;
  AppendItem(GetRootItem(),wxT("Untitled Object"),-1,-1,data);
  Expand(GetRootItem());
  
  sceneData = (SceneItemData*)GetItemData(treeItemID);
  if(!sceneData->objects)
  {
    sceneData->objects = data;
    sceneData->currObject = data;
  }
  else
  {
    sceneData->currObject->next = data;
    sceneData->currObject = data;
  }

  this->Update();
}

void TM_ObjectTree::OnOM_NewSubObject(wxCommandEvent &event)
{
  ObjectItemData  *objectData;

  SubObjectItemData *data = new SubObjectItemData();
  AppendItem(treeItemID,wxT("Untitled SubObject"),-1,-1,data);
  Expand(treeItemID);

  objectData = (ObjectItemData*)GetItemData(treeItemID);
  if(!objectData->subObjects)
  {
    objectData->subObjects = data;
    objectData->currSubObject = data;
  }
  else
  {
    objectData->currSubObject->next = data;
    objectData->currSubObject = data;
  }
}

void TM_ObjectTree::OnOM_Delete(wxCommandEvent &event)
{
  if(treeItemID.IsOk())
  {
    wxMessageDialog confirmDlg(this,wxT("Do wish to delete this item?"),wxT("Confirm Delete"),wxYES_NO|wxICON_QUESTION,contextPoint);
    if(confirmDlg.ShowModal()==wxID_YES)
    {
      Delete(treeItemID);
      mainFrame->imagePanel->Update();
      mainFrame->imagePanel->Refresh();
    }
  }
}

void TM_ObjectTree::OnSOM_Delete(wxCommandEvent &event)
{
  if(treeItemID.IsOk())
  {
    wxMessageDialog confirmDlg(this,wxT("Do wish to delete this item?"),wxT("Confirm Delete"),wxYES_NO|wxICON_QUESTION,contextPoint);
    if(confirmDlg.ShowModal()==wxID_YES)
    {
      Delete(treeItemID);
      mainFrame->imagePanel->Update();
      mainFrame->imagePanel->Refresh();
    }
  }
}

void TM_ObjectTree::OnSelectionChanged(wxTreeEvent &event)
{
  wxTreeItemId      id;
  MyTreeItemData    *treeItemData;

  id = event.GetItem();
  treeItemData = (MyTreeItemData*)GetItemData(id);
  if(treeItemData->itemType!=SubObjectItem)
  {
    int selectedTool = mainFrame->toolPalette->GetSelectedTool();
    if(selectedTool==tool_createpoint||selectedTool==tool_ellipse)
    {mainFrame->toolPalette->SetSelectedTool(tool_pointer);}
    mainFrame->toolPalette->DisableTool(tool_createpoint);
    mainFrame->toolPalette->DisableTool(tool_ellipse);
    SetFocus();
  }
  else
  {
    mainFrame->toolPalette->EnableTool(tool_createpoint);
    mainFrame->toolPalette->EnableTool(tool_ellipse);
    SetFocus();
  }

}

void TM_ObjectTree::MoveSelectedPointsTo(int pID, int xCord, int yCord)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i,xOff,yOff;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->id==pID)
        {
          xOff =  xCord-currPoint->x;
          yOff =  yCord-currPoint->y;
          goto IDFOUND;
        }
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }
  IDFOUND:;
  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->selected)
        {
          currPoint->x+=xOff;
          currPoint->y+=yOff;
        }
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

}

void TM_ObjectTree::MoveSelectedPoints(int xOff, int yOff)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->selected)
        {
          currPoint->x+=xOff;
          currPoint->y+=yOff;
        }
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }
}

bool TM_ObjectTree::IsPointSelected(int x, int y)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i,xtest,ytest;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        xtest=currPoint->x-x;if(xtest<0){xtest*=-1;}
        ytest=currPoint->y-y;if(ytest<0){ytest*=-1;}
        if(xtest<=4&&ytest<=4){if(currPoint->selected){return true;}else{return false;}}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }
return false;
}

int TM_ObjectTree::GetPointID(int x, int y)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i,xtest,ytest;
  int                   rVal=0;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        xtest=currPoint->x-x;if(xtest<0){xtest*=-1;}
        ytest=currPoint->y-y;if(ytest<0){ytest*=-1;}
        if(xtest<=4&&ytest<=4){return currPoint->id;}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

  return rVal;
}

int TM_ObjectTree::TogglePoint(int x, int y)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i,xtest,ytest;
  int                   rVal=0;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        xtest=currPoint->x-x;if(xtest<0){xtest*=-1;}
        ytest=currPoint->y-y;if(ytest<0){ytest*=-1;}
        if(xtest<=4&&ytest<=4){if(currPoint->selected){currPoint->selected=0;}else{currPoint->selected=1;};return currPoint->id;}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

  return rVal;
}

int TM_ObjectTree::SelectPoint(int x, int y)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i,xtest,ytest;
  int                   rVal=0;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        xtest=currPoint->x-x;if(xtest<0){xtest*=-1;}
        ytest=currPoint->y-y;if(ytest<0){ytest*=-1;}
        if(xtest<=4&&ytest<=4){currPoint->selected=1;return currPoint->id;}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

  return rVal;
}

int TM_ObjectTree::SelectPoint(int id)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i;
  int                   rVal=0;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->id==id){currPoint->selected=1;return currPoint->id;}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

  return rVal;
}


int TM_ObjectTree::SelectPoints(int x, int y, int width, int height)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i;
  int                   rVal=0;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->x>=x && currPoint->x<=x+width)
        {
          if(currPoint->y>=y && currPoint->y<=y+height)
          {
            if(currPoint->selected){currPoint->selected=false;}else{currPoint->selected=true;}
            rVal++;
          }
        }
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }

  return rVal;
}


void TM_ObjectTree::DeSelectPoints(void)
{
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  wxTreeItemId          rootItem;
  int                   i;

  rootItem = GetRootItem();
  sceneData = (SceneItemData*)GetItemData(rootItem);

  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      currPoint = &currSubObject->points;
      for(i=1;i<=currSubObject->count;i++)
      {
        if(currPoint->selected){currPoint->selected=0;}
        currPoint = currPoint->next;
      }
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }
}












