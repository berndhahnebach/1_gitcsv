
pkglib_LTLIBRARIES=libMeshGui.la

# BUILT_SOURCES=\
#
libMeshGui_la_SOURCES=\
	AppMeshGui.cpp \
	Command.cpp

# the library search path.
libMeshGui_la_LDFLAGS = -L../App $(all_libraries)

libMeshGui_la_LIBADD   = \
		-lFreeCADGui \
		-lMesh

# rule for Qt MetaObject Compiler:
moc_%.cpp: %.h
	cd $(@D) && $(MOC) $(<F) -o $(@F)

# rule for Qt MetaObject Compiler:
%.moc: %.h
	cd $(@D) && $(MOC) $(<F) -o $(@F)

# rules for Qt User Interface Compiler:
# (it adds path to #include automatically, so cd before calling it)
%.h: %.ui
	cd $(@D) && $(UIC) $(<F) -o $(@F)
%.cpp: %.h %.ui
	cd $(@D) && $(UIC) -o $(@F) -impl $(*F).h $(*F).ui

# set the include path found by configure
INCLUDES= $(all_includes) -I../../../ -I../App