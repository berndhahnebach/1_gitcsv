/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <werner.wm.mayer@gmx.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
# include <qapplication.h>
# include <qevent.h>
# include <qpainter.h>
# include <qpixmap.h>
# include <qpopupmenu.h>
# include <Inventor/events/SoEvent.h>
# include <Inventor/events/SoKeyboardEvent.h>
# include <Inventor/events/SoLocation2Event.h>
# include <Inventor/events/SoMouseButtonEvent.h>
#endif

#include <Base/Console.h>

#include "MouseModel.h"
#include "View3DInventor.h"
#include "View3DInventorViewer.h"

using namespace Gui; 

AbstractMouseModel::AbstractMouseModel() : _pcView3D(0)
{
}

void AbstractMouseModel::grabMouseModel( Gui::View3DInventorViewer* viewer )
{
  _pcView3D=viewer;
  m_cPrevCursor = _pcView3D->getWidget()->cursor();
  
  // do initialization of your mousemodel
  initialize();
}

void AbstractMouseModel::releaseMouseModel()
{
  // do termination of your mousemodel
  terminate();

  _pcView3D->getWidget()->setCursor(m_cPrevCursor);
  _pcView3D = 0;
}

int AbstractMouseModel::handleEvent(const SoEvent * const ev, const SbViewportRegion& vp)
{
  int ret=Continue;

  const SbVec2s& sz = vp.getWindowSize(); 
  short w,h; sz.getValue(w,h);

  SbVec2s loc = ev->getPosition();
  short x,y; loc.getValue(x,y);
  y = h-y; // the origin is at the left bottom corner (instead of left top corner)

  if (ev->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) 
  {
    const SoMouseButtonEvent * const event = (const SoMouseButtonEvent *) ev;
    const SbBool press = event->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

    if ( press )
    {
      float fRatio = vp.getViewportAspectRatio();
      SbVec2f pos = ev->getNormalizedPosition(vp);
      float pX,pY; pos.getValue(pX,pY);

      SbVec2f org = vp.getViewportOrigin();
      float Ox, Oy; org.getValue( Ox, Oy );

      SbVec2f siz = vp.getViewportSize();
      float dX, dY; siz.getValue( dX, dY );

      // now calculate the real points respecting aspect ratio information
      //
      if ( fRatio > 1.0f )
      {
        pX = ( pX - 0.5f*dX ) * fRatio + 0.5f*dX;
        pos.setValue(pX,pY);
      }
      else if ( fRatio < 1.0f )
      {
        pY = ( pY - 0.5f*dY ) / fRatio + 0.5f*dY;
        pos.setValue(pX,pY);
      }

      _clPoly.push_back( pos );

      ret = mouseButtonEvent(reinterpret_cast<const SoMouseButtonEvent*>(ev), QPoint(x,y));
    }
    else
    {
      ret = mouseButtonEvent(reinterpret_cast<const SoMouseButtonEvent*>(ev), QPoint(x,y));
    }
  }
  else if (ev->getTypeId().isDerivedFrom(SoLocation2Event::getClassTypeId()))
  {
    ret = locationEvent(reinterpret_cast<const SoLocation2Event*>(ev), QPoint(x,y));
  }
  else if (ev->getTypeId().isDerivedFrom(SoKeyboardEvent::getClassTypeId()))
  {
    SoKeyboardEvent * ke = (SoKeyboardEvent *)ev;
    switch (ke->getKey())
    {
    case SoKeyboardEvent::ESCAPE:
      releaseMouseModel();
      ret = Cancel;
      break;
    default:
      ret = keyboardEvent(reinterpret_cast<const SoKeyboardEvent*>(ev));
      break;
    }
  }

  if ( ret == Restart )
    _clPoly.clear();

  return ret;
}

// -----------------------------------------------------------------------------------

BaseMouseModel::BaseMouseModel()
  :AbstractMouseModel()
{
}

// -----------------------------------------------------------------------------------

