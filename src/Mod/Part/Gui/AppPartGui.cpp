/** \file AppPartGui.cpp
 *  \brief 
 *  \author $Author$
 *  \version $Revision$
 *  \date    $Date$
 */


/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *   for detail see the LICENCE text file.                                 *
 *   J�rgen Riegel 2002                                                    *
 *                                                                         *
 ***************************************************************************/

 
#include "../../../Config.h"
#ifdef _PreComp_
#	include "PreCompiled.h"
#else
#endif

#include "../../../App/Application.h"
#include "../../../Base/Console.h"

#include "../../../Gui/Application.h"

void CreateCommands(void);

/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

*/


/* module functions */
static PyObject *                                 /* returns object */
message(PyObject *self, PyObject *args)           /* self unused in modules */
{                                                 /* args from python call */
    char *fromPython, result[64];
    if (! PyArg_Parse(args, "(s)", &fromPython))  /* convert Python -> C */
        return NULL;                              /* null=raise exception */
    else {
        strcpy(result, "Hello, ");                /* build up C string */
        strcat(result, fromPython);               /* add passed Python string */
        return Py_BuildValue("s", result);        /* convert C -> Python */
    }
}

/* registration table  */
static struct PyMethodDef hello_methods[] = {
    {"message", message, 1},       /* method name, C func ptr, always-tuple */
    {NULL, NULL}                   /* end of table marker */
};





// python intry
extern "C" {
#ifdef _DEBUG
void __declspec(dllexport) initAppPartGuiD() {
#else
void __declspec(dllexport) initAppPartGui() {
#endif

#ifdef _DEBUG
	(void) Py_InitModule("AppPartGuiD", hello_methods);   /* mod name, table ptr */
#else
	(void) Py_InitModule("AppPartGui", hello_methods);   /* mod name, table ptr */
#endif

	GetConsole().Log("AppPartGui loaded\n");

	GetApplication();
	ApplicationWindow::Instance;

	// instanciating the commands
	CreateCommands();


	return;
}
} // extern "C" {




/*
#include <boost/python/class_builder.hpp>
namespace python = boost::python;

std::string info() { return "hello, world"; }


BOOST_PYTHON_MODULE_INIT(getting_started1)
{
    // Create an object representing this extension module.
    python::module_builder this_module("getting_started1");

    // Add regular functions to the module.
    this_module.def(info, "Info");
    //this_module.def(square, "square");
}
*/