/***************************************************************************
                          FCApplication.cpp  -  description
                             -------------------
    begin                : Tue Jan 2 2001
    copyright            : (C) 2001 by Juergen Riegel
    email                : juergen.riegel@web.de
 ***************************************************************************/


#include "PreCompiled.h"

#ifndef _PreComp_
#	include <TDocStd_Application.hxx>
#	include <TDataStd_Real.hxx>
#	include <TDataStd_Name.hxx>
#	include <Handle_TDataStd_Name.hxx>
#	include <TDataStd_Integer.hxx>
#	include <Handle_TDataStd_Integer.hxx>
#endif


#include "Document.h"
#include "Label.h"
#include "../Base/Console.h"
#include "../Base/Exception.h"
#include "Application.h"
#include "Attribute.h"


//===========================================================================
// FCLabel - Warpper for the TDF_Label classes
//===========================================================================

//--------------------------------------------------------------------------
// Exported functions
//--------------------------------------------------------------------------

FCPyHandle<FCLabel> FCLabel::GetLabel(int iN)
{
	return FCPyHandle<FCLabel>(_pcDocument->HasPyLabel( _cLabel.FindChild(iN)));
}

/// Gets a child label by name (Name Attribute), creats if not exist
FCPyHandle<FCLabel> FCLabel::GetLabel(const char* sName)
{

	// create a new label and put a name on it
	TDF_Label cL = _cLabel.NewChild();
	TDataStd_Name::Set(cL, TCollection_ExtendedString((Standard_CString)sName));
	return FCPyHandle<FCLabel>(_pcDocument->HasPyLabel( cL ));	


//	return FCPyHandle<FCLabel>(_pcDocument->HasPyLabel( _cLabel.FindChild(iN)));
}

/// checks if the label is there
bool FCLabel::HasLabel(int iN)
{
	//return _cLabel.FindChild(iN,false);
	return false;
}


/// checks if the label is there by name (Name Attribute)
bool FCLabel::HasLabel(const char* sName)
{
	// scann all child labels to find the right name
	for (TDF_ChildIterator it(_cLabel); it.More(); it.Next())
	{
		Handle(TDataStd_Name) NameAttr;
		if(it.Value().FindAttribute(TDataStd_Name::GetID(),NameAttr))
			if( TCollection_ExtendedString((Standard_CString) sName) == NameAttr->Get()/*.ToExtString()*/)
				return true;
	}

	return false;

}



/// Get all child labels
std::vector<FCPyHandle<FCLabel> > FCLabel::GetLabels(void)
{
	std::vector<FCPyHandle<FCLabel> > vhcLabels;

	for (TDF_ChildIterator it(_cLabel); it.More(); it.Next())
		vhcLabels.push_back( FCPyHandle<FCLabel>(_pcDocument->HasPyLabel(it.Value())));

	return vhcLabels;
}

//--------------------------------------------------------------------------
// Type structure
//--------------------------------------------------------------------------

PyTypeObject FCLabel::Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,						/*ob_size*/
	"FCLabel",				/*tp_name*/
	sizeof(FCLabel),			/*tp_basicsize*/
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
PyMethodDef FCLabel::Methods[] = {
  {"GetLabel",         (PyCFunction) sPyGetLabel,         Py_NEWARGS},
  {"GetName",          (PyCFunction) sPyGetName,          Py_NEWARGS},
  {"HasChildren",      (PyCFunction) sPyHasChildren,      Py_NEWARGS},
  {"AttributeCount",   (PyCFunction) sPyAttributeCount,   Py_NEWARGS},
  {"ChildrenCount",    (PyCFunction) sPyChildrenCount,    Py_NEWARGS},
  {"GetRoot",          (PyCFunction) sPyGetRoot,          Py_NEWARGS},
  {"GetFather",        (PyCFunction) sPyGetFather,        Py_NEWARGS},

  {NULL, NULL}		/* Sentinel */
};

//--------------------------------------------------------------------------
// Parents structure
//--------------------------------------------------------------------------
PyParentObject FCLabel::Parents[] = {&FCLabel::Type, NULL};     

//--------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------
FCLabel::FCLabel(const TDF_Label &cLabel,FCDocument *pcDocument, PyTypeObject *T) 
 : _cLabel(cLabel),_pcDocument(pcDocument),FCPyObject( T)
{
	GetConsole().Log("Create Label %p\n",this);
	
    //printf("Tag. %d\n",cLabel.Tag());
    //printf("Depth. %d\n",cLabel.Depth());

}

PyObject *FCLabel::PyMake(PyObject *ignored, PyObject *args)	// Python wrapper
{
  //return new FCLabel();			// Make new Python-able object
  return 0;
}

//--------------------------------------------------------------------------
//  FCLabel destructor 
//--------------------------------------------------------------------------
FCLabel::~FCLabel()						// Everything handled in parent
{
	GetConsole().Log("Destroy Label %p\n",this);
} 

