# FreeCAD Part module  
# (c) 2001 Juergen Riegel
#
# Part design module

#***************************************************************************
#*   (c) Juergen Riegel (juergen.riegel@web.de) 2002                       *
#*                                                                         *
#*   This file is part of the FreeCAD CAx development system.              *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU Lesser General Public License (LGPL)    *
#*   as published by the Free Software Foundation; either version 2 of     *
#*   the License, or (at your option) any later version.                   *
#*   for detail see the LICENCE text file.                                 *
#*                                                                         *
#*   FreeCAD is distributed in the hope that it will be useful,            *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
#*   GNU Library General Public License for more details.                  *
#*                                                                         *
#*   You should have received a copy of the GNU Library General Public     *
#*   License along with FreeCAD; if not, write to the Free Software        *
#*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  *
#*   USA                                                                   *
#*                                                                         *
#*   Juergen Riegel 2002                                                   *
#***************************************************************************/

# import FreeCAD modules
import FreeCAD,FreeCADGui
# import the App Test module
import TestApp               #Test as Module name not possible

#---------------------------------------------------------------------------
# define the Commands of the Test Application module
#---------------------------------------------------------------------------
class TestCmd:
    "Test1 commando object"
    def Activated(self):
        TestApp.TestGui("TestApp.All")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1', 'MenuText': 'Test 1', 'ToolTip': 'First very important Test!!!'}

class TestAllCmd:
    "Test all commando object"
    def Activated(self):
        TestApp.TestGui("TestApp.All")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1', 'MenuText': 'Test all', 'ToolTip': 'Runs all tests at once (can take very long!)'}

class TestDocCmd:
    "Document test commando object"
    def Activated(self):
        TestApp.TestGui("Document")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Test Document',
                'ToolTip' : 'Test the document (creation, save, load and destruction)'}

class TestBaseCmd:
    "Base test commando object"
    def Activated(self):
        TestApp.TestGui("Base")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Test base',
                'ToolTip' : 'Test the basic functions of FreeCAD'}

class TestAllTextCmd:
    "Test all commando object"
    def Activated(self):
        TestApp.TestText("TestApp.All")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1', 'MenuText': 'Test all', 'ToolTip': 'Runs all tests at once (can take very long!)'}

class TestDocTextCmd:
    "Document test commando object"
    def Activated(self):
        TestApp.TestText("Document")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Test Document',
                'ToolTip' : 'Test the document (creation, save, load and destruction)'}

class TestBaseTextCmd:
    "Base test commando object"
    def Activated(self):
        TestApp.TestText("Base")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Test base',
                'ToolTip' : 'Test the basic functions of FreeCAD'}





class TestWorkbenchCmd:
    "Workbench test"
    def Activated(self):
    	i=0
    	while (i<20):
	        FreeCADGui.ActivateWorkbench("Import")
	        FreeCADGui.UpdateGui()
	        FreeCADGui.ActivateWorkbench("<none>")
	        FreeCADGui.UpdateGui()
	        FreeCADGui.ActivateWorkbench("Part design")
	        FreeCADGui.UpdateGui()
	        FreeCADGui.ActivateWorkbench("Sketcher")
	        FreeCADGui.UpdateGui()
	        print i
	        i=i+1

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Test workbench',
                'ToolTip' : 'Test the switching of workbenches in FreeCAD'}

class TestCreateMenuCmd:
    "Base test commando object"
    def Activated(self):
        TestApp.TestGui("Menu.MenuCreateCases")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Add menu',
                'ToolTip' : 'Test the menu stuff of FreeCAD'}

class TestDeleteMenuCmd:
    "Base test commando object"
    def Activated(self):
        TestApp.TestGui("Menu.MenuDeleteCases")

    def GetResources(self):
        return {'Pixmap'  : 'Std_Tool1',
        		'MenuText': 'Remove menu',
                'ToolTip' : 'Test the menu stuff of FreeCAD'}

#---------------------------------------------------------------------------
# Adds the commands to the FreeCAD command manager
#---------------------------------------------------------------------------
FreeCADGui.AddCommand('Test_Test'        ,TestCmd())
FreeCADGui.AddCommand('Test_TestAllText' ,TestAllTextCmd())
FreeCADGui.AddCommand('Test_TestDocText' ,TestDocTextCmd())
FreeCADGui.AddCommand('Test_TestBaseText',TestBaseTextCmd())
FreeCADGui.AddCommand('Test_TestAll'     ,TestAllCmd())
FreeCADGui.AddCommand('Test_TestDoc'     ,TestDocCmd())
FreeCADGui.AddCommand('Test_TestBase'    ,TestBaseCmd())
FreeCADGui.AddCommand('Test_TestWork'    ,TestWorkbenchCmd())
FreeCADGui.AddCommand('Test_TestCreateMenu'    ,TestCreateMenuCmd())
FreeCADGui.AddCommand('Test_TestDeleteMenu'    ,TestDeleteMenuCmd())

