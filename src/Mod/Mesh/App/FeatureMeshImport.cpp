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
#include "FeatureMeshImport.h"

#include "Core/MeshIO.h"


using namespace Mesh;
using namespace MeshCore;

void FeatureMeshImport::initFeature(void)
{
  addProperty("String","FileName");
}

int FeatureMeshImport::execute(TFunction_Logbook& log)
{

  std::string FileName =getPropertyString("FileName");

  // ask for read permisson
	if ( access(FileName.c_str(), 4) != 0 )
  {
    setError("FeatureMeshImport::Execute() not able to open %s!\n",FileName.c_str());
    return 1;
  }

  MeshSTL aReader(*(_cMesh.getKernel()) );

  // read file
  FileStream str( FileName.c_str(), std::ios::in);
  if ( !aReader.Load( str ) )
  {
    setError("FeatureMeshImport::Execute() not able import %s!\n",FileName.c_str());
    return 1;
  }

  return 0;
}

