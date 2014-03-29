#include "gtest/gtest.h"

#include <cstdio>
#include <vector>

#include <sys/time.h>

#include "Adafruit_Thermal.h"

using namespace std;

TEST(AdafruitThermal, expected_bytes_are_written) {
	// Create a temp file to hold data written to the printer.
	auto file = tmpfile();
	Adafruit_Thermal printer(fileno(file));

	printer.begin();
	printer.println("test");

	// Read the contents of the file to verify data.
	rewind(file);
	vector<uint8_t> written;
	int c = fgetc(file);
	while (c != EOF) {
		written.push_back((uint8_t)c);
		c = fgetc(file);
	}
	// Verify expected bytes were written.
	vector<uint8_t> expected = { 0xFF, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B,
							  	 0x1B, 0x40, 0x1B, 0x37, 0x14, 0xC8, 0xFA, 0x12, 0x23, 0x8E, 
							  	 't', 'e', 's', 't', '\n' };
	EXPECT_EQ(written.size(), expected.size());
	for (size_t i = 0; i < written.size(); ++i) {
		EXPECT_EQ(written[i], expected[i]);
	}
}

TEST(AdafruitThermal, writes_delay_for_expected_time) {
	auto file = tmpfile();
	Adafruit_Thermal printer(fileno(file));
	printer.begin();
	
	timeval start, end;
	gettimeofday(&start, NULL);
	printer.print("test\ntest");
	gettimeofday(&end, NULL);

	unsigned long long exectime = (end.tv_sec - start.tv_sec)*1000000L + (end.tv_usec - start.tv_usec);
	EXPECT_GE(exectime, 740804);
}