/* XPM */
static const char *cursor_polypick[]={
"32 32 2 1",
"# c #646464",
". c None",
"................................",
"................................",
".......#........................",
".......#........................",
".......#........................",
"................................",
".......#........................",
"..###.###.###...................",
".......#...............#........",
"......................##........",
".......#..............#.#.......",
".......#.............#..#.......",
".......#............#...#.......",
"....................#....#......",
"...................#.....#......",
"..................#......#......",
"............#.....#.......#.....",
"...........#.##..#........#.....",
"..........#....##.........#.....",
".........#...............#......",
"........#................#......",
".......#................#.......",
"......#.................#.......",
".....#.................#........",
"....#####..............#........",
".........#########....#.........",
"..................#####.........",
"................................",
"................................",
"................................",
"................................",
"................................"};

PolyPickerMouseModel::PolyPickerMouseModel() 
{
  m_iRadius    = 2;
  m_iNodes     = 0;
  m_bWorking   = false;
  m_bDrawNodes = true;
}

void PolyPickerMouseModel::initialize()
{
  QPixmap p(cursor_polypick);
  QCursor cursor( p, 7, 7 );
  _pcView3D->getWidget()->setCursor(cursor);
}

void PolyPickerMouseModel::terminate()
{
//  _pcView3D->getGLWidget()->releaseMouse();
}

void PolyPickerMouseModel::redraw()
{
  if ( _cNodeVector.size() > 2 )
  {
    QPoint start = _cNodeVector.front();
    for ( std::vector<QPoint>::iterator it = _cNodeVector.begin()+1; it != _cNodeVector.end(); ++it )
    {
      _pcView3D->drawLine(start.x(),start.y(),it->x(), it->y() );
      start = *it;
    }
  }
  draw();
}

void PolyPickerMouseModel::draw ()
{
  if ( m_bWorking )
  {
    if (m_iNodes < int(_cNodeVector.size()))
    {
      m_iNodes = int(_cNodeVector.size());
      // drawing the point
      if (m_bDrawNodes == true)
      {
        _pcView3D->drawNode(m_iXnew-m_iRadius,m_iYnew-m_iRadius,2*m_iRadius,2*m_iRadius);
      }

      if ( _cNodeVector.size() > 2 )
      {
        QPoint start = _cNodeVector.front();
        _pcView3D->drawLine(m_iXnew,m_iYnew,start.x(), start.y() );
      }
    }
    else
    {
      _pcView3D->drawLine(m_iXnew,m_iYnew,m_iXold,m_iYold );
      if ( _cNodeVector.size() > 1 )
      {
        QPoint start = _cNodeVector.front();
        _pcView3D->drawLine(m_iXnew,m_iYnew,start.x(), start.y() );
      }
    }
  }
}

PolyPickerMouseModel::~PolyPickerMouseModel()
{
}

int PolyPickerMouseModel::mouseButtonEvent( const SoMouseButtonEvent * const e, const QPoint& pos )
{
  const int button = e->getButton();
  const SbBool press = e->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

  if ( press )
  {
    switch ( button )
    {
    case SoMouseButtonEvent::BUTTON1:
      {
        // start working from now on
        if ( !m_bWorking )    
        {
          m_bWorking = true;
          // clear the old polygon
          _cNodeVector.clear();
          _pcView3D->getGLWidget()->update();
//          _pcView3D->getGLWidget()->grabMouse();
        }

        _cNodeVector.push_back(pos);

        m_iXnew = pos.x();  m_iYnew = pos.y();
        m_iXold = pos.x();  m_iYold = pos.y();
      } break;
    case SoMouseButtonEvent::BUTTON2:
      {
        if ( _cNodeVector.size() > 0 )
        {
          if ( _cNodeVector.back() != pos )
            _cNodeVector.push_back(pos);
          m_iXnew = pos.x();  m_iYnew = pos.y();
          m_iXold = pos.x();  m_iYold = pos.y();
        }

        QCursor cur = _pcView3D->getWidget()->cursor();
        _pcView3D->getWidget()->setCursor(m_cPrevCursor);
//        _pcView3D->getGLWidget()->releaseMouse();
      
        QPopupMenu menu;
        int fi = menu.insertItem("Finish");
        int ca = menu.insertItem("Cancel");
        if ( getPolygon().size() < 3 )
          menu.setItemEnabled(fi,false);
        int id = menu.exec(QCursor::pos());
        
//        _pcView3D->getGLWidget()->grabMouse();
    
        if ( id == fi )
        {
          releaseMouseModel();
          return Finish;
        }
        else if ( id == ca )
        {
          releaseMouseModel();
          return Cancel;
        }
        else
        {
          m_bWorking = false;
          m_iNodes = 0;
          _pcView3D->getWidget()->setCursor(cur);
          return Restart;
        }
      } break;
    default:
      {
      } break;
    }
  }

  return Continue;
}

