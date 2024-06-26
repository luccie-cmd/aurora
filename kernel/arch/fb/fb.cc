// This software is licensed under the GPL 3.0 license.
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.

#include "fb.h"
#include <debug.h>

volatile struct limine_framebuffer_request _FramebuffersRequest = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = NULL,
};
size_t _Fbcount = 0;
struct limine_framebuffer** _Framebuffers = NULL;
struct limine_framebuffer* _MainFramebuffer = NULL;
uint64_t screenX, screenY;

void FbInit(){
    screenX = 0;
    screenY = 0;
    if (_FramebuffersRequest.response == NULL){
        while(1){
            __asm__("nop");
        }
    }
    _Framebuffers = _FramebuffersRequest.response->framebuffers;

    _Fbcount = _FramebuffersRequest.response->framebuffer_count;
    if (_Fbcount > 0)
        _MainFramebuffer = _Framebuffers[0];
}
void FbPutPixel(uint64_t xpos, uint64_t ypos, uint8_t r, uint8_t g, uint8_t b){
    struct limine_framebuffer* fb = _Framebuffers[0];
    uint64_t fbwidth = fb->width;
    uint64_t fbheight = fb->height;
    if (xpos >= fbwidth || ypos >= fbheight){
        return; // Invalid pixel coordinates, exit function
    }
    size_t offset = (ypos * fbwidth + xpos) * (fb->bpp / 8);
    volatile uint8_t* pixelAddress = (volatile uint8_t*)(fb->address) + offset;
    uint8_t rShift = fb->red_mask_shift;
    uint8_t gShift = fb->green_mask_shift;
    uint8_t bShift = fb->blue_mask_shift;
    uint32_t rMask = (1 << fb->red_mask_size) - 1;
    uint32_t gMask = (1 << fb->green_mask_size) - 1;
    uint32_t bMask = (1 << fb->blue_mask_size) - 1;
    *(volatile uint32_t*)(pixelAddress) &= ~(rMask << rShift);
    *(volatile uint32_t*)(pixelAddress) &= ~(gMask << gShift);
    *(volatile uint32_t*)(pixelAddress) &= ~(bMask << bShift);

    *(volatile uint32_t*)(pixelAddress) |= (r & rMask) << rShift;
    *(volatile uint32_t*)(pixelAddress) |= (g & gMask) << gShift;
    *(volatile uint32_t*)(pixelAddress) |= (b & bMask) << bShift;
}
void FbPutc(char c){
    (void)c;
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){
            FbPutPixel(screenX+x, screenY+y, 255, 255, 255);
        }
    }
}