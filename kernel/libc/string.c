// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <string.h>
#include <assert.h>

errnum errno = 0;
char* errnoInfo = NULL;
char *errnoLookup[] = {
    "No error was found",
    "No disks are present",
    "Invalid value",
    "No syscall handle in place",
    "TODO",
    "No memory is avaliable",
    "Exception occured",
    "Unhandled interrupt",
};

static_assert(sizeof(errnoLookup)/sizeof(errnoLookup[0]) == MAX_ERRNO, "Not all errno numbers have been declared");

void *memset(void *str, int c, size_t n){
    char *s = (char*)str;
    while(n--){
        *s++ = c;
    }
    return str;
}
void *memcpy(void *dest, const void * src, size_t n){
    char *d = (char*)dest;
    const char *s = (const char*)src;
    while(n--){
        *d++ = *s++;
    }
    return dest;
}

char *strcat(char *restrict dest, const char *restrict src){
    char *d = dest;
    while(*src){
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

char *strerr(errnum err){
    return errnoLookup[err];
}