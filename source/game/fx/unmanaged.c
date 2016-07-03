/* unmanaged.c */
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
 * __________________________________________________________________
 *
 * <unmanaged>
 * Place for kind of "unmanaged code". No, there's no C# scent here -
 * what I am referring to for "unmanaged code" is the code that were
 * generated from 3rd-party tools, like particle code, that is expected
 * to be usable right away without additional tinkering. That is - they
 * are generated and used right away without being modified in any way -
 * whatever that would be (coding styles, algorithms, ...)
 *
 * Authors: Florastamine (florastamine@gmail.com)
 * __________________________________________________________________
 */
#define __VALID(entptr, position) if(!entptr)  return; if(!position) position = vector(entptr->x, entptr->y, entptr->z)

void particle_fire_set( ENTITY *entptr, VECTOR *position )
{
    __VALID(entptr, position);

    #ifdef    __PARTICLE_FIRE
        fire_create(position);
    #endif
}

void particle_smoke_set( ENTITY *entptr, VECTOR *position )
{
    __VALID(entptr, position);

    #ifdef    __PARTICLE_SMOKE
        smoke_create(position);
    #endif
}

void particle_sparkle_set( ENTITY *entptr, VECTOR *position )
{
    __VALID(entptr, position);

    #ifdef    __PARTICLE_SPARKLE
        sparkle_create(position);
    #endif
}

/*
 * void particle_process( ENTITY *emitter, VECTOR *pos, int id )
 *
 * Processes a particle. Merely a particle_X_set() wrapper with
 * additional checks and warnings - and for a more convenient method of
 * rendering particles. You don't want to create a lot of actions pointer (void * pointer)
 * and then assign them individually, instead you just want one unified action pointer
 * where you can specify which effect you want to use.
 */
void particle_process( ENTITY *emitter, VECTOR *pos, int id )
{
    id = (int) ifelse(id >= 0, id, abs(id));

    switch(id)
    {
        case    FIRE:        { particle_fire_set(my, pos); break; }
        case    SMOKE:       { particle_smoke_set(my, pos); break; }
        case    SPARKLE:     { particle_sparkle_set(my, pos); break; }

        default:
        {
            game_log_write( str_printf(NULL, "Invalid particle ID specified for entity %s. Particle will not be rendered.", _chr(str_for_entname(NULL, my))) );

            #ifdef    DEBUG
                ent_create(game_asset_get_2d_sprite("warning.png"), &my->x, NULL);
            #endif
        }
    }
}
