#include "DemoManMonitor.h"

using namespace std;

DemoManMonitor::DemoManMonitor(AudioSink* audioSink, KeywordSpotter* spotter, std::vector<uint8_t>* alarmWav):
	_audioSink(audioSink), 
	_spotter(spotter),
	_alarmWav(alarmWav) 
{}

DemoManMonitor::~DemoManMonitor() {

}

void DemoManMonitor::update() {
	auto keyword = _spotter->update();
	if (keyword != "") {
		// Keyword was spotted, sound alarm and print ticket.
		_audioSink->playWav(_alarmWav);
	}
}
