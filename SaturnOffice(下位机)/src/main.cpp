#include <Arduino.h>
#include <PubSubClient.h>
#include <string>
#include <SimpleDHT.h>
#include "SmartConfig.h"

#define LIGHT 12
#define AIRFAN 13
#define SWITCH1 0
#define SWITCH2 10
#define SWITCH3 14
#define DTHPIN 5

SimpleDHT11 dht11(DTHPIN);
const char *ONENET_SERVER = "183.230.40.39";		//OneNET服务器地址
const int ONENET_PORT = 6002;		//OneNET服务器端口
const char *MQTT_DEVICE_ID = "684859260";		//OneNET的设备ID
const char *MQTT_PRODUCT_ID = "404868";			//OneNET的产品ID
const char *MQTT_PASSWORD = "Saturn001";		//OneNET的鉴权信息
long lastMsg = 0;
char msgBuff[600];
char dataTemplate[] = "{\"AirFanSwitch\":%d, \"AirFanLevel\":%d, \"Temperature\":%d, \"Humidity\":%d, \"LightSwitch\":%d, \"LightLevel\":%d, \"Switch1\":%d, \"Switch2\":%d, \"Switch3\":%d}";
char msgJson[300];
char debugBuff[600];
unsigned short jsonLength = 0;
uint8_t debugBuffStartIndex = 0;
int currentLightLevel = 0;
int currentAirFanLevel = 0;

WiFiClient espClient;		//WiFiClient对象
PubSubClient client(espClient);		//MQTT对象，参数是WiFiClient对象

std::pair<std::string, int> split(std::string str, std::string pat)
{
	std::pair<std::string, int> result;

	int index = str.find(pat);
	std::string key = str.substr(0, index);
}

/**
 * 回调函数
 * topic:主题
 * payload:消息体
*/
void callback(char *topic, byte *payload, unsigned int length)
{
	std::string callbackMsg = "";

	Serial.print("消息到达[");
	Serial.print(topic);
	Serial.print("]:");

	for(size_t i = 0; i < length; ++i)
	{
		Serial.print((char)payload[i]);
		callbackMsg += (char)payload[i];
	}
	Serial.println();

	int index = callbackMsg.find(":");
	String callbackMsgStr = callbackMsg.c_str();
	String key = callbackMsgStr.substring(0, index);
	String valueStr = callbackMsgStr.substring(index + 1);
	int value = valueStr.toInt();

	if((key.equals("AirFanSwitch")) && (0 == value))
	{
		currentAirFanLevel = 0;
		analogWrite(AIRFAN, currentAirFanLevel);
	}

	if((key.equals("AirFanSwitch")) && (1 == value))
	{
		currentAirFanLevel = 300;
		analogWrite(AIRFAN, currentAirFanLevel);
	}

	if(key.equals("AirFanLevel"))
	{
		currentAirFanLevel = value;
		analogWrite(AIRFAN, currentAirFanLevel);
	}

	if((key.equals("LightSwitch")) && (0 == value))
	{
		currentLightLevel = 0;
		analogWrite(LIGHT, currentLightLevel);
	}

	if((key.equals("LightSwitch")) && (1 == value))
	{
		currentLightLevel = 300;
		analogWrite(LIGHT, currentLightLevel);
	}

	if(key.equals("LightLevel"))
	{
		currentLightLevel = value;
		analogWrite(LIGHT, currentLightLevel);
	}

	if(key.equals("Switch1"))
	{
		if(1 == value)
		{
			digitalWrite(SWITCH1, HIGH);
		}
		else
		{
			digitalWrite(SWITCH1, LOW);
		}
	}

	if(key.equals("Switch2"))
	{
		if(1 == value)
		{
			digitalWrite(SWITCH2, HIGH);
		}
		else
		{
			digitalWrite(SWITCH2, LOW);
		}
	}

	if(key.equals("Switch3"))
	{
		if(1 == value)
		{
			digitalWrite(SWITCH3, HIGH);
		}
		else
		{
			digitalWrite(SWITCH3, LOW);
		}
	}
}

//服务器的连接
void reconnect()		
{
	while(!client.connected())
	{
		Serial.println("正在连接MQTT服务器...");
		if(client.connect(MQTT_DEVICE_ID, MQTT_PRODUCT_ID, MQTT_PASSWORD))
		{
			Serial.println("连接成功！");
		}
		else
		{
			Serial.print("连接失败！状态码：");
			Serial.print(client.state());
			Serial.println(" 5秒后再次尝试...");
			delay(5000);
		}
	}
}

int currentSwitch(byte level)
{
	if(level > 0)
	{
		return 1;
	}

	return 0;
}

void setup() 
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LIGHT, OUTPUT);
	pinMode(AIRFAN, OUTPUT);
	pinMode(SWITCH1, OUTPUT);
	pinMode(SWITCH2, OUTPUT);
	pinMode(SWITCH3, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	analogWrite(LIGHT, 0);
	analogWrite(AIRFAN, 0);
	digitalWrite(SWITCH1, LOW);
	digitalWrite(SWITCH2, LOW);
	digitalWrite(SWITCH3, LOW);

	if(!autoConfig())
	{
		smartConfig();
	}

	client.setServer(ONENET_SERVER, ONENET_PORT);
	client.setCallback(callback);
}

void loop() 
{
	if(!client.connected())
	{
		reconnect();
	}

	client.loop();		//客户端一直循环监听服务器的消息

	long currentTime = millis();
	if((currentTime - lastMsg) > 10000)
	{
		lastMsg = currentTime;
	}

	byte currentTemperature;
	byte currentHumidity;
	dht11.read(&currentTemperature, &currentHumidity, NULL);
	delay(500);
	Serial.printf("当前温度：%d摄氏度， 当前湿度%d%\n", (int)currentTemperature, (int)currentHumidity);

	snprintf(msgJson, 200, dataTemplate, currentSwitch(currentAirFanLevel), currentAirFanLevel, (int)currentTemperature, (int)currentHumidity, currentSwitch(currentLightLevel), currentLightLevel, digitalRead(SWITCH1), digitalRead(SWITCH2), digitalRead(SWITCH3));

	memset(msgBuff, 0, 600);
	jsonLength = strlen(msgJson);

	msgBuff[0] = char(0x03);		//OneNET规定的报文使用协议
	msgBuff[1] = char(jsonLength >> 8);		
	msgBuff[2] = char(jsonLength & 0xff);		//OneNET规定的报文长度，共占两个字节

	memcpy(msgBuff + 3, msgJson, strlen(msgJson));

	client.publish("$dp", (uint8_t *)msgBuff, 3 + strlen(msgJson));
}

/**
消息到达[$creq/e837fc69-13f6-551a-bc03-f573c58c7711]:AirFanSwitch:0
消息到达[$creq/542b2639-ec0c-5aca-8365-cdc112fed08b]:AirFanLevel:10
消息到达[$creq/3e9e7b28-54c9-5228-b7a0-848895b8e88c]:LightSwitch:0
消息到达[$creq/0cb3ad13-69ba-5076-b161-be69a37983eb]:LightLevel:964
消息到达[$creq/c27d0d10-c097-5d08-a008-12060f841270]:Switch1:0
消息到达[$creq/a8b56341-8b2e-5155-b9c5-9c68346e9d9c]:Switch2:1
消息到达[$creq/6ab9a089-6b02-508b-a7e5-f2d72ddf68c5]:Switch3:1
*/