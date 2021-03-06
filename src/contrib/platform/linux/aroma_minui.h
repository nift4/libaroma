#ifndef AROMA_MINUI_H
#define AROMA_MINUI_H

#ifdef __cplusplus
extern "C" {
#endif

int aroma_minui_init(void);
int aroma_minui_get_fb_width(void);
int aroma_minui_get_fb_height(void);
void* aroma_minui_get_data(void);
int aroma_minui_pixel_bytes(void);
int aroma_minui_row_bytes(void);
void aroma_minui_flip(void);
void aroma_minui_exit(void);
void aroma_minui_fill_red(void);
#ifdef __cplusplus
}
#endif

#endif // AROMA_MINUI_H
