// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include <string.h>

namespace std{
    void *memset(void *str, int c, size_t n){
        char *s = (char*)str;
        while(n--){
            *s++ = c;
        }
        return str;
    }
}