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
#	include <assert.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include "Handle.h"
#include "Exception.h"

using namespace Base;

//**************************************************************************
// Construction/Destruction

// here the implemataion! description should take place in the header file!
FCHandled::FCHandled() 
	: _lRefCount(0)
{


}

FCHandled::~FCHandled()
{
	if(_lRefCount != 0)
		throw Exception("handled deleted with Rerferences!!!!!\n");
}


//**************************************************************************
// separator for other implemetation aspects



void  FCHandled::AttachRef(void* pHandle)
{
	_lRefCount++;
}

void  FCHandled::DetachRef(void* pHandle)
{
	assert(_lRefCount > 0); 
	if (--_lRefCount == 0)
	{
		OnLastRef();
		try
		{
			delete this;
		}
		catch(...)
		{
			throw Exception("FChandled freed twice !!!!!\n");
		}
	}
}




