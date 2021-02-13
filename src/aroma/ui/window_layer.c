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
 * Filename		: window_layer.c
 * Description : multi layer window
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 07/04/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_window_layer_c__
#define __libaroma_window_layer_c__
#include <aroma_internal.h>
#include "ui_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
/* max touch x dp to trigger sidebar */
#define _LIBAROMA_LAYER_SIDEBAR_TOUCH_DP 16
/* max touch x dp to trigger pulldown */
#define _LIBAROMA_LAYER_PULLDOWN_TOUCH_DP 16

/* root window client data */
typedef struct{
	LIBAROMA_WINDOWP win;
	LIBAROMA_WINDOWP sidebar;
	LIBAROMA_WINDOWP pulldown;
	byte sidebar_showed;
	byte pulldown_showed;
	int sidebar_xpos;
	int sidebar_req_x;
	int pulldown_ypos;
	int pulldown_req_y;
	int sidebar_velocity;
	int pulldown_velocity;
	byte pulldown_drawtype;
	byte redraw;

	LIBAROMA_MUTEX mutex;
	LIBAROMA_CANVASP tdc;
	byte on_direct_canvas;
	byte (*ori_ui_thread)(LIBAROMA_WINDOWP);

	byte pd_touched;
	byte sb_touched;
	int touch_x;
	int touch_y;

	byte pd_allow_scroll;
	byte sb_allow_scroll;
	long pd_client_touch_start;
	long sb_client_touch_start;
	LIBAROMA_MSG pd_pretouched_msg;
	LIBAROMA_MSG sb_pretouched_msg;
	LIBAROMA_CONTROLP pd_pretouched;
	LIBAROMA_CONTROLP sb_pretouched;

	LIBAROMA_WINDOW_SIDEBAR_SLIDE_CB slide_cb;
	LIBAROMA_WINDOW_PULLDOWN_SLIDE_CB pulldown_slide_cb;
	LIBAROMA_FLING pd_fling;
	LIBAROMA_FLING sb_fling;
} _LIBAROMA_WINDOW_LAYER, *_LIBAROMA_WINDOW_LAYERP;

/* window handler */
byte _libaroma_window_layer_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h);
void _libaroma_window_layer_postfree(LIBAROMA_WINDOWP win);
LIBAROMA_CANVASP _libaroma_window_layer_control_draw_begin(
	LIBAROMA_WINDOWP win,LIBAROMA_CONTROLP cctl
);
byte _libaroma_window_layer_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval
);
byte _libaroma_window_layer_updatedc(LIBAROMA_WINDOWP win);
static LIBAROMA_WINDOW_HANDLER _libaroma_window_layer_handler={
	prefree:NULL,
	postfree:_libaroma_window_layer_postfree,
	updatebg:NULL,
	invalidate:NULL,
	sync:_libaroma_window_layer_sync,
	message_hooker:_libaroma_window_layer_message_hooker,
	control_draw_flush:NULL,
	control_erasebg:NULL,
	control_isvisible:NULL,
	control_draw_begin:_libaroma_window_layer_control_draw_begin
};

/*
 * Function		: _libaroma_window_layer_check
 * Return Value: _LIBAROMA_WINDOW_LAYERP
 * Descriptions: get root window client data
 */
static inline _LIBAROMA_WINDOW_LAYERP _libaroma_window_layer_check(
		LIBAROMA_WINDOWP win){
	if (!win){
		return NULL;
	}
	if (win->parent!=NULL){
		return NULL;
	}
	if (win->handler!=&_libaroma_window_layer_handler){
		return NULL;
	}
	return (_LIBAROMA_WINDOW_LAYERP) win->client_data;
} /* End of _libaroma_window_layer_check */

/*
 * Function		: _libaroma_window_layer_postfree
 * Return Value: void
 * Descriptions: postfree client_data
 */
void _libaroma_window_layer_postfree(LIBAROMA_WINDOWP win){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return;
	}

	if (me->sidebar){
		if (me->sidebar->active){
			me->sidebar->active=0;
			LIBAROMA_MSG msgia;
			msgia.msg=LIBAROMA_MSG_WIN_INACTIVE;
			int i;
			for (i=0;i<me->sidebar->childn;i++){
				if (me->sidebar->childs[i]->handler->message){
					me->sidebar->childs[i]->handler->message(
						me->sidebar->childs[i], &msgia
					);
				}
			}
		}
		libaroma_mutex_lock(me->mutex);
		libaroma_window_free(me->sidebar);
		libaroma_mutex_unlock(me->mutex);
	}
	if (me->pulldown){
		if (me->pulldown->active){
			me->pulldown->active=0;
			LIBAROMA_MSG msgia;
			msgia.msg=LIBAROMA_MSG_WIN_INACTIVE;
			int i;
			for (i=0;i<me->pulldown->childn;i++){
				if (me->pulldown->childs[i]->handler->message){
					me->pulldown->childs[i]->handler->message(
						me->pulldown->childs[i], &msgia
					);
				}
			}
		}
		libaroma_mutex_lock(me->mutex);
		libaroma_window_free(me->pulldown);
		libaroma_mutex_unlock(me->mutex);
	}
	libaroma_mutex_lock(me->mutex);
	if (me->tdc){
		libaroma_canvas_free(me->tdc);
	}
	win->ui_thread=me->ori_ui_thread;
	win->handler = NULL;
	win->client_data = NULL;
	libaroma_mutex_unlock(me->mutex);
	libaroma_mutex_free(me->mutex);
	free(me);
} /* End of _libaroma_window_layer_postfree */

/*
 * Function		: _libaroma_window_layer_updatedc
 * Return Value: byte
 * Descriptions: update nondirect dc
 */
