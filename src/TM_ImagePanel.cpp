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


BEGIN_EVENT_TABLE( TM_ImagePanel, wxPanel )
  EVT_PAINT(TM_ImagePanel::OnPaint)
  EVT_SIZE(TM_ImagePanel::OnSize)
  EVT_SCROLLWIN(TM_ImagePanel::OnScroll)
  EVT_MOTION(TM_ImagePanel::OnMotion)
  EVT_KEY_DOWN(TM_ImagePanel::OnKeyDown)
  EVT_KEY_UP(TM_ImagePanel::OnKeyUp)
  EVT_CHAR(TM_ImagePanel::OnChar)
  EVT_LEFT_DOWN(TM_ImagePanel::OnLeftDown)
  EVT_LEFT_UP(TM_ImagePanel::OnLeftUp)
END_EVENT_TABLE()

TM_ImagePanel::TM_ImagePanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name): wxPanel(parent, id, pos, size, style, name)
{
  loadedImage       = 0;
  loadedImageBitmap = 0;
  loadedImageDC     = 0;

  spaceDown = false;
  drag_point = 0;
  show_image = true;
  ld_motion=false;
  ld_control=false;
  clear_on_no_drag=false;
  clear_drag_rectangle=false;

  scaleFactor = 1;


  SetBackgroundStyle(wxBG_STYLE_CUSTOM);
  AdjustScrollBars();
}

TM_ImagePanel::~TM_ImagePanel(void) 
{
}

void TM_ImagePanel::ZoomIn(void) 
{
  float     hScrollPos,vScrollPos,hMax,vMax;
  float     hRatio,vRatio;

  if(scaleFactor>=1){scaleFactor++;}
  else if(scaleFactor<-2){scaleFactor++;}
  else if(scaleFactor==-2){scaleFactor=1;}
  
  hMax=GetScrollRange(wxHORIZONTAL);
  vMax=GetScrollRange(wxVERTICAL);
  hScrollPos=GetScrollPos(wxHORIZONTAL);
  vScrollPos=GetScrollPos(wxVERTICAL);

  hRatio = hMax/hScrollPos;
  vRatio = vMax/vScrollPos;

  AdjustScrollBars();
  
  hMax=GetScrollRange(wxHORIZONTAL);
  vMax=GetScrollRange(wxVERTICAL);

  hScrollPos = hMax/hRatio;
  vScrollPos = vMax/vRatio;

  SetScrollPos(wxHORIZONTAL,hScrollPos);
  SetScrollPos(wxVERTICAL,vScrollPos);

  Refresh();
  Update();
}

void TM_ImagePanel::ZoomOut(void) 
{
  float     hScrollPos,vScrollPos,hMax,vMax;
  float     hRatio,vRatio;

  if(scaleFactor<=-2){scaleFactor--;}
  else if(scaleFactor>1){scaleFactor--;}
  else if(scaleFactor==1){scaleFactor=-2;}

  hMax=GetScrollRange(wxHORIZONTAL);
  vMax=GetScrollRange(wxVERTICAL);
  hScrollPos=GetScrollPos(wxHORIZONTAL);
  vScrollPos=GetScrollPos(wxVERTICAL);

  hRatio = hMax/hScrollPos;
  vRatio = vMax/vScrollPos;

  AdjustScrollBars();

  hMax=GetScrollRange(wxHORIZONTAL);
  vMax=GetScrollRange(wxVERTICAL);

  hScrollPos = hMax/hRatio;
  vScrollPos = vMax/vRatio;

  SetScrollPos(wxHORIZONTAL,hScrollPos);
  SetScrollPos(wxVERTICAL,vScrollPos);


  Refresh();
  Update();
}

