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
 * Filename		: listitem_check.c
 * Description : list item check
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 07/03/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_listitem_check_c__
#define __libaroma_listitem_check_c__
#include <aroma_internal.h>
#include "../../ui/ui_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
/* LIST ITEM HANDLER */
byte _libaroma_listitem_check_message(
	LIBAROMA_CONTROLP, LIBAROMA_CTL_LIST_ITEMP,byte,dword,int,int);
void _libaroma_listitem_check_draw(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP,LIBAROMA_CANVASP,word,byte);
void _libaroma_listitem_check_destroy(
	LIBAROMA_CONTROLP,LIBAROMA_CTL_LIST_ITEMP);
static LIBAROMA_CTL_LIST_ITEM_HANDLER _libaroma_listitem_check_handler =
{
	message:_libaroma_listitem_check_message,
	draw:_libaroma_listitem_check_draw,
	destroy:_libaroma_listitem_check_destroy
};

/* LIST ITEM INTERNAL DATA */
typedef struct{
	byte selected;
	byte onchangeani;
	char * main_text;
	char * extra_text;
	byte font_small;
	LIBAROMA_CANVASP icon;
	int h;
	LIBAROMA_LISTITEM_CB change_cb;
	voidp change_data;
} _LIBAROMA_LISTITEM_CHECK, * _LIBAROMA_LISTITEM_CHECKP;

byte _libaroma_ctl_list_dodraw_item(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item
);

int libaroma_listitem_get_selected_index_from(
	LIBAROMA_CONTROLP ctl,
	int start_index
){
	int i;
	int item_count=libaroma_ctl_list_get_item_count(ctl);
	if (start_index<0) start_index=0;
	for (i=start_index; i<item_count; i++){
		LIBAROMA_CTL_LIST_ITEMP item;
		item=libaroma_ctl_list_get_item_internal(ctl, i, 0);
		if (item==NULL) continue;
		if (item->handler!=&_libaroma_listitem_check_handler)
			continue;
		_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;
		if (mi->selected)
			return i;
	}
	return -1;
}

LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_get_selected_from(
	LIBAROMA_CONTROLP ctl,
	int start_index
){
	LIBAROMA_CTL_LIST_ITEMP item;
	int i;
	if (start_index<0) start_index=0;
	int item_count=libaroma_ctl_list_get_item_count(ctl);
	for (i=start_index; i<item_count; i++){
		item=libaroma_ctl_list_get_item_internal(ctl, i, 0);
		if (item==NULL) continue;
		if (item->handler!=&_libaroma_listitem_check_handler)
			continue;
		_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;
		if (mi->selected)
			return item;
	}
	return NULL;
}

byte libaroma_listitem_set_selected(
	LIBAROMA_CONTROLP ctl,
	LIBAROMA_CTL_LIST_ITEMP item,
	byte selected){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return 0;
	}
	_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;
	if (mi->selected==selected) return 1;
	byte is_ondraw=0;
	if (selected>=10){
		is_ondraw=1;
		selected-=10;
	}
	if (!selected){
		if (mi->change_cb){
			if (mi->change_cb(
				ctl, item, item->id, mi->selected, mi->change_data, LIBAROMA_LISTITEM_CB_UNSELECTED
			)){
				mi->selected=0;
				mi->onchangeani=1;
			}
			else{
				return 0;
			}
		}
		else{
			mi->selected=0;
			mi->onchangeani=1;
		}
	}
	else{
		if (mi->change_cb){
			if (mi->change_cb(
				ctl, item, item->id, mi->selected, mi->change_data, LIBAROMA_LISTITEM_CB_SELECTED
			)){
				mi->selected=1;
				mi->onchangeani=1;
			}
			else{
				return 0;
			}
		}
		else{
			mi->selected=1;
			mi->onchangeani=1;
		}
	}
	if (mi->onchangeani){
		if (!is_ondraw){
			_libaroma_ctl_list_dodraw_item(ctl,item);
		}
	}
	return 1;
}

/*
 * Function		: libaroma_listitem_isoption
 * Return Value: byte
 * Descriptions: check if item is option checkbox
 */
byte libaroma_listitem_isoption(
	LIBAROMA_CTL_LIST_ITEMP item
){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return 0;
	}
	if (item->flags&LIBAROMA_LISTITEM_CHECK_OPTION){
		return 1;
	}
	return 0;
} /* End of libaroma_listitem_isoption */

void _libaroma_listitem_check_group_uncheck_others(
	LIBAROMA_LISTITEM_CHECK_GROUPP grp,
	LIBAROMA_CTL_LIST_ITEMP item
);

/*
 * Function		: _libaroma_listitem_check_message
 * Return Value: byte
 * Descriptions: message handler
 */
