#include <stdint.h>
#include <fstream>
#include <unistd.h>
#include <wiicpp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <map>
#include <Python.h>

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

    BalanceBoard() {
        wii = new CWii();
        reloadWiimotes = false;
        

        sensor["total"] = 0;


        while(wiimotes.size() == 0) {
            wii->FindAndConnect(1);
            wiimotes = wii->GetWiimotes();
            
            if (wiimotes.size() == 0)
              cout << "Error: no connected Wiimote" << endl;
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

BalanceBoard * bb;

static PyObject * wiicpp_foo(PyObject *self, PyObject* args) {
    bb = new BalanceBoard();

    while(bb->wiimotes.size()) { // Go so long as there are wiimotes left to poll
        bb->poll();
        bb->printSensors();
    }
  	return PyLong_FromLong(42L);
}

BalanceBoard * wiicpp_init(PyObject *self, PyObject* args) {
  	return new BalanceBoard();
}

PyObject * wiicpp_get_total(PyObject *self, PyObject* args) {
    bb->poll();
  	return PyFloat_FromDouble(bb->sensor["total"]);
}

#ifdef __cplusplus
extern "C" {
#endif 

static PyMethodDef wiicpp_methods[] = {
    {"foo", wiicpp_foo, METH_NOARGS, "Return the meaning of everything."},
    {"init", wiicpp_init, METH_NOARGS, "init."},
    {"get_total", wiicpp_get_total, METH_NOARGS, "get total weight."},
    {NULL, NULL}
};

static struct PyModuleDef wiicppmodule = {
    {}, /* m_base */
    "wiicpp",  /* m_name */
    0,  /* m_doc */
    0,  /* m_size */
    wiicpp_methods,  /* m_methods */
    0,  /* m_reload */
    0,  /* m_traverse */
    0,  /* m_clear */
    0,  /* m_free */
};

PyObject* PyInit_wiicpp() {
    return PyModule_Create(&wiicppmodule);
}

#ifdef __cplusplus
}  // extern "C"
#endif 
