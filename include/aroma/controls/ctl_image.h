/********************************************************************[libaroma]*
 * Copyright (C) 2011-2015 Ahmad Amarullah (http://amarullz.com/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *			http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *______________________________________________________________________________
 *
 * Filename		: ctl_image.h
 * Description : image control
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 28/01/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_aroma_h__
	#error "Include <aroma.h> instead."
#endif
#ifndef __libaroma_ctl_image_h__
#define __libaroma_ctl_image_h__

/* flags */
#define LIBAROMA_CTL_IMAGE_FREE			0x1		//duplicate image on control creation, and free on control freeing
#define LIBAROMA_CTL_IMAGE_SHARED		0x2		//don't duplicate nor free image
#define LIBAROMA_CTL_IMAGE_FILL			0x4		//scale source to fill image control
#define LIBAROMA_CTL_IMAGE_CENTER		0x8		//center unscaled source in image control
#define LIBAROMA_CTL_IMAGE_HIDDEN		0x16	//create hidden image control

byte libaroma_ctl_image_set_image(
	LIBAROMA_CONTROLP ctl,char * src,byte update);

byte libaroma_ctl_image_set_canvas(
	LIBAROMA_CONTROLP ctl,LIBAROMA_CANVASP src,byte update);

byte libaroma_ctl_image_getflags(
	LIBAROMA_CONTROLP ctl);

byte libaroma_ctl_image_setflags(
	LIBAROMA_CONTROLP ctl,byte flags);

LIBAROMA_CONTROLP libaroma_ctl_image(
	LIBAROMA_WINDOWP win, word id, char * src,
	int x, int y, int w, int h
);

LIBAROMA_CONTROLP libaroma_ctl_image_canvas_ex(
	LIBAROMA_WINDOWP win, word id, LIBAROMA_CANVASP src,
	int x, int y, int w, int h, byte flags
);

byte libaroma_ctl_image_hidden(
	LIBAROMA_CONTROLP ctl,byte hidden);

byte libaroma_ctl_image_isvisible(
	LIBAROMA_CONTROLP ctl);

byte libaroma_ctl_image_update(
	LIBAROMA_CONTROLP ctl);
	
#endif /* __libaroma_ctl_image_h__ */
