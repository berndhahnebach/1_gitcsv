/***************************************************************************
                          Tools.cpp  -  description
                             -------------------
    begin                : 2003/02/23 10:19:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file Tools.cpp
 *  \brief A set tool classes
 *  \author Werner Mayer
 *  \version 0.1
 *  \date    2003/02/23
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   Werner Mayer 2002                                                     *
 *                                                                         *
 ***************************************************************************/
 

#include "PreCompiled.h"


#ifndef _PreComp_
# include <qpixmap.h>
#endif

#include "Tools.h"
#include <qobjcoll.h>

QPixmap FCTools::resize(int w, int h, QPixmap p)
{
  QPixmap pix = p;
  
  if (pix.width() == 0 || pix.height() == 0)
    return pix; // do not resize a null pixmap

  QPalette pal = qApp->palette();
  QColorGroup g = pal.disabled();

  QPixmap pm = pix;
  pm.resize(w,h);
  pm.fill(g.light());

  QPainter pt;
  pt.begin( &pm );
  pt.setPen( g.light() );
  pt.drawPixmap(1, 1, pix);
  pt.setPen( g.text() );
  pt.drawPixmap(0, 0, pix);
  pt.end();
  return pm;
}

QPixmap FCTools::fillUp(int w, int h, QPixmap p)
{
  if (p.width() == 0 || p.height() == 0)
    w = 1;

  QPixmap pix = p;
  int x = pix.width () > w ? 0 : (w - pix.width ())/2;
  int y = pix.height() > h ? 0 : (h - pix.height())/2;

  QPalette pal = qApp->palette();
  QColorGroup g = pal.disabled();

  QPixmap pm;
  pm.resize(w,h);

#if 0 // the fillTransparentRect() method is not yet implemented correctly
  pm = fillTransparentRect(0, 0, w, h, pm);
  pm = fillOpaqueRect(x, y, p.width(), p.height(), pm);
#else
  pm.fill(g.base());
#endif
  QPainter pt;
  pt.begin( &pm );
  pt.setPen( g.light() );
  pt.drawPixmap(x, y, pix);
  pt.end();
  return pm;
}

void FCTools::clearToolButtons(QToolBar* tb)
{
  if ( !tb->children() )
  	return;
  QObjectListIt it( *tb->children() );
  QObject * obj;
  while( (obj=it.current()) != 0 ) 
  {
  	++it;
	  if ( obj->isWidgetType() )
    {
      if ( obj->inherits("QToolButton") || obj->inherits("QToolBarSeparator") )
  	    delete obj;
    }
  }
}

QPixmap FCTools::fillOpaqueRect(int x, int y, int w, int h, QPixmap p)
{
  if (!p.mask())
    return p; // sorry, but cannot do anything

  QBitmap b = *p.mask();

  // make a opaque pixmap
  QPixmap p2 (w,h);
  p2.fill();

  // modify the mask
  bitBlt(&b, x, y, &p2, 0, 0, w, h, Qt::CopyROP, false);

  p.setMask(b);
  
  return p;
}

QPixmap FCTools::fillTransparentRect(int x, int y, int w, int h, QPixmap p)
{
  if (!p.mask())
    return p; // sorry, but cannot do anything

  QBitmap b = *p.mask();

  // make a transparent pixmap ???
  QPixmap p2 (w,h);
  p2.fill();

  // modify the mask
  bitBlt(&b, x, y, &p2, 0, 0, w, h, Qt::CopyROP, false);

  p.setMask(b);
  
  return p;
}
