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
# include <fcntl.h>
# include <TFunction_Logbook.hxx>
# include <ios>
#endif

#include <Base/Console.h>
#include <Base/Exception.h>
#include <Base/Sequencer.h>

#include "Core/Algorithm.h"
#include "Core/Evaluation.h"
#include "Core/Iterator.h"
#include "Core/Visitor.h"

#include "Core/SetOperations.h"

#include "FeatureMeshSetOperations.h"


using namespace Mesh;
using namespace std;

PROPERTY_SOURCE(Mesh::SetOperations, Mesh::Feature)


SetOperations::SetOperations(void)
{
  ADD_PROPERTY(Source1  ,(0));
  ADD_PROPERTY(Source2  ,(0));
 
}

int SetOperations::execute(void)
{
  Mesh::Feature *mesh1  = dynamic_cast<Mesh::Feature*>(Source1.getValue());
  Mesh::Feature *mesh2  = dynamic_cast<Mesh::Feature*>(Source2.getValue());

  if ((mesh1 != NULL) && (mesh2 != NULL))
  {
    MeshWithProperty& propMesh1 = mesh1->getMesh();
    MeshWithProperty& propMesh2 = mesh2->getMesh();

    MeshKernel& meshKernel1 = *propMesh1.getKernel();
    MeshKernel& meshKernel2 = *propMesh2.getKernel();

    MeshCore::MeshKernel* res = getMesh().getKernel(); // Result Meshkernel

    MeshCore::SetOperations setOp(meshKernel1, meshKernel2, *res, MeshCore::SetOperations::Union, 1.0e-5);
    setOp.Do();


  }
  else
  { // Error mesh property
  }

    

  

  return 0;
}
