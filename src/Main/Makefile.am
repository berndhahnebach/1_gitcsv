bin_PROGRAMS=FreeCAD FreeCADCmd
lib_LTLIBRARIES=FreeCAD.la

#--------------------------------------------------------------------------------------
FreeCAD_SOURCES= \
	MainGui.cpp

# the library search path.
FreeCAD_LDFLAGS = -L../Base -L../App -L../Gui $(all_libraries)

FreeCAD_LDADD=\
	-lFreeCADBase \
	-lFreeCADApp \
	-lFreeCADGui

#--------------------------------------------------------------------------------------
FreeCADCmd_SOURCES= \
	MainCmd.cpp

# the library search path.
FreeCADCmd_LDFLAGS = -L../Base -L../App $(all_libraries)

FreeCADCmd_LDADD=\
	-lFreeCADBase \
	-lFreeCADApp

#--------------------------------------------------------------------------------------
FreeCAD_la_SOURCES= \
	MainPy.cpp

# the library search path.
FreeCAD_la_LDFLAGS = -L../Base -L../App $(all_libraries) -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -module -avoid-version

FreeCAD_la_LIBADD=\
	-lFreeCADBase \
	-lFreeCADApp

#--------------------------------------------------------------------------------------

# set the include path found by configure
INCLUDES= $(all_includes) $(QT_CXXFLAGS)

EXTRA_DIST = \
		freecad.rc \
		FreeCADMain.dsp \
		FreeCADMain.vcproj \
		FreeCADMainCmd.dsp \
		FreeCADMainCmd.vcproj \
		FreeCADMainPy.dsp \
		FreeCADMainPy.vcproj \
		icon.ico