byte _libaroma_window_layer_updatedc(LIBAROMA_WINDOWP win){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	if (win->active!=1){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (!me->on_direct_canvas){
		if (me->tdc){
			byte sidebar_draw=0;
			byte pulldown_draw=0;
			if (win->active==1){
				if (me->sidebar_showed){
					if (me->sidebar){
						if (me->sidebar_xpos>0){
							libaroma_draw_ex(
								win->dc,
								me->sidebar->dc,
								0, me->sidebar->y,
								me->sidebar->w-me->sidebar_xpos, 0,
								me->sidebar_xpos,
								me->sidebar->h,
								0,0xff
							);
							libaroma_draw_ex(win->dc,me->tdc,
								me->sidebar_xpos,0,
								me->sidebar_xpos,0,
								win->dc->w-me->sidebar_xpos,win->dc->h,
								LIBAROMA_DRAW_TO_BLACK,
								245-(150*me->sidebar_xpos/me->sidebar->w)
							);

							sidebar_draw=1;
						}
					}
				}
				if (me->pulldown_showed){
					if (me->pulldown){
						if (me->pulldown_ypos>0){
							libaroma_draw_ex(				//draw pulldown window
								win->dc,													//dest
								me->pulldown->dc,										//src
								0, 															//destx
								(me->pulldown_drawtype==LIBAROMA_PULLDOWN_CLEAN)?
									0:me->pulldown->y,									//desty
								0, 															//srcx
								(me->pulldown_drawtype==LIBAROMA_PULLDOWN_CLEAN)?
									0:(me->pulldown->h-me->pulldown_ypos),		//srcy
								win->dc->w,													//width
								(me->pulldown_drawtype==LIBAROMA_PULLDOWN_CLEAN)?
									me->pulldown_ypos:me->pulldown->h,			//height
								0,0xff														//usealpha, level
							);
							if ((win->dc->h - me->pulldown_ypos) > 0){
								libaroma_draw_ex(				//draw alpha section
									win->dc,me->tdc,										//dest, src
									0,me->pulldown_ypos,								//destx, desty
									0,me->pulldown_ypos,								//srcx, srcy
									win->dc->w, win->dc->h-me->pulldown_ypos,			//width, height
									(me->pulldown_drawtype==LIBAROMA_PULLDOWN_CLEAN)?
										0:LIBAROMA_DRAW_TO_BLACK,							//alphaflags
									(me->pulldown_drawtype==LIBAROMA_PULLDOWN_CLEAN)?
										255:(245-(150*me->pulldown_ypos/me->pulldown->h))//alphalevel
								);
							}
							//printf("drawing window at x %d, y %d (srcx %d, srcy %d) with w %d, h %d\n",
							//		0, me->pulldown->y, 0, me->pulldown->h-me->pulldown_ypos,
							//		win->dc->w, me->pulldown->h);
							//printf("pulldown has width %d, height %d, x %d and y %d\n",
							//					win->dc->w, me->pulldown->h, 0, me->pulldown_ypos);
							pulldown_draw=1;
						}
					}
				}
			}
			if (!sidebar_draw && !pulldown_draw){
				libaroma_draw(win->dc,me->tdc,0,0,0);
			}
		}
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of _libaroma_window_layer_updatedc */

/*
 * Function		: _libaroma_window_layer_sync
 * Return Value: byte
 * Descriptions: window sync
 */
byte _libaroma_window_layer_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	if (!win->lock_sync){
		if (!libaroma_window_isactive(win)){
			ALOGW("libaroma_window_sync win is not active window");
			return 0;
		}
		if (win->dc==NULL){
			ALOGW("window_invalidate dc is null");
			return 0;
		}
		_libaroma_window_layer_updatedc(win);
		/* sync workspace */
		libaroma_wm_sync(win->x+x,win->y+y,w,h);
	}
	return 1;
} /* End of _libaroma_window_layer_sync */

/*
 * Function		: _libaroma_window_layer_control_draw_begin
 * Return Value: LIBAROMA_CANVASP
 * Descriptions: get canvas for child control
 */
LIBAROMA_CANVASP _libaroma_window_layer_control_draw_begin(
	LIBAROMA_WINDOWP win,LIBAROMA_CONTROLP cctl
){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return NULL;
	}
	LIBAROMA_CANVASP c=NULL;
	libaroma_mutex_lock(me->mutex);
	LIBAROMA_CANVASP dc = (me->on_direct_canvas)?win->dc:me->tdc;
	if (!dc){
		libaroma_mutex_unlock(me->mutex);
		return NULL;
	}
	c = libaroma_canvas_area(
		dc, cctl->x, cctl->y, cctl->w, cctl->h
	);
	libaroma_mutex_unlock(me->mutex);
	return c;
} /* End of _libaroma_window_layer_control_draw_begin */

/*
 * Function		: libaroma_window_layer_direct_canvas
 * Return Value: byte
 * Descriptions: set as direct canvas
 */
byte libaroma_window_layer_direct_canvas(LIBAROMA_WINDOWP win, byte state){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (state){
		if (!me->on_direct_canvas){
			if ((win->dc)&&(me->tdc)) {
				libaroma_draw(win->dc,me->tdc,0,0,0);
			}
			if (me->tdc){
				libaroma_canvas_free(me->tdc);
				me->tdc=NULL;
			}
			me->on_direct_canvas=1;
		}
	}
	else{
		if (me->on_direct_canvas){
			if (win->dc){
				if (!me->tdc){
					me->tdc = libaroma_canvas(
						win->dc->w, win->dc->h
					);
				}
				if (me->tdc) {
					libaroma_draw(me->tdc,win->dc,0,0,0);
				}
			}
			me->on_direct_canvas=0;
		}
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of libaroma_window_layer_direct_canvas */

/*
 * Function		: _libaroma_window_layer_set_sidebar_pos
 * Return Value: byte
 * Descriptions: set sidebar position
 */
byte _libaroma_window_layer_set_sidebar_pos(LIBAROMA_WINDOWP win, int x){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (!me->sidebar){
		libaroma_mutex_unlock(me->mutex);
		return 0;
	}
	if (x>0){
		if (x>me->sidebar->w){
			x=me->sidebar->w;
		}
		if (!me->sidebar_showed){
			if (!me->sidebar->active){
				/* activate sidebar */
				LIBAROMA_MSG msgr;
				dword rv=0;
				msgr.msg=LIBAROMA_MSG_WIN_MEASURED;
				libaroma_mutex_unlock(me->mutex);
				me->sidebar->handler->message_hooker(me->sidebar,&msgr,&rv);
				msgr.msg=LIBAROMA_MSG_WIN_ACTIVE;
				me->sidebar->handler->message_hooker(me->sidebar,&msgr,&rv);
				libaroma_window_invalidate(me->sidebar,0);
				libaroma_mutex_lock(me->mutex);
			}
			libaroma_mutex_unlock(me->mutex);
			libaroma_window_layer_direct_canvas(win,0);
			libaroma_mutex_lock(me->mutex);
			me->sidebar_showed=1;
		}
		if (me->sidebar_xpos!=x){
			me->redraw=1;
		}
		if (x==me->sidebar->w){
			if (me->sidebar_showed!=2){
				me->sidebar_showed=2;
			}
		}
		else if (me->sidebar_showed==2){
			me->sidebar_showed=1;
		}
		me->sidebar_xpos=x;
	}
	else{
		if (me->sidebar_showed){
			me->sidebar_showed=0;
			libaroma_mutex_unlock(me->mutex);
			libaroma_window_layer_direct_canvas(win,1);
			libaroma_mutex_lock(me->mutex);
		}
		if (me->sidebar_xpos!=0){
			me->redraw=1;
		}
		me->sidebar_xpos=0;
	}
	if (me->slide_cb){
		me->slide_cb(me->sidebar, me->sidebar_xpos, me->sidebar->w);
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of _libaroma_window_layer_set_sidebar_pos */

/*
 * Function		: _libaroma_window_layer_set_pulldown_pos
 * Return Value: byte
 * Descriptions: set pulldown position
 */
byte _libaroma_window_layer_set_pulldown_pos(LIBAROMA_WINDOWP win, int y){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (!me->pulldown){
		libaroma_mutex_unlock(me->mutex);
		return 0;
	}
	if (y>0){
		if (y>me->pulldown->h){
			printf("Touch Y %d is greater than window height %d!\n", y, me->pulldown->h);
			y=me->pulldown->h;
		}
		if (!me->pulldown_showed){
			if (!me->pulldown->active){
				/* activate pulldown */
				LIBAROMA_MSG msgr;
				dword rv=0;
				msgr.msg=LIBAROMA_MSG_WIN_MEASURED;
				libaroma_mutex_unlock(me->mutex);
				me->pulldown->handler->message_hooker(me->pulldown,&msgr,&rv);
				msgr.msg=LIBAROMA_MSG_WIN_ACTIVE;
				me->pulldown->handler->message_hooker(me->pulldown,&msgr,&rv);
				libaroma_window_invalidate(me->pulldown,0);
				libaroma_mutex_lock(me->mutex);
			}
			libaroma_mutex_unlock(me->mutex);
			libaroma_window_layer_direct_canvas(win,0);
			libaroma_mutex_lock(me->mutex);
			me->pulldown_showed=1;
		}
		if (me->pulldown_ypos!=y){
			me->redraw=1;
		}
		if (y==me->pulldown->y){
			if (me->pulldown_showed!=2){
				me->pulldown_showed=2;
			}
		}
		else if (me->pulldown_showed==2){
			me->pulldown_showed=1;
		}
		me->pulldown_ypos=y;
	}
	else{
		if (me->pulldown_showed){
			me->pulldown_showed=0;
			libaroma_mutex_unlock(me->mutex);
			libaroma_window_layer_direct_canvas(win,1);
			libaroma_mutex_lock(me->mutex);
		}
		if (me->pulldown_ypos!=0){
			me->redraw=1;
		}
		me->pulldown_ypos=0;
	}
	if (me->pulldown_slide_cb){
		me->pulldown_slide_cb(me->pulldown, me->pulldown_ypos, me->pulldown->h);
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of _libaroma_window_layer_set_pulldown_pos */

byte _libaroma_window_layer_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	if (win->active==1){
		if (!me->pulldown_showed && me->sidebar_showed==2){
			if (me->sidebar->handler->message_hooker(
					me->sidebar, msg, retval
				)){
				return 1;
			}
		}/*
		if (me->sidebar_showed==2){
			if (me->sidebar){
			}
		}*/
		else if (me->pulldown_showed==2){
			if (me->pulldown){
				if (me->pulldown->handler->message_hooker(
						me->pulldown, msg, retval
					)){
					return 1;
				}
			}
		}
		else if (me->pd_touched==10){
			/* cancel any touch event */
			if (msg->msg==LIBAROMA_MSG_TOUCH){
				return 1;
			}
		}
		else if (me->pulldown || me->sidebar){
			if (me->pulldown){
				switch (msg->msg){
					case LIBAROMA_MSG_TOUCH:
						{
							int x = msg->x;
							int y = msg->y;
							libaroma_window_calculate_pos(win,NULL,&x,&y);
							if (msg->state==LIBAROMA_HID_EV_STATE_DOWN){
								if (y<libaroma_dp(_LIBAROMA_LAYER_PULLDOWN_TOUCH_DP) ||
								(y>=(me->pulldown->h-libaroma_dp(20)) && me->pulldown_showed)){
									// start showing pulldown
									printf("Touching to show/hide! y=%d\n", y);
									libaroma_mutex_lock(me->mutex);
									me->pulldown_velocity=0;
									if (y>=(me->pulldown->h-libaroma_dp(20))){
										me->pulldown_req_y=me->pulldown->h;
									}
									else me->pulldown_req_y=libaroma_dp(15);
									me->pd_touched=1;
									me->touch_x=x;
									me->touch_y=y;
									libaroma_fling_down(&me->pd_fling, y);
									libaroma_mutex_unlock(me->mutex);
									return 1;
								}
							}
							else if (me->pd_touched){
								if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
									libaroma_mutex_lock(me->mutex);/*
									if (y>=(me->pulldown->h-libaroma_dp(20))){
										me->pulldown_req_y=me->pulldown->h;
										libaroma_fling_move(&me->fling, me->pulldown->h);
										libaroma_mutex_unlock(me->mutex);
										return 1;
									}*/
									int reqy=y;
									if (me->pd_touched==2){
										reqy=(me->pulldown->h)+(y-me->touch_y);
									}
									me->pulldown_req_y=MAX(
										libaroma_dp(_LIBAROMA_LAYER_PULLDOWN_TOUCH_DP),reqy);
									libaroma_fling_move(&me->pd_fling, y);
									libaroma_mutex_unlock(me->mutex);
								}
								else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
									libaroma_mutex_lock(me->mutex);
									me->pulldown_velocity=
										((int) (libaroma_fling_up(&me->pd_fling, y)*1.3)>>8);
									if (me->pulldown_velocity>=0||y<=(me->pulldown->h)/2){
										printf("Swiped up!\n");
										me->pulldown_req_y=0;
									}
									else{
										printf("Swiped down!\n");
										me->pulldown_req_y=me->pulldown->h;
										me->pulldown_velocity=abs(me->pulldown_velocity);
									}
									if (me->pulldown_velocity){
										/* fix velocity */
										int diff = me->pulldown->h;
										me->pulldown_velocity = MAX(MIN(
											me->pulldown_velocity, 0.45*diff),0.05*diff);
									}
									if (me->pulldown_req_y!=me->pulldown_ypos){
										me->pd_touched=10;
									}
									libaroma_mutex_unlock(me->mutex);
								}
								return 1;
							}
						}
						break;
				}
			}
			if (me->sidebar){
				switch (msg->msg){
				case LIBAROMA_MSG_TOUCH:
					{
						int x = msg->x;
						int y = msg->y;
						libaroma_window_calculate_pos(win,NULL,&x,&y);
						if (msg->state==LIBAROMA_HID_EV_STATE_DOWN){
							if (x<libaroma_dp(_LIBAROMA_LAYER_SIDEBAR_TOUCH_DP)){
								libaroma_mutex_lock(me->mutex);
								me->sidebar_velocity=0;
								me->sidebar_req_x=libaroma_dp(15);
								me->sb_touched=1;
								me->touch_x=x;
								me->touch_y=y;
								libaroma_fling_down(&me->sb_fling, x);
								libaroma_mutex_unlock(me->mutex);
								return 1;
							}
						}
						else if (me->sb_touched){
							if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
								libaroma_mutex_lock(me->mutex);
								int reqx=x;
								if (me->sb_touched==2){
									reqx=(me->sidebar->w)+(x-me->touch_x);
								}
								me->sidebar_req_x=MAX(
									libaroma_dp(_LIBAROMA_LAYER_SIDEBAR_TOUCH_DP),reqx);
								libaroma_fling_move(&me->sb_fling, x);
								libaroma_mutex_unlock(me->mutex);
							}
							else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
								libaroma_mutex_lock(me->mutex);
								me->sidebar_velocity=
									((int) (libaroma_fling_up(&me->sb_fling, x)*1.3)>>8);
								if (me->sidebar_velocity>=0){
									me->sidebar_req_x=0;
								}
								else{
									me->sidebar_req_x=me->sidebar->w;
									me->sidebar_velocity=abs(me->sidebar_velocity);
								}
								if (me->sidebar_velocity){
									/* fix velocity */
									int diff = me->sidebar->w;
									me->sidebar_velocity = MAX(MIN(
										me->sidebar_velocity, 0.45*diff),0.05*diff);
								}
								if (me->sidebar_req_x!=me->sidebar_xpos){
									me->sb_touched=10;
								}
								libaroma_mutex_unlock(me->mutex);
							}
							return 1;
						}
					}
					break;
				}
			}
		}
	}
	return 0;
}

/*
 * Function		: _libaroma_window_layer_ui_thread
 * Return Value: byte
 * Descriptions: layered window ui thread
 */
byte _libaroma_window_layer_ui_thread(LIBAROMA_WINDOWP win) {
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	byte need_sync=0;
	if (win->active==1){
		libaroma_mutex_lock(me->mutex);
		if ((me->sidebar)&&(me->sidebar_req_x!=-1)){
			/* show - hide sidebar */
			if (!me->sidebar->active){
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_sidebar_pos(win,1);
				libaroma_mutex_lock(me->mutex);
			}
			else if (me->sidebar->w<=0){
				/* invalid sidebar */
				me->sidebar_req_x=-1;
			}
			else if (me->sidebar_req_x!=me->sidebar_xpos){
				int move_sz = (me->sidebar_req_x-me->sidebar_xpos);
				if (me->sidebar_velocity!=0){
					me->sidebar_velocity=(me->sidebar_velocity*246)>>8;
					int minw=MAX(1,0.05*me->sidebar->w);
					if (me->sidebar_velocity<=minw){
						me->sidebar_velocity=minw;
					}
					if (move_sz<0){
						move_sz = 0-me->sidebar_velocity;
					}
					else{
						move_sz = me->sidebar_velocity;
					}
				}
				else{
					move_sz = (move_sz<<6)>>8;
				}
				if (abs(move_sz)<2){
					if (me->sidebar_req_x<me->sidebar_xpos){
						move_sz=-1;
					}
					else{
						move_sz=1;
					}
				}
				int target_sz = me->sidebar_xpos+move_sz;
				if (target_sz>=me->sidebar->w){
					if ((me->sb_touched)&&(me->sb_touched!=10)){
						target_sz=me->sidebar->w-1;
					}
					else{
						if (me->sb_touched==10){
							me->sb_touched=0;
						}
						target_sz=me->sidebar->w;
						me->sidebar_req_x=-1;
						me->sidebar_velocity=0;
					}
				}
				else if (target_sz<=0){
					if ((me->sb_touched)&&(me->sb_touched!=10)){
						target_sz=1;
					}
					else{
						if (me->sb_touched==10){
							me->sb_touched=0;
						}
						target_sz=0;
						me->sidebar_req_x=-1;
						me->sidebar_velocity=0;
					}
				}
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_sidebar_pos(win,target_sz);
				libaroma_mutex_lock(me->mutex);
			}
		}

		if ((me->pulldown)&&(me->pulldown_req_y!=-1)){
			/* show - hide pulldown */
			if (!me->pulldown->active){
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_pulldown_pos(win,1);
				libaroma_mutex_lock(me->mutex);
			}
			else if (me->pulldown->h<=0){
				/* invalid pulldown */
				me->pulldown_req_y=-1;
			}
			else if (me->pulldown_req_y!=me->pulldown_ypos){
				int move_sz = (me->pulldown_req_y-me->pulldown_ypos);
				if (me->pulldown_velocity!=0){
					me->pulldown_velocity=(me->pulldown_velocity*246)>>8;
					int minh=MAX(1,0.05*me->pulldown->h);
					if (me->pulldown_velocity<minh){
						me->pulldown_velocity=minh;
					}
					if (move_sz<0){
						move_sz = 0-me->pulldown_velocity;
					}
					else{
						move_sz = me->pulldown_velocity;
					}
				}
				else{
					move_sz = (move_sz<<6)>>8;
				}
				if (abs(move_sz)<2){
					if (me->pulldown_req_y<me->pulldown_ypos){
						move_sz=-1;
					}
					else{
						move_sz=1;
					}
				}
				int target_sz = me->pulldown_ypos+move_sz;
				if (target_sz>=me->pulldown->h){
					if ((me->pd_touched)&&(me->pd_touched!=10)){
						target_sz=me->pulldown->y-1;
					}
					else{
						if (me->pd_touched==10){
							me->pd_touched=0;
						}
						target_sz=me->pulldown->h;
						me->pulldown_req_y=-1;
						me->pulldown_velocity=0;
					}
				}
				else if (target_sz<=0){
					if ((me->pd_touched)&&(me->pd_touched!=10)){
						target_sz=1;
					}
					else{
						if (me->pd_touched==10){
							me->pd_touched=0;
						}
						target_sz=0;
						me->pulldown_req_y=-1;
						me->pulldown_velocity=0;
					}
				}
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_pulldown_pos(win,target_sz);
				libaroma_mutex_lock(me->mutex);
			}
		}

		libaroma_mutex_unlock(me->mutex);

		if (!me->pulldown_showed && me->sidebar_showed){
			if (me->sidebar){
				if (me->sidebar->ui_thread){
					if (me->sidebar->ui_thread(me->sidebar)){
						need_sync=1;
					}
				}
			}
		}

		if (me->pulldown_showed){
			if (me->pulldown){
				if (me->pulldown->ui_thread){
					if (me->pulldown->ui_thread(me->pulldown)){
						need_sync=1;
					}
				}
			}
		}
	}

	if (me->ori_ui_thread){
		if (me->ori_ui_thread(win)){
			need_sync = 1;
		}
	}

	libaroma_mutex_lock(me->mutex);
	if (me->redraw){
		need_sync=1;
		me->redraw=0;
	}
	libaroma_mutex_unlock(me->mutex);
	if (need_sync){
		if (win->active==1){
			_libaroma_window_layer_updatedc(win);
		}
	}
	return need_sync;
} /* End of _libaroma_window_layer_ui_thread */

/*
 * Function		: libaroma_window_layer_init
 * Return Value: byte
 * Descriptions: init window as layered window
 */
byte libaroma_window_layer_init(LIBAROMA_WINDOWP win){
	if (!win){
		ALOGW("window_root_register win is not valid pointer");
		return 0;
	}
	if (win->parent){
		ALOGW("window_root_register win is not root window");
		return 0;
	}
	if (win->client_data){
		if (win->handler!=&_libaroma_window_layer_handler){
			ALOGW("window_root_register window is not valid root window");
			return 0;
		}
		return 1;
	}
	_LIBAROMA_WINDOW_LAYERP me =
		(_LIBAROMA_WINDOW_LAYERP) calloc(sizeof(_LIBAROMA_WINDOW_LAYER),1);
	if (!me){
		ALOGW("window_root_register cannot alloc internal data");
		return 0;
	}
	libaroma_mutex_init(me->mutex);
	libaroma_mutex_lock(me->mutex);
	me->win = win;
	me->on_direct_canvas=1;
	me->ori_ui_thread=win->ui_thread;
	me->sidebar_req_x=-1;
	me->pulldown_req_y=-1;
	win->handler = &_libaroma_window_layer_handler;
	win->client_data = me;
	win->ui_thread=_libaroma_window_layer_ui_thread;
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of libaroma_window_layer_init */

/*
 * Function		: libaroma_window_layer_release
 * Return Value: byte
 * Descriptions: release layer window
 */
byte libaroma_window_layer_release(LIBAROMA_WINDOWP win){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return 0;
	}
	_libaroma_window_layer_postfree(win);
	libaroma_window_invalidate(win,1);
	return 1;
} /* End of libaroma_window_layer_release */

/**************************** PULLDOWN ************************************/

/* sidebar window handler */
byte _libaroma_window_pulldown_invalidate(LIBAROMA_WINDOWP win, byte sync);
byte _libaroma_window_pulldown_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h);
byte _libaroma_window_pulldown_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval
);

