#include "PreCompiled.h"
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

#ifndef _PreComp_
#	include <qapplication.h>
#	include <qsize.h>
#	include <qpainter.h>
#	include <qpixmap.h>
#	include <qcolor.h>
#	include <qfont.h>
#	include <qcursor.h>
#	include <qpoint.h>
#	include <qstyle.h>
#endif

#include "kexipropertyeditoritem.h"

// Helper class for QSizePolicy Editor

class spHelper
{
	public:
	spHelper() {;}
	~spHelper() {;}

	static QStringList	list();
	static QString		valueToKey(int key);
	static QSizePolicy::SizeType		keyToValue(const QString &key);
};

QString
spHelper::valueToKey(int key)
{
	switch(key)
	{
		case QSizePolicy::Fixed: return QString("Fixed");
		case QSizePolicy::Minimum: return QString("Minimum");
		case QSizePolicy::Maximum: return QString("Maximum");
		case QSizePolicy::Preferred: return QString("Preferred");
		case QSizePolicy::MinimumExpanding: return QString("MinimumExpanding");
		case QSizePolicy::Expanding: return QString("Expanding");
		case QSizePolicy::Ignored: return QString("Ignored");
		default: return QString();
	}
}

QStringList
spHelper::list()
{
	QStringList list;
	list << "Fixed" << "Maximum" << "Minimum" << "Preferred" << "Expanding"
		<< "MinimumExpanding" << "Ignored";
	return list;
}

QSizePolicy::SizeType
spHelper::keyToValue(const QString &key)
{
	if(key == "Fixed") return QSizePolicy::Fixed;
	if(key == "Minimum") return QSizePolicy::Minimum;
	if(key == "Maximum") return QSizePolicy::Maximum;
	if(key == "Preferred") return QSizePolicy::Preferred;
	if(key == "MinimumExpanding") return QSizePolicy::MinimumExpanding;
	if(key == "Expanding") return QSizePolicy::Expanding;
	if(key == "Ignored") return QSizePolicy::Ignored;

	return QSizePolicy::Expanding;
}

// ======== KexiPropertyEditorItem ============================

KexiPropertyEditorItem::KexiPropertyEditorItem(KexiPropertyEditorItem *par, KexiProperty *property,
	KexiPropertyEditorItem *after)
 : CListViewItem(par, after, property->desc().isEmpty() ? property->name() : property->desc()
	, property->valueText())
{
	m_order = parent()->childCount();
//	m_value = property->value();
//	m_oldvalue = m_value;
	m_property=property;
//	m_childprop = 0;

	//add children
	KexiProperty::List *children = m_property->children();
	if (children) {
		m_children = new Dict();
		KexiProperty::ListIterator it(*children);
		KexiPropertyEditorItem *item = 0;
		for (;it.current();++it) {
			item = new KexiPropertyEditorItem(this, it.current(), item);
			m_children->insert( it.current()->name(), item );
		}
	}
	else {
		m_children = 0;
	}

//	m_childprop->setAutoDelete(true);
	updateValue();
}

KexiPropertyEditorItem::KexiPropertyEditorItem(QListView *par, const QString &text)
 : CListViewItem(par, text, "")
{
	m_order = listView()->childCount();
	//	m_value = "";
	m_property= new KexiProperty();
//	m_oldvalue=m_value;
//	m_childprop = 0;
	m_children = 0;
	setSelectable(false);
	setOpen(true);
}

/*KexiPropertyEditorItem* KexiPropertyEditorItem::createChildItem(const QString& name)
{
	KexiProperty *childProp = m_property->child(name);
	if (!childProp)
		return 0;
	return new KexiPropertyEditorItem(this, childProp);
}*/

/*void
KexiPropertyEditorItem::setValue(QVariant value)
{
	setText(1, format(value));
	m_value = value;
}
*/

