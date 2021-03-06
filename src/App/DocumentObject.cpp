/***************************************************************************
 *   Copyright (c) J�rgen Riegel          (juergen.riegel@web.de)          *
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
#endif

#include "Document.h"
#include "DocumentObject.h"
#include "DocumentObjectPy.h"

using namespace App;


PROPERTY_SOURCE(App::DocumentObject, App::PropertyContainer)


//===========================================================================
// DocumentObject
//===========================================================================

DocumentObject::DocumentObject(void)
:pcObjectPy(0),_pDoc(0)
{
  _solidMaterial.ambientColor.set(0.2f,0.2f,0.2f);
  _solidMaterial.diffuseColor.set(0.8f,0.8f,0.8f);
  _solidMaterial.specularColor.set(0.0f,0.0f,0.0f);
  _solidMaterial.emissiveColor.set(0.0f,0.0f,0.0f);
  _solidMaterial.shininess = 0.2f;
  _solidMaterial.transparency = 0.0f;

  _lineMaterial.ambientColor.set(0.2f,0.2f,0.2f);
  _lineMaterial.diffuseColor.set(0.1f,0.1f,0.1f);
  _lineMaterial.specularColor.set(0.0f,0.0f,0.0f);
  _lineMaterial.emissiveColor.set(0.0f,0.0f,0.0f);
  _lineMaterial.shininess = 0.0f;
  _lineMaterial.transparency = 0.0f;
  _lineSize = 2.0f;

  _pointMaterial.ambientColor.set(0.9f,0.9f,0.9f);
  _pointMaterial.diffuseColor.set(0.8f,0.8f,0.8f);;
  _pointMaterial.specularColor.set(0.0f,0.0f,0.0f);
  _pointMaterial.emissiveColor.set(0.0f,0.0f,0.0f);
  _pointMaterial.shininess = 0.3f;
  _pointMaterial.transparency = 0.0f;
  _pointSize = 2.0;
  //_showMode = "Flat";

  ADD_PROPERTY(name,("Unnamed"));
  ADD_PROPERTY(visibility,(true));
  ADD_PROPERTY(showMode,("Flat"));

  touchTime.setToActual();
  touchViewTime.setToActual();

}

DocumentObject::~DocumentObject(void)
{
  if(pcObjectPy)
  {
    pcObjectPy->setInvalid();
    pcObjectPy->DecRef();
  }
}

App::Document &DocumentObject::getDocument(void)
{
  return *_pDoc;
}

void DocumentObject::setDocument(App::Document* doc)
{
  _pDoc=doc;
}

void DocumentObject::onBevorChange(const Property* prop)
{
  if(_pDoc)
    _pDoc->onBevorChangeProperty(this,prop);
};

  /// get called by the container when a Proptery was changed
void DocumentObject::onChanged(const Property* prop)
{
  if(_pDoc)
    _pDoc->onChangedProperty(this,prop);
};

Base::PyObjectBase *DocumentObject::GetPyObject(void)
{
  if(!pcObjectPy){
    // ref counter is set to 1
    pcObjectPy = new DocumentObjectPy(this);
  }
  pcObjectPy->IncRef();
	return pcObjectPy; 
}

void DocumentObject::Touch(void)
{
  touchTime.setToActual();
}

void DocumentObject::TouchView(void)
{
  touchViewTime.setToActual();
}