byte _libaroma_listitem_check_message(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item,
		byte msg,
		dword param,
		int x,
		int y){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return 0;
	}
	_LIBAROMA_LISTITEM_CHECKP mi =
		(_LIBAROMA_LISTITEM_CHECKP) item->internal;
	switch (msg){
		case LIBAROMA_CTL_LIST_ITEM_MSG_THREAD:
			{
				return 0;
			}
			break;
		case LIBAROMA_CTL_LIST_ITEM_MSG_TOUCH_DOWN:
			{
				//printf("list item #%i -> down\n",item->id);
				mi->onchangeani=0;
				return LIBAROMA_CTL_LIST_ITEM_MSGRET_HAVE_ADDONS_DRAW;
			}
			break;
		case LIBAROMA_CTL_LIST_ITEM_MSG_TOUCH_HOLDED:
			{
				mi->onchangeani=0;
				libaroma_window_post_command_ex(
					LIBAROMA_CMD_SET(LIBAROMA_CMD_HOLD, 0, ctl->id),
					mi->selected, item->id, 0, (voidp) item
				);
				//printf("list item #%i -> holded\n",item->id);
			}
			break;
		case LIBAROMA_CTL_LIST_ITEM_MSG_TOUCH_UP:
			{
				mi->onchangeani=0;
				if (param!=LIBAROMA_CTL_LIST_ITEM_MSGPARAM_HOLDED){
					//printf("Touch up! x: %d, y: %d, checkx: %d, checkwidth: %d\n",
					//	  x, y, item->checkx, item->checkwidth);
					if (item->flags&LIBAROMA_LISTITEM_CHECK_HAS_SUBMENU){
						if (x >= (item->checkx-(item->checkwidth)) && x<= (item->checkx + (item->checkwidth*1.5))) {
							if (libaroma_listitem_isoption(item)){
								_libaroma_listitem_check_group_uncheck_others(
									libaroma_listitem_check_find_group(ctl, item), item);
								if (!mi->selected)
									libaroma_listitem_set_selected(ctl, item, 11);
							}
							else libaroma_listitem_set_selected(ctl,item,mi->selected?10:11);
						}
						else {
							libaroma_window_post_command_ex(
								LIBAROMA_CMD_SET(LIBAROMA_CMD_CLICK, 0, ctl->id),
								mi->selected, item->id, 0, (voidp) item
							);
						}
					}
					else {
						if (libaroma_listitem_isoption(item)){
							_libaroma_listitem_check_group_uncheck_others(
								libaroma_listitem_check_find_group(ctl, item), item);
							if (!mi->selected) {
								libaroma_listitem_set_selected(ctl,item,11);
								libaroma_window_post_command_ex(
									LIBAROMA_CMD_SET(LIBAROMA_CMD_CLICK, 0, ctl->id),
									1, item->id, 0, (voidp) item
								);
							}
						}
						else {
							libaroma_listitem_set_selected(ctl,item,mi->selected?10:11);
							libaroma_window_post_command_ex(
								LIBAROMA_CMD_SET(LIBAROMA_CMD_CLICK, 0, ctl->id),
								mi->selected, item->id, 0, (voidp) item
							);
						}
					}
				}
				return 0;
			}
			break;
		case LIBAROMA_CTL_LIST_ITEM_MSG_TOUCH_CANCEL:
			{
				//printf("list item #%i -> touch canceled by scroll\n",item->id);
				mi->onchangeani=0;
				return 0;
			}
			break;
	}
	return 0;
} /* End of _libaroma_listitem_check_message */

/*
 * Function		: _libaroma_listitem_check_draw
 * Return Value: void
 * Descriptions: item draw routine
 */
