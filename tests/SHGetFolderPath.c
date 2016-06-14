#include <acknex.h>
#include <windows.h>

#define		CSIDL_COMMON_DESKTOPDIRECTORY		25

HRESULT WINAPI SHGetFolderPath( HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath );

APIA(SHGetFolderPath, shell32)

int main(void)
{
	char *path = (char *) malloc(sizeof(char) * 1024);

	HRESULT h = SHGetFolderPath(NULL, CSIDL_COMMON_DESKTOPDIRECTORY, NULL, 0, path);
	if( h == S_OK )
		printf("%s", path);

	free(path);
	sys_exit(0);
}
