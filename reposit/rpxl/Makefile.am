
#SUBDIRS = conversions functions rpxll rpxllib register utilities

EXTRA_DIST = \
    callingrange.cpp \
    callingrange.hpp \
    configuration.cpp \
    configuration.hpp \
    convert_oper.cpp \
    convert_oper.hpp \
    functioncall.cpp \
    functioncall.hpp \
    loop.hpp \
    objectwrapperxl.hpp \
    objectwrapperxl.cpp \
    rangereference.cpp \
    rangereference.hpp \
    repositoryxl.cpp \
    repositoryxl.hpp \
    repositxl.hpp \
    rpxldefines.hpp \
    xloper.hpp
 
dist-hook:
	mkdir -p $(distdir)/conversions  $(distdir)/functions $(distdir)/rpxll $(distdir)/rpxllib $(distdir)/register $(distdir)/utilities
	cp -p $(srcdir)/conversions/*.?pp $(distdir)/conversions
	cp -p $(srcdir)/functions/*.?pp $(distdir)/functions
	cp -p $(srcdir)/rpxll/*.vcproj $(distdir)/rpxll
	cp -p $(srcdir)/rpxllib/*.vcproj $(distdir)/rpxllib
	cp -p $(srcdir)/register/*.?pp $(distdir)/register
	cp -p $(srcdir)/utilities/*.?pp $(distdir)/utilities

