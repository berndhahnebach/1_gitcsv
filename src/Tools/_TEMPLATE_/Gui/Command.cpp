/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
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
# include <qaction.h>
# include <BRepPrimAPI_MakeBox.hxx>
# include <TopoDS_Shape.hxx>
# include <TNaming_Builder.hxx>
#endif

#include <Base/Exception.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/Command.h>


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================
// Cmd_TEMPLATE_Test THIS IS JUST A TEST COMMAND
//===========================================================================
DEF_STD_CMD(Cmd_TEMPLATE_Test);

Cmd_TEMPLATE_Test::Cmd_TEMPLATE_Test()
  :CppCommand("_TEMPLATE__Test")
{
  sAppModule    = QT_TR_NOOP("_TEMPLATE_");
  sGroup        = QT_TR_NOOP("_TEMPLATE_");
  sMenuText     = QT_TR_NOOP("Test1");
  sToolTipText  = QT_TR_NOOP("_TEMPLATE_ Test function");
  sWhatsThis    = QT_TR_NOOP("_TEMPLATE_ Test function");
  sStatusTip    = QT_TR_NOOP("_TEMPLATE_ Test function");
  sPixmap       = "Test1";
  iAccel        = Qt::CTRL+Qt::Key_T;
}

void Cmd_TEMPLATE_Test::activated(int iMsg)
{
  Base::Console().Message("Hello, World!\n");
}

void CreateCommands(void)
{
  Gui::CommandManager &rcCmdMgr = Gui::ApplicationWindow::Instance->commandManager();
  rcCmdMgr.addCommand(new Cmd_TEMPLATE_Test());
}
