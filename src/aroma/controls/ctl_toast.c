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
 * Filename		: ctl_toast.c
 * Description : toast control
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 06/02/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_ctl_toast_c__
#define __libaroma_ctl_toast_c__
#include <aroma_internal.h>
#include "../ui/ui_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
#define _LIBAROMA_CTL_TOAST_ANI_TIMING 500

/* HANDLER */
dword _libaroma_ctl_toast_msg(LIBAROMA_CONTROLP, LIBAROMA_MSGP);
void _libaroma_ctl_toast_draw (LIBAROMA_CONTROLP, LIBAROMA_CANVASP);
void _libaroma_ctl_toast_destroy(LIBAROMA_CONTROLP);
byte _libaroma_ctl_toast_thread(LIBAROMA_CONTROLP);
static LIBAROMA_CONTROL_HANDLER _libaroma_ctl_toast_handler={
	message:_libaroma_ctl_toast_msg,
	draw:_libaroma_ctl_toast_draw,
	focus:NULL,
	destroy:_libaroma_ctl_toast_destroy,
	thread:_libaroma_ctl_toast_thread
};

/*
 * Structure	 : __LIBAROMA_CTL_TOAST
 * Typedef		 : _LIBAROMA_CTL_TOAST, * _LIBAROMA_CTL_TOASTP
 * Descriptions: toast control internal structure
 */
typedef struct __LIBAROMA_CTL_TOAST _LIBAROMA_CTL_TOAST;
typedef struct __LIBAROMA_CTL_TOAST * _LIBAROMA_CTL_TOASTP;
struct __LIBAROMA_CTL_TOAST{
	char * text;
	byte style;
	word color;
	byte gravity;
	LIBAROMA_CANVASP icon;
	byte animating;
	byte visible;
	int timeout;

	LIBAROMA_MUTEX mutex;
	LIBAROMA_CANVASP rest_canvas;
};

/*
 * Function		: _libaroma_ctl_toast_internal_draw
 * Return Value: void
 * Descriptions: internal toast draw
 */
void _libaroma_ctl_toast_internal_draw(LIBAROMA_CONTROLP ctl){
	_LIBAROMA_CTL_TOASTP me = (_LIBAROMA_CTL_TOASTP) ctl->internal;
	if (ctl->window==NULL){
		return;
	}

	libaroma_mutex_lock(me->mutex);
	LIBAROMA_CANVASP rest_canvas=NULL;

	//byte keepcolor_disabled = (me->style&LIBAROMA_CTL_TOAST_KEEP_COLOR)?1:0;
	rest_canvas = libaroma_canvas/*_alpha*/(ctl->w,ctl->h);
	//libaroma_canvas_fillalpha(rest_canvas, 0, 0, ctl->w, ctl->h, 0xFF);
	byte has_icon=(me->icon!=NULL)?1:0;
	byte icon_isleft=1;//(me->style&LIBAROMA_CTL_TOAST_ICON_LEFT)?1:0;
	if ((me->style&LIBAROMA_CTL_TOAST_ICON_RIGHT)
		||(me->style&LIBAROMA_CTL_TOAST_ICON_CENTER))
		icon_isleft=0;
	int iconsz=libaroma_dp(16);
	int ix = libaroma_dp(24);
	int iy = libaroma_dp(14);
	int iw = ctl->w-ix*2;
	int ih = ctl->h-iy*2;
	byte isdark=libaroma_color_isdark(me->color);
	word text_color = isdark?RGB(FFFFFF):RGB(0);

	libaroma_draw_rect(rest_canvas, 0, 0, ctl->w, ctl->h, me->color, 0xFF);

	int textw=ctl->w-libaroma_dp(48);//iw-(has_icon?iconsz:0);

	/* draw text */
	LIBAROMA_TEXT textp = libaroma_text(
		(me->text==NULL)?"":me->text,
		text_color,
		textw,
		LIBAROMA_TEXT_CENTER,
		0
	);
	// libaroma_mutex_unlock(me->mutex);
	int y = (ctl->h>>1) - ((libaroma_text_height(textp)>>1)+libaroma_dp(2));
	int textx=(ctl->w-textw)/2;//libaroma_dp(16)+(icon_isleft?iconsz:0)+ix;
	libaroma_text_draw_color(rest_canvas,textp,textx/*libaroma_dp(has_icon?(icon_isleft?20:8):8)+ix*/,y,
		text_color
	);

	libaroma_text_free(textp);
/*
	if (has_icon) { // draw icon
		int iconx;
		//byte centered=0;
		int allW=libaroma_dp(iconsz+2)+textw;
		int startX=(ctl->w/2)-(allW/2);
		if (me->text==NULL || (me->icon_flags&LIBAROMA_CTL_TOAST_ICON_CENTER)) {
			iconx=(ctl->w/2)-(iconsz/2); //if no text (or forced), icon is centered
			//centered=1;
		}
		else {
			if (icon_isleft) iconx=startX;
			else iconx=startX+textw+libaroma_dp(2);
		}
		int icony=(ctl->h/2)-(iconsz/2);
		ALOGI("Going to draw %s icon with %ddp at %d, %d. \n"
				"Button info: X=%d, W=%d, left=%d and circle=%d",
						((centered)?"centered":(icon_isleft?"left":"right")), iconsz, iconx, icony, ctl->x, ctl->w, icon_isleft, is_circle);
		libaroma_draw_scale_smooth(rest_canvas, me->icon, iconx, icony, iconsz, iconsz, 0, 0, me->icon->w, me->icon->h);
		if (!is_disabled || (is_disabled && keepcolor_disabled)){
			libaroma_draw_scale_smooth(push_canvas, me->icon, iconx, icony, iconsz, iconsz, 0, 0, me->icon->w, me->icon->h);
		}
	}*/

	/* save it */
	// libaroma_mutex_lock(me->mutex);
	LIBAROMA_CANVASP rc=me->rest_canvas;
	me->rest_canvas=rest_canvas;
	if (rc!=NULL){
		libaroma_canvas_free(rc);
	}
	//me->forcedraw=1;
	libaroma_mutex_unlock(me->mutex);
} /* End of _libaroma_ctl_toast_internal_draw */

