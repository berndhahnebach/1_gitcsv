# FreeCAD init module
# (c) 2001 J�rgen Riegel
#
# Gathering all the information to start FreeCAD
# This is the second one of three init scripts, the third one
# runs when the gui is up

#***************************************************************************
#*   (c) J�rgen Riegel (juergen.riegel@web.de) 2002                        *   
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


# imports the one and only
import FreeCAD

try:
	import sys,os,dircache
except:
	FreeCAD.PrintError("Seams the python standard libs are not installed, bailing out!")
	raise

def InitDocumentation():
	# Checking on FreeCAD documentation path ++++++++++++++++++++++++++++++++++++++++++
	if os.path.isdir(FreeCAD.ConfigGet("HomePath")+'Doc'):
		if os.path.isdir(FreeCAD.ConfigGet("HomePath")+'Doc\\free-cad.sourceforge.net'):
			FreeCAD.ConfigSet("DocPath",FreeCAD.ConfigGet("HomePath")+'Doc\\free-cad.sourceforge.net')


def InitApplications():
	# Checking on FreeCAD Module path ++++++++++++++++++++++++++++++++++++++++++
	ModDir = '../src/Mod'
	#print FreeCAD.ConfigGet("HomePath")
	if os.path.isdir(FreeCAD.ConfigGet("HomePath")+'Mod'):
		ModDir = FreeCAD.ConfigGet("HomePath")+'Mod'
	else:
		if os.path.isdir(FreeCAD.ConfigGet("HomePath")+'src\\Mod'):
			ModDir = FreeCAD.ConfigGet("HomePath")+'src\\Mod'
	sys.path.append( '..\\bin' )
	sys.path.append( ModDir )
	Log("   Using "+ModDir+" as module path!\n")
	# Searching modules dirs +++++++++++++++++++++++++++++++++++++++++++++++++++
	ModDirs = dircache.listdir(ModDir)
	#print ModDirs
	Log('   Searching modules...\n')
	ModPar = App.ParamGet("System parameter:Modules")
	for Dir in ModDirs:
		if ( (Dir != 'CVS') & (Dir != '__init__.py')):
			Log('      Init: ' + Dir + '... ')
			ModGrp = ModPar.GetGroup(Dir)
			sys.path.append( os.path.join(ModDir,Dir) )
			InstallFile = os.path.join(os.path.join(ModDir,Dir),"Init.py")
			if ( os.path.exists(InstallFile) ):
				execfile(InstallFile)
				Log('done\n')
			else:
				Wrn("Init.py not found! "+Dir+" not initialized!\n")

#			else:
#				FreeCAD.PrintMessage('new! install it...\n')
#				InstallFile = os.path.join(os.path.join(ModDir,Dir),"Install.py")
#				if ( os.path.exists(InstallFile) ):
#					execfile(InstallFile)
#				else:
#					Wrn("         Install.py not found! "+Dir+" not installed!\n")


# some often used shortcuts (for lazy people like me ;-)
App = FreeCAD
Log = FreeCAD.PrintLog
Msg = FreeCAD.PrintMessage
Err = FreeCAD.PrintError
Wrn = FreeCAD.PrintWarning

Log ('\nFreeCAD init running....\n')

# init evera application by importing Init.py
InitApplications()

# set to no gui, is overwriten by InitGui
App.GuiUp = 0

Log ('\nFreeCAD init done\n')
   
    

