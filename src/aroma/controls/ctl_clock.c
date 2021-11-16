#ifndef __libaroma_ctl_clock_c__
#define __libaroma_ctl_clock_c__
#include <aroma_internal.h>
#include "../ui/ui_internal.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#define MYPI 3.14159265358979323846

dword _libaroma_ctl_clock_msg(LIBAROMA_CONTROLP, LIBAROMA_MSGP);
void _libaroma_ctl_clock_draw(LIBAROMA_CONTROLP, LIBAROMA_CANVASP);
void _libaroma_ctl_clock_destroy(LIBAROMA_CONTROLP);
byte _libaroma_ctl_clock_thread(LIBAROMA_CONTROLP);
dword _libaroma_ctl_clock_msg(LIBAROMA_CONTROLP ctl, LIBAROMA_MSGP msg);

void gettime(int *hour, int *min, int *sec)
{
	//hal_rtc_time_t cur_time;
	//hal_rtc_get_time(&cur_time);
	time_t now = time(NULL);
	struct tm *tm_struct = localtime(&now);
	*hour = tm_struct->tm_hour;
	*min = tm_struct->tm_min;
	*sec = tm_struct->tm_sec;
}

static LIBAROMA_CONTROL_HANDLER _libaroma_ctl_clock_handler =
	{
		message : _libaroma_ctl_clock_msg,
		draw : _libaroma_ctl_clock_draw,
		focus : NULL,
		destroy : _libaroma_ctl_clock_destroy,
		thread : _libaroma_ctl_clock_thread
	};

typedef struct __LIBAROMA_CTL_CLOCK _LIBAROMA_CTL_CLOCK;
typedef struct __LIBAROMA_CTL_CLOCK *_LIBAROMA_CTL_CLOCKP;
struct __LIBAROMA_CTL_CLOCK
{
	LIBAROMA_CANVASP bg;
	byte style;
	word hcolor;
	word mcolor;
	word scolor;
	word bcolor;
	word ccolor;
	int hour;
	int min;
	int sec;
	byte forcedraw;

	LIBAROMA_MUTEX mutex;
};

dword _libaroma_ctl_clock_msg(LIBAROMA_CONTROLP ctl, LIBAROMA_MSGP msg)
{
	/* internal check */
	_LIBAROMA_CTL_CHECK(
		_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, 0);

	switch (msg->msg){
		case LIBAROMA_MSG_WIN_ACTIVE:
		case LIBAROMA_MSG_WIN_INACTIVE:
		case LIBAROMA_MSG_WIN_RESIZE:{
			libaroma_mutex_lock(me->mutex);
			me->forcedraw = 1;
			libaroma_mutex_unlock(me->mutex);
		} break;
		case LIBAROMA_MSG_TOUCH:{
			int x = msg->x;
			int y = msg->y;
			//ALOGD("=============>touch x %d y %d<=============== \r\t", x, y);
			/* touch handler */
			if (msg->state == LIBAROMA_HID_EV_STATE_UP){
				libaroma_window_post_command_ex(
					LIBAROMA_CMD_SET(LIBAROMA_CMD_CLICK, 0, ctl->id),
					0, 0, 0, ctl
				);
			}
		} break;
	}
	return 0;
}

LIBAROMA_CONTROLP libaroma_ctl_clock(
	LIBAROMA_WINDOWP win,
	word id, int x,	int y,
	int w, int h,
	byte style
){
	/* init internal data */
	_LIBAROMA_CTL_CLOCKP me = (_LIBAROMA_CTL_CLOCKP)
		calloc(sizeof(_LIBAROMA_CTL_CLOCK), 1);
	if (!me)
	{
		ALOGW("libaroma_ctl_clock alloc clock memory failed");
		return NULL;
	}

	/* set internal data */
	libaroma_mutex_init(me->mutex);
	gettime(&me->hour, &me->min, &me->sec);
	me->hcolor=RGB(342756);
	me->mcolor=RGB(348856);
	me->scolor=RGB(349999);
	me->bcolor=RGB(10DCA0);
	me->ccolor=RGB(FF00FF);
	me->style=style;

	/* init control */
	LIBAROMA_CONTROLP ctl = libaroma_control_new(id, x, y, w, h,
												 libaroma_dp(48), libaroma_dp(48), /* min size */
												 (voidp)me, &_libaroma_ctl_clock_handler, win);
	if (!ctl)
	{
		libaroma_mutex_free(me->mutex);
		free(me);
		return NULL;
	}
	me->forcedraw = 1;
	return ctl;
} /* End of libaroma_ctl_clock */

void libaroma_ctl_clock_set_color(LIBAROMA_CONTROLP ctl, byte type, word color){
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, );
	libaroma_mutex_lock(me->mutex);
	switch (type){
		case LIBAROMA_CTL_CLOCK_COLOR_HOUR:
			me->hcolor=color;
			break;
		case LIBAROMA_CTL_CLOCK_COLOR_MINUTE:
			me->mcolor=color;
			break;
		case LIBAROMA_CTL_CLOCK_COLOR_SECOND:
			me->scolor=color;
			break;
		case LIBAROMA_CTL_CLOCK_COLOR_BORDER:
			me->bcolor=color;
			break;
		case LIBAROMA_CTL_CLOCK_COLOR_CENTER:
			me->ccolor=color;
			break;
	}
	libaroma_mutex_unlock(me->mutex);
}

