#include "util.h"
#include "controls.h"
#include "colors.h"
#include "geometry.h"
#include "main.h"
#include <stdio.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>


/*Call the updateFrametime() and updateFramerate()
 *functions. Expects the currentTime in seconds.*/
void updateMeasures(float deltaTime)
{
    updateFrametime(deltaTime);
    updateFramerate();
}




/*Get the time taken to draw a single frame.*/
void updateFrametime(float deltaTime)
{
    static float interval = 0.0;
    
    interval += deltaTime;

    if(interval >= 1.0)
    {
        /*Get it back into ms.*/
        globalUtility.frametime = deltaTime * 1000;
        interval = 0.0;
    }
}




/*Get how many frames are drawn per second.*/
void updateFramerate()
{
    /*Have to convert the frametime to seconds.*/
    globalUtility.framerate = 1.0 / (globalUtility.frametime / 1000.0);
}




/*Draw the text to screen for performance measures.*/
void drawTextToScreen()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    char buffer[BUFFER_LEN];
    glColor3fv(yellow);
        
    glRasterPos2i(2, 90);
    snprintf(buffer, BUFFER_LEN, "Framerate: %.2f fps", globalUtility.framerate);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);
 
    glRasterPos2i(2, 70);
    snprintf(buffer, BUFFER_LEN, "Frametime: %.2f ms", globalUtility.frametime);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);

    glRasterPos2i(2, 50);
    snprintf(buffer, BUFFER_LEN, "Tesselation: %d", global.tesselation);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);

    glRasterPos2i(2, 30);
    snprintf(buffer, BUFFER_LEN, "Red:%-3dBlue:%d", 
            globalScore.red, globalScore.blue);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}




/*Print a message notifying which player won the game.*/
void drawTextPlayerWon()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    char buffer[BUFFER_LEN];
    glColor3fv(yellow);

    glRasterPos2i(2, 90);
    
    /*Display which player won.*/
    if(globalScore.playerOneWon)
        snprintf(buffer, BUFFER_LEN, "Winner is player 1 (red)");
    else
        snprintf(buffer, BUFFER_LEN, "Winner is player 2 (blue)");
    
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);
 
    /*Display a message notifying the players how to restart the game.*/
    glRasterPos2i(2, 70);
    snprintf(buffer, BUFFER_LEN, "Hit 'R' to restart the game.");
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned char *) buffer);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}




/*Find the distance between two vectors.*/
float distance(struct vertex v1, struct vertex v2)
{
    struct vertex between = subtract(v1, v2);
    return length(between.x, between.y, between.z);
}




/*Find a vector between two others.*/
struct vertex subtract(struct vertex v1, struct vertex v2)
{
    struct vertex new;
    
    new.x = v1.x - v2.x;
    new.y = v1.y - v2.y;
    new.z = v1.z - v2.z;

    return new;
}




/*Find the length of a vector.*/
float length(float x, float y, float z)
{
    return sqrt(x*x + y*y + z*z);   
}




/*Get the length of a vector with x and y points.*/
float getVectorLength2f(float x, float y)
{
    return sqrtf((x * x) + (y * y));
}




/*Get the length of a vector with x, y and z points.*/
float getVectorLength3f(float x, float y, float z)
{
    return sqrtf((x * x) + (y * y) + (z * z));
}




/*Normalize a set of vector coordinates consisting of x and y.*/
void normalizeVectors2f(float *x, float *y, float length)
{
    *x /= length;
    *y /= length;
}




/*Normalize a set of vector coordinates consisting of x, y and z.*/
void normalizeVectors3f(float *x, float *y, float *z, float length)
{
    normalizeVectors2f(x,  y, length);
    *z /= length;
}




/*Function to check whether the dynamic memory allocation         
 *succeeded. If so assign realPointer the address.*/ 
struct vertex * memoryCheck(struct vertex *check)
{   
    if(check == NULL)
    {
        printf("Unable to allocate memory.\n");   
        exit(1);
    }
    else
        return check;
}




/*Function to check whether the dynamic memory allocation         
 *succeeded. If so assign realPointer the address. For 
 *vertex2f structures.*/ 
struct vertex2f * memoryCheck2f(struct vertex2f *check)
{   
    if(check == NULL)
    {
        printf("Unable to allocate memory.\n");   
        exit(1);
    }
    else
        return check;
}
