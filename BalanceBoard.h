#include <stdint.h>
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <map>

using std::vector;
using std::map;

class BalanceBoard {

public:

    BalanceBoard();
    ~BalanceBoard() {};

    bool reloadWiimotes;
    float total, topLeft, topRight, bottomLeft, bottomRight;

    map<string,float> sensor;

    //float minTotal, minTopLeft, minTopRight, minBottomLeft, minBottomRight;

    CWii * wii; // Defaults to 4 remotes
    vector<CWiimote> wiimotes;

    bool isConnected;

    void connect();
    void handleEvent(CWiimote &wm);
    void poll();
    void printSensors();
    void inputLoop();
    
    bool hasWiiMotes();
    float getTotal();

};
