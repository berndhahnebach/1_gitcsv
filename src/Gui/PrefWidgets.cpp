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
# include <qlayout.h>
#endif

#include "PrefWidgets.h"
#include "FileDialog.h"
#include "../Base/Console.h"

using Base::Console;
using namespace Gui;

/** Constructs a preference widget with the name \a name. If \a bInstall
 * is true a PrefWidgetHandler is installed. By default \a bInstall is set 
 * to true.
 */
PrefWidget::PrefWidget( bool bInstall )
 : WindowParameter(""), pHandler(0L)
{
  if (bInstall)
  {
    // install a handler for automation stuff
    pHandler = new PrefWidgetHandler(this);
  }
}

/**
 * Destroys the widget and detaches it from its parameter group.
 */
PrefWidget::~PrefWidget()
{
  if (getWindowParameter().IsValid())
    getWindowParameter()->Detach(this);
  delete pHandler;
}

/** Sets the preference name to \a name. */
void PrefWidget::setEntryName( const QCString& name )
{
  m_sPrefName = name;
}

/** Returns the widget's preference name. */
QCString PrefWidget::entryName() const
{
  return m_sPrefName;
}

/** Sets the preference path to \a path. */
void PrefWidget::setParamGrpPath( const QCString& path )
{
#ifdef FC_DEBUG
  if (getWindowParameter().IsValid())
  {
    if ( paramGrpPath() != path )
      Base::Console().Warning("Widget already attached\n");
  }
#endif

  if ( paramGrpPath() != path )
  {
    if ( setGroupName( path ) )
    {
      m_sPrefGrp = path;
      assert(getWindowParameter().IsValid());
      getWindowParameter()->Attach(this);
    }
  }
}

/** Returns the widget's preferences path. */
QCString PrefWidget::paramGrpPath() const
{
  return m_sPrefGrp;
}

/** Returns the last installed handler. */
PrefWidgetHandler* PrefWidget::getHandler()
{
  return pHandler;
}

/** Installs a new handler \a h. */
void PrefWidget::installHandler(PrefWidgetHandler* h)
{
  delete pHandler;
  pHandler = h;
}

/** 
 * This method is called if one ore more values in the parameter settings are changed 
 * where getParamGrp() points to. 
 * Note: This method is called for each parameter inside the parameter group. So
 * you have to filter out the appropriate parameter with the name \a sReason.
 * \a rCaller calls this method.
 */
void PrefWidget::OnChange(FCSubject<const char*> &rCaller, const char * sReason)
{
  if ( QString(sReason) == QString(m_sPrefName) )
    restorePreferences();
}

// --------------------------------------------------------------------

PrefWidgetHandler::PrefWidgetHandler( PrefWidget* p ) : pPref(p)
{
}

/**
 * Saves the current preferences of the managed preference widget and emits 
 * the saved() signal..
 * All preference widget attached to the same parameter group are notified.
 */
void PrefWidgetHandler::onSave()
{
  pPref->savePreferences();
  if ( pPref->getWindowParameter().IsValid() )
    pPref->getWindowParameter()->Notify( pPref->entryName() );

  emit saved();
}

/**
 * Restores the preferences of the managed widget and emits the restored() signal.
 */
void PrefWidgetHandler::onRestore()
{
  pPref->restorePreferences();
  emit restored();
}

// --------------------------------------------------------------------

PrefSpinBox::PrefSpinBox ( QWidget * parent, const char * name )
  : FloatSpinBox(parent, name), PrefWidget(name)
{
}

PrefSpinBox::~PrefSpinBox()
{
}

void PrefSpinBox::restorePreferences()
{
  if ( getWindowParameter().IsNull() )
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  double fVal;
  if (decimals() == 0)
    fVal = (double)getWindowParameter()->GetInt( entryName(), QSpinBox::value() );
  else
    fVal = (double)getWindowParameter()->GetFloat( entryName() , value() );

  setValue(fVal);
}

void PrefSpinBox::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  if (decimals() == 0)
    getWindowParameter()->SetInt( entryName() , (int)value() );
  else
    getWindowParameter()->SetFloat( entryName(), value() );
}

QCString PrefSpinBox::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefSpinBox::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefSpinBox::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefSpinBox::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefLineEdit::PrefLineEdit ( QWidget * parent, const char * name )
  : QLineEdit(parent, name), PrefWidget(name)
{
}

PrefLineEdit::~PrefLineEdit()
{
}

void PrefLineEdit::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  std::string txt = getWindowParameter()->GetASCII( entryName(), text().latin1() );
  setText(txt.c_str());
}

void PrefLineEdit::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  getWindowParameter()->SetASCII( entryName(), text().latin1() );
}

QCString PrefLineEdit::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefLineEdit::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefLineEdit::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefLineEdit::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefFileChooser::PrefFileChooser ( QWidget * parent, const char * name )
  : FileChooser(parent, name), PrefWidget(name)
{
}

PrefFileChooser::~PrefFileChooser()
{
}

void PrefFileChooser::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  std::string txt = getWindowParameter()->GetASCII( entryName(), text().latin1() );
  setText(txt.c_str());
}

void PrefFileChooser::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  getWindowParameter()->SetASCII( entryName(), text().latin1() );
}

QCString PrefFileChooser::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefFileChooser::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefFileChooser::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefFileChooser::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefComboBox::PrefComboBox ( QWidget * parent, const char * name )
  : QComboBox(parent, name), PrefWidget(name)
{
}

PrefComboBox::~PrefComboBox()
{
}

