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
# include <qpopupmenu.h>
#endif

#include "CommandBarManager.h"
#include "ToolBarManager.h"
#include "Application.h"
#include "Command.h"
#include "ToolBox.h"
#include "ToolBoxBar.h"

using namespace Gui;
using DockWnd::ToolBox;
using DockWnd::ToolBoxBar;

CommandBarManager* CommandBarManager::_instance=0;

CommandBarManager* CommandBarManager::getInstance()
{
  if ( !_instance )
    _instance = new CommandBarManager;
  return _instance;
}

CommandBarManager::CommandBarManager() : _toolBox(0L)
{
}

CommandBarManager::~CommandBarManager()
{
}

void CommandBarManager::setToolBox( DockWnd::ToolBox* tb )
{
  _toolBox = tb;
}

void CommandBarManager::setup( ToolBarItem* toolBar ) const
{
  if ( !toolBar || !_toolBox )
    return; // empty tool bar

  QPtrList<QToolBar> tbs;
  QPtrList<QWidget> rem;
  int ct = _toolBox->count();
  for ( int i=0; i<ct; i++ )
  {
    QWidget* w = _toolBox->item( i );
    if ( w && w->inherits("QToolBar") )
    {
      if ( toolBar->findItem( w->name() ) )
      {
        tbs.append( reinterpret_cast<QToolBar*>(w) );
      }
      else
      {
        // first save all widgets to remove in a temporary list
        rem.append( w );
      }
    }
  }

  // now remove the widgets
  QWidget* w;
  for ( w = rem.first(); w; w = rem.next() )
  {
     _toolBox->removeItem( w );
  }
  
  CommandManager& mgr = ApplicationWindow::Instance->commandManager();
  QPtrList<ToolBarItem> items = toolBar->getItems();

  ToolBarItem* item;
  for ( item = items.first(); item; item = items.next() )
  {
    QToolBar* bar;
    for ( bar = tbs.first(); bar; bar = tbs.next() )
    {
      if ( bar->name() == item->command() )
        break;
    }

    if ( !bar )
    {
      bar = new ToolBoxBar( item->command(), _toolBox, item->command().latin1() );
      _toolBox->addItem( bar, item->command() );

      QPtrList<ToolBarItem> subitems = item->getItems();
      ToolBarItem* subitem;
      for ( subitem = subitems.first(); subitem; subitem = subitems.next() )
      {
        if ( subitem->command() == "Separator" )
          bar->addSeparator();
        else
          mgr.addTo( subitem->command().latin1(), bar );
      }
    }
  }
}
