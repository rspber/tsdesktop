
#pragma once

class SerialUSB {
public:
    bool begin(const uint boudRate);

    void print(const char* text = 0);
    void print(const uint number);

    void println(const char* text = 0);
    void println(const uint number);
};

extern SerialUSB Serial;