static LIBAROMA_WINDOW_HANDLER _libaroma_window_pulldown_handler={
	prefree:NULL,
	postfree:NULL,
	updatebg:NULL,
	invalidate:_libaroma_window_pulldown_invalidate,
	sync:_libaroma_window_pulldown_sync,
	message_hooker:_libaroma_window_pulldown_message_hooker,
	control_draw_flush:NULL,
	control_erasebg:NULL,
	control_isvisible:NULL,
	control_draw_begin:NULL
};

/*
 * Function		: _libaroma_window_pulldown_invalidate
 * Return Value: byte
 * Descriptions: invalidate
 */
byte _libaroma_window_pulldown_invalidate(LIBAROMA_WINDOWP win, byte sync){
	if (win->handler!=&_libaroma_window_pulldown_handler){
		return 0;
	}
	if ((win->dc)&&(win->bg)){
		libaroma_draw(win->dc,win->bg,0,0,0);
		/* draw childs */
		int i;
#ifdef LIBAROMA_CONFIG_OPENMP
	#pragma omp parallel for
#endif
		for (i=0;i<win->childn;i++){
			libaroma_control_draw(win->childs[i], 0);
		}
	}
	if (sync){
		return _libaroma_window_pulldown_sync(win,0,0,win->w,win->h);
	}
	return 1;
} /* End of _libaroma_window_pulldown_invalidate */

