

# shell and operating system
import os,sys
#sys.path.append( "E:\\Develop\\Projekte\\FreeCADWin\\src\\Tools" )

import DistTools, FileTools

# line seperator 
ls = os.linesep
# path seperator
ps = os.pathsep
# dir seperator
ds = os.sep

DistName = DistTools.BuildDistName()

#DistSrc  = DistName + "_src"
#DistSrc  = DistName + "_src_bin_win"
#DistBin  = DistName + "_src_bin_win"
DistSrc  = DistName + "Bin"
DistBin  = DistName + "Bin"
DistDir  = "../../DistTemp/"

#====================================================================
# script asume to run in src/Tools

DistTools.EnsureDir(DistDir)
if (DistTools.EnsureDir(DistDir+DistBin) == 1):
    raise "Dist path already there!!"

#====================================================================
# copy src 
sys.stdout.write( 'Copy src Tree ...\n')
DistTools.EnsureDir(DistDir+DistSrc+'/src')
FileTools.cpallWithFilter('../../src',DistDir+DistSrc+'/src',FileTools.SetUpFilter(DistTools.SrcFilter))

#====================================================================
# copy bin and lib 
sys.stdout.write( 'Copy bin and lib Tree ...\n')
DistTools.EnsureDir(DistDir+DistBin+'/bin')
FileTools.cpallWithFilter('../../bin',DistDir+DistBin+'/bin',FileTools.SetUpFilter(DistTools.BinFilter))
DistTools.EnsureDir(DistDir+DistBin+'/lib')
FileTools.cpallWithFilter('../../lib',DistDir+DistBin+'/lib',FileTools.SetUpFilter(DistTools.LibFilter))

#====================================================================
# copy Modules
sys.stdout.write( 'Copy modul Tree ...\n')
DistTools.EnsureDir(DistDir+DistBin+'/Mod')
FileTools.cpallWithFilter('../../src/Mod',DistDir+DistBin+'/Mod',FileTools.SetUpFilter(DistTools.ModFilter))

#====================================================================
# copy top level files

#FileTools.cpfile("../Doc/README.html",DistDir+DistBin+"/README.html")
#FileTools.cpfile("../Doc/INSTALL.html",DistDir+DistBin+"/INSTALL.html")
#FileTools.cpfile("../Doc/LICENSE.GPL.html",DistDir+DistBin+"/LICENSE.GPL.html")
#FileTools.cpfile("../Doc/LICENSE.LGPL.html",DistDir+DistBin+"/LICENSE.LGPL.html")
#DistTools.cpfile("../Tools/BuildTool.py",DistDir+DistBin+"/BuildTool.py")

#====================================================================
# ziping a archiv
os.popen("rar.exe a "+DistDir+DistBin+".rar "+ DistDir+DistBin)

FileTools.rmall(DistDir+DistBin)