void _libaroma_listitem_check_draw(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item,
		LIBAROMA_CANVASP cv,
		word bgcolor,
		byte state){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return;
	}
	_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;

	byte is_dark=libaroma_color_isdark(bgcolor);
	word textcolor, graycolor;
	word flags=item->flags;

	if (!(state&LIBAROMA_CTL_LIST_ITEM_DRAW_ADDONS)){
		int vpad = 8;
		int seph = (flags&LIBAROMA_LISTITEM_WITH_SEPARATOR)?1:0;
		byte small_icon = (flags&LIBAROMA_LISTITEM_CHECK_SMALL_ICON)?1:0;

		if (state&LIBAROMA_CTL_LIST_ITEM_DRAW_PUSHED){
			word selcolor = is_dark?RGB(444444):RGB(bbbbbb);
			libaroma_draw_rect(
				cv,
				0,0,
				cv->w,
				cv->h-libaroma_dp(1),
				selcolor,0xff
			);
			textcolor= is_dark?RGB(ffffff):RGB(000000);
			graycolor= is_dark?RGB(888888):RGB(777777);
		}
		else{
			textcolor= is_dark?RGB(ffffff):RGB(000000);
			graycolor= is_dark?RGB(888888):RGB(777777);
		}

		if ((item->next)&&(flags&LIBAROMA_LISTITEM_WITH_SEPARATOR)){
			if (!libaroma_listitem_nonitem(item->next)){
				int sepxp=0;
				if (flags&LIBAROMA_LISTITEM_SEPARATOR_TEXTALIGN){
					sepxp=libaroma_dp(72);
				}
				libaroma_draw_rect(
					cv,
					sepxp,
					cv->h-libaroma_dp(1),
					cv->w-sepxp,
					libaroma_dp(1),
					is_dark?RGB(555555):RGB(dddddd),
					0xff
				);
			}
		}

		int icoh=libaroma_dp(vpad*2+seph);
		int tw = cv->w-libaroma_dp(32);
			//(flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL)?52:88
		//);
		if (flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL){
			item->is_leftcontrol=1;
		} else { item->is_leftcontrol=0; }

		int tx = libaroma_dp(16);
		int dpsz=libaroma_dp(small_icon?24:40);
		if (mi->icon){
			icoh+=dpsz;
		}

		if ((mi->icon)||(item->flags&LIBAROMA_LISTITEM_CHECK_INDENT_NOICON)){
			tw-=libaroma_dp(small_icon?40:56);
			tx+=libaroma_dp(small_icon?40:56);
		}

		int ty = libaroma_dp(vpad*2);
		LIBAROMA_TEXT mtextp=NULL;
		LIBAROMA_TEXT etextp=NULL;

		/* prepare main text */
		int txtsh=0;
		int m_h=0;
		if (mi->main_text){
			mtextp = libaroma_text(
				mi->main_text,
				textcolor,
				tw,
				LIBAROMA_FONT(0,mi->font_small?2:4)|
				LIBAROMA_TEXT_FIXED_INDENT|
				LIBAROMA_TEXT_FIXED_COLOR|
				LIBAROMA_TEXT_NOHR|(mi->font_small?LIBAROMA_TEXT_SINGLELINE:0),
				137
			);
			m_h=libaroma_text_height(mtextp);
			ty+=m_h;
			txtsh+=m_h;
		}

		/* prepare extra text */
		int e_h = 0;
		int etremsz=0;
		if (mi->extra_text){
			etextp = libaroma_text(
				mi->extra_text,
				graycolor,
				tw,
				LIBAROMA_FONT(0,mi->font_small?1:3)|
				LIBAROMA_TEXT_FIXED_INDENT|
				LIBAROMA_TEXT_FIXED_COLOR|
				LIBAROMA_TEXT_NOHR|(mi->font_small?LIBAROMA_TEXT_SINGLELINE:0),
				143
			);
			e_h=libaroma_text_height(etextp)-(libaroma_font_size_px(mi->font_small?1:3) / 3.5);
			ty+=e_h;
			txtsh+=e_h;
			etremsz=2;
		}

		/* calculate whole height */
		ty+=libaroma_dp(vpad*2+seph);
		int my_h = MAX(icoh,ty);

		/* draw icon */
		if (mi->icon){
			if ((flags&LIBAROMA_LISTITEM_CHECK_FREE_ICON)||
				(!(flags&LIBAROMA_LISTITEM_CHECK_SHARED_ICON))){
				libaroma_draw(
					cv,
					mi->icon,
					libaroma_dp(16),
					(my_h>>1) - (dpsz>>1),
					1
				);
			}
			else{
				libaroma_draw_scale_smooth(
					cv, mi->icon,
					libaroma_dp(16),
					(my_h>>1) - (dpsz>>1),
					dpsz,
					dpsz,
					0, 0, mi->icon->w, mi->icon->h
				);
			}
		}

		int txt_sy=(my_h>>1)-((txtsh>>1)+libaroma_dp(2+etremsz+seph));

		/* draw main text */
		if (mtextp){
			libaroma_text_draw(cv,mtextp,tx,txt_sy);
			txt_sy+=m_h;
			libaroma_text_free(mtextp);
		}

		/* draw extra text */
		if (etextp){
			libaroma_text_draw(cv,etextp,tx,txt_sy);
			libaroma_text_free(etextp);
		}


		if (my_h!=mi->h){
			mi->h=my_h;
			libaroma_ctl_list_item_setheight(
				ctl, item, my_h
			);
		}
	}

	/* addons draw */
	if (!(state&LIBAROMA_CTL_LIST_ITEM_DRAW_CACHE)){
		byte is_switch = (flags&LIBAROMA_LISTITEM_CHECK_SWITCH)?1:0;
		float relstate=1;
		if ((item->state!=NULL)&&(mi->onchangeani)){
			if (item->state->ripple.touched==2){
				relstate=0;
			}
			else if (libaroma_ripple_current(&item->state->ripple,release_start)){
				float curelstate=libaroma_ripple_current(
					&item->state->ripple,release_state
				);
				if (curelstate<0.25){
					relstate=0;
				}
				else if (curelstate>0.75){
					relstate=1;
				}
				else{
					relstate=MAX(MIN((curelstate-0.25) * 2,1),0);
					relstate=libaroma_cubic_bezier_swiftout(relstate);
					relstate=MAX(MIN(1,relstate),0);
				}
			}
		}

		int xpos = cv->w - libaroma_dp(36);
		if (flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL){
			/* left position */
			xpos = libaroma_dp((item->flags&LIBAROMA_LISTITEM_CHECK_SMALL_ICON)?28:36);
		}
		int ypos = cv->h>>1;
		if (is_switch){
			word h_color_rest	 = RGB(ECECEC);
			word h_color_active = libaroma_colorget(ctl,NULL)->primary;
			word b_color_rest	 = RGB(B2B2B2);
			word b_color_active = libaroma_colorget(ctl,NULL)->primary_light;

			word bc0=mi->selected?b_color_rest:b_color_active;
			word bc1=mi->selected?b_color_active:b_color_rest;
			word hc0=mi->selected?h_color_rest:h_color_active;
			word hc1=mi->selected?h_color_active:h_color_rest;

			word bc = libaroma_alpha(bc0,bc1,relstate*0xff);
			word hc = libaroma_alpha(hc0,hc1,relstate*0xff);

			/* draw background */
			int b_width	 = libaroma_dp(34);
			int b_height	= libaroma_dp(14);

			float selrelstate = mi->selected?relstate:1-relstate;
			int base_x = xpos-(b_width>>1);
			int h_sz = libaroma_dp(20);
			int base_w = b_width - h_sz;

			item->checkwidth=b_width;
			item->checkx=base_x;
			int h_draw_x = base_x + round(base_w*selrelstate);
			int h_draw_y = ypos-(h_sz>>1);

			libaroma_gradient_ex1(cv,
				xpos-(b_width>>1),
				ypos-(b_height>>1),
				b_width,
				b_height,
				bc,bc,
				(b_height>>1),0x1111,
				0xff,0xff,
				0
			);

			int rsz = libaroma_dp(1);

			/* shadow */
			byte new_shadow = 1;
			if (item->state!=NULL){
				if (item->state->cache_client!=NULL){
					/* shadow already cached */
					libaroma_draw_opacity(cv,
						item->state->cache_client,
						h_draw_x-rsz,
						h_draw_y,3,0x30
					);
					new_shadow=0;
				}
			}
			if (new_shadow){
				LIBAROMA_CANVASP bmask = libaroma_canvas_ex(h_sz,h_sz,1);
				libaroma_canvas_setcolor(bmask,0,0);
				libaroma_gradient(bmask,0,0,h_sz,h_sz,0,0,h_sz>>1,0x1111);
				LIBAROMA_CANVASP scv = libaroma_blur_ex(bmask,rsz,1,0);
				libaroma_canvas_free(bmask);

				libaroma_draw_opacity(cv,scv,h_draw_x-rsz,h_draw_y,3,0x30);
				if (item->state!=NULL){
					/* will automatically freed by list control */
					item->state->cache_client=scv;
				}
				else{
					libaroma_canvas_free(scv);
				}
			}

			/* handle */
			libaroma_gradient_ex1(cv,
				h_draw_x,
				h_draw_y,
				h_sz,
				h_sz,
				hc,hc,
				(h_sz>>1),0x1111,
				0xff,0xff,
				0
			);
		}
		else{
			byte isoption=(LIBAROMA_LISTITEM_CHECK_OPTION&flags)?1:0;
			word cprimary=is_dark?
			libaroma_colorget(ctl,NULL)->primary_light:
			libaroma_colorget(ctl,NULL)->primary;
			word dprimary=libaroma_colorget(ctl,NULL)->control_secondary_text;
			int rsz	= libaroma_dp(18);
			item->checkwidth=rsz;
			item->checkx=xpos;
			/* init cache */
			byte new_cache = 0;
			LIBAROMA_CANVASP checkcache = NULL;
			if ((mi->selected)||(item->state!=NULL)){
				new_cache=1;
				if (item->state!=NULL){
					if (item->state->cache_client!=NULL){
						checkcache = item->state->cache_client;
						new_cache=0;
					}
				}
				if (new_cache){
					checkcache = libaroma_canvas_ex(rsz,rsz,1);
					memset(checkcache->alpha,0,rsz*rsz);

					if (isoption){
						libaroma_gradient_ex1(checkcache,
							0,0,rsz,rsz,cprimary,cprimary,rsz>>1,0x1111,
							0xff,0xff,2);
						int vrsz=rsz-libaroma_dp(4);
						libaroma_gradient_ex1(checkcache,
							libaroma_dp(2),libaroma_dp(2),
							vrsz,vrsz,bgcolor,bgcolor,vrsz>>1,0x1111,
							0xff,0xff,2|LIBAROMA_DRAW_NO_DST_ALPHA);
						vrsz-=libaroma_dp(4);
						libaroma_gradient_ex1(checkcache,
							libaroma_dp(4),libaroma_dp(4),
							vrsz,vrsz,cprimary,cprimary,vrsz>>1,0x1111,
							0xff,0xff,2|LIBAROMA_DRAW_NO_DST_ALPHA);
					}
					else{
						libaroma_gradient_ex1(checkcache,
							0,0,rsz,rsz,cprimary,cprimary,libaroma_dp(2),0x1111,
							0xff,0xff,2);

						/* tick */
						LIBAROMA_PATHP path=libaroma_path(12*rsz>>6, 27*rsz>>6);
						libaroma_path_add(path, 25*rsz>>6, 40*rsz>>6);
						libaroma_path_add(path, 52*rsz>>6, 13*rsz>>6);
						libaroma_path_add(path, 57*rsz>>6, 18*rsz>>6);
						libaroma_path_add(path, 25*rsz>>6, 50*rsz>>6);
						libaroma_path_add(path,	7*rsz>>6, 32*rsz>>6);
						libaroma_path_draw(checkcache, path, 0, 0, 2, 0.5);
						libaroma_path_free(path);
					}
					if (item->state!=NULL){
						/* will automatically freed by list control */
						item->state->cache_client=checkcache;
						new_cache=0;
					}
				}
			}

			if (relstate<1){
				float rrelstate = 1-relstate;
				int sel_sz = rsz * (mi->selected?relstate:rrelstate);
				int hal_sz = rsz * (mi->selected?rrelstate:relstate);
				if (hal_sz>0){
					libaroma_gradient_ex1(cv,
						xpos-(hal_sz>>1),
						ypos-(hal_sz>>1),
						hal_sz,
						hal_sz,
						dprimary,dprimary,
						libaroma_dp(isoption?hal_sz:2),0x1111,
						0xff,0xff,
						0
					);
					int irsz=hal_sz-libaroma_dp(4);
					if (irsz>0){
						libaroma_gradient_ex1(cv,
							xpos-(irsz>>1),
							ypos-(irsz>>1),
							irsz,irsz,
							bgcolor,bgcolor,
							libaroma_dp(isoption?irsz:1),0x1111,
							0xff,0xff,
							0
						);
					}
				}
				if (sel_sz>0){
					libaroma_draw_scale_nearest(
						cv, checkcache,
						xpos-(sel_sz>>1),
						ypos-(sel_sz>>1),
						sel_sz, sel_sz,
						0, 0, rsz, rsz
					);
				}
			}
			else if (mi->selected){
				libaroma_draw(
					cv, checkcache,
					xpos-(rsz>>1),
					ypos-(rsz>>1),
					1
				);
			}
			else{
				libaroma_gradient_ex1(cv,
					xpos-(rsz>>1),
					ypos-(rsz>>1),
					rsz,
					rsz,
					dprimary,dprimary,
					libaroma_dp(isoption?rsz:2),0x1111,
					0xff,0xff,
					0
				);
				int irsz=libaroma_dp(14);
				libaroma_gradient_ex1(cv,
					xpos-(irsz>>1),
					ypos-(irsz>>1),
					irsz,irsz,
					bgcolor,bgcolor,
					libaroma_dp(isoption?irsz:1),0x1111,
					0xff,0xff,
					0
				);
			}

			if (new_cache){
				libaroma_canvas_free(checkcache);
			}

		}

	}

} /* End of _libaroma_listitem_check_draw */