void TM_ImagePanel::OnMotion(wxMouseEvent & event)
{
  int                   hScrollPos,vScrollPos;

  hScrollPos = GetScrollPos(wxHORIZONTAL);
  vScrollPos = GetScrollPos(wxVERTICAL);

  int selectedTool = mainFrame->toolPalette->GetSelectedTool();
  
  if(selectedTool==tool_hand)
  {
    if(event.LeftIsDown())
    {
      wxPoint mousePoint = event.GetPosition();
      int deltaX,deltaY;
      deltaX = (spacePoint.x-mousePoint.x);
      deltaY = (spacePoint.y-mousePoint.y);
      this->SetScrollbar(wxHORIZONTAL,this->GetScrollPos(wxHORIZONTAL)+deltaX,0,this->GetScrollRange(wxHORIZONTAL),true);
      this->SetScrollbar(wxVERTICAL,this->GetScrollPos(wxVERTICAL)+deltaY,0,this->GetScrollRange(wxVERTICAL),true);
      Refresh();
      Update();
      spacePoint = mousePoint;
    }
  }
  else if(selectedTool==tool_pointer)
  {
    if(drag_point)
    {
      if(event.Dragging())
      {
        int mouseDownX = event.GetX()+hScrollPos;
        int mouseDownY = event.GetY()+vScrollPos;
        ld_motion = true;
        
        mainFrame->objectTree->MoveSelectedPointsTo(drag_point,mouseDownX/scaleFactor,mouseDownY/scaleFactor);
        
        Refresh();
        Update();
      }
    }
    else
    {
      if(event.Dragging())
      {
        wxClientDC    dc(this);
        selectionCurr.x=event.GetX();
        selectionCurr.y=event.GetY();

        int width,height;
        width=selectionPrev.x-selectionStart.x;
        height=selectionPrev.y-selectionStart.y;
        dc.SetLogicalFunction(wxINVERT);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(selectionStart.x,selectionStart.y,width,height);
        dc.SetBrush(wxNullBrush);
        dc.SetLogicalFunction(wxCOPY);
        
        width=selectionCurr.x-selectionStart.x;
        height=selectionCurr.y-selectionStart.y;
        dc.SetLogicalFunction(wxINVERT);
        dc.SetBrush(*wxTRANSPARENT_BRUSH);
        dc.DrawRectangle(selectionStart.x,selectionStart.y,width,height);
        dc.SetBrush(wxNullBrush);
        dc.SetLogicalFunction(wxCOPY);
        
        clear_drag_rectangle = true;

        selectionPrev.x=event.GetX();
        selectionPrev.y=event.GetY();
      }
    }
  }
  if(!event.LeftIsDown())
  {
    if(spaceDown)
    {
      wxPoint mousePoint = event.GetPosition();
      int deltaX,deltaY;
      deltaX = (spacePoint.x-mousePoint.x);
      deltaY = (spacePoint.y-mousePoint.y);
      this->SetScrollbar(wxHORIZONTAL,this->GetScrollPos(wxHORIZONTAL)+deltaX,0,this->GetScrollRange(wxHORIZONTAL),true);
      this->SetScrollbar(wxVERTICAL,this->GetScrollPos(wxVERTICAL)+deltaY,0,this->GetScrollRange(wxVERTICAL),true);
      Refresh();
      Update();
      spacePoint = mousePoint;
    }
  }

  event.Skip();
}

void TM_ImagePanel::OnChar(wxKeyEvent &event)
{
  int			      keyCode;
  int           modifiers;
  wxTreeItemId  itemID;

  int selectedTool = mainFrame->toolPalette->GetSelectedTool();
  modifiers = event.GetModifiers();
  keyCode=event.GetKeyCode();

  switch(keyCode)
  {
    case WXK_UP:
                if(selectedTool==tool_pointer)
                {
                if(modifiers==wxMOD_SHIFT){mainFrame->objectTree->MoveSelectedPoints(0,-10);}
                else{mainFrame->objectTree->MoveSelectedPoints(0,-1);}
                Refresh();
                Update();
                }
                break;

    case WXK_DOWN:
                if(selectedTool==tool_pointer)
                {
                if(modifiers==wxMOD_SHIFT){mainFrame->objectTree->MoveSelectedPoints(0,10);}
                else{mainFrame->objectTree->MoveSelectedPoints(0,1);}
                Refresh();
                Update();
                }
                break;

    case WXK_LEFT:
                if(selectedTool==tool_pointer)
                {
                if(modifiers==wxMOD_SHIFT){mainFrame->objectTree->MoveSelectedPoints(-10,0);}
                else{mainFrame->objectTree->MoveSelectedPoints(-1,0);}
                Refresh();
                Update(); 
                }
                break;
                
    case WXK_RIGHT:
                if(selectedTool==tool_pointer)
                {
                if(modifiers==wxMOD_SHIFT){mainFrame->objectTree->MoveSelectedPoints(10,0);}
                else{mainFrame->objectTree->MoveSelectedPoints(1,0);}
                Refresh();
                Update(); 
                }
                break;
    case WXK_TAB:
                if(selectedTool==tool_pointer)
                {
                  itemID = mainFrame->objectTree->GetSelection();
                  if(itemID.IsOk())
                  {
                    MyTreeItemData *data = (MyTreeItemData*)mainFrame->objectTree->GetItemData(itemID);
                    if(data->itemType != SubObjectItem)
                    {data=0;}
                    if(data)
                    {
                      if(!modifiers)
                      {
                        ((SubObjectItemData*)data)->SelectionIncrement();
                      }
                      else if(modifiers==wxMOD_SHIFT)
                      {
                        ((SubObjectItemData*)data)->SelectionDecrement();
                      }
                    }
                  }
                  Update();
                  Refresh();
                }
                break;
  }
  


}

