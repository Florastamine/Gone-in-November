#include <acknex.h>
#include <strio.c>

int main(void)
{
	char *fn = file_dialog("Select which example you want to run: ", "*.c");
	
	if( fn ) {
		exec( "%EXE_DIR%\\acknex", fn );
		sys_exit(0);
	}
}
