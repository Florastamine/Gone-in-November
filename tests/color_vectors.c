/* color_vectors.c */
/*
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *                     Version 2, December 2004
 *
 *  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 *  Everyone is permitted to copy and distribute verbatim or modified
 *  copies of this license document, and changing it is allowed as long
 *  as the name is changed.
 *
 *             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 *   0. You just DO WHAT THE FUCK YOU WANT TO.
 */
#include <acknex.h>
#include "..\\source\\utils\\utilities.h"

int main(void)
{
    while( !ready() ) wait(1.0);

    const STRING *gstr = "I don't wanna die";
    draw_textmode( "Courier", 1, 25.0, 100.0 );
    draw_begin();

    while( !key_esc )
    {
        draw_text(gstr, 10, 10, COLOR_SOFT_PURPLE);
        draw_text(gstr, 10, 35, COLOR_LIME);
        draw_text(gstr, 10, 60, COLOR_CHERRY);
        draw_text(gstr, 10, 85, COLOR_INDIGO);
        draw_text(gstr, 10, 110, COLOR_CRIMSON);
        draw_text(gstr, 10, 135, COLOR_YELLOW);
        draw_text(gstr, 10, 160, COLOR_LAVENDER_ROSE);
        draw_text(gstr, 10, 185, COLOR_BITTER_LEMON);
        draw_text(gstr, 10, 210, COLOR_PEACH_ORANGE);
        draw_text(gstr, 10, 235, COLOR_PALE_LILAC);
        draw_text(gstr, 10, 260, COLOR_TEAL);
        draw_text(gstr, 10, 285, COLOR_ORCHID);
        draw_text(gstr, 10, 310, COLOR_APPLE_GREEN);
        draw_text(gstr, 10, 335, COLOR_ARCTIC_LIME);
        draw_text(gstr, 10, 360, COLOR_AQUAMARINE);
        draw_text(gstr, 10, 385, COLOR_BLEU_DE_FRANCE);
        draw_text(gstr, 10, 410, COLOR_SCARLET);
        draw_text(gstr, 10, 435, COLOR_SAND);
        draw_text(gstr, 10, 460, COLOR_PEACH);
        draw_text(gstr, 10, 485, COLOR_PEAR);
        draw_text(gstr, 10, 510, COLOR_PEARL);
        wait(1.0);
    }

    RETURN (0);
}
