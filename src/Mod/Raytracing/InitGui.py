# Raytracing gui init module
# (c) 2003 Juergen Riegel
#
# Gathering all the information to start FreeCAD
# This is the second one of three init scripts, the third one
# runs when the gui is up

#***************************************************************************
#*   (c) Juergen Riegel (juergen.riegel@web.de) 2002                        *
#*                                                                         *
#*   This file is part of the FreeCAD CAx development system.              *
#*                                                                         *
#*   This program is free software; you can redistribute it and/or modify  *
#*   it under the terms of the GNU General Public License (GPL)            *
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



class RaytracingWorkbench ( Workbench ):
	"Raytracing workbench object"
	def Start(self):
		# load the module
		try:
			Log ('Loading RaytracingGui module')
			import RaytracingGui
		except:
			Err('   can not load RaytracingGui')
			raise
		else:	
			Log ('   Set up Raytracing toolbar...\n')
			list = ["Raytracing_WriteCamera"]
			Gui.ToolbarAppendItems("RaytracingTools", list, 0)

			Log ('   Set up Raytracing commandbar...\n')
			list = ["Raytracing_WriteCamera"]
			Gui.CommandbarAppendItems("RaytracingTools", list, 0)

			Log ('   Set up Raytracing menues...\n')

	def Stop(self):
		Log ('   Stoping Raytracing workbench...\n')
		Gui.CommandbarDelete("RaytracingTools")
		Gui.ToolbarDelete("RaytracingTools")


#Gui.WorkbenchAdd("Raytracing",RaytracingWorkbench())
Gui.WorkbenchAdd("Raytracing",RaytracingWorkbench())