/*
 * Function		: _libaroma_window_pulldown_sync
 * Return Value: byte
 * Descriptions: sync pulldown
 */
byte _libaroma_window_pulldown_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h){
	if (win->handler!=&_libaroma_window_pulldown_handler){
		return 0;
	}
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	if (me->pulldown_showed){
		me->redraw=1;
	}
	return 1;
} /* End of _libaroma_window_pulldown_sync */

/*
 * Function		: libaroma_window_pulldown_show
 * Return Value: byte
 * Descriptions: show/hide pulldown
 */
byte libaroma_window_pulldown_show(LIBAROMA_WINDOWP win, byte show){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (show){
		if (!me->pulldown_showed){
			libaroma_mutex_unlock(me->mutex);
			_libaroma_window_layer_set_pulldown_pos(win->parent,1);
			libaroma_mutex_lock(me->mutex);
			me->pulldown_req_y=win->h;
			me->pd_touched=10;
			libaroma_mutex_unlock(me->mutex);
			return 1;
		}
	}
	else{
		if (me->pulldown_showed){
			if (me->pulldown_showed==2){
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_pulldown_pos(win->parent,win->h-1);
				libaroma_mutex_lock(me->mutex);
			}
			me->pulldown_req_y=0;
			me->pd_touched=10;
			libaroma_mutex_unlock(me->mutex);
			return 1;
		}
	}
	libaroma_mutex_unlock(me->mutex);
	return 0;
} /* End of libaroma_window_pulldown_show */

