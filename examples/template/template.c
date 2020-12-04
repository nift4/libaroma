
void template_page(LIBAROMA_WINDOWP parent){

	LIBAROMA_WINDOWP win = libaroma_window(
		NULL, 0, 0, LIBAROMA_SIZE_FULL, LIBAROMA_SIZE_FULL);	
	
	printf("Showing window!\n");
	statusbar_end_thread(); /* stop sb thread to play window open/close animations */
	libaroma_window_anishow(win, LIBAROMA_WINDOW_SHOW_ANIMATION_PAGE_TOP, 300);
	statusbar_start_thread(); /* start it again so it actually works */
	printf("Window should be visible.\n");
	do{				
		LIBAROMA_MSG msg;
		dword command=libaroma_window_pool(win,&msg);
		byte cmd	= LIBAROMA_CMD(command);
		word id		= LIBAROMA_CMD_ID(command);
		byte param	= LIBAROMA_CMD_PARAM(command);
		
		switch (msg.msg){
			case LIBAROMA_MSG_KEY_POWER:
			case LIBAROMA_MSG_KEY_SELECT:
				{
					if (msg.state==0){
						//libaroma_png_save(libaroma_fb()->canvas,"/tmp/screenshot.png");
						printf("Close key pressed!\n");
						win->onpool=0;
					}
				}
				break;
		}
		
		if (cmd == 1){ //window received click event, handle it
			/*
			if (id==buttonname->id){ 
			} 
			*/
			/* if list item was clicked, it receives the click event
				so use this instead */
			/*
			if (id==somelist->id){
				if (msg.key==somelistitem->id){
					
				}
			}
			*/
		}
		
	}
	while(win->onpool);
	
	printf("Closing window!\n");
	statusbar_end_thread(); /* needed in order to play window animations */
	libaroma_window_aniclose(win, parent, LIBAROMA_WINDOW_CLOSE_ANIMATION_PAGE_BOTTOM, 300);
	statusbar_start_thread();/* start it again so the statusbar animation plays */
}
