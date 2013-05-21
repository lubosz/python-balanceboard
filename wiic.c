#include <Python.h>
#include <wiic/wiic.h>

PyObject* PyInit_wiic(void); /* Forward */
#define MAX_WIIMOTES 1
int exiting = 0;

void handle_event(struct wiimote_t* wm) {
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A))		printf("A pressed\n");

	if(wm->exp.type == EXP_BALANCE_BOARD) {
		struct balance_board_t* bb = (balance_board_t*)&wm->exp.bb;
		
		printf("balance board top left weight: %f\n", bb->pressure_weight.top_left);
		printf("balance board top right weight: %f\n", bb->pressure_weight.top_right);
		printf("balance board bottom left weight: %f\n", bb->pressure_weight.bottom_left);
		printf("balance board bottom right weight: %f\n", bb->pressure_weight.bottom_right);
		printf("balance board total weight: %f\n", bb->pressure_weight.weight);
	}
}

void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
	exiting = 1;
}


static PyObject * wiic_foo(PyObject *self, PyObject* args);

int main(int argc, char **argv) {
  printf("Hello");
  
  wiic_foo(0, 0);
  
  Py_Exit(0);
  return 0;
}

static PyObject * wiic_foo(PyObject *self, PyObject* args) {
  wiimote** wiimotes;
	int found = 0;

	wiimotes =  wiic_init(MAX_WIIMOTES);

	while (!found) {
		found = wiic_find(wiimotes, MAX_WIIMOTES, 5);
		if (!found) {
	  	printf ("No wiimotes found.\n");
  		sleep(2);
		}
	}

	int connected = wiic_connect(wiimotes, found, 0);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}

	while (!exiting) {
		if (wiic_poll(wiimotes, connected)) {
		  int i = 0;
			for (; i < MAX_WIIMOTES; ++i) {
				switch (wiimotes[i]->event) {
					case WIIC_EVENT:
					  printf ("Event.\n");
						handle_event(wiimotes[i]);
						break;

					case WIIC_DISCONNECT:
					case WIIC_UNEXPECTED_DISCONNECT:
					  printf ("DISCONNECT.\n");
						handle_disconnect(wiimotes[i]);
						break;
						
					case WIIC_BALANCE_BOARD_INSERTED:
						printf("Balance Board connected.\n");
						break;
						
					case WIIC_BALANCE_BOARD_REMOVED:
						printf("Balance Board disconnected.\n");
						break;
						
					default:
					  printf ("Unknow Event %d.\n", wiimotes[i]->event);
					  handle_event(wiimotes[i]);
						break;
				}
			}
		}
	}
	wiic_cleanup(wiimotes, MAX_WIIMOTES);
	return PyLong_FromLong(42L);
}

static PyMethodDef wiic_methods[] = {
    {"foo", wiic_foo, METH_NOARGS, "Return the meaning of everything."},
    {NULL, NULL}
};

static struct PyModuleDef wiicmodule = {
    {}, /* m_base */
    "wiic",  /* m_name */
    0,  /* m_doc */
    0,  /* m_size */
    wiic_methods,  /* m_methods */
    0,  /* m_reload */
    0,  /* m_traverse */
    0,  /* m_clear */
    0,  /* m_free */
};

PyObject* PyInit_wiic() {
    return PyModule_Create(&wiicmodule);
}
