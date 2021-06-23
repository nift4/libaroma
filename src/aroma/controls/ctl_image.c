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
 * Filename		: ctl_image.c
 * Description : image control
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 28/01/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_ctl_image_c__
#define __libaroma_ctl_image_c__
#include <aroma_internal.h>
#include "../ui/ui_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
/* HANDLER */
dword _libaroma_ctl_image_msg(LIBAROMA_CONTROLP, LIBAROMA_MSGP);
void _libaroma_ctl_image_draw (LIBAROMA_CONTROLP, LIBAROMA_CANVASP);
void _libaroma_ctl_image_destroy(LIBAROMA_CONTROLP);
byte _libaroma_ctl_image_thread(LIBAROMA_CONTROLP ctl);
static LIBAROMA_CONTROL_HANDLER _libaroma_ctl_image_handler={
	message:_libaroma_ctl_image_msg,
	draw:_libaroma_ctl_image_draw,
	focus:NULL,
	destroy:_libaroma_ctl_image_destroy,
	thread:_libaroma_ctl_image_thread
};

/*
 * Structure	 : __LIBAROMA_CTL_IMAGE
 * Typedef		 : _LIBAROMA_CTL_IMAGE, * _LIBAROMA_CTL_IMAGEP
 * Descriptions: internal label data
 */
typedef struct __LIBAROMA_CTL_IMAGE _LIBAROMA_CTL_IMAGE;
typedef struct __LIBAROMA_CTL_IMAGE * _LIBAROMA_CTL_IMAGEP;
struct __LIBAROMA_CTL_IMAGE{
	LIBAROMA_CANVASP img;
	LIBAROMA_MUTEX mutex;
	byte update;
	byte hidden;
	byte flags;
};

byte _libaroma_ctl_image_thread(LIBAROMA_CONTROLP ctl) {
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	if (me->update){
		libaroma_mutex_lock(me->mutex);
		me->update=0;
		libaroma_mutex_unlock(me->mutex);
		return 1;
	}
	return 0;
}

void _libaroma_ctl_image_destroy(LIBAROMA_CONTROLP ctl){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP,
	);
	libaroma_mutex_lock(me->mutex);
	byte isshared=(me->flags&LIBAROMA_CTL_IMAGE_SHARED)?1:0;
	if (me->img && !isshared){
		libaroma_canvas_free(me->img);
	}
	libaroma_mutex_unlock(me->mutex);
	libaroma_mutex_free(me->mutex);
	free(me);
}

void _libaroma_ctl_image_draw(
		LIBAROMA_CONTROLP ctl, LIBAROMA_CANVASP c){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP,
	);
	libaroma_control_erasebg(ctl,c);
	if (!me->hidden){
		libaroma_mutex_lock(me->mutex);
		if (me->img){
			if (me->flags&LIBAROMA_CTL_IMAGE_FILL){
				/* stretch image to fill control canvas */
				libaroma_draw_scale_smooth(c,me->img,
					0,0,c->w,c->h,
					0,0,me->img->w,me->img->h
				);
			}
			else {
				int x=0, y=0;
				if (me->flags&LIBAROMA_CTL_IMAGE_CENTER){
					/* center x/y */
					x=(c->w - me->img->w)/2;
					y=(c->h - me->img->h)/2;
				}
				/* draw directly */
				libaroma_draw(c, me->img, x, y, 1);
			}
		}
		libaroma_mutex_unlock(me->mutex);
	}
}

