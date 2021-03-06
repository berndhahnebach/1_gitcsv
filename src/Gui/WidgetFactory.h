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


#ifndef __FC_WIDGET_FACTORY_H__
#define __FC_WIDGET_FACTORY_H__

#include <qdialog.h>
#include <vector>

# include <qwidgetfactory.h>

#include <Base/Factory.h>
#include <Base/PyExportImp.h>
#include "DlgPreferencesImp.h"
#include "DlgCustomizeImp.h"

class QPushButton;
class QGridLayout;

namespace Gui {

/** 
 * The widget factory provides methods for the dynamic creation of widgets.
 * To create these widgets once they must be registered to the factory.
 * To register them use WidgetProducer or any subclasses; to register a
 * preference page use PrefPageProducer instead.
 * \author Werner Mayer
 */
class GuiExport WidgetFactoryInst : public Base::Factory
{
public:
  static WidgetFactoryInst& instance();
  static void destruct ();

  QWidget* createWidget (const char* sName, QWidget* parent=0) const;
  QWidget* createPrefWidget(const char* sName, QWidget* parent, const char* sPref);

private:
  static WidgetFactoryInst* _pcSingleton;

  WidgetFactoryInst(){}
  ~WidgetFactoryInst(){}
};

inline GuiExport WidgetFactoryInst& WidgetFactory()
{
  return WidgetFactoryInst::instance();
}

// --------------------------------------------------------------------

/**
 * The WidgetProducer class is a value-based template class that provides 
 * the ability to create widgets dynamically. 
 * \author Werner Mayer
 */
template <class CLASS>
class WidgetProducer: public Base::AbstractProducer
{
public:
  /** 
   * Register a special type of widget to the WidgetFactoryInst.
   */
  WidgetProducer ()
  {
    WidgetFactoryInst::instance().AddProducer(typeid(CLASS).name(), this);
  }

  virtual ~WidgetProducer (){}

  /** 
   * Creates an instance of the specified widget.
   */
  virtual void* Produce () const
  {
    return (void*)(new CLASS);
  }
};

// --------------------------------------------------------------------

/**
 * The PrefPageProducer class is a value-based template class that provides 
 * the ability to create preference pages dynamically. 
 * \author Werner Mayer
 */
template <class CLASS>
class PrefPageProducer: public Base::AbstractProducer
{
public:
  /** 
   * Register a special type of preference page to the WidgetFactoryInst.
   */
  PrefPageProducer (const char* group)
  {
    WidgetFactoryInst::instance().AddProducer( typeid(CLASS).name(), this );
    Gui::Dialog::DlgPreferencesImp::addPage( typeid(CLASS).name(), group );
  }

  virtual ~PrefPageProducer (){}

  /** 
   * Creates an instance of the specified widget.
   */
  virtual void* Produce () const
  {
    return (void*)(new CLASS);
  }
};

// --------------------------------------------------------------------

/**
 * The CustomPageProducer class is a value-based template class that provides 
 * the ability to create custom pages dynamically. 
 * \author Werner Mayer
 */
template <class CLASS>
class CustomPageProducer: public Base::AbstractProducer
{
public:
  /** 
   * Register a special type of customize page to the WidgetFactoryInst.
   */
  CustomPageProducer ()
  {
    WidgetFactoryInst::instance().AddProducer( typeid(CLASS).name(), this );
    Gui::Dialog::DlgCustomizeImp::addPage( typeid(CLASS).name() );
  }

  virtual ~CustomPageProducer (){}

  /** 
   * Creates an instance of the specified widget.
   */
  virtual void* Produce () const
  {
    return (void*)(new CLASS);
  }
};

// --------------------------------------------------------------------

/**
 * The widget factory supplier class registers all kinds of
 * preference pages and widgets..
 * \author Werner Mayer
 */
class WidgetFactorySupplier
{
private:
  // Singleton
  WidgetFactorySupplier();
  static WidgetFactorySupplier *_pcSingleton;

public:
  static WidgetFactorySupplier &instance();
  friend WidgetFactorySupplier &GetWidgetFactorySupplier();
};

inline WidgetFactorySupplier &GetWidgetFactorySupplier()
{
  return WidgetFactorySupplier::instance();
}

// ----------------------------------------------------

/**
 * The ContainerDialog class acts as a container to embed any kinds of widgets that
 * do not inherit from QDialog. This class also provides an "Ok" and a "Cancel" button.
 * At most this class is used to embed widgets which are created from .ui files.
 * \author Werner Mayer
 */
class ContainerDialog : public QDialog
{
  Q_OBJECT

public:
  ContainerDialog( QWidget* templChild );
  ~ContainerDialog();

