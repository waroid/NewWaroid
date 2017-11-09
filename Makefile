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

SOUNDS = data/sound/Common_Booting_Sound.wav \
	data/sound/12.7mm_GAU-19B_HMG.wav \
	data/sound/2_c_Hit.wav \
	data/sound/20mm_M134D_Minigun.wav \
	data/sound/20mm_M61_Vulcan.wav \
	data/sound/25mm_M242_Chaingun.wav \
	data/sound/3_c_destruction.wav \
	data/sound/30mm_2A72_Autocannon.wav \
	data/sound/4_c_revival.wav \
	data/sound/5_2nd_Weapon.wav \
	data/sound/6_2nd_Weapon.wav \
	data/sound/7_2nd_Weapon.wav

	
JSONS = data/json/WaroidRobotData.json \
	data/json/WaroidWeaponData.json

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
	sudo mkdir -p $(DATADIR)
	sudo cp -f $(TARGET) $(BINDIR)
	sudo cp -f $(SOUNDS) $(DATADIR)
	sudo cp -f $(JSONS) $(DATADIR)

uninstall:
	- sudo killall $(TARGET)
	sudo rm -fr $(DATADIR)
	sudo rm -f $(BINDIR)/$(TARGET)

kill:
	- sudo killall $(TARGET)

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
