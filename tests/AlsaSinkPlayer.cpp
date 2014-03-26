// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple tool to test playing back audio with the AlsaSink class.

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <exception>
#include <iostream>
#include <iterator>
#include <vector>

#include "AlsaSink.h"

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
		// Create a 16khz mono device, grab data from stdin, and play it on the audio device.
		AlsaSink sink;
		sink.open(argv[1], 16000, 1, SND_PCM_FORMAT_S16_LE);
		vector<uint8_t> buffer(16000);
		while (shouldRun) {
			cin.read((char*)buffer.data(), buffer.size());
			sink.play(buffer);
		}
	}
	catch (runtime_error ex) {
		cerr << "ERROR: " << ex.what() << endl;
	}
	return 0;
}