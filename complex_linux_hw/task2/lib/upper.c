#include "convert.h"
#include <ctype.h>

void upper(char *str)
{
    while(*str)
    {
        *str = toupper(*str);
        str++;
    }
}
