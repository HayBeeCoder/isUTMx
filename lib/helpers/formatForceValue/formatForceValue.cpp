#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void addCommas(char *numberStr)
{
    int len = strlen(numberStr);
    int commas = (len - 1) / 3;                // Number of commas needed
    int newLen = len + commas;                 // New length with commas
    char *result = (char *)malloc(newLen + 1); // Cast to char* // +1 for the null terminator
    result[newLen] = '\0';                     // Null terminate the new string

    int j = newLen - 1;
    int count = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        result[j--] = numberStr[i];
        count++;

        // Insert comma after every 3 digits, except at the start
        if (count == 3 && i != 0)
        {
            result[j--] = ',';
            count = 0;
        }
    }
    strcpy(numberStr, result); // Copy back to original
    // Serial.print("Number");
    free(result);              // Free the temporary memory
}

void formatForceValue(double value, char *output, size_t outputSize)
{
    char numberStr[20]; // Temporary string for formatted number

    if (value < 1000.0)
    {
        // Less than 1 kN
        snprintf(output, outputSize, "%.1fN", value);
    }
    else if (value < 1000000.0)
    {
        // Between 1 kN and 1 MN
        snprintf(output, outputSize, "%.1fN", value);
        // snprintf(output, outputSize, "%.4fkN", value / 1000.0);
    }
    else if (value < 10000000.0)
    {
        // Between 1 MN and 10 MN
        snprintf(output, outputSize, "%.1fN", value);
    }
    else
    {
        // 1 GN and above
        // snprintf(output, outputSize, "%.1fGN", value / 1000000000.0);
        snprintf(output, outputSize, "%.1fN", value );
        // snprintf(output, outputSize, "%.1fkN", value / 1000.0);
    }
    // addCommas(output);                             // Add commas to the formatted number
    // snprintf(output, outputSize, "%s", numberStr); // Copy to output
}
