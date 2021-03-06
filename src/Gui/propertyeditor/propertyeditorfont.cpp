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
# include <qcolordialog.h>
# include <qfontdialog.h>
# include <qpainter.h>
#endif

#include <App/PropertyStandard.h>

#include "propertyeditorfont.h"
#include "Widgets.h"

using namespace Gui::PropertyEditor;

TYPESYSTEM_SOURCE(Gui::PropertyEditor::FontEditorItem, Gui::PropertyEditor::EditableItem);

FontEditorItem::FontEditorItem()
{
}

FontEditorItem::FontEditorItem( QListView* lv, const QString& text, const QVariant& value )
  :EditableItem( lv, value )
{
  setText( 0, text );
  setText( 1, value.toFont().family() );
}

QWidget* FontEditorItem::createEditor( int column, QWidget* parent )
{
  if ( column == 0 )
    return 0;

  QPushButton* editor = new QPushButton( parent, "FontEditorItem::edit" );
  editor->setText( overrideValue().toFont().family() );
  connect(editor, SIGNAL(clicked()), this, SLOT(onChangeFont()));
  return editor;
}

void FontEditorItem::stopEdit( int column )
{
  setText( column, overrideValue().toFont().family() );
}

void FontEditorItem::setDefaultEditorValue( QWidget* editor )
{
  QPushButton* btn = dynamic_cast<QPushButton*>(editor);
  btn->setText( value().toFont().family() );
}

QVariant FontEditorItem::currentEditorValue( QWidget* editor ) const
{
  QPushButton* btn = dynamic_cast<QPushButton*>(editor);

  QVariant var;
  var.asFont().setFamily( btn->text() );
  return var;
}

void FontEditorItem::onChangeFont()
{
  QPushButton* btn = (QPushButton*)sender();

  if ( btn )
  {
    bool ok;
    QFont font( btn->text() ); 
    font = QFontDialog::getFont( &ok, font, listView() );

    if ( ok ) 
    {
      onValueChanged();
      btn->setText( font.family() );
    }
  }
}

QVariant FontEditorItem::convertFromProperty(const std::vector<App::Property*>& prop)
{
  QVariant var;
  var.asFont().setFamily( "" );
  return var;
}

void FontEditorItem::convertToProperty(const QVariant&)
{
}

// ======================================================================

TYPESYSTEM_SOURCE(Gui::PropertyEditor::ColorEditorItem, Gui::PropertyEditor::EditableItem);

ColorEditorItem::ColorEditorItem()
{
}

ColorEditorItem::ColorEditorItem( QListView* lv, const QString& text, const QVariant& value )
    :EditableItem( lv, value )
{
  setText( 0, text );
  _color = value.toColor();
}

QWidget* ColorEditorItem::createEditor( int column, QWidget* parent )
{
  if ( column == 0 )
    return 0;

  Gui::ColorButton* editor = new Gui::ColorButton( parent, "ColorEditorItem::edit" );

  editor->setColor( _color );

  connect(editor, SIGNAL( changed() ), this, SLOT( onValueChanged() ));
  return editor;
}

void ColorEditorItem::stopEdit( int column )
{
  _color = overrideValue().toColor();
}

void ColorEditorItem::setDefaultEditorValue( QWidget* editor )
{
  Gui::ColorButton* btn = dynamic_cast<Gui::ColorButton*>(editor);
  btn->setColor( value().toColor() );
}

QVariant ColorEditorItem::currentEditorValue( QWidget* editor ) const
{
  Gui::ColorButton* btn = dynamic_cast<Gui::ColorButton*>(editor);

  QVariant var;
  var.asColor() = btn->color();
  return var;
}

void ColorEditorItem::paintCell(QPainter* p, const QColorGroup& cg, int column, int width, int align)
{
  EditableItem::paintCell( p, cg, column, width, align );
 
  if ( column == 1 )
  {
    p->fillRect( 2, 2, width-5, height()-4, _color );
    p->save();
    p->setPen( QPen( QObject::black, 1 ) );
    p->drawRect( 2, 2, width-5, height()-4 );
    p->restore();
    p->save();
    p->setPen( QPen( cg.dark(), 1 ) );
    p->drawLine( 0, height() - 1, width, height() - 1 );
    p->drawLine( width - 1, 0, width - 1, height() );
    p->restore();
  }
}

QVariant ColorEditorItem::convertFromProperty(const std::vector<App::Property*>& prop)
{
  QVariant value;
  if ( prop.size() > 0 )
  {
    App::PropertyColor* pPropColor = (App::PropertyColor*)prop.front();
    App::Color col = pPropColor->getValue();
    value.asColor() = QColor((int)(255.0f*col.r),(int)(255.0f*col.g),(int)(255.0f*col.b));
    _color = value.toColor();
  }

  return value;
}

void ColorEditorItem::convertToProperty(const QVariant& val)
{
  QColor col = val.toColor();
  for (std::vector<App::Property*>::iterator it = _prop.begin(); it != _prop.end(); ++it)
  {
    App::PropertyColor* pPropColor = (App::PropertyColor*)*it;
    pPropColor->setValue( (float)col.red()/255.0f, (float)col.green()/255.0f, (float)col.blue()/255.0f );
  }
}

#include "moc_propertyeditorfont.cpp"