int PolyPickerMouseModel::locationEvent( const SoLocation2Event * const e, const QPoint& pos )
{
  // do all the drawing stuff for us
  QPoint clPoint = pos;

  if ( m_bWorking )
  {
    // check the position
    QRect r = _pcView3D->getGLWidget()->rect();
    if ( !r.contains( clPoint ) )
    {
      if( clPoint.x() < r.left() )
          clPoint.setX( r.left() );
      if( clPoint.x() > r.right() )
          clPoint.setX( r.right() );
      if( clPoint.y() < r.top() )
          clPoint.setY( r.top() );
      if( clPoint.y() > r.bottom() )
          clPoint.setY( r.bottom() );

#ifdef FC_OS_WINDOWS
      QPoint newPos = _pcView3D->getGLWidget()->mapToGlobal( clPoint );
      QCursor::setPos( newPos );
#endif
    }
  }

  draw();
  m_iXnew = clPoint.x();
  m_iYnew = clPoint.y();
  draw();

  return Continue;
}

int PolyPickerMouseModel::keyboardEvent( const SoKeyboardEvent * const e )
{
  return Continue;
}

// -----------------------------------------------------------------------------------

SelectionMouseModel::SelectionMouseModel()
{
  m_bWorking = false;
}

SelectionMouseModel::~SelectionMouseModel()
{
}

void SelectionMouseModel::initialize()
{
}

void SelectionMouseModel::terminate()
{
}

void SelectionMouseModel::draw ()
{
  if (m_bWorking)
    _pcView3D->drawRect( m_iXold, m_iYold, m_iXnew, m_iYnew );
}

int SelectionMouseModel::mouseButtonEvent( const SoMouseButtonEvent * const e, const QPoint& pos )
{
  const int button = e->getButton();
  const SbBool press = e->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

  int ret = Continue;

  if ( press )
  {
    switch ( button )
    {
    case SoMouseButtonEvent::BUTTON1:
      {
        m_bWorking = true;
        m_iXold = m_iXnew = pos.x(); 
        m_iYold = m_iYnew = pos.y();
      } break;
    default:
      {
      } break;
    }
  }
  else
  {
    switch ( button )
    {
    case SoMouseButtonEvent::BUTTON1:
      {
        m_bWorking = false;
        ret = Finish;
      } break;
    default:
      {
      } break;
    }
  }

  return ret;
}

int SelectionMouseModel::locationEvent( const SoLocation2Event * const e, const QPoint& pos )
{
  draw();
  m_iXnew = pos.x(); 
  m_iYnew = pos.y();
  draw();
  return Continue;
}

int SelectionMouseModel::keyboardEvent( const SoKeyboardEvent * const e )
{
  return Continue;
}

// -----------------------------------------------------------------------------------

