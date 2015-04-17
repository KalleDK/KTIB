CXX=g++
SOURCES=main.cpp Kar.cpp KarContainer.cpp Message.cpp MessageQueue.cpp Thread.cpp MessageThread.cpp Bridge.cpp SocketServer.cpp SocketClient.cpp KarBus.cpp RS485.cpp
OBJECTS=$(SOURCES:.cpp=.o)
DEPS=$(SOURCES:.cpp=.d)
EXE=FlexPMS
CXXFLAGS=-I.

$(EXE):$(DEPS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) -lpthread -lmysqlcppconn

%.d:%.cpp
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

clean:
	rm $(OBJECTS) $(EXE) *.d *.o

-include $(DEPS)
