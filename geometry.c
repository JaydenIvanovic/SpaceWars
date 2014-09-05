/***************************************************/
/*This file contains the code used to draw the     */
/*geometry and show the normals.                   */
/***************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "main.h"
#include "geometry.h"
#include "controls.h"
#include "colors.h"
#include "lighting.h"
#include "util.h"


/*Draws a set of axi at the specified location.
 *Red = x  Green = y  Blue = z*/
void drawAxis(float x, float y, float z)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glColor3fv(red);
    glVertex3f(x, y, z);
    glVertex3f(x + 0.8, y, z);
    
    glColor3fv(green);
    glVertex3f(x, y, z);
    glVertex3f(x, y + 0.8, z);
    
    glColor3fv(blue);
    glVertex3f(x, y, z);
    glVertex3f(x, y, z + 0.8);
    
    glEnd();
    glEnable(GL_LIGHTING);
}




/*Store the vertices needed for normal visualizations
  *on a ring of a particular size.*/
struct vertex2f * storeRingNormalVertices(float size, struct vertex2f *ring)
{

    /*Plus 1 because the loop below is inclusive. e.g. <=*/
    /*Multiply by 2 as 2 vertices are stored each iteration.*/
    globalGeometry.ringNormalVisVerts = (currentTesselation() + 1) * 2; 
    struct vertex2f *checkRealloc;
    checkRealloc = realloc(ring, globalGeometry.ringNormalVisVerts * sizeof(struct vertex2f));
    ring = memoryCheck2f(checkRealloc);

    int vcount = 0;
    for(float i = 0; i <= currentTesselation(); i++)
    {
        float theta = (i/currentTesselation()) * (2.0 * M_PI);
        float x = size * cos(theta);
        float y = size * sin(theta);
        float normalx = -x;
        float normaly = -y;
        normalizeVectors2f(&normalx, &normaly, getVectorLength2f(normalx, normaly));

        ring[vcount].x = x;
        ring[vcount].y = y;
        vcount++;

        if(size == (float)RING_SIZE)
        {
            /*Drawing the large rings normals(need to know for scaling)*/
            ring[vcount].x = (normalx + x) * N_SCALE_LRG_RING;
            ring[vcount].y = (normaly + y) * N_SCALE_LRG_RING;
        }
        else
        {
            /*The normal vector will be pointing at <0,0>
              and therefore we can not scale it like the above.*/
            ring[vcount].x =  x * N_SCALE_SML_RING;
            ring[vcount].y = y * N_SCALE_SML_RING;
        }
        vcount++;
    }
    return ring;
}




/*Draws normals for the ring, given its size
 *e.g. Inner or Outer ring.*/
void drawRingNormals(float size)
{
    glDisable(GL_LIGHTING);
    glColor3fv(yellow);
    if(size == SAIL_SIZE)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, globalGeometry.innerRingNormalVis);
        glDrawArrays(GL_LINES, 0, globalGeometry.ringNormalVisVerts);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else if(size == (float)RING_SIZE)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, globalGeometry.outerRingNormalVis);
        glDrawArrays(GL_LINES, 0, globalGeometry.ringNormalVisVerts);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glEnable(GL_LIGHTING);
}




/*Encapsulate common functionality for the 
 *functions which store ring vertices.*/
void addRingVertex(struct vertex *v, float x, float y, 
                   struct vertex *n, float normalx, float normaly, 
                   float z)
{
    /*Store the normals.*/
    n->x = normalx + x; 
    n->y = normaly + y;
    n->z = z;
    
    /*Store the vertices.*/
    if(z == Z_NEG)
    {
        v->x = x * RING_V_SCALE;
        v->y = y * RING_V_SCALE;
    }
    else
    {
        v->x = x;
        v->y = y;
    }
    v->z = z;
}




