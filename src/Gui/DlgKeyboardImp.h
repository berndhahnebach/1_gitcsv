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


#ifndef DLGKEYBOARD_IMP_H
#define DLGKEYBOARD_IMP_H

#include "DlgKeyboard.h"

#ifndef _PreComp_
# include <qmap.h>
#endif


namespace Gui {
class Command;
namespace Dialog {

/** Shows an overview of all available commands of all groups and modules.
 * You can customize your workbenches just by drag&dropping any commands
 * onto the toolbars or commandbars. But you cannot modify any standard toolbars or
 * commandbars such as "File operations, Standard views, ...". It is only poosible to
 * customize your own toolbars or commandbars.
 * \author Werner Mayer
 */
class DlgCustomKeyboardImp : public DlgCustomKeyboardBase
{ 
public:
  DlgCustomKeyboardImp( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~DlgCustomKeyboardImp();

protected slots:
  void onDescription( const QString& );
  void onGroupSelected( const QString& );
  void onAssign();
  void onReset();
  void onResetAll();
  void onShortcutPressed(const QString&);

protected:
  void showEvent( QShowEvent* );
};

} // namespace Dialog
} // namespace Gui

#endif // DLGKEYBOARD_IMP_H 
