/* This file is part of the KDE project
   Copyright (C) 2003 Lucijan Busch <lucijan@gmx.at>
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

#ifndef PROPERTYBUFFER_H
#define PROPERTYBUFFER_H

#include <qobject.h>
#include <qdict.h>
#include <qptrvector.h>

#include "kexiproperty.h"

/** This class is a QDict<KexiProperty> which holds properties to be shown in
    Property Editor. Properties are indexed by their names, case insensitively.
    \sa KexiPropertyEditor for help on how to use KexiPropertyBuffer.
    \sa KexiProperty to see how to create properties.
**/
class GuiExport KexiPropertyBuffer : public QObject, public KexiProperty::Dict
{
	Q_OBJECT

	public:
		typedef QPtrVector<KexiPropertyBuffer> Vector;

		/*! Creates an empty KexiPropertyBuffer, i.e. a QMap<QString, KexiProperty>.
		 \a type_name means a name of this property buffer type. See typeName() description
		 for more information on type names.
		*/
		KexiPropertyBuffer(QObject *parent, const QString &type_name);

		virtual ~KexiPropertyBuffer();

		/*! \return an ordered list of properties */
		KexiProperty::List* list() { return &m_list; }

		/*! Add \a property to buffer with property->name() as key in QMap.
		 Same as insert(\a property->name(), \a property).
		 The \a property object will be owned by this buffer.
		*/
		void add(KexiProperty *property);

		/* Change the value of property whose key is \a property to \a value.
		  By default, it only calls KexiProperty::setValue().
		*/
		virtual void changeProperty(const QCString &property, const QVariant &value);

		/* A name of this property buffer type, that is usable when
		 we want to know if two property buffer objects have the same type.
		 For example, \a type_name may be "KexiDB::Field::Integer" for property buffer
		 for given selected field of type integer, in "Alter Table Dialog".
		 This avoids e.g. reloading of all KexiPropertyEditor's contents.
		 Also, this allows to know if two property-buffer objects are compatible
		 by their property sets.
		 For comparing purposes, type names are case insensitive.
		*/
		QString typeName() const { return m_typeName; }

		/*! Removes all properties from the buffer and destroys them. */
		virtual void clear();

		void debug();

	signals:
		/*! This signal is emitted when \a property has changed
		   (i.e. when changeProperty() was called).
		*/
		void propertyChanged(KexiPropertyBuffer &buf, KexiProperty &property);//const QString &property, const QVariant &value);

		/*! Parameterless version of the above method. */
		void propertyChanged();
		
		void propertyReset(KexiPropertyBuffer &buf, KexiProperty &property);

		void destroying();

	protected:
		QString m_typeName;
		KexiProperty::List m_list;

	friend class KexiProperty;
};

#endif