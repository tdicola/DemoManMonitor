// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple tool to test recording audio with the AlsaSource class.

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

#include "AlsaSource.h"

using namespace std;

bool shouldRun = true;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Must provide hardware name as command line parameter!" << endl;
		return 1;
	}
	// Signal handler to catch ctrl-c and shut down the main loop gracefully.
	signal(SIGINT, [](int param){ shouldRun = false; });
	try {
		// Create a 16khz mono source, start recording 0.5 seconds of audio at a time, and echo it to stdout.
		AlsaSource source;
		source.open(argv[1], 16000, 1, SND_PCM_FORMAT_S16_LE);
		vector<uint8_t> buffer(16000);
		while (shouldRun) {
			source.record(buffer);		
			copy(begin(buffer), end(buffer), ostream_iterator<uint8_t>(cout));
		}
	}
	catch (runtime_error ex) {
		cerr << "ERROR: " << ex.what() << endl;
		return 1;
	}
	return 0;
}