void TM_ImagePanel::OnKeyDown(wxKeyEvent& event)
{
  if(!spaceDown)
  {
    spacePoint = event.GetPosition();
    if(event.GetKeyCode()==WXK_SPACE){spaceDown=true;}
  }
  event.Skip();
}

void TM_ImagePanel::OnKeyUp(wxKeyEvent& event)
{
  if(event.GetKeyCode()==WXK_SPACE){spaceDown=false;}
  event.Skip();
}

void TM_ImagePanel::OnLeftUp(wxMouseEvent& event)
{
  int selectedTool = mainFrame->toolPalette->GetSelectedTool();
  int hScrollPos = GetScrollPos(wxHORIZONTAL);
  int vScrollPos = GetScrollPos(wxVERTICAL);
  int topleftX,topleftY,width,height;

  if(selectedTool==tool_pointer)
  {
    if(!ld_motion)
    {
      if(clear_on_no_drag)
      {
        if(drag_point)
        {
          mainFrame->objectTree->DeSelectPoints();
          mainFrame->objectTree->SelectPoint(drag_point);
          clear_on_no_drag=false;
          Update();
          Refresh();
        }
      }
    }
    ld_motion = false;
  }
  if(clear_drag_rectangle)
  {
    wxClientDC    dc(this);

    width=selectionPrev.x-selectionStart.x;
    height=selectionPrev.y-selectionStart.y;
    dc.SetLogicalFunction(wxINVERT);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(selectionStart.x,selectionStart.y,width,height);
    dc.SetBrush(wxNullBrush);
    dc.SetLogicalFunction(wxCOPY);
    
    
    if(selectionStart.x<event.GetX()){topleftX=selectionStart.x+hScrollPos;width=event.GetX()-selectionStart.x;}
    else{topleftX=event.GetX()+hScrollPos;width=selectionStart.x-event.GetX();}
    if(selectionStart.y<event.GetY()){topleftY=selectionStart.y+vScrollPos;height=event.GetY()-selectionStart.y;}
    else{topleftY=event.GetY()+vScrollPos;height=selectionStart.y-event.GetY();}
    
    mainFrame->objectTree->SelectPoints(topleftX/scaleFactor,topleftY/scaleFactor,width/scaleFactor,height/scaleFactor);
    
    clear_drag_rectangle = false;

    Update();
    Refresh();
    
  }
}