/*
 * Function		: _libaroma_window_pulldown_message_hooker
 * Return Value: byte
 * Descriptions: pulldown message hooker
 */
byte _libaroma_window_pulldown_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval
){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	byte return_state=0;
	libaroma_mutex_lock(me->mutex);
	switch (msg->msg){
		case LIBAROMA_MSG_KEY_BACK:
			{
				libaroma_mutex_unlock(me->mutex);
				libaroma_window_pulldown_show(win, 0);
				libaroma_mutex_lock(me->mutex);
				return_state=1;
				goto end_message;
			}
			break;
		case LIBAROMA_MSG_TOUCH:
			{
				if (win->parent->active!=1){
					return_state=0;
					goto end_message;
				}
				/* touch handler */
				if (msg->state==LIBAROMA_HID_EV_STATE_DOWN){
					memcpy(&me->pd_pretouched_msg,msg,sizeof(LIBAROMA_MSG));
					win->touched = NULL;
					me->pd_pretouched=NULL;

					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);
					if (y<win->h){
						int i;
						for (i=0;i<win->childn;i++){
							if (_libaroma_window_is_inside(win->childs[i],x,y)){
								me->pd_pretouched = win->childs[i];
								break;
							}
						}
						if (me->pd_pretouched!=NULL){
							if (me->pd_pretouched->handler->message){
								me->pd_client_touch_start=libaroma_tick();
							}
							else{
								me->pd_pretouched=NULL;
							}
						}
					}
					/*
					else{
						libaroma_mutex_unlock(me->mutex);
						libaroma_window_sidebar_show(win, 0);
						libaroma_mutex_lock(me->mutex);
					}*/
					me->pd_allow_scroll=2;
					me->touch_x=x;
					me->touch_y=y;
					libaroma_fling_down(&me->pd_fling, y);
				}
				else if (win->touched!=NULL){
					if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
						if (win->touched->handler->message){
							*retval=win->touched->handler->message(win->touched, msg);
						}
					}
					else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
						if (win->touched->handler->message){
							*retval=win->touched->handler->message(win->touched, msg);
						}
						win->touched=NULL;
					}
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);
					if (y>=win->h){
						libaroma_mutex_unlock(me->mutex);
						libaroma_window_pulldown_show(win, 0);
						libaroma_mutex_lock(me->mutex);
					}
					else if (me->pd_pretouched){
						if (me->pd_pretouched->handler->message){
							me->pd_pretouched->handler->message(
								me->pd_pretouched,&me->pd_pretouched_msg);
							me->pd_pretouched->handler->message(
								me->pd_pretouched,msg);
						}
						me->pd_pretouched=NULL;
						me->pd_client_touch_start=0;
						me->pd_allow_scroll=0;
						me->touch_x=x;
						me->touch_y=y;
						me->redraw=1;
					}
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);

					if (me->pd_allow_scroll==2){
						int move_sz = me->touch_x - x;
						int move_sz_y = me->touch_y - y;
						int scrdp=libaroma_dp(24);
						if ((abs(move_sz_y)>=scrdp)&&(abs(move_sz_y)>=abs(move_sz))){
							/* halt the scroll and send to control */
							if (me->pd_pretouched){
								if (me->pd_pretouched->handler->message){
									me->pd_client_touch_start=0;
									win->touched=me->pd_pretouched;
									me->pd_pretouched=NULL;
									win->touched->handler->message(
										win->touched,&me->pd_pretouched_msg);
									win->touched->handler->message(
										win->touched,msg);
								}
								else{
									me->pd_pretouched=NULL;
								}
								me->pd_client_touch_start=0;
								me->pd_allow_scroll=0;
								me->touch_x=x;
								me->touch_y=y;
								me->redraw=1;
							}
						}
						else if (abs(move_sz)>=scrdp){
							me->pd_allow_scroll=1;
							me->pd_client_touch_start=0;
							me->pd_pretouched=NULL;
							win->touched=NULL;
							me->pulldown_showed=1;
							me->pd_touched=2;
							me->touch_x=x;
							me->touch_y=y;
							libaroma_mutex_unlock(me->mutex);
							_libaroma_window_layer_set_pulldown_pos(win->parent,win->h-1);
							libaroma_mutex_lock(me->mutex);
						}
					}
				}
				return_state=1;
				goto end_message;
			}
			break;
		case LIBAROMA_MSG_WIN_ACTIVE:
			{
				if (!win->active){
					int i;
					win->active=1;
					for (i=0;i<win->childn;i++){
						if (win->childs[i]->handler->message){
							win->childs[i]->handler->message(win->childs[i], msg);
						}
					}
				}
			}
			break;
		case LIBAROMA_MSG_WIN_RESIZE:
			{
				int i;
				for (i=0;i<win->childn;i++){
					if (win->childs[i]->handler->message){
						win->childs[i]->handler->message(win->childs[i], msg);
					}
				}
			}
			break;
		case LIBAROMA_MSG_WIN_MEASURED:
			{
				/*int target_h = libaroma_window_usedp(2)?win->rh:libaroma_dp(win->rh);
				target_h = libaroma_window_measure_calculate(
					target_h,win->rh,win->parent->h,1,0
				);*/
				win->x=win->y=win->rx=win->ry=win->left=win->top=0;
				win->ax=win->x;
				win->ay=win->y;
				win->w	= win->parent->w;
				win->h	= win->parent->h;
				if (libaroma_window_usedp(2)){
					win->rw=win->width=libaroma_px(win->w);
					win->rh=win->height=libaroma_px(win->h);
				}
				else{
					win->rw=win->width= win->w;
					win->rh=win->height= win->h;
				}
				if (win->dc){
					if ((win->dc->w!=win->w)||(win->dc->h!=win->h)){
						libaroma_canvas_free(win->dc);
						if (win->bg){
							libaroma_canvas_free(win->bg);
						}
						win->dc=NULL;
						win->bg=NULL;
					}
				}
				if (!win->dc){
					win->dc = libaroma_canvas(win->w,win->h);
					win->bg = libaroma_canvas(win->w,win->h);
					libaroma_canvas_setcolor(win->dc,0xffff,0);
					libaroma_canvas_setcolor(win->bg,0xffff,0);
				}

				/* remeasured all childs */
				int i;
				for (i=0;i<win->childn;i++){
					libaroma_window_measure(win,win->childs[i]);
				}
			}
			break;
	}
