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
 * Filename		: ctl_toast.h
 * Description : toast control
 *
 * + This is part of libaroma, an embedded ui toolkit.
 * + 06/02/15 - Author(s): Ahmad Amarullah
 *
 */
#ifndef __libaroma_aroma_h__
	#error "Include <aroma.h> instead."
#endif
#ifndef __libaroma_ctl_toast_h__
#define __libaroma_ctl_toast_h__

//#define LIBAROMA_CTL_TOAST_		0x
#define LIBAROMA_CTL_TOAST_COLORED		0x1
#define LIBAROMA_CTL_TOAST_OPAQUE		0x2

/* toast icon style */
#define LIBAROMA_CTL_TOAST_ICON_CENTER	0x4
#define LIBAROMA_CTL_TOAST_ICON_RIGHT	0x8
#define LIBAROMA_CTL_TOAST_ICON_OUTSIDE	0x16

/* for code readability - do nothing */
#define LIBAROMA_CTL_TOAST_ICON_LEFT		0x0
#define LIBAROMA_CTL_TOAST_ICON_INSIDE		0x0
#define LIBAROMA_CTL_TOAST_ICON_TRANSPARENT	0x0

/* window-relative toast position */
#define LIBAROMA_CTL_TOAST_GRAVITY_BOTTOM	0x0 //default
#define LIBAROMA_CTL_TOAST_GRAVITY_TOP		0x1
#define LIBAROMA_CTL_TOAST_GRAVITY_CENTER	0x2

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
);

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
);

/*
 * Function		: libaroma_ctl_toast_seticon
 * Return Value: byte
 * Descriptions: set toast icon
 */
byte libaroma_ctl_toast_seticon(
		LIBAROMA_CONTROLP ctl,
		LIBAROMA_CANVASP icon,
		byte icon_flags
);

/*
 * Function		: libaroma_ctl_toast_style
 * Return Value: byte
 * Descriptions: set toast style
 */
byte libaroma_ctl_toast_style(
		LIBAROMA_CONTROLP ctl,
		byte toast_style,
		word toast_color
);

/*
 * Function		: libaroma_ctl_toast_gravity
 * Return Value: byte
 * Descriptions: set toast gravity
 */
byte libaroma_ctl_toast_gravity(
		LIBAROMA_CONTROLP ctl,
		byte gravity
);

/*
 * Function		: libaroma_ctl_toast_hide
 * Return Value: byte
 * Descriptions: hide toast
 */
byte libaroma_ctl_toast_hide(
		LIBAROMA_CONTROLP ctl
);

/*
 * Function		: libaroma_ctl_toast_show
 * Return Value: byte
 * Descriptions: show toast
 */
byte libaroma_ctl_toast_show(
		LIBAROMA_CONTROLP ctl
);

/*
 * Function		: libaroma_ctl_toast_text
 * Return Value: byte
 * Descriptions: set toast text
 */
byte libaroma_ctl_toast_text(
		LIBAROMA_CONTROLP ctl,
		const char * text
);

/*
 * Function		: libaroma_ctl_toast_is_disabled
 * Return Value: byte
 * Descriptions: check if control is toast
 */
byte libaroma_ctl_is_toast(LIBAROMA_CONTROLP ctl);

#endif /* __libaroma_ctl_toast_h__ */
