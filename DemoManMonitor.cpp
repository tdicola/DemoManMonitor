// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "DemoManMonitor.h"

#include <iostream>

using namespace std;

DemoManMonitor::DemoManMonitor(size_t bufferSize,
							   Adafruit_Thermal* printer, 
							   AudioSource* audioSource,
							   AudioSink* audioSink,
							   KeywordSpotter* spotter,
							   std::vector<uint8_t>* alarm):
	_printer(printer),
	_audioSource(audioSource),
	_audioSink(audioSink), 
	_spotter(spotter),
	_alarm(alarm),
	_buffer(bufferSize),
	_ticketSteps()
{
	setTicketSteps();
}

DemoManMonitor::~DemoManMonitor() 
{}

void DemoManMonitor::update() {
	// Grab a buffer of audio.
	_audioSource->record(_buffer);
	// Look for a keyword.
	string keyword = _spotter->process(_buffer);
	if (keyword != "") {
		// Keyword was spotted, sound alarm.
		cout << "==== KEYWORD SPOTTED: " << keyword << endl;
		raiseAlarm(keyword);
	}
}

// Define a macro for adding a ticket step in the setTicketSteps
// function further below.
#define ADD_TICKET_STEP(command) \
  { _ticketSteps.push_back([](Adafruit_Thermal* printer) { \
  	command; \
  }); }

void DemoManMonitor::setTicketSteps() {
	// Define the steps for printing the fine ticket.
	// Because each printer command will block until the
	// printer is ready, an array of commands is defined
	// as a continuation that will allow execution of 
	// printer commands while audio plays.
	_ticketSteps.clear();
	ADD_TICKET_STEP(printer->upsideDownOn());
	ADD_TICKET_STEP(printer->justify('C'));
	ADD_TICKET_STEP(printer->doubleWidthOn());
	ADD_TICKET_STEP(printer->boldOn());
	ADD_TICKET_STEP(printer->println("***** FINE *****"));
	ADD_TICKET_STEP(printer->boldOff());
	ADD_TICKET_STEP(printer->doubleWidthOff());
	ADD_TICKET_STEP(printer->feed(2));
	ADD_TICKET_STEP(printer->println("YOU ARE FINED:"));
	ADD_TICKET_STEP(printer->feed(1));
	ADD_TICKET_STEP(printer->doubleWidthOn());
	ADD_TICKET_STEP(printer->boldOn());
	ADD_TICKET_STEP(printer->println("ONE CREDIT"));
	ADD_TICKET_STEP(printer->boldOff());
	ADD_TICKET_STEP(printer->doubleWidthOff());
	ADD_TICKET_STEP(printer->feed(1));
	ADD_TICKET_STEP(printer->println("FOR VIOLATION OF THE"));
	ADD_TICKET_STEP(printer->println("VERBAL MORALITY STATUTES"));
	ADD_TICKET_STEP(printer->feed(2));
	ADD_TICKET_STEP(printer->doubleWidthOn());
	ADD_TICKET_STEP(printer->boldOn());
	ADD_TICKET_STEP(printer->println("***** FINE *****"));
	ADD_TICKET_STEP(printer->feed(6));
}

void DemoManMonitor::raiseAlarm(const std::string& keyword) {
	// Stop audio recording while the alarm is raised.
	_audioSource->pause();
	// Play audio and print the ticket at the same time.
	// Since the Pi only has one core and timing is somewhat
	// critical (for smooth audio playback), a tight loop to
	// update audio and ticket printing state will be executed.
	size_t frame = 0;
	bool step = 0;
	// Enable audio playback.
	_audioSink->resume();
	while (frame < _alarm->size() && step < _ticketSteps.size()) {
		// Check if audio should be added to the buffer.
		if (frame < _alarm->size()) {
			auto available = _audioSink->available();
			if (available > 0) {
				size_t toPlay = min(available, _alarm->size() - frame);
				_audioSink->play(&(_alarm->data()[frame]), toPlay);
				frame += toPlay;
			}
		}
		// Check if the printer is ready for a new command.
		if (step < _ticketSteps.size() && _printer->ready()) {
			// Execute the current step and increment to the next one.
			_ticketSteps[step](_printer);
			step++;
		}
	}
	// Stop audio playback.
	_audioSink->pause();
	// Enable recording again.
	_audioSource->resume();
}