/*Store the vertices needed to draw the planetary ring.*/
void storeOuterRingVertices()
{
    struct vertex2f *checkRealloc2f;
    struct vertex *checkRealloc;

    /*2 vertices per iteration.*/
    globalGeometry.outerRingVerts = (currentTesselation() + 1) * 2;

    /*Assign memory for textures.*/
    checkRealloc2f = realloc(globalGeometry.outerRingTex, 
                             globalGeometry.outerRingVerts * sizeof(struct vertex2f));
    globalGeometry.outerRingTex = memoryCheck2f(checkRealloc2f);

    /*Assign memory for normals.*/
    checkRealloc = realloc(globalGeometry.outerRingNormals,
                           globalGeometry.outerRingVerts * sizeof(struct vertex));
    globalGeometry.outerRingNormals = memoryCheck(checkRealloc);

    /*Assign memory for vertices.*/
    checkRealloc = realloc(globalGeometry.outerRing,
                           globalGeometry.outerRingVerts * sizeof(struct vertex));
    globalGeometry.outerRing = memoryCheck(checkRealloc);

    int vcount = 0;
    /*Calculate and store the respective vertices.*/
    for(float i = 0; i <= currentTesselation(); i++)
    {
        float theta = (i/currentTesselation()) * (2.0 * M_PI);
        float x = RING_SIZE * cos(theta);
        float y = RING_SIZE * sin(theta);
        float normalx = -x;
        float normaly = -y;
        
        normalizeVectors2f(&normalx, &normaly, getVectorLength2f(normalx, normaly));
        
        //printf("u:%f, v:%f\n", i/currentTesselation() * 6, 6.0);
        globalGeometry.outerRingTex[vcount].x = i/currentTesselation() * 6;
        globalGeometry.outerRingTex[vcount].y = 1.0;

        addRingVertex(&globalGeometry.outerRing[vcount], x , y,
                &globalGeometry.outerRingNormals[vcount], normalx, normaly,
                Z_NEG);
        
        vcount++;
  
        //printf("u:%f, v:%f\n", i/currentTesselation() * 6, 0.0);
        globalGeometry.outerRingTex[vcount].x = i/currentTesselation() * 6; 
        globalGeometry.outerRingTex[vcount].y = 0.0;

        addRingVertex(&globalGeometry.outerRing[vcount], x , y,
                &globalGeometry.outerRingNormals[vcount], normalx, normaly,
                Z_POS);
        
        vcount++;
    }
}




/*Draw a ring. Initially was going to be used
  to draw both rings but now is used only for
  the outer ring.*/
