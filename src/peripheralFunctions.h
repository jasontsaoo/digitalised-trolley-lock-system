#include <Arduino.h>

// gpio number of the solenoid lock
int LOCK = 12;
// gpio number of the external hall sensor
int externalHALL = 34;

// FIRST function for internal hall sensor
bool lockInsertion1()
{
    float x = 0;
    for (int i = 0; i < 10000; i++)
    {
        x += hallRead();
        delayMicroseconds(100);
    }
    x = x / 10000.000000;

    if (x < 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// SECOND function for internal hall sensor
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
    if (x > 2000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

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