/***************************************************************************
                          DlgSettingsImp.cpp  -  description
                             -------------------
    begin                : 2002/08/19 21:11:52
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file $RCSfile$
 *  \brief 
 *  \author Werner Mayer
 *  \version $Revision$
 *  \date    $Date$
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




#include "../Config.h"
#ifdef _PreComp_
#	include "PreCompiled.h"
#else
#	include <qmessagebox.h>
#	include <qiconview.h> 
#	include <qfiledialog.h>
#	include <qcombobox.h>
#       include <Python.h>
#endif

#include "DlgSettingsImp.h"
#include "Application.h"
#include "PrefWidgets.h"

/* 
 *  Constructs a FCDlgSettings which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FCDlgSettings::FCDlgSettings( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : FCDlgSettingsBase( parent, name, modal, fl )
{
  EnableBookmarks->getHandler()->restore();
  EnableHistory->getHandler()->restore();
  NbOfBookmarks->getHandler()->restore();
  NbOfHistoryItems->getHandler()->restore();
  LineEditURL->getHandler()->restore();
  LineEditBrowser->getHandler()->restore();
  connect( buttonOk, SIGNAL( clicked() ), EnableBookmarks->getHandler(), SLOT( save() ) );
  connect( buttonOk, SIGNAL( clicked() ), EnableHistory->getHandler(), SLOT( save() ) );
  connect( buttonOk, SIGNAL( clicked() ), NbOfBookmarks->getHandler(), SLOT( save() ) );
  connect( buttonOk, SIGNAL( clicked() ), NbOfHistoryItems->getHandler(), SLOT( save() ) );
  connect( buttonOk, SIGNAL( clicked() ), LineEditURL->getHandler(), SLOT( save() ) );
  connect( buttonOk, SIGNAL( clicked() ), LineEditBrowser->getHandler(), SLOT( save() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
FCDlgSettings::~FCDlgSettings()
{
    // no need to delete child widgets, Qt does it all for us
}

#include "DlgSettings.cpp"
#include "moc_DlgSettings.cpp"
#include "moc_DlgSettingsImp.cpp"