if OCC_LIBRARY_FILES

lib_LTLIBRARIES=libRaytracing.la Raytracing.la

BUILT_SOURCES=\
		FreeCADpov.h

libRaytracing_la_SOURCES=\
		AppRaytracingPy.cpp \
		PovTools.cpp \
		PovTools.h \
		PreCompiled.cpp \
		PreCompiled.h \
		RayFeature.cpp \
		RayFeature.h


FreeCADpov.h: FreeCADpov
	$(PYTHON) $(top_srcdir)/Tools/PythonToCPP.py $(<F) $@


# the library search path.
libRaytracing_la_LDFLAGS = -L../../../Base -L../../../App -L../../Part/App -L$(OCC_LIB) $(all_libraries) -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
libRaytracing_la_CPPFLAGS = -DAppPartExport= -DAppRaytracingExport= -DFeatureRayExportPov=

libRaytracing_la_LIBADD   = \
		-lFreeCADBase \
		-lFreeCADApp \
		-lPart

#--------------------------------------------------------------------------------------
# Loader of libRaytracing

Raytracing_la_SOURCES=\
		AppRaytracing.cpp

# the library search path.
Raytracing_la_LDFLAGS = $(libRaytracing_la_LDFLAGS) -module -avoid-version
Raytracing_la_CPPFLAGS = $(libRaytracing_la_CPPFLAGS)

Raytracing_la_LIBADD   = \
		-lRaytracing

#--------------------------------------------------------------------------------------

# set the include path found by configure
INCLUDES= $(all_includes) -I$(OCC_INC) -I../../../

endif

libdir = $(prefix)/Mod/Raytracing

CLEANFILES = $(BUILT_SOURCES)

EXTRA_DIST = \
		resources/FCAnimation.inc \
		resources/FCAnimation.ini \
		resources/FCAnimation.pov \
		resources/FCSimple.pov \
		AppRaytracing.dsp \
		AppRaytracing.vcproj \
		FreeCADpov \
		Libs.cpp
