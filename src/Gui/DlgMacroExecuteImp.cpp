/** \file DlgMacroExecuteImp.cpp
 *  \brief  
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 *   
 */

/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        * 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/


/** Precompiled header stuff
 *  on some compilers the precompiled header option gain significant compile 
 *  time! So every external header (libs and system) should included in 
 *  Precompiled.h. For systems without precompilation the header needed are
 *  included in the else fork.
 */
#include "../Config.h"
#ifdef _PreComp_
#	include "PreCompiled.h"
#else
#endif
#include "DlgMacroExecuteImp.h"

/* 
 *  Constructs a DlgMacroExecuteImp which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DlgMacroExecuteImp::DlgMacroExecuteImp( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : DlgMacroExecute( parent, name, modal, fl ),FCWindow(name)
{
}

/*  
 *  Destroys the object and frees any allocated resources
 */
DlgMacroExecuteImp::~DlgMacroExecuteImp()
{
    // no need to delete child widgets, Qt does it all for us
}

/* 
 * public slot
 */
void DlgMacroExecuteImp::OnExecute()
{
    qWarning( "DlgMacroExecuteImp::OnExecute() not yet implemented!" ); 
}
/* 
 * public slot
 */
void DlgMacroExecuteImp::OnNewFolder()
{
    qWarning( "DlgMacroExecuteImp::OnNewFolder() not yet implemented!" ); 
}
/* 
 * public slot
 */
void DlgMacroExecuteImp::OnRecord()
{
    qWarning( "DlgMacroExecuteImp::OnRecord() not yet implemented!" ); 
}
/* 
 * public slot
 */
void DlgMacroExecuteImp::OnNewListItemPicked(QListViewItem*)
{
    qWarning( "DlgMacroExecuteImp::OnNewListItemPicked(QListViewItem*) not yet implemented!" ); 
}

// compile the mocs and Dialog
#include "DlgMacroExecute.cpp"
#include "moc_DlgMacroExecute.cpp"
#include "moc_DlgMacroExecuteImp.cpp"