void
KexiPropertyEditorItem::paintCell(QPainter *p, const QColorGroup & cg, int column, int width, int align)
{
	int margin = listView()->itemMargin();

	if(column == 1)
	{
		switch(m_property->type())
		{
			case QVariant::Pixmap:
			{
				p->fillRect(0,0,width,height(),QBrush(backgroundColor()));
				p->drawPixmap(margin, margin, m_property->value().toPixmap());
				break;
			}
			case QVariant::Color:
			{
				p->fillRect(0,0,width,height(), QBrush(backgroundColor()));
				QColor ncolor = m_property->value().toColor();
				p->setBrush(ncolor);
				p->drawRect(margin, margin, width - 2*margin, height() - 2*margin);
				QColorGroup nGroup(cg);
				break;
			}
			case QVariant::Bool:
			{
				p->fillRect(0,0,width,height(), QBrush(backgroundColor()));
				if(m_property->value().toBool())
				{
					p->drawText(QRect(margin,0,width,height()-1), Qt::AlignVCenter, QString("True"));
				}
				else
				{
					p->drawText(QRect(margin,0,width,height()-1), Qt::AlignVCenter, QString("False"));
				}
				break;
			}

			default:
			{
				if(depth()==0)
					return;
				QListViewItem::paintCell(p, cg, column, width, align);
//				p->fillRect(0,0,width,height(), QBrush(backgroundColor()));
				break;
			}
		}
	}
	else
	{
		if(depth()==0)
			return;

		p->fillRect(0,0,width, height(), QBrush(backgroundColor()));

		if(isSelected())
		{
			QRect r (0,0,width, height());
	    p->save();
		  QApplication::style().drawPrimitive(QStyle::PE_Panel, p, r, cg, QStyle::Style_Sunken, QStyleOption(1,1) );
	    p->restore();
//			p->fillRect(0,0,width, height(), QBrush(cg.highlight()));
//			p->setPen(cg.highlightedText());
		}

		QFont f = listView()->font();
		p->save();
//		if(modified())
		if (m_property->changed())
			f.setBold(true);
		p->setFont(f);
		p->drawText(QRect(margin,0,width, height()-1), Qt::AlignVCenter, text(0));
		p->restore();

		p->setPen( QColor(200,200,200) ); //like in table view
		p->drawLine(width-1, 0, width-1, height()-1);
	}

	p->setPen( QColor(200,200,200) ); //like in t.v.
	p->drawLine(-50, height()-1, width, height()-1 );
}

void
KexiPropertyEditorItem::paintBranches(QPainter *p, const QColorGroup &cg, int w, int y, int h)
{
	p->eraseRect(0,0,w,h);
	CListViewItem *item = (CListViewItem*)firstChild();
	if(!item)
		return;

	p->save();
	p->translate(0,y);
	while(item)
	{
		p->fillRect(0,0,w, item->height(), QBrush(item->backgroundColor()));
		p->fillRect(-50,0,50, item->height(), QBrush(item->backgroundColor()));
		p->save();
		p->setPen( QColor(200,200,200) ); //like in t.v.
		p->drawLine(-50, item->height()-1, w, item->height()-1 );
		p->restore();

		if(item->isSelected())
		{
			p->fillRect(0,0,w, item->height(), QBrush(cg.highlight()));
			p->fillRect(-50,0,50, item->height(), QBrush(cg.highlight()));
		}

		if(item->firstChild())
		{
			p->drawRect(2, item->height()/2 -4, 9, 9);
			p->drawLine(4, item->height()/2, 8, item->height()/2);
			if(!item->isOpen())
				p->drawLine(6, item->height()/2 - 2, 6, item->height()/2 +2);
		}

		p->translate(0, item->totalHeight());
		item = (CListViewItem*)item->nextSibling();
	}
	p->restore();
}

void
KexiPropertyEditorItem::setup()
{
	QListViewItem::setup();
	if(depth()==0)
		setHeight(0);
}

