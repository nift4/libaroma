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
	printf("I/MINUI_WRAPPER(): starting up minui drm backend\n");
	#ifndef LIBAROMA_OLD_MINUI
	GRSurface *gr_draw = gr_init();
	if (gr_draw==NULL) {
		printf("E/MINUI_WRAPPER(): failed to start up minui backend (R.I.P.)\n");
		return 0;
	}
	printf("I/MINUI_WRAPPER(): gr_draw surface bpp is %d\n", gr_draw->pixel_bytes*8);
	int row_bytes=gr_draw->width*gr_draw->pixel_bytes;
	aroma_surface = gr_create(gr_draw->width, gr_draw->height, row_bytes, gr_draw->pixel_bytes);
	if (aroma_surface==NULL) return 0;
	#else
	if (gr_init()!=0) {
		printf("E/MINUI_WRAPPER(): failed to start up minui backend (R.I.P.)\n");
		return 0;
	}
	printf("I/MINUI_WRAPPER(): Trying to allocate aroma_surface memory\n");
	aroma_surface=(GRSurface*) malloc(sizeof(GRSurface));
	aroma_surface->width = gr_fb_width();
	printf("I/MINUI_WRAPPER(): got minui fb width (%d)\n", aroma_surface->width);
	aroma_surface->height = gr_fb_height();
	printf("I/MINUI_WRAPPER(): got minui fb height (%d)\n", aroma_surface->height);
	aroma_surface->pixel_bytes = gr_fb_pixel_bytes();
	printf("I/MINUI_WRAPPER(): pixel bytes set (%d)\n", aroma_surface->pixel_bytes);
	aroma_surface->row_bytes = aroma_surface->width*aroma_surface->pixel_bytes;//gr_draw->row_bytes;
	printf("I/MINUI_WRAPPER(): surface size set (%d)\n", aroma_surface->row_bytes);
	printf("I/MINUI_WRAPPER(): allocating framebuffer data (to draw here later)\n");
	aroma_surface->data = (unsigned char*) malloc(sizeof(uint32_t) * aroma_surface->height * aroma_surface->row_bytes);
	#endif /* LIBAROMA_OLD_MINUI */
	printf("I/MINUI_WRAPPER(): dumping surface info:\n"
			"I/MINUI_WRAPPER():   width: %d\n"
			"I/MINUI_WRAPPER():   height: %d\n"
			"I/MINUI_WRAPPER():   row_bytes: %d\n"
			"I/MINUI_WRAPPER():   pixel_bytes: %d\n",
			aroma_surface->width, aroma_surface->height,
			aroma_surface->row_bytes, aroma_surface->pixel_bytes);
	return 1;
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
	#ifndef LIBAROMA_OLD_MINUI
	uint8_t *data=aroma_surface->data();
	return (void*)data;
	#else
	return aroma_surface->data;
	#endif /* LIBAROMA_OLD_MINUI */
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
