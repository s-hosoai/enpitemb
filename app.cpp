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

GR_PEACH_WlanBP3595 wlan;
DigitalOut red_led(LED1);
DigitalOut green_led(LED2);

static void _wlan_inf_callback(uint8_t ucType, uint16_t usWid, uint16_t usSize, uint8_t *pucData);

void task_main(intptr_t exinf) {
	GR_PEACH_WlanBP3595 wlan;
	TCPSocketServer server;
	TCPSocketConnection connection;
	int ret;

	wlan.setWlanCbFunction(_wlan_inf_callback);
	ret = wlan.init(IP_ADDRESS, SUBNET_MASK, DEFAULT_GATEWAY);
	if (ret != 0) {
		red_led = 1;
		while (1) {
			dly_tsk(1000);
		}
	}

	ret = wlan.connect(WLAN_SSID, WLAN_PSK);
	if (ret != 0) {
		red_led = 1;
		while (1) {
			dly_tsk(1000);
		}
	}

	server.bind(SERVER_PORT);
	server.listen();

	while (1) {
		server.accept(connection);
		printf("Connection from: %s\n", connection.get_address());
		connection.send_all((char *) SEND_MESSAGE, sizeof(SEND_MESSAGE) - 1);
		connection.close();
	}
}

static void _wlan_inf_callback(uint8_t ucType, uint16_t usWid, uint16_t usSize,
		uint8_t *pucData) {
	if (ucType == 'I') {
		if (usWid == 0x0005) {
			if (pucData[0] == 0x01) {
				green_led = 1;
			} else {
				green_led = 0;
			}
		}
	}
}
