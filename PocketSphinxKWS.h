// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// PocketSphinx-based keyword spotting algorithm.

#pragma once

#include <stdexcept>

#include <pocketsphinx.h>

#include "KeywordSpotter.h"

class PocketSphinxKWS: public KeywordSpotter {
public:
	PocketSphinxKWS();
	~PocketSphinxKWS();
	void initialize(cmd_ln_t* config, const std::string& keywordFileName);
	virtual std::string process(std::vector<uint8_t>& buffer);

	static cmd_ln_t* parseConfig(int argc, char* argv[]);

private:
	ps_decoder_t* _ps;

};