/* XPM */
static const char *xpm_cursor[]={
"32 32 2 1",
". c None",
"# c #ffffff",
"................................",
"................................",
"................................",
"................................",
".............######.............",
"..........###......###..........",
".........#............#.........",
".......##..............##.......",
"......#..................#......",
".....#....................#.....",
".....#....................#.....",
"....#......................#....",
"....#......................#....",
"....#......................#....",
"...#........................#...",
"...#...........##...........#...",
"...#...........##...........#...",
"...#........................#...",
"....#......................#....",
"....#......................#....",
"....#......................#....",
".....#....................#.....",
".....#....................#.....",
"......#..................#......",
".......##..............##.......",
".........#............#.........",
"..........###......###..........",
".............######.............",
"................................",
"................................",
"................................",
"................................"};

CirclePickerMouseModel::CirclePickerMouseModel()
: _nRadius(50), _bWorking(false)
{
  QPoint p = QCursor::pos();
  m_iXnew = p.x(); 
  m_iYnew = p.y();
}

CirclePickerMouseModel::~CirclePickerMouseModel()
{
}

void CirclePickerMouseModel::initialize()
{
  QPixmap p(xpm_cursor);
  QCursor cursor( p );
  _pcView3D->getWidget()->setCursor(cursor);
}

void CirclePickerMouseModel::terminate()
{
  draw();
}

void CirclePickerMouseModel::draw ()
{
  char szBuf[20];
  float fRad = float(_nRadius)/10.0f;//float(getView()->Convert(Standard_Integer(_nRadius)));

  sprintf(szBuf, "%.2f", fRad);
  _pcView3D->drawCircle(m_iXnew, m_iYnew, _nRadius);
  _pcView3D->drawText(m_iXnew+9, m_iYnew-9, szBuf);
}

int CirclePickerMouseModel::mouseButtonEvent( const SoMouseButtonEvent * const e, const QPoint& pos )
{
  const int button = e->getButton();
  const SbBool press = e->getState() == SoButtonEvent::DOWN ? TRUE : FALSE;

  int ret = Continue;

  if ( press )
  {
    int delta = 120 / QApplication::wheelScrollLines();

    switch ( button )
    {
    case SoMouseButtonEvent::BUTTON4:
      {
        draw();
        if ( _nRadius < 20 )
          delta /= 2;
        _nRadius = 5>(_nRadius + delta / 10)?5:(_nRadius + delta / 10);
        draw();
      } break;
    case SoMouseButtonEvent::BUTTON5:
      {
        draw();
        delta = -delta;
        if ( _nRadius < 20 )
          delta /= 2;
        _nRadius = 5>(_nRadius + delta / 10)?5:(_nRadius + delta / 10);
        draw();
      } break;
    default:
      {
      } break;
    }
  }
  else
  {
    switch ( button )
    {
    case SoMouseButtonEvent::BUTTON2:
      {
        QCursor cur = _pcView3D->getWidget()->cursor();
        _pcView3D->getWidget()->setCursor(m_cPrevCursor);
      
        QPopupMenu menu;
        int fi = menu.insertItem("Finish");
        int ca = menu.insertItem("Cancel");
        int id = menu.exec(QCursor::pos());
    
        if ( id == fi )
        {
          releaseMouseModel();
          return Finish;
        }
        else if ( id == ca )
        {
          releaseMouseModel();
          return Cancel;
        }
        else
        {
          _pcView3D->getWidget()->setCursor(cur);
        }
      } break;
    default:
      {
      } break;
    }
  }

  return ret;
}

int CirclePickerMouseModel::locationEvent( const SoLocation2Event * const e, const QPoint& pos )
{
  if ( !_bWorking )
  {
    _bWorking = true;
    m_iXnew = pos.x(); 
    m_iYnew = pos.y();
    draw();
  }
  else
  {
    draw();
    m_iXnew = pos.x(); 
    m_iYnew = pos.y();
    draw();
  }
  return Continue;
}

int CirclePickerMouseModel::keyboardEvent( const SoKeyboardEvent * const e )
{
  return Continue;
}
