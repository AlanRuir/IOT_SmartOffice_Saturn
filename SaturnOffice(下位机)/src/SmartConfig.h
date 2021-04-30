#ifndef SMARTCONFIG_H
#define SMARTCONFIG_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

//智能配网，esp32提供了官方的智能配网功能，用户可以通过安信可微信公众号进行配网，.smartConfigDone()返回true则配网成功
void smartConfig();

//自动配网，esp32根据以前联网的热点账号/密码来配置网络，询问20次，每次间隔1秒，若成功则立即返回true，若20次后仍未成功，则返回false
bool autoConfig();

#endif