end_message:
	libaroma_mutex_unlock(me->mutex);
	return return_state;
} /* End of _libaroma_window_pulldown_message_hooker */

/*
 * Function		: _libaroma_window_pulldown_ui_thread
 * Return Value: byte
 * Descriptions: window pulldown ui thread
 */
byte _libaroma_window_pulldown_ui_thread(LIBAROMA_WINDOWP win) {
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}

	int i;
	byte need_sync = 0;
	if (win->active==1){
		/* pretouched */
		libaroma_mutex_lock(me->mutex);
		if ((me->pd_client_touch_start!=0)&&
				(libaroma_tick()-me->pd_client_touch_start>180)){
			me->pd_client_touch_start=0;
			if (me->pd_pretouched!=NULL){
				win->touched=me->pd_pretouched;
				me->pd_pretouched=NULL;
				if (win->touched->handler->message){
					win->touched->handler->message(
						win->touched,&me->pd_pretouched_msg);
				}
			}
		}
		libaroma_mutex_unlock(me->mutex);


#ifdef LIBAROMA_CONFIG_OPENMP
	#pragma omp parallel for
#endif
		for (i=0;i<win->childn;i++){
			LIBAROMA_CONTROLP c=win->childs[i];
			if (c->handler->thread!=NULL){
				if (c->handler->thread(c)){
					if(libaroma_control_draw(c,0)){
						need_sync=1;
					}
				}
			}
		}
	}
	return need_sync;
} /* End of _libaroma_window_pulldown_ui_thread */

/*
 * Function		: libaroma_window_pulldown
 * Return Value: LIBAROMA_WINDOWP
 * Descriptions: new or get pulldown window
 */
LIBAROMA_WINDOWP libaroma_window_pulldown(LIBAROMA_WINDOWP win, int height, byte showtype){
	if (!libaroma_window_layer_init(win)){
		return NULL;
	}
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return NULL;
	}
	if (me->pulldown){
		return me->pulldown;
	}

	libaroma_mutex_lock(me->mutex);
	LIBAROMA_WINDOWP pulldown = (LIBAROMA_WINDOWP)
		calloc(sizeof(LIBAROMA_WINDOW),1);
	if (!pulldown){
		libaroma_mutex_unlock(me->mutex);
		ALOGW("window_pulldown alloc pulldown data failed");
		return NULL;
	}
	pulldown->rh = height;
	pulldown->handler=&_libaroma_window_pulldown_handler;
	pulldown->parent=win;
	pulldown->ui_thread=_libaroma_window_pulldown_ui_thread;
	me->pulldown=pulldown;
	if (!showtype) me->pulldown_drawtype=LIBAROMA_PULLDOWN_SLIDE;
	else me->pulldown_drawtype=showtype;
	me->redraw=1;
	libaroma_mutex_unlock(me->mutex);
	return pulldown;
} /* End of libaroma_window_pulldown */


/*
 * Function		: libaroma_window_pulldown_onslide
 * Return Value: byte
 * Descriptions: set pulldown slide position callback
 */
