SOURCES = DemoManMonitor.cpp AlsaSource.cpp AlsaSink.cpp
OUTPUT = DemoManMonitor
CXX = g++-4.8
CXX_FLAGS = -std=c++11 -Wall -Werror
LD_FLAGS = 
PROJECT_DIR = .
GTEST_DIR = ./gtest-1.7.0
TEST_SOURCES = ./tests/test_DemoManMonitor.cpp
LIBS = -lasound


main: $(SOURCES) main.cpp
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) $(SOURCES) $(LIBS) main.cpp -o $(OUTPUT)

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest-all.o gtest_main.o
	$(CXX) $(CXX_FLAGS) -I$(PROJECT_DIR) -isystem $(GTEST_DIR)/include -pthread $(TEST_SOURCES) $(SOURCES) $(LIBS) gtest_main.o gtest-all.o -o testrunner

AlsaSourceRecorder: $(SOURCES)
	$(CXX) $(CXX_FLAGS) -I$(PROJECT_DIR) $(SOURCES) ./tests/AlsaSourceRecorder.cpp -o AlsaSourceRecorder $(LIBS)

AlsaSinkPlayer: $(SOURCES)
	$(CXX) $(CXX_FLAGS) -I$(PROJECT_DIR) $(SOURCES) ./tests/AlsaSinkPlayer.cpp -o AlsaSinkPlayer $(LIBS)

gtest-all.o:
	$(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o:
	$(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -pthread -c $(GTEST_DIR)/src/gtest_main.cc

clean:
	rm -f *.o
	rm -f testrunner
	rm -f $(OUTPUT)
	rm -f AlsaSourceRecorder
	rm -f AlsaSinkPlayer
