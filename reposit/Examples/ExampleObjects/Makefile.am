
SUBDIRS = Enumerations Library Objects Serialization ValueObjects

LDFLAGS = -lboost_filesystem -lboost_serialization -lboost_regex -lboost_system

noinst_LTLIBRARIES = libExampleObjects.la

libExampleObjects_la_SOURCES =

noinst_HEADERS = \
    accountexample.hpp

libExampleObjects_la_LIBADD = \
    Enumerations/libEnumerations.la \
    Serialization/libSerialization.la \
    ValueObjects/libValueObjects.la

EXTRA_DIST = \
    ExampleObjects_vc9.vcproj

