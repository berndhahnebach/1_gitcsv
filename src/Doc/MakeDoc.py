

# shell and operating system
import os,sys
sys.path.append( "E:\\Develop\\FreeCADWin\\src\\Tools" )

import FCFileTools

# line seperator 
ls = os.linesep
# path seperator
ps = os.pathsep
# dir seperator
ds = os.sep

#====================================================================
# script asume to run in src/Doc
os.chdir("e:/Develop/FreeCADWin/src/Doc")
LogFile = open("MakeDoc.log",'w')
if not os.path.isdir("../../Doc"):
    os.mkdir("../../Doc")
if not os.path.isdir("../../Doc/res"):
    os.mkdir("../../Doc/res")
FCFileTools.cpfile("index.html","../../doc/index.html")
FCFileTools.cpfile("FreeCAD.css","../../doc/res/FreeCAD.css")

#====================================================================
sys.stdout.write ('Running source documentation ...')
# running doxygen with the parameters from the config file
text = os.popen("doxygen doxygen.cfg").read()
LogFile.write(text)
if not os.path.isdir("../../Doc/FrameWork"):
    os.mkdir("../../Doc/FrameWork")

#====================================================================
sys.stdout.write( ' done\n  Generate HTML ...')
FCFileTools.cpall("html","../../doc/FrameWork")

#====================================================================
sys.stdout.write(' done\n  Generate DVI ...')
os.chdir("latex")
text = os.popen("latex refman.tex").read()
LogFile.write(text)
text = os.popen("makeindex refman.idx").read()
LogFile.write(text)
text = os.popen("latex refman.tex").read()
text = os.popen("latex refman.tex").read()
text = os.popen("latex refman.tex").read()
FCFileTools.cpfile("refman.dvi","../../../doc/FrameWork/FrameWork.dvi")

#====================================================================
sys.stdout.write (' done\n  Generate PS ...')
text = os.popen("dvips refman.dvi").read()
LogFile.write(text)
FCFileTools.cpfile("refman.ps","../../../doc/FrameWork/FrameWork.ps")

#====================================================================
sys.stdout.write (' done\n  Generate PDF ...')
text = os.popen("pdflatex refman.tex").read()
LogFile.write(text)
FCFileTools.cpfile("refman.pdf","../../../doc/FrameWork/FrameWork.pdf")
os.chdir("..")

#====================================================================
sys.stdout.write (' done\n  Clean up temporary files ...')
FCFileTools.rmall("html")
FCFileTools.rmall("latex")

#====================================================================
sys.stdout.write (' done\nCreating manuals\n')
if not os.path.isdir("../../Doc/Manuals"):
    os.mkdir("../../Doc/Manuals")
os.chdir("Manuals")

#====================================================================
sys.stdout.write('  Generate DVI ...')
text = os.popen("latex Design_Specification.tex").read()
LogFile.write(text)
text = os.popen("makeindex Design_Specification.idx").read()
LogFile.write(text)
text = os.popen("latex Design_Specification.tex").read()
text = os.popen("latex Design_Specification.tex").read()
text = os.popen("latex Design_Specification.tex").read()
FCFileTools.cpfile("Design_Specification.dvi","../../../doc/Manuals/Design_Specification.dvi")

text = os.popen("latex Manual.tex").read()
LogFile.write(text)
text = os.popen("makeindex Manual.idx").read()
LogFile.write(text)
text = os.popen("latex Manual.tex").read()
text = os.popen("latex Manual.tex").read()
text = os.popen("latex Manual.tex").read()
FCFileTools.cpfile("Manual.dvi","../../../doc/Manuals/Manual.dvi")

#====================================================================
sys.stdout.write (' done\n  Generate PS ...')
text = os.popen("dvips Design_Specification.dvi").read()
LogFile.write(text)
FCFileTools.cpfile("Design_Specification.ps","../../../doc/Manuals/Design_Specification.ps")
text = os.popen("dvips Manual.dvi").read()
LogFile.write(text)
FCFileTools.cpfile("Manual.ps","../../../doc/Manuals/Manual.ps")

#====================================================================
sys.stdout.write (' done\n  Generate PDF ...')
text = os.popen("pdflatex Design_Specification.tex").read()
LogFile.write(text)
FCFileTools.cpfile("Design_Specification.pdf","../../../doc/Manuals/Design_Specification.pdf")
text = os.popen("pdflatex Manual.tex").read()
LogFile.write(text)
FCFileTools.cpfile("Manual.pdf","../../../doc/Manuals/Manual.pdf")

#====================================================================
sys.stdout.write (' done\n  Clean up temporary files ...')
LogFile.close()

#====================================================================
sys.stdout.write (' done\nDocumentation done!\n')




#print text