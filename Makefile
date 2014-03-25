SOURCES = main.cpp
OUTPUT = DemoManMonitor
CXX = g++-4.8
CXX_FLAGS = -std=c++11
LD_FLAGS = 
GTEST_DIR = ./gtest-1.7.0
TEST_SOURCES = ./tests/test_foo.cpp


main: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) $(SOURCES) -o $(OUTPUT)

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest
	$(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -pthread $(TEST_SOURCES) $(GTEST_DIR)/src/gtest_main.cc gtest-all.o -o testrunner

gtest:
	$(CXX) $(CXX_FLAGS) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

clean:
	rm -f *.o
	rm -f testrunner
	rm -f $(OUTPUT)