static void * _libaroma_ctl_toast_req_internal_draw_thread(void * ctl){
	_libaroma_ctl_toast_internal_draw((LIBAROMA_CONTROLP) ctl);
	return NULL;
}
void _libaroma_ctl_toast_req_internal_draw(LIBAROMA_CONTROLP ctl){
	LIBAROMA_THREAD pp;
	libaroma_thread_create(
		&pp,_libaroma_ctl_toast_req_internal_draw_thread,(void *) ctl);
	libaroma_thread_detach(pp);
}

/*
 * Function		: _libaroma_ctl_toast_hide_timer
 * Return Value: void
 * Descriptions: toast hiding timer
 */
static void * _libaroma_ctl_toast_hide_timer(void * control){
	LIBAROMA_CONTROLP ctl=(LIBAROMA_CONTROLP) control;
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP,
	);
	libaroma_sleep(me->timeout);
	me->visible=0;
	return NULL;
}

void _libaroma_ctl_toast_start_timer(LIBAROMA_CONTROLP ctl){
	LIBAROMA_THREAD pp;
	libaroma_thread_create(
		&pp,_libaroma_ctl_toast_hide_timer,(void *) ctl);
	libaroma_thread_detach(pp);
}

/*
 * Function		: _libaroma_ctl_toast_draw
 * Return Value: void
 * Descriptions: draw callback
 */
void _libaroma_ctl_toast_draw(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CANVASP c){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP,
	);
	ALOGI("Drawing toast into control canvas");
	libaroma_mutex_lock(me->mutex);
	if (me->rest_canvas==NULL){
		_libaroma_ctl_toast_internal_draw(ctl);
	}
	libaroma_draw(c, me->rest_canvas, 0, 0, 0);
	libaroma_mutex_unlock(me->mutex);
} /* End of _libaroma_ctl_toast_draw */

/*
 * Function		: _libaroma_ctl_toast_thread
 * Return Value: byte
 * Descriptions: control thread callback
 */
byte _libaroma_ctl_toast_thread(LIBAROMA_CONTROLP ctl) {
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	//libaroma_mutex_lock(me->mutex);

	/*
	if (me->animating){
		libaroma_mutex_unlock(me->mutex);
		return 1;
	}*/
	//libaroma_mutex_unlock(me->mutex);
	ALOGI("Toast thread returning %d", me->visible);
	return me->visible;
} /* End of _libaroma_ctl_toast_thread */

/*
 * Function		: _libaroma_ctl_toast_destroy
 * Return Value: void
 * Descriptions: destroy callback
 */