/*
 * Function		: _libaroma_listitem_check_release_internal
 * Return Value: void
 * Descriptions: release internal data
 */
void _libaroma_listitem_check_release_internal(_LIBAROMA_LISTITEM_CHECKP mi,
	word flags){
	if (mi->main_text){
		free(mi->main_text);
	}
	if (mi->extra_text){
		free(mi->extra_text);
	}
	if (mi->icon){
		if ((flags&LIBAROMA_LISTITEM_CHECK_FREE_ICON)||
			(!(flags&LIBAROMA_LISTITEM_CHECK_SHARED_ICON))){
			libaroma_canvas_free(mi->icon);
		}
	}
	free(mi);
} /* End of _libaroma_listitem_check_release_internal */

byte libaroma_listitem_check_set_cb(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item,
		LIBAROMA_LISTITEM_CB cb,
		voidp data){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return 0;
	}
	_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;
	if (mi->change_cb){
		mi->change_cb(
			ctl, item, item->id, mi->selected, mi->change_data, LIBAROMA_LISTITEM_CB_FREE
		);
		mi->change_cb=NULL;
		mi->change_data=NULL;
	}
	if (cb){
		mi->change_cb=cb;
		mi->change_data=data;
	}
	return 1;
}

/*
 * Function		: _libaroma_listitem_check_destroy
 * Return Value: void
 * Descriptions: destroy check item
 */
