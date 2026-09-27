#include "touchCalibration.h"

#include <Arduino.h>
#include <TFT_eSPI.h>


// ==================================================
// استفاده از TFT اصلی که در main.cpp ساخته شده
// ==================================================

extern TFT_eSPI tft;


// ==================================================
// TOUCH CALIBRATION
// ==================================================

extern "C"
{

void touch_calibration_start(void)
{
    uint16_t calData[5];


    // ==================================================
    // شروع Calibration
    // ==================================================

    Serial.println();
    Serial.println("--------------------------------");
    Serial.println("TOUCH CALIBRATION START");
    Serial.println("--------------------------------");


    // ==================================================
    // پاک کردن صفحه
    // ==================================================

    tft.fillScreen(
        TFT_BLACK
    );


    // ==================================================
    // متن راهنما
    // ==================================================

    tft.setCursor(
        20,
        0
    );


    tft.setTextFont(
        2
    );


    tft.setTextSize(
        1
    );


    tft.setTextColor(
        TFT_WHITE,
        TFT_BLACK
    );


    tft.println(
        "Touch corners as indicated"
    );


    tft.setTextFont(
        1
    );


    tft.println();


    // ==================================================
    // شروع Calibration واقعی
    // ==================================================

    tft.calibrateTouch(
        calData,
        TFT_MAGENTA,
        TFT_BLACK,
        15
    );


    // ==================================================
    // Calibration کامل شد
    // ==================================================

    Serial.println();
    Serial.println(
        "Touch calibration completed"
    );


    Serial.println(
        "New calibration values:"
    );


    Serial.print(
        "uint16_t calData[5] = { "
    );


    for (
        uint8_t i = 0;
        i < 5;
        i++
    )
    {
        Serial.print(
            calData[i]
        );


        if (
            i < 4
        )
        {
            Serial.print(
                ", "
            );
        }
    }


    Serial.println(
        " };"
    );


    Serial.println(
        "Applying new calibration..."
    );


    // ==================================================
    // اعمال Calibration جدید
    // ==================================================

    tft.setTouch(
        calData
    );


    // ==================================================
    // نمایش نتیجه
    // ==================================================

    tft.fillScreen(
        TFT_BLACK
    );


    tft.setCursor(
        10,
        10
    );


    tft.setTextFont(
        2
    );


    tft.setTextSize(
        1
    );


    tft.setTextColor(
        TFT_GREEN,
        TFT_BLACK
    );


    tft.println(
        "Calibration complete!"
    );


    tft.setTextFont(
        1
    );


    tft.println();


    tft.println(
        "New calibration applied."
    );


    // ==================================================
    // کمی مکث برای نمایش نتیجه
    // ==================================================

    delay(
        1000
    );


    Serial.println(
        "TOUCH CALIBRATION END"
    );

    Serial.println(
        "Returning to SETTINGS..."
    );

    Serial.println(
        "--------------------------------"
    );

    Serial.println();
}

}