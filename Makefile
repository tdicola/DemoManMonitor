SOURCES = DemoManMonitor.cpp
OUTPUT = DemoManMonitor
CXX = g++-4.8
CXX_FLAGS = -std=c++11 -Wall -Werror
LD_FLAGS = 
PROJECT_DIR = .
GTEST_DIR = ./gtest-1.7.0
TEST_SOURCES = ./tests/test_DemoManMonitor.cpp


main: $(SOURCES) main.cpp
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) $(SOURCES) main.cpp -o $(OUTPUT)

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest
	$(CXX) $(CXX_FLAGS) -I$(PROJECT_DIR) -isystem $(GTEST_DIR)/include -pthread $(TEST_SOURCES) $(SOURCES) $(GTEST_DIR)/src/gtest_main.cc gtest-all.o -o testrunner

gtest:
	$(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

clean:
	rm -f *.o
	rm -f testrunner
	rm -f $(OUTPUT)
