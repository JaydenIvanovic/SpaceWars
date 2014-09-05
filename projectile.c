#include "projectile.h"
#include "colors.h"
#include "lighting.h"
#include "controls.h"
#include "util.h"
#include "sound.h"
#include "geometry.h"
#include "main.h"
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>


/*Call each teams helper function which sets 
 *the firing cannons initial position and rotation. 
 *Sets initial gravity to.*/
void initFiringVariables()
{
    redFiringInit();
    blueFiringInit();

    /*The suns position. Useful for testing
     *the distance between it and a projectile
     *to account for pull.*/
    globalProjectile.sunPos.x = 0.0;
    globalProjectile.sunPos.y = 0.0;
    globalProjectile.sunPos.z = 0.0;

    globalProjectile.pIndex = 0;
}




/*Initialize the red teams firing position
 *and default rotation angle.*/
void redFiringInit()
{
    /*Position*/
    globalProjectile.redPos.x = -BASE_X_POS;
    globalProjectile.redPos.y = BASE_Y_POS;
    globalProjectile.redPos.z = BASE_Z_POS;

    /*Rotation*/
    globalProjectile.redRot.x = BASE_X_ROT;
    globalProjectile.redRot.y = BASE_Y_ROT;
    globalProjectile.redRot.z = BASE_Z_ROT;
}




/*Initialize the blue teams firing position
 *and default rotation angle.*/
void blueFiringInit()
{
    /*Position*/
    globalProjectile.bluePos.x = BASE_X_POS;
    globalProjectile.bluePos.y = BASE_Y_POS;
    globalProjectile.bluePos.z = BASE_Z_POS;

    /*Rotation*/
    globalProjectile.blueRot.x = -BASE_X_ROT;
    globalProjectile.blueRot.y = BASE_Y_ROT;
    globalProjectile.blueRot.z = BASE_Z_ROT;
}




/*Set the default projectile gravity in
 *regards to the team.*/
void setProjectileGravConst(struct projectile *newProj, char team)
{
    if(team == RED_BASE)
    {
        /*If y is negative we want to pull up 
         *towards the sun.*/
        if((int)globalProjectile.redRot.y < 0)
            newProj->gravconst = GRAV_CONST;
        else if((int)globalProjectile.redRot.y > 0)
            newProj->gravconst = -GRAV_CONST;
        else
            newProj->gravconst = 0.0;
    }
    else
    {
        if((int)globalProjectile.blueRot.y < 0)
            newProj->gravconst = GRAV_CONST;
        else if((int)globalProjectile.blueRot.y > 0)
            newProj->gravconst = -GRAV_CONST;
        else
            newProj->gravconst = 0.0;
    }
}




/*Set default starting position and velocity.*/
void setDefaultProjectileState(struct projectile *newProj, char team)
{
    if(team == RED_BASE)
    {
        newProj->lastDistance = globalProjectile.redPos.x;

        newProj->position.x = globalProjectile.redPos.x;
        newProj->position.y = globalProjectile.redPos.y;
        newProj->position.z = globalProjectile.redPos.z;

        newProj->velocity.x = globalProjectile.redRot.x;
        newProj->velocity.y = globalProjectile.redRot.y;
        newProj->velocity.z = globalProjectile.redRot.z;
    }
    else
    {
        newProj->lastDistance = globalProjectile.bluePos.x;

        newProj->position.x = globalProjectile.bluePos.x;
        newProj->position.y = globalProjectile.bluePos.y;
        newProj->position.z = globalProjectile.bluePos.z;

        newProj->velocity.x = globalProjectile.blueRot.x;
        newProj->velocity.y = globalProjectile.blueRot.y;
        newProj->velocity.z = globalProjectile.blueRot.z;
    }
}




/*Create a new projectile and add it to
 *to the array to keep track of.*/
void createProjectile(char base)
{
    /*If the array is currently full, skip.*/
    if(globalProjectile.pIndex >= PROJ_MAX)
        globalProjectile.pIndex = 0;

    struct projectile newProj;

    newProj.color = base;

    setDefaultProjectileState(&newProj, base);
    setProjectileGravConst(&newProj, base);

    newProj.gravity.x = 0.0;
    newProj.gravity.y = newProj.gravconst;
    newProj.gravity.z = 0.0;

    globalProjectile.projectiles[globalProjectile.pIndex] = newProj;
    globalProjectile.pIndex++;

    glutPostRedisplay();
}




/*Rotate the missile left. Blue and red bases
 *have their own rotation functions as the meaning
 *of left and right are different for both, but
 *when calling the method from the controls.c file
 *this way is easier to read.*/
void rotateRedLeft()
{
    if(globalProjectile.redRot.y >= RED_MAX_VEL)
        return;

    globalProjectile.redRot.y += CANNON_ROTATE_SPEED;
}




