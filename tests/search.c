#include <acknex.h>
#include "..\\source\\utils\\utilities.h"

int main(void)
{
    video_screen = 0;

    const STRING *substr = "substR";
    TEXT *text = txt_create(0, 1);

    txt_addstring(text, "my substr");
    txt_addstring(text, "your substr");
    txt_addstring(text, "substr");
    txt_addstring(text, "subSTR");
    txt_addstring(text, "SUBSTR");
    txt_addstring(text, "substr her");

    // Comparing strings with case sensivity (true), should yield -1
    printf("\n%i", (int) search(text, substr, true));

    // Comparing strings without case sensivity, should yield 3
    printf("\n%i", (int) search(text, substr, false));

    while( !key_esc ) wait(1.0);
    RETURN (0);
}
