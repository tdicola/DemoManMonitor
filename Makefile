OUTPUT = DemoManMonitor
CXX = g++-4.8
CXX_FLAGS = -std=c++11 -Wall -Werror
PROJECT_DIR = .
GTEST_DIR = $(PROJECT_DIR)/gtest-1.7.0
SOURCES = DemoManMonitor.cpp AlsaSource.cpp AlsaSink.cpp PocketSphinxKWS.cpp
TEST_SOURCES = ./tests/test_DemoManMonitor.cpp
LIBS = -lpocketsphinx -lsphinxbase -lsphinxad -lasound
INCLUDES = -I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx
TEST_INCLUDES = -I$(PROJECT_DIR) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)


main: $(SOURCES) main.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(SOURCES) $(LIBS) main.cpp -o $(OUTPUT)

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest-all.o gtest_main.o
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) $(TEST_SOURCES) -pthread $(LIBS) gtest_main.o gtest-all.o -o testrunner

AlsaSourceRecorder: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSourceRecorder.cpp -o AlsaSourceRecorder $(LIBS)

AlsaSinkPlayer: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSinkPlayer.cpp -o AlsaSinkPlayer $(LIBS)

gtest-all.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest_main.cc

clean:
	rm -f *.o
	rm -f $(OUTPUT)
	rm -f testrunner
	rm -f AlsaSourceRecorder
	rm -f AlsaSinkPlayer
