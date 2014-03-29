// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple tool to test playing back audio with the AlsaSink class.

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "AlsaSink.h"

using namespace std;

bool shouldRun = true;

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Must provide hardware name and file name as command line parameters!" << endl;
		return 1;
	}
	try {
		// Read the file data into memory.
		ifstream input(argv[2], ios::in | ios::binary);
		input.seekg (0, input.end);
		size_t length = input.tellg();
		input.seekg (0, input.beg);
		vector<uint8_t> buffer(length);
		input.read((char*)buffer.data(), length);
		// Create a 16khz mono device, grab data from stdin, and play it on the audio device.
		AlsaSink sink;
		sink.open(argv[1], 16000, 1, SND_PCM_FORMAT_S16_LE);
		sink.play(buffer);
	}
	catch (runtime_error ex) {
		cerr << "ERROR: " << ex.what() << endl;
		return 1;
	}
	return 0;
}