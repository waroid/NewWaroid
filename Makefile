DEPEND_FILE = depend_file
BINDIR = /usr/local/bin
DATADIR = /usr/local/share/waroid

CC = gcc
CXX = g++
CFLAGS = 
CXXFLAGS = -D__RPI__

INCPATH = 
LINK = g++
LIBS = -lm -lpthread -lwiringPi -lasound -lsndfile

# FILE
CSOURCES = 

CPPSOURCES = main.cpp \
	communication/GRCAcceptSession.cpp \
	communication/GRCBaseSession.cpp \
	communication/GRCConnectSession.cpp \
	communication/GRCSerialSession.cpp \
	communication/GRCSockAddr.cpp \
	communication/GRCTcpSession.cpp \
	core/GRCBuffer.cpp \
	core/GRCCoreUtil.cpp \
	core/GRCJsonData.cpp \
	core/GRCLogger.cpp \
	core/GRCObject.cpp \
	core/GRCString.cpp \
	sound/GRCSoundWorker.cpp \
	sound/GRCWave.cpp \
	ControlBoardSession.cpp \
	GameSession.cpp \
	RobotData.cpp \
	RobotInfo.cpp \
	UserSession.cpp \
	WeaponData.cpp \
	Manager.cpp

OBJECTS = $(CSOURCES:%.c=%.o)
OBJECTS += $(CPPSOURCES:%.cpp=%.o)

SOUNDS = sound/Common_Booting_Sound.wav \
	sound/12.7mm_GAU-19B_HMG.wav \
	sound/2_c_Hit.wav \
	sound/20mm_M134D_Minigun.wav \
	sound/20mm_M61_Vulcan.wav \
	sound/25mm_M242_Chaingun.wav \
	sound/3_c_destruction.wav \
	sound/30mm_2A72_Autocannon.wav \
	sound/4_c_revival.wav \
	sound/5_2nd_Weapon.wav \
	sound/6_2nd_Weapon.wav \
	sound/7_2nd_Weapon.wav

	
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
