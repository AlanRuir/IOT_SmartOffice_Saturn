#include "SmartConfig.h"

//智能配网，esp32提供了官方的智能配网功能，用户可以通过安信可微信公众号进行配网，.smartConfigDone()返回true则配网成功
void smartConfig()
{
	WiFi.mode(WIFI_STA);
	Serial.println("智能配网中...");
	WiFi.beginSmartConfig();

	while(true)
	{
		Serial.print(".");
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		delay(500);

		if(WiFi.smartConfigDone())
		{
			Serial.println("智能配网成功！");
			Serial.printf("热点账号：%s\n", WiFi.SSID().c_str());
			Serial.printf("热点密码：%s\n", WiFi.psk().c_str());
			digitalWrite(LED_BUILTIN, HIGH);
			break;
		}
	}
}

//自动配网，esp32根据以前联网的热点账号/密码来配置网络，询问20次，每次间隔1秒，若成功则立即返回true，若20次后仍未成功，则返回false
bool autoConfig()
{
	WiFi.begin();

	for(int i = 0; i < 20; ++i)
	{
		int wifiStatus = WiFi.status();

		if(WL_CONNECTED == wifiStatus)
		{
			Serial.println("自动配网成功！");
			Serial.printf("热点账号：%s\n", WiFi.SSID().c_str());
			Serial.printf("热点密码：%s\n", WiFi.psk().c_str());
			Serial.print("本机IP：");
			Serial.println(WiFi.localIP());
			Serial.printf("网关IP：");
			Serial.println(WiFi.gatewayIP());
			digitalWrite(LED_BUILTIN, HIGH);

			return true;
		}
		else
		{
			Serial.println("自动配网中...");
			digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
			delay(1000);
		}
	}

	Serial.println("自动配网失败！");
	return false;
}