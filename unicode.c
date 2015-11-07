// @Talemon
#include <acknex.h>
#include <stdio.h>

void file_str_writeu(FILE* arg_fp, STRING* arg_str)
{
	int i;
	for(i = 0; i < str_len(arg_str) * 2; i++)
	{
		var ch = (arg_str->chars)[i];
		file_asc_write(arg_fp, ch);
	}
}

void file_str_writelineu(FILE* arg_fp, STRING* arg_str)
{
	file_str_write(arg_fp, arg_str);
	file_asc_write(arg_fp, 0x0a); // \n first byte
	file_asc_write(arg_fp, 0x00); // \n second byte
}

void file_str_writebomu(FILE* arg_fp)
{
	file_asc_write(arg_fp, 0xff); // 
	file_asc_write(arg_fp, 0xfe); // Byte-order mark(BOM) for UCS-2 LE
}

FILE* file_create_writeu(char* arg_filename)
{
	FILE* fp = file_open_write(arg_filename);
	file_str_writebomu(fp);
	return fp;
}

int main()
{
	return 0;
}
