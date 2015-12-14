#include <acknex.h>
#include "..\\source\\utils\\utilities.h"

#define    INCLUDE_GUARD_HEADER "#ifndef    __BINDINGS_H__\r\n#define    __BINDINGS_H__\r\n"
#define    INCLUDE_GUARD_FOOTER "\r\n#endif"
#define    FILE "..\\source\\game\\bind\\bindings.h"

TEXT     *files_list = { strings = 10000; }
STRING   *current_directory = "";
int       files_found = 0;

void binder_read_data(STRING *directory, STRING *extension);
void binder_write_data(STRING *file);

void binder_write_header(STRING *file);
void binder_write_footer(STRING *file);

void binder_perform(STRING *file, TEXT *dirs, TEXT *exts);

int main(void)
{
	while( !ready() ) wait(1.0);
	video_screen = 0;

	TEXT *directories = txt_create(0, 1);
	TEXT *extensions  = txt_create(0, 1);

	txt_addstring(directories, "..\\scene");
	txt_addstring(directories, "..\\fx");
	txt_addstring(directories, "..\\object");
	
	txt_addstring(directories, "..\\2d");
	txt_addstring(directories, "..\\2d\\fx");
	txt_addstring(directories, "..\\2d\\gui");
	txt_addstring(directories, "..\\2d\\sprites");

	txt_addstring(extensions, "ogg");
	txt_addstring(extensions, "wav");

	txt_addstring(extensions, "png");
	txt_addstring(extensions, "tga");
	txt_addstring(extensions, "mdl");
	txt_addstring(extensions, "jpg");
	
	txt_addstring(extensions, "fx");
	txt_addstring(extensions, "fxo");
	txt_addstring(extensions, "wmb");
	txt_addstring(extensions, "hmp");
	
	binder_perform( FILE, directories, extensions );

	txt_remove_ex(directories);
	txt_remove_ex(extensions);

	sys_exit(0);
}

void binder_perform(STRING *file, TEXT *dirs, TEXT *exts)
{
	binder_write_header(file);
	
	int i, j;
	for (i = 0; i < dirs->strings; i++)
	{
		for (j = 0; j < exts->strings; j++)
		{
			binder_read_data((dirs->pstring)[i], (exts->pstring)[j]);
			binder_write_data(file);
		}
	}
	
	binder_write_footer(file);	
}

void binder_write_header(STRING *file)
{
	fixed channel = file_open_write(file);
	if(channel)
	{
		file_str_write(channel, "/* ");
		file_str_write(channel, FILE);
		file_str_write(channel, " */\n");
		file_str_write(channel, INCLUDE_GUARD_HEADER);
		
		file_close(channel);
	}
}

void binder_write_footer(STRING *file)
{
	fixed channel = file_open_append(file);
	if(channel)
	{
		file_str_write(channel, INCLUDE_GUARD_FOOTER);
		file_str_write(channel, " /* ");
		file_str_write(channel, FILE);
		file_str_write(channel, " */");
		
		file_asc_write(channel, 13);
		file_asc_write(channel, 10);
		
		file_close(channel);
	}
}

void binder_write_data(STRING *file)
{
	if( files_found <= 0 ) return;
	
	int i = 0;
	fixed channel = file_open_append(file);
	STRING *str = "";

	if (channel)
	{
		file_str_write(channel, "\r\n/*-----------------------------*/\r\n");
		file_str_write(channel, "/* ");
		file_str_write(channel, current_directory);
		file_str_write(channel, " */\r\n");
		file_str_write(channel, "/*-----------------------------*/\r\n");
		for (i = 0; i < files_found; i++)
		{
			str_cpy(str, "#define PRAGMA_BIND \"");
			str_cat(str, (files_list->pstring)[i]);
			str_cat(str, "\"\r\n");
			file_str_write(channel, str);
		}
		
		file_close(channel);
	}
}

void binder_read_data(STRING *directory, STRING *extension)
{
	STRING *searchStr = "";
	str_cpy(current_directory, directory);
	str_cpy(searchStr, directory);
	str_cat(searchStr, "\\*.");
	str_cat(searchStr, extension);

	files_found = txt_for_dir(files_list, searchStr);
}
