/* This file is part of the KDE project
   Copyright (C) 2002   Lucijan Busch <lucijan@gmx.at>
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004 Jaroslaw Staniek <js@iidea.pl>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

/* Modifications for FreeCAD from 06-13-2004
    + use FreeCAD's export macro GuiExport instead of KEXICORE_EXPORT
    + added class ListViewItem
    + PropertyEditorItem inherits from ListViewItem instead of KListViewItem
    + use QListView instead of KListView

   Modifications for FreeCAD from 10-19-2004
    + use namespace Gui::Kexi instead of prefix
*/

#ifndef KEXIPROPERTYEDITORITEM_H
#define KEXIPROPERTYEDITORITEM_H

#ifndef _PreComp_
# include <qlistview.h>
# include <qptrlist.h>
# include <qdict.h>
#endif

#include "kexiproperty.h"

namespace Gui {
namespace Kexi {

class GuiExport ListViewItem : public QListViewItem
{
public:
  ListViewItem ( QListView * parent ) : QListViewItem(parent)
  {
  }
  ListViewItem ( QListViewItem * parent ) : QListViewItem ( parent )
  {
  }
  ListViewItem ( QListView * parent, QListViewItem * after ) : QListViewItem ( parent, after )
  {
  }
  ListViewItem ( QListViewItem * parent, QListViewItem * after ) : QListViewItem(parent, after)
  {
  }
  ListViewItem ( QListView * parent, QString label1, QString label2 = QString::null,
                 QString label3 = QString::null, QString label4 = QString::null,
                 QString label5 = QString::null, QString label6 = QString::null,
                 QString label7 = QString::null, QString label8 = QString::null )
      : QListViewItem (parent, label1, label2, label3, label4, label5, label6, label7, label8)
  {
  }
  ListViewItem ( QListViewItem * parent, QListViewItem * after, QString label1,
                 QString label2 = QString::null, QString label3 = QString::null,
                 QString label4 = QString::null, QString label5 = QString::null,
                 QString label6 = QString::null, QString label7 = QString::null,
                 QString label8 = QString::null )
      : QListViewItem (parent, after, label1, label2, label3, label4, label5, label6, label7, label8)
  {
  }

  QColor backgroundColor()
  {
    return QColor(  250, 248, 235 );
  }
};

/** This class is a subclass of KListViewItem which is associated to a property.
    It also takes care of drawing custom contents.
 **/
//! An item in PropertyEditorItem associated to a property
class GuiExport PropertyEditorItem : public ListViewItem
{
public:
  typedef QDict<PropertyEditorItem> Dict;

  /*! Creates a PropertyEditorItem child of \a parent, associated to \a property.
   Within property editor, items are created in PropertyEditor::fill(), 
   every time the buffer is updated.
   It \a property has not desctiption set, its name (i.e. not i18n'ed) is reused.
  */
  PropertyEditorItem(PropertyEditorItem *parent, Property *property,
                         PropertyEditorItem *after);

  //! Creates PropertyEditor Top Item which is necessary for drawing all branches.
  PropertyEditorItem(QListView *parent, const QString &text);

  ~PropertyEditorItem();

  /*! \return item's name (ie property's name). */
  const QCString  name() { return m_property->name(); }
  /*! \return item's QVariant::Type (ie property's QVariant::Type). */
  QVariant::Type  type() { return m_property->type(); }
  //  //! \return item's current value
  //  QVariant  value() { return m_value; }
  //  //! \return item's old value (ie property's old value)
  //  QVariant  oldValue() { return m_oldvalue; }
  /*! \return a pointer to the property associated to this item. */
  Property* property() { return m_property;}

  //  //! Sets item value to \a value.
  //  void  setValue(QVariant value);
  /*! \return the composed item value.
      This function is used by property editor to update composed items, ie items with child items.
      It updates parent item when one of its child changes.
  */ 
  //  QVariant  getComposedValue();
  /*! Updates the value of child items, when the value of the parent was changed
        with setValue(). Used by PropertyEditor.
  */
  void updateChildrenValue();

  /*! \return true if the item has been modified. */
  //  bool  modified() { return !(m_value==m_oldvalue);}

  /*! Updates text on of this item, for current property value.
   If \a alsoParent is true, parent item (if present) is also updated. */
  virtual void updateValue(bool alsoParent = true);

  /*  typedef enum ItemsOrder {
        DefaultOrder = 2;
      };
  */
  //! Reimplemented for sorting
  //  virtual QString key ( int column, bool ascending ) const;
  virtual int compare( QListViewItem *i, int col, bool ascending ) const;


protected:
  /*! Reimplemented from KListViewItem to draw custom contents. Properties names are wriiten in bold if
      modified. Also takes care of drawing borders around the cells as well as pixmaps or colors if necessary.
  */
  virtual void paintCell(QPainter *p, const QColorGroup & cg, int column, int width, int align);

  /*! Reimplemented from KListViewItem to draw custom contents. It takes care of drawing the [+] and [-]
      signs only if the item has children.
  */
  virtual void paintBranches(QPainter *p, const QColorGroup &cg, int w, int y, int h);

  //! Reimplemented from KListViewItem to hide the top item.
  virtual void setup();

  //  /*! Creates a child item based on property child named by \a name.
  //   \return the newly created child item or 0 if there is not such property child. */
  //  PropertyEditorItem* createChildItem(const QString& name);

private:
  //  QVariant  m_value;
  //  QVariant  m_oldvalue;
  Property  *m_property;
  //  QPtrList<Property>  *m_childprop;
  Dict  *m_children;

  uint m_order;
};

} // namespace Kexi
} // namespace Gui

#endif
