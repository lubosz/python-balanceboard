#include "BalanceBoard.h"

BalanceBoard::BalanceBoard() {
      isConnected = false;
      total = 0;
      topLeft = 0; topRight = 0; bottomLeft = 0; bottomRight = 0;
};

void BalanceBoard::connect() {
    wii = new CWii();
    reloadWiimotes = false;

    sensor["total"] = 0;

    while(wiimotes.size() == 0) {
        wii->FindAndConnect(1);
        wiimotes = wii->GetWiimotes();

        if (wiimotes.size() == 0)
            cout << "Error: no connected Wiimote" << endl;
    }
    isConnected = true;
}

void BalanceBoard::handleEvent(CWiimote &wm) {
    if (wm.Buttons.isPressed(CButtons::BUTTON_A)) {
        printf("A pressed\n");
    }

    if (wm.ExpansionDevice.GetType() == wm.ExpansionDevice.TYPE_BALANCE_BOARD) {
        wm.ExpansionDevice.BalanceBoard.WeightSensor.GetWeight(sensor["total"], topLeft, topRight, bottomLeft, bottomRight);
    }
}

void BalanceBoard::poll() {
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

void BalanceBoard::printSensors() {
    cout << "\r" << topLeft << "------" << topRight << "\n|\n|\n|     " << sensor["total"] << "\n|\n|\n" << bottomLeft << "------" << bottomRight << "\n\n\n\n\n\n\n\n\n";
}

float BalanceBoard::getTotal() {
    return sensor["total"];
}

bool BalanceBoard::hasWiiMotes() {
    return wiimotes.size() != 0;
}

void BalanceBoard::inputLoop() {
    while(!hasWiiMotes()) { // Go so long as there are wiimotes left to poll
        poll();
        printSensors();
    }
}