void drawRing(float size, float color[3])
{
    /*Texture calls.*/
    glBindTexture(GL_TEXTURE_2D, globalTexture.grassT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    /*Smooth texture.*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    /*Draw vertices.*/
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glTexCoordPointer(2, GL_FLOAT, 0, globalGeometry.outerRingTex);
    glNormalPointer(GL_FLOAT, 0, globalGeometry.outerRingNormals);
    glVertexPointer(3, GL_FLOAT, 0, globalGeometry.outerRing);

    glDrawArrays(GL_QUAD_STRIP, 0, globalGeometry.outerRingVerts);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /*Draw the normals if toggled.*/
    if(debugNormals())
        drawRingNormals(size);

    /*Draw axis if in debug mode.*/
    if(debugAxis())
    {
        drawAxis(globalGeometry.outerRing[0].x,
                 globalGeometry.outerRing[0].y,
                 globalGeometry.outerRing[0].z);
    }
}




/*Takes a 2D array consisting of the data needed to draw the shadows.
 *This function scales the vertices to project a shadow on the outer
 *ring.*/
void drawShadowOverlap()
{
    /*Add transparency.*/
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.2, 0.2, 0.2, 0.7);
    
    /*Loop through each set of shadows via checking at what count a break 
     *should occur. Call glEnd() so QUAD_STRIP does not link all the 
     *shadows together. Offset by 0.01, to combat z fighting. startfrom
     *is a 'memory' variable to remind the inner for loop where it last 
     *broke and where it should start from again.*/
    int startfrom = 0;
    for(int j = 0; j < NUM_SHADOWS; j++)
    {
        glBegin(GL_QUAD_STRIP);
        for(int i = startfrom; i < globalGeometry.shadowBreaks[j]; i++, startfrom++)
        {
            /*Scale our precalculated geometry to apply the shadow on 
             *outer ring.*/
            float x = globalGeometry.shadow[i].x * (RING_SIZE - 0.01);
            float y = globalGeometry.shadow[i].y * (RING_SIZE - 0.01);
            float z = globalGeometry.shadow[i].z;
            glVertex3f(x, y, z);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
    
    /*Draw the start point of the shadows.*/
    if(debugAxis())
    {
        drawAxis(globalGeometry.shadow[0].x * (RING_SIZE - 0.01), 
                 globalGeometry.shadow[0].y * (RING_SIZE - 0.01), 
                 globalGeometry.shadow[0].z);       
    }
    glEnable(GL_LIGHTING);
}




/*Store the vertices needed to construct the inner ring.*/
void storeInnerRingVertices()
{
    struct vertex2f *checkRealloc2f;
    struct vertex *checkRealloc;

    /*2 vertices per iteration.*/
    globalGeometry.innerRingVerts = (currentTesselation() + 1) * 2;
    globalGeometry.shadowVerts = 0;

    /*Assign memory for textures.*/
    checkRealloc2f = realloc(globalGeometry.innerRingTex, 
                             globalGeometry.innerRingVerts * sizeof(struct vertex2f));
    globalGeometry.innerRingTex = memoryCheck2f(checkRealloc2f);

    /*Assign memory for normals.*/
    checkRealloc = realloc(globalGeometry.innerRingNormals,
                           globalGeometry.innerRingVerts * sizeof(struct vertex));
    globalGeometry.innerRingNormals = memoryCheck(checkRealloc);

    /*Assign memory for vertices.*/
    checkRealloc = realloc(globalGeometry.innerRing,
                           globalGeometry.innerRingVerts * sizeof(struct vertex));
    globalGeometry.innerRing = memoryCheck(checkRealloc);

    /*Assign memory for vertices.*/
    checkRealloc = realloc(globalGeometry.shadow,
                           globalGeometry.innerRingVerts * sizeof(struct vertex));
    globalGeometry.shadow = memoryCheck(checkRealloc);

    int check, vcount, breaks;
    check = vcount = breaks = 0;
    float splitNum = currentTesselation() / 8; /*Divide into 8 pieces.*/
    float tessel = currentTesselation() / NUM_SAILS; // Divide into the four sails.

    #if 1
    for(float i = 0; i <= currentTesselation(); i++)
    {
        /*Determine whether to draw a shade sail
         *or not.*/
        if( (int)i % (int)splitNum == 0)
        {
            /*Open sail.*/
            if(check == 0)
            {
                check = 1;
                if((int)i != 0)
                {
                    /*Record where the shadow should be broken for later use.*/
                    globalGeometry.shadowBreaks[breaks] = globalGeometry.shadowVerts;
                    breaks++;
                }
            }
            /*Closed sail.*/
            else
                check = 0;
        }

        float theta = (i/currentTesselation()) * (2.0 * M_PI);
        float x = SAIL_SIZE * cos(theta);
        float y = SAIL_SIZE * sin(theta);
     
        /*Perform vector operations to get normals and draw 
         additional vertices.*/
        float normalx = -x;
        float normaly = -y;
        normalizeVectors2f(&normalx, &normaly, getVectorLength2f(normalx, normaly));
        
        /*Get the first vertex on the negative z axis' normals
         *and vertex coordinates.*/
        addRingVertex(&globalGeometry.innerRing[vcount], x , y,
                &globalGeometry.innerRingNormals[vcount], normalx, normaly,
                Z_NEG);

        vcount++;

        /*Get the second vertex on the positive z axis' normals
         *and vertex coordinates.*/
        addRingVertex(&globalGeometry.innerRing[vcount], x , y,
                &globalGeometry.innerRingNormals[vcount], normalx, normaly,
                Z_POS);

        vcount++;

        /*If were currently drawing a shadow we save the vertices
         *for the function which draws overlapping shadows and for
         *drawing the closed sails.*/
        if(check == 0)
        {
            globalGeometry.shadow[globalGeometry.shadowVerts].x = x * RING_V_SCALE;
            globalGeometry.shadow[globalGeometry.shadowVerts].y = y * RING_V_SCALE;
            globalGeometry.shadow[globalGeometry.shadowVerts].z = Z_NEG;    
            
            globalGeometry.innerRingTex[globalGeometry.shadowVerts].x = i / tessel;
            globalGeometry.innerRingTex[globalGeometry.shadowVerts].y = 1.0;

            globalGeometry.shadowVerts++;
            
            globalGeometry.shadow[globalGeometry.shadowVerts].x = x;
            globalGeometry.shadow[globalGeometry.shadowVerts].y = y;
            globalGeometry.shadow[globalGeometry.shadowVerts].z = Z_POS;

            globalGeometry.innerRingTex[globalGeometry.shadowVerts].x = i / tessel;
            globalGeometry.innerRingTex[globalGeometry.shadowVerts].y = 0.0;

            globalGeometry.shadowVerts++;
        }
    }
    #endif
}




/*Draw a ring. With sails. (Only draws the sails
 *to make first person view better to look at)*/
void drawInnerRing(float size, float color[3])
{
    setOpenSailMaterial();
    
    #if 1
    glBindTexture(GL_TEXTURE_2D, globalTexture.metalT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    #endif
     
    int startfrom = 0; 

    /*Closed sails outside of the ring.*/
    for(int j = 0; j < NUM_SHADOWS; j++)
    {
        int count = 0;
        
        glBegin(GL_QUAD_STRIP);
        for(int i = startfrom; i < globalGeometry.shadowBreaks[j]; i++, startfrom++)
        {
            glTexCoord2fv((float *)&globalGeometry.innerRingTex[count]);
            count++;
            glNormal3fv((float *)&globalGeometry.innerRingNormals[i]);
            glVertex3fv((float *)&globalGeometry.shadow[i]);
        }
        glEnd();
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    /*Draw the normals if toggled.*/
    if(debugNormals())
        drawRingNormals(size);

    /*Draw axis if in debug mode.*/
    if(debugAxis())
    {
        drawAxis(globalGeometry.shadow[0].x,
                 globalGeometry.shadow[0].y,
                 globalGeometry.shadow[0].z);
    }

    /*Draw a shadow using the gathered vertices.*/
    drawShadowOverlap();
}




/*Store the vertices that are required to draw the sun
 *in regards to the current tesselation level.*/
void storeSunVertices()
{
    /*We get two vertices at each iteration
     *and we iterate for 'tesselation' number of times.*/
    globalGeometry.sunVertices = (currentTesselation() + 1) * (currentTesselation() + 1) * 6;

    /*Construct another pointer to ensure that we only
     *store the new pointer returned by realloc if it
     *is not null.*/
    struct vertex *checkRealloc;
    checkRealloc = realloc(globalGeometry.sun, 
                           globalGeometry.sunVertices * sizeof(struct vertex));
    globalGeometry.sun = memoryCheck(checkRealloc);
    checkRealloc = realloc(globalGeometry.sunNormals, 
                           globalGeometry.sunVertices * sizeof(struct vertex));
    globalGeometry.sunNormals = memoryCheck(checkRealloc);

    /*Init variables.*/
    float i, j, theta, phi;
    float x, y, z, normalx, normaly, normalz;
    x = y = z = normalx = normaly = normalz = 0;
    float step_theta = 2.0 * M_PI / currentTesselation();
    float step_phi = M_PI / currentTesselation();

    /*Count for the vertices. (The vertex
     *structures for the sun).*/
    int vcount = 0;
    
    /*Iterate through the loop and store the vertices
     *into the dynamically created array.*/
    for(i = 0; i <= currentTesselation(); i++)
    {
        phi = i / (float)currentTesselation() * M_PI;
        for(j = 0; j <= currentTesselation(); j++)
        {
            theta = (j/currentTesselation()) * (2.0 * M_PI);
            
            addSunVertex(theta, phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;
            
            addSunVertex(theta + step_theta, phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;

            addSunVertex(theta + step_theta, phi + step_phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;

            addSunVertex(theta, phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;

            addSunVertex(theta + step_theta, phi + step_phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;
            
            addSunVertex(theta, phi + step_phi, &x, &y, &z, 
                    &normalx, &normaly, &normalz, &vcount);
            vcount++;
        }
    }
}




/*Store the vertices that are required to draw the 
 *normals around the sun for debugging and visualization
 *purposes.Should be called after storeSunVertices().*/
void storeSunNormalVertices()
{
    /*Multiply by 4 as we get 4 vertices each inner iteration of the loop below.*/
    globalGeometry.sunNormalVisVerts = (currentTesselation() + 1) * (currentTesselation() + 1) * 4;

    struct vertex *checkRealloc;
    checkRealloc = realloc(globalGeometry.sunNormalVis, 
                           globalGeometry.sunNormalVisVerts * sizeof(struct vertex));
    globalGeometry.sunNormalVis = memoryCheck(checkRealloc);
  
    float i, j, theta, theta2, phi;
    float x, y, z, normalx, normaly, normalz;
    x = y = z = normalx = normaly = normalz = 0;
    int vcount = 0;
    
    for(i = 0; i <= currentTesselation(); i++)
    {
        phi = i / (float)currentTesselation() * M_PI;
        for(j = 0; j <= currentTesselation(); j++)
        {
            theta = (j/currentTesselation()) * (2.0 * M_PI);
            calcCircularVertex(theta, phi, &x, &y, &z, 
                               &normalx, &normaly, &normalz);     
            globalGeometry.sunNormalVis[vcount].x = x;
            globalGeometry.sunNormalVis[vcount].y = y;
            globalGeometry.sunNormalVis[vcount].z = z;
            vcount++;
            globalGeometry.sunNormalVis[vcount].x = (normalx + x) * N_SCALE_SUN;
            globalGeometry.sunNormalVis[vcount].y = (normaly + y) * N_SCALE_SUN;
            globalGeometry.sunNormalVis[vcount].z = (normalz + z) * N_SCALE_SUN;
            vcount++;

            theta2 = ((i + 1.0)/ currentTesselation()) * (2.0 * M_PI);
            calcCircularVertex(theta2, phi, &x, &y, &z, 
                               &normalx, &normaly, &normalz);      
            globalGeometry.sunNormalVis[vcount].x = x;
            globalGeometry.sunNormalVis[vcount].y = y;
            globalGeometry.sunNormalVis[vcount].z = z;
            vcount++;
            globalGeometry.sunNormalVis[vcount].x = (normalx + x) * N_SCALE_SUN;
            globalGeometry.sunNormalVis[vcount].y = (normaly + y) * N_SCALE_SUN;
            globalGeometry.sunNormalVis[vcount].z = (normalz + z) * N_SCALE_SUN;
            vcount++;
        }
    }
}




/*Draw the sun into the scene.*/
void drawSphereFilled()
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, globalGeometry.sunNormals);
    glVertexPointer(3, GL_FLOAT, 0, globalGeometry.sun);

    glDrawArrays(GL_TRIANGLES, 0, globalGeometry.sunVertices);
    
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /*Draw the normal visualizations if toggled.*/
    if(debugNormals())
        drawSphereNormals(1.0);

    /*Draw axis if in debug mode.*/
    if(debugAxis())
        drawAxis(0.0, 0.0, 0.0);
}




/*Draw the debugging visualizations of the normals
 *for the sphere/sun.*/
void drawSphereNormals(float scale)
{
    glDisable(GL_LIGHTING);
    glColor3fv(yellow);
    
    glBegin(GL_LINES);
    for(int i = 0; i < globalGeometry.sunNormalVisVerts; i++)
    {
        glVertex3f(globalGeometry.sunNormalVis[i].x * scale,
                globalGeometry.sunNormalVis[i].y * scale,
                globalGeometry.sunNormalVis[i].z * scale);
    }
    glEnd();
    
    glEnable(GL_LIGHTING);
}




/*Store the background of GL_POINTS.*/
void storeStarBackground()
{
    struct vertex *checkRealloc;
    checkRealloc = realloc(globalGeometry.stars, STAR_NUM * sizeof(struct vertex));
    globalGeometry.stars = memoryCheck(checkRealloc);
    
    int *checkInt;
    checkInt = realloc(globalGeometry.starSize, STAR_NUM * sizeof(int));
    if(checkInt == NULL)
    {
        printf("Unable to provide enough memory.\n");
        exit(1);
    }
    else
        globalGeometry.starSize = checkInt;

    for(int i = 0; i < STAR_NUM; i++)
    {
        /*Find x, y co-ords in the range MIN_AXIS_LEN to MAX_AXIS_LEN.
         *The z axis must be in range MIN_AXIS_LEN to Z_NEG so we don't
         *draw stars on top of the geometry.*/
        float x = MIN_AXIS_LEN + (float)rand()/((float)RAND_MAX / (MAX_AXIS_LEN - MIN_AXIS_LEN));
        float y = MIN_AXIS_LEN + (float)rand()/((float)RAND_MAX / (MAX_AXIS_LEN - MIN_AXIS_LEN));
        float z = MIN_AXIS_LEN + (float)rand()/((float)RAND_MAX / (Z_NEG - MIN_AXIS_LEN));
        int size = rand() % 3;

        /*Can't set size to 0 so if
         *we get 0 default to returning a 1.*/
        if(size == 0)
            size = 1;

        /*Store.*/
        globalGeometry.stars[i].x = x;
        globalGeometry.stars[i].y = y;
        globalGeometry.stars[i].z = z;
        globalGeometry.starSize[i] = size;
    }
}




void drawStarBackground()
{
    glDisable(GL_LIGHTING);
    
    glColor3fv(white);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(int i = 0; i < STAR_NUM; i++)
    {
        glPointSize(globalGeometry.starSize[i]);
        glBegin(GL_POINTS);
        glVertex3fv((float *)&globalGeometry.stars[i]);
        glEnd();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}




/*Store the base vertices used by all bases
 *of the scene. Always call after storing the
 *sun as we simply scale those vertices.*/
void storeBase()
{
    struct vertex *checkRealloc;
    checkRealloc = realloc(globalGeometry.bases, 
                           globalGeometry.sunVertices * sizeof(struct vertex));
    globalGeometry.bases = memoryCheck(checkRealloc);
    checkRealloc = realloc(globalGeometry.baseNormals, 
                           globalGeometry.sunVertices * sizeof(struct vertex));
    globalGeometry.baseNormals = memoryCheck(checkRealloc);

    for(int i = 0; i < globalGeometry.sunVertices; i++)
    {
        globalGeometry.baseNormals[i].x = globalGeometry.sunNormals[i].x * BASE_SIZE;
        globalGeometry.baseNormals[i].y = globalGeometry.sunNormals[i].y * BASE_SIZE;
        globalGeometry.baseNormals[i].z = globalGeometry.sunNormals[i].z * BASE_SIZE;

        globalGeometry.bases[i].x = globalGeometry.sun[i].x * BASE_SIZE;
        globalGeometry.bases[i].y = globalGeometry.sun[i].y * BASE_SIZE;
        globalGeometry.bases[i].z = globalGeometry.sun[i].z * BASE_SIZE;
    }
}




/*Draw a player base in the given color.
 *Relies on vertices calculated in storeSunVertices().*/
void drawBase(float color[3])
{
    setBaseMaterial(color);
    
    /*Apply Normals and Draw Vertices.*/
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glNormalPointer(GL_FLOAT, 0, globalGeometry.baseNormals);
    glVertexPointer(3, GL_FLOAT, 0, globalGeometry.bases);

    glDrawArrays(GL_TRIANGLES, 0, globalGeometry.sunVertices);
    
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /*Draw the normal visualizations if toggled.*/
    if(debugNormals())
        drawSphereNormals(N_SCALE_BASE);

    /*Draw axis if in debug mode.*/
    if(debugAxis())
        drawAxis(0.0, 0.0, 0.0);
}




/*Add sun vertex. Encapsulates some common functionality that
 *was present when storing sun vertices.*/
void addSunVertex(float theta, float phi, float *x, float *y, float *z,
                  float *normalx, float *normaly, float *normalz, int *vcount)
{
    struct vertex* sun;
    struct vertex* normals;

    calcCircularVertex(theta, phi, x, y, z, 
                       normalx, normaly, normalz);          
    
    normals = &globalGeometry.sunNormals[*vcount];
    normals->x = *normalx + *x;
    normals->y = *normaly + *y;
    normals->z = *normalz + *z;
    
    sun = &globalGeometry.sun[*vcount];
    sun->x = *x;
    sun->y = *y;
    sun->z = *z;
}




/*Calculates the x,y,z co-ords and the appropriate normals.*/
void calcCircularVertex(float theta, float phi,
                        float *x, float *y, float *z,
                        float *normalx, float *normaly, float *normalz)
{
    *x = RADIUS * sin(phi) * cos(theta);
    *y = RADIUS * sin(phi) * sin(theta);
    *z = RADIUS * cos(phi);

    /*Calculate the vector in the direction of the
    *tangent and normalize.*/
    *normalx = *x - 0;
    *normaly = *y - 0;
    *normalz = *z - 0;
    float length = getVectorLength3f(*normalx, *normaly, *normalz);
    normalizeVectors3f(normalx, normaly, normalz, length);
}




/*Free all dynamic memory.*/
void freeMemory()
{
    free(globalGeometry.sun);    
    free(globalGeometry.sunNormals);
    free(globalGeometry.sunNormalVis);
    free(globalGeometry.outerRing);
    free(globalGeometry.outerRingNormals);
    free(globalGeometry.outerRingTex);
    free(globalGeometry.outerRingNormalVis);
    free(globalGeometry.innerRing);
    free(globalGeometry.innerRingNormals);
    free(globalGeometry.innerRingTex);
    free(globalGeometry.innerRingNormalVis);
    free(globalGeometry.shadow);
    free(globalGeometry.stars);
    free(globalGeometry.starSize);
    free(globalGeometry.bases);
    free(globalGeometry.baseNormals);
}




/*Helper function to run all the other functions
 *which calculate the vertices needed to draw the
 *geometry.*/
void calcGeometryVertices()
{
    storeSunVertices();
    storeSunNormalVertices();
    storeOuterRingVertices();
    storeInnerRingVertices();
    globalGeometry.outerRingNormalVis = storeRingNormalVertices(RING_SIZE, 
                                        globalGeometry.outerRingNormalVis);
    globalGeometry.innerRingNormalVis = storeRingNormalVertices(SAIL_SIZE,
                                        globalGeometry.innerRingNormalVis);
    storeBase();
}
