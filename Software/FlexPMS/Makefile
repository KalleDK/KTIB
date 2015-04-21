CXX=g++-4.7
SOURCES=main.cpp KarBus.cpp Bridge.cpp Kar.cpp KarContainer.cpp Message.cpp MessageQueue.cpp Thread.cpp MessageThread.cpp SocketServer.cpp SocketClient.cpp RS485.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(SOURCES:.cpp=.d)
EXE=FlexPMS
CXXFLAGS=-I.

$(EXE):$(DEPS) $(OBJECTS)
	$(CXX) -Wall -g -std=c++0x $(CXXFLAGS) -o $@ $(OBJECTS) -lpthread -lmysqlcppconn

%.d:%.cpp
	$(CXX) -Wall -g -std=c++0x -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

clean:
	rm $(OBJECTS) $(EXE) *.d *.o

-include $(DEPS)
