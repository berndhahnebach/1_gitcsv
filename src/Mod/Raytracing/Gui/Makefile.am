if OCC_LIBRARY_FILES

lib_LTLIBRARIES=libRaytracingGui.la RaytracingGui.la

BUILT_SOURCES=\
		DlgSettingsRay.cpp \
		DlgSettingsRay.h \
		moc_DlgSettingsRay.cpp \
		moc_DlgSettingsRayImp.cpp \
		FreeCADpov.h \
		Raytracing_de.h

libRaytracingGui_la_SOURCES=\
		Command.cpp \
		DlgSettingsRayImp.cpp \
		DlgSettingsRayImp.h \
		PreCompiled.cpp \
		PreCompiled.h \
		Workbench.cpp \
		Workbench.h

# the library search path.
libRaytracingGui_la_LDFLAGS = -L../../../Base -L../../../App -L../../../Gui -L../App -L$(OCC_LIB) $(all_libraries) -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
libRaytracingGui_la_CPPFLAGS = -DAppPartExport= -DAppRaytracingGuiExport= -DFeatureRayExportPov=

libRaytracingGui_la_LIBADD   = \
		-lFreeCADGui \
		-lRaytracing

#--------------------------------------------------------------------------------------
# Loader of libRaytracingGui

RaytracingGui_la_SOURCES=\
		AppRaytracingGui.cpp

# the library search path.
RaytracingGui_la_LDFLAGS = $(libRaytracingGui_la_LDFLAGS) -module -avoid-version
RaytracingGui_la_CPPFLAGS = $(libRaytracingGui_la_CPPFLAGS)

RaytracingGui_la_LIBADD   = \
		-lRaytracingGui

#--------------------------------------------------------------------------------------

# rule for Qt MetaObject Compiler:
moc_%.cpp: %.h
	cd $(@D) && $(QT_MOC) $(<F) -o $(@F)

# rule for Qt MetaObject Compiler:
%.moc: %.h
	cd $(@D) && $(QT_MOC) $(<F) -o $(@F)

# rules for Qt User Interface Compiler:
# (it adds path to #include automatically, so cd before calling it)
%.h: %.ui
	cd $(@D) && $(QT_UIC) $(<F) -o $(@F)
%.cpp: %.h %.ui
	cd $(@D) && $(QT_UIC) -o $(@F) -impl $(*F).h $(*F).ui

# rule for translation files using python script :
%.h: %.ts
	$(PYTHON) ../../../Tools/qembed.py $(<F)  $(@F) $(*F)

FreeCADpov.h: FreeCADpov
	$(PYTHON) ../../../Tools/PythonToCPP.py $(<F) $@

# set the include path found by configure
INCLUDES= $(all_includes) $(QT_CXXFLAGS) -I$(OCC_INC) -I../../../ -I../App

endif

libdir = $(prefix)/Mod/Raytracing

CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST = \
		DlgSettingsRay.ui \
		FreeCADpov \
		Raytracing_de.ts \
		AppRaytracingGui.dsp \
		AppRaytracingGui.vcproj \
		Libs.cpp
