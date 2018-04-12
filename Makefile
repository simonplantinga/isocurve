#############################################################################
# Makefile for implicit curve approximation
#############################################################################

####### filib++ definitions
#FIDIR   =       /net/users/amit/softwares/filib

####### Compiler, tools and options

CC	=	gcc
CXX	=	g++
CFLAGS	=	-pipe -Wall -W -O2 -DNO_DEBUG
CXXFLAGS=	-pipe -Wall -W -O2 -DNO_DEBUG 
#CXXFLAGS=	-pipe -Wall -W -O2 -DDEBUG  -g
#INCPATH	=	-I$(FIDIR)/include
LINK	=	g++
#LFLAGS	=	-L$(FIDIR)/lib -Wl,-rpath=$(FIDIR)/lib
#LIBS	=	$(SUBLIBS) -L/usr/X11R6/lib -lqt -lXext -lX11 -lm -lfi -lieee
LIBS	=	$(SUBLIBS) -L/usr/X11R6/lib -lXext -lX11 -lm -lieee -lgmp -lmpfr

####### Files

HEADERS =	curve.h \
		quadtree.h \
		epsdraw.h \
		approximate.h
SOURCES =	main.cpp \
		quadtree.cpp \
		epsdraw.cpp \
		approximate.cpp
OBJECTS =	main.o \
		quadtree.o \
		epsdraw.o \
		approximate.o
INTERFACES =	
DIST	=	
TARGET	=	icurve
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: $(TARGET)

$(TARGET): $(OBJECTS) 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f $(OBJECTS) $(TARGET)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

main.o: main.cpp \
		curve.h \
		quadtree.h \
		approximate.h \
		testfunc.h

quadtree.o: quadtree.cpp \
		quadtree.h \
		epsdraw.h

epsdraw.o: epsdraw.cpp \
		epsdraw.h

approximate.o: approximate.cpp \
		approximate.h

