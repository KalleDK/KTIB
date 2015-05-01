MKDIR=mkdir -p
ECHO=echo
RM=rm -rf

ARGV=
ARCH=host
BUILD_DIR=${ARCH}
SRC_DIR=src
SOURCES=${SRC_DIR}/main.cpp ${SRC_DIR}/KarContainer.cpp ${SRC_DIR}/SocketClient.cpp ${SRC_DIR}/SocketServer.cpp ${SRC_DIR}/MessageQueue.cpp ${SRC_DIR}/Bridge.cpp ${SRC_DIR}/MessageThread.cpp ${SRC_DIR}/Message.cpp ${SRC_DIR}/RS485.cpp ${SRC_DIR}/Kar.cpp ${SRC_DIR}/Thread.cpp ${SRC_DIR}/KarBus.cpp ${SRC_DIR}/Protocol.cpp ${SRC_DIR}/SensorOe.cpp ${SRC_DIR}/SensorOeContainer.cpp
OBJECTS=${SOURCES:${SRC_DIR}/%.cpp=${BUILD_DIR}/%.o}
DEPS_CPP=${SOURCES:${SRC_DIR}/%.cpp=${BUILD_DIR}/%.dpp}
DEPS=${DEPS_CPP:${SRC_DIR}/%.c=${BUILD_DIR}/%.d}
EXECUTABLE=FlexPMS
CXXFLAGS=-Wall
LD=-lpthread 
MAKEFILE_TARGETS_WITHOUT_INCLUDE := clean help 

ifeq (${ARCH},target)
	CXXFLAGS+= -std=c++0x -I/home/stud/raspberrypi/rootfs/usr/include/ 
 	LD+= -lmysqlcppconn -L/home/stud/raspberrypi/rootfs/usr/lib/ -Wl,-rpath=/home/stud/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf/,-rpath=/home/stud/raspberrypi/rootfs/lib/arm-linux-gnueabihf/
	#CXX=arm-angstrom-linux-gnueabi-g++
	CXX=arm-linux-gnueabihf-g++
else
	CXXFLAGS+= -std=c++11
	LD+= -lmysqlcppconn
	CXX=g++-4.7
endif

ifdef DEBUG
	CXXFLAGS+= -DDEBUG -g
endif

.PHONY: clean help run

${EXECUTABLE}: ${OBJECTS}
	${CXX} -o ${BUILD_DIR}/${EXECUTABLE} ${OBJECTS} ${LD}

${BUILD_DIR}/%.d: ${SRC_DIR}/%.c
	@${MKDIR} ${BUILD_DIR}
	@${CXX} -MT $(@:.d=.o) -MM $< > $@
	@${ECHO} '	$${CXX} -c -o $$@ $$< $${CXXFLAGS}' >> $@

${BUILD_DIR}/%.dpp: ${SRC_DIR}/%.cpp
	@${MKDIR} ${BUILD_DIR}
	@${CXX} -MT $(@:.dpp=.o) -MM $< > $@
	@${ECHO} '	$${CXX} -c -o $$@ $$< $${CXXFLAGS}' >> $@

clean: 
	@if [ -d "${BUILD_DIR}" ]; then ${RM} -v  "${BUILD_DIR}"; fi

run: ${EXECUTABLE}
	./${BUILD_DIR}/${EXECUTABLE} ${ARGV}

help:
	@${ECHO} "Help"

ifeq ($(filter $(MAKECMDGOALS),$(MAKEFILE_TARGETS_WITHOUT_INCLUDE)),)
-include $(DEPS)
endif
