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


#include "PreCompiled.h"

#ifndef _PreComp_
#endif

#include <Base/Sequencer.h>
#include <Base/Exception.h>

#include "Builder.h"
#include "MeshKernel.h"

using namespace MeshCore;


MeshBuilder::MeshBuilder (MeshKernel& kernel) : _meshKernel(kernel)
{
}

MeshBuilder::~MeshBuilder (void)
{
}

void MeshBuilder::Initialize (unsigned long ctFacets, bool deletion)
{
  if (deletion)
  {
    // Clear the mesh structure and free all memory
    _meshKernel.Clear();
    
    // Allocate new memory that is needed later on. If AddFacet() gets called exactly ctFacets times there is no wastage of memory
    // otherwise the vector reallocates ~50% of its future memory usage.
    // Note: A feature of the std::vector implementation is that it can hold more memory (capacity) than it actually needs (size).
    //       This usually happens if its elements are added without specifying its final size. Later on it's a bit tricky to free the wasted 
    //       memory. So we're strived to avoid the wastage of memory.
    _meshKernel._aclFacetArray.reserve(ctFacets);

    // Usually the number of vertices is the half of the number of facets. So we reserve this memory with 10% surcharge
    // To save memory we hold an array with iterators that point to the right vertex (insertion order) in the set, instead of
    // holding the vertex array twice.
    unsigned long ctPoints = ctFacets / 2;
    _pointsIterator.reserve((unsigned long)(float(ctPoints)*1.10f));
	  _ptIdx = 0;
  }
  else
  {
    for (MeshPointArray::_TConstIterator it1 = _meshKernel._aclPointArray.begin(); it1 != _meshKernel._aclPointArray.end(); it1++)
    {
			MeshPointIterator pit = _points.insert(*it1);			
      _pointsIterator.push_back(pit);
    }
    _ptIdx = _points.size();

    // As we have a copy of our vertices in the set we must clear them from our array now  But we can keep its
    // memory as we reuse it later on anyway.
    _meshKernel._aclPointArray.clear();
    // additional memory
    unsigned long newCtFacets = _meshKernel._aclFacetArray.size()+ctFacets;
    _meshKernel._aclFacetArray.reserve(newCtFacets);
    unsigned long ctPoints = newCtFacets / 2;
    _pointsIterator.reserve((unsigned long)(float(ctPoints)*1.10f));
  }

	Base::Sequencer().start("create mesh structure...", ctFacets * 2);
}

void MeshBuilder::AddFacet (const MeshGeomFacet& facet, bool takeFlag, bool takeProperty)
{
  unsigned char flag = 0;
  unsigned long prop = 0;
  if (takeFlag)
    flag = facet._ucFlag;
  if (takeProperty)
    prop = facet._ulProp;

  AddFacet(facet._aclPoints[0], facet._aclPoints[1], facet._aclPoints[2], facet.GetNormal(), flag, prop);
}

void MeshBuilder::AddFacet (const Vector3D& pt1, const Vector3D& pt2, const Vector3D& pt3, const Vector3D& normal, unsigned char flag, unsigned long prop)
{
	Vector3D facetPoints[4] = { pt1, pt2, pt3, normal };
	AddFacet(facetPoints, flag, prop);
}

void MeshBuilder::AddFacet (Vector3D* facetPoints, unsigned char flag, unsigned long prop)
{
	Base::Sequencer().next(true); // allow to cancel

	// adjust circulation direction
  if ((((facetPoints[1] - facetPoints[0]) % (facetPoints[2] - facetPoints[0])) * facetPoints[3]) < 0.0f)
	{		
		std::swap(facetPoints[1], facetPoints[2]);
	}

	MeshFacet mf;
  mf._ucFlag = flag;
  mf._ulProp = prop;

  int i = 0;
	for (i = 0; i < 3; i++)
	{
		MeshPoint pt(facetPoints[i]);			
		std::set<MeshPoint>::iterator p = _points.find(pt);
		if (p == _points.end())
		{
			mf._aulPoints[i] = _ptIdx;
      pt._ulProp = _ptIdx++;
      // keep an iterator to the right vertex
			MeshPointIterator it = _points.insert(pt);			
      _pointsIterator.push_back(it);
		}
		else
			mf._aulPoints[i] = p->_ulProp;
	}		

  // check for degenerated facet (one edge has length 0)
  if ((mf._aulPoints[0] == mf._aulPoints[1]) || (mf._aulPoints[0] == mf._aulPoints[2]) || (mf._aulPoints[1] == mf._aulPoints[2]))
    return;

  _meshKernel._aclFacetArray.push_back(mf);
}

