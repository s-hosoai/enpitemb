#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"

#include "mbed.h"

#include "app_config.h"

#include "Zumo.h"
#include "Milkcocoa.h"

extern void onpush(MQTT::MessageData& md);
Serial pc(USBTX, USBRX);
Zumo zumo;

static int speed = 80;
static int steer = 0;

void onpush(MQTT::MessageData& md)
{
	pc.printf("onpush\n\r");
    MQTT::Message &message = md.message;
    DataElement de = DataElement((char*)message.payload);
	int v = de.getInt("z");
}

void task_main(intptr_t exinf) {
	pc.baud(9600);
	pc.printf("program start.");
	MQTTBP3595 *ipstack = new MQTTBP3595(WLAN_SSID, WLAN_PSK);
	MClient *client = new MClient(ipstack);
	Milkcocoa *milkcocoa = new Milkcocoa(client, MQTT_SERVER,
			MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);
	pc.printf("connecting to milkcocoa");
	milkcocoa->connect();
	pc.printf("connected.");

	DataElement de = DataElement();
	de.setValue("z", "0");
	milkcocoa->push("zumo", de);
	zumo.driveTank(100, 100);
	dly_tsk(1000);
	zumo.driveTank(0, 0);
	de.setValue("z", "1");

	while (1) {
		milkcocoa->loop();
//		readIr(irbits);
//		if (irbits.left && irbits.center && irbits.right) {
//			// on the end line. stop.
//			driveTank(0, 0);
//		} else if (irbits.left) {
//			// turn strong right
//			driveTank(-0.1 * speed, speed);
//		} else if (irbits.right) {
//			// turn strong left
//			driveTank(speed, -0.1 * speed);
//		} else if (irbits.center) {
//			// move forward
//			driveTank(speed, speed);
//		} else {
//			//turn right
//			driveTank(speed, 0.1 * speed);
//		}
		dly_tsk(1000);
	}
}

