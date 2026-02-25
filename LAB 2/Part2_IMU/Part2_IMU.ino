/************************************************************
 * Example1_Basics.ino
 * ICM 20948 Arduino Library Demo
 * Use the default configuration to stream 9-axis IMU data
 * Owen Lyke @ SparkFun Electronics
 * Original Creation Date: April 17 2019
 *
 * Please see License.md for the license information.
 *
 * Distributed as-is; no warranty is given.
 ************************************************************/
#include "ICM_20948.h" // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU
#include <Adafruit_NeoPixel.h>
#define NUM_LEDS    1     // Only one LED on board
#define LED_PIN     8     // Data pin connected to onboard RGB LED

#define SERIAL_PORT Serial
#define WIRE_PORT Wire // Your desired Wire port. Used when "USE_SPI" is not defined
// The value of the last bit of the I2C address.
// On the SparkFun 9DoF IMU breakout the default is 1, and when the ADR jumper is closed the value becomes 0
#define AD0_VAL 1
ICM_20948_I2C myICM; // Otherwise create an ICM_20948_I2C object
Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);



void setup()
{
  
    SERIAL_PORT.begin(115200);
    pixel.setBrightness(50);
    pixel.begin(); 
    while (!SERIAL_PORT)
    {
    };
    WIRE_PORT.begin(23,22);
    WIRE_PORT.setClock(400000);

    //myICM.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

    bool initialized = false;
    while (!initialized)
    {
        myICM.begin(WIRE_PORT, AD0_VAL);
        SERIAL_PORT.print(F("Initialization of the sensor returned: "));
        SERIAL_PORT.println(myICM.statusString());
        if (myICM.status != ICM_20948_Stat_Ok)
        {
            SERIAL_PORT.println("Trying again...");
            delay(500);
        }
        else
        {
            initialized = true;
        }
    }
}

void loop()
{
    if (myICM.dataReady())
    {
        myICM.getAGMT();
        if(myICM.accX() < -500){
          pixel.setPixelColor(0, pixel.Color(0, 0, 255)); // Display Red// LED turns blue
          pixel.show(); 
        }
        if(myICM.accX() > 500){
          pixel.setPixelColor(0, pixel.Color(255, 0, 0)); // Display Red// LED turns red
          pixel.show(); 
        }
        if(myICM.accY() < -500){
          pixel.setPixelColor(0, pixel.Color(0, 255, 0)); // Display Red// LED turns yello
          pixel.show(); 
        }
        if(myICM.accY() > 500){
          pixel.setPixelColor(0, pixel.Color(100, 0, 255)); // Display Red// LED turns purple
          pixel.show(); 
        }
        if(abs(myICM.accX()) < 500 && abs(myICM.accY()) < 500 && myICM.accZ() > 1200){
          pixel.setPixelColor(0, pixel.Color(0, 255, 170)); // Display Red// LED turns purple
          pixel.show(); 
        }
        if(abs(myICM.accX()) < 500 && abs(myICM.accY()) < 500 && myICM.accZ() < 800){
          pixel.setPixelColor(0, pixel.Color(170, 255, 0)); // Display Red// LED turns purple
          pixel.show(); 
        }
        
                // The values are only updated when you call `getAGMT`
        //    printRawAGMT( myICM.agmt );  // Uncomment this to see the raw values, taken directly from the agmt structure
        printScaledAGMT(&myICM); // This function takes into account the scale settings from when the measurement was made to calculate the values with units
        delay(30);
    }
    else
    {
        SERIAL_PORT.println("Waiting for data");
        delay(500);
    }
}
void printFormattedFloat(float val, uint8_t leading, uint8_t decimals)
{
    float aval = abs(val);
    if (val < 0)
    {
        SERIAL_PORT.print("-");
    }
    else
    {
        SERIAL_PORT.print(" ");
    }
    for (uint8_t indi = 0; indi < leading; indi++)
    {
        uint32_t tenpow = 0;
        if (indi < (leading - 1))
        {
            tenpow = 1;
        }
        for (uint8_t c = 0; c < (leading - 1 - indi); c++)
        {
            tenpow *= 10;
        }
        if (aval < tenpow)
        {
            SERIAL_PORT.print("0");
        }
        else
        {
            break;
        }
    }
    if (val < 0)
    {
        SERIAL_PORT.print(-val, decimals);
    }
    else
    {
        SERIAL_PORT.print(val, decimals);
    }
}

void printScaledAGMT(ICM_20948_I2C *sensor)
{
    SERIAL_PORT.print("Scaled. Acc (mg) [ ");
    printFormattedFloat(sensor->accX(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->accY(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->accZ(), 5, 2);
    SERIAL_PORT.print(" ], Gyr (DPS) [ ");
    printFormattedFloat(sensor->gyrX(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->gyrY(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->gyrZ(), 5, 2);
    SERIAL_PORT.print(" ], Mag (uT) [ ");
    printFormattedFloat(sensor->magX(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->magY(), 5, 2);
    SERIAL_PORT.print(", ");
    printFormattedFloat(sensor->magZ(), 5, 2);
    SERIAL_PORT.print(" ], Tmp (C) [ ");
    printFormattedFloat(sensor->temp(), 5, 2);
    SERIAL_PORT.print(" ]");
    SERIAL_PORT.println();
}