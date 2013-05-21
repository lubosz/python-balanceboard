#include <stdint.h>
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <map>

using std::vector;
using std::map;

class BalanceBoard {

public:

    bool reloadWiimotes;
    float total, topLeft, topRight, bottomLeft, bottomRight;

    map<string,float> sensor;

    //float minTotal, minTopLeft, minTopRight, minBottomLeft, minBottomRight;

    CWii * wii; // Defaults to 4 remotes
    vector<CWiimote> wiimotes;

    BalanceBoard() {
        wii = new CWii();
        reloadWiimotes = false;
        wii->FindAndConnect(1);

        sensor["total"] = 0;


        while(!wiimotes.size()) {
            cout << "Error: no connected Wiimote" << endl;
            // Find and connect to the wiimotes
            wiimotes = wii->FindAndConnect(1);
        }
    }

    void handleEvent(CWiimote &wm) {
        if (wm.Buttons.isPressed(CButtons::BUTTON_A)) {
            printf("A pressed\n");
        }

        if (wm.ExpansionDevice.GetType() == wm.ExpansionDevice.TYPE_BALANCE_BOARD) {
            wm.ExpansionDevice.BalanceBoard.WeightSensor.GetWeight(sensor["total"], topLeft, topRight, bottomLeft, bottomRight);
        }
    }

    void poll() {
        if(reloadWiimotes) {
            // Regenerate the list of wiimotes
            wiimotes = wii->GetWiimotes();
            reloadWiimotes = 0;
        }

        //Poll the wiimotes to get the status like pitch or roll
        if(wii->Poll()) {
            for(vector<CWiimote>::iterator i = wiimotes.begin(); i != wiimotes.end(); ++i) {
                // Use a reference to make working with the iterator handy.
                CWiimote & wiimote = *i;
                switch(wiimote.GetEvent()) {
                case CWiimote::EVENT_EVENT:
                    handleEvent(wiimote);
                    break;

                case CWiimote::EVENT_DISCONNECT:
                case CWiimote::EVENT_UNEXPECTED_DISCONNECT:
                    reloadWiimotes = 1;
                    break;

                default:
                    cout << "Other case\n";
                    break;
                }
            }
        }
    }

    void printSensors() {
        cout << "\r" << topLeft << "------" << topRight << "\n|\n|\n|     " << sensor["total"] << "\n|\n|\n" << bottomLeft << "------" << bottomRight << "\n\n\n\n\n\n\n\n\n";
    }

};


void pollWimotes() {

}

int main(int argc, char** argv) {
    cout.precision(3);

    //pollWimotes();
    BalanceBoard bb = BalanceBoard();

    while(bb.wiimotes.size()) { // Go so long as there are wiimotes left to poll
        bb.poll();
        bb.printSensors();
    }
    //std::thread wiiPollThread(pollWimotes);
    //wiiPollThread.join();

    return 0;
}
