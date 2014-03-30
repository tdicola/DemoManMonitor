#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib
./DemoManMonitor -hmm ./models/hub4wsj_sc_8k/ -dict ./models/cmu07a.dic -lm ./models/wsj0vp.5000.DMP
