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


#ifndef __PropteryGeo_H__
#define __PropteryGeo_H__

// Std. configurations

#include <Base/Vector3D.h>
#include <Base/Matrix.h>
#include <Base/Placement.h>

#include "Property.h"
#include "PropertyLinks.h"

namespace Base {
  class Writer;
}

namespace App
{
  class Feature;
  class Placement;



/** Vector properties
 * This is the father of all properties handling Integers.
 */
class AppExport PropertyVector: public Property
{
  TYPESYSTEM_HEADER();

public:

       
	/**
	 * A constructor.
	 * A more elaborate description of the constructor.
	 */
	PropertyVector();

	/**
	 * A destructor.
	 * A more elaborate description of the destructor.
	 */
	~PropertyVector();

	/** Sets the property 
	 */
  void setValue(const Base::Vector3f &vec);
  void setValue(float x, float y, float z);

	/** This method returns a string representation of the property
	 */
	const Base::Vector3f &getValue(void) const;

  virtual PyObject *getPyObject(void);
  virtual void setPyObject(PyObject *);

  virtual void Save (Base::Writer &writer) const;
  virtual void Restore(Base::XMLReader &reader);

  virtual Property *Copy(void) const;
  virtual void Paste(const Property &from);

private:

  Base::Vector3f _cVec;

};





class AppExport PropertyVectorList: public PropertyLists
{
  TYPESYSTEM_HEADER();

public:

       
	/**
	 * A constructor.
	 * A more elaborate description of the constructor.
	 */
	PropertyVectorList();

	/**
	 * A destructor.
	 * A more elaborate description of the destructor.
	 */
	~PropertyVectorList();

  virtual void setSize(int newSize){_lValueList.resize(newSize);}   
  virtual int getSize(void) const {return _lValueList.size();}   

	/** Sets the property 
	 */
	void setValue(const Base::Vector3f&);
  
  /// index operator
  const Base::Vector3f& operator[] (const int idx) const {return _lValueList.operator[] (idx);}
  
  
  void  set1Value (const int idx, const Base::Vector3f& value){_lValueList.operator[] (idx) = value;}

  const std::vector<Base::Vector3f> &getValues(void) const{return _lValueList;}

  virtual PyObject *getPyObject(void);
  virtual void setPyObject(PyObject *);

  virtual void Save (Base::Writer &writer) const;
  virtual void Restore(Base::XMLReader &reader);

  virtual void SaveDocFile (Base::Writer &writer) const;
  virtual void RestoreDocFile(Base::Reader &reader);

  virtual Property *Copy(void) const;
  virtual void Paste(const Property &from);

private:

  std::vector<Base::Vector3f> _lValueList;

};

/** Vector properties
 * This is the father of all properties handling Integers.
 */
class AppExport PropertyMatrix: public Property
{
  TYPESYSTEM_HEADER();

public:

       
	/**
	 * A constructor.
	 * A more elaborate description of the constructor.
	 */
	PropertyMatrix();

	/**
	 * A destructor.
	 * A more elaborate description of the destructor.
	 */
	~PropertyMatrix();

	/** Sets the property 
	 */
  void setValue(const Base::Matrix4D &mat);

	/** This method returns a string representation of the property
	 */
	const Base::Matrix4D &getValue(void) const;

  virtual PyObject *getPyObject(void);
  virtual void setPyObject(PyObject *);

  virtual void Save (Base::Writer &writer) const;
  virtual void Restore(Base::XMLReader &reader);

  virtual Property *Copy(void) const;
  virtual void Paste(const Property &from);

private:
  Base::Matrix4D _cMat;
};

/** Vector properties
 * This is the father of all properties handling Integers.
 */
class AppExport PropertyPlacement: public Property
{
  TYPESYSTEM_HEADER();

public:

       
	/**
	 * A constructor.
	 * A more elaborate description of the constructor.
	 */
	PropertyPlacement();

	/**
	 * A destructor.
	 * A more elaborate description of the destructor.
	 */
	~PropertyPlacement();

	/** Sets the property 
	 */
  void setValue(const Base::Placement &pos);

	/** This method returns a string representation of the property
	 */
	const Base::Placement &getValue(void) const;

  virtual PyObject *getPyObject(void);
  virtual void setPyObject(PyObject *);

  virtual void Save (Base::Writer &writer) const;
  virtual void Restore(Base::XMLReader &reader);

  virtual Property *Copy(void) const;
  virtual void Paste(const Property &from);

private:
  Base::Placement _cPos;
};

/** the general Link Poperty
 *  Main Purpose of this property is to Link Objects and Feautures in a document.
 */
class AppExport PropertyPlacementLink: public PropertyLink
{
  TYPESYSTEM_HEADER();

public:

       
	/**
	 * A constructor.
	 * A more elaborate description of the constructor.
	 */
	PropertyPlacementLink();

	/**
	 * A destructor.
	 * A more elaborate description of the destructor.
	 */
	~PropertyPlacementLink();


	/** This method returns the linked DocumentObject
	 */
	App::Placement * getPlacementObject(void) const;

  virtual Property *Copy(void) const;
  virtual void Paste(const Property &from);

};


} // namespace App

#endif // __PropteryStandard_H__
