#include <kernel.h>
#include "kernel_cfg.h"
#include "app.h"

#include "mbed.h"

#include "app_config.h"

#include "Zumo.h"
#include "Milkcocoa.h"
#include "GR_PEACH_Camera.h"
#include "GR_PEACH_WlanBP3595.h"


#define SEND_MESSAGE "HELLO WORLD\r\n"
static void _wlan_inf_callback(uint8_t ucType, uint16_t usWid, uint16_t usSize,
		uint8_t *pucData);

class MySerial: Serial {
public:
	MySerial(PinName tx, PinName rx) :
			Serial(tx, rx) {
	}
	int send_binary(const void* buf, int length) {
		return write(buf, length);
	}
};

Serial pc(USBTX, USBRX);
GR_PEACH_Camera camera;
GR_PEACH_WlanBP3595 wlan;
DigitalOut red_led(LED1);
DigitalOut green_led(LED3);

void error_wait(int ret, const char* str) {
	if (ret != 0) {
		pc.printf(str);
		/* error */
		red_led = 1;
		while (1) {
			Thread::wait(1000);
		}
	}
}

void task_main(intptr_t exinf) {
	pc.baud(115200);
	camera.start();
	const char *buf;
//	unsigned int size = snapshot_req(&buf);
//	pc.printf("ready to file receive.");
//	dly_tsk(3000);
//	pc.send_binary(buf, size);
	wlan.setWlanCbFunction(_wlan_inf_callback);

	pc.printf("\r\ninitializing\r\n");
	int ret = wlan.init();
	error_wait(ret, "init");

	pc.printf("wlan connecting\r\n");
	ret = wlan.connect(WLAN_SSID, WLAN_PSK);
	error_wait(ret, "wifi connect");

	TCPSocketConnection socket;
	pc.printf("socket connecting\r\n");
	ret = socket.connect("192.168.179.101", 9000);
	error_wait(ret, "socket connect");

	char* str = strdup("hello world!\r\n");
	ret = socket.send(str, strlen(str));
	pc.printf("%s%d\r\n", str, ret);

	socket.close();
	wlan.disconnect();
}

static void _wlan_inf_callback(uint8_t ucType, uint16_t usWid, uint16_t usSize,
		uint8_t *pucData) {
	if (ucType == 'I') {
		if (usWid == 0x0005) {    // WID_STATUS
			if (pucData[0] == 0x01) {     // CONNECTED
				green_led = 1;
			} else {
				green_led = 0;
			}
		}
	}
}
