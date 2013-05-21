#include <stdint.h>
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <map>

//PyObject* PyInit_wiicpp(void); /* Forward */

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

    BalanceBoard();
    void handleEvent(CWiimote &wm);
    void poll();
    void printSensors();
    void inputLoop();
    
    bool hasWiiMotes();
    float getTotal();

};