void _libaroma_listitem_check_destroy(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CTL_LIST_ITEMP item){
	if (item->handler!=&_libaroma_listitem_check_handler){
		return;
	}
	_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP) item->internal;
	if (mi->change_cb){
		mi->change_cb(
			ctl, item, item->id, mi->selected, mi->change_data, LIBAROMA_LISTITEM_CB_FREE
		);
	}
	_libaroma_listitem_check_release_internal(mi, item->flags);
} /* End of _libaroma_listitem_check_destroy */

/*
 * Function		: libaroma_listitem_check
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: create check item
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_check(
		LIBAROMA_CONTROLP ctl,
		int id,
		byte selected,
		const char * main_text,
		const char * extra_text,
		LIBAROMA_CANVASP icon,
		word flags,
		int at_index){
	/* check valid list control */
	if (!libaroma_ctl_list_is_valid(ctl)){
		ALOGW("listitem_check control is not valid list control");
		return 0;
	}
	_LIBAROMA_LISTITEM_CHECKP mi = (_LIBAROMA_LISTITEM_CHECKP)
		calloc(sizeof(_LIBAROMA_LISTITEM_CHECK),1);
	if (!mi){
		ALOGW("listitem_check cannot allocate internal data");
		return NULL;
	}
	mi->selected=selected;
	mi->icon=NULL;
	int vpad = 8;
	int seph = (flags&LIBAROMA_LISTITEM_WITH_SEPARATOR)?1:0;

	if (!icon){
		if (flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL){
			flags|=LIBAROMA_LISTITEM_CHECK_INDENT_NOICON;
		}
	}
	else{
		if (flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL){
			flags&=~LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL;
		}
	}
	if (flags&LIBAROMA_LISTITEM_CHECK_SMALL_ICON){
		mi->font_small=1;
		//ALOGI("Listitem uses SMALL font");
	}
		//else ALOGI("Listitem uses BIG font");
	/* init icon */
	int h = 0;
	if (icon){
		int dpsz=libaroma_dp((flags&LIBAROMA_LISTITEM_CHECK_SMALL_ICON)?24:40);
		if ((flags&LIBAROMA_LISTITEM_CHECK_FREE_ICON)||
			(!(flags&LIBAROMA_LISTITEM_CHECK_SHARED_ICON))){
			mi->icon=libaroma_canvas_ex(dpsz,dpsz,1);
			if (mi->icon){
				flags|=LIBAROMA_LISTITEM_CHECK_INDENT_NOICON;
				memset(mi->icon->alpha,0,mi->icon->s);
				libaroma_draw_scale_smooth(
					mi->icon, icon,
					0, 0, dpsz, dpsz,
					0, 0, icon->w, icon->h
				);
			}
			if (flags&LIBAROMA_LISTITEM_CHECK_FREE_ICON){
				libaroma_canvas_free(icon);
			}
		}
		else{
			mi->icon = icon;
		}
		h=dpsz;
	}
	mi->main_text=(main_text?strdup(main_text):NULL);
	mi->extra_text=(extra_text?strdup(extra_text):NULL);

	/* calculate height */
	/*
	int th = (mi->main_text?libaroma_font_size_px(4):0)*1.2;
	th += (mi->extra_text?libaroma_font_size_px(3):0)*1.2;
	th += libaroma_dp(4);
	*/
	int tw = ctl->w-libaroma_dp((flags&LIBAROMA_LISTITEM_CHECK_LEFT_CONTROL)?52:88);
	if ((mi->icon)||(flags&LIBAROMA_LISTITEM_CHECK_INDENT_NOICON)){
		tw-=libaroma_dp((flags&LIBAROMA_LISTITEM_CHECK_SMALL_ICON)?40:56);
	}
	int th = libaroma_dp(vpad*2);

	if (tw>0){
		if (mi->main_text){
			LIBAROMA_TEXT mtextp = libaroma_text(
				mi->main_text,
				0,
				tw,
				LIBAROMA_FONT(0,mi->font_small?2:4)|
				LIBAROMA_TEXT_FIXED_INDENT|
				LIBAROMA_TEXT_FIXED_COLOR|
				LIBAROMA_TEXT_NOHR|(mi->font_small?LIBAROMA_TEXT_SINGLELINE:0),
				137
			);
			th+=libaroma_text_height(mtextp);
			libaroma_text_free(mtextp);
		}

		/* prepare extra text */
		if (mi->extra_text){
			LIBAROMA_TEXT etextp = libaroma_text(
				mi->extra_text,
				0,
				tw,
				LIBAROMA_FONT(0,mi->font_small?1:3)|
				LIBAROMA_TEXT_FIXED_INDENT|
				LIBAROMA_TEXT_FIXED_COLOR|
				LIBAROMA_TEXT_NOHR|(mi->font_small?LIBAROMA_TEXT_SINGLELINE:0),
				143
			);
			th+=libaroma_text_height(etextp);
			th-=(libaroma_font_size_px(mi->font_small?1:3) / 3.5);
			libaroma_text_free(etextp);
		}
	}

	h = MAX(h,th)+libaroma_dp(vpad*2+seph);
	mi->h=h;
	// create group variable and assign it if item is option checkbox
	LIBAROMA_LISTITEM_CHECK_GROUPP grp;
	if (flags&LIBAROMA_LISTITEM_CHECK_OPTION){
		//if groups stack doesn't exist, initialize it
		if (libaroma_ctl_list_get_groups(ctl)==NULL)
			libaroma_ctl_list_init_opt_groups(ctl);
		//chek if there are any groups at stack
		int grp_count=libaroma_ctl_list_get_groups(ctl)->n;
		ALOGI("Checking for initial group");
		//if there are no groups, create one
		if (!grp_count){
			ALOGI("Empty group list, creating first group");
			grp=libaroma_listitem_check_create_group(ctl);
		}
		else { //if there are groups, use the last one
		ALOGI("Going to use group %d (grp_count is %d)", grp_count-1, grp_count);
			grp=libaroma_listitem_check_get_group_at(ctl, grp_count-1);
		}
	}
	LIBAROMA_CTL_LIST_ITEMP item = libaroma_ctl_list_add_item_internal(
		ctl,
		id,
		h,
		LIBAROMA_CTL_LIST_ITEM_RECEIVE_TOUCH|flags,
		(voidp) mi,
		&_libaroma_listitem_check_handler,
		at_index
	);
	if (!item){
		ALOGW("listitem_check add_item_internal failed");
		_libaroma_listitem_check_release_internal(mi,flags);
		return NULL;
	}
	if (flags&LIBAROMA_LISTITEM_CHECK_OPTION){
		ALOGI("Adding option item to initial group");
		libaroma_listitem_check_add_to_group(grp, item); //add to the group obtained above
	}
	return item;
} /* End of libaroma_listitem_check */

