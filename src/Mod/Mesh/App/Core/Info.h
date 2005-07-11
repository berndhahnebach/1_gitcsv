/***************************************************************************
 *   Copyright (c) 2005 Imetric 3D GmbH                                    *
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


#ifndef MESH_INFO_H
#define MESH_INFO_H

#include "MeshKernel.h"

namespace Mesh {

class MeshKernel;

/**
 * Determines information about the mesh data structure.
 */
class AppMeshExport MeshInfo
{
public:
  MeshInfo (MeshKernel &rclM);
  virtual ~MeshInfo (void) {}
  /** 
   * Calculates the surface area of the mesh object.
   */
  float GetSurface() const;
  /** 
   * Calculates the volume of the mesh object. Therefore the mesh must be a solid, if not 0
   * is returned.
   */
  float GetVolume () const;
  /**
   * Writes general information about the mesh structure into the stream.
   */
  std::ostream& GeneralInformation (std::ostream& rclStream) const;
  /**
   * Writes detailed information about the mesh structure into the stream.
   */
  std::ostream& DetailedInformation (std::ostream& rclStream) const;
  /**
   * Writes internal information about the mesh structure into the stream.
   */
  std::ostream& InternalInformation (std::ostream& rclStream) const;

protected:
  /**
   * Writes detailed point information.
   */
  std::ostream& DetailedPointInfo (std::ostream& rclStream) const;
  /**
   * Writes detailed edge information.
   */
  std::ostream& DetailedEdgeInfo (std::ostream& rclStream) const;
  /**
   * Writes detailed facet information.
   */
  std::ostream& DetailedFacetInfo (std::ostream& rclStream) const;
  /**
   * Writes internal point information.
   */
  std::ostream& InternalPointInfo (std::ostream& rclStream) const;
  /**
   * Writes internal edge information.
   */
  std::ostream& InternalEdgeInfo (std::ostream& rclStream) const;
  /**
   * Writes internal facet information.
   */
  std::ostream& InternalFacetInfo (std::ostream& rclStream) const;


protected:
  MeshKernel &_rclMesh;   // const reference to mesh data structure

private:
  MeshInfo(void);         // not accessible default constructor
};


} // namespace Mesh

#endif