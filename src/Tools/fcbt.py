#!python
#  FreeCAD Build Tool
# (c) 2004 Juergen Riegel


import os,sys,string

help1 = """
FreeCAD Build Tool
Usage:
   fcbt <module name> [module parameter]
possible modules are:
 - DistSrc         (DS)   Build a source Distr. of the aktuall source tree
 - DistBin         (DB)   Build a binary Distr. of the aktuall source tree
 - DistSetup       (DI)   Build a Setup Distr. of the aktuall source tree
 - DistAll         (DA)   Run all three above modules
 - BuildDoc        (BD)   Create the documentation (source docs)
 - NextBuildNumber (NBN)  Increase the Build Number of this Version 
 - CreateModule    (CM)   Insert a new FreeCAD Module in the module directory

For help on the modules type:
  fcbt <module name> ?

Input Command:
"""

if(len(sys.argv) < 2):
	sys.stdout.write(help1)
	CmdRaw = sys.stdin.readline()[:-1]
else:
	CmdRaw = sys.argv[1]
	
Cmd = string.lower(CmdRaw)


if Cmd == "distsrc" or Cmd == "ds" :
	import fcbt.DistSrc
elif Cmd == "distbin" or Cmd == "db":
	import fcbt.DistBin
elif Cmd == "distsetup" or Cmd == "di":
	import fcbt.DistSetup
elif Cmd == "distall" or Cmd == "da":
	import fcbt.DistSrc
	import fcbt.DistBin
	import fcbt.DistSetup
elif Cmd == "builddoc" or Cmd == "bd":
	import fcbt.BuildDoc
elif Cmd == "nextbuildnumber" or Cmd == "nbn":
	import fcbt.NextBuildNumber
else:
	print CmdRaw + " is an unknown command!\n"
	sys.exit(1)

	
	

	