/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de) 2002     *
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
# include <TDF_ChildIterator.hxx>
# include <TDF_Tool.hxx>
# include <TCollection_AsciiString.hxx>
# include <TDF_ListIteratorOfAttributeList.hxx>
# include <TFunction_Logbook.hxx>
# include <TFunction_DriverTable.hxx>
# include <TFunction_Function.hxx>
# include <Standard_GUID.hxx>
# include <TNaming_Builder.hxx>
# include <TDataStd_Name.hxx>
# include <sstream>
#endif

#include "../Base/PyExportImp.h"
#include "../Base/Console.h"
#include "../Base/Exception.h"
using Base::Console;

#include "Document.h"
#include "Feature.h"
#include "FeatureAttr.h"
#include "Function.h"
#include "Property.h"
#include "PropertyAttr.h"
#include "FeaturePy.h"

using namespace App;




//--------------------------------------------------------------------------
// Type structure
//--------------------------------------------------------------------------

PyTypeObject App::FeaturePy::Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,						/*ob_size*/
	"FeaturePy",				/*tp_name*/
	sizeof(FeaturePy),			/*tp_basicsize*/
	0,						/*tp_itemsize*/
	/* methods */
	PyDestructor,	  		/*tp_dealloc*/
	0,			 			/*tp_print*/
	__getattr, 				/*tp_getattr*/
	__setattr, 				/*tp_setattr*/
	0,						/*tp_compare*/
	__repr,					/*tp_repr*/
	0,						/*tp_as_number*/
	0,						/*tp_as_sequence*/
	0,						/*tp_as_mapping*/
	0,						/*tp_hash*/
	0,						/*tp_call */
};

//--------------------------------------------------------------------------
// Methods structure
//--------------------------------------------------------------------------
PyMethodDef App::FeaturePy::Methods[] = {
//  {"Undo",         (PyCFunction) sPyUndo,         Py_NEWARGS},
	PYMETHODEDEF(AddFeature)

	{NULL, NULL}		/* Sentinel */
};

//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject App::FeaturePy::Parents[] = {&PyObjectBase::Type, NULL};

//--------------------------------------------------------------------------
//t constructor
//--------------------------------------------------------------------------
App::FeaturePy::FeaturePy(Feature *pcFeature, PyTypeObject *T)
: PyObjectBase( T), _pcFeature(pcFeature)
{
	Base::Console().Log("Create FeaturePy: %p \n",this);
}

PyObject *FeaturePy::PyMake(PyObject *ignored, PyObject *args)	// Python wrapper
{
  //return new FeaturePy(name, n, tau, gamma);			// Make new Python-able object
	return 0;
}

Base::PyObjectBase *Feature::GetPyObject(void)
{
	return new FeaturePy(this);
}


//--------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------
FeaturePy::~FeaturePy()						// Everything handled in parent
{
	Base::Console().Log("Destroy FeaturePy: %p \n",this);
}


//--------------------------------------------------------------------------
// FeaturePy representation
//--------------------------------------------------------------------------
PyObject *FeaturePy::_repr(void)
{
  std::stringstream a;
  a << "Feature: [ ";
  for(std::map<std::string,int>::const_iterator It = _pcFeature->_PropertiesMap.begin();It!=_pcFeature->_PropertiesMap.end();It++)
  {
    a << It->first << "=" << _pcFeature->GetProperty(It->first.c_str()).GetAsString() << "; ";
  }
  a << "]" << std::endl;
	return Py_BuildValue("s", a.str().c_str());
}
//--------------------------------------------------------------------------
// FeaturePy Attributes
//--------------------------------------------------------------------------
PyObject *FeaturePy::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{ 
	PY_TRY{
		if (Base::streq(attr, "XXXX"))						
			return Py_BuildValue("i",1); 
		else
      // search in PropertyList
      if( _pcFeature->_PropertiesMap.find(attr) != _pcFeature->_PropertiesMap.end())
        return Py_BuildValue("s", _pcFeature->GetProperty(attr).GetAsString());
      else
			  _getattr_up(PyObjectBase); 						
	}PY_CATCH;
} 

int FeaturePy::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{ 
	if (Base::streq(attr, "XXXX")){						// settable new state
		//_pcDoc->SetUndoLimit(PyInt_AsLong(value)); 
		return 1;
  }else
      // search in PropertyList
      if( _pcFeature->_PropertiesMap.find(attr) != _pcFeature->_PropertiesMap.end()){
        try{
          SetProperty(attr,value);
        }catch(...){
          return 1;
        }
        return 0;
      }else
			  return PyObjectBase::_setattr(attr, value); 						
} 

void FeaturePy::SetProperty(const char *attr, PyObject *value)
{
  //char sBuf[256];
  //sprintf(sBuf,"%f",PyFloat_AsDouble(value));
  //_pcFeature->GetProperty(attr).Set(sBuf);
  if( PyObject_TypeCheck(value, &PyFloat_Type) )
    (dynamic_cast<PropertyFloat&>(_pcFeature->GetProperty(attr))).SetValue(PyFloat_AsDouble(value));
  else if( PyObject_TypeCheck(value, &PyInt_Type) )
    (dynamic_cast<PropertyInteger&>(_pcFeature->GetProperty(attr))).SetValue(PyInt_AsLong(value));
  else if( PyObject_TypeCheck(value, &PyString_Type) )
    (dynamic_cast<PropertyString&>(_pcFeature->GetProperty(attr))).Set(PyString_AsString(value));
  else
    throw Base::Exception("Not fiting type");
  _pcFeature->TouchProperty(attr);

}


//--------------------------------------------------------------------------
// Python wrappers
//--------------------------------------------------------------------------

PYFUNCIMP_D(FeaturePy,AddFeature)
{
	//char *pstr;
    //if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C 
    //    return NULL;                             // NULL triggers exception 

	Py_Error(PyExc_Exception,"Not implemented!");
}

