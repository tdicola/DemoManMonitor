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
	vector<uint8_t> playbuf;
	virtual void play(uint8_t* buffer, size_t frames) {
		for (size_t i = 0; i < frames; ++i) {
			playbuf.push_back(buffer[i]);
		}
	}
	virtual unsigned long available() {
		return 1024;
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
	Adafruit_Thermal printer(fileno(tmpfile()));
	MockAudioSource audioSource;
	MockAudioSink audioSink;
	MockKeywordSpotter spotter("foo");
	vector<uint8_t> alarm = { 1, 2, 3 };
	DemoManMonitor monitor(1, &printer, &audioSource, &audioSink, &spotter, &alarm);

	monitor.update();

	//EXPECT_EQ(&alarm, audioSink.played);
	EXPECT_EQ(3, audioSink.playbuf.size());
	for (size_t i = 0; i < audioSink.playbuf.size(); ++i) {
		EXPECT_EQ(alarm[i], audioSink.playbuf[i]);
	}
}
