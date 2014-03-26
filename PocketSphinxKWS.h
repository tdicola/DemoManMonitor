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
	// Initialize PocketSphinx with list of keywords specified in a file (only way to send a list of keywords unfortunately).
	void initialize(cmd_ln_t* config, const std::string& keywordFileName);
	// Process a buffer of audio to look for keywords.
	virtual std::string process(std::vector<uint8_t>& buffer);
	// Parse command line parameters into PocketSphinx configuration.
	static cmd_ln_t* parseConfig(int argc, char* argv[]);

private:
	ps_decoder_t* _ps;

};
