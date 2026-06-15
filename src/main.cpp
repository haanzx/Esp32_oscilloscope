/*
    Esp32_oscilloscope.cpp (Converted to PlatformIO)
    Based on project by Bojan Jurca: https://github.com/BojanJurca/Esp32_oscilloscope
*/

#include <Arduino.h> // Wajib di PlatformIO
#include <WiFi.h>

#define USE_FILE_SYSTEM // Beri komentar (//) jika tidak menggunakan LittleFS (maka web disajikan dari memori RAM)

// Configure the servers you want to use in server_config.h before including their header files.
#include "server_config.h" // first inclusion - configuration part

#include <ostream.hpp>          // convenience wrapper for using cout instead of Serial.print
#include <Cstring.hpp>          // C-style strings with C++ operators that reside on the stack (no heap allocation)

#ifdef USE_FILE_SYSTEM
    // Arduino library: ThreadSafeFS
    #include <LittleFS.h>               // or SPIFFS or FFat, ...
    #include <threadSafeFS.h>           // thread-safe wrapper file system wrapper
    threadSafeFS::FS TSFS (LittleFS);   // use thread-safe wrapper arround selected LittleFS
    using File = threadSafeFS::File;    // use thread-safe wrapper for all file operations in your code from now on
#else
    #include "amber_oscilloscope_html.h"     // use RAM version
#endif

// Create the default configuration files and read from them
#include "server_config.h" // second inclusion - implementation part

#include <httpServer.h>
httpServer_t *httpServer = NULL;

// Include oscilloscope
#define USE_I2S_INTERFACE             
#include "oscilloscope.h"

#ifdef USE_FILE_SYSTEM
    #include <ftpServer.h>
    ftpServer_t *ftpServer = NULL;
#endif

#ifdef USE_mDNS
    #include <ESPmDNS.h>
#endif

// Prototype deklarasi fungsi (Wajib di C++ murni/PlatformIO)
String httpRequestHandlerCallback (const char *httpRequest, httpServer_t::httpConnection_t *hcn);
void wsRequestHandlerCallback (const char *httpRequest, httpServer_t::webSocket_t *webSocket);

// ----- handle HTTP requests -----
String httpRequestHandlerCallback (const char *httpRequest, httpServer_t::httpConnection_t *hcn) { 
    // Must be reentrant !!!
    #define httpRequestIs(X) (strstr(httpRequest,X)==httpRequest)

    #ifdef USE_FILE_SYSTEM
        // if HTTP request is GET /oscilloscope.html HTTP server will fetch the file but let us redirect GET / and GET /index.html to it as well
        if (httpRequestIs ("GET / ") || httpRequestIs ("GET /index.html ")) {
            hcn->setHttpReplyHeaderField ("Location", "/oscilloscope.html");
            hcn->setHttpReplyStatus ("307 temporary redirect"); 
            return "Redirecting ..."; 
        }
    #else
        if (httpRequestIs ("GET / ") || httpRequestIs ("GET /index.html ") || httpRequestIs ("GET /oscilloscope.html ")) {
            return amber_oscilloscope_html;
        }
    #endif

    return ""; 
}

void wsRequestHandlerCallback (const char *httpRequest, httpServer_t::webSocket_t *webSocket) {
    // Must be reentrant !!!
    #define httpRequestIs(X) (strstr(httpRequest,X)==httpRequest)
    
    if (httpRequestIs ("GET /runOscilloscope"))      
        runOscilloscope (webSocket);
}

void setup () {
    // Initialize console output.
    cinit ();

    #ifdef USE_FILE_SYSTEM
        // If a file system is used, mount it now.
        LittleFS.begin (true);
    #endif

    // Connect to the WiFi router.
    WiFi.onEvent ([] (WiFiEvent_t event, WiFiEventInfo_t info) {
        if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
            cout << "[WiFi][STA] " "connected";
            #ifdef POWER_SAVING
                esp_err_t err = esp_wifi_set_ps (POWER_SAVING);
                if (err == ESP_OK)
                    cout << "[power saving] " "is on";
                else
                    cout << "[power saving] " "couldn't set power saving";
            #endif
        } else if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            cout << "[WiFi][STA] " "got IP address: " << WiFi.localIP ();

            #ifdef USE_mDNS
                if (MDNS.begin (HOSTNAME))
                    cout << "[mDNS] started for " << HOSTNAME;
                else
                    cout << "[mDNS] did not start";
                #ifdef USE_FILE_SYSTEM
                    MDNS.addService ("ftp", "tcp", 21);            
                #endif
                MDNS.addService ("http", "tcp", 80);
            #endif            
        }
    });

    #ifdef USE_FILE_SYSTEM
        WiFi_start (TSFS); 
    #else
        WiFi_start ();
    #endif

    // Start the HTTP server.
    #ifdef USE_FILE_SYSTEM
        httpServer = new (std::nothrow) httpServer_t (TSFS, 
                                                      httpRequestHandlerCallback, 
                                                      wsRequestHandlerCallback); 
    #else
        httpServer = new (std::nothrow) httpServer_t (httpRequestHandlerCallback, 
                                                      wsRequestHandlerCallback); 
    #endif

    if (httpServer && *httpServer)
        cout << "[httpServer] " "started";
    else
        cout << "[httpServer] " "did not start";

    #ifdef USE_FILE_SYSTEM
        // Start the FTP server.
        ftpServer = new (std::nothrow) ftpServer_t (TSFS);  
        if (ftpServer && *ftpServer)
            cout << "[ftpServer] " "started";
        else
            cout << "[ftpServer] " "did not start";
    #endif
}

void loop () {
    // Kosong (menggunakan task internal bawaan library)
}