void TM_ImagePanel::OnLeftDown(wxMouseEvent& event)
{
  wxTreeItemId          itemID;
  int                   hScrollPos,vScrollPos;
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  int selectedTool = mainFrame->toolPalette->GetSelectedTool();
  
  if(selectedTool==tool_createpoint)
  {    
    itemID = mainFrame->objectTree->GetSelection();
    if(itemID.IsOk())
    {
      MyTreeItemData *data = (MyTreeItemData*)mainFrame->objectTree->GetItemData(itemID);
      if(data->itemType != SubObjectItem)
      {LOG("Error!:item not a sub item type");return;}
      hScrollPos = GetScrollPos(wxHORIZONTAL);
      vScrollPos = GetScrollPos(wxVERTICAL);
      ((SubObjectItemData*)data)->Add((event.GetX()+hScrollPos)/scaleFactor,(event.GetY()+vScrollPos)/scaleFactor);
      Refresh();
      Update();
    }
  }
  else if(selectedTool==tool_pointer)
  { 
    hScrollPos = GetScrollPos(wxHORIZONTAL);
    vScrollPos = GetScrollPos(wxVERTICAL);

    if(event.ControlDown()){ld_control=true;}else{ld_control=false;}
    
    if(ld_control)
    {
      mainFrame->objectTree->TogglePoint((event.GetX()+hScrollPos)/scaleFactor,(event.GetY()+vScrollPos)/scaleFactor);
    }
    else
    {
      if(!mainFrame->objectTree->IsPointSelected((event.GetX()+hScrollPos)/scaleFactor,(event.GetY()+vScrollPos)/scaleFactor))
      {
        mainFrame->objectTree->DeSelectPoints();
        drag_point = mainFrame->objectTree->SelectPoint((event.GetX()+hScrollPos)/scaleFactor,(event.GetY()+vScrollPos)/scaleFactor);
      }
      else
      {
        drag_point = mainFrame->objectTree->GetPointID((event.GetX()+hScrollPos)/scaleFactor,(event.GetY()+vScrollPos)/scaleFactor);
        clear_on_no_drag = true;
      }
    }

    selectionStart.x=event.GetX();
    selectionStart.y=event.GetY();
    selectionPrev.x=event.GetX();
    selectionPrev.y=event.GetY();
    selectionCurr.x=event.GetX();
    selectionCurr.y=event.GetY(); 
    
    Update();
    Refresh();
  }
  spacePoint = event.GetPosition();
  event.Skip();
}


void TM_ImagePanel::OnScroll(wxScrollWinEvent& event)
{
  int pos;

  if(event.GetEventType()==wxEVT_SCROLLWIN_LINEUP)
  {
    if(event.GetOrientation()==wxHORIZONTAL)
    {
      pos = GetScrollPos(wxHORIZONTAL);
      SetScrollPos(wxHORIZONTAL,pos-1,true);
    }
    else if(event.GetOrientation()==wxVERTICAL)
    {
      pos = GetScrollPos(wxVERTICAL);
      SetScrollPos(wxVERTICAL,pos-1,true);
    }
  }
  else if(event.GetEventType()==wxEVT_SCROLLWIN_LINEDOWN)
  {
    if(event.GetOrientation()==wxHORIZONTAL)
    {
      pos = GetScrollPos(wxHORIZONTAL);
      SetScrollPos(wxHORIZONTAL,pos+1,true);
    }
    else if(event.GetOrientation()==wxVERTICAL)
    {
      pos = GetScrollPos(wxVERTICAL);
      SetScrollPos(wxVERTICAL,pos+1,true);
    }
  }
  else if(event.GetEventType()==wxEVT_SCROLLWIN_PAGEUP)
  {
    if(event.GetOrientation()==wxHORIZONTAL)
    {
      pos = GetScrollPos(wxHORIZONTAL);
      SetScrollPos(wxHORIZONTAL,pos-10,true);
    }
    else if(event.GetOrientation()==wxVERTICAL)
    {
      pos = GetScrollPos(wxVERTICAL);
      SetScrollPos(wxVERTICAL,pos-10,true);
    }
  }
  else if(event.GetEventType()==wxEVT_SCROLLWIN_PAGEDOWN)
  {
    if(event.GetOrientation()==wxHORIZONTAL)
    {
      pos = GetScrollPos(wxHORIZONTAL);
      SetScrollPos(wxHORIZONTAL,pos+10,true);
    }
    else if(event.GetOrientation()==wxVERTICAL)
    {
      pos = GetScrollPos(wxVERTICAL);
      SetScrollPos(wxVERTICAL,pos+10,true);
    }
  }
  else if(event.GetEventType()==wxEVT_SCROLLWIN_THUMBTRACK)
  {
    if(event.GetOrientation()==wxHORIZONTAL)
    {
      pos = event.GetPosition();
      SetScrollPos(wxHORIZONTAL,pos,true);
    }
    else if(event.GetOrientation()==wxVERTICAL)
    {
      pos = event.GetPosition();
      SetScrollPos(wxVERTICAL,pos,true);
    }
  }

  Refresh();
  Update();
}


