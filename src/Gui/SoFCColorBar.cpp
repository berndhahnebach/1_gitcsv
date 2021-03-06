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
# include <Inventor/SbViewportRegion.h>
# include <Inventor/events/SoMouseButtonEvent.h>
# include <Inventor/nodes/SoBaseColor.h>
# include <Inventor/nodes/SoCoordinate3.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoMaterialBinding.h>
# include <Inventor/nodes/SoIndexedFaceSet.h>
# include <Inventor/nodes/SoSwitch.h>
# include <Inventor/nodes/SoTransform.h>
# include <qapplication.h>
# include <qcursor.h>
# include <qstring.h>
# include <qpopupmenu.h>
#endif

#include <Inventor/nodes/SoEventCallback.h>

#include "SoFCColorBar.h"
#include "SoFCColorGradient.h"
#include "SoFCColorLegend.h"

using namespace Gui;

SO_NODE_ABSTRACT_SOURCE(SoFCColorBarBase);

/*!
  Constructor.
*/
SoFCColorBarBase::SoFCColorBarBase()
{
  SO_NODE_CONSTRUCTOR(SoFCColorBarBase);
}

/*!
  Destructor.
*/
SoFCColorBarBase::~SoFCColorBarBase()
{
  //delete THIS;
}

// doc from parent
void SoFCColorBarBase::initClass(void)
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoFCColorBarBase,SoSeparator,"Separator");
}

// --------------------------------------------------------------------------

SO_NODE_SOURCE(SoFCColorBar);

/*!
  Constructor.
*/
SoFCColorBar::SoFCColorBar()
{
  SO_NODE_CONSTRUCTOR(SoFCColorBar);

//  SoEventCallback * cb = new SoEventCallback;
//  cb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), eventCallback, this);
//  insertChild(cb, 0);

  pColorMode = new SoSwitch;
  addChild(pColorMode);

  _colorBars.push_back( new SoFCColorGradient );
  _colorBars.push_back( new SoFCColorLegend );

  for ( std::vector<SoFCColorBarBase*>::const_iterator it = _colorBars.begin(); it != _colorBars.end(); ++it )
    pColorMode->addChild( *it );
  pColorMode->whichChild = 0;
}

/*!
  Destructor.
*/
SoFCColorBar::~SoFCColorBar()
{
  //delete THIS;
}

// doc from parent
void SoFCColorBar::initClass(void)
{
  SO_NODE_INIT_CLASS(SoFCColorBar,SoFCColorBarBase,"Separator");
}

SoFCColorBarBase* SoFCColorBar::getActiveBar() const
{
  int child = pColorMode->whichChild.getValue();
  return _colorBars[child];
}

void SoFCColorBar::setViewerSize( const SbVec2s& size )
{
  // don't know why the parameter range isn't between [-1,+1]
  float fRatio = ((float)size[0])/((float)size[1]);
  _fMinX=  4.0f, _fMaxX=4.5f;
  _fMinY= -4.0f, _fMaxY=4.0f;
  if ( fRatio > 1.0f )
  {
    _fMinX = 4.0f * fRatio;
    _fMaxX = _fMinX+0.5f;
  }
  else if ( fRatio < 1.0f )
  {
    _fMinY =  -4.0f / fRatio;
    _fMaxY =   4.0f / fRatio;
  }

  for ( std::vector<SoFCColorBarBase*>::const_iterator it = _colorBars.begin(); it != _colorBars.end(); ++it )
    (*it)->setViewerSize( size );
}

void SoFCColorBar::setRange( float fMin, float fMax, int prec )
{
  for ( std::vector<SoFCColorBarBase*>::const_iterator it = _colorBars.begin(); it != _colorBars.end(); ++it )
    (*it)->setRange( fMin, fMax, prec );
}

bool SoFCColorBar::isVisible ( float fVal ) const
{
  return this->getActiveBar()->isVisible( fVal );
}

float SoFCColorBar::getMinValue (void) const
{
  return this->getActiveBar()->getMinValue();
}

float SoFCColorBar::getMaxValue (void) const
{
  return this->getActiveBar()->getMaxValue();
}

bool SoFCColorBar::customize()
{
  return this->getActiveBar()->customize();
}

App::Color SoFCColorBar::getColor( float fVal ) const
{
  return this->getActiveBar()->getColor( fVal );
}

void SoFCColorBar::eventCallback(void * userdata, SoEventCallback * node)
{
  const SoEvent * event = node->getEvent();
  if (event->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
    const SoMouseButtonEvent*  e = reinterpret_cast<const SoMouseButtonEvent*>(event);
    if ((e->getButton() == SoMouseButtonEvent::BUTTON2)) {
      if (e->getState() == SoButtonEvent::UP) {
      }
    }
  }
}

void SoFCColorBar::handleEvent (SoHandleEventAction *action) 
{
  const SoEvent * event = action->getEvent();

  // check for mouse button events
  if (event->getTypeId().isDerivedFrom(SoMouseButtonEvent::getClassTypeId())) {
    const SoMouseButtonEvent*  e = reinterpret_cast<const SoMouseButtonEvent*>(event);

    // calculate the mouse position relative to the colorbar
    //
    const SbViewportRegion&  vp = action->getViewportRegion(); 
    float fRatio = vp.getViewportAspectRatio();
    SbVec2f pos = event->getNormalizedPosition(vp);
    float pX,pY; pos.getValue(pX,pY);

    pX = pX*10.0f-5.0f;
    pY = pY*10.0f-5.0f;

    // now calculate the real points respecting aspect ratio information
    //
    if ( fRatio > 1.0f )
    {
      pX = pX * fRatio;
    }
    else if ( fRatio < 1.0f )
    {
      pY = pY / fRatio;
    }

    // check if the cursor is near to the color bar
    if ( _fMinX > pX || pX > _fMaxX || _fMinY > pY || pY > _fMaxY )
      return; // not inside the rectangle

    // left mouse pressed
    if ((e->getButton() == SoMouseButtonEvent::BUTTON1)) {
      if (e->getState() == SoButtonEvent::DOWN) {
        // double click event
        action->setHandled();
        if ( _timer.restart() < QApplication::doubleClickInterval() )
        {
          if ( getActiveBar()->customize() )
            Notify(0);
        }
      }
    }
    // right mouse pressed
    else if ((e->getButton() == SoMouseButtonEvent::BUTTON2)) {
      if (e->getState() == SoButtonEvent::UP) {
        action->setHandled();

        SoFCColorBarBase* current = getActiveBar();
        QPopupMenu menu;
        menu.setCheckable( true );
        int i=0;
        for ( std::vector<SoFCColorBarBase*>::const_iterator it = _colorBars.begin(); it != _colorBars.end(); ++it )
        {
          int id = menu.insertItem( (*it)->getColorBarName(), i++ );
          menu.setItemChecked( id, (*it) == current );
        }

        menu.insertSeparator();
        int opt = menu.insertItem("Options...");
        int id = menu.exec( QCursor::pos() );

        if ( id > -1 && pColorMode->whichChild.getValue() != id )
          pColorMode->whichChild = id;
        else if ( id == opt )
        {
          if ( customize() )
            Notify(0);
        }
      }
    }
  }
}