void PrefComboBox::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  FCParameterGrp::handle  hPGrp = getWindowParameter()->GetGroup( entryName() );
  std::vector<std::string> items = hPGrp->GetASCIIs("Item");

  if (items.size() > 0)
    clear();

  for (std::vector<std::string>::const_iterator it = items.begin(); it != items.end(); ++it)
    insertItem(it->c_str());

  int item = hPGrp->GetInt("currentItem", currentItem());
  setCurrentItem(item);
}

void PrefComboBox::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  FCParameterGrp::handle  hPGrp = getWindowParameter()->GetGroup( entryName() );
  hPGrp->Clear();

  int size = int(count());
  for (int i = 0; i < size; i++)
  {
    char szBuf[200];
    sprintf(szBuf, "Item%d", i);
    hPGrp->SetASCII(szBuf, text(i).latin1());
  }

  hPGrp->SetInt("currentItem", currentItem());
}

QCString PrefComboBox::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefComboBox::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefComboBox::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefComboBox::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefListBox::PrefListBox ( QWidget * parent, const char * name, WFlags f )
  : QListBox(parent, name, f), PrefWidget(name)
{
}

PrefListBox::~PrefListBox()
{
}

void PrefListBox::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  FCParameterGrp::handle  hPGrp = getWindowParameter()->GetGroup( entryName() );
  std::vector<std::string> items = hPGrp->GetASCIIs("Item");

  if (items.size() > 0)
    clear();

  for (std::vector<std::string>::const_iterator it = items.begin(); it != items.end(); ++it)
    insertItem(it->c_str());

  int item = hPGrp->GetInt("currentItem", currentItem());
  setCurrentItem(item);
}

void PrefListBox::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  FCParameterGrp::handle  hPGrp = getWindowParameter()->GetGroup( entryName() );
  hPGrp->Clear();

  int size = int(count());
  for (int i = 0; i < size; i++)
  {
    char szBuf[200];
    sprintf(szBuf, "Item%d", i);
    hPGrp->SetASCII(szBuf, text(i).latin1());
  }

  hPGrp->SetInt("currentItem", currentItem());
}

QCString PrefListBox::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefListBox::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefListBox::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefListBox::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefCheckBox::PrefCheckBox ( QWidget * parent, const char * name )
  : QCheckBox(parent, name), PrefWidget(name)
{
}

PrefCheckBox::~PrefCheckBox()
{
}

void PrefCheckBox::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  bool enable = getWindowParameter()->GetBool( entryName(), isChecked() );
  setChecked(enable);
}

void PrefCheckBox::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  getWindowParameter()->SetBool( entryName(), isChecked() );
}

QCString PrefCheckBox::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefCheckBox::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefCheckBox::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefCheckBox::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefRadioButton::PrefRadioButton ( QWidget * parent, const char * name )
  : QRadioButton(parent, name), PrefWidget(name)
{
}

PrefRadioButton::~PrefRadioButton()
{
}

void PrefRadioButton::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  bool enable = getWindowParameter()->GetBool( entryName(), isChecked() );
  setChecked(enable);
}

void PrefRadioButton::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  getWindowParameter()->SetBool( entryName() , isChecked() );
}

QCString PrefRadioButton::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefRadioButton::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefRadioButton::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefRadioButton::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefSlider::PrefSlider ( QWidget * parent, const char * name )
  : QSlider(parent, name), PrefWidget(name)
{
}

PrefSlider::~PrefSlider()
{
}

void PrefSlider::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  FCParameterGrp::handle hPrefs = getWindowParameter()->GetGroup( entryName() );
  int o = hPrefs->GetInt("Orientation", orientation());
  setOrientation(Qt::Orientation(o));
  int min = hPrefs->GetInt("MinValue", minValue());
  int max = hPrefs->GetInt("MaxValue", maxValue());
  int val = hPrefs->GetInt("Value", value());
  setMinValue(min);
  setMaxValue(max);
  setValue(val);
}

void PrefSlider::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  FCParameterGrp::handle hPrefs = getWindowParameter()->GetGroup( entryName() );
  hPrefs->SetInt("Orientation", int(orientation()));
  hPrefs->SetInt("MinValue", minValue());
  hPrefs->SetInt("MaxValue", maxValue());
  hPrefs->SetInt("Value", value());
}

QCString PrefSlider::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefSlider::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefSlider::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefSlider::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

// --------------------------------------------------------------------

PrefColorButton::PrefColorButton ( QWidget * parent, const char * name )
  : ColorButton(parent, name), PrefWidget(name)
{
}

PrefColorButton::~PrefColorButton()
{
}

void PrefColorButton::restorePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot restore!\n");
    return;
  }

  QColor col = color();
  long lcol = (col.blue() << 16) | (col.green() << 8) | col.red();

  lcol = getWindowParameter()->GetInt( entryName(), lcol );
  int b = lcol >> 16;  lcol -= b << 16;
  int g = lcol >> 8;   lcol -= g << 8;
  int r = lcol;

  setColor(QColor(r,g,b));
}

void PrefColorButton::savePreferences()
{
  if (getWindowParameter().IsNull())
  {
    Console().Warning("Cannot save!\n");
    return;
  }

  QColor col = color();
  long lcol = (col.blue() << 16) | (col.green() << 8) | col.red();

  getWindowParameter()->SetInt( entryName(), lcol );
}

QCString PrefColorButton::entryName () const
{
  return PrefWidget::entryName();
}

QCString PrefColorButton::paramGrpPath () const
{
  return PrefWidget::paramGrpPath();
}

void PrefColorButton::setEntryName ( const QCString& name )
{
  PrefWidget::setEntryName(name);
}

void PrefColorButton::setParamGrpPath ( const QCString& name )
{
  PrefWidget::setParamGrpPath(name);
}

#include "moc_PrefWidgets.cpp"