/********************GROUPS MANAGEMENT**********************/

/*
 * Function		: libaroma_listitem_check_create_named_group
 * Return Value: LIBAROMA_LISTITEM_CHECK_GROUPP
 * Descriptions: create listitem group with optional caption
 */
LIBAROMA_LISTITEM_CHECK_GROUPP libaroma_listitem_check_create_named_group(
	LIBAROMA_CONTROLP list,
	char *name
){
	if (list==NULL) return NULL;
	ALOGI("Creating new group! name is %s", name);
	if (libaroma_ctl_list_get_groups(list)==NULL)
		libaroma_ctl_list_init_opt_groups(list);
	int grp_count=libaroma_ctl_list_get_groups(list)->n;
	LIBAROMA_LISTITEM_CHECK_GROUPP grp=malloc(sizeof(LIBAROMA_LISTITEM_CHECK_GROUP));
	if (grp==NULL) {
		ALOGE("Cannot allocate memory for group!");
		return NULL;
	}
	grp->index=grp_count;
	grp->list_index=libaroma_ctl_list_get_item_count(list);//groups->n;
	grp->list=list;
	grp->items=libaroma_stack(NULL);
	if (name!=NULL){
		libaroma_listitem_caption(list, grp->index, name, -1);
	}
	libaroma_stack_add_at(libaroma_ctl_list_get_groups(list), grp_count, (voidp)grp, sizeof(LIBAROMA_LISTITEM_CHECK_GROUP));
	ALOGI("New group count is %d", grp_count);
	return grp;
}