byte libaroma_window_pulldown_onslide(
	LIBAROMA_WINDOWP win, LIBAROMA_WINDOW_PULLDOWN_SLIDE_CB cb){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	me->pulldown_slide_cb = cb;
	ALOGI("Init pulldown slide callback");
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of libaroma_window_pulldown_onslide */


/******************************* SIDEBAR **************************************/

/* sidebar window handler */
byte _libaroma_window_sidebar_invalidate(LIBAROMA_WINDOWP win, byte sync);
byte _libaroma_window_sidebar_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h);
byte _libaroma_window_sidebar_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval
);
static LIBAROMA_WINDOW_HANDLER _libaroma_window_sidebar_handler={
	prefree:NULL,
	postfree:NULL,
	updatebg:NULL,
	invalidate:_libaroma_window_sidebar_invalidate,
	sync:_libaroma_window_sidebar_sync,
	message_hooker:_libaroma_window_sidebar_message_hooker,
	control_draw_flush:NULL,
	control_erasebg:NULL,
	control_isvisible:NULL,
	control_draw_begin:NULL
};

/*
 * Function		: _libaroma_window_sidebar_invalidate
 * Return Value: byte
 * Descriptions: invalidate
 */
byte _libaroma_window_sidebar_invalidate(LIBAROMA_WINDOWP win, byte sync){
	if (win->handler!=&_libaroma_window_sidebar_handler){
		return 0;
	}
	if ((win->dc)&&(win->bg)){
		libaroma_draw(win->dc,win->bg,0,0,0);
		/* draw childs */
		int i;
#ifdef LIBAROMA_CONFIG_OPENMP
	#pragma omp parallel for
#endif
		for (i=0;i<win->childn;i++){
			libaroma_control_draw(win->childs[i], 0);
		}
	}
	if (sync){
		return _libaroma_window_sidebar_sync(win,0,0,win->w,win->h);
	}
	return 1;
} /* End of _libaroma_window_sidebar_invalidate */

/*
 * Function		: _libaroma_window_sidebar_sync
 * Return Value: byte
 * Descriptions: sync sidebar
 */
byte _libaroma_window_sidebar_sync(LIBAROMA_WINDOWP win,
	int x,int y,int w,int h){
	if (win->handler!=&_libaroma_window_sidebar_handler){
		return 0;
	}
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	if (me->sidebar_showed){
		me->redraw=1;
	}
	return 1;
} /* End of _libaroma_window_sidebar_sync */

/*
 * Function		: libaroma_window_sidebar_show
 * Return Value: byte
 * Descriptions: show/hide sidebar
 */
byte libaroma_window_sidebar_show(LIBAROMA_WINDOWP win, byte show){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	if (show){
		if (!me->sidebar_showed){
			libaroma_mutex_unlock(me->mutex);
			_libaroma_window_layer_set_sidebar_pos(win->parent,1);
			libaroma_mutex_lock(me->mutex);
			me->sidebar_req_x=win->w;
			me->sb_touched=10;
			libaroma_mutex_unlock(me->mutex);
			return 1;
		}
	}
	else{
		if (me->sidebar_showed){
			if (me->sidebar_showed==2){
				libaroma_mutex_unlock(me->mutex);
				_libaroma_window_layer_set_sidebar_pos(win->parent,win->w-1);
				libaroma_mutex_lock(me->mutex);
			}
			me->sidebar_req_x=0;
			me->sb_touched=10;
			libaroma_mutex_unlock(me->mutex);
			return 1;
		}
	}
	libaroma_mutex_unlock(me->mutex);
	return 0;
} /* End of libaroma_window_sidebar_show */



/*
 * Function		: _libaroma_window_sidebar_message_hooker
 * Return Value: byte
 * Descriptions: sidebar message hooker
 */
