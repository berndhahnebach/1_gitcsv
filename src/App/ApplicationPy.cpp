/***************************************************************************
 *   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License (LGPL)   *
 *   as published by the Free Software Foundation; either version 2 of     *
 *   the License, or (at your option) any later version.                   *
 *   for detail see the LICENCE text file.                                 *
 *                                                                         *
 *   FreeCAD is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        * 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with FreeCAD; if not, write to the Free Software        * 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
 *   USA                                                                   *
 *                                                                         *
 *   Juergen Riegel 2002                                                   *
 ***************************************************************************/



#include "PreCompiled.h"

#ifndef _PreComp_
#	include <TColStd_SequenceOfExtendedString.hxx>
#	include <TCollection_ExtendedString.hxx>
# include <sstream>
#endif


#include "Application.h"
#include "Document.h"
#include "Function.h"
//#include "DocTypeAttr.h"
//#include "DocType.h"

// FreeCAD Base header
#include "../Base/Interpreter.h"
#include "../Base/Exception.h"
#include "../Base/Parameter.h"
#include "../Base/Console.h"
#include "../Base/EnvMacros.h"
#include "../Base/Factory.h"


//using Base::GetConsole;
using namespace Base;
using namespace App;



//**************************************************************************
// Python stuff

// Application Methods						// Methods structure
PyMethodDef Application::Methods[] = {
	{"DocNew",         (PyCFunction) Application::sNew,            1},
	{"DocOpen",        (PyCFunction) Application::sOpen,           1},
	{"DocSave"  ,      (PyCFunction) Application::sSave,           1},
	{"DocSaveAs",      (PyCFunction) Application::sSaveAs,         1},
	{"DocGet",         (PyCFunction) Application::sGet,            1},
	{"ParamGet",       (PyCFunction) Application::sGetParam,       1},
	{"Version",        (PyCFunction) Application::sGetVersion,     1},
	{"ConfigGet",      (PyCFunction) Application::sGetConfig,      1},
	{"ConfigSet",      (PyCFunction) Application::sSetConfig,      1},
	{"ConfigDump",     (PyCFunction) Application::sDumpConfig,     1},
	{"TemplateAdd",    (PyCFunction) Application::sTemplateAdd,    1},
	{"TemplateDelete", (PyCFunction) Application::sTemplateDelete ,1},
	{"TemplateGet",    (PyCFunction) Application::sTemplateGet    ,1},

  {NULL, NULL}		/* Sentinel */
};

PYFUNCIMP_S(Application,sOpen)
{
    char *pstr;
    if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C
        return NULL;                             // NULL triggers exception


	try {
		// return new document
		return (GetApplication().Open(pstr)->GetPyObject());
	}
	catch(Base::Exception e) {
		PyErr_SetString(PyExc_IOError, e.what());
		return 0L;
	}
	catch(Standard_Failure e)
	{
		Handle(Standard_Failure) E = Standard_Failure::Caught();
		std::stringstream strm;

		strm << E << endl;
		//strm.freeze();
		PyErr_SetString(PyExc_IOError, strm.str().c_str());
		return 0L;
	}

}

