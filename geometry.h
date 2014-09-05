#ifndef GEOM_H
#define GEOM_H


#define N_SCALE_LRG_RING 1.3
#define N_SCALE_SML_RING 0.8
#define N_SCALE_SUN 0.3
#define N_SCALE_BASE 1.2
#define RING_V_SCALE 0.9
#define SAIL_SIZE 1.0
#define RING_SIZE 2.8
#define MAX_SHADOW_VERT 50
#define NUM_SHADOWS 4
#define NUM_SAILS 4
#define Z_POS 0.25
#define Z_NEG -0.25
#define SRANGE 0.3
#define STAR_NUM 400
#define BASE_SIZE 3.5
#define RADIUS 0.1
#define MID_BASE_X 2.6
#define MID_BASE_Y 0.0
#define SIDE_BASE_X 2.4
#define SIDE_BASE_Y 1.0
#define RED_BASE 'r'
#define BLUE_BASE 'b'


struct vertex{
    float x;
    float y;
    float z;
};


struct vertex2f{
    float x;
    float y;
};


struct{
    struct vertex *sun;
    struct vertex *sunNormals;
    struct vertex *sunNormalVis;
    int sunVertices;
    int sunNormalVisVerts;

    struct vertex *outerRing;
    struct vertex *outerRingNormals;
    struct vertex2f *outerRingTex;
    struct vertex2f *outerRingNormalVis;
    int outerRingVerts;

    struct vertex *innerRing;
    struct vertex *innerRingNormals;
    struct vertex2f *innerRingTex;
    struct vertex2f *innerRingNormalVis;
    int innerRingVerts;

    struct vertex *shadow;
    int shadowBreaks[3];
    int shadowVerts;
    
    int ringNormalVisVerts;

    struct vertex *bases;
    struct vertex *baseNormals;

    struct vertex *stars;
    int *starSize;
}globalGeometry;


struct vertex2f * storeRingNormalVertices(float, struct vertex2f *);
void drawRingNormals(float);
void addRingVertex(struct vertex *, float, float, struct vertex *, 
                   float, float, float);
void storeOuterRingVertices();
void drawRing(float, float[]);
void drawShadowOverlap();
void drawRingOutline(float, float, float);
void storeInnerRingVertices();
void drawInnerRing(float, float[]);
void storeSunVertices();
void storeSunNormalVertices();
void drawSphereFilled();
void drawSphereNormals(float);
void storeStarBackground();
void drawStarBackground();
void storeBase();
void drawBase(float[]);
void drawAxis(float, float, float);
void addSunVertex(float, float, float *, float *, float *,
                  float *, float *, float *, int *);
void calcCircularVertex(float, float, float *, float *, float *,
                        float *, float *, float *);
void freeMemory();
void calcGeometryVertices();


#endif
