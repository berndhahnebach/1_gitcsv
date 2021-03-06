/***************************************************************************
 *   Copyright (c) 2006 Werner Mayer <werner.wm.mayer@gmx.de>              *
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
# include <BRep_Builder.hxx>
# include <BRepBuilderAPI_MakePolygon.hxx>
# include <gp_Pnt.hxx>
# include <TopoDS_Wire.hxx>
#endif

#include "FeaturePartPolygon.h"

using namespace Part;

PROPERTY_SOURCE(Part::Polygon, Part::Feature)


Polygon::Polygon()
{
  ADD_PROPERTY(Nodes,(Base::Vector3f()));
  ADD_PROPERTY(Close,(false));
}

Polygon::~Polygon()
{
}

int Polygon::execute(void)
{
  BRepBuilderAPI_MakePolygon poly;
  const std::vector<Base::Vector3f> nodes = Nodes.getValues();

  for ( std::vector<Base::Vector3f>::const_iterator it = nodes.begin(); it != nodes.end(); ++it ) {
    gp_Pnt pnt(it->x, it->y, it->z);
    poly.Add(pnt);
  }

  if ( Close.getValue() )
    poly.Close();

  TopoDS_Wire wire = poly.Wire();
  setShape(wire);

  return 0;
}

