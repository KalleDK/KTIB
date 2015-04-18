CXX=g++
SOURCES=main.cpp Bridge.cpp Kar.cpp KarContainer.cpp Message.cpp MessageQueue.cpp Thread.cpp MessageThread.cpp SocketServer.cpp SocketClient.cpp KarBus.cpp RS485.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(SOURCES:.cpp=.d)
EXE=FlexPMS
CXXFLAGS=-I.

$(EXE):$(DEPS) $(OBJECTS)
	$(CXX) -Wall -g $(CXXFLAGS) -o $@ $(OBJECTS) -lpthread -lmysqlcppconn

%.d:%.cpp
	$(CXX) -Wall -g -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

clean:
	rm $(OBJECTS) $(EXE) *.d *.o

-include $(DEPS)
