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


#ifndef __ACTION_H__
#define __ACTION_H__

#ifndef _PreComp_
# include <qaction.h>
# include <qtooltip.h>
# include <string>
# include <vector>
#endif

class FCCommand;

namespace Gui 
{

/**
 * Allows actions to be added to widgets other than toolbars or popup menus.
 * @author Werner Mayer
 */
class GuiExport Action : public QAction
{
  Q_OBJECT

public:
  Action ( FCCommand* pcCmd,QObject * parent = 0, const char * name = 0, bool toggle = FALSE );
  virtual ~Action();

  virtual bool addTo(QWidget *);
  bool removeFrom ( QWidget * w );
  
  void setEnabled  ( bool ) ;

  FCCommand* GetCommand() { return _pcCmd; }

public slots:
  void onActivated ();
  void onToggled   ( bool ); 

protected slots:
  void onToolButtonToggled ( bool );
  void onClearStatusText   ();
  void onShowStatusText    ( const QString& );
  void onDestroyed();

protected:
  std::vector<QWidget*> widgets;

private:
  FCCommand *_pcCmd;
};

// --------------------------------------------------------------------

/**
 * This allows several actions to be added to a toolbar or menu.
 * For menus a submenu is created for toolbars a combo box.
 * @author Werner Mayer
 */
class GuiExport ActionGroup : public Action
{
  Q_OBJECT

public:
  ActionGroup ( FCCommand* pcCmd,QObject * parent = 0, const char * name = 0, bool toggle = FALSE );
  virtual ~ActionGroup();

  virtual bool addTo(QWidget *);

  void setItems  (const std::vector<std::string>& items);
  void insertItem(const char* item);
  void removeItem(const char* item);
  
  void activate(int);
  void activate(const QString&);
  void clear();

public slots:
  void onActivated (int i);

private slots:
  void onAboutToShow();

protected:
  std::vector<std::string> mItems;
};

// --------------------------------------------------------------------

/**
 * Special action for the undo button
 * @author Werner Mayer
 */
class GuiExport UndoAction : public Action
{
  Q_OBJECT

public:
  UndoAction ( FCCommand* pcCmd,QObject * parent = 0, const char * name = 0, bool toggle = FALSE );
  ~UndoAction(){ delete tipGroup; }
  
  bool addTo(QWidget *);

private:
  QToolTipGroup* tipGroup;
};

// --------------------------------------------------------------------

/**
 * Special action for the redo button
 * @author Werner Mayer
 */
class GuiExport RedoAction : public Action
{
  Q_OBJECT

public:
  RedoAction ( FCCommand* pcCmd,QObject * parent = 0, const char * name = 0, bool toggle = FALSE );
  ~RedoAction(){ delete tipGroup; }
  
  bool addTo(QWidget *);

private:
  QToolTipGroup* tipGroup;
};

} // namespace Gui

#endif // __ACTION_H__