PYFUNCIMP_S(Application,sNew)
{
    char *pstr = 0;
    if (!PyArg_ParseTuple(args, "|s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	PY_TRY{
		Document*	pDoc = GetApplication().New(pstr);
		if (pDoc)
			return pDoc->GetPyObject();
		else
		{
			PyErr_SetString(PyExc_IOError, "Unknown Template");
			return NULL;
		}
	}PY_CATCH;
}


PYFUNCIMP_S(Application,sSave)
{
    char *pstr;
    if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	//Instance().Message("%s",pstr);				 // process massage 
	Py_INCREF(Py_None);
	return Py_None;                              // None: no errors
}


PYFUNCIMP_S(Application,sSaveAs)
{
    char *pstr;
    if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	//Instance().Message("%s",pstr);				 // process massage 
	Py_INCREF(Py_None);
	return Py_None;                              // None: no errors 
}

PYFUNCIMP_S(Application,sGet)
{
  char *pstr=0;
  if (!PyArg_ParseTuple(args, "|s", &pstr))     // convert args: Python->C 
    return NULL;                             // NULL triggers exception 

  if(pstr == 0){
    Base::FCPyObject *p = GetApplication().Active()->GetPyObject();
    p->_INCREF();
	  return p;
  }

	Py_INCREF(Py_None);
	return Py_None;                              // None: no errors 
}

PYFUNCIMP_S(Application,sGetParam)
{
    char *pstr=0;
    if (!PyArg_ParseTuple(args, "s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	PY_TRY{
		return GetPyObject(GetApplication().GetParameterGroupByPath(pstr)); 
	}PY_CATCH;
}


PYFUNCIMP_S(Application,sGetConfig)
{
	char *pstr=0;

    if (!PyArg_ParseTuple(args, "|s", &pstr))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
	if(pstr) // if parameter give deticated group
		return Py_BuildValue("s",GetApplication()._mConfig[pstr].c_str()); 
	else
	{
		PyObject *pDict = PyDict_New();
		for(std::map<std::string,std::string>::iterator It= GetApplication()._mConfig.begin();It!=GetApplication()._mConfig.end();It++)
		{
			PyBuf Buf(It->second.c_str()),Buf2(It->first.c_str());
			PyDict_SetItemString(pDict,Buf2.str,PyString_FromString(Buf.str));
		}
		return pDict;
		
	}
}

PYFUNCIMP_S(Application,sDumpConfig)
{

    if (!PyArg_ParseTuple(args, "") )    // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	std::string str;
	for(std::map<std::string,std::string>::iterator It= GetApplication()._mConfig.begin();It!=GetApplication()._mConfig.end();It++)
	{
		str += It->first ;
		int size = It->first.size();
		for(int l = 0; l < (28-size) ; l++)
			str += " ";

		str += "= " + It->second + "\r\n";
	}
	return Py_BuildValue("s",str.c_str());
		
}

PYFUNCIMP_S(Application,sSetConfig)
{
	char *pstr,*pstr2;

    if (!PyArg_ParseTuple(args, "ss", &pstr,&pstr2))  // convert args: Python->C 
        return NULL; // NULL triggers exception 

	GetApplication()._mConfig[pstr] = pstr2;

	Py_INCREF(Py_None);
	return Py_None;
}

PYFUNCIMP_S(Application,sGetVersion)
{
    if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
        return NULL; // NULL triggers exception 

	PyObject* pList = PyList_New(6); 
	PyObject *pItem;
	pItem = PyInt_FromLong(Application::VersionMajor);
	PyList_SetItem(pList, 0, pItem);
	pItem = PyInt_FromLong(Application::VersionMinor);
	PyList_SetItem(pList, 1, pItem);
	pItem = PyInt_FromLong(Application::VersionBuild);
	PyList_SetItem(pList, 2, pItem);
	pItem = PyString_FromString(Application::VersionDisDa);
	PyList_SetItem(pList, 3, pItem);
	pItem = PyString_FromString(Application::VersionTime);
	PyList_SetItem(pList, 4, pItem);
	pItem = PyString_FromString(Application::VersionDate);
	PyList_SetItem(pList, 5, pItem);

	return pList;
}

PYFUNCIMP_S(Application,sTemplateAdd)
{
	char*       psKey;
	PyObject*   pcObject;
	if (!PyArg_ParseTuple(args, "sO", &psKey,&pcObject))     // convert args: Python->C 
		return NULL;										// NULL triggers exception 

	Py_INCREF(pcObject);

	PyDict_SetItemString(GetApplication()._pcTemplateDictionary,psKey,pcObject);

	Py_INCREF(Py_None);
	return Py_None;
} 

PYFUNCIMP_S(Application,sTemplateDelete)
{
	char*       psKey;
	if (!PyArg_ParseTuple(args, "s", &psKey))     // convert args: Python->C 
		return NULL;										// NULL triggers exception 

	PyDict_DelItemString(GetApplication()._pcTemplateDictionary,psKey);

	Py_INCREF(Py_None);
    return Py_None;
} 
/*
PYFUNCIMP_S(Application,sWorkbenchActivate)
{
	char*       psKey;
	if (!PyArg_ParseTuple(args, "s", &psKey))     // convert args: Python->C 
		return NULL;										// NULL triggers exception 

	Instance->ActivateWorkbench(psKey);

  Py_INCREF(Py_None);
    return Py_None;
}
*/
PYFUNCIMP_S(Application,sTemplateGet)
{
    if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 

	return GetApplication()._pcTemplateDictionary;
}
