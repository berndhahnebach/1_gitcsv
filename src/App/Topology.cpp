/** \file Topology.cpp
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */

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


/** Precompiled header stuff
 *  on some compilers the precompiled header option gain significant compile 
 *  time! So every external header (libs and system) should included in 
 *  Precompiled.h. For systems without precompilation the header needed are
 *  included in the else fork.
 */
#include "PreCompiled.h"

#ifndef _PreComp_
#	include <assert.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include "Topology.h"
#include "../Base/Exception.h"
#include "../Base/Console.h"

using Base::Console;


//===========================================================================
// FCTopoShape - Warpper for the TopoDS classes
//===========================================================================

//--------------------------------------------------------------------------
// Exported functions
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Type structure
//--------------------------------------------------------------------------

PyTypeObject FCTopoShape::Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,						/*ob_size*/
	"FCLabel",				/*tp_name*/
	sizeof(FCTopoShape),	/*tp_basicsize*/
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
PyMethodDef FCTopoShape::Methods[] = {
  {"HasChild",         (PyCFunction) sPyHasChild,         Py_NEWARGS},

  {NULL, NULL}		/* Sentinel */
};

//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject FCTopoShape::Parents[] = {&FCTopoShape::Type, NULL};     

//--------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------
FCTopoShape::FCTopoShape(const TopoDS_Shape &cShape, PyTypeObject *T) 
 : PyObjectBase( T), _cTopoShape(cShape)
{
	Console().Log("Create TopoShape %p\n",this);
}

PyObject *FCTopoShape::PyMake(PyObject *ignored, PyObject *args)	// Python wrapper
{
  return new FCTopoShape(TopoDS_Shape());			// Make new Python-able object
}

//--------------------------------------------------------------------------
//  FCLabel destructor 
//--------------------------------------------------------------------------
FCTopoShape::~FCTopoShape()						// Everything handled in parent
{
	Console().Log("Destroy TopoShape %p\n",this);
} 

//--------------------------------------------------------------------------
// FCLabel Attributes
//--------------------------------------------------------------------------
PyObject *FCTopoShape::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{ 
	try{
		// Access the number of attributes at this label
		if (Base::streq(attr, "AttributeCount"))
		{
			return Py_BuildValue("i", 1); 
		}else
			_getattr_up(PyObjectBase); 						// send to parent
	}catch(...){
		Console().Log("Exception in FCTopoShape::_getattr()\n");
		return 0;
	}
} 

int FCTopoShape::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{ 
	if (Base::streq(attr, "Real"))						// settable new state
		; 
	else  
		return PyObjectBase::_setattr(attr, value);	// send up to parent
	return 0;
} 


//--------------------------------------------------------------------------
// Python wrappers
//--------------------------------------------------------------------------

PyObject *FCTopoShape::PyHasChild(PyObject *args)
{ 
    if (!PyArg_ParseTuple(args, "" ))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
	Py_Return; 
}
 




//**************************************************************************
//**************************************************************************
// Seperator for additional classes
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



