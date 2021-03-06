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


#ifndef MESHALGORITHM_H
#define MESHALGORITHM_H

#include <set>
#include <vector>
#include <Wm3Plane3.h>

#include "MeshKernel.h"
#include "Elements.h"
#include <Base/Vector3D.h>

// forward declarations

namespace Base{
  class ViewProjMethod;
  class Polygon2D;
}

namespace MeshCore {

class MeshGeomFacet;
class MeshGeomEdge;
class MeshKernel;
class MeshFacetGrid;
class MeshFacetArray;

/**
 * The MeshAlgorithm class provides algorithms base on meshes.
 */
class AppMeshExport MeshAlgorithm
{
public:
  /// Construction
  MeshAlgorithm (const MeshKernel &rclM) : _rclMesh(rclM) { }
  /// Destruction
  ~MeshAlgorithm (void) { }

public:
  /**
   * Searches for the nearest facet to the ray defined by (\arclPt, \a rclDir). The point \a rclRes holds
   * the intersection point with the ray and the nearest facet with index \a rulFacet.
   * \note This method tests all facets so it should only be used occassionally.
   */
  bool NearestFacetOnRay (const Base::Vector3f &rclPt, const Base::Vector3f &rclDir, Base::Vector3f &rclRes,
                          unsigned long &rulFacet) const;
  /**
   * Searches for the nearest facet to the ray defined by (\arclPt, \a rclDir). The point \a rclRes holds
   * the intersection point with the ray and the nearest facet with index \a rulFacet.
   * \note This method is optimized by using a grid. So this method be used for a lot of tests..
   */
  bool NearestFacetOnRay (const Base::Vector3f &rclPt, const Base::Vector3f &rclDir, const MeshFacetGrid &rclGrid,
                          Base::Vector3f &rclRes, unsigned long &rulFacet) const;
  /**
   * Searches for the nearest facet to the ray defined by (\arclPt, \a rclDir). The point \a rclRes holds
   * the intersection point with the ray and the nearest facet with index \a rulFacet.
   * \note This method tests all facets taken from \a raulFacets so it should only be used occassionally.
   */
  bool NearestFacetOnRay (const Base::Vector3f &rclPt, const Base::Vector3f &rclDir, const std::vector<unsigned long> &raulFacets,
                          Base::Vector3f &rclRes, unsigned long &rulFacet) const;
  /**
   * Searches for the nearest facet to the ray defined by (\arclPt, \a rclDir). The point \a rclRes holds
   * the intersection point with the ray and the nearest facet with index \a rulFacet.
   * More a search radius around the ray of \a fMaxSearchArea is defined.
   * \note This method is optimized by using a grid. So this method be used for a lot of tests..
   */
  bool NearestFacetOnRay (const Base::Vector3f &rclPt, const Base::Vector3f &rclDir, float fMaxSearchArea,
                          const MeshFacetGrid &rclGrid, Base::Vector3f &rclRes, unsigned long &rulFacet) const;
  /**
   * Checks from the viewpoint \a rcView if the vertex \a rcVertex is visible or it is hidden by a facet. 
   * If the vertex is visible true is returned, false otherwise.
   */
  bool IsVertexVisible (const Base::Vector3f &rcVertex, const Base::Vector3f &rcView, const MeshFacetGrid &rclGrid ) const;
  /**
   * Calculates the average length of edges.
   */
  float GetAverageEdgeLength() const;
  /**
   * Returns all boundaries of the mesh.
   */
  void GetMeshBorders (std::list<std::vector<Base::Vector3f> > &rclBorders) const;
  /**
   * Returns all boundaries of a subset the mesh defined by \a raulInd.
   */
  void GetFacetBorders (const std::vector<unsigned long> &raulInd, std::list<std::vector<Base::Vector3f> > &rclBorders) const;
  /** Sets to all facets in \a raulInds the properties in raulProps. 
   * \note Both arrays must have the same size.
   */
  void SetFacetsProperty(const std::vector<unsigned long> &raulInds, const std::vector<unsigned long> &raulProps) const;
  /** Sets to all facets the flag \a tF. */
  void SetFacetFlag (MeshFacet::TFlagType tF) const;
  /** Sets to all points the flag \a tF. */
  void SetPointFlag (MeshPoint::TFlagType tF) const;
  /** Resets of all facets the flag \a tF. */
  void ResetFacetFlag (MeshFacet::TFlagType tF) const;
  /** Resets of all points the flag \a tF. */
  void ResetPointFlag (MeshPoint::TFlagType tF) const;
  /** Sets to all facets in \a raulInds the flag \a tF. */
  void SetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const;
  /** Sets to all points in \a raulInds the flag \a tF. */
  void SetPointsFlag (const std::vector<unsigned long> &raulInds, MeshPoint::TFlagType tF) const;
  /** Resets from all facets in \a raulInds the flag \a tF. */
  void ResetFacetsFlag (const std::vector<unsigned long> &raulInds, MeshFacet::TFlagType tF) const;
  /** Resets from all points in \a raulInds the flag \a tF. */
  void ResetPointsFlag (const std::vector<unsigned long> &raulInds, MeshPoint::TFlagType tF) const;
  /** Count all facets with the flag \a tF. */
  unsigned long CountFacetFlag (MeshFacet::TFlagType tF) const;
  /** Count all points with the flag \a tF. */
  unsigned long CountPointFlag (MeshPoint::TFlagType tF) const;
  /** Returns all geometric points from the facets in \a rvecIndices. */
  void PointsFromFacetsIndices (const std::vector<unsigned long> &rvecIndices, std::vector<Base::Vector3f> &rvecPoints) const;
  /**
   * Returns the indices of all facets that have at least one point that lies inside the tool mesh. The direction
   * \a dir is used to try to foraminate the facets of the tool mesh and counts the number of formainated facets.
   * If this number is odd the considered point lies inside otherwise outside.
   * @note The tool mesh must be a valid solid.
   * @note It's not tested if \a rToolMesh is a valid solid. In case it is not the result is undefined.
   */
  void GetFacetsFromToolMesh( const MeshKernel& rToolMesh, const Base::Vector3f& rcDir, std::vector<unsigned long> &raclCutted ) const;
  /**
   * Does basically the same as method above except it uses a mesh grid to speed up the computation.
   */
  void GetFacetsFromToolMesh( const MeshKernel& rToolMesh, const Base::Vector3f& rcDir, const MeshFacetGrid& rGrid, std::vector<unsigned long> &raclCutted ) const;
  /**
   * Projects the determined facets through projection with \a pclProj into the 2D plane and checks for
   * intersection with the polygon.
   * If \a bInner is \a true than all facets with at least one corner inside the polygon get deleted. If \a
   * bInner is \a false then all facets with at least one corner outside the polygon get deleted.
   * This algorithm is optimized by using a grid.
   */
  void CheckFacets (const MeshFacetGrid &rclGrid, const Base::ViewProjMethod* pclProj, const Base::Polygon2D& rclPoly,
                    bool bInner, std::vector<unsigned long> &rclRes) const;
  /**
   * Determines all facets of the given array \a raclFacetIndices that lie at the edge or that
   * have at least neighbour facet that is not inside the array. The resulting array \a raclResultIndices
   * is not be deleted before the algorithm starts. \a usLevel indicates how often the algorithm is 
   * repeated.
   */
  void CheckBorderFacets (const std::vector<unsigned long> &raclFacetIndices, 
                          std::vector<unsigned long> &raclResultIndices, unsigned short usLevel = 1) const;
  /**
   * Invokes CheckBorderFacets() to get all border facets of \a raclFacetIndices. Then the content of
   * \a raclFacetIndices is replaced by all facets that can be deleted.
   * \note The mesh structure is not modified by this method. This is in the responsibility of the user.
   */
  void CutBorderFacets (std::vector<unsigned long> &raclFacetIndices, unsigned short usLevel = 1) const;
  /**
   * Determines all border points as indices of the facets in \a raclFacetIndices. The points are unsorted.
   */
  void GetBorderPoints (const std::vector<unsigned long> &raclFacetIndices, std::set<unsigned long> &raclResultPointsIndices) const;
  /** Tessellates the shape \a aShape and replaces the mesh structure with the created facets. */
#if 0 // move to a module that uses Part and Mesh
  bool MeshTopoShape(TopoDS_Shape aShape, float fAccuracy, float fAngle) const;
#endif
  /** Computes the surface of the mesh. */
  float Surface (void) const;
  /** Subsamples the mesh with point distance \a fDist and stores the points in \a rclPoints. */
  void SubSampleByDist  (float fDist, std::vector<Base::Vector3f> &rclPoints) const;
  /**
   * Subsamples the mesh to produce around \a ulCtPoints. \a ulCtPoints should be greater
   * than 5 * number of facets.
   */
  void SubSampleByCount (unsigned long ulCtPoints, std::vector<Base::Vector3f> &rclPoints) const;
  /** Returns only the points of the mesh without actually sampling the data. */
  void SubSampleAllPoints(std::vector<Base::Vector3f> &rclPoints) const;
  /**
   * Searches for all facets that intersect the "search tube" with radius \a r around the polyline. 
   */
  void SearchFacetsFromPolyline (const std::vector<Base::Vector3f> &rclPolyline, float fRadius,
                                 const MeshFacetGrid& rclGrid, std::vector<unsigned long> &rclResultFacetsIndices) const;
  /** Projects a point directly to the mesh (means nearest facet), the result is the facet index and
   * the formainate point, use second version with grid for more performance.
   */
  bool NearestPointFromPoint (const Base::Vector3f &rclPt, unsigned long &rclResFacetIndex, Base::Vector3f &rclResPoint) const;
  bool NearestPointFromPoint (const Base::Vector3f &rclPt, const MeshFacetGrid& rclGrid,
                              unsigned long &rclResFacetIndex, Base::Vector3f &rclResPoint) const;
  bool NearestPointFromPoint (const Base::Vector3f &rclPt, const MeshFacetGrid& rclGrid, float fMaxSearchArea,
                              unsigned long &rclResFacetIndex, Base::Vector3f &rclResPoint) const;
  /** Cuts the mesh with a plane. The result is a list of polylines. */
  bool CutWithPlane (const Wm3::Plane3<float> &rclPlane, const MeshFacetGrid &rclGrid, std::list<std::vector<Base::Vector3f> > &rclResult, float fMinEps = 1.0e-2f) const;
  bool CutWithPlane (const Base::Vector3f &clBase, const Base::Vector3f &clNormal, const MeshFacetGrid &rclGrid,
                     std::list<std::vector<Base::Vector3f> > &rclResult, float fMinEps = 1.0e-2f, bool bConnectPolygons = false) const;
  /** Gets all facets that cut the plane and lying between the the two points. */
  void GetFacetsFromPlane (const MeshFacetGrid &rclGrid, const Wm3::Plane3<float>& clPlane, const Base::Vector3f &rclLeft, const Base::Vector3f &rclRight, std::vector<unsigned long> &rclRes) const;
  /** Returns true if the distance from the \a rclPt to the facet \a ulFacetIdx is less than \a fMaxDistance.
   * If this restriction is met \a rfDistance is set to the actual distance, otherwise false is returned.
   */
  bool Distance (const Base::Vector3f &rclPt, unsigned long ulFacetIdx, float fMaxDistance, float &rfDistance) const;
   
protected:
  /** Helper method to connect the intersection points to polylines. */
  bool ConnectLines (std::list<std::pair<Base::Vector3f, Base::Vector3f> > &rclLines, std::list<std::vector<Base::Vector3f> >&rclPolylines,
                    float fMinEps) const;
  bool ConnectPolygons(std::list<std::vector<Base::Vector3f> > &clPolyList, std::list<std::pair<Base::Vector3f,
                       Base::Vector3f> > &rclLines) const;
  /** Searches the nearest facet in \a raulFacets to the ray (\a rclPt, \a rclDir). */
  bool RayNearestField (const Base::Vector3f &rclPt, const Base::Vector3f &rclDir, const std::vector<unsigned long> &raulFacets,
                        Base::Vector3f &rclRes, unsigned long &rulFacet, float fMaxAngle = F_PI) const;

protected:
  const MeshKernel      &_rclMesh; /**< The mesh kernel. */
};

/**
 * The MeshRefPointToFacets builds up a structure to have access to all facets indexing
 * a point.
 * \note If the underlying mesh kernel gets changed this structure becomes invalid and must
 * be rebuilt.
 */
class AppMeshExport MeshRefPointToFacets : public std::vector<std::set<MeshFacetArray::_TConstIterator> >
{
public:
  /// Construction
  MeshRefPointToFacets (const MeshKernel &rclM) : _rclMesh(rclM) 
  { Rebuild(); }
  /// Destruction
  ~MeshRefPointToFacets (void)
  {
    for (std::vector<std::set<MeshFacetArray::_TConstIterator> >::iterator it = begin(); it != end(); ++it)
      it->clear();
    clear();
    std::vector<std::set<MeshFacetArray::_TConstIterator> >().swap(*this);
  }

