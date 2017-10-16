DEPEND_FILE = depend_file
BINDIR = /usr/local/bin
DATADIR = /usr/local/share/waroid

CC = gcc
CXX = g++
CFLAGS = 
CXXFLAGS = 

INCPATH = 
LINK = g++
LIBS = -lm -lpthread -lwiringPi -lasound -lsndfile

# FILE
CSOURCES = 

CPPSOURCES = main.cpp \
	core/GRCLogger.cpp \
	core/GRCWave.cpp \
	communication/GRCSockAddr.cpp \
	communication/GRCBaseSession.cpp \
	communication/GRCSerialSession.cpp \
	communication/GRCTcpSession.cpp \
	jsoncpp.cpp \
	RobotData.cpp \
	WeaponData.cpp \
	ControlBoardSession.cpp \
	GameServerSession.cpp \
	UserSession.cpp \
	RobotInfo.cpp \
	Manager.cpp

OBJECTS = $(CSOURCES:%.c=%.o)
OBJECTS += $(CPPSOURCES:%.cpp=%.o)

SOUNDS = sound/startup.wav \
	sound/test.wav
	
JSONS = json/WaroidRobotData.json \
	json/WaroidWeaponData.json

TARGET = NewWaroidServer

# Implict rules
.SUFFIXES : .cpp .c

.cpp.o: $(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<
.c.o: $(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

.PHONY: all clean depend install uninstall kill

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPEND_FILE)

depend:
	#$(CC) -MM $(CSOURCES) > $(DEPEND_FILE)
	$(CXX) -MM $(CPPSOURCES) > $(DEPEND_FILE)

rebuild: clean depend all

install:
	mkdir -p $(DATADIR)
	cp -f $(TARGET) $(BINDIR)
	cp -f $(SOUNDS) $(DATADIR)
	cp -f $(JSONS) $(DATADIR)

uninstall:
	- killall $(TARGET)
	rm -fr $(DATADIR)
	rm -f $(BINDIR)/$(TARGET)

kill:
	- killall $(TARGET)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(MAKECMDGOALS), install)
ifneq ($(MAKECMDGOALS), uninstall)
ifneq ($(CSOURCES), )
ifneq ($(CPPSOURCE), )
-include $(DEPEND_FILE)
endif
endif
endif
endif
endif
endif