byte _libaroma_window_sidebar_message_hooker(
		LIBAROMA_WINDOWP win, LIBAROMA_MSGP msg, dwordp retval
){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	byte return_state=0;
	libaroma_mutex_lock(me->mutex);
	switch (msg->msg){
		case LIBAROMA_MSG_KEY_BACK:
			{
				libaroma_mutex_unlock(me->mutex);
				libaroma_window_sidebar_show(win, 0);
				libaroma_mutex_lock(me->mutex);
				return_state=1;
				goto end_message;
			}
			break;
		case LIBAROMA_MSG_TOUCH:
			{
				if (win->parent->active!=1){
					return_state=0;
					goto end_message;
				}
				/* touch handler */
				if (msg->state==LIBAROMA_HID_EV_STATE_DOWN){
					memcpy(&me->sb_pretouched_msg,msg,sizeof(LIBAROMA_MSG));
					win->touched = NULL;
					me->sb_pretouched=NULL;

					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);
					if (x<win->w){
						int i;
						for (i=0;i<win->childn;i++){
							if (_libaroma_window_is_inside(win->childs[i],x,y)){
								me->sb_pretouched = win->childs[i];
								break;
							}
						}
						if (me->sb_pretouched!=NULL){
							if (me->sb_pretouched->handler->message){
								me->sb_client_touch_start=libaroma_tick();
							}
							else{
								me->sb_pretouched=NULL;
							}
						}
					}
					/*
					else{
						libaroma_mutex_unlock(me->mutex);
						libaroma_window_sidebar_show(win, 0);
						libaroma_mutex_lock(me->mutex);
					}*/
					me->sb_allow_scroll=2;
					me->touch_x=x;
					me->touch_y=y;
					libaroma_fling_down(&me->sb_fling, x);
				}
				else if (win->touched!=NULL){
					if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
						if (win->touched->handler->message){
							*retval=win->touched->handler->message(win->touched, msg);
						}
					}
					else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
						if (win->touched->handler->message){
							*retval=win->touched->handler->message(win->touched, msg);
						}
						win->touched=NULL;
					}
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);
					if (x>=win->w){
						libaroma_mutex_unlock(me->mutex);
						libaroma_window_sidebar_show(win, 0);
						libaroma_mutex_lock(me->mutex);
					}
					else if (me->sb_pretouched){
						if (me->sb_pretouched->handler->message){
							me->sb_pretouched->handler->message(
								me->sb_pretouched,&me->sb_pretouched_msg);
							me->sb_pretouched->handler->message(
								me->sb_pretouched,msg);
						}
						me->sb_pretouched=NULL;
						me->sb_client_touch_start=0;
						me->sb_allow_scroll=0;
						me->touch_x=x;
						me->touch_y=y;
						me->redraw=1;
					}
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
					int x = msg->x;
					int y = msg->y;
					libaroma_window_calculate_pos(win->parent,NULL,&x,&y);

					if (me->sb_allow_scroll==2){
						int move_sz = me->touch_x - x;
						int move_sz_y = me->touch_y - y;
						int scrdp=libaroma_dp(24);
						if ((abs(move_sz_y)>=scrdp)&&(abs(move_sz_y)>=abs(move_sz))){
							/* halt the scroll and send to control */
							if (me->sb_pretouched){
								if (me->sb_pretouched->handler->message){
									me->sb_client_touch_start=0;
									win->touched=me->sb_pretouched;
									me->sb_pretouched=NULL;
									win->touched->handler->message(
										win->touched,&me->sb_pretouched_msg);
									win->touched->handler->message(
										win->touched,msg);
								}
								else{
									me->sb_pretouched=NULL;
								}
								me->sb_client_touch_start=0;
								me->sb_allow_scroll=0;
								me->touch_x=x;
								me->touch_y=y;
								me->redraw=1;
							}
						}
						else if (abs(move_sz)>=scrdp){
							me->sb_allow_scroll=1;
							me->sb_client_touch_start=0;
							me->sb_pretouched=NULL;
							win->touched=NULL;
							me->sidebar_showed=1;
							me->sb_touched=2;
							me->touch_x=x;
							me->touch_y=y;
							libaroma_mutex_unlock(me->mutex);
							_libaroma_window_layer_set_sidebar_pos(win->parent,win->w-1);
							libaroma_mutex_lock(me->mutex);
						}
					}
				}
				return_state=1;
				goto end_message;
			}
			break;
		case LIBAROMA_MSG_WIN_ACTIVE:
			{
				if (!win->active){
					int i;
					win->active=1;
					for (i=0;i<win->childn;i++){
						if (win->childs[i]->handler->message){
							win->childs[i]->handler->message(win->childs[i], msg);
						}
					}
				}
			}
			break;
		case LIBAROMA_MSG_WIN_RESIZE:
			{
				int i;
				for (i=0;i<win->childn;i++){
					if (win->childs[i]->handler->message){
						win->childs[i]->handler->message(win->childs[i], msg);
					}
				}
			}
			break;
		case LIBAROMA_MSG_WIN_MEASURED:
			{
				int target_w	= libaroma_window_usedp(2)?win->rw:libaroma_dp(win->rw);
				target_w = libaroma_window_measure_calculate(
					target_w,win->rw,win->parent->w,1,0
				);
				int max_target_w = win->parent->w-libaroma_dp(56);
				if (target_w>max_target_w){
					target_w=max_target_w;
				}
				win->x=win->y=win->rx=win->ry=win->left=win->top=0;
				win->ax=win->x;
				win->ay=win->y;
				win->w	= target_w;
				win->h	= win->parent->h;
				if (libaroma_window_usedp(2)){
					win->rw=win->width=libaroma_px(win->w);
					win->rh=win->height=libaroma_px(win->h);
				}
				else{
					win->rw=win->width= win->w;
					win->rh=win->height= win->h;
				}
				if (win->dc){
					if ((win->dc->w!=win->w)||(win->dc->h!=win->h)){
						libaroma_canvas_free(win->dc);
						if (win->bg){
							libaroma_canvas_free(win->bg);
						}
						win->dc=NULL;
						win->bg=NULL;
					}
				}
				if (!win->dc){
					win->dc = libaroma_canvas(win->w,win->h);
					win->bg = libaroma_canvas(win->w,win->h);
					libaroma_canvas_setcolor(win->dc,0xffff,0);
					libaroma_canvas_setcolor(win->bg,0xffff,0);
				}

				/* remeasured all childs */
				int i;
				for (i=0;i<win->childn;i++){
					libaroma_window_measure(win,win->childs[i]);
				}
			}
			break;
	}
end_message:
	libaroma_mutex_unlock(me->mutex);
	return return_state;
} /* End of _libaroma_window_sidebar_message_hooker */

/*
 * Function		: _libaroma_window_sidebar_ui_thread
 * Return Value: byte
 * Descriptions: window sidebar ui thread
 */
byte _libaroma_window_sidebar_ui_thread(LIBAROMA_WINDOWP win) {
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}

	int i;
	byte need_sync = 0;
	if (win->active==1){
		/* pretouched */
		libaroma_mutex_lock(me->mutex);
		if ((me->sb_client_touch_start!=0)&&
				(libaroma_tick()-me->sb_client_touch_start>180)){
			me->sb_client_touch_start=0;
			if (me->sb_pretouched!=NULL){
				win->touched=me->sb_pretouched;
				me->sb_pretouched=NULL;
				if (win->touched->handler->message){
					win->touched->handler->message(
						win->touched,&me->sb_pretouched_msg);
				}
			}
		}
		libaroma_mutex_unlock(me->mutex);


#ifdef LIBAROMA_CONFIG_OPENMP
	#pragma omp parallel for
#endif
		for (i=0;i<win->childn;i++){
			LIBAROMA_CONTROLP c=win->childs[i];
			if (c->handler->thread!=NULL){
				if (c->handler->thread(c)){
					if(libaroma_control_draw(c,0)){
						need_sync=1;
					}
				}
			}
		}
	}
	return need_sync;
} /* End of _libaroma_window_sidebar_ui_thread */

/*
 * Function		: libaroma_window_sidebar
 * Return Value: LIBAROMA_WINDOWP
 * Descriptions: new or get sidebar window
 */
LIBAROMA_WINDOWP libaroma_window_sidebar(LIBAROMA_WINDOWP win, int width){
	if (!libaroma_window_layer_init(win)){
		return NULL;
	}
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win);
	if (!me){
		return NULL;
	}
	if (me->sidebar){
		return me->sidebar;
	}

	libaroma_mutex_lock(me->mutex);
	LIBAROMA_WINDOWP sidebar = (LIBAROMA_WINDOWP)
		calloc(sizeof(LIBAROMA_WINDOW),1);
	if (!sidebar){
		libaroma_mutex_unlock(me->mutex);
		ALOGW("window_sidebar alloc sidebar data failed");
		return NULL;
	}
	sidebar->rw = width;
	sidebar->handler=&_libaroma_window_sidebar_handler;
	sidebar->parent=win;
	sidebar->ui_thread=_libaroma_window_sidebar_ui_thread;
	me->sidebar=sidebar;
	me->redraw=1;
	libaroma_mutex_unlock(me->mutex);
	return sidebar;
} /* End of libaroma_window_sidebar */


/*
 * Function		: libaroma_window_sidebar_onslide
 * Return Value: byte
 * Descriptions: set sidebar slide position callback
 */
byte libaroma_window_sidebar_onslide(
	LIBAROMA_WINDOWP win, LIBAROMA_WINDOW_SIDEBAR_SLIDE_CB cb){
	_LIBAROMA_WINDOW_LAYERP me = _libaroma_window_layer_check(win->parent);
	if (!me){
		return 0;
	}
	libaroma_mutex_lock(me->mutex);
	me->slide_cb = cb;
	ALOGI("Init sidebar slide callback");
	libaroma_mutex_unlock(me->mutex);
	return 1;
} /* End of libaroma_window_sidebar_onslide */

#ifdef __cplusplus
}
#endif

#endif /* __libaroma_window_layer_c__ */