dword _libaroma_ctl_image_msg(LIBAROMA_CONTROLP ctl, LIBAROMA_MSGP msg){
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);

	switch(msg->msg){
		case LIBAROMA_MSG_WIN_ACTIVE:
		case LIBAROMA_MSG_WIN_INACTIVE:
		case LIBAROMA_MSG_WIN_RESIZE:
			{
				libaroma_mutex_lock(me->mutex);
				me->update=1;
				libaroma_mutex_unlock(me->mutex);
			}
			break;
	}
	return 0;
}
byte libaroma_ctl_image_update(
	LIBAROMA_CONTROLP ctl
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->update=1;
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

byte libaroma_ctl_image_hidden(
	LIBAROMA_CONTROLP ctl,byte hidden){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	libaroma_mutex_lock(me->mutex);
	me->hidden=hidden;
	me->update=1;
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

byte libaroma_ctl_image_isvisible(
	LIBAROMA_CONTROLP ctl
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	return (me->hidden)?0:1;
}

byte libaroma_ctl_image_getflags(
	LIBAROMA_CONTROLP ctl
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	return me->flags;
}

byte libaroma_ctl_image_setflags(
	LIBAROMA_CONTROLP ctl,byte flags
){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	libaroma_mutex_lock(me->mutex);
	if (flags&LIBAROMA_CTL_IMAGE_HIDDEN){
		ALOGI("ctl_image set hidden");
		me->hidden=1;
	}
	byte isshared=(flags&LIBAROMA_CTL_IMAGE_SHARED)?1:0;
	if (isshared && me->img){
		/* duplicate old canvas and use copy */
		LIBAROMA_CANVASP image_dup = libaroma_canvas_dup(me->img);
		if (!image_dup){
			ALOGW("libaroma_ctl_image_setflags failed to duplicate shared image");
			return 0;
		}
		me->img=image_dup;
	}
	me->flags=flags;
	me->update=1;
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

byte libaroma_ctl_image_set_image(
	LIBAROMA_CONTROLP ctl,char * src,byte update){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	libaroma_mutex_lock(me->mutex);
	byte isshared=(me->flags&LIBAROMA_CTL_IMAGE_SHARED)?1:0;
	if (!isshared && me->img){
		libaroma_canvas_free(me->img);
	}
	me->img=libaroma_image_file(src,1);
	if (update){
		me->update=1;
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

byte libaroma_ctl_image_set_canvas(
	LIBAROMA_CONTROLP ctl,LIBAROMA_CANVASP src,byte update){
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_image_handler, _LIBAROMA_CTL_IMAGEP, 0
	);
	libaroma_mutex_lock(me->mutex);
	byte isshared=(me->flags&LIBAROMA_CTL_IMAGE_SHARED)?1:0;
	if (me->img && !isshared){
		libaroma_canvas_free(me->img);
	}
	me->img = libaroma_canvas_dup(src);
	if (update){
		me->update=1;
	}
	libaroma_mutex_unlock(me->mutex);
	return 1;
}

LIBAROMA_CONTROLP libaroma_ctl_image_canvas_ex(
	LIBAROMA_WINDOWP win, word id, LIBAROMA_CANVASP src,
	int x, int y, int w, int h, byte flags
){
	_LIBAROMA_CTL_IMAGEP me = (_LIBAROMA_CTL_IMAGEP)
		calloc(sizeof(_LIBAROMA_CTL_IMAGE),1);
	if (!me){
		ALOGW("libaroma_ctl_image alloc label memory failed");
		return NULL;
	}
	byte isshared=(flags&LIBAROMA_CTL_IMAGE_SHARED)?1:0;
	ALOGI("Initializing image control with %s image", isshared?"shared":"freeable");
	me->flags=flags;
	me->img = isshared?src:libaroma_canvas_dup(src);
	me->update=1;
	libaroma_mutex_init(me->mutex);
	LIBAROMA_CONTROLP ctl =
		libaroma_control_new(
			id,
			x, y, w, h,
			1,1,
			me,
			&_libaroma_ctl_image_handler,
			win
		);
	if (!ctl){
		if (me->img && !isshared){
			libaroma_canvas_free(me->img);
		}
		libaroma_mutex_free(me->mutex);
		free(me);
	}
	return ctl;
}


LIBAROMA_CONTROLP libaroma_ctl_image_ex(
	LIBAROMA_WINDOWP win, word id, char * src,
	int x, int y, int w, int h, byte flags
){
	LIBAROMA_CANVASP cv=NULL;
	if (src){
		cv=libaroma_image_file(src,1);
	}
	LIBAROMA_CONTROLP out = libaroma_ctl_image_canvas_ex(
		win,id,cv,x,y,w,h, flags|LIBAROMA_CTL_IMAGE_FREE
	);
	if (cv){
		libaroma_canvas_free(cv);
	}
	return out;
}
#ifdef __cplusplus
}
#endif

#endif /* __libaroma_ctl_image_c__ */
