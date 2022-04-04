#include <Arduino.h>

// gpio pin for battery analog reading
#define VBATPIN A9
// gpio pin of the solenoid lock
int LOCK = 12;
// gpio pin of the external hall sensor
int externalHALL = 34;
// gpio pin for led
int LED = 27;

// function for lock
void toggleSolenoid(bool input)
{
    if (input == true)
    {
        digitalWrite(LOCK, HIGH);
    }
    else
    {
        digitalWrite(LOCK, LOW);
    }
}

// function for LED
void toggleLED(bool input)
{
    if (input == true)
    {
        digitalWrite(LED, HIGH);
    }
    else
    {
        digitalWrite(LED, LOW);
    }
}

// FIRST function to detect lock insertion (checking internal hall sensor)
bool lockInsertion_hall()
{
    float x = 0;
    for (int i = 0; i < 10000; i++)
    {
        x += hallRead();
        delayMicroseconds(100);
    }
    x = x / 10000.000000;

    if (x < 2) // trolley 1 uses 2, trolley 2 uses 1.3
    {
        Serial.print("hall value is: ");
        Serial.println(x);
        return true;
    }
    else
    {
        Serial.print("hall value is: ");
        Serial.println(x);
        return false;
    }
}
// SECOND function to detect lock insertion (checking battery level increase)
bool lockInsertion_battery()
{
    float total_battery1 = 0;
    for (int k = 0; k < 1000; k++)
    {

        double measuredvbat = analogRead(VBATPIN);
        measuredvbat *= 2;    // we divided by 2, so multiply back
        measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
        measuredvbat /= 1024; // convert to voltage
        total_battery1 += measuredvbat;
        delayMicroseconds(10);
    }
    total_battery1 = total_battery1 / 1000;

    Serial.print("vbatt1 is: ");
    Serial.println(total_battery1);

    delay(3000);

    float total_battery2 = 0;
    for (int k = 0; k < 1000; k++)
    {

        double measuredvbat = analogRead(VBATPIN);
        measuredvbat *= 2;    // we divided by 2, so multiply back
        measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
        measuredvbat /= 1024; // convert to voltage
        total_battery2 += measuredvbat;
        delayMicroseconds(10);
    }
    total_battery2 = total_battery2 / 1000;

    Serial.print("vbatt2 is: ");
    Serial.println(total_battery2);

    if ((total_battery2 - total_battery1) > 0.01)
    {
        Serial.println("vbatt2 is larger than vbatt1, its charging!");
        return true;
    }
    else
    {
        Serial.println("vbatt1 is still smaller than vbatt2, no charging.... :(");
        return false;
    }
}

// function for external hall sensor
bool flapConnected()
{
    long x = 0;
    for (int i = 0; i < 1000; i++)
    {
        x += analogRead(externalHALL);
        delayMicroseconds(100);
    }
    x = (double)x / 1000.;
    Serial.print("External hall sensor value:");
    Serial.println(x);

    if (x > 2100) // trolley 1 uses >2100, trolley 2 uses > 2000
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
bool lockInsertion_battery1()
{
    float testList1[100];
    for (int k = 0; k < 100; k++)
    {
        float total_battery = 0;
        for (int i = 0; i < 10; i++)
        {
            double measuredvbat = analogRead(VBATPIN);
            measuredvbat *= 2;    // we divided by 2, so multiply back
            measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
            measuredvbat /= 1024; // convert to voltage
            total_battery += measuredvbat;
            delayMicroseconds(10);
        }

        testList1[k] = total_battery / 10;
    }
    float max1 = testList1[0];
    for (int j = 1; j < 100; j++)
    {
        if (testList1[j] > max1)
        {
            max1 = testList1[j]; // to find max1
        }
    }

    Serial.print("vbatt max1 is: ");
    Serial.println(max1);

    delay(1000);

    float testList2[100];
    for (int k = 0; k < 100; k++)
    {
        float total_battery = 0;
        for (int i = 0; i < 10; i++)
        {
            double measuredvbat = analogRead(VBATPIN);
            measuredvbat *= 2;    // we divided by 2, so multiply back
            measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
            measuredvbat /= 1024; // convert to voltage
            total_battery += measuredvbat;
            delayMicroseconds(10);
        }

        testList2[k] = total_battery / 10;
    }
    float max2 = testList2[0];
    for (int j = 1; j < 100; j++)
    {
        if (testList2[j] > max2)
        {
            max2 = testList2[j]; // to find max2
        }
    }

    Serial.print("vbatt max2 is: ");
    Serial.println(max2);

    if (max2 > max1)
    {
        Serial.println("max2 is larger than max1, its charging!");
        return true;
    }
    else
    {
        Serial.println("max1 is still smaller than max2, no charging.... :(");
        return false;
    }
}
*/

/* old SECOND function for internal hall sensor
bool lockInsertion2()
{
    long h = 0;
    int i = 0;
    for (i = 0; i < 1000; i++)
    {
        h += hallRead();
        delayMicroseconds(100);
    }

    if (((double)h / 1000.) < 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}
*/
