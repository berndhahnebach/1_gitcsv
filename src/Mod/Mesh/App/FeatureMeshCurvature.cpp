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


#include "PreCompiled.h"
#ifndef _PreComp_
# include <Wm3Vector3.h>
# include <Wm3MeshCurvature.h>
#endif

#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/Matrix.h>
#include <Base/Sequencer.h>
#include "FeatureMeshCurvature.h"

#include "Core/Elements.h"
#include "Core/Iterator.h"



using namespace Mesh;
using namespace MeshCore;

PROPERTY_SOURCE(Mesh::Curvature, Mesh::Feature)



Curvature::Curvature(void)
{
  ADD_PROPERTY(Source,(0));
  ADD_PROPERTY(CurvInfo, (CurvatureInfo()));
}

int Curvature::execute(void)
{
  Feature *pcFeat  = dynamic_cast<Feature*>(Source.getValue());
  if(!pcFeat || pcFeat->getStatus() != Valid)
  {
    setError("No mesh object attached.");
    return 1;
  }
 
  // get all points
  const MeshKernel& rMesh = pcFeat->getMesh();
  std::vector< Wm3::Vector3<float> > aPnts;
  MeshPointIterator cPIt( rMesh );
  for ( cPIt.Init(); cPIt.More(); cPIt.Next() )
  {
    Wm3::Vector3<float> cP( cPIt->x, cPIt->y, cPIt->z );
    aPnts.push_back( cP );
  }

  // get all point connections
  std::vector<int> aIdx;
  const std::vector<MeshFacet>& raFts = rMesh.GetFacets();
  for ( std::vector<MeshFacet>::const_iterator jt = raFts.begin(); jt != raFts.end(); ++jt )
  {
    for (int i=0; i<3; i++)
    {
      aIdx.push_back( (int)jt->_aulPoints[i] );
    }
  }

  // compute vertex based curvatures
  Wm3::MeshCurvature<float> meshCurv(rMesh.CountPoints(), &(aPnts[0]), rMesh.CountFacets(), &(aIdx[0]));

  // get curvature information now
  const Wm3::Vector3<float>* aMaxCurvDir = meshCurv.GetMaxDirections();
  const Wm3::Vector3<float>* aMinCurvDir = meshCurv.GetMinDirections();
  const float* aMaxCurv = meshCurv.GetMaxCurvatures();
  const float* aMinCurv = meshCurv.GetMinCurvatures();

  CurvInfo.setSize(rMesh.CountPoints());
  for ( unsigned long i=0; i<rMesh.CountPoints(); i++ )
  {
    CurvatureInfo ci;
    ci.cMaxCurvDir = Base::Vector3f( aMaxCurvDir[i].X(), aMaxCurvDir[i].Y(), aMaxCurvDir[i].Z() );
    ci.cMinCurvDir = Base::Vector3f( aMinCurvDir[i].X(), aMinCurvDir[i].Y(), aMinCurvDir[i].Z() );
    ci.fMaxCurvature = aMaxCurv[i];
    ci.fMinCurvature = aMinCurv[i];
    CurvInfo.set1Value(i, ci);
  }

  return 0;
}

const MeshCore::MeshKernel& Curvature::getMesh() const
{
  Mesh::Feature *pcFeat  = dynamic_cast<Mesh::Feature*>(Source.getValue() );
  if ( pcFeat )
    return pcFeat->getMesh();
  else
    return Mesh.getValue();
}
