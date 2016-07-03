/* unmanaged.h */
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
#ifndef    _UNMANAGED_H_
#define    _UNMANAGED_H_

#define    __PARTICLE_FIRE
#define    __PARTICLE_SMOKE
#define    __PARTICLE_SPARKLE

#define    FIRE         1
#define    SMOKE        2
#define    SPARKLE      3

#define PRAGMA_PRINT "\nUnmanaged code initialization..."

#ifdef    __PARTICLE_FIRE
    #include "fire.c"
#endif

#ifdef    __PARTICLE_SMOKE
    #include "smoke.c"
#endif

#ifdef    __PARTICLE_SPARKLE
    #include "sparkle.c"
#endif

void particle_fire_set( ENTITY *entptr, VECTOR *position );
void particle_smoke_set( ENTITY *entptr, VECTOR *position );
void particle_sparkle_set( ENTITY *entptr, VECTOR *position );

/*
 * void particle_process( ENTITY *emitter, VECTOR *pos, int id )
 *
 * Processes a particle. Merely a particle_X_set() wrapper with
 * additional checks and warnings - and for a more convenient method of
 * rendering particles. You don't want to create a lot of actions pointer (void * pointer)
 * and then assign them individually, instead you just want one unified action pointer
 * where you can specify which effect you want to use.
 */
void particle_process( ENTITY *emitter, VECTOR *pos, int id );

#include "unmanaged.c"
#endif /* unmanaged.h */
