#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "main.h"
#include "geometry.h"
#include "controls.h"
#include "lighting.h"
#include "colors.h"
#include "texture.h"
#include "util.h"
#include "projectile.h"
#include "sound.h"


int main(int argc, char **argv)
{
    /*Initialize and create window.*/
    glutInit(&argc, argv);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    global.windowID = glutCreateWindow("Assignment 2 - s3331571");

    /*Load images to be 'transformed' into textures.*/
    globalTexture.grass = loadImage("./Texture/grass.jpg");
    globalTexture.metal = loadImage("./Texture/metal.jpg");
    
    /*Load textures from the images.*/
    globalTexture.grassT = loadTexture(globalTexture.grass);
    globalTexture.metalT = loadTexture(globalTexture.metal);
    glEnable(GL_TEXTURE_2D);

    /*Initialize any variables.*/
    initVariables();
    initFiringVariables();
    initPlayerBases();
    initScores();
   
    glEnable(GL_DEPTH_TEST);
    
    /*Call helper method to initilize lighting.*/
    initLighting();   

    /*Setup Callbacks.*/
    glutDisplayFunc(display);
    glutMotionFunc(mouseMove);
    //glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutIdleFunc(idle);
    
    /*Give random function a seed.*/
    srand(time(NULL));

    /*Calculate initial vertices.*/
    storeStarBackground();
    calcGeometryVertices();
    
    setupAllMusic();

    glutMainLoop();
}




void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    
    /*If split view is activated.*/
    if(global.split)
    {
        /*Red*/
        firstPersonView(90.0, -90.0, 0);
        showTextDisplay(TRUE);
        /*Blue*/
        firstPersonView(-90.0, 90.0, 350);
    }
    else
    {
        glPushMatrix();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glOrtho(MIN_AXIS_LEN, MAX_AXIS_LEN, MIN_AXIS_LEN, 
                MAX_AXIS_LEN, MIN_AXIS_LEN, MAX_AXIS_LEN);
        renderScene();
        glPopMatrix();
    }
    glutSwapBuffers();
}




/*Setup a viewport of a first person view from a base.*/
void firstPersonView(float baseRotation, float starRotation, int leftY)
{
    glPushMatrix();
    
    glTranslatef(0.0, 0.0, -2.3);
    /*Rotate on x axis to get the flat ringworld.*/
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    /*Rotate on z to get to a base.*/
    glRotatef(baseRotation, 0.0, 0.0, 1.0);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 700.0f/350.0f, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, leftY, 700, 350);

    /*Draw the star background perspective.*/
    glPushMatrix();
    glRotatef(starRotation, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, -2.2);
    drawStarBackground();
    glPopMatrix();

    renderScene();
    glPopMatrix();
}




/*Render all the objects into the scene. With appropriate
 *transformations applied.*/
void renderScene()
{
    if(!global.split)
    {
        drawStarBackground();
        showTextDisplay(FALSE);
    }

    /*Save matrix for rotation before calculating
    how far to zoom in.*/
    glMatrixMode(GL_PROJECTION);
    glScalef(cameraVar.zoom, cameraVar.zoom, cameraVar.zoom);
    glMatrixMode(GL_MODELVIEW);

    if(!global.split)
        rotateCamera();
    
    drawSceneWorld();  
    drawSceneProjectiles();
    drawSceneBases();

    /*Print any errors that may pop up.*/
    int error = glGetError();
    if(error != GL_NO_ERROR)
    {
        printf("%s\n", gluErrorString(error));
        exit(1);
    }

    /*Reset projection matrix to default
     *so the text and star background does not
     *get scaled next iteration.*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}




/*Helper function to apply transformations
 *to each base without too much duplicate code.*/
void drawSceneBase(float color[COLOR_SIZE], float x, float y)
{
    glPushMatrix();
    glRotatef(global.animateRing, 0.0, 0.0, 1.0);
    glTranslatef(x, y, 0.0);
    drawBase(color);
    glPopMatrix();
}




/*Apply appropriate transformations and draw
 *the player bases.*/
void drawSceneBases()
{
    if(globalBase.redMid)
        drawSceneBase(red, -MID_BASE_X, 0.0);
    if(globalBase.redNorth)
        drawSceneBase(red, -SIDE_BASE_X, SIDE_BASE_Y);
    if(globalBase.redSouth)
        drawSceneBase(red, -SIDE_BASE_X, -SIDE_BASE_Y);

    if(globalBase.blueMid)
        drawSceneBase(blue, MID_BASE_X, 0.0);
    if(globalBase.blueNorth)
        drawSceneBase(blue, SIDE_BASE_X, SIDE_BASE_Y);
    if(globalBase.blueSouth)
        drawSceneBase(blue, SIDE_BASE_X, -SIDE_BASE_Y);
}




/*Apply transformations and draw projectiles
 *to the scene.*/
void drawSceneProjectiles()
{
    glPushMatrix();
    glRotatef(global.animateRing, 0.0, 0.0, 1.0);
    drawProjectiles();
    glPopMatrix();
}




/*Draw the rings and sun applying transformations.*/
void drawSceneWorld()
{
    /*Draw the sun.*/
    setSunMaterial();
    drawSphereFilled();

    /*Draw the rings.*/
    glPushMatrix();
    glRotatef(global.animateRing, 0.0, 0.0, 1.0);
    setRingWorldMaterial();
    drawRing(RING_SIZE, blue);
    setOpenSailMaterial();
    drawInnerRing(SAIL_SIZE, light_grey);
    glPopMatrix();
}




/*Init player scores.*/
void initScores()
{
    globalScore.red = 0;
    globalScore.blue = 0;
}




/*Initialize the number of player bases.*/
void initPlayerBases()
{
    globalScore.playerOneWon = FALSE;
    globalScore.gameOver = FALSE;

    /*Set a boolean testing whether a base is alive or not.*/
    globalBase.redNorth = TRUE;
    globalBase.redSouth = TRUE;
    globalBase.redMid = TRUE;
    globalBase.blueNorth = TRUE;
    globalBase.blueSouth = TRUE;
    globalBase.blueMid = TRUE;
}




/*Test whether a player won by checking which
 *bases are still alive.*/
int playerWon()
{
    /*The gameOver variable is used so the score only updates once rather
     *than continually until the user hits the restart button.*/

    /*Red lost.*/
    if(!globalBase.redNorth && !globalBase.redSouth && !globalBase.redMid)
    {
        if(globalScore.gameOver == FALSE)
        {
            globalScore.blue++;
            globalScore.gameOver = TRUE;
            globalScore.playerOneWon = FALSE;
        }
        return TRUE;
    }

    /*Blue lost.*/
    if(!globalBase.blueNorth && !globalBase.blueSouth && !globalBase.blueMid)
    {
        if(globalScore.gameOver == FALSE)
        {
            globalScore.red++;
            globalScore.gameOver = TRUE;
            globalScore.playerOneWon = TRUE;
        }
        return TRUE;
    }
    
    return FALSE;
}




/*Show the text display in either of the views.*/
void showTextDisplay(int firstPerson)
{
    glPushMatrix();
    
    glLoadIdentity();

    if(!firstPerson)
        glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, MIN_AXIS_LEN, MAX_AXIS_LEN);
    else
        glOrtho(0, WINDOW_WIDTH / 2, 0, WINDOW_HEIGHT / 2, MIN_AXIS_LEN, MAX_AXIS_LEN);
    
    if(playerWon())
        drawTextPlayerWon();
    else
        drawTextToScreen();
    
    glPopMatrix();
}