void _libaroma_ctl_toast_destroy(
		LIBAROMA_CONTROLP ctl){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP,
	);
	libaroma_mutex_lock(me->mutex);
	if (me->rest_canvas!=NULL){
		libaroma_canvas_free(me->rest_canvas);
		me->rest_canvas=NULL;
	}
	if (me->text!=NULL){
		free(me->text);
	}
	libaroma_mutex_unlock(me->mutex);
	libaroma_mutex_free(me->mutex);
	free(me);
} /* End of _libaroma_ctl_toast_destroy */

/*
 * Function		: _libaroma_ctl_toast_msg
 * Return Value: byte
 * Descriptions: message callback
 */
dword _libaroma_ctl_toast_msg(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_MSGP msg){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);

	switch(msg->msg){
		case LIBAROMA_MSG_WIN_ACTIVE:
		case LIBAROMA_MSG_WIN_INACTIVE:
		case LIBAROMA_MSG_WIN_RESIZE:
			{
				libaroma_mutex_lock(me->mutex);
				ALOGI("Hiding toast if visible!");
				//me->forcedraw=1;
				if (me->visible)
					me->visible=0;
				libaroma_mutex_unlock(me->mutex);
			}
			break;/*
		case LIBAROMA_MSG_TOUCH:
			{
				if (me->style&LIBAROMA_CTL_TOAST_DISABLED){
					// no thing for disabled toast
					return 0;
				}
				int x = msg->x;
				int y = msg->y;
				libaroma_window_calculate_pos(NULL,ctl,&x,&y);

				// touch handler
				if (msg->state==LIBAROMA_HID_EV_STATE_DOWN){
					libaroma_ripple_down(&me->ripple, x, y);
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_UP){
					byte res = libaroma_ripple_up(&me->ripple,0);
					if ((res&LIBAROMA_RIPPLE_TOUCHED)&&
							(!(res&LIBAROMA_RIPPLE_HOLDED))){
						libaroma_window_post_command(
							LIBAROMA_CMD_SET(LIBAROMA_CMD_CLICK, 0, ctl->id)
						);
					}
				}
				else if (msg->state==LIBAROMA_HID_EV_STATE_MOVE){
					if (!((x>=0)&&(y>=0)&&(x<ctl->w)&&(y<ctl->h))) {
						libaroma_ripple_cancel(&me->ripple);
					}
					else{
						libaroma_ripple_move(&me->ripple, x, y);
					}
				}
			}
			break;*/
	}
	return 0;
} /* End of _libaroma_ctl_toast_msg */

/*
 * Function		: libaroma_ctl_toast
 * Return Value: LIBAROMA_CONTROLP
 * Descriptions: create toast control
 */
LIBAROMA_CONTROLP libaroma_ctl_toast(
		LIBAROMA_WINDOWP win,
		word id,
		const char * text,
		int timeout,
		byte gravity,
		byte toast_style,
		word toast_color
){
	/* init internal data */
	_LIBAROMA_CTL_TOASTP me = (_LIBAROMA_CTL_TOASTP)
			calloc(sizeof(_LIBAROMA_CTL_TOAST),1);
	if (!me){
		ALOGW("libaroma_ctl_toast alloc toast memory failed");
		return NULL;
	}
	/* set internal data */
	libaroma_mutex_init(me->mutex);
	me->text = strdup(text);
	me->style = toast_style;
	me->color = toast_color;
	me->gravity=gravity;
	me->visible=0;
	//me->
	int max_textw=libaroma_px(win->w)-libaroma_dp(48); //48dp for left-right side
	LIBAROMA_TEXT parsed_text=libaroma_text(text, RGB(FFFFFF), win->w-48, LIBAROMA_TEXT_CENTER, 0);
	int textw=libaroma_text_width(parsed_text);
	int texth=libaroma_text_height(parsed_text);

	int x, y, w, h;
	w=textw+libaroma_dp(48);
	h=texth+libaroma_dp(28);
	x=(win->w/2)-(w/2);
	//y=win->h-y;
	switch (gravity){
		case LIBAROMA_CTL_TOAST_GRAVITY_TOP:{
				y=libaroma_dp(48);
			}
			break;
		case LIBAROMA_CTL_TOAST_GRAVITY_CENTER:{
				y=(win->h/2)-libaroma_dp(24);
			}
			break;
		default:
		case LIBAROMA_CTL_TOAST_GRAVITY_BOTTOM:{
				y=win->h-h-libaroma_dp(48);
			}
			break;
	}

	/* init control */
	LIBAROMA_CONTROLP ctl =
		libaroma_control_new(
			id,
			x, y, w, h,
			libaroma_dp(48),libaroma_dp(48), /* min size */
			(voidp) me,
			&_libaroma_ctl_toast_handler,
			win
		);
	if (!ctl){
		libaroma_mutex_free(me->mutex);
		if (me->text!=NULL){
			free(me->text);
		}
		free(me);
		return NULL;
	}
	return ctl;
} /* End of libaroma_ctl_toast */