#if 0
QVariant
KexiPropertyEditorItem::getComposedValue()
{
	switch(m_property->type())
	{
		case QVariant::Size:
		{
			QSize s;
			QVariant v;
			v = (*m_children)["width"]->property()->value();
			s.setWidth(v.toInt());
			(*m_children)["width"]->property()->setValue(v.toInt());

			v = (*m_children)["height"]->property()->value();
			s.setHeight(v.toInt());
			(*m_children)["height"]->property()->setValue(v.toInt());

			setValue(s);
			return s;
		}
		case QVariant::Point:
		{
			QPoint p;
			QVariant v;
			v = (*m_children)["x"]->value();
			p.setX(v.toInt());
			(*m_children)["x"]->property()->setValue(v.toInt());

			v = (*m_children)["y"]->value();
			p.setY(v.toInt());
			(*m_children)["y"]->property()->setValue(v.toInt());

			setValue(p);
			return p;
		}
		case QVariant::Rect:
		{
			QRect r;
			QVariant v;
			v = (*m_children)["x"]->value();
			r.setX(v.toInt());
			(*m_children)["x"]->property()->setValue(v.toInt());

			v = (*m_children)["y"]->value();
			r.setY(v.toInt());
			(*m_children)["y"]->property()->setValue(v.toInt());

			v = (*m_children)["width"]->value();
			r.setWidth(v.toInt());
			(*m_children)["width"]->property()->setValue(v.toInt());

			v = (*m_children)["height"]->value();
			r.setHeight(v.toInt());
			(*m_children)["height"]->property()->setValue(v.toInt());

			setValue(r);
			return r;
		}
		case QVariant::SizePolicy:
		{
			QSizePolicy p;
			QVariant v;
			v = (*m_children)["hSize"]->value();
			p.setHorData(spHelper::keyToValue(v.toString()));
			(*m_children)["hSize"]->property()->setValue(v.toString());

			v = (*m_children)["vSize"]->value();
			p.setVerData(spHelper::keyToValue(v.toString()));
			(*m_children)["vSize"]->property()->setValue(v.toString());

			v = (*m_children)["hStretch"]->value();
			p.setHorStretch(v.toInt());
			(*m_children)["hStretch"]->property()->setValue(v.toInt());

			v = (*m_children)["vStretch"]->value();
			p.setVerStretch(v.toInt());
			(*m_children)["vStretch"]->property()->setValue(v.toInt());

			setValue(p);
			return p;
		}

		default:
		{
			return 0;
		}

	}
}
#endif

/* let's move this to KexiProperty?
void
KexiPropertyEditorItem::updateChildValue()
{
	if(m_property->type() == QVariant::Rect)
	{
		QRect r = m_value.toRect();
		(*m_children)["x"]->property()->setValue(r.x());
		(*m_children)["x"]->setValue(r.x());
		(*m_children)["y"]->property()->setValue(r.y());
		(*m_children)["y"]->setValue(r.y());
		(*m_children)["width"]->property()->setValue(r.width());
		(*m_children)["width"]->setValue(r.width());
		(*m_children)["height"]->property()->setValue(r.height());
		(*m_children)["height"]->setValue(r.height());
	}
}*/

KexiPropertyEditorItem::~KexiPropertyEditorItem()
{
	if(depth() == 0)
		delete m_property;
	delete m_children;

/*	switch(m_property->type())
	{
		case QVariant::Point:
		case QVariant::Rect:
		case QVariant::Size:
		case QVariant::SizePolicy:
		{
//			delete m_childprop;
//			delete m_children;
		}
		default:
		{
			return;
		}
	}*/
}

void KexiPropertyEditorItem::updateValue(bool alsoParent)
{
	setText( 1, m_property->valueText() );
	if (alsoParent && parent())
		static_cast<KexiPropertyEditorItem*>(parent())->updateValue();
}

void
KexiPropertyEditorItem::updateChildrenValue()
{
	//update children value
	KexiPropertyEditorItem *it = static_cast<KexiPropertyEditorItem*>(firstChild());
	while (it) {
		it->updateValue(false);
		it = static_cast<KexiPropertyEditorItem*>(it->nextSibling());
	}
}

/*QString KexiPropertyEditorItem::key( int column, bool ascending ) const
{
	if (!ascending)
		return QListViewItem::key(column, ascending);

	return QString().sprintf("%5.5d", m_order);
}*/

int KexiPropertyEditorItem::compare( QListViewItem *i, int col, bool ascending ) const
{
	if (!ascending)
		return -QListViewItem::key( col, ascending ).localeAwareCompare( i->key( col, ascending ) );

	return m_order - static_cast<KexiPropertyEditorItem*>(i)->m_order;
//		QString().sprintf("%5.5d", m_order).localeAwareCompare( QString().sprintf("%5.5d", i->m_order) );
}