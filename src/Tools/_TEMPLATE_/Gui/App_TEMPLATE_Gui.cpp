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
#endif

#include "Workbench.h"

#include <App/Application.h>
#include <Base/Console.h>
#include <Gui/Application.h>
#include <Gui/WorkbenchFactory.h>

// use a different name to CreateCommand()
void Create_TEMPLATE_Commands(void);


/* registration table  */
static struct PyMethodDef _TEMPLATE_Guimethods[] = {
    {NULL, NULL}                   /* end of table marker */
};


/* Python entry */
extern "C" {
void _TEMPLATE_GuiExport init_TEMPLATE_Gui() {
  if ( !Gui::Application::Instance )
  {
    PyErr_SetString(PyExc_ImportError, "Cannot load Gui module in console application.");
    return;
  }

  // instanciating the commands
  Create_TEMPLATE_Commands();
  Gui::WorkbenchFactory().AddProducer("_TEMPLATE_ design", new Gui::WorkbenchProducer<_TEMPLATE_Gui::Workbench>);

  // ADD YOUR CODE HERE
  //
  //

  (void) Py_InitModule("_TEMPLATE_Gui", _TEMPLATE_Guimethods);   /* mod name, table ptr */
  Base::Console().Log("App_TEMPLATE_Gui loaded\n");

  return;
}
} // extern "C" {
