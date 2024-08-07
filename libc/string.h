// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <stddef.h>
#include <errno.h>

void* memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void * src, size_t n);
char *strcat(char *restrict dest, const char *restrict src);
char* strerr(errnum err);