  QPushButton* buttonOk; /**< The Ok button. */
  QPushButton* buttonCancel; /**< The cancel button. */

private:
  QGridLayout* MyDialogLayout;
};

// ----------------------------------------------------

/**
 * The PyResource class provides an interface to create widgets or to load .ui files from Python.
 * With
 * \code
 * d = Gui.CreateDialog("test.ui")
 * \endcode
 *
 * you can create a PyResource object containing the widget. If a relative file name 
 * is given PyResource looks first in the current working directory and afterwards in 
 * the home path where FreeCAD resides.
 * 
 * If the appropriate .ui file cannot be found or creation fails an exception is thrown.
 * In case the widget in the .ui file does not inherit from QDialog it is embedded in a
 * \ref ContainerDialog object.
 * To show the widget you can call
 * \code
 * d.Show()
 * \endcode
 *
 * Furthermore it is possible to get or set values from any widgets inside
 * the parent widget or to connect a Python callback function with a widget.
 * \remark The callback function must have exactly one parameter. This parameter
 * points to the dialog you have just created.
 * \code
 * # define a callback function with one argument
 * def TestCall(obj):
 *      # sets the value from lineedit if "Button_Name" was pressed
 *      obj.SetValue("lineedit", "text", "Show this text here!")
 *      print "Button clicked"
 *
 * d = Gui.CreateDialog("test.ui")
 * d.Connect("Button_Name", "clicked()", TestCall)
 * d.Show()
 * \endcode
 *
 * If the button with the name "Button_Name" is clicked the message "Button clicked" is
 * printed.
 * For example if you have a QLineEdit inside your widget you can set the text with
 * \code
 * # sets "Show this text here!" to the text property
 * d.SetValue("lineedit", "text", "Show this text here!")
 * d.Show()
 * \endcode
 *
 * or retrieve the entered text with
 * \code
 * f = d.GetValue("lineedit", "text")
 * print f
 * \endcode
 *
 * \author Werner Mayer
 */
class PyResource : public Base::PyObjectBase
{
  // always start with Py_Header
  Py_Header;

protected:
  ~PyResource();

public:
  PyResource(PyTypeObject *T = &Type);

  void load(const char* name);
  bool connect(const char* sender, const char* signal, PyObject* cb);

  /// for construction in Python
  static PyObject *PyMake(PyObject *, PyObject *);

  //---------------------------------------------------------------------
  // python exports goes here +++++++++++++++++++++++++++++++++++++++++++
  //---------------------------------------------------------------------
  PyObject *_getattr(char *attr);             // __getattr__ function
  // getter setter
  int _setattr(char *attr, PyObject *value);  // __setattr__ function

  // methods
  PYFUNCDEF_D(PyResource, value);
  PYFUNCDEF_D(PyResource, setValue);
  PYFUNCDEF_D(PyResource, show);
  PYFUNCDEF_D(PyResource, connect);

private:
  std::vector<class SignalConnect*> mySingals;
  QDialog* myDlg;
};

/**
 * The SignalConnect class provides the abitlity to make a connection
 * between the callback function of a Python object and the slot onExecute().
 * This mechanism is used in the Python/Qt framework.
 * \author Werner Mayer
 */
class SignalConnect : public QObject
{
  Q_OBJECT

public:
  SignalConnect( Base::PyObjectBase* res, PyObject* cb, QObject* sender);
  ~SignalConnect();

public slots:
  void onExecute();

private:
  PyObject* myResource;
  PyObject* myCallback;
  QObject*  mySender;
};

/**
 * The QtWidgetFactory class provides the abitlity to use the widget factory 
 * framework of FreeCAD within the framework provided by Qt. This class extends
 * the QWidgetFactory by the creation of FreeCAD specific widgets.
 * \author Werner Mayer
 */
class QtWidgetFactory : public QWidgetFactory
{
public:
  QtWidgetFactory() : QWidgetFactory(){}
  ~QtWidgetFactory(){}

  /**
   * Creates a widget of the type \a className withe the parent \a parent.
   * Fore more details see the documentation to QWidgetFactory.
   */
  QWidget* createWidget( const QString & className, QWidget * parent, const char * name ) const
  {
    QString cname = QString("class %1").arg(className);
    return WidgetFactory().createWidget( cname.latin1(), parent );
  }
};

} // namespace Gui

#endif // __FC_WIDGET_FACTORY_H__
