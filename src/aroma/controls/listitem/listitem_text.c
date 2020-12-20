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
 * + 25/06/15 - Author(s): Ahmad Amarullah
 * + 14/12/20 - Modified by: MLXProjects
 *
 */
#ifndef __libaroma_listitem_text_c__
#define __libaroma_listitem_text_c__
#include <aroma_internal.h>

#include "../../ui/ui_internal.h"


/* LIST ITEM HANDLER */
byte _libaroma_listitem_text_message(
	LIBAROMA_CONTROLP, LIBAROMA_CTL_LIST_ITEMP,byte,dword,int,int);
void _libaroma_listitem_text_draw(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP,LIBAROMA_CANVASP,word,byte);
void _libaroma_listitem_text_destroy(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP);
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
	byte redraw;
	int vpad;
	int hpad;
	int font_id;
	int font_size;
	word flags;
	int h;
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
	
	if (!(state&LIBAROMA_CTL_LIST_ITEM_DRAW_ADDONS)){
		int icoh=libaroma_dp(mi->vpad*2);
		int tx = mi->hpad;//libaroma_dp(16);
		int tw = cv->w - (tx*2);//libaroma_dp(52);
		int ty = libaroma_dp(mi->vpad*2);
		LIBAROMA_TEXT mtextp=NULL;
		
		/* prepare main text */
		int txtsh=0;
		int m_h=0;
		if (mi->text){
			mtextp = libaroma_text(
				mi->text,
				mi->textcolor,
				tw,
				LIBAROMA_FONT(mi->font_id,mi->font_size)|mi->flags,
				137
			);
			m_h=libaroma_text_height(mtextp);
			ty+=m_h;
			txtsh+=m_h;
		}
		
		
		int etremsz=0;
		/* calculate whole height */
		ty+=libaroma_dp(mi->vpad*2);
		int my_h = MAX(icoh,ty);
		
		int txt_sy=((my_h>>1)-((txtsh>>1)+libaroma_dp(2+etremsz)))+libaroma_dp(2);
		
		/* draw main text */
		if (mtextp){
			//libaroma_text_draw(cv,mtextp,tx,txt_sy
			libaroma_draw_text(cv, mi->text, tx, txt_sy, mi->textcolor, tw, mi->flags, 0);
			txt_sy+=m_h;
			libaroma_text_free(mtextp);
		}
		
		if (my_h!=mi->h){
			mi->h=my_h;
			libaroma_ctl_list_item_setheight(
				ctl, item, my_h
			);
		}
	}
} /* End of _libaroma_listitem_text_draw */

/*
 * Function		: _libaroma_listitem_text_release_internal
 * Return Value: void
 * Descriptions: release internal data
 */
void _libaroma_listitem_text_release_internal(
	_LIBAROMA_LISTITEM_TEXTP mi){
	if (mi->text){
		free(mi->text);
	}
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
	if (!item){
		return;
	}
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	if (!text){
		return;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
	mi->text=text;
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
	if (!item){
		return;
	}
	if (item->handler!=&_libaroma_listitem_text_handler){
		return;
	}
	if (!text){
		return;
	}
	_LIBAROMA_LISTITEM_TEXTP mi = (_LIBAROMA_LISTITEM_TEXTP) item->internal;
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
		const char * text,
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
	int h = 0;
	mi->textcolor=textcolor;
	mi->text=(text?strdup(text):NULL);
	mi->vpad=vpad;
	mi->hpad=hpad;
	mi->font_id=font_id;
	mi->font_size=font_size?font_size:3;
	mi->flags=LIBAROMA_FONT(font_id,font_size)|flags;
	/* calculate height */
	int tw = ctl->w-hpad;//-libaroma_dp(52);
	int th = libaroma_dp(vpad*2);
	if (tw>0){
		if (mi->text){
			LIBAROMA_TEXT mtextp = libaroma_text(
				mi->text,
				0,
				tw,
				LIBAROMA_FONT(font_id,font_size)|flags,
				137
			);
			th+=libaroma_text_height(mtextp);
			libaroma_text_free(mtextp);
		}
	}
	
	h = MAX(h,th)+libaroma_dp(vpad*2);
	mi->h=h;
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
		const char * text,
		int vpad, int hpad, int font_id, int font_size, word flags, byte updateable,
		int at_index){
	return libaroma_listitem_text_color(ctl,id,text,
		libaroma_colorget(ctl,NULL)->accent,vpad,hpad,font_id,font_size,flags,updateable,at_index);
}

#endif /* __libaroma_listitem_text_c__ */