/*
 * Function		: libaroma_listitem_check_add_to_group
 * Return Value: void
 * Descriptions: add item to listitem group
 */
void libaroma_listitem_check_add_to_group(
	LIBAROMA_LISTITEM_CHECK_GROUPP group,
	LIBAROMA_CTL_LIST_ITEMP item
){
	if (group==NULL || item==NULL) return;
	LIBAROMA_LISTITEM_CHECK_GROUPP oldgrp=libaroma_listitem_check_find_group(group->list, item);
	if (oldgrp!=NULL) //remove item from other groups
		libaroma_listitem_check_remove_from_group(group, item); //TODO: reorder list after this
	ALOGI("Adding item %d to group %d", group->items->n, group->index);
	libaroma_stack_add_at(group->items, group->items->n, (voidp)item, sizeof(LIBAROMA_CTL_LIST_ITEM));
}

/*
 * Function		: libaroma_listitem_check_remove_from_group
 * Return Value: void
 * Descriptions: remove item from group
 */
void libaroma_listitem_check_remove_from_group(
	LIBAROMA_LISTITEM_CHECK_GROUPP group,
	LIBAROMA_CTL_LIST_ITEMP item
){
	int index=libaroma_listitem_check_get_ingroup_index(group, item);
	ALOGI("Remove from group: got index %d (%svalid)", index, index<0?"in":"");
	if (index>=0)
		libaroma_stack_delete(group->items, index);
}

/*
 * Function		: libaroma_listitem_check_get_group_at
 * Return Value: LIBAROMA_LISTITEM_CHECK_GROUPP
 * Descriptions: get listitem group from list at index
 */
LIBAROMA_LISTITEM_CHECK_GROUPP libaroma_listitem_check_get_group_at(
	LIBAROMA_CONTROLP list,
	int index
){
	if (index<0 || list==NULL)
		return NULL;
	int i;
	ALOGI("Looking for group at index %d", index);
	for (i=0; i<libaroma_ctl_list_get_groups(list)->n; i++){
		LIBAROMA_LISTITEM_CHECK_GROUPP grp=(LIBAROMA_LISTITEM_CHECK_GROUPP)libaroma_stack_get(libaroma_ctl_list_get_groups(list), i);
		ALOGI("Group found at id %d, checking if null", i);
		if (grp==NULL) return NULL;
		ALOGI("Group wasn't NULL, checking for %d==%d", grp->index, index);
		if (grp->index==index){
			ALOGI("Group index is the same!");
			ALOGI("Checking for same list...");
			if (grp->list==list){
				ALOGI("List is the same!");
				return grp;
			}
		}
	}
	printf("ERROR: Could not find group!\n");
	return NULL;
}

/*
 * Function		: libaroma_listitem_check_get_ingroup_index
 * Return Value: int
 * Descriptions: get item index inside group
 */
int libaroma_listitem_check_get_ingroup_index(
	LIBAROMA_LISTITEM_CHECK_GROUPP group,
	LIBAROMA_CTL_LIST_ITEMP item
){
	if (group==NULL) {
		ALOGI("Find item index called with null group!");
		return -1;
	}
	int i;
	ALOGI("Going to look for item index in group with %d items", group->items->n);
	for (i=0; i<group->items->n; i++){
		LIBAROMA_CTL_LIST_ITEMP itm=(LIBAROMA_CTL_LIST_ITEMP)libaroma_stack_get(group->items, i);
		if (libaroma_ctl_list_items_equal(itm, item))
			return i;
	}
	ALOGI("Could not find item index in group!\n");
	return -1;
}

