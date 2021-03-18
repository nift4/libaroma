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
 * Filename		: listitem_text.c
 * Description	: text item source
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 16/03/21 - Author(s): Michael Jauregui
 *
 */
#ifndef __libaroma_listitem_text_c__
#define __libaroma_listitem_text_c__
#include <aroma_internal.h>
#include "../../ui/ui_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* LIST ITEM HANDLER */
byte _libaroma_listitem_text_message(
	LIBAROMA_CONTROLP, LIBAROMA_CTL_LIST_ITEMP,byte,dword,int,int);
void _libaroma_listitem_text_draw(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP,LIBAROMA_CANVASP,word,byte);
void _libaroma_listitem_text_destroy(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP);
void *_libaroma_listitem_text_loader(void *info);
static LIBAROMA_CTL_LIST_ITEM_HANDLER _libaroma_listitem_text_handler =
{
	message:_libaroma_listitem_text_message,
	draw:_libaroma_listitem_text_draw,
	destroy:_libaroma_listitem_text_destroy
};

/* LIST ITEM INTERNAL DATA */
typedef struct{
	word textcolor;
	char * text;
	LIBAROMA_TEXT cached_text; //used for initial control draw
	byte updateable;
	byte redraw;
	byte isparsing; //useful if draw is called while parsing the text
	int vpad;
	int hpad;
	int font_id;
	int font_size;
	word flags;
	int w;
	int h;
	LIBAROMA_THREAD loader;
	LIBAROMA_CTL_LIST_ITEMP item;
	LIBAROMA_CONTROLP ctl;
} _LIBAROMA_LISTITEM_TEXT, * _LIBAROMA_LISTITEM_TEXTP;

/*
 * Function		: _libaroma_listitem_text_message
 * Return Value: byte
 * Descriptions: message handler
 */
byte _libaroma_listitem_text_message(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item,
		byte msg,
		dword param,
		int x,
		int y){
	if (item->handler!=&_libaroma_listitem_text_handler){
		return 0;
	}
	switch (msg){
		case LIBAROMA_CTL_LIST_ITEM_MSG_THREAD:
			{
				_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
				if (mi->redraw){
					mi->redraw=0;
					return LIBAROMA_CTL_LIST_ITEM_MSGRET_NEED_DRAW;
				}
			}
			break;
	}
	return 0;
} /* End of _libaroma_listitem_text_message */

/*
 * Function		: _libaroma_listitem_text_loader
 * Return Value: void*
 * Descriptions: prepares the text to be drawn
 */
void *_libaroma_listitem_text_loader(void *info){
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) info;
	ALOGV("Starting listitem_text loader thread");
	mi->cached_text=libaroma_text(mi->text, mi->textcolor, mi->w, LIBAROMA_FONT(mi->font_id,mi->font_size)|mi->flags, 0);
	if (mi->cached_text==NULL){ //maybe text parser exited before finishing?
		mi->isparsing=0;
		return NULL;
	}
	int newh=libaroma_text_height(mi->cached_text)+(mi->vpad*2);
	libaroma_ctl_list_item_setheight(mi->ctl, mi->item, newh);
	mi->isparsing=0;
	mi->redraw=1;
	return NULL;
}

/*
 * Function		: _libaroma_listitem_text_draw
 * Return Value: void
 * Descriptions: item draw routine
 */
void _libaroma_listitem_text_draw(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item,
		LIBAROMA_CANVASP cv,
		word bgcolor,
		byte state){
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
	mi->w=cv->w;
	// calculate max text width
	int tw = mi->w - (mi->hpad*2);
	// try to use the cached text, if any
	LIBAROMA_TEXT text=mi->cached_text;
	if (text==NULL){ // parse text
		if (!mi->isparsing){
			mi->isparsing=1;
			libaroma_thread_create(&(mi->loader), _libaroma_listitem_text_loader, (voidp) mi);
			libaroma_thread_detach(mi->loader);
			//libaroma_thread_set_hiprio(mi->loader); //this seems to hang the window until almost loaded
		}
		libaroma_draw_text(cv, "Please wait...", mi->hpad, mi->vpad, mi->textcolor, cv->w-(mi->hpad*2),
							LIBAROMA_FONT(mi->font_id, mi->font_size)|LIBAROMA_TEXT_SINGLELINE, cv->h-(mi->vpad*2));
		return;
	}
	// draw text to control canvas
	if (text!=NULL)
		libaroma_text_draw_ex(cv, text, mi->hpad, mi->vpad, 0, 0, cv->h-(mi->vpad*2), 0, 0, 0, 0, 0, 0);
	if (mi->cached_text!=NULL){ //if text was cached, delete it (draw event should mean full redraw)
		mi->cached_text=NULL;
	}
	// free text variable (only needed for drawing)
	if (text!=NULL)
		libaroma_text_free(text);
} /* End of _libaroma_listitem_text_draw */

