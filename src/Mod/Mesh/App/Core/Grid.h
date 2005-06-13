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


#ifndef MESH_GRID_H
#define MESH_GRID_H

#ifndef _PreComp_
# include <set>
#endif

#include "MeshKernel.h"
#include "Vector3D.h"

#define  MESH_CT_GRID          256     // Default value for number of elements per grid
#define  MESH_MAX_GRIDS        100000  // Default value for maximum number of grids
#define  MESH_CT_GRID_PER_AXIS 20


namespace Mesh {

class MeshKernel;
class BoundBox3D;
class Vector3D;
class MeshGeomFacet;
class MeshGrid;

//#define MESHGRID_BBOX_EXTENSION 1.0e-3f
#define MESHGRID_BBOX_EXTENSION 10.0f

/**
 * The MeshGrid allows to divide a global mesh object into smaller regions
 * of elements (e.g. facets, points or edges) depending on the resolution
 * of the grid. All grid elements in the grid structure have the same size.
 *
 * Grids can be used within algorithms to avoid to iterate through all elements,
 * so grids can speed up algorithms dramatically.
 */
class AppMeshExport MeshGrid
{
protected:
  /** @name Construction */
  //@{
  /// Construction
  MeshGrid (const MeshKernel &rclM);
  /// Construction
  MeshGrid (void);
  /// Destruction
  virtual ~MeshGrid (void) { }
  //@}

public:
  /** Attaches the mesh kernel to this grid, an already attached mesh gets detached. The grid gets rebuilt 
   * automatically. */
  virtual void Attach (const MeshKernel &rclM);
  /** Rebuilds the grid structure. */
  virtual void Rebuild (unsigned long ulPerGrid = MESH_CT_GRID, unsigned long ulMaxGrid = MESH_MAX_GRIDS);
  /** Rebuilds the grid structure. */
  virtual void Rebuild (int iCtGridPerAxis = MESH_CT_GRID_PER_AXIS);
  /** Rebuilds the grid structure. */
  virtual void Rebuild (unsigned long ulX, unsigned long ulY, unsigned long ulZ);
  
  /** @name Search */
  //@{
  /** Searches for elements lying in the intersection area of the grid and the bounding box. */
  virtual unsigned long InSide (const BoundBox3D &rclBB, std::vector<unsigned long> &raulElements, bool bDelDoubles = true) const;
  /** Searches for elements lying in the intersection area of the grid and the bounding box. */
  virtual unsigned long InSide (const BoundBox3D &rclBB, std::set<unsigned long> &raulElementss) const;
  /** Searches for elements lying in the intersection area of the grid and the bounding box. */
  virtual unsigned long InSide (const BoundBox3D &rclBB, std::vector<unsigned long> &raulElements, 
                                const Vector3D &rclOrg, float fMaxDist, bool bDelDoubles = true) const;
  /** Searches for the nearest grids that contain elements from a point, the result are grid indices. */
  void SearchNearestFromPoint (const Vector3D &rclPt, std::set<unsigned long> &rclInd) const;
  //@}

