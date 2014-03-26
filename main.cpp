// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Main application.

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "AlsaSink.h"
#include "AlsaSource.h"
#include "DemoManMonitor.h"
#include "PocketSphinxKWS.h"

using namespace std;

#define ALARM_FILE		"alarm_trimmed.raw"
#define RECORD_HW		"plughw:1,0"
#define PLAYBACK_HW		"plughw:2,0"
#define KEYWORD_FILE	"keywords.txt"

bool shouldRun = true;

int main(int argc, char* argv[]) {
	cout << "Demolition Man Verbal Morality Statute Monitor" << endl;
	try {
		// Signal handler to catch ctrl-c in the main loop and shut down gracefully (i.e. call destructors).
		signal(SIGINT, [](int param){ shouldRun = false; });

		// Load alarm raw audio.
		ifstream input(ALARM_FILE, ios::in | ios::binary);
		input.seekg (0, input.end);
		size_t length = input.tellg();
		input.seekg (0, input.beg);
		vector<uint8_t> alarm(length);
		input.read((char*)alarm.data(), length);

		// Initialize audio sink and source.
		AlsaSink sink;
		sink.open(PLAYBACK_HW, 16000, 1, SND_PCM_FORMAT_S16_LE);
		AlsaSource source;
		source.open(RECORD_HW, 16000, 1, SND_PCM_FORMAT_S16_LE);

		// Initialize keyword spotter.
		PocketSphinxKWS spotter;
		spotter.initialize(PocketSphinxKWS::parseConfig(argc, argv), KEYWORD_FILE);

		// Initialize main logic.
		DemoManMonitor monitor(8000, &source, &sink, &spotter, &alarm);

		while (shouldRun) {
			monitor.update();
		}
	}
	catch (runtime_error ex) {
		cerr << "ERROR: " << ex.what() << endl;
		return 1;
	}

	return 0;
}
