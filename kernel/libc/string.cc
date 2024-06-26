#include <string.h>

void *memset(void *str, int c, size_t n){
    char *s = (char*)str;
    while(n--){
        *s++ = c;
    }
    return str;
}