/***************************************************************************
                          PropertyPage.cpp  -  description
                             -------------------
    begin                : 2002/10/19 10:47:44
    copyright            : (C) 2002 by Werner Mayer
    email                : werner.wm.mayer@gmx.de
 ***************************************************************************/

/** \file PropertyPage.cpp
 *  \brief The property page base class
 *  \author Werner Mayer
 *  \version 0.1
 *  \date    2003/01/06
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
#	include <qbutton.h>
#	include <qlabel.h>
#	include <qlayout.h>
#endif

#include "PropertyPage.h"

using namespace Gui::Dialog;

CPropertyPage::CPropertyPage() 
{
  bChanged = false;
}

void CPropertyPage::apply()
{
}

void CPropertyPage::cancel()
{
}

bool CPropertyPage::isModified()
{
  return bChanged;
}

void CPropertyPage::setModified(bool b)
{
  bChanged = b;
}

void CPropertyPage::onApply()
{
  if (isModified())
    apply();

  setModified(false);
}

void CPropertyPage::onCancel()
{
  if (isModified())
  {
    cancel();
    setModified(false);
  }
}

// -----------------------------------------------------------------

PreferencePage::PreferencePage()
{
}

PreferencePage::~PreferencePage()
{
}
