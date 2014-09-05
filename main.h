#include "texture.h"

#ifndef M_PI
#define M_PI (4.0 * atan(1.0))
#endif

#define MAX_AXIS_LEN 4.0
#define MIN_AXIS_LEN -4.0
#define NUM_OF_BASES 3
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

void display();
void firstPersonView(float, float, int);
void renderScene();
void drawSceneBases();
void drawSceneProjectiles();
void drawSceneWorld();
void initScores();
void initPlayerBases();
int playerWon();
void showTextDisplay(int);

struct{
    GLuint grassT;
    GLuint metalT;
    I3DImage *grass;
    I3DImage *metal;
}globalTexture;

struct{
    int red;
    int blue;
    int gameOver;
    int playerOneWon;
}globalScore;

struct{
    int redNorth;
    int redSouth;
    int redMid;
    int blueNorth;
    int blueSouth;
    int blueMid;
}globalBase;
