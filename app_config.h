/**** User Selection *********/

/** Network setting **/

#define USE_DHCP				(1)					/* Select  0(static configuration) or 1(use DHCP) */
#if (USE_DHCP == 0)
	#define IP_ADDRESS			("192.168.0.2")		/* IP address */
	#define SUBNET_MASK			("255.255.255.0")	/* Subnet mask */
	#define DEFAULT_GATEWAY		("192.168.0.3")		/* Default gateway */
#endif

#define NETWORK_TYPE			(1)					/* Select  0(EthernetInterface) or 1(GR_PEACH_WlanBP3595) */
#if (NETWORK_TYPE == 1)
//	#define WLAN_SSID			("SSIDofYourAP")	/* SSID */
//	#define WLAN_PSK			("PSKofYourAP")		/* PSK(Pre-Shared Key) */

//	#define WLAN_SSID			("ERTL_GLOBAL")		/* SSID */
//	#define WLAN_PSK			("ertl__toppers")	/* PSK(Pre-Shared Key) */

//	#define WLAN_SSID			("RX501NC_0832")	/* SSID */
//	#define WLAN_PSK			("50220988")		/* PSK(Pre-Shared Key) */

//	#define WLAN_SSID			("planex-2.4-ea118c")	/* SSID */
//	#define WLAN_PSK			("12345678")		/* PSK(Pre-Shared Key) */

	#define WLAN_SECURITY		NSAPI_SECURITY_WPA2	/* NSAPI_SECURITY_NONE, NSAPI_SECURITY_WEP, NSAPI_SECURITY_WPA or NSAPI_SECURITY_WPA2 */
#endif

/** Camera setting **/

#if 0
#define VIDEO_INPUT_METHOD		(VIDEO_CMOS_CAMERA)	/* Select  VIDEO_CVBS or VIDEO_CMOS_CAMERA */
#else
#define VIDEO_INPUT_METHOD		(VIDEO_CVBS)		/* Select  VIDEO_CVBS or VIDEO_CMOS_CAMERA */
#endif

#define VIDEO_INPUT_FORMAT		(VIDEO_YCBCR422)	/* Select  VIDEO_YCBCR422 or VIDEO_RGB888 or VIDEO_RGB565 */
#define USE_VIDEO_CH			(0)					/* Select  0 or 1 If selecting VIDEO_CMOS_CAMERA, should be 0.) */
#define VIDEO_PAL				(1)					/* Select  0(NTSC) or 1(PAL) If selecting VIDEO_CVBS, this parameter is not referenced.) */

/** SD card spi channel setting **/

#define SD_SPICH			(2)
/*****************************/