/*
 * Function		: libaroma_ctl_toast_icon
 * Return Value: LIBAROMA_CONTROLP
 * Descriptions: create toast control - with icon
 */
LIBAROMA_CONTROLP libaroma_ctl_toast_icon(
		LIBAROMA_WINDOWP win,
		word id,
		const char * text,
		int timeout,
		byte gravity,
		LIBAROMA_CANVASP icon,
		byte icon_flags,
		byte toast_style,
		word toast_color
){
	LIBAROMA_CONTROLP toast=libaroma_ctl_toast(win, id, text, timeout, gravity, toast_style, toast_color);
	libaroma_ctl_toast_seticon(toast, icon, icon_flags);
	return toast;
} /* End of libaroma_ctl_toast_icon */

/*
 * Function		: libaroma_ctl_toast_seticon
 * Return Value: byte
 * Descriptions: set toast icon
 */
byte libaroma_ctl_toast_seticon(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CANVASP icon,
		byte icon_flags
){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->icon = icon;

	me->style |= icon_flags;

	libaroma_mutex_unlock(me->mutex);
	_libaroma_ctl_toast_req_internal_draw(ctl);
	return 1;
} /* End of libaroma_ctl_toast_style */

/*
 * Function		: libaroma_ctl_toast_style
 * Return Value: byte
 * Descriptions: set toast style
 */
byte libaroma_ctl_toast_style(
		LIBAROMA_CONTROLP ctl,
		byte toast_style,
		word toast_color
){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->style = toast_style;
	me->color = toast_color;
	libaroma_mutex_unlock(me->mutex);
	_libaroma_ctl_toast_req_internal_draw(ctl);
	return 1;
} /* End of libaroma_ctl_toast_style */

/*
 * Function		: libaroma_ctl_toast_text
 * Return Value: byte
 * Descriptions: set toast text
 */
byte libaroma_ctl_toast_text(
		LIBAROMA_CONTROLP ctl,
		const char * text
){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	if (me->text!=NULL){
		free(me->text);
	}
	me->text = strdup(text);
	libaroma_mutex_unlock(me->mutex);
	_libaroma_ctl_toast_req_internal_draw(ctl);
	return 1;
} /* End of libaroma_ctl_toast_text */

/*
 * Function		: libaroma_ctl_toast_gravity
 * Return Value: byte
 * Descriptions: set toast gravity
 */
byte libaroma_ctl_toast_gravity(
		LIBAROMA_CONTROLP ctl,
		byte gravity
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->gravity=gravity;
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

/*
 * Function		: libaroma_ctl_toast_gravity
 * Return Value: byte
 * Descriptions: set toast timeout
 */
byte libaroma_ctl_toast_timeout(
		LIBAROMA_CONTROLP ctl,
		int timeout
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->timeout=timeout;
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

/*
 * Function		: libaroma_ctl_is_toast
 * Return Value: byte
 * Descriptions: check if control is toast
 */
byte libaroma_ctl_is_toast(LIBAROMA_CONTROLP ctl){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	return 1; // if _LIBAROMA_CTL_CHECK passed, we can return true
} /* End of libaroma_ctl_is_toast */

/*
 * Function		: libaroma_ctl_toast_hide
 * Return Value: byte
 * Descriptions: hide toast
 */
byte libaroma_ctl_toast_hide(
		LIBAROMA_CONTROLP ctl
){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->visible=0;
	libaroma_mutex_unlock(me->mutex);
	return 1; // if _LIBAROMA_CTL_CHECK passed, we can return true
}

/*
 * Function		: libaroma_ctl_toast_show
 * Return Value: byte
 * Descriptions: show toast
 */
byte libaroma_ctl_toast_show(
		LIBAROMA_CONTROLP ctl
){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_toast_handler, _LIBAROMA_CTL_TOASTP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->visible=1;
	libaroma_mutex_unlock(me->mutex);
	return 1; // if _LIBAROMA_CTL_CHECK passed, we can return true
}

#ifdef __cplusplus
}
#endif

#endif /* __libaroma_ctl_toast_c__ */
