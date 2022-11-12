// turn on debug messages
#define VERBOSE

#include "EloquentSurveillance.h"
#include "TelegramChat.h"

#define WIFI_SSID "DARKMATTER"
#define WIFI_PASS "8848191317"

#define USER_TOKEN "vxQ3IAW3JAisAXul7tcc9UOjx60Bxxxx"
#define DEVICE_TOKEN "UnsaA0bYOaklxxxx"

#define BOT_TOKEN "5382193647:AAHjstcfcEkQKH6ocop-l3L55Y7lssxxaxx"
#define CHAT_ID 5743309xxx


/**
 * Instantiate motion detector
 */
EloquentSurveillance::Motion motion;
/**
 * Instantiation dashboard connector
 */
EloquentSurveillance::Dashboard dashboard(USER_TOKEN, DEVICE_TOKEN);
/**
 * Instantiation telegram 
 */
EloquentSurveillance::TelegramChat chat(BOT_TOKEN, CHAT_ID);

/**
 *
 */
void setup() 
{
    Serial.begin(115200);
    delay(3000);
    debug("INFO", "Init");

    /**
       * Configure camera model
       * You have access to the global variable `camera`
       * Allowed values are:
       *  - aithinker()
       *  - m5()
       *  - m5wide()
       *  - eye()
       *  - wrover()
       */
    camera.aithinker(); 
    camera.vga();
    camera.highQuality();

    /**
     * See MotionDetectionExample for more details
     */
    motion.setMinChanges(0.1);
    motion.setMinPixelDiff(10);
    motion.setMinSizeDiff(0.05);
    motion.debounce(10000L);

    /**
     * Initialize the camera
     * If something goes wrong, print the error message
     */
    while (!camera.begin())
        debug("ERROR", camera.getErrorMessage());

    /**
     * Connect to WiFi
     * If something goes wrong, print the error message
     */
    while (!wifi.connect(WIFI_SSID, WIFI_PASS))
        debug("ERROR", wifi.getErrorMessage());

    debug("SUCCESS", "Camera OK");
}

/**
 *
 */
void loop() 
{
    /**
     * Try to capture a frame
     * If something goes wrong, print the error message
     */
    if (!camera.capture()) {
        debug("ERROR", camera.getErrorMessage());
        return;
    }

    /**
     * Await for motion
     */
    if (!motion.update())
      return;
    if (motion.detect()) 
    {
        debug("INFO", String("Motion detected in ") + motion.getExecutionTimeInMicros() + " us");

        bool messageResponse = chat.sendMessage("Motion detected");
        debug("TELEGRAM MSG", messageResponse ? "OK" : "ERR");

        //Uncomment Below lines to send photo along with the notification in telegram
        //bool photoResponse = chat.sendPhoto();
        //debug("TELEGRAM PHOTO", photoResponse ? "OK" : "ERR");

    }
    if (!motion.detect())
      return;

    /**
     * Upload motion frames to dashboard.
     * You can change 5 with any value you like, even 1
     */
     if (dashboard.sendMany(5))
      debug("SUCCESS", "Frames uploaded to dashboard");
     else
      debug("ERROR", "Upload error");
}
        
    