/*
 * Function		: _libaroma_listitem_text_release_internal
 * Return Value: void
 * Descriptions: release internal data
 */
void _libaroma_listitem_text_release_internal(
	_LIBAROMA_LISTITEM_TEXTP mi){
	if (mi->isparsing){
		libaroma_text_exit_parser();
		libaroma_sleep(50);
	}
	if (mi->text!=NULL){
		free(mi->text);
	}
	if (mi->cached_text!=NULL)
		libaroma_text_free(mi->cached_text);
	free(mi);
} /* End of _libaroma_listitem_text_release_internal */

/*
 * Function		: _libaroma_listitem_text_destroy
 * Return Value: void
 * Descriptions: destroy text item
 */
void _libaroma_listitem_text_destroy(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item){
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	_libaroma_listitem_text_release_internal(
		(_LIBAROMA_LISTITEM_TEXTP) item->internal
	);
} /* End of _libaroma_listitem_text_destroy */

/*
 * Function		: libaroma_listitem_text_set
 * Return Value: void
 * Descriptions: set item text
 */
void libaroma_listitem_text_set(
		LIBAROMA_CTL_LIST_ITEMP item,
		char *text){
	if (!item || !text){
		return;
	}
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
	if (!mi->updateable){
		ALOGE("Cannot set new text to non-updateable item");
		return;
	}
	if (mi->text) free(mi->text);
	mi->text=strdup(text);
	mi->redraw=1;
}

/*
 * Function		: libaroma_listitem_text_add
 * Return Value: void
 * Descriptions: add text to item
 */
void libaroma_listitem_text_add(
		LIBAROMA_CTL_LIST_ITEMP item,
		char *text, byte freeold){
	if (!item || !text){
		return;
	}
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
	if (!mi->updateable){
		ALOGE("Cannot add text to non-updateable item");
		return;
	}
	char *newtext=malloc(strlen(mi->text) + strlen(text) +2);
	strcpy(newtext, mi->text);
	strcat(newtext, text);
	if (freeold){
		free(mi->text);
	}
	mi->text=newtext;
	mi->redraw=1;
}

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
		int at_index){
	/* check valid list control */
	if (!libaroma_ctl_list_is_valid(ctl)){
		ALOGW("listitem_text control is not valid list control");
		return 0;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP)
		calloc(sizeof(_LIBAROMA_LISTITEM_TEXT),1);
	if (!mi){
		ALOGW("listitem_text cannot allocate internal data");
		return NULL;
	}
	if (vpad<0) vpad=0;
	mi->ctl=ctl;
	mi->textcolor=textcolor;
	mi->text=strdup(text);
	mi->vpad=vpad;
	mi->updateable=updateable;
	mi->hpad=hpad;
	mi->font_id=font_id;
	mi->font_size=font_size?font_size:3;
	mi->flags=flags;
	int h = (vpad==0)?16:(vpad*2);

	LIBAROMA_CTL_LIST_ITEMP item = libaroma_ctl_list_add_item_internal(
		ctl,
		id,
		h,updateable?LIBAROMA_CTL_LIST_ITEM_REGISTER_THREAD:0,
		(voidp) mi,
		&_libaroma_listitem_text_handler,
		at_index
	);
	if (!item){
		ALOGW("listitem_text add_item_internal failed");
		_libaroma_listitem_text_release_internal(mi);
	}
	mi->item=item;
	return item;
} /* End of libaroma_listitem_text */

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
		int at_index){
	return libaroma_listitem_text_color(ctl,id,text,
		libaroma_colorget(ctl,NULL)->accent,vpad,hpad,font_id,font_size,flags,updateable,at_index);
}

#ifdef __cplusplus
}
#endif
#endif /* __libaroma_listitem_text_c__ */

