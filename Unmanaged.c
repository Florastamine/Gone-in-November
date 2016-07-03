TEXT *__level_load_event_text = {
	strings = 1;
	layer   = 999;
	string("");

	font = "Arial#20b";
}

void init() {
	__level_load_event_text->pos_x = 15.0;
	__level_load_event_text->pos_y = screen_size.y - 42.0;

	__level_load_event_text->flags |= SHOW;
}

void __level_load_event(var percent)
{
	if( !g_init )
	{
		init();
		g_init = true;
	}

	if(percent < 0.1)
		str_cpy((__level_load_event_text->pstring)[0], "Loading");
	else if(percent >= 100.0)
		__level_load_event_text->flags &= ~(SHOW);
	else
		str_cat((__level_load_event_text->pstring)[0], ".");
}
