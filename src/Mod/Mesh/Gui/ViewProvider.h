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


#ifndef __VIEWPROVIDERMESH_H__
#define __VIEWPROVIDERMESH_H__

#include <Gui/ViewProvider.h>


class SoSeparator;
class SbVec3f;

namespace MeshGui {

/**
 * The ViewProviderInventorMesh class creates
 * a node representing the mesh data structure.
 * @author Werner Mayer
 */
class ViewProviderInventorMesh : public Gui::ViewProviderInventor
{
public:
  ViewProviderInventorMesh();
  virtual ~ViewProviderInventorMesh();

  /** 
   * Extracts the mesh data from the feature \a pcFeature and creates
   * an Inventor node \a SoNode with these data. 
   */
  virtual SoNode* create(App::Feature *pcFeature);

protected:
};

} // namespace MeshGui


#endif // __VIEWPROVIDERMESH_H__