  /// Rebuilds up data structure
  void Rebuild (void);
  void Neighbours (unsigned long ulFacetInd, float fMaxDist, std::vector<MeshFacetArray::_TConstIterator> &rclNb);

protected:
  void SearchNeighbours(MeshFacetArray::_TConstIterator pFIter, const Base::Vector3f &rclCenter, float fMaxDist, std::vector<MeshFacetArray::_TConstIterator> &rclNb);

protected:
  const MeshKernel  &_rclMesh; /**< The mesh kernel. */
};

/**
 * The MeshRefFacetToFacets builds up a structure to have access to all facets sharing 
 * at least one same point.
 * \note If the underlying mesh kernel gets changed this structure becomes invalid and must
 * be rebuilt.
 */
class AppMeshExport MeshRefFacetToFacets : public std::vector<std::set<MeshFacetArray::_TConstIterator> >
{
public:
  /// Construction
  MeshRefFacetToFacets (const MeshKernel &rclM) : _rclMesh(rclM)
  { Rebuild(); }
  /// Destruction
  ~MeshRefFacetToFacets (void)
  {
    for (std::vector<std::set<MeshFacetArray::_TConstIterator> >::iterator it = begin(); it != end(); ++it)
      it->clear();
    clear();
    std::vector<std::set<MeshFacetArray::_TConstIterator> >().swap(*this);
  }
  /// Rebuilds up data structure
  void Rebuild (void);