  /** Returns the lengths of the grid elements in x,y and z direction. */
  virtual void  GetGridLengths (float &rfLenX, float &rfLenY, float &rfLenZ) const
  { rfLenX = _fGridLenX; rfLenY = _fGridLenY; rfLenZ = _fGridLenZ; }
  /** Returns the number of grid elements in x,y and z direction. */
  virtual void GetCtGrids (unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const
  { rulX = _ulCtGridsX;  rulY = _ulCtGridsY;  rulZ = _ulCtGridsZ; }

  /** @name Boundings */
  //@{
  /** Returns the bounding box of a given grid element. */
  inline BoundBox3D  GetBoundBox (unsigned long ulX, unsigned long ulY, unsigned long ulZ) const;
  /** Returns the bounding box of the whole. */
  inline BoundBox3D  GetBoundBox     (void) const;
  /** Returns a extended bounding box of the mesh object. */
  inline BoundBox3D  GetMeshBoundBox (void) const;
  //@}
  /** Returns the number of elements in a given grid. */
  unsigned long GetCtElements(unsigned long ulX, unsigned long ulY, unsigned long ulZ) const
  { return _aulGrid[ulX][ulY][ulZ].size(); }
  /** Validates the grid structure and rebuilds it if needed. Must be implemented in sub-classes. */
  virtual void Validate (const MeshKernel &rclM) = 0;

protected:
  virtual void Position (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const;
  inline void Pos (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const;
  /** Checks if this is a valid grid position. */
  inline bool CheckPos (unsigned long ulX, unsigned long ulY, unsigned long ulZ) const;
  /** Initializes the size of the internal structure. */
  virtual void InitGrid (void);
  /** Deletes the grid structure. */
  virtual void Clear (void);
  /** Calculates the grid length dependent on maximum number of grids. */
  virtual void CalculateGridLength (unsigned long ulCtGrid, unsigned long ulMaxGrids);
  /** Calculates the grid length dependent on the number of grids per axis. */
  virtual void CalculateGridLength (int    iCtGridPerAxis);
  /** Rebuilds the grid structure. Must be implemented in sub-classes. */
  virtual void RebuildGrid (void) = 0;
  /** Returns the number of stored elements. Must be implemented in sub-classes. */
  virtual unsigned long HasElements (void) const = 0;
  void GetHull (unsigned long ulX, unsigned long ulY, unsigned long ulZ, unsigned long ulDistance, std::set<unsigned long> &raclInd) const;
  unsigned long AddElements (unsigned long ulX, unsigned long ulY, unsigned long ulZ,  std::set<unsigned long> &raclInd) const;

protected:
  std::vector<std::vector<std::vector<std::set<unsigned long> > > >  _aulGrid;   /**< Grid data structure. */
  const MeshKernel* _pclMesh;     /**< The mesh kernel. */
  unsigned long     _ulCtElements;/**< Number of grid elements for validation issues. */
  unsigned long     _ulCtGridsX;  /**< Number of grid elements in z. */
  unsigned long     _ulCtGridsY;  /**< Number of grid elements in z. */
  unsigned long     _ulCtGridsZ;  /**< Number of grid elements in z. */
  float             _fGridLenX;   /**< Length of grid elements in x. */
  float             _fGridLenY;   /**< Length of grid elements in y. */
  float             _fGridLenZ;   /**< Length of grid elements in z. */
  float             _fMinX;       /**< Grid null position in x. */
  float             _fMinY;       /**< Grid null position in y. */ 
  float             _fMinZ;       /**< Grid null position in z. */

  // friends
  friend class MeshGridIterator;
  friend class MeshGridIteratorStatistic;
};

/**
 * Special grid class that stores facet indices of the mesh object
 * in its grids.
 */
class AppMeshExport MeshFacetGrid: public MeshGrid
{
public:
  /** @name Construction */
  //@{
  /// Construction
  MeshFacetGrid (const MeshKernel &rclM);
  /// Construction
  MeshFacetGrid (void) : MeshGrid() { }
  /// Construction
  MeshFacetGrid (const MeshKernel &rclM, unsigned long ulX, unsigned long ulY, unsigned long ulZ);
  /// Construction
  MeshFacetGrid (const MeshKernel &rclM, int   iCtGridPerAxis);
  /// Construction
  MeshFacetGrid (const MeshKernel &rclM, float fGridLen);
  /// Destruction
  virtual ~MeshFacetGrid (void) { }
  //@}
 
  /** @name Search */
  //@{
  /** Searches for the nearest facet from a point. */
  unsigned long SearchNearestFromPoint (const Vector3D &rclPt) const;
  /** Searches for the nearest facet from a point with the maximum search area. */
  unsigned long SearchNearestFromPoint (const Vector3D &rclPt, float fMaxSearchArea) const;
  /** Searches for the nearest facet in a given grid element and returns the facet index and the actual distance. */
  void SearchNearestFacetInGrid(unsigned long ulX, unsigned long ulY, unsigned long ulZ, const Vector3D &rclPt, 
                                float &rfMinDist, unsigned long &rulFacetInd) const;
  /** Does basically the same as the method above unless that grid neighbours upto the order of \a ulDistance
   * are introduced into the search. */
  void SearchNearestFacetInHull (unsigned long ulX, unsigned long ulY, unsigned long ulZ, unsigned long ulDistance, 
                                 const Vector3D &rclPt, unsigned long &rulFacetInd, float &rfMinDist) const;
  //@}

  /** Validates the grid structure and rebuilds it if needed. */
  virtual void Validate (const MeshKernel &rclM);
  /** Validates the grid structure and rebuilds it if needed. */
  virtual void Validate (void);

protected:
  /** Returns the grid numbers to the given point \a rclPoint. */
  inline void Pos (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const;
  /** Returns the grid numbers to the given point \a rclPoint. */
  inline void PosWithCheck (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const;
  /** Adds a new facet element to the grid structure. \a rclFacet is the geometric facet and \a ulFacetIndex 
   * the corresponding index in the mesh kernel. */
  inline void Add (const MeshGeomFacet &rclFacet, unsigned long ulFacetIndex, float fEpsilon = 0.0f);
  /** Returns the number of stored elements. */
  unsigned long HasElements (void) const
  { return _pclMesh->CountFacets(); }
  /** Rebuilds the grid structure. */
  virtual void RebuildGrid (void);
};

/**
 * Special grid class that stores point indices of the mesh object
 * in its grids.
 */
class AppMeshExport MeshPointGrid: public MeshGrid
{
public:
  /** @name Construction */
  //@{
  /// Construction
  MeshPointGrid (void);
  /// Construction
  MeshPointGrid (const MeshKernel &rclM);
  /// Construction
  MeshPointGrid (const MeshKernel &rclM, int   iCtGridPerAxis);
  /// Construction
  MeshPointGrid (const MeshKernel &rclM, float fGridLen);
  /// Construction
  MeshPointGrid (const MeshKernel &rclM, unsigned long ulX, unsigned long ulY, unsigned long ulZ);
  /// Destruction
  virtual ~MeshPointGrid (void) {}
  //@}

  /** Finds all points that lie in the same grid as the point \a rclPoint. */
  unsigned long FindElements(const Vector3D &rclPoint, std::set<unsigned long>& aulElements) const;
  /** Validates the grid structure and rebuilds it if needed. */
  virtual void Validate (const MeshKernel &rclM);
  /** Validates the grid structure and rebuilds it if needed. */
  virtual void Validate (void);

protected:
  /** Adds a new point element to the grid structure. \a rclPt is the geometric point and \a ulPtIndex 
   * the corresponding index in the mesh kernel. */
  void AddPoint (const MeshPoint &rclPt, unsigned long ulPtIndex, float fEpsilon = 0.0f);
  /** Returns the grid numbers to the given point \a rclPoint. */
  void Pos(const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const;
  /** Returns the number of stored elements. */
  unsigned long HasElements (void) const
  { return _pclMesh->CountPoints(); }
  /** Rebuilds the grid structure. */
  virtual void RebuildGrid (void);
};

/**
 * The MeshGridIterator class provides an interface to walk through
 * all grid elements of a mesh grid.
 */
class AppMeshExport MeshGridIterator
{
public:
  /// Construction
  MeshGridIterator (const MeshGrid &rclG);
  /** Returns the bounding box of the current grid element. */
  BoundBox3D GetBoundBox (void) const
  { return _rclGrid.GetBoundBox(_ulX, _ulY, _ulZ); }
  /** Returns indices of the elements in the current grid. */
  void GetElements (std::vector<unsigned long> &raulElements) const
  {
    raulElements.insert(raulElements.end(), _rclGrid._aulGrid[_ulX][_ulY][_ulZ].begin(), _rclGrid._aulGrid[_ulX][_ulY][_ulZ].end());
  }
  /** @name Iteration */
  //@{
  /** Sets the iterator to the first element*/
  void  Init (void)
  { _ulX = _ulY = _ulZ = 0; }
  /** Checks if the iterator has not yet reached the end position. */  
  bool  More (void) const
  { return (_ulZ < _rclGrid._ulCtGridsZ); }
  /** Go to the next grid. */
  void  Next (void)
  {
    if (++_ulX >= (_rclGrid._ulCtGridsX)) _ulX = 0; else return;
    if (++_ulY >= (_rclGrid._ulCtGridsY)) { _ulY = 0; _ulZ++; } else return;
  }
  //@}

  /** @name Tests with rays */
  //@{
  /** Searches for facets around the ray. */
  bool InitOnRay (const Vector3D &rclPt, const Vector3D &rclDir, std::vector<unsigned long> &raulElements);
  /** Searches for facets around the ray. */
  bool InitOnRay (const Vector3D &rclPt, const Vector3D &rclDir, float fMaxSearchArea, std::vector<unsigned long> &raulElements);
  /** Searches for facets around the ray. */
  bool NextOnRay (std::vector<unsigned long> &raulElements);
  //@}
  
  /** Returns the grid number of the current position. */
  void GetGridPos (unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const
  { rulX = _ulX; rulY = _ulY; rulZ = _ulZ; }

protected:
  const MeshGrid& _rclGrid; /**< The mesh kernel. */
  unsigned long   _ulX;     /**< Number of grids in x. */
  unsigned long   _ulY;     /**< Number of grids in y. */
  unsigned long   _ulZ;     /**< Number of grids in z. */ 
  Vector3D        _clPt;    /**< Base point of search ray. */
  Vector3D        _clDir;   /**< Direction of search ray. */
  bool	          _bValidRay; /**< Search ray ok? */
  float	          _fMaxSearchArea;
};

// --------------------------------------------------------------

inline BoundBox3D  MeshGrid::GetBoundBox (unsigned long ulX, unsigned long ulY, unsigned long ulZ) const
{
  float fX, fY, fZ;
  
  fX = _fMinX + (float(ulX) * _fGridLenX);
  fY = _fMinY + (float(ulY) * _fGridLenY);
  fZ = _fMinZ + (float(ulZ) * _fGridLenZ);

  return BoundBox3D(fX, fY, fZ, fX + _fGridLenX, fY + _fGridLenY, fZ + _fGridLenZ);
}

inline BoundBox3D  MeshGrid::GetBoundBox (void) const
{
  return BoundBox3D(_fMinX, _fMinY, _fMinZ, _fMinX + (_fGridLenX * float(_ulCtGridsX)),
                    _fMinY + (_fGridLenY * float(_ulCtGridsY)), _fMinZ + (_fGridLenZ * float(_ulCtGridsZ)));
}

inline BoundBox3D  MeshGrid::GetMeshBoundBox (void) const
{
	BoundBox3D clBBenlarged = _pclMesh->GetBoundBox();
	clBBenlarged.Enlarge(MESHGRID_BBOX_EXTENSION);

	return clBBenlarged;
}

inline bool MeshGrid::CheckPos (unsigned long ulX, unsigned long ulY, unsigned long ulZ) const
{
  return ((ulX < _ulCtGridsX) && (ulY < _ulCtGridsY) && (ulZ < _ulCtGridsZ));
}

// --------------------------------------------------------------

// alle Facets die innerhalb der BB liegen
inline void MeshFacetGrid::Pos (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const
{
  rulX = (unsigned long)((rclPoint.x - _fMinX) / _fGridLenX);
  rulY = (unsigned long)((rclPoint.y - _fMinY) / _fGridLenY);
  rulZ = (unsigned long)((rclPoint.z - _fMinZ) / _fGridLenZ);

  assert((rulX < _ulCtGridsX) && (rulY < _ulCtGridsY) && (rulZ < _ulCtGridsZ));
}

// alle Facets die innerhalb der BB liegen
inline void MeshFacetGrid::PosWithCheck (const Vector3D &rclPoint, unsigned long &rulX, unsigned long &rulY, unsigned long &rulZ) const
{
	if ( rclPoint.x < _fMinX)
		rulX = 0;
	else
	{
		rulX = (unsigned long)((rclPoint.x - _fMinX) / _fGridLenX);
		if (rulX >= _ulCtGridsX)
			rulX = (_ulCtGridsX-1);
	}

	if ( rclPoint.y < _fMinY)
		rulY = 0;
	else
	{
		rulY = (unsigned long)((rclPoint.y - _fMinY) / _fGridLenY);
		if (rulY >= _ulCtGridsY)
			rulY = (_ulCtGridsY-1);
	}

	if ( rclPoint.z < _fMinZ)
		rulZ = 0;
	else
	{
		rulZ = (unsigned long)((rclPoint.z - _fMinZ) / _fGridLenZ);
		if (rulZ >= _ulCtGridsZ)
			rulZ = (_ulCtGridsZ-1);
	}

	assert((rulX < _ulCtGridsX) && (rulY < _ulCtGridsY) && (rulZ < _ulCtGridsZ));
}

/*alle Facets die innerhalb der BB liegen*/
inline void MeshFacetGrid::Add (const MeshGeomFacet &rclFacet, unsigned long ulFacetIndex, float fEpsilon)
{
#if 0
  unsigned long  i, ulX, ulY, ulZ, ulX1, ulY1, ulZ1, ulX2, ulY2, ulZ2;
  
  ulX1 = ulY1 = ulZ1 = ULONG_MAX;
  ulX2 = ulY2 = ulZ2 = 0;

  for (i = 0; i < 3; i++)
  {
    Pos(rclFacet._aclPoints[i], ulX, ulY, ulZ);
    _aulGrid[ulX][ulY][ulZ].insert(ulFacetIndex);
    ulX1 = RSmin<unsigned long>(ulX1, ulX); ulY1 = RSmin<unsigned long>(ulY1, ulY); ulZ1 = RSmin<unsigned long>(ulZ1, ulZ);
    ulX2 = RSmax<unsigned long>(ulX2, ulX); ulY2 = RSmax<unsigned long>(ulY2, ulY); ulZ2 = RSmax<unsigned long>(ulZ2, ulZ);
  }
  
  // falls Facet ueber mehrere BB reicht
  if ((ulX1 < ulX2) || (ulY1 < ulY2) || (ulZ1 < ulZ2))
  {
    for (ulX = ulX1; ulX <= ulX2; ulX++)
    {
      for (ulY = ulY1; ulY <= ulY2; ulY++)
      {
        for (ulZ = ulZ1; ulZ <= ulZ2; ulZ++)
        {
          if (CMeshFacetFunc::BBoxContainFacet(GetBoundBox(ulX, ulY, ulZ), rclFacet) == TRUE)
            _aulGrid[ulX][ulY][ulZ].insert(ulFacetIndex);
        }
      }
    }
  }
#else
  unsigned long ulX, ulY, ulZ;

  unsigned long ulX1, ulY1, ulZ1, ulX2, ulY2, ulZ2;

  BoundBox3D clBB;

  clBB &= rclFacet._aclPoints[0];
  clBB &= rclFacet._aclPoints[1];
  clBB &= rclFacet._aclPoints[2];

  //float fDiagonalLength = clBB.CalcDiagonalLength();

  //clBB.Enlarge(fEpsilon*fDiagonalLength);

  Pos(Vector3D(clBB.MinX,clBB.MinY,clBB.MinZ), ulX1, ulY1, ulZ1);
  Pos(Vector3D(clBB.MaxX,clBB.MaxY,clBB.MaxZ), ulX2, ulY2, ulZ2);
  
  /*
  if (ulX1 > 0) ulX1--;
  if (ulY1 > 0) ulY1--;
  if (ulZ1 > 0) ulZ1--;
  
  if (ulX2 < (_ulCtGridsX-1)) ulX2++;
  if (ulY2 < (_ulCtGridsY-1)) ulY2++;
  if (ulZ2 < (_ulCtGridsZ-1)) ulZ2++;
  */

  // falls Facet ueber mehrere BB reicht
  if ((ulX1 < ulX2) || (ulY1 < ulY2) || (ulZ1 < ulZ2))
  {
    for (ulX = ulX1; ulX <= ulX2; ulX++)
    {
      for (ulY = ulY1; ulY <= ulY2; ulY++)
      {
        for (ulZ = ulZ1; ulZ <= ulZ2; ulZ++)
        {
//          if (MeshFacetFunc::BBoxContainFacet(GetBoundBox(ulX, ulY, ulZ), rclFacet) == TRUE)
            _aulGrid[ulX][ulY][ulZ].insert(ulFacetIndex);
        }
      }
    }
  }
  else
    _aulGrid[ulX1][ulY1][ulZ1].insert(ulFacetIndex);

#endif
}

} // namespace Mesh

#endif // MESH_GRID_H