/*Rotate the missile right.*/
void rotateRedRight()
{
    if(globalProjectile.redRot.y <= RED_MIN_VEL)
        return;

    globalProjectile.redRot.y -= CANNON_ROTATE_SPEED;
}




/*Rotate the missile left.*/
void rotateBlueLeft()
{
    if(globalProjectile.blueRot.y <= BLUE_MIN_VEL)
        return;

    #if 0
    printf("x:%f  y:%f\n", globalProjectile.blueRot.x, globalProjectile.blueRot.y);
    #endif

    globalProjectile.blueRot.y -= CANNON_ROTATE_SPEED;
}




/*Rotate the missile right.*/
void rotateBlueRight()
{
    if(globalProjectile.blueRot.y >= BLUE_MAX_VEL)
        return;

    #if 0
    printf("x:%f  y:%f\n", globalProjectile.blueRot.x, globalProjectile.blueRot.y);
    #endif

    globalProjectile.blueRot.y += CANNON_ROTATE_SPEED;
}




/*Update projectile positions to adjust for gravitational
  pull from the sun.*/
void updateProjectileGravity(struct projectile *proj)
{
    float d = distance(proj->position, globalProjectile.sunPos); 
    float lastDist = proj->lastDistance;

    #if 1
    if(d < lastDist)
        /*Equation for newtons gravitational pull.*/
        proj->gravity.y = proj->gravconst * ( (1 * 1) / (d * d) );  
    #endif

    proj->lastDistance = d;
}




/*Update projectile positions.*/
void updateProjectilePos(float dt)
{
    if(globalProjectile.pIndex == 0)
        return;

    for(int i = 0; i < globalProjectile.pIndex; i++)
    {
        /* Update how close the projectile is to the sun
           and adjust gravity.*/
        updateProjectileGravity(&globalProjectile.projectiles[i]);

        /* Update the position by the velocity and change
           in time. */
        globalProjectile.projectiles[i].position.x += 
            globalProjectile.projectiles[i].velocity.x * ( dt / 2 );
        globalProjectile.projectiles[i].position.y += 
            globalProjectile.projectiles[i].velocity.y * ( dt / 2 );
        globalProjectile.projectiles[i].position.z += 
            globalProjectile.projectiles[i].velocity.z * ( dt / 2 );

        /* Update the velocity given the acceleration and
           change in time. */
        globalProjectile.projectiles[i].velocity.x += 
            globalProjectile.projectiles[i].gravity.x * ( dt / 2 );
        globalProjectile.projectiles[i].velocity.y += 
            globalProjectile.projectiles[i].gravity.y * ( dt / 2 );
        globalProjectile.projectiles[i].velocity.z += 
            globalProjectile.projectiles[i].gravity.z * ( dt / 2 );
    }
}




/*Draw projectiles.*/
void drawProjectiles()
{
    for(int i = 0; i < globalProjectile.pIndex; i++)
    {
        glPushMatrix();

        if(globalProjectile.projectiles[i].color == RED_BASE)
            setBaseMaterial(red);
        else if(globalProjectile.projectiles[i].color == BLUE_BASE)
            setBaseMaterial(blue);

        glTranslatef(globalProjectile.projectiles[i].position.x,
                globalProjectile.projectiles[i].position.y,
                globalProjectile.projectiles[i].position.z);
        drawSphereFilled();

        glPopMatrix();
    }
}




/*Determine if there was a collision between two
 *sphere objects. r1 and r2 for the radius of each.*/
int isCollision(struct vertex pos1, struct vertex pos2, 
                float r1, float r2)
{
    float d = distance(pos1, pos2); 

    /*Sum of the two radii.*/
    float rSum = r1 + r2;

    if(d < rSum)
        return TRUE; 
    return FALSE; 
}




/*Find the sum of the radii of a projectile
 *and given base.*/
int isBaseCollision(struct projectile proj, struct vertex base, 
                    char color)
{
    /*Base = RADIUS * BASE_SIZE as it's been scaled.*/
    float baseRadius = RADIUS * BASE_SIZE;
   
    /*Projectile has original radius size.*/
    int collided = isCollision(proj.position, base, RADIUS, baseRadius);
    
    if(collided && proj.color != color)
        return TRUE; 
    return FALSE; 
}




/*Larger operation to perform than the other collision tests
 *due to it being a little more complex. Therefore encapsulate
 *it into a separate function for readability.*/
int isSailCollision(struct vertex projPos, struct vertex sailCenter)
{
    if(isCollision(projPos, sailCenter, RADIUS, SAIL_SIZE))
    {
        int startfrom = 0;

        for(int j = 0; j < NUM_SHADOWS; j++)
        {
            /*Shadow breaks holds the integers where one shadow stops and another
             *should begin. That is why the startfrom variable is included to return
             *k to the value it was before the previous iteration completed.*/
            for(int k = startfrom; k < globalGeometry.shadowBreaks[j]; k++, startfrom++)
            {
                struct vertex sail;
                
                sail.x = globalGeometry.shadow[k].x * (SAIL_SIZE + 0.001);
                sail.y = globalGeometry.shadow[k].y * (SAIL_SIZE + 0.001);
                sail.z = globalGeometry.shadow[k].z;
                
                float dist = distance(projPos, sail);
                
                /*Magic number which seems to work well.*/
                if(dist <= SAIL_COLLISION_DIST)
                    return TRUE;
            }
        }
    }
    return FALSE;
}




