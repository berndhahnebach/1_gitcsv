/***************************************************************************
 *   Copyright (c) 2005 Berthold Grupp                                     *
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

#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include <set>
#include <vector>

#include "MeshKernel.h"
#include <Base/Vector3D.h>


namespace MeshCore
{
class MeshKernel;
class MeshPoint;
class MeshGeomFacet;

class AppMeshExport MeshBuilder
{
  /**
 * Class for creating the mesh structure by adding facets. Building the structure needs 3 steps:
 * 1. initializing  
 * 2. adding the facets
 * 3. finishing
 * \code
 * // Sample Code for building a mesh structure
 * MeshBuilder builder(someMeshReference);
 * builder.Initialize(numberOfFacets);
 * ...
 * for (...)
 *   builder.AddFacet(...);
 * ...
 * builder.Finish();
 * \endcode
 * @author Berthold Grupp
 */

private:
  /** @name Helper class */
  //@{
	class Edge
	{
		public:
		unsigned long	pt1, pt2, facetIdx;

		Edge (unsigned long p1, unsigned long p2, unsigned long idx)
		{
			facetIdx = idx;
			if (p1 > p2)
			{
				pt1 = p2;
				pt2 = p1;
			}
			else
			{
				pt1 = p1;
				pt2 = p2;
			}
		}

		bool operator < (const Edge &e) const
		{
			return (pt1 == e.pt1) ? (pt2 < e.pt2) : (pt1 < e.pt1);
		}

		bool operator > (const Edge &e) const
		{
			return (pt1 == e.pt1) ? (pt2 > e.pt2) : (pt1 > e.pt1);
		}

		bool operator == (const Edge &e) const
		{
			return (pt1 == e.pt1) && (pt2 == e.pt2);
		}
	};
  //@}

	MeshKernel& _meshKernel;

	std::set<MeshPoint>	_points;

  // keep an array of iterators pointing to the vertex inside the set to save memory
  typedef std::pair<std::set<MeshPoint>::iterator, bool> MeshPointIterator;
  std::vector<MeshPointIterator> _pointsIterator;
	unsigned long				_ptIdx; 

  void SetNeighbourhood  ();
  // As it's forbidden to insert a degenerated facet but insert its vertices anyway we must remove them 
  void RemoveUnreferencedPoints();

public:
	MeshBuilder(MeshKernel &rclM);
	~MeshBuilder(void);

  /** Initializes the class. Must be done before adding facets 
   * @param \a ctFacets count of facets. 
   * @param \a deletion if true (default) the mesh-kernel will be cleared otherwise you can add new facets on an existing mesh-kernel
   * @remark To be efficient you should add exactly \a ctFacets with AddFacet(), otherwise you'll possibly run into wastage of memory
   * and performance problems.
   */
	void Initialize (unsigned long ctFacets, bool deletion = true);

  /** adding facets */
  /** Add new facet
    * facet \a the facet
    * @param \a takeFlag if true the flag from the MeshGeomFacet will be taken
   */
	void AddFacet (const MeshGeomFacet& facet, bool takeFlag = false, bool takePropery = false);
  /** Add new facet
   */
	void AddFacet (const Base::Vector3f& pt1, const Base::Vector3f& pt2, const Base::Vector3f& pt3, const Base::Vector3f& normal, unsigned char flag = 0, unsigned long prop = 0);
  /** Add new facet
   * @param facetPoints Array of vectors (size 4) in order of vec1, vec2, vec3, normal
   */
	void AddFacet (Base::Vector3f* facetPoints, unsigned char flag = 0, unsigned long prop = 0);

  /** Finishes building up the mesh structure. Must be done after adding facets.
   * @param \a freeMemory if false (default) only the memory of internal structures gets freed, otherwise 
   * additional unneeded memory in the mesh structure is tried to be freed.
   * @remark If you have called AddFacet() as many times as specified in Initialize() then absolutely no memory is wasted
   * and you can leave the default value.
   */
	void Finish (bool freeMemory=false);

  friend class MeshKernel;
};

} // namespace MeshCore

#endif 
