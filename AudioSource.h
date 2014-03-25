#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class AudioSource {
public:
	virtual ~AudioSource() {}
	virtual std::vector<uint8_t>* getAudio() = 0;

};
