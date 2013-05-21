#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiic.h>

#define MAX_WIIMOTES 4

int exiting = 0;

void handle_event(struct wiimote_t* wm) {
	//printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A))		printf("A pressed\n");
	
	//if(wm->exp.type == EXP_BALANCE_BOARD) {
	if (wm->unid == 1) {
		struct balance_board_t* bb = (balance_board_t*)&wm->exp.bb;
		
		printf("balance board top left weight: %f\n", bb->pressure_weight.top_left);
		printf("balance board top right weight: %f\n", bb->pressure_weight.top_right);
		printf("balance board bottom left weight: %f\n", bb->pressure_weight.bottom_left);
		printf("balance board bottom right weight: %f\n", bb->pressure_weight.bottom_right);
		printf("balance board total weight: %f\n", bb->pressure_weight.weight);
	}
}

void handle_read(struct wiimote_t* wm, byte* data, unsigned short len) {
	int i = 0;

	printf("\n\n--- DATA READ [wiimote id %i] ---\n", wm->unid);
	printf("finished read of size %i\n", len);
	for (; i < len; ++i) {
		if (!(i%16))
			printf("\n");
		printf("%x ", data[i]);
	}
	printf("\n\n");
}

void handle_ctrl_status(struct wiimote_t* wm) {
	printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);

	printf("attachment:      %i\n", wm->exp.type);
	printf("speaker:         %i\n", WIIC_USING_SPEAKER(wm));
	printf("ir:              %i\n", WIIC_USING_IR(wm));
	printf("leds:            %i %i %i %i\n", WIIC_IS_LED_SET(wm, 1), WIIC_IS_LED_SET(wm, 2), WIIC_IS_LED_SET(wm, 3), WIIC_IS_LED_SET(wm, 4));
	printf("battery:         %f %%\n", wm->battery_level);
}

void handle_disconnect(wiimote* wm) {
	printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
	exiting = 1;
}

int main(int argc, char** argv) {
	wiimote** wiimotes;
	wiimote* wiimote;
	int found, connected;

	wiimotes =  wiic_init(MAX_WIIMOTES);

	found = wiic_find(wiimotes, MAX_WIIMOTES, 5);
	if (!found) {
		printf ("No wiimotes found.");
		return 0;
	}
	
	connected = wiic_connect(wiimotes, found, 0);
	if (connected)
		printf("Connected to %i wiimotes (of %i found).\n", connected, found);
	else {
		printf("Failed to connect to any wiimote.\n");
		return 0;
	}

	while (!exiting) {
		if (wiic_poll(wiimotes, connected)) {
		
		wiimote = wiimotes[0];
			
				switch (wiimote->event) {
					case WIIC_EVENT:
						handle_event(wiimote);
						printf("EVENT.\n");
						break;

					case WIIC_STATUS:
						handle_ctrl_status(wiimote);
						printf("STATUS.\n");
						break;

					case WIIC_DISCONNECT:
					case WIIC_UNEXPECTED_DISCONNECT:
					printf("DISCONNECT.\n");
						handle_disconnect(wiimote);
						break;

					case WIIC_READ_DATA:
            printf("READ.\n");
						break;
						
					case WIIC_BALANCE_BOARD_INSERTED:
						printf("Balance Board connected.\n");
						break;
						
					case WIIC_BALANCE_BOARD_REMOVED:
						printf("Balance Board disconnected.\n");
						break;
					
					case 0:
					  printf ("NULLEVENT.\n");
						switch (wiimote->unid) {
              case 1:						
					      handle_event(wiimote);
					      break;
					    default:
    						printf ("Unknow unid %d.\n", wiimote->unid);
		            break;			    
						}
					  break;
						
					default:
					  printf ("Unknow Case %d.\n", wiimote->event);
						

						break;
			}
		}
	}
	wiic_cleanup(wiimotes, MAX_WIIMOTES);
	return 0;
}
