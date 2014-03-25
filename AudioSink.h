#pragma once

#include <cstdint>
#include <vector>

class AudioSink {
public:
	virtual ~AudioSink() {}
	virtual void playWav(std::vector<uint8_t>* wav) = 0;

};
