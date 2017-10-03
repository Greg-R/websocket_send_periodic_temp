#include "mgos.h"
#include "mgos_http_server.h"
#include "mgos_dht.h"

//  Clock update interval.
#define INTERVAL_SECONDS 1
//  POSIX time related data structures.
time_t maintime;
struct tm * localstruct;
char utctime[50];
char count;  //  Used to schedule temp and humidity updates.
//  JSON message which is sent to web page using mg_printf_websocket_frame().
char* timeString = "{\"messageType\":\"serverTime\",\"serverTime\":\"%s\"}";
char* tempString = "{\"messageType\":\"temperature\",\"currentTemp\":\"%2.1f\"}";
char* humidString = "{\"messageType\":\"humidity\",\"currentHumidity\":\"%2.1f\"}";
struct mgos_dht * dht11;
float tempC, humidity;

//  Using defaults for http options in http response.
struct mg_serve_http_opts http_opts = {.document_root = "."};

float ctof(float tempC) {
	float tempf;
	tempf = tempC * 9.0/5.0 + 32;
	return tempf;
}

//  This is the HTTP server handler.  It is also handling Websocket.
//  Not all of the possible events are being handled.
static void ev_handler(struct mg_connection *nc, int ev, void *ev_data, void *user_data) {
	struct http_message *hm = (struct http_message *) ev_data;
	switch (ev) {
	case MG_EV_HTTP_REQUEST: {
		mg_serve_http(nc, hm, http_opts);
		break;
	}
	case MG_EV_SEND: {
		// puts("MG_EV_SEND event fired!");
		break;
	}
	case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
		//  This sets the periodic time updating in motion.
		mg_set_timer(nc, mg_time() + INTERVAL_SECONDS);
		break;
	}
	case MG_EV_WEBSOCKET_FRAME: {
		// printf("Websocket message received:\n");
		break;
	}
	case MG_EV_CLOSE: {
		break;
	}
	//  The timer event causes a JSON string to be sent to the
	//  webpage via Websocket.
	case MG_EV_TIMER: {
		maintime = time(NULL);
		localstruct = localtime(&maintime);
//		printf("The seconds are %d.\n", localstruct->tm_sec);
		strftime(utctime, sizeof(utctime), "%A %B %d %T", localstruct);
		tempC = mgos_dht_get_temp(dht11);
//		printf("The temperature in degrees C is %2.1f.\n", ctof(tempC));
		humidity = mgos_dht_get_humidity(dht11);
//		printf("The humidity in percent is %2.1f.\n", humidity);
		// printf("Time is %s.\n", utctime);
		//  Update time every 1 second; temp and humidity every 10 seconds.
		mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, timeString, utctime);
		if (count == 10) {
		mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, tempString, ctof(tempC));
		mg_printf_websocket_frame(nc, WEBSOCKET_OP_TEXT, humidString, humidity);
		count = 0;
		}
		count++;
		//  A new timer is set.
		mg_set_timer(nc, mg_time() + INTERVAL_SECONDS);
		break;
	}
	}
}

enum mgos_app_init_result mgos_app_init(void) {
	struct mg_connection * nc;
	void * user_data = "";
	bool dhtInit;


	//  Get the server handle.
	if((nc = mgos_get_sys_http_server()) == NULL) {
		puts("The value of nc is NULL");
	}

	//  Bind the event handler to the HTTP server.
	mgos_register_http_endpoint("/", ev_handler, user_data);

	if((dht11 = mgos_dht_create(2, DHT22)) == NULL) {
		puts("DHT11 handle not created");
	}

	dhtInit = mgos_dht_init();
	if(dhtInit) {
		puts("DHT sensor initialized.");
	}

//	tempC = mgos_dht_get_temp(dht11);
//	printf("The initial temperature in degrees C is %2.1f.\n", tempC);

	return MGOS_APP_INIT_SUCCESS;
}

