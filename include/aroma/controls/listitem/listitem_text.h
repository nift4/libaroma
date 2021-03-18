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
 * Filename		: listitem_text.h
 * Description : text item header
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 25/06/15 - Author(s): Ahmad Amarullah
 * + 14/12/20 - Modified by: MLXProjects
 *
 */
#ifndef __libaroma_aroma_h__
	#error "Include <aroma.h> instead."
#endif
#ifndef __libaroma_listitem_text_h__
#define __libaroma_listitem_text_h__

/*
 * Function		: libaroma_listitem_text_set
 * Return Value: void
 * Descriptions: set item text
 */
void libaroma_listitem_text_set(
		LIBAROMA_CTL_LIST_ITEMP item,
		char *text);
/*
 * Function		: libaroma_listitem_text_add
 * Return Value: void
 * Descriptions: add text to item
 */
void libaroma_listitem_text_add(
		LIBAROMA_CTL_LIST_ITEMP item,
		char *text, byte freeold);
/*
 * Function		: libaroma_listitem_text_color
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: create text item with custom color
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_text_color(
		LIBAROMA_CONTROLP ctl,
		int id,
		char * text,
		word textcolor,
		int vpad,
		int hpad,
		int font_id,
		int font_size,
		word flags,
		byte updateable,
		int at_index);

/*
 * Function		: libaroma_listitem_text
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: create text item
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_text(
		LIBAROMA_CONTROLP ctl,
		int id,
		char * text,
		int vpad, int hpad, int font_id, int font_size, word flags, byte updateable,
		int at_index);

#endif /* __libaroma_listitem_text_h__ */
