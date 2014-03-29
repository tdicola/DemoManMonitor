#include "gtest/gtest.h"

#include "Adafruit_Thermal.h"
#include "DemoManMonitor.h"
#include "AudioSink.h"
#include "KeywordSpotter.h"

using namespace std;

class MockAudioSource: public AudioSource {
public:
	virtual void record(vector<uint8_t>& buffer) {
	}
	virtual void pause() {}
	virtual void resume() {}
};

class MockAudioSink: public AudioSink {
public:
	virtual void play(vector<uint8_t>& buffer) {
		played = &buffer;
	}
	vector<uint8_t>* played;
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

TEST(DemoManMonitor, update_plays_wav_when_keyword_spotted) {
	Adafruit_Thermal printer(fileno(tmpfile()));
	MockAudioSource audioSource;
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarm;
	DemoManMonitor monitor(1, &printer, &audioSource, &audioSink, &spotter, &alarm);

	monitor.update();

	EXPECT_EQ(&alarm, audioSink.played);
}
