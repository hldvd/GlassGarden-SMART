#pragma once

/*
------------------------------------------------------------
GlassGarden

File : NetworkManager.h

وظیفه:
مدیریت ارتباط WiFi

Version : 1.0.0
------------------------------------------------------------
*/

#pragma once

class NetworkManager
{
public:
    void begin();
    void update();
    bool isConnected() const;

private:
    bool connected = false;
    unsigned long reconnectTimer = 0;
};

extern NetworkManager network;