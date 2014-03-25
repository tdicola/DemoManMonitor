#include <cstdint>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "DemoManMonitor.h"
#include "AudioSink.h"
#include "KeywordSpotter.h"

using namespace std;

class MockAudioSink: public AudioSink {
public:
	virtual void playWav(vector<uint8_t>* wav) {
		playedWav = wav;
	}
	vector<uint8_t>* playedWav;
};

class MockKeywordSpotter: public KeywordSpotter {
public:
	MockKeywordSpotter(const string& keyword): keyword(keyword) {}
	string keyword;
	virtual string update() {
		return keyword;
	}
};

TEST(DemoManMonitor, update_plays_wav_when_keyword_spotted) {
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarmWav;
	DemoManMonitor monitor(&audioSink, &spotter, &alarmWav);

	monitor.update();

	ASSERT_EQ(&alarmWav, audioSink.playedWav);
}
