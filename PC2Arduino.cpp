//Code taken from https://github.com/ZainUlMustafa/Connect-And-Use-Arduino-via-Cpp-Software-Made-In-Any-IDE

/*#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;
#include"SerialPort.h"


char output[MAX_DATA_LENGTH];

char port[] = "\\\\.\\COM3";

int main() {
	SerialPort arduino(port);
	if (arduino.isConnected()) {
		cout << "Connection made" << endl << endl;
	}
	else {
		cout << "Error in port name" << endl << endl;
	}
	while (arduino.isConnected()) {
		cout << "Enter your command: " << endl;
		string data;
		cin >> data;

		char* charArray = new char[data.size() + 1];
		copy(data.begin(), data.end(), charArray);
		charArray[data.size()] = '\n'; //for Arduino to know end of string

		arduino.writeSerialPort(charArray, MAX_DATA_LENGTH);
		arduino.readSerialPort(output, MAX_DATA_LENGTH);

		cout << ">> " << output << endl;

		delete[] charArray;
	}
	return 0;
}*/
