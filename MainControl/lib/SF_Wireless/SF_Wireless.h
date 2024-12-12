#ifndef _SF_Wireless_h
#define _SF_Wireless_h


#include <WiFi.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include "SF_BLDC_shared_struct.h"


class SF_Wireless{
    public:
        SF_Wireless(const char* ssid, const char* password);

        WiFiUDP UDP;//创建Udp

        void WiFiInit(IPAddress ipClient, IPAddress ipServidor,unsigned int location);
        SF_BLDC_Wireless_DATA recFromWireless();
        void sendToWireless(SF_BLDC_DATA& values);
        SF_BLDC_Wireless_DATA recValues;

    
    private:
        const char* _ssid;
        const char* _password;
        IPAddress _ipClient;
        IPAddress _ipServidor;
        IPAddress _subnet; // 不在这里初始化
        unsigned int _location ;//通讯端口

        float motorNum;
        float P,I,D;

        uint8_t M0_mode, M1_mode;
        float M0_target, M1_target;
        float M1_VelP,M1_VelI,M1_VelD;
        float M0_AngleP,M0_AngleI,M0_AngleD;
        float M1_AngleP,M1_AngleI,M1_AngleD;
        float M0_CurrentP,M0_CurrentI,M0_CurrentD;
        float M1_CurrentP,M1_CurrentI,M1_CurrentD;
};

#endif


