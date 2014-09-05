/*************************************************/
/*This file contains code for mouse movement     */
/*and toggling different views.                  */
/*************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "controls.h"
#include "geometry.h"
#include "util.h"
#include "main.h"
#include "projectile.h"
#include "sound.h"
#include "texture.h"



/*Initialize the variables for the start of the application.*/
void initVariables()
{
    global.tesselation = INIT_TESSELATION;
    global.wireframe = FALSE;
    global.debugging = FALSE;
    global.animation = FALSE;
    global.speedUp = FALSE;
    global.slowDown = FALSE;
    cameraVar.updateRotation = FALSE;
    cameraVar.zoom = INIT_ZOOM;
    global.animateRing = 0;
    global.animateSail = 0;
    global.showNormals = FALSE;
    global.overlappingShadows = TRUE;
    global.split = FALSE;
}




/*Determines what to do with the data gathered from moving the mouse 
 *in certain directions whilst holding a corresponding button. The button
 *which is being held is discovered via the function 'mouse'.*/
void mouseMove(int x, int y)
{
    if(mouseVar.holdingLeft)
    {
        #if 0
        printf("x:%d, y:%d\n", x, y);
        printf("Before: changex:%f, changey:%f, rotatex:%f, rotatey:%f\n", 
                changex, changey, rotatex, rotatey);
        #endif
        cameraVar.changex = x - cameraVar.rotatex;
        cameraVar.rotatex += cameraVar.changex;
        cameraVar.changey = y - cameraVar.rotatey;
        cameraVar.rotatey += cameraVar.changey;
        #if 0
        printf("After: changex:%f, changey:%f, rotatex:%f, rotatey:%f\n", 
                changex, changey, rotatex, rotatey);
        #endif
        cameraVar.updateRotation = TRUE;
        glutPostRedisplay();
    }
    else if(mouseVar.holdingRight)
    {
        #if 0
        printf("Zoom variable on z axis: %f\n", cameraVar.zoom);
        #endif 
        /*y is decreasing as lower values are at the top of the window
         *and greater ones at the bottom of the window.*/
        if(y < cameraVar.zoomY)
        {
            cameraVar.zoom  += ZOOM_STEP;
            if(cameraVar.zoom > MAX_AXIS_LEN){
                printf("Reached max zoom.\n");
                cameraVar.zoom = MAX_AXIS_LEN;
            }
            #if 0
            printf("Zoom IN: zoom=%f  zoomY=%f  y=%d\n", 
                    cameraVar.zoom, cameraVar.zoomY, y);
            #endif
        }
        else if(y > cameraVar.zoomY)
        {
            cameraVar.zoom -= ZOOM_STEP;
            /*0 is the smallest it can go as scaling via a negative number will
             *still enlarge the world but it will be 'upside down' when compared
             *to scaling via a positive factor.*/
            if(cameraVar.zoom < MIN_ZOOM){
                printf("Reached min zoom.\n");
                cameraVar.zoom = MIN_ZOOM;
            }
            #if 0
            printf("Zoom OUT: zoom=%f  zoomY=%f  y=%d\n", cameraVar.zoom, cameraVar.zoomY, y);
            #endif
        }
        cameraVar.zoomY = y;
        glutPostRedisplay();
    }
}




/*Function for determining which mouse button is being 
 *held or released(left or right mouse button)*/
void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN) 
    {    
        if(button == GLUT_LEFT_BUTTON){
            mouseVar.holdingLeft = TRUE;
            cameraVar.rotatex = x;
            cameraVar.rotatey = y;
        }
        else if(button == GLUT_RIGHT_BUTTON){
            mouseVar.holdingRight = TRUE;
            cameraVar.zoomY = y;
        }
    }
    else if(state == GLUT_UP)
    {
        if(button == GLUT_LEFT_BUTTON){
            mouseVar.holdingLeft = FALSE;
            cameraVar.rotatex = x;
            cameraVar.rotatey = y;
        }
        else if(button == GLUT_RIGHT_BUTTON){
            mouseVar.holdingRight = FALSE;
            cameraVar.zoomY = y;
        }
    }
}




/*Rotate the camera if the user has interacted with the scene
 *and let it be known that this rotation occured so it does
 *not continue to rotate.*/
void rotateCamera()
{
    #if 0
    printf("Before Rotate\n");
    printf("Change in x:%f  Change in y:%f\n",
            cameraVar.changex, cameraVar.changey);
    #endif

    if(cameraVar.updateRotation)
    {
        glRotatef(cameraVar.changey, 1.0, 0.0, 0.0);
        glRotatef(cameraVar.changex, 0.0, 1.0, 0.0);
        cameraVar.updateRotation = FALSE;
    }
}




/*Handle tesselation.
 *Pass '+' to increase. 
 *Pass '-' to decrease.*/
void tesselation(char sign)
{
    if(sign == '+')
    {
        //if(global.tesselation < INIT_TESSELATION) 
        global.tesselation *= 2;
    }
    else if(sign == '-')
    {
        if(global.tesselation > MIN_TESSELATION)
            global.tesselation /= 2;
    }
    calcGeometryVertices();
    glutPostRedisplay();
}




