/****************************************************************************************************************************
  WebServer_ESP32_SC_ENC_Debug.h

  For Ethernet shields using ESP32_SC_ENC (ESP32_S2/S3/C3 + LwIP ENC28J60)

  WebServer_ESP32_SC_ENC28J60 is a library for the ESP32_S2/S3/C3 with LwIP Ethernet ENC28J60

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/WebServer_ESP32_SC_ENC
  Licensed under GPLv3 license

  Version: 1.2.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      13/12/2022 Initial coding for ESP32_S3_ENC (ESP32_S3 + LwIP ENC28J60)
  1.1.0   K Hoang      19/12/2022 Add support to ESP32_S2_ENC (ESP32_S2 + LwIP ENC28J60)
  1.2.0   K Hoang      20/12/2022 Add support to ESP32_C3_ENC (ESP32_C3 + LwIP ENC28J60)
  1.2.1   K Hoang      11/01/2023 Increase default SPI clock to 20MHz from 8MHz
 *****************************************************************************************************************************/

#pragma once

#ifndef WEBSERVER_ESP32_SC_ENC_DEBUG_H
    #define WEBSERVER_ESP32_SC_ENC_DEBUG_H

///////////////////////////////////////

    #include <stdio.h>

///////////////////////////////////////

    #ifdef DEBUG_ETHERNET_WEBSERVER_PORT
        #define ET_DEBUG_OUTPUT DEBUG_ETHERNET_WEBSERVER_PORT
    #else
        #define ET_DEBUG_OUTPUT Serial
    #endif

///////////////////////////////////////

// Change _ETHERNET_WEBSERVER_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

    #ifndef _ETHERNET_WEBSERVER_LOGLEVEL_
        #define _ETHERNET_WEBSERVER_LOGLEVEL_ 0
    #endif

///////////////////////////////////////

const char EWS_MARK[]  = "[EWS] ";
const char EWS_SPACE[] = " ";
const char EWS_LINE[]  = "========================================\n";

    #define EWS_PRINT_MARK EWS_PRINT(EWS_MARK)
    #define EWS_PRINT_SP   EWS_PRINT(EWS_SPACE)
    #define EWS_PRINT_LINE EWS_PRINT(EWS_LINE)

    #define EWS_PRINT      ET_DEBUG_OUTPUT.print
    #define EWS_PRINTLN    ET_DEBUG_OUTPUT.println

///////////////////////////////////////

    #define ET_LOG(x)       \
        {                   \
            EWS_PRINTLN(x); \
        }
    #define ET_LOG0(x)    \
        {                 \
            EWS_PRINT(x); \
        }
    #define ET_LOG1(x, y)   \
        {                   \
            EWS_PRINT(x);   \
            EWS_PRINTLN(y); \
        }
    #define ET_LOG2(x, y, z) \
        {                    \
            EWS_PRINT(x);    \
            EWS_PRINT(y);    \
            EWS_PRINTLN(z);  \
        }
    #define ET_LOG3(x, y, z, w) \
        {                       \
            EWS_PRINT(x);       \
            EWS_PRINT(y);       \
            EWS_PRINT(z);       \
            EWS_PRINTLN(w);     \
        }

///////////////////////////////////////

    #define ET_LOGERROR(x)                     \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
        }
    #define ET_LOGERROR_LINE(x)                \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
            EWS_PRINT_LINE;                    \
        }
    #define ET_LOGERROR0(x)                    \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT(x);                      \
        }
    #define ET_LOGERROR1(x, y)                 \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(y);                    \
        }
    #define ET_LOGERROR2(x, y, z)              \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(z);                    \
        }
    #define ET_LOGERROR3(x, y, z, w)           \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 0) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(z);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(w);                    \
        }

///////////////////////////////////////

    #define ET_LOGWARN(x)                      \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
        }
    #define ET_LOGWARN_LINE(x)                 \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
            EWS_PRINT_LINE;                    \
        }
    #define ET_LOGWARN0(x)                     \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT(x);                      \
        }
    #define ET_LOGWARN1(x, y)                  \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(y);                    \
        }
    #define ET_LOGWARN2(x, y, z)               \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(z);                    \
        }
    #define ET_LOGWARN3(x, y, z, w)            \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 1) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(z);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(w);                    \
        }

///////////////////////////////////////

    #define ET_LOGINFO(x)                      \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
        }
    #define ET_LOGINFO_LINE(x)                 \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
            EWS_PRINT_LINE;                    \
        }
    #define ET_LOGINFO0(x)                     \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT(x);                      \
        }
    #define ET_LOGINFO1(x, y)                  \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(y);                    \
        }
    #define ET_LOGINFO2(x, y, z)               \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(z);                    \
        }
    #define ET_LOGINFO3(x, y, z, w)            \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 2) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(z);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(w);                    \
        }

///////////////////////////////////////

    #define ET_LOGDEBUG(x)                     \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
        }
    #define ET_LOGDEBUG_LINE(x)                \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINTLN(x);                    \
            EWS_PRINT_LINE;                    \
        }
    #define ET_LOGDEBUG0(x)                    \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT(x);                      \
        }
    #define ET_LOGDEBUG1(x, y)                 \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(y);                    \
        }
    #define ET_LOGDEBUG2(x, y, z)              \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(z);                    \
        }
    #define ET_LOGDEBUG3(x, y, z, w)           \
        if (_ETHERNET_WEBSERVER_LOGLEVEL_ > 3) \
        {                                      \
            EWS_PRINT_MARK;                    \
            EWS_PRINT(x);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(y);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINT(z);                      \
            EWS_PRINT_SP;                      \
            EWS_PRINTLN(w);                    \
        }

///////////////////////////////////////

#endif  // WEBSERVER_ESP32_SC_ENC_DEBUG_H
