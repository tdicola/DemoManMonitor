#include "gtest/gtest.h"

#include <algorithm>
#include <iterator>
#include <fstream>

#include "Adafruit_Thermal.h"
#include "DemoManMonitor.h"
#include "AudioSink.h"
#include "KeywordSpotter.h"

using namespace std;

class MockAudioSource: public AudioSource {
public:
	virtual bool record(vector<uint8_t>& buffer) {
		return true;
	}
	virtual void pause() {}
	virtual void resume() {}
};

class MockAudioSink: public AudioSink {
public:
	vector<uint8_t> played;
	virtual void play(vector<uint8_t>& buffer) {
		played.clear();
		copy(begin(buffer), end(buffer), back_inserter(played));
	}
	virtual void playAsync(std::vector<uint8_t>& buffer) {
		play(buffer);
	}
	virtual bool asyncUpdate() {
		return false;
	}
	virtual void pause() {}
	virtual void resume() {}
};

class MockKeywordSpotter: public KeywordSpotter {
public:
	MockKeywordSpotter(const string& keyword): keyword(keyword) {}
	string keyword;
	virtual string process(vector<uint8_t>& buffer) {
		return keyword;
	}
};

TEST(DemoManMonitor, update_plays_alarm_when_keyword_spotted) {
	auto file = tmpfile();
	Adafruit_Thermal printer(fileno(file));
	printer.begin();
	MockAudioSource audioSource;
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarm = { 1, 2, 3 };
	DemoManMonitor monitor(1, &printer, &audioSource, &audioSink, &spotter, &alarm);

	monitor.update();

	// Verify alarm audio was sent to sink.
	EXPECT_EQ(3, audioSink.played.size());
	for (size_t i = 0; i < audioSink.played.size(); ++i) {
		EXPECT_EQ(alarm[i], audioSink.played[i]);
	}
}

TEST(DemoManMonitor, update_prints_ticket_when_keyword_spotted) {
	auto file = tmpfile();
	Adafruit_Thermal printer(fileno(file));
	printer.begin();
	MockAudioSource audioSource;
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarm = { 1, 2, 3 };
	DemoManMonitor monitor(1, &printer, &audioSource, &audioSink, &spotter, &alarm);

	monitor.update();

	// Read data written to printer for verification.
	rewind(file);
	vector<uint8_t> written;
	int c = fgetc(file);
	while (c != EOF) {
		written.push_back((uint8_t)c);
		c = fgetc(file);
	}

	// Verify something was written.
	EXPECT_GT(written.size(), 0);
}

TEST(DemoManMonitor, quiet_mode_does_not_play_audio_or_print_ticket) {
	auto file = tmpfile();
	Adafruit_Thermal printer(fileno(file));
	printer.begin();
	MockAudioSource audioSource;
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarm = { 1, 2, 3 };
	DemoManMonitor monitor(1, &printer, &audioSource, &audioSink, &spotter, &alarm);

	monitor.setQuietMode(true);
	monitor.update();

	// Read data written to printer for verification.
	rewind(file);
	vector<uint8_t> written;
	int c = fgetc(file);
	while (c != EOF) {
		written.push_back((uint8_t)c);
		c = fgetc(file);
	}

	// Verify nothing was written to printer.
	EXPECT_EQ(21, written.size());
	// Verify no alarm audio was sent to sink.
	EXPECT_EQ(audioSink.played.size(), 0);
}