void MeshBuilder::SetNeighbourhood ()
{
  std::set<Edge> edges;

  int facetIdx = 0;

  for (MeshFacetArray::_TIterator it = _meshKernel._aclFacetArray.begin(); it != _meshKernel._aclFacetArray.end(); it++)
  {
  	Base::Sequencer().next(true); // allow to cancel

    MeshFacet& mf = *it;

	  for (int i = 0; i < 3; i++)
	  {
		  Edge edge(mf._aulPoints[i], mf._aulPoints[(i+1)%3], facetIdx);

		  std::set<Edge>::iterator e = edges.find(edge);
		  if (e != edges.end())
		  { // edge exists, set neighbourhood
        MeshFacet& mf1 = _meshKernel._aclFacetArray[e->facetIdx];

        if (mf1._aulPoints[0] == edge.pt1)
        {
          if (mf1._aulPoints[1] == edge.pt2)
            mf1._aulNeighbours[0] = facetIdx;
          else
            mf1._aulNeighbours[2] = facetIdx;
        }
        else if (mf1._aulPoints[0] == edge.pt2)
        {
          if (mf1._aulPoints[1] == edge.pt1)
            mf1._aulNeighbours[0] = facetIdx;
          else
            mf1._aulNeighbours[2] = facetIdx;
        }
        else
          mf1._aulNeighbours[1] = facetIdx;

        mf._aulNeighbours[i] = e->facetIdx;
		  }
      else
		  {  // new edge
			  edges.insert(edge);
		  }
	  }

    facetIdx++;
  }
}

void MeshBuilder::RemoveUnreferencedPoints()
{
  _meshKernel._aclPointArray.SetFlag(MeshPoint::INVALID);
  for ( MeshFacetArray::_TConstIterator it = _meshKernel._aclFacetArray.begin(); it != _meshKernel._aclFacetArray.end(); ++it )
  {
    for ( int i=0; i<3; i++ )
     _meshKernel._aclPointArray[it->_aulPoints[i]].ResetInvalid();
  }

  unsigned long uValidPts = std::count_if(_meshKernel._aclPointArray.begin(), _meshKernel._aclPointArray.end(), std::mem_fun_ref(&MeshPoint::IsValid));
  if ( uValidPts < _meshKernel.CountPoints() )
    _meshKernel.RemoveInvalids();
}

void MeshBuilder::Finish (bool freeMemory)
{
  // now we can resize the vertex array to the exact size and copy the vertices with their correct positions in the array
  unsigned long i=0;
  _meshKernel._aclPointArray.resize(_pointsIterator.size());
  for ( std::vector<MeshPointIterator>::iterator it = _pointsIterator.begin(); it != _pointsIterator.end(); ++it)
    _meshKernel._aclPointArray[i++] = *(it->first);

  // free all memory of the internal structures
  // Note: this scope is needed to free memory immediately
  { std::vector<MeshPointIterator>().swap(_pointsIterator); }
  _points.clear();
//  _points.swap(_points);

  SetNeighbourhood();
  RemoveUnreferencedPoints();

  // if AddFacet() has been called more often (or even less) as specified in Initialize() we have a wastage of memory
  if ( freeMemory )
  {
    unsigned long cap = _meshKernel._aclFacetArray.capacity();
    unsigned long siz = _meshKernel._aclFacetArray.size();
    // wastage of more than 5%
    if ( cap > siz+siz/20 )
    {
      try {
        unsigned long i=0;
        MeshFacetArray faces(siz);
        for ( MeshFacetArray::_TIterator it = _meshKernel._aclFacetArray.begin(); it != _meshKernel._aclFacetArray.end(); ++it )
          faces[i++]=*it;
        _meshKernel._aclFacetArray.swap(faces);
      } catch ( const Base::MemoryException&) {
        // sorry, we cannot reduce the memory
      }
    }
  }

  // FIXME: Does this work for MS compiler version 8 or higher? At least, with version 6 and STLport this has no effect.
  // release some memory
//  _meshKernel._aclFacetArray.swap(_meshKernel._aclFacetArray);
//  _meshKernel._aclPointArray.swap(_meshKernel._aclPointArray);

  _meshKernel.RecalcBoundBox();

	Base::Sequencer().stop();
}
