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


#ifndef PROPERTYEDITORDATE_H
#define PROPERTYEDITORDATE_H

#include "propertyeditoritem.h"


namespace Gui {
namespace PropertyEditor {

/**
 * Change a time property.
 * \author Werner Mayer
 */
class GuiExport TimeEditorItem: public EditableItem
{
  TYPESYSTEM_HEADER();

public:
  TimeEditorItem( QListView* lv, const QString& text, const QVariant& value );

protected:
  QWidget* createEditor( int column, QWidget* parent );
  virtual void stopEdit( int column );
  virtual void setDefaultEditorValue( QWidget* editor );
  virtual QVariant currentEditorValue( QWidget* editor ) const;
  virtual QVariant convertFromProperty(const std::vector<App::Property*>&);
  virtual void convertToProperty(const QVariant&);

private:
  TimeEditorItem();
};

/**
 * Change a date property.
 * \author Werner Mayer
 */
class GuiExport DateEditorItem: public EditableItem
{
  TYPESYSTEM_HEADER();

public:
  DateEditorItem( QListView* lv, const QString& text, const QVariant& value );

protected:
  QWidget* createEditor( int column, QWidget* parent );
  virtual void stopEdit( int column );
  virtual void setDefaultEditorValue( QWidget* editor );
  virtual QVariant currentEditorValue( QWidget* editor ) const;
  virtual QVariant convertFromProperty(const std::vector<App::Property*>&);
  virtual void convertToProperty(const QVariant&);

private:
  DateEditorItem();
};

/**
 * Change a datetime property.
 * \author Werner Mayer
 */
class GuiExport DateTimeEditorItem: public EditableItem
{
  TYPESYSTEM_HEADER();

public:
  DateTimeEditorItem( QListView* lv, const QString& text, const QVariant& value );

protected:
  QWidget* createEditor( int column, QWidget* parent );
  virtual void stopEdit( int column );
  virtual void setDefaultEditorValue( QWidget* editor );
  virtual QVariant currentEditorValue( QWidget* editor ) const;
  virtual QVariant convertFromProperty(const std::vector<App::Property*>&);
  virtual void convertToProperty(const QVariant&);

private:
  DateTimeEditorItem();
};

} //namespace PropertyEditor
} //namespace Gui

#endif

