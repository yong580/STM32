#include "myprintf.h"
#include "math.h"

void ftprintf(float value, int num)
{
    int intb = (int)value;
    int inta = ((int)(value * pow(10, num)) % (int)pow(10, num));
    printf("%d.%d\r\n", intb, inta);
}