/*Turn debugging on or off.*/
void debugging()
{
    if(global.debugging)
        global.debugging = FALSE;
    else
        global.debugging = TRUE;
    glutPostRedisplay();
}




/*Handle switching wireframe on and off.*/
void wireframe()
{
    if(global.wireframe)
    {
        global.wireframe = FALSE;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else 
    {
        global.wireframe = TRUE;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glutPostRedisplay();
}




/*Update the global animation variable so other functions
 *can determine whether to update and continue the animation.*/
void animate()
{
    if(global.animation)
        global.animation = FALSE;
    else
        global.animation = TRUE;
}




/*Toggle normals on and off*/
void normals()
{
    if(global.showNormals)
        global.showNormals = FALSE;
    else
        global.showNormals = TRUE;
}




/*Toggle split screen on and off.*/
void splitScreen()
{
    if(global.split)
        global.split = FALSE;
    else
        global.split = TRUE;
}




/*Toggle shadowing methods.*/
void shadows()
{
    if(global.overlappingShadows)
        global.overlappingShadows = FALSE;
    else
        global.overlappingShadows = TRUE;
}




/*Update the angle at which to rotate along the x axis
 *for the ring and sail animation.*/
void updateAnimation(float deltaTime)
{
    static float speed = MIN_SPEED;
    #if 0
    printf("Speed:%f   TimeChange:%f\n", speed, deltaTime);
    #endif
    if(global.animation)
    {
        if(global.slowDown)
        {
            speed -= SPEED_INC;
            /*Prevent our object from rotating
             *in the opposite direction via capping at 0.*/
            if(speed <= MIN_SPEED)
                speed = MIN_SPEED;
            global.slowDown = FALSE;
        }
        else if(global.speedUp)
        {
            speed += SPEED_INC;
            /*Cap for similar reasons as above and theres no
             *need to rotate ridiculously fast :P*/
            if(speed > MAX_SPEED)
                speed = MAX_SPEED;
            global.speedUp = FALSE;
        }
        global.animateSail += SAIL_ROTATION_INC * (speed * deltaTime);
        global.animateRing += RING_ROTATION_INC * (speed * deltaTime);
    }
}




/*Idle function to update animation.*/
void idle()
{
    float t, deltaT;
    static float tLast = 0.0;

    /*Get the elapsed time and convert
     it from milliseconds to seconds.*/
    t = glutGet(GLUT_ELAPSED_TIME);
    t /= 1000.0;
    
    /*Calculate change in time.*/
    deltaT = t-tLast;
    
    updateMeasures(deltaT);
    updateAnimation(deltaT);
    updateProjectilePos(deltaT);
    checkForCollisions();

    tLast = t;
    glutPostRedisplay();
}




/*When called frees memory and closes
 *the application.*/
void endApplication()
{
    stopAllMusic();
    freeImage(globalTexture.grass);
    freeImage(globalTexture.metal);
    freeMemory();
    glutDestroyWindow(global.windowID);
    exit(0);
}




/*Create the projectile and play a sound.*/
void shootARocket(char team)
{
    glutIgnoreKeyRepeat(TRUE);
    if(team == RED_BASE)
    {
        createProjectile(team);
            
    }
    else if(team == BLUE_BASE)
    {
        createProjectile(team);
    }
    playRocketSound();
}




/*Keyboard commands for user interaction.*/
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '+':
            tesselation('+');
            break;
        case '-':
            tesselation('-');
            break;
        case '1':
            debugging();
            break;
        case '2':
            normals();
            break;
        case '3':
            wireframe();
            break;
        case '4':
            splitScreen();
            break;
        case 'a':
            glutIgnoreKeyRepeat(FALSE);
            rotateRedLeft();
            break;
        case 'd':
            glutIgnoreKeyRepeat(FALSE);
            rotateRedRight();;
            break;
        case ' ':
            shootARocket(RED_BASE);
            break;
        case '/':
            shootARocket(BLUE_BASE);
            break;
        #if 0
        case 'r':
            animate();
            break;
        case 'S':
            global.speedUp = TRUE;
            break;
        case 's':
            global.slowDown = TRUE;
            break;
        #endif
        case 'R':
            initPlayerBases(); //restart the game.
            break;
        case 27: //escape key
            endApplication();
            break;
        default:
            break;
    }
}




/*Keyboard callback to detect the arrow keys.*/
void keyboardSpecial(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            glutIgnoreKeyRepeat(FALSE);
            rotateBlueLeft();
            break;
        case GLUT_KEY_RIGHT:
            glutIgnoreKeyRepeat(FALSE);
            rotateBlueRight();
            break;
        default:
            break;
    }
}




/*The following functions return the only globals
  which should be accessable by other files for
  the application.*/
int currentTesselation(){return global.tesselation;}
int debugNormals(){return global.showNormals;}
int debugAxis(){return global.debugging;}
int overlapShadow(){return global.overlappingShadows;}