void TM_ImagePanel::AdjustScrollBars(void)
{
  int     clientWidth,clientHeight,imageWidth,imageHeight;

  if(loadedImageDC)
  {
    GetClientSize(&clientWidth,&clientHeight);
    imageWidth = loadedImageBitmap->GetWidth();
    imageHeight = loadedImageBitmap->GetHeight();
    if(scaleFactor>1)
    {
      imageWidth*=scaleFactor;
      imageHeight*=scaleFactor;
    }
    else if(scaleFactor<1)
    {
      imageWidth/=abs(scaleFactor);
      imageHeight/=abs(scaleFactor);
    }
    
    if(imageWidth>clientWidth)
    {
      SetScrollbar(wxHORIZONTAL,GetScrollPos(wxHORIZONTAL),0,imageWidth-clientWidth,true);
    }
    else{SetScrollbar(wxHORIZONTAL,0,0,0,true);}

    if(imageHeight>clientHeight)
    {
      SetScrollbar(wxVERTICAL,GetScrollPos(wxVERTICAL),0,imageHeight-clientHeight,true);
    }
    else{SetScrollbar(wxVERTICAL,0,0,0,true);}
  }
  else
  {
    SetScrollbar(wxHORIZONTAL,0,0,0,true);
    SetScrollbar(wxVERTICAL,0,0,0,true);
  }
}

void TM_ImagePanel::CenterScrollBars(void)
{
  int     clientWidth,clientHeight,imageWidth,imageHeight;

  if(loadedImageDC)
  {
    GetClientSize(&clientWidth,&clientHeight);
    imageWidth = loadedImageBitmap->GetWidth();
    imageHeight = loadedImageBitmap->GetHeight();
    if(scaleFactor>1)
    {
      imageWidth*=scaleFactor;
      imageHeight*=scaleFactor;
    }
    else if(scaleFactor<1)
    {
      imageWidth/=abs(scaleFactor);
      imageHeight/=abs(scaleFactor);
    }
    
    if(imageWidth>clientWidth)
    {
      SetScrollbar(wxHORIZONTAL,((imageWidth-clientWidth)/2),0,imageWidth-clientWidth,true);
    }
    else{SetScrollbar(wxHORIZONTAL,0,0,0,true);}

    if(imageHeight>clientHeight)
    {
      SetScrollbar(wxVERTICAL,((imageHeight-clientHeight)/2),0,imageHeight-clientHeight,true);
    }
    else{SetScrollbar(wxVERTICAL,0,0,0,true);}
  }
  else
  {
    SetScrollbar(wxHORIZONTAL,0,0,0,true);
    SetScrollbar(wxVERTICAL,0,0,0,true);
  }
}

int TM_ImagePanel::LoadImage(wxString str)
{
  wxStopWatch  *stopWatch;
  
  stopWatch = new wxStopWatch();


  scaleFactor=1;
  SetScrollbar(wxHORIZONTAL,0,0,0,true);
  SetScrollbar(wxVERTICAL,0,0,0,true);

  
  if(loadedImageDC){loadedImageDC->SelectObjectAsSource(wxNullBitmap); delete loadedImageDC;loadedImageDC=0;}
  if(loadedImageBitmap){delete loadedImageBitmap;loadedImageBitmap=0;}

  stopWatch->Start(0);
  loadedImage = new wxImage(str.c_str(),wxBITMAP_TYPE_ANY,-1);
  wxLogMessage(wxT("Time Elapsed: %d ms"),stopWatch->Time());
  
  stopWatch->Start(0);
  loadedImageBitmap = new wxBitmap(*loadedImage,-1);
  wxLogMessage(wxT("Time Elapsed: %d ms"),stopWatch->Time());
  
  if(loadedImage){delete loadedImage;loadedImage=0;}

  stopWatch->Start(0);
  loadedImageDC = new wxMemoryDC(*loadedImageBitmap);
  wxLogMessage(wxT("Time Elapsed: %d ms"),stopWatch->Time());
  if (!loadedImageDC->IsOk()){LOG_ERROR}
  
  delete stopWatch;

  AdjustScrollBars();
  CenterScrollBars();

  Refresh();
  Update();
  
return 0;
}

