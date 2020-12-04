int NAVBAR_HEIGHT=30;

void navbar_draw(LIBAROMA_WINDOWP win){
	#define			ID_NAVMENU		71
	#define			ID_NAVHOME		72
	#define			ID_NAVBACK		73
	
	printf("Creating navbar!\n");
	LIBAROMA_CANVASP menu_icon = libaroma_canvas(libaroma_dp(NAVBAR_HEIGHT), libaroma_dp(NAVBAR_HEIGHT));
	LIBAROMA_CANVASP home_icon = libaroma_canvas(libaroma_dp(NAVBAR_HEIGHT), libaroma_dp(NAVBAR_HEIGHT));
	LIBAROMA_CANVASP back_icon = libaroma_canvas(libaroma_dp(NAVBAR_HEIGHT), libaroma_dp(NAVBAR_HEIGHT));
	
	libaroma_draw_scale_smooth(menu_icon, libaroma_image_uri("res:///icons/menu.png"), 
									0, 0, menu_icon->w, menu_icon->h, 0, 0, 30, 30);
	libaroma_draw_scale_smooth(home_icon, libaroma_image_uri("res:///icons/home.png"), 
									0, 0, menu_icon->w, menu_icon->h, 0, 0, 30, 30);
	libaroma_draw_scale_smooth(back_icon, libaroma_image_uri("res:///icons/back.png"),
									0, 0, menu_icon->w, menu_icon->h, 0, 0, 30, 30);
	
	int menubtn_x=(win->w/2)/2;
	int backbtn_x=((win->w/2)/2)*3;	
	LIBAROMA_CONTROLP menubtn_list = libaroma_ctl_list(
		win, ID_NAVMENU,
		0, libaroma_px(win->h-libaroma_dp(NAVBAR_HEIGHT)), 96, NAVBAR_HEIGHT,
		0, 0,
		RGB(000000),
		LIBAROMA_CTL_SCROLL_NO_INDICATOR);
	libaroma_listitem_image(menubtn_list, ID_NAVMENU, menu_icon, 
								NAVBAR_HEIGHT, LIBAROMA_CTL_LIST_ITEM_RECEIVE_TOUCH, -1);
	LIBAROMA_CONTROLP homebtn_list = libaroma_ctl_list(
		win, ID_NAVHOME,
		libaroma_px((win->w/2)-(libaroma_dp(96)/2)), libaroma_px(win->h-libaroma_dp(NAVBAR_HEIGHT)), 96, NAVBAR_HEIGHT,
		0, 0,
		RGB(000000),
		LIBAROMA_CTL_SCROLL_NO_INDICATOR);
	libaroma_listitem_image(homebtn_list, ID_NAVHOME, home_icon, 
								NAVBAR_HEIGHT, LIBAROMA_CTL_LIST_ITEM_RECEIVE_TOUCH, -1);	
	LIBAROMA_CONTROLP backbtn_list = libaroma_ctl_list(
		win, ID_NAVBACK,
		libaroma_px(win->w-libaroma_dp(96)), libaroma_px(win->h-libaroma_dp(NAVBAR_HEIGHT)), 96, NAVBAR_HEIGHT,
		0, 0,
		RGB(000000),
		LIBAROMA_CTL_SCROLL_NO_INDICATOR);
	libaroma_listitem_image(backbtn_list, ID_NAVBACK, back_icon, 
								NAVBAR_HEIGHT, LIBAROMA_CTL_LIST_ITEM_RECEIVE_TOUCH, -1);	
	
	
	printf("********DEBUG********\n");
	printf("Navbar height: %dpx->%ddp\n", NAVBAR_HEIGHT, libaroma_dp(NAVBAR_HEIGHT));
	printf("Menulist size: %dx%d at %d, %d\n", menubtn_list->w, menubtn_list->h, menubtn_list->x, menubtn_list->y);
	printf("Homelist size: %dx%d at %d, %d\n", homebtn_list->w, homebtn_list->h, homebtn_list->x, homebtn_list->y);
	printf("Backlist size: %dx%d at %d, %d\n", backbtn_list->w, backbtn_list->h, backbtn_list->x, backbtn_list->y);
	printf("*********************\n");
}