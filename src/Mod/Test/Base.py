#***************************************************************************
#*   (c) Juergen Riegel (juergen.riegel@web.de) 2004                       *   
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
#*   Juergen Riegel 2004                                                   *
#***************************************************************************/

import FreeCAD, os, unittest

class ConsoleTestCase(unittest.TestCase):
    def testPrint(self):
        FreeCAD.PrintMessage("   Printing message\n")
        FreeCAD.PrintError("   Printing error\n")
        FreeCAD.PrintWarning("   Printing warning\n")
        FreeCAD.PrintLog("   Printing Log\n")


class ParameterTestCase(unittest.TestCase):
    def setUp(self):
        self.TestPar = FreeCAD.ParamGet("System parameter:Test")
        
    def testGroup(self):
        #FreeCAD.PrintLog("Base::ParameterTestCase::testGroup\n")
        # check on Group creation
        Temp = self.TestPar.GetGroup("44")
        self.failUnless(self.TestPar.HasGroup("44"),"Test on created group failed")
        # check on Deletion
        self.TestPar.RemGroup("44")
        self.failUnless(not self.TestPar.HasGroup("44"),"Test on delete group failed")
        Temp =0

        #check on special conditions
    def testInt(self):
        #FreeCAD.PrintLog("Base::ParameterTestCase::testInt\n")
        #Temp = FreeCAD.ParamGet("System parameter:Test/44")
        # check on Int
        self.TestPar.SetInt("44",4711)
        self.failUnless(self.TestPar.GetInt("44") == 4711,"In and out error at Int")
        # check on Deletion
        self.TestPar.RemInt("44")
        self.failUnless(self.TestPar.GetInt("44",1) == 1,"Deletion error at Int")
        

    def testBool(self):
        #FreeCAD.PrintLog("Base::ParameterTestCase::testBool\n")
        # check on Int
        self.TestPar.SetBool("44",1)
        self.failUnless(self.TestPar.GetBool("44") == 1,"In and out error at Bool")
        # check on Deletion
        self.TestPar.RemBool("44")
        self.failUnless(self.TestPar.GetBool("44",0) == 0,"Deletion error at Bool")

    def testFloat(self):
        #FreeCAD.PrintLog("Base::ParameterTestCase::testFloat\n")
        #Temp = FreeCAD.ParamGet("System parameter:Test/44")
        # check on Int
        self.TestPar.SetFloat("44",4711.4711)
        self.failUnless(self.TestPar.GetFloat("44") == 4711.4711,"In and out error at Float")
        # check on Deletion
        self.TestPar.RemFloat("44")
        self.failUnless(self.TestPar.GetFloat("44",1.1) == 1.1,"Deletion error at Float")

    def testNesting(self):
        # Parameter testing
        #FreeCAD.PrintLog("Base::ParameterTestCase::testNesting\n")
        for i in range(50):
            self.TestPar.SetFloat(`i`,4711.4711)
            self.TestPar.SetInt(`i`,4711)
            self.TestPar.SetBool(`i`,1)
            Temp = self.TestPar.GetGroup(`i`)
            for l in range(50):
                Temp.SetFloat(`l`,4711.4711)
                Temp.SetInt(`l`,4711)
                Temp.SetBool(`l`,1)
        Temp = 0
        
    def tearDown(self):
        #remove all
        TestPar = FreeCAD.ParamGet("System parameter:Test")
        TestPar.Clear()