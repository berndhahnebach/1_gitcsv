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
#endif

#include "Workbench.h"
#include <Gui/ToolBarManager.h>
#include <Gui/MenuManager.h>

using namespace PointsGui;

TYPESYSTEM_SOURCE(PointsGui::Workbench, Gui::StdWorkbench)

Workbench::Workbench()
{
}

Workbench::~Workbench()
{
}

Gui::ToolBarItem* Workbench::setupToolBars() const
{
  Gui::ToolBarItem* root = StdWorkbench::setupToolBars();
  Gui::ToolBarItem* pnt = new Gui::ToolBarItem( root );
  pnt->setCommand( "PointsTools" );
  *pnt << "Points_Import" << "Points_Export";
  return root;
}

Gui::ToolBarItem* Workbench::setupCommandBars() const
{
  // point tools
  Gui::ToolBarItem* root = new Gui::ToolBarItem;
  Gui::ToolBarItem* pnt = new Gui::ToolBarItem( root );
  pnt->setCommand( "PointsTools" );
  *pnt << "Points_Import" << "Points_Export";
  return root;
}

Gui::MenuItem* Workbench::setupMenuBar() const
{
  Gui::MenuItem* root = StdWorkbench::setupMenuBar();
  Gui::MenuItem* item = root->findItem( "&Windows" );
  Gui::MenuItem* pnts = new Gui::MenuItem;
  root->insertItem( item, pnts );

  Gui::MenuItem* test = new Gui::MenuItem;
  test->setCommand( "Test" );
  *test << "Points_Transform";
 
  pnts->setCommand( "&Points" );
  *pnts << test << "Separator" << "Points_Import" << "Points_Export";
  return root;
}