//--------------------------------------------------------------------------
// FCLabel Attributes
//--------------------------------------------------------------------------
PyObject *FCLabel::_getattr(char *attr)				// __getattr__ function: note only need to handle new state
{ 
	try{
		// Access the number of attributes at this label
		if (streq(attr, "AttributeCount"))						
			return Py_BuildValue("i", _cLabel.NbAttributes()); 
		else if (streq(attr, "ChildrenCount"))					
			return Py_BuildValue("i", _cLabel.NbChildren()); 
		else if (streq(attr, "Root"))						
			return _pcDocument->HasPyLabel( _cLabel.Root()); 
		else if (streq(attr, "Father"))						
			return _pcDocument->HasPyLabel( _cLabel.Father()); 
		else if (streq(attr, "Real")){
			Handle(TDataStd_Real) RealAttr;
			if(_cLabel.FindAttribute(TDataStd_Real::GetID(),RealAttr))
				return Py_BuildValue("d",RealAttr->Get()); 
			else
				return 0;
		}else if (streq(attr, "Int")){
			Handle(TDataStd_Integer) IntAttr;
			if(_cLabel.FindAttribute(TDataStd_Integer::GetID(),IntAttr))
				return Py_BuildValue("d",IntAttr->Get()); 
			else
				return 0;
		}else if (streq(attr, "Name")){
			Handle(TDataStd_Name) NameAttr;
			if(_cLabel.FindAttribute(TDataStd_Name::GetID(),NameAttr))
				//return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#ifdef FC_OS_LINUX /* will "u" work? */ // u is unicode as ToExtString is!
				return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#else
				return Py_BuildValue("s",NameAttr->Get()); 
#endif				
			else
				return 0;
		}else
			_getattr_up(FCPyObject); 						// send to parent
	}catch(...){
		GetConsole().Log("Exception in FCLabel::_getattr()\n");
		return 0;
	}
} 

int FCLabel::_setattr(char *attr, PyObject *value) 	// __setattr__ function: note only need to handle new state
{ 
	if (streq(attr, "Real"))						// settable new state
		TDataStd_Real::Set(_cLabel, PyFloat_AsDouble(value)); 
	else if (streq(attr, "Int"))						// settable new state
		TDataStd_Integer::Set(_cLabel, PyInt_AsLong(value)); 
	else if (streq(attr, "Name"))						// settable new state
		TDataStd_Name::Set(_cLabel, (short*)PyString_AsString(value)); 
		//TDataStd_Name::Set(_cLabel, (short*)PyUnicode_AsUnicode(value)); 
	else  
 
		return FCPyObject::_setattr(attr, value);	// send up to parent
	return 0;
} 


//--------------------------------------------------------------------------
// Python wrappers
//--------------------------------------------------------------------------

PyObject *FCLabel::PyGetLabel(PyObject *args)
{ 
	int Tag;
    if (!PyArg_ParseTuple(args, "i",&Tag ))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
	FCPyHandle<FCLabel> hcL = GetLabel( Tag);
	hcL->_INCREF();
	return hcL.GetPyObject();
}
 
PyObject *FCLabel::PyHasChildren(PyObject *args)
{ 
	if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	return Py_BuildValue("i",HasChildren()?1:0);
} 

PyObject *FCLabel::PyAttributeCount(PyObject *args)
{ 
	if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	return Py_BuildValue("i",_cLabel.NbAttributes());
} 

PyObject *FCLabel::PyChildrenCount(PyObject *args)
{ 
	if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	return Py_BuildValue("i",_cLabel.NbChildren());
}
 
PyObject *FCLabel::PyGetRoot(PyObject *args)
{ 
	if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	return _pcDocument->HasPyLabel( _cLabel.Root());
} 

PyObject *FCLabel::PyGetFather(PyObject *args)
{ 
	if (!PyArg_ParseTuple(args, ""))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	return _pcDocument->HasPyLabel( _cLabel.Father()); 
} 

PyObject *FCLabel::PyGetName(PyObject *args)
{ 
	char *pstr;
	if (!PyArg_ParseTuple(args, "s",&pstr))     // convert args: Python->C 
		return NULL;                             // NULL triggers exception 
	Handle(TDataStd_Name) NameAttr;
	if(_cLabel.FindAttribute(TDataStd_Name::GetID(),NameAttr))
		//return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#ifdef FC_OS_LINUX /* will "u" work? */ // u is unicode as ToExtString is!
		return Py_BuildValue("u",NameAttr->Get().ToExtString()); 
#else
		return Py_BuildValue("s",NameAttr->Get()); 
#endif				
	else
		return 0;
} 


/*
PyObject *FCLabel::PyIsDifferent(PyObject *args)
{ 
	PyObject* pcObject;
    if (!PyArg_ParseTuple(args, "O!", &Type, &pcObject))     // convert args: Python->C 
        return NULL;                             // NULL triggers exception 
	
	//_cLabel.IsDifferent( ((FCLabel*)pcObject)->GetLabel() );

	return Py_BuildValue("i", _cLabel.IsDifferent( ((FCLabel*)pcObject)->GetLabel() ) );
	//Undo(); 
	//Py_Return; 
} 

*/


