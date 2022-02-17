#include <Arduino.h>

// gpio number of the solenoid lock
int LOCK = 12;
// gpio number of the external hall sensor
int externalHALL = 34;

// function for internal hall sensor
bool lockInsertion()
{
    long x = 0;
    for (int i = 0; i < 1000; i++)
    {
        x += hallRead();
        delayMicroseconds(1000);
    }
    x = (double)x / 1000.;

    if (x < 1)
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
    if (x > 2200)
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