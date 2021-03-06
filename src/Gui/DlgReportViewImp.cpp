/***************************************************************************
 *   Copyright (c) 2004 Werner Mayer <werner.wm.mayer@gmx.de>              *
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

#include "DlgReportViewImp.h"
#include "PrefWidgets.h"


using namespace Gui::Dialog;

/**
 *  Constructs a DlgReportViewImp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DlgReportViewImp::DlgReportViewImp( QWidget* parent, const char* name, WFlags fl )
: DlgReportViewBase(parent, name, fl)
{
}

/** 
 *  Destroys the object and frees any allocated resources
 */
DlgReportViewImp::~DlgReportViewImp()
{
}

void DlgReportViewImp::saveSettings()
{
  checkLogging->onSave();
  checkWarning->onSave();
  checkError->onSave();
  colorText->onSave();
  colorLogging->onSave();
  colorWarning->onSave();
  colorError->onSave();
}

void DlgReportViewImp::loadSettings()
{
  checkLogging->onRestore();
  checkWarning->onRestore();
  checkError->onRestore();
  colorText->onRestore();
  colorLogging->onRestore();
  colorWarning->onRestore();
  colorError->onRestore();
}

#include "DlgReportView.cpp"
#include "moc_DlgReportView.cpp"