void TM_ImagePanel::OnPaint(wxPaintEvent& event)
{
  int hScrollPos,vScrollPos;
  int xx,yy,ww,hh,dataSize,subImageDataSize,rectWidth,rectHeight,i,j,k,tmpPos,subPos,ii,jj;
  unsigned char  *tempImageData;
  unsigned char  *subImageData;
  wxRect rect;
  wxPoint point;
  SceneItemData        *sceneData;
  ObjectItemData       *currObject;
  SubObjectItemData    *currSubObject;
  PointList            *currPoint;
  int prevX,prevY;

  hScrollPos = GetScrollPos(wxHORIZONTAL);
  vScrollPos = GetScrollPos(wxVERTICAL);




  wxAutoBufferedPaintDC dc(this);
  dc.SetBackground(*wxMEDIUM_GREY_BRUSH);
  dc.Clear();

    
   

  if(loadedImageDC && mainFrame->imagePanel->show_image)
  {
    if(scaleFactor==1)
    {  
      wxRegionIterator upd(GetUpdateRegion());
      while(upd)
      {
        rect = upd.GetRect();
        point = rect.GetTopLeft();
        dc.Blit(point.x,point.y,rect.GetWidth(),rect.GetHeight(),loadedImageDC,point.x+hScrollPos,point.y+vScrollPos,wxCOPY,false,-1,-1);
       upd++;
      }
    }
    else if(scaleFactor>1)
    {
      wxRegionIterator upd(GetUpdateRegion());
      while(upd)
      {
        rect = upd.GetRect();
        rectWidth = rect.GetWidth();
        rectHeight = rect.GetHeight();
        point = rect.GetTopLeft();
        
        xx = (point.x+hScrollPos)/scaleFactor;
        yy = (point.y+vScrollPos)/scaleFactor;
        ww = rectWidth/scaleFactor;if(rectWidth%scaleFactor){ww++;}
        hh = rectHeight/scaleFactor;if(rectHeight%scaleFactor){hh++;}

        if((xx+ww)>loadedImageBitmap->GetWidth()){ww=loadedImageBitmap->GetWidth()-xx;rectWidth=ww*scaleFactor;}
        if((yy+hh)>loadedImageBitmap->GetHeight()){hh=loadedImageBitmap->GetHeight()-yy;rectHeight=hh*scaleFactor;}

        dataSize = rectWidth*rectHeight*3;
        tempImageData = (unsigned char*)malloc(dataSize);

        loadedImageDC->SelectObject(wxNullBitmap);
        wxRect subRect(xx,yy,ww,hh);
        wxBitmap subBitmap = loadedImageBitmap->GetSubBitmap(subRect);
        wxImage subImage = subBitmap.ConvertToImage();
        subImageData = subImage.GetData();
        subImageDataSize = ww*hh*3;
        loadedImageDC->SelectObject(*loadedImageBitmap);
        
        for(i=0;i<rectHeight;i++)
        {
          for(j=0;j<rectWidth;j++)
          {
            tmpPos = (i*rectWidth*3)+(j*3);
            ii = i/scaleFactor;
            jj = j/scaleFactor;
            subPos = (ii*ww*3)+(jj*3);
            tempImageData[tmpPos+0] = subImageData[subPos+0];
            tempImageData[tmpPos+1] = subImageData[subPos+1];
            tempImageData[tmpPos+2] = subImageData[subPos+2];
          }
        }

        wxImage tempImage(rectWidth,rectHeight,false);
        tempImage.SetData(tempImageData);
        wxBitmap tempBitmap(tempImage,-1);
        loadedImageDC->SelectObject(tempBitmap);
        dc.Blit(point.x,point.y,rectWidth,rectHeight,loadedImageDC,0,0,wxCOPY,false,-1,-1);
        loadedImageDC->SelectObject(*loadedImageBitmap);

      upd++;
      }
    }
    else if(scaleFactor<1)
    {
      scaleFactor*=-1;
      wxRegionIterator upd(GetUpdateRegion());
      while(upd)
      {
        rect = upd.GetRect();
        rectWidth = rect.GetWidth();
        rectHeight = rect.GetHeight();
        point = rect.GetTopLeft();
        
        xx = (point.x+hScrollPos)*scaleFactor;
        yy = (point.y+vScrollPos)*scaleFactor;
        ww = rectWidth*scaleFactor;//if(rectWidth%scaleFactor){ww++;}
        hh = rectHeight*scaleFactor;//if(rectHeight%scaleFactor){hh++;}

        if((xx+ww)>loadedImageBitmap->GetWidth()){ww=loadedImageBitmap->GetWidth()-xx;rectWidth=ww/scaleFactor;if(ww%scaleFactor){rectWidth++;}}
        if((yy+hh)>loadedImageBitmap->GetHeight()){hh=loadedImageBitmap->GetHeight()-yy;rectHeight=hh/scaleFactor;if(hh%scaleFactor){rectHeight++;}}

        dataSize = rectWidth*rectHeight*3;
        tempImageData = (unsigned char*)malloc(dataSize);

        loadedImageDC->SelectObject(wxNullBitmap);
        wxRect subRect(xx,yy,ww,hh);
        wxBitmap subBitmap = loadedImageBitmap->GetSubBitmap(subRect);
        wxImage subImage = subBitmap.ConvertToImage();
        subImageData = subImage.GetData();
        subImageDataSize = ww*hh*3;
        loadedImageDC->SelectObject(*loadedImageBitmap);
        
        for(i=0;i<rectHeight;i++)
        {
          for(j=0;j<rectWidth;j++)
          {
            tmpPos = (i*rectWidth*3)+(j*3);
            ii = i*scaleFactor;
            jj = j*scaleFactor;
            subPos = (ii*ww*3)+(jj*3);
            tempImageData[tmpPos+0] = subImageData[subPos+0];
            tempImageData[tmpPos+1] = subImageData[subPos+1];
            tempImageData[tmpPos+2] = subImageData[subPos+2];
          }
        }

        wxImage tempImage(rectWidth,rectHeight,false);
        tempImage.SetData(tempImageData);
        wxBitmap tempBitmap(tempImage,-1);
        loadedImageDC->SelectObject(tempBitmap);
        dc.Blit(point.x,point.y,rectWidth,rectHeight,loadedImageDC,0,0,wxCOPY,false,-1,-1);
        loadedImageDC->SelectObject(*loadedImageBitmap);

      upd++;
      }
    scaleFactor*=-1;
    }
  }

  
  wxTreeItemId rootItem = mainFrame->objectTree->GetRootItem();
  sceneData = (SceneItemData*)mainFrame->objectTree->GetItemData(rootItem);
  
  currObject = sceneData->objects;
  while(currObject)
  {
    currSubObject = currObject->subObjects;
    while(currSubObject)
    {
      //drawing code-------------------------------------------------------------------------------------
      currPoint = &currSubObject->points;
      prevX = currPoint->x*scaleFactor-hScrollPos;
      prevY = currPoint->y*scaleFactor-vScrollPos;
      currPoint = currPoint->next;
      dc.SetLogicalFunction(wxINVERT);
      for(i=1;i<currSubObject->count;i++)
      {
        dc.DrawLine(prevX,prevY,currPoint->x*scaleFactor-hScrollPos,currPoint->y*scaleFactor-vScrollPos);
        prevX = currPoint->x*scaleFactor-hScrollPos;
        prevY = currPoint->y*scaleFactor-vScrollPos;
        currPoint = currPoint->next;
      }
      dc.SetLogicalFunction(wxCOPY);

      currPoint = &currSubObject->points;
      for(i=0;i<currSubObject->count;i++)
      {
        if(currPoint->selected==1)
        {
          dc.SetBrush(*wxGREEN_BRUSH);
          dc.DrawCircle(currPoint->x*scaleFactor-hScrollPos,currPoint->y*scaleFactor-vScrollPos,4*scaleFactor);
          dc.SetBrush(wxNullBrush);
        }
        else
        {dc.DrawCircle(currPoint->x*scaleFactor-hScrollPos,currPoint->y*scaleFactor-vScrollPos,4*scaleFactor);}
        currPoint = currPoint->next;
      }
      //drawing code-------------------------------------------------------------------------------------
      currSubObject = currSubObject->next;
    }
    currObject = currObject->next;
  }
  
}

void TM_ImagePanel::OnSize(wxSizeEvent& event)
{
  AdjustScrollBars(); 
  //the following insures correct drawing for growing a window at scrollPos==scrollMax....why does that happen anyway?
  Refresh();
  Update();
}