#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"

#include "mbed.h"

#include "app_config.h"

#include "Milkcocoa.h"

#define WLAN_SSID               ("Xperia Z5 Compact_f316")                // SSID
#define WLAN_PSK                ("miyu7283")                 // PSK(Pre-Shared Key)
#define MILKCOCOA_APP_ID      "readiy2t3sw5"
#define MILKCOCOA_DATASTORE   "mbed"
#define MILKCOCOA_SERVERPORT  1883
const char MQTT_SERVER[]  = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] = __TIME__ MILKCOCOA_APP_ID;
const int MAX_SENSOR_VALUE = 1024;
const int IR_THRESHOLD = 200;

extern void onpush(MQTT::MessageData& md);
Serial pc(USBTX, USBRX);

DigitalOut dir_left(D8);
DigitalOut dir_right(D7);
PwmOut pwm_left(P5_0);      //TIOC0
PwmOut pwm_right(P8_14);    //TIOC2
static DigitalInOut ir_pins[] =
		{ DigitalInOut(D5), DigitalInOut(A2), DigitalInOut(A0), DigitalInOut(
				D11), DigitalInOut(A3), DigitalInOut(D4) };
static DigitalOut emitter(A4);
static Timer timer;
typedef struct IrBitField_S {
	int left :1;
	int center :1;
	int right :1;
} IrBitField_T;

static int speed = 80;
static int steer = 0;

static int constrain(int input, int min, int max) {
	if (input < min) {
		return min;
	} else if (max < input) {
		return max;
	} else {
		return input;
	}
}

void driveTank(int left, int right) {
	int _lmotor = constrain(left, -255, 255);
	int _rmotor = constrain(right, -255, 255);

	if (_lmotor < 0) {
		dir_left = 1;
		_lmotor = -_lmotor;
	} else {
		dir_left = 0;
	}
	if (_rmotor < 0) {
		dir_right = 1;
		_rmotor = -_rmotor;
	} else {
		dir_right = 0;
	}
	pwm_left.write(_lmotor / 255.0f);
	pwm_right.write(_rmotor / 255.0f);
}

void readAnalogIrValue(unsigned int *values) {
	int i;
	emitter = 1;
	wait_us(200);
	for (i = 0; i < 6; i++) {
		values[i] = MAX_SENSOR_VALUE - 1;
		ir_pins[i].output();
		ir_pins[i] = 1;
	}
	for (i = 0; i < 6; i++) {
		ir_pins[i].input();
		ir_pins[i] = 0;
	}
	timer.start();
	timer.reset();
	unsigned int time = 0;
	while (timer.read_us() < MAX_SENSOR_VALUE) {
		time = timer.read_us();
		for (i = 0; i < 6; i++) {
			if (ir_pins[i] == 0 && time < values[i]) {
				values[i] = time;
			}
		}
	}
	timer.stop();
	emitter = 0;
	wait_us(200);
}

void readIr(IrBitField_T &irbits) {
	unsigned int irvalues[6];
	readAnalogIrValue(irvalues);
	irbits.right = (irvalues[0] > IR_THRESHOLD);
	irbits.center = (irvalues[3] > IR_THRESHOLD);
	irbits.left = (irvalues[5] > IR_THRESHOLD);
}

void onpush(MQTT::MessageData& md)
{
	pc.printf("onpush\n\r");
    MQTT::Message &message = md.message;
    DataElement de = DataElement((char*)message.payload);
	int v = de.getInt("v");
}

void task_main(intptr_t exinf) {
	pc.baud(9600);
	IrBitField_T irbits;
	while (1) {
		readIr(irbits);
		if (irbits.left && irbits.center && irbits.right) {
			// on the end line. stop.
			driveTank(0, 0);
		} else if (irbits.left) {
			// turn strong right
			driveTank(-0.1 * speed, speed);
		} else if (irbits.right) {
			// turn strong left
			driveTank(speed, -0.1 * speed);
		} else if (irbits.center) {
			// move forward
			driveTank(speed, speed);
		} else {
			//turn right
			driveTank(speed, 0.1 * speed);
		}
		dly_tsk(100);
	}
}

