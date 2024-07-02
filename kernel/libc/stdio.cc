// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <arch/fb/fb.h>
#include <debug.h>

namespace std{
    int putc(char c){
        arch::fb::putc(c);
        return 1;
    }
    
    int puts(const char* str)
    {
        int i = 0;
        while(*str)
        {
            putc(*str);
            str++;
            i++;
        }
        return i;
    }
    
    #define PRINTF_STATE_NORMAL         0
    #define PRINTF_STATE_LENGTH         1
    #define PRINTF_STATE_LENGTH_SHORT   2
    #define PRINTF_STATE_LENGTH_LONG    3
    #define PRINTF_STATE_SPEC           4
    
    #define PRINTF_LENGTH_DEFAULT       0
    #define PRINTF_LENGTH_SHORT_SHORT   1
    #define PRINTF_LENGTH_SHORT         2
    #define PRINTF_LENGTH_LONG          3
    #define PRINTF_LENGTH_LONG_LONG     4
    
    const char g_HexChars[] = "0123456789abcdef";
    
    void printf_unsigned(unsigned long long number, int radix)
    {
        char buffer[32];
        int pos = 0;
    
        // convert number to ASCII
        do 
        {
            unsigned long long rem = number % radix;
            number /= radix;
            buffer[pos++] = g_HexChars[rem];
        } while (number > 0);
    
        // print number in reverse order
        while (--pos >= 0)
            putc(buffer[pos]);
    }
    
    void printf_signed(long long number, int radix)
    {
        if (number < 0)
        {
            putc('-');
            printf_unsigned(-number, radix);
        }
        else printf_unsigned(number, radix);
    }
    
    int vprintf(const char* fmt, va_list args)
    {
        int state = PRINTF_STATE_NORMAL;
        int length = PRINTF_LENGTH_DEFAULT;
        int radix = 10;
        bool sign = false;
        bool number = false;
    
        while (*fmt)
        {
            switch (state)
            {
                case PRINTF_STATE_NORMAL:
                    switch (*fmt)
                    {
                        case '%':   state = PRINTF_STATE_LENGTH;
                                    break;
                        default:    putc(*fmt);
                                    break;
                    }
                    break;
    
                case PRINTF_STATE_LENGTH:
                    switch (*fmt)
                    {
                        case 'h':   length = PRINTF_LENGTH_SHORT;
                                    state = PRINTF_STATE_LENGTH_SHORT;
                                    break;
                        case 'l':   length = PRINTF_LENGTH_LONG;
                                    state = PRINTF_STATE_LENGTH_LONG;
                                    break;
                        default:    goto PRINTF_STATE_SPEC_;
                    }
                    break;
    
                case PRINTF_STATE_LENGTH_SHORT:
                    if (*fmt == 'h')
                    {
                        length = PRINTF_LENGTH_SHORT_SHORT;
                        state = PRINTF_STATE_SPEC;
                    }
                    else goto PRINTF_STATE_SPEC_;
                    break;
    
                case PRINTF_STATE_LENGTH_LONG:
                    if (*fmt == 'l')
                    {
                        length = PRINTF_LENGTH_LONG_LONG;
                        state = PRINTF_STATE_SPEC;
                    }
                    else goto PRINTF_STATE_SPEC_;
                    break;
    
                case PRINTF_STATE_SPEC:
                PRINTF_STATE_SPEC_:
                    switch (*fmt)
                    {
                        case 'c':   putc((char)va_arg(args, int));
                                    break;
    
                        case 's':   
                                    puts(va_arg(args, const char*));
                                    break;
    
                        case '%':   putc('%');
                                    break;
    
                        case 'd':
                        case 'i':   radix = 10; sign = true; number = true;
                                    break;
    
                        case 'u':   radix = 10; sign = false; number = true;
                                    break;
    
                        case 'X':
                        case 'x':
                        case 'p':   radix = 16; sign = false; number = true;
                                    break;
    
                        case 'o':   radix = 8; sign = false; number = true;
                                    break;
    
                        // ignore invalid spec
                        default:    break;
                    }
    
                    if (number)
                    {
                        if (sign)
                        {
                            switch (length)
                            {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_signed(va_arg(args, int), radix);
                                                            break;
    
                            case PRINTF_LENGTH_LONG:        printf_signed(va_arg(args, long), radix);
                                                            break;
    
                            case PRINTF_LENGTH_LONG_LONG:   printf_signed(va_arg(args, long long), radix);
                                                            break;
                            }
                        }
                        else
                        {
                            switch (length)
                            {
                            case PRINTF_LENGTH_SHORT_SHORT:
                            case PRINTF_LENGTH_SHORT:
                            case PRINTF_LENGTH_DEFAULT:     printf_unsigned(va_arg(args, unsigned int), radix);
                                                            break;
                                                            
                            case PRINTF_LENGTH_LONG:        printf_unsigned(va_arg(args, unsigned  long), radix);
                                                            break;
    
                            case PRINTF_LENGTH_LONG_LONG:   printf_unsigned(va_arg(args, unsigned  long long), radix);
                                                            break;
                            }
                        }
                    }
    
                    // reset state
                    state = PRINTF_STATE_NORMAL;
                    length = PRINTF_LENGTH_DEFAULT;
                    radix = 10;
                    sign = false;
                    number = false;
                    break;
            }
    
            fmt++;
        }
        return 0;
    }
    
    int printf(const char* fmt, ...){
        va_list args;
        va_start(args, fmt);
        int N = vprintf(fmt, args);
        va_end(args);
        return N;
    }
    
    void print_buffer(const char* msg, const void* buffer, uint32_t count)
    {
        const uint8_t* u8Buffer = (const uint8_t*)buffer;
        
        puts(msg);
        for (uint16_t i = 0; i < count; i++)
        {
            putc(g_HexChars[u8Buffer[i] >> 4]);
            putc(g_HexChars[u8Buffer[i] & 0xF]);
        }
        puts("\n");
    }
}