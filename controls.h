#define ZOOM_STEP 0.01
#define SAIL_ROTATION_INC 3.0
#define RING_ROTATION_INC 1.0
#define INIT_ZOOM 1.0
#define INIT_TESSELATION 64
#define MAX_TESSELATION 300
#define MIN_TESSELATION 8
#define MIN_ZOOM 0.2
#define MAX_SPEED 6.0
#define MIN_SPEED 0.5
#define SPEED_INC 0.5

void initVariables();
void mouseMove(int, int);
void mouse(int, int, int, int);
void rotateCamera();
void tesselation(char);
void debugging();
void wireframe();
void animate();
void normals();
void splitScreen();
void shadows();
void updateAnimation(float);
void idle();
void shootARocket(char);
void endApplication();
void keyboard(unsigned char, int, int);
void keyboardSpecial(int, int, int);
int currentTesselation();
int debugNormals();
int debugAxis();
int overlapShadow();

/*Global variables for controlling
  the camera, mouse and debugging 
  features.*/
struct{
    int tesselation;
    int wireframe;
    int debugging;
    int animation;
    float animateRing;
    float animateSail;
    int slowDown;
    int speedUp;
    int showNormals;
    int overlappingShadows;
    int split;
    int windowID;
}global;

struct{
    float rotatex;
    float rotatey;
    float changex;
    float changey;
    float zoom;
    float zoomY;
    int updateRotation;
}cameraVar;

struct{
    int holdingLeft;
    int holdingRight;
}mouseVar;
