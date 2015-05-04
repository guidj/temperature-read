CXX=g++
SOURCES=main.cpp tempread.cpp
EXEC=tempreader
FAKE_SOURCES=fakereader.cpp
FAKE_EXEC=fakereader
LIBS = -lm
MKDIR = mkdir -p
BIN = ../bin

$(EXEC): $(SOURCES)
	$(CXX) $(SOURCES) $(LIBS) -o $(EXEC)
	$(CXX) $(FAKE_SOURCES) -o $(FAKE_EXEC)

	$(MKDIR) $(BIN)

	mv $(EXEC) $(BIN)/$(EXEC)
	mv $(FAKE_EXEC) $(BIN)/$(FAKE_EXEC)

clean:
	rm $(BIN)/$(EXEC)
	rm $(BIN)/$(FAKE_EXEC)