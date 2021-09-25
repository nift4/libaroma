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
 * Filename		: listitem_image.h
 * Description : image list item
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 17/06/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_aroma_h__
	#error "Include <aroma.h> instead."
#endif
#ifndef __libaroma_listitem_image_h__
#define __libaroma_listitem_image_h__

/* option signature */
#define LIBAROMA_LISTITEM_IMAGE_FILL					0x0100
#define LIBAROMA_LISTITEM_IMAGE_PROPORTIONAL	0x0200
#define LIBAROMA_LISTITEM_IMAGE_SHARED				0x0400
#define LIBAROMA_LISTITEM_IMAGE_FREE					0x0800
#define LIBAROMA_LISTITEM_IMAGE_PARALAX			 0x1000

/*
 * Function		: libaroma_listitem_image
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: create option item
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_image(
		LIBAROMA_CONTROLP ctl,
		int id,
		LIBAROMA_CANVASP image,
		int h,
		word flags,
		int at_index);

/*
 * Function		: libaroma_listitem_image_get
 * Return Value: LIBAROMA_CANVASP
 * Descriptions: get current item image
 */
LIBAROMA_CANVASP libaroma_listitem_image_get(
		LIBAROMA_CTL_LIST_ITEMP item);

/*
 * Function		: libaroma_listitem_image_set
 * Return Value: byte
 * Descriptions: set item image
 */
byte libaroma_listitem_image_set(
		LIBAROMA_CTL_LIST_ITEMP item,
		LIBAROMA_CANVASP cv);

#endif /* __libaroma_listitem_image_h__ */
