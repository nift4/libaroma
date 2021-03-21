/*
 * ctl_clock.h
 *
 *  Created on: 2016年8月7日
 *      Author: fine
 *  Modified on: 2021/3/21
 *      By: MLX
 */

#ifndef __libaroma_aroma_h__
	#error "Include <aroma.h> instead."
#endif

#ifndef INCLUDE_AROMA_CONTROLS_CTL_CLOCK_H_
#define INCLUDE_AROMA_CONTROLS_CTL_CLOCK_H_
#define LIBAROMA_CTL_CLOCK_NO_BORDER	0x1

#define LIBAROMA_CTL_CLOCK_COLOR_HOUR	0
#define LIBAROMA_CTL_CLOCK_COLOR_MINUTE	1
#define LIBAROMA_CTL_CLOCK_COLOR_SECOND	2
#define LIBAROMA_CTL_CLOCK_COLOR_BORDER	3
#define LIBAROMA_CTL_CLOCK_COLOR_CENTER	4

LIBAROMA_CONTROLP libaroma_ctl_clock(LIBAROMA_WINDOWP win, word id, int x,
		int y, int w, int h, byte clock_style);

void libaroma_ctl_clock_set_color(LIBAROMA_CONTROLP ctl, byte type, word color);

void libaroma_ctl_clock_set_bg(LIBAROMA_CONTROLP ctl, LIBAROMA_CANVASP bg);

#define libaroma_ctl_clock_disable_border(ctl) libaroma_ctl_clock_set_border(ctl, 0)
#define libaroma_ctl_clock_enable_border(ctl) libaroma_ctl_clock_set_border(ctl, 1)
void libaroma_ctl_clock_set_border(LIBAROMA_CONTROLP ctl, byte enabled);

#endif /* INCLUDE_AROMA_CONTROLS_CTL_CLOCK_H_ */