/*
 * Function		: libaroma_listitem_check_get_ingroup_item
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: get item at index inside group
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_check_get_ingroup_item(
	LIBAROMA_LISTITEM_CHECK_GROUPP group,
	int index
){
	LIBAROMA_CTL_LIST_ITEMP item=(LIBAROMA_CTL_LIST_ITEMP)libaroma_stack_get(group->items, index);
	if (item==NULL)
		ALOGI("Could not find item in group!\n");
	return item;
}

/*
 * Function		: libaroma_listitem_check_find_group
 * Return Value: LIBAROMA_LISTITEM_CHECK_GROUPP
 * Descriptions: get group for item at list
 */
LIBAROMA_LISTITEM_CHECK_GROUPP libaroma_listitem_check_find_group(
	LIBAROMA_CONTROLP list,
	LIBAROMA_CTL_LIST_ITEMP item
){
	int i;
	ALOGI("Trying to find group for item at %p", item);
	for (i=0; i<libaroma_ctl_list_get_groups(list)->n; i++){
		LIBAROMA_LISTITEM_CHECK_GROUPP grp=(LIBAROMA_LISTITEM_CHECK_GROUPP)libaroma_stack_get(libaroma_ctl_list_get_groups(list), i);
		if (libaroma_listitem_check_get_ingroup_index(grp/*groups[i]*/, item)!=-1)
			return grp;
	}
	return NULL;
}

/*
 * Function		: libaroma_listitem_check_get_group_selected
 * Return Value: LIBAROMA_CTL_LIST_ITEMP
 * Descriptions: get selected item at group
 */
LIBAROMA_CTL_LIST_ITEMP libaroma_listitem_check_get_group_selected(
	LIBAROMA_LISTITEM_CHECK_GROUPP group
){
	if (group==NULL) return NULL;
	int index=libaroma_listitem_check_get_group_selected_index(group);
	return libaroma_listitem_check_get_ingroup_item(group, index);
}

/*
 * Function		: libaroma_listitem_check_get_group_selected_index
 * Return Value: int
 * Descriptions: get selected item index at group
 */
int libaroma_listitem_check_get_group_selected_index(
	LIBAROMA_LISTITEM_CHECK_GROUPP group
){
	if (group==NULL) return -1;
	int i;
	int item_count=group->items->n;
	ALOGI("SEARCHING SELECTED ITEM AT GROUP %d, MAX COUNT IS %d", group->index, item_count);
	for (i=0; i<item_count; i++){
		ALOGI("SEARCHING ITEM AT INDEX %d", i);
		LIBAROMA_CTL_LIST_ITEMP item=(LIBAROMA_CTL_LIST_ITEMP)libaroma_stack_get(group->items, i);
		if (item==NULL) break;
		if (!libaroma_listitem_isoption(item))
			continue;
		_LIBAROMA_LISTITEM_CHECKP check=(_LIBAROMA_LISTITEM_CHECKP)item->internal;
		ALOGI("FOUND OPT ITEM, SELECTED IS %d", check->selected);
		if (check->selected)
			return i;
	}
	return -1;
}

/*
 * Function		: libaroma_listitem_check_get_selected_group
 * Return Value: LIBAROMA_LISTITEM_CHECK_GROUPP
 * Descriptions: get last touched group
 */
LIBAROMA_LISTITEM_CHECK_GROUPP libaroma_listitem_check_get_selected_group(
	LIBAROMA_CONTROLP list
){
	if (list==NULL)
		return NULL;
	LIBAROMA_CTL_LIST_ITEMP touched=libaroma_ctl_list_get_touched_item(list);
	LIBAROMA_LISTITEM_CHECK_GROUPP grp=libaroma_listitem_check_find_group(list, touched);
	if (grp==NULL){
		ALOGI("group for touched item not found");
		return NULL;
	}
	else {
		//int index=libaroma_listitem_check_get_ingroup_index(grp, touched);
		ALOGI("item touched at group %d", grp->index);
		return grp;
	}
}

/*
 * Function		: _libaroma_listitem_check_group_uncheck_others
 * Return Value: void
 * Descriptions: uncheck items other than <item> inside group
 */
void _libaroma_listitem_check_group_uncheck_others(
	LIBAROMA_LISTITEM_CHECK_GROUPP group,
	LIBAROMA_CTL_LIST_ITEMP item
){
	if (group==NULL) return;
	int i;
	for (i=0; i<group->items->n; i++){
		LIBAROMA_CTL_LIST_ITEMP itm=libaroma_listitem_check_get_ingroup_item(group, i);
		if (item==NULL) {
			ALOGI("Item at %d is NULL! Exiting", i);
			break;
		}
		if (!libaroma_listitem_isoption(itm)){
			ALOGI("Not disabling non-option item");
			continue;
		}
		if (!libaroma_ctl_list_items_equal(itm, item)){
			ALOGI("Item %d found, disabling", i);
			libaroma_listitem_set_selected(group->list, itm, 0);
		}
	}
}

#ifdef __cplusplus
}
#endif

#endif /* __libaroma_listitem_check_c__ */
