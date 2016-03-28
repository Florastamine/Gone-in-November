TEXT *__level_load_event_text = {
	strings = 1;
	string("");

	font = "Arial#20b";
}

int g_calls = 0;
bool g_init  = false;

void init() {
	VECTOR v;
	vec_set(&v, gui_screen_get_center());

	gui_text_set_pos(__level_load_event_text, v.x, v.y);
	__level_load_event_text->flags |= (SHOW);
}

void __level_load_event(var percent)
{
	if(!g_init)
	{
		init();
		g_init = true;
	}

	if(percent < 0.1)
		str_cpy((__level_load_event_text->pstring)[0], "Loading");
	else if(percent > 100.0 || var_cmp(percent, 100.0))
		__level_load_event_text->flags &= ~(SHOW);
	else
	{
        str_cat((__level_load_event_text->pstring)[0], ".");
        g_calls++;
    }
}
