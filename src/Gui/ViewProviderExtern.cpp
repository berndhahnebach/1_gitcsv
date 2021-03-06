/***************************************************************************
 *   Copyright (c) 2004 J�rgen Riegel <juergen.riegel@web.de>              *
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
# include <qlistview.h>
# include <Inventor/nodes/SoMaterial.h>
# include <Inventor/nodes/SoSeparator.h>
# include <Inventor/nodes/SoSwitch.h>
#endif

/// Here the FreeCAD includes sorted by Base,App,Gui......
#include <Base/Console.h>
#include <Base/Exception.h>
#include <App/Material.h>
#include <App/Feature.h>
#include "Selection.h"
#include "ViewProviderExtern.h"
#include "Tree.h"

using std::vector;
using std::string;


using namespace Gui;

PROPERTY_SOURCE(Gui::ViewProviderExtern, Gui::ViewProvider)

      
ViewProviderExtern::ViewProviderExtern()
{

}


ViewProviderExtern::~ViewProviderExtern()
{

}


void ViewProviderExtern::setModeByString(const char* name, const char* ivFragment)
{
  SoInput in;
  in.setBuffer((void*)ivFragment,strlen(ivFragment));

  setModeBySoInput(name,in);

}


void ViewProviderExtern::setModeByFile(const char* name, const char* ivFileName)
{
  SoInput in;
  if (in.openFile(ivFileName)) 
  {
     setModeBySoInput(name,in);
  }
}


void ViewProviderExtern::setModeBySoInput(const char* name, SoInput &ivFileInput)
{
  SoSeparator * root = SoDB::readAll(&ivFileInput);
  if (root) {

    vector<string>::iterator pos = std::find<vector<string>::iterator,string>(modes.begin(),modes.end(),string(name));

    if(pos == modes.end())
    { // new mode
      modes.push_back(name);
      addDisplayMode(root, name);
      setDisplayMode(name);
    }else
    { // existing mode
      // not implemented yet
      assert(0);
      root->unref();
    }
  }else
    throw Base::Exception("nothing read");
  return;
}



std::vector<std::string> ViewProviderExtern::getModes(void)
{
  // empty
  return modes;
}

void ViewProviderExtern::setTransparency(float trans)
{

}

void ViewProviderExtern::setColor(const App::Color &c)
{
  
}



