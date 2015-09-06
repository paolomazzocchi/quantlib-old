
SUBDIRS = conversions enumerations loop valueobjects

AM_CPPFLAGS = -I${top_srcdir}
includedir = $(prefix)/include/rp

include_HEADERS = \
    addin.hpp \
    config.hpp \
    exception.hpp \
    group.hpp \
    iless.hpp \
    libraryobject.hpp \
    logger.hpp \
    object.hpp \
    objectwrapper.hpp \
    observable.hpp \
    processor.hpp \
    property.hpp \
    range.hpp \
    reposit.hpp \
    repository.hpp \
    rpdefines.hpp \
    serializationfactory.hpp \
    singleton.hpp \
    utilities.hpp \
    valueobject.hpp

EXTRA_DIST = \
    auto_link.hpp

lib_LTLIBRARIES = libreposit.la
LDFLAGS = -lboost_filesystem -lboost_regex -lboost_serialization -release $(PACKAGE_VERSION)
if RP_LINK_LOG4CXX
LDFLAGS += -llog4cxx
endif

libreposit_la_SOURCES = \
    addin.cpp \
    logger.cpp \
    processor.cpp \
    repository.cpp \
    serializationfactory.cpp \
    utilities.cpp

libreposit_la_LIBADD = \
    enumerations/libEnumerations.la

