// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Test printing data to a thermal printer.

#include <iostream>

#include "Adafruit_Thermal.h"

using namespace std;

bool shouldRun = true;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Must provide serial port as command line parameter!" << endl;
		return 1;
	}
	
	Adafruit_Thermal printer(argv[1]);

	printer.begin();
	printer.upsideDownOn();
	printer.justify('C');
	printer.doubleHeightOn();
	printer.doubleWidthOn();
	printer.boldOn();
	printer.println("***** FINE *****");
	printer.boldOff();
	printer.doubleWidthOff();
	printer.feed(2);
	printer.println("YOU ARE FINED:");
	printer.feed(1);
	printer.doubleWidthOn();
	printer.boldOn();
	printer.println("ONE CREDIT");
	printer.boldOff();
	printer.doubleWidthOff();
	printer.feed(1);
	printer.println("FOR VIOLATION OF THE");
	printer.println("VERBAL MORALITY STATUTES");
	printer.feed(2);
	printer.doubleWidthOn();
	printer.boldOn();
	printer.println("***** FINE *****");
	printer.feed(6);


	return 0;
}