/*Test whether this projectile has collided with any others.
 *Return the position of the other projectile it collided with.
 *This function will return NO_COLLISION if no collision occured.*/
int projectileCollided(struct vertex projPos, int posIndex)
{
    for(int i = 0; i < globalProjectile.pIndex; i++)
    {
        /*If i and posIndex are the same then its the same 
         *projectile and the loop should skip it.*/
        if(i == posIndex)
            continue;

        if(isCollision(projPos, globalProjectile.projectiles[i].position,
                    RADIUS, RADIUS))
            return i;
    }
    return NO_COLLISION;
}




/*Remove the projectile from the array as it has collided
 *with something. Shift the rest of the array along and
 *update the current max index.*/
void deleteProjectile(int pos)
{
    globalProjectile.pIndex -= 1;

    for(int i = pos; i < globalProjectile.pIndex; i++)
        globalProjectile.projectiles[i] = globalProjectile.projectiles[i + 1];
}




/*If a collision occured, call this function to handle it.*/
void handleCollision(int i)
{
    //printf("Got a collision! Proj:%d\n", i);
    deleteProjectile(i);
    playCrashSound();
}




/*Check for base collisions.*/
void checkBaseCollisions(struct projectile proj, int i)
{
    /*Positions of the bases.*/
    struct vertex midBlueBase = {MID_BASE_X, MID_BASE_Y, 0.0};
    struct vertex northBlueBase = {SIDE_BASE_X, SIDE_BASE_Y, 0.0};
    struct vertex southBlueBase = {SIDE_BASE_X, -SIDE_BASE_Y, 0.0};
    struct vertex midRedBase = {-MID_BASE_X, 0.0, 0.0};
    struct vertex northRedBase = {-SIDE_BASE_X, SIDE_BASE_Y, 0.0};
    struct vertex southRedBase = {-SIDE_BASE_X, -SIDE_BASE_Y, 0.0};

    /*Test the bases for collision only if they are alive(this is given
     *via the globalBase struct.)*/

    /*Test blue bases.*/
    if(isBaseCollision(proj, midBlueBase, BLUE_BASE) && globalBase.blueMid)
    {
        handleCollision(i);
        globalBase.blueMid = FALSE;
    }
    if(isBaseCollision(proj, northBlueBase, BLUE_BASE) && globalBase.blueNorth)
    {
        handleCollision(i);
        globalBase.blueNorth = FALSE;
    }
    if(isBaseCollision(proj, southBlueBase, BLUE_BASE) && globalBase.blueSouth)
    {
        handleCollision(i);
        globalBase.blueSouth = FALSE;
    }

    /*Test red bases.*/
    if(isBaseCollision(proj, midRedBase, RED_BASE) && globalBase.redMid)
    {
        handleCollision(i);
        globalBase.redMid = FALSE;
    }
    if(isBaseCollision(proj, northRedBase, RED_BASE) && globalBase.redNorth)
    {
        handleCollision(i);
        globalBase.redNorth = FALSE;
    }
    if(isBaseCollision(proj, southRedBase, RED_BASE) && globalBase.redSouth)
    {
        handleCollision(i);
        globalBase.redSouth = FALSE;
    }
}




/*Check whether any collisions occurred.*/
void checkForCollisions()
{
    struct vertex center = {0.0, 0.0, 0.0};
  
    for(int i = 0; i < globalProjectile.pIndex; i++)
    {
        struct vertex projPos = globalProjectile.projectiles[i].position;

        checkBaseCollisions(globalProjectile.projectiles[i], i);
       
        /*Test for outer ring collision. Negate the collision result 
         *to get when the sum of the radii is less than the distance.*/
        if(!isCollision(projPos, center, RADIUS, RING_SIZE))
            handleCollision(i);

        /*Test sun collision.*/
        if(isCollision(projPos, center, RADIUS, RADIUS))
            handleCollision(i);

        /*Test for sail collision.*/
        if(isSailCollision(projPos, center))
            handleCollision(i);

        /*Test if this projectile colllided with another projectile.*/
        int collided = projectileCollided(projPos, i);
        if(collided != NO_COLLISION)
        {
            printf("Got a collision! Proj:%d\n", i);
            deleteProjectile(i);
            /*Subtract 1 as the previously deleted projectile
             *will move the array one step to the left.*/
            deleteProjectile(collided - 1);
            playCrashSound();
        }
    }
}
