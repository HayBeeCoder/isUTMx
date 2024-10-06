#include <Arduino.h>

void formatExtensometerValue(String value, char *output, size_t outputSize)
{
    snprintf(output, outputSize, "%smm", value.c_str());
}