  /// Returns a set of facets sharing one or more points with the facet with index \a ulFacetIndex.
  const std::set<MeshFacetArray::_TConstIterator>& Neighbours (unsigned long ulFacetIndex) const
  { return operator[](ulFacetIndex); }

protected:
  const MeshKernel  &_rclMesh; /**< The mesh kernel. */
};

/**
 * The MeshRefPointToPoints builds up a structure to have access to all neighbour points  
 * of a point. Two points are neighbours if there is an edge indexing both points.
 * \note If the underlying mesh kernel gets changed this structure becomes invalid and must
 * be rebuilt.
 */
class AppMeshExport MeshRefPointToPoints : public std::vector<std::set<MeshPointArray::_TConstIterator> >
{
public:
  /// Construction
  MeshRefPointToPoints (const MeshKernel &rclM) : _rclMesh(rclM) 
  { Rebuild(); }
  /// Destruction
  ~MeshRefPointToPoints (void)
  {
    for (std::vector<std::set<MeshPointArray::_TConstIterator> >::iterator it = begin(); it != end(); ++it)
      it->clear();
    clear();
    std::vector<std::set<MeshPointArray::_TConstIterator> >().swap(*this);
  }

  /// Rebuilds up data structure
  void Rebuild (void);

protected:
  const MeshKernel  &_rclMesh; /**< The mesh kernel. */
};

class AppMeshExport MeshPolygonTriangulation
{
public:
  MeshPolygonTriangulation();
  MeshPolygonTriangulation(const std::vector<Base::Vector3f>& raclPoints);
  virtual ~MeshPolygonTriangulation();

	bool compute();
  void setPolygon(const std::vector<Base::Vector3f>& raclPoints);
  std::vector<MeshGeomFacet> getFacets(){ return _aclFacets;};

private:
  std::vector<Base::Vector3f>        _aclPoints;
  std::vector<MeshGeomFacet> _aclFacets;

	class Triangulate
	{
	public:
		// triangulate a contour/polygon, places results in STL vector
		// as series of triangles.indicating the points
    static bool Process(const std::vector<Base::Vector3f> &contour, std::vector<unsigned long> &result);

		// compute area of a contour/polygon
    static float Area(const std::vector<Base::Vector3f> &contour);

		// decide if point Px/Py is inside triangle defined by
		// (Ax,Ay) (Bx,By) (Cx,Cy)
		static bool InsideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py);

    static bool _invert;
	private:
    static bool Snip(const std::vector<Base::Vector3f> &contour,int u,int v,int w,int n,int *V);
	};
};


}; // namespace MeshCore 

#endif  // MESH_ALGORITHM_H 
