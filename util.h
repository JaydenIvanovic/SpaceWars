#include "geometry.h"

#define BUFFER_LEN 40

void updateMeasures(float);
void updateFrametime(float);
void updateFramerate();
void drawTextToScreen();
void drawTextPlayerWon();
float distance(struct vertex, struct vertex);
struct vertex subtract(struct vertex, struct vertex);
float length(float, float, float);
float getVectorLength2f(float, float);
float getVectorLength3f(float, float, float);
void normalizeVectors2f(float *, float *, float);
void normalizeVectors3f(float *, float *, float *, float);
struct vertex * memoryCheck(struct vertex *);
struct vertex2f * memoryCheck2f(struct vertex2f *);

struct{
    float frametime;
    float framerate;
}globalUtility;
