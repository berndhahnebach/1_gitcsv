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
    + use QPushButton instead of KPushButton
    + PropertyEditor inherits from QListView instead of KListView
    + added method PropertyEditor::resizeColumns()

   Modifications for FreeCAD from 10-19-2004
    + use namespace Gui::Kexi instead of prefix
*/

#ifndef KEXIPROPERTYEDITOR_H
#define KEXIPROPERTYEDITOR_H

#ifndef _PreComp_
# include <qvariant.h>
# include <qguardedptr.h>
# include <qdict.h>
# include <qlistview.h>
#endif

#include "kexipropertyeditoritem.h"

class QPushButton;


namespace Gui {
namespace Kexi {

class Property;
class PropertyBuffer;
class PropertySubEditor;

/** This class is a KListView which shows an item for each property in its PropertyBuffer.
    When an item gets the focus, an editor (eg. a line edit, a spin box, etc.) is shown, which allows to modify property value.
    Pressing Enter key allow to validate the input (ie save it in the buffer), whereas Esc key undo previous input.
    Properties can be reset using "Revert to defaults" push button next to editor.
    Modified items names are written in bold.\n
    \n
    Example of usage of PropertyEditor :
    \code
    // Create a Property Buffer
    m_buffer = new PropertyBuffer(this);
    // Add properties into the buffer
    m_buffer->add( new Property("Name", "Name") );
    m_buffer->add( new Property("Int", 43) );
    
    // for a string list property
    QStringList list;
    list.append("MyItem");
    list.append("OtherItem");
    list.append("Item");
    m_buffer->add( new Property("list","Item" , list) );
    [...]
    
    PropertyEditor *edit = new PropertyEditor(this,false);
    edit->setBuffer(m_buffer);
    \endcode
    PropertyEditor receives propery value changes from the assigned buffer,
    and automatically updates visually, if needed.
*/
//! A list view to edit any type of properties
class GuiExport PropertyEditor : public QListView
{
  Q_OBJECT

public:
  /*! Creates an empty PropertyEditor with \a parent as parent widget. If \a autoSync == true,
      properties values are automatically synced as soon as editor contents change (eg the user types text, etc.)
      and the values are written in the buffer. Otherwise, buffer is updated only when selected item changes 
      or user presses Enter key. Each property can overwrite this if its autoSync() == 0 or 1.
  */
  PropertyEditor(QWidget *parent=0, bool autoSync=true, const char *name=0);
  ~PropertyEditor();

  /*! Reset the list, ie clears all items in the list.
     if \a editorOnly is true, then only the current editor will be cleared, not the whole list.
  */
  void  reset(bool editorOnly = false);
  /*! Sets \a b as Property Editor's buffer. The list will be automatically filled with a list item for each Property
     in the buffer.
  */
  void  setBuffer(PropertyBuffer *b);
  virtual QSize sizeHint() const;

  /*! \return editor item named with \a name or null if such item not found. */
  PropertyEditorItem* item(const QString& name) const { return m_items[name]; }

signals:
  /*! This signal is emitted when a property value has changed, ie when the user presses Enter or when another item
      gets the focus. \a propname is the name of the property and \a value is the new value of this property.
  */
  void  valueChanged(const QString &propname, QVariant value);

public slots:
  /*! This slot resets the value of an item, using Property::oldValue().
     It is called when pressing the "Revert to defaults" button
  */
  void    resetItem();
  /*! This slot updates the positions of current editor and revert button.
     It is called when double-clicking list's header.
  */
  void    moveEditor();
  /*! Fills the list with an item for each property in the buffer.
     You shouldn't need to call this, as it is automatically called in setBuffer().
  */
  void  fill();

protected slots:
  /*! This slot is called when the user presses Enter key and when the selected item changes.
      It takes care of saving editor value into buffer.
  */
  void  slotEditorAccept(PropertySubEditor *editor);
  /*! This slot is called when the user press Esc key.
      It undoes last input, setting item value as it was on editor's creation.
      If autoSync == true, it sets item value to Property::oldValue().
  */
  void  slotEditorReject(PropertySubEditor *editor);
  /*! This slot is called every time the editor contents change.
     If AutoSync is true, the buffer is updated. Otherwise, does nothing.
     This slot also takes care of syncing composed items (eg. QRect item with x, y, width and height chilren).
  */
  void  slotValueChanged(PropertySubEditor *editor);

  /*! This slot updates editor and revert buttons position and size when the columns are resized. */
  void  slotColumnSizeChanged(int section, int oldS, int newS);
  void  slotColumnSizeChanged(int section);

  /*! This slot is called when the user clicks the list view. It takes care of deleting current editor and
     creating a new editor for the newly selected item.
  */
  void  slotClicked(QListViewItem *item);

  void  slotExpanded(QListViewItem *item);
  void  slotCollapsed(QListViewItem *item);

  /*! Receives signals on \a prop property change from buffer \a buf. */
  void  slotPropertyChanged(PropertyBuffer &buf,Property &prop);

  /*! Called when current buffer is about to be destroyed. */
  void  slotBufferDestroying();
protected:
  /*! Creates an editor for the list item \a i in the rect \a geometry, and displays revert button
      if property is modified (ie PropertyEditorItem::modified() == true).
      The editor type depends on Property::type() of the item's property.
  */
  void  createEditor(PropertyEditorItem *i);//, const QRect &geometry);
  /*! Reimplemented from KListView to update editor and revert button position. */
  void  resizeEvent(QResizeEvent *ev);
  void  resizeColumns();

  void showDefaultsButton( bool show );

protected slots:
  void slotPropertyReset(PropertyBuffer &buf,Property &prop);

private:
  QGuardedPtr<PropertySubEditor> m_currentEditor;
  PropertyEditorItem *m_editItem;
  PropertyEditorItem *m_topItem; //The top item is used to control the drawing of every branches.
  QGuardedPtr<PropertyBuffer> m_buffer;
  QPushButton *m_defaults; // "Revert to defaults" button
  PropertyEditorItem::Dict m_items;
  bool m_sync : 1;
  bool slotValueChanged_enabled : 1;
};

} // namespace Kexi
} // namespace Gui

#endif
