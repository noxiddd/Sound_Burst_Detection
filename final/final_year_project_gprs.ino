#include <stdio.h>  /* snprintf() */
#include <float.h>

#include "Adafruit_FONA.h"

#define BAUD_RATE 9600

/* GSM modem */

#define FONA_SERIAL   Serial3
#define FONA_RST_PIN  7

#define URL "69.160.108.53/homepage.php"
#define APN "web.digiceljamaica.com"

#define MAX_FLT_STR_LEN (3 + FLT_MANT_DIG - FLT_MIN_EXP)
/* 
 *  "distMod01=&distMod02=&distMod03=" is 32 characters
 *  maximum length of float base 10 string representation x 3
 */
#define MAX_CONTENT_LENGTH (32 + MAX_FLT_STR_LEN * 3)

static Adafruit_FONA fona = Adafruit_FONA(FONA_RST_PIN);

void
setup()
{
    uint16_t statuscode;
    uint16_t datalen;
    int content_length;
    char data[MAX_CONTENT_LENGTH + 1];
    char distMod01[MAX_FLT_STR_LEN + 1] = "30";
    char distMod02[MAX_FLT_STR_LEN + 1] = "40";
    char distMod03[MAX_FLT_STR_LEN + 1] = "50";
  
    Serial.begin(BAUD_RATE);

    FONA_SERIAL.begin(BAUD_RATE);

    if (!fona.begin(FONA_SERIAL)) {
        Serial.println("Couldn't find FONA");
        for (;;);
    }

    fona.setGPRSNetworkSettings(F(APN));
    Serial.println("delay start");
    delay(10000);
    Serial.println("delay stop");

    Serial.println("enabling GPRS");
    while (!fona.enableGPRS(true));
    Serial.println("GPRS enabled");

    content_length = snprintf(data, sizeof data, "distMod01=%s&distMod02=%s&distMod03=%s", distMod01, distMod02, distMod03);

   if (!fona.HTTP_POST_start(URL, F("application/x-www-form-urlencoded"), (uint8_t *) data, content_length, &statuscode, &datalen)) {}
       Serial.println("Failed!");
       
   }
  
   
void
loop()
{
   digitalWrite(13,HIGH);
   delay(100);
   digitalWrite(13,LOW);
   delay(100);
}
