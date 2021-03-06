#include "aroma_minui.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <minui.h>

#ifdef __cplusplus
extern "C" {
#endif
static GRSurface* aroma_surface;
int aroma_minui_init(void) {
	printf("aroma_minui_init: starting up minui drm backend\n");
	GRSurface *gr_draw = gr_init();
	if (gr_draw==NULL) {
		printf("aroma_minui_init: failed to start up minui drm backend (F)\n");
		return 1;
	}
	printf("aroma_minui_init: gr_draw surface bpp is %d\n", gr_draw->pixel_bytes*8);
	printf("aroma_minui_init: Trying to allocate aroma_surface memory\n");
	aroma_surface=malloc(sizeof(GRSurface));
	aroma_surface->width = gr_fb_width();
	printf("aroma_minui_init: got minui fb width (%d)\n", aroma_surface->width);
	aroma_surface->height = gr_fb_height();
	printf("aroma_minui_init: got minui fb height (%d)\n", aroma_surface->height);
	aroma_surface->pixel_bytes = gr_draw->pixel_bytes;
	printf("aroma_minui_init: pixel bytes set (%d)\n", aroma_surface->pixel_bytes);
	aroma_surface->row_bytes = aroma_surface->width*aroma_surface->pixel_bytes;//gr_draw->row_bytes;
	printf("aroma_minui_init: surface size set (%d)\n", aroma_surface->row_bytes);
	printf("aroma_minui_init: allocating framebuffer data (to draw here later)\n");
	aroma_surface->data = (unsigned char*) malloc(sizeof(uint32_t) * aroma_surface->height * aroma_surface->row_bytes);
	/*printf("aroma_minui_init: dumping surface info:\n"
			"width: %d\n"
			"height: %d\n"
			"row_bytes: %d\n"
			"pixel_bytes: %d\n",
			aroma_surface->width, aroma_surface->height,
			aroma_surface->row_bytes, aroma_surface->pixel_bytes);*/
	printf("aroma_minui_init: Thanks, come back soon! :D\n");
	return 0;
}

void aroma_minui_exit(void){
	gr_exit();
}

void aroma_minui_fill_color(int r, int g, int b, int a){
	gr_color(r, g, b, a);
	gr_fill(0, 0, gr_fb_width(), gr_fb_height());
	gr_flip();
}

int aroma_minui_pixel_bytes(void){
	return aroma_surface->pixel_bytes;
}

int aroma_minui_row_bytes(void){
	return aroma_surface->row_bytes;
}

int aroma_minui_get_fb_width(void) {
	//printf("aroma_minui_get_fb_width: asked for width\n");
	return aroma_surface->width;
}

int aroma_minui_get_fb_height(void) {
	//printf("aroma_minui_get_fb_height: asked for height\n");
	return aroma_surface->height;
}

void* aroma_minui_get_data(void) {
	//printf("aroma_minui_get_data: asked for data\n");
	return aroma_surface->data;
}

void aroma_minui_flip(void) {
	//printf("aroma_minui_flip: blitting aroma_surface onto main display\n");
	gr_blit(aroma_surface, 0, 0, aroma_surface->width, aroma_surface->height, 0, 0);
	//printf("aroma_minui_flip: flipping surfaces\n");
	gr_flip();
	//printf("aroma_minui_flip: flipping done\n");
}

#ifdef __cplusplus
}
#endif
