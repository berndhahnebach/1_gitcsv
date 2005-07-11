/***************************************************************************
 *   Copyright (c) 2005 Werner Mayer <werner.wm.mayer@gmx.de>              *
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


#ifndef FEATURE_MESH_IMPORT_H
#define FEATURE_MESH_IMPORT_H

#include "MeshFeature.h"

namespace Mesh
{

/**
 * The FeatureMeshImport class reads the any supported mesh format
 * into the FreeCAD workspace.
 * @author Werner Mayer
 */
class FeatureMeshImport : public MeshFeature
{
public:
  /** Adds the "filename" property to this feature. */
	virtual void InitLabel(const TDF_Label &rcLabel);
  /** 
   * Loads the actual mesh data into memory. If the read of data fails 
   * the value 2 is returned, if the "filename" property is invalid 1 is
   * returned, 0 otherwise.
   */
	virtual Standard_Integer Execute(TFunction_Logbook& log);

  /** @todo */
	virtual void Validate(TFunction_Logbook& log);

  /// Returns the Name/Type of the feature
  virtual const char *Type(void){return "MeshImport";};
};

}

#endif // FEATURE_MESH_IMPORT_H 