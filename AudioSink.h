// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Simple audio playback sink interface.

#pragma once

#include <cstdint>
#include <vector>

class AudioSink {
public:
	virtual ~AudioSink() {}
	// Play back the provided buffer of audio.  This will block until the audio is played.
	virtual void play(std::vector<uint8_t>& buffer) = 0;
	// Start playback of the provided buffer asyncronously.
	virtual void playAsync(std::vector<uint8_t>& buffer) = 0;
	// Called regularly during async playback.  Returns true if there is audio left to play.
	virtual bool asyncUpdate() = 0;
	// Stop playing audio.
	virtual void pause() = 0;
	// Resume playing audio after stop.
	virtual void resume() = 0;
};