void libaroma_ctl_clock_set_border(LIBAROMA_CONTROLP ctl, byte enable){
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, );
	libaroma_mutex_lock(me->mutex);
	if (enable && me->style&LIBAROMA_CTL_CLOCK_NO_BORDER)
		me->style=0;//LIBAROMA_CTL_CLOCK_NO_BORDER;
	else if (!enable&&!(me->style&LIBAROMA_CTL_CLOCK_NO_BORDER))
		me->style|=LIBAROMA_CTL_CLOCK_NO_BORDER;
	libaroma_mutex_unlock(me->mutex);
}

void libaroma_ctl_clock_set_bg(
	LIBAROMA_CONTROLP ctl,
	LIBAROMA_CANVASP bg
){
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, );
	libaroma_mutex_lock(me->mutex);
	if (me->bg!=NULL)
		libaroma_canvas_free(me->bg);
	me->bg=(bg==NULL)?NULL:libaroma_canvas_dup(bg);
	me->forcedraw=1;
	libaroma_mutex_unlock(me->mutex);
}

void getLocationOnCircles(
	int cx, int cy,
	int radius, int angle,
	int *outx, int *outy
){
	float tmp = angle * (2 * MYPI / 360);
	if (angle >= 0 && angle <= 90)
	{
		*outx = cx + radius * sin(tmp);
		*outy = cy - radius * cos(tmp);
	}
	else if (angle > 90 && angle <= 180)
	{
		tmp = tmp - MYPI / 2;
		*outx = cx + radius * cos(tmp);
		*outy = cy + radius * sin(tmp);
	}
	else if (angle > 180 && angle <= 270)
	{
		tmp = tmp - MYPI;
		*outx = cx - radius * sin(tmp);
		*outy = cy + radius * cos(tmp);
	}
	else if (angle > 270 && angle <= 360)
	{
		tmp = tmp - (MYPI + MYPI / 2);
		*outx = cx - radius * cos(tmp);
		*outy = cy - radius * sin(tmp);
	}
	//printf("loacation on x:%d  y:%d  \t\n", *outx, *outy);
}

void _libaroma_ctl_clock_draw(LIBAROMA_CONTROLP ctl, LIBAROMA_CANVASP c)
{
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, );
	if (me->bg==NULL) libaroma_control_erasebg(ctl, c);
	else libaroma_draw_scale_smooth(c, me->bg, 0, 0, ctl->w, ctl->h, 0, 0, me->bg->w, me->bg->h);
	int sz = MIN(c->w >> 1, c->h >> 1);
	int outx, outy;
	libaroma_draw_circle(c, RGB(ff0000), sz, sz, sz - 130, 0xff);
	libaroma_draw_circle(c, RGB(00ff00), sz, sz, sz - 150, 0xff);
	if (!(me->style&LIBAROMA_CTL_CLOCK_NO_BORDER))
		libaroma_draw_arc(c, sz, sz, sz, sz, 15, 0, 360, me->bcolor,
					  0x80, 0, 0.8);
	int tmphour = me->hour > 12 ? me->hour - 12 : me->hour;
	getLocationOnCircles(sz, sz, (sz/3)*2, tmphour * 1.0 / 12 * 360,
						  &outx, &outy);
	libaroma_draw_line(c, sz, sz, outx, outy, 13, me->hcolor,
					   0x7f, 0);
	getLocationOnCircles(sz, sz, sz, me->min * 1.0 / 60 * 360,
						  &outx, &outy);
	libaroma_draw_line(c, sz, sz, outx, outy, 8, me->mcolor,
					   0x7f, 0);
	getLocationOnCircles(sz, sz, (sz/8)*7,
						  me->sec * 1.0 / 60 * 360, &outx, &outy);
	libaroma_draw_line(c, sz, sz, outx, outy, 5, me->scolor,
					   0x7f, 0);

	libaroma_draw_circle(c, me->ccolor, sz, sz, 20, 0xff);
}

void _libaroma_ctl_clock_destroy(LIBAROMA_CONTROLP ctl)
{
	/* internal check */
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, );
	libaroma_mutex_lock(me->mutex);

	libaroma_mutex_unlock(me->mutex);
	libaroma_mutex_free(me->mutex);
	free(me);
}

byte _libaroma_ctl_clock_thread(LIBAROMA_CONTROLP ctl)
{
	/* internal check */
	_LIBAROMA_CTL_CHECK(_libaroma_ctl_clock_handler, _LIBAROMA_CTL_CLOCKP, 0);
	int hour = 0, min = 0, sec = 0;
	gettime(&hour, &min, &sec);
	if (me->hour != hour || me->min != min || me->sec != sec)
	{
		me->hour = hour;
		me->min = min;
		me->sec = sec;
		me->forcedraw = 1;
	}
	if (me->forcedraw)
	{
		me->forcedraw = 0;
		return 1;
	}
	return 0;
}

#endif
