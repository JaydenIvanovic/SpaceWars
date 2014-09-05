#include <GL/glut.h>
#include "colors.h"

/*Initialize lighting.*/
void initLighting()
{
    //GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};

    /*For first person view from each perspective.*/
    GLfloat light_position1[] = {-1.0, 0.0, 0.0, 0.0};
    GLfloat light_position2[] = {1.0, 0.0, 0.0, 0.0};
    
    GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
    
    #if 0
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat emissive[] = {0.8 ,0.8, 0.8, 1.0};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    #endif

    glShadeModel(GL_SMOOTH);
    
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

    #if 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    #endif

    #if 1
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    //glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
    #endif

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    #if 1
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);
    #endif
}




/*Set the suns materials*/
void setSunMaterial()
{
   /*Set materials for lighting and draw the object.*/
    GLfloat emission[] = {0.01, 0.01, 0.01, 0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}




/*Set the base materials*/
void setBaseMaterial(float color[3])
{
   /*Set materials for lighting and draw the object.*/
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}




/*Set materials of the outer plant ring.*/
void setRingWorldMaterial()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}




/*Sets the materials for the outer ring when the shadows
 are drawn as part of the geometry itself.*/
void setRingWorldMaterialSplitShadows()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, light_grey);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialfv(GL_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}




/*Sets the material for the part of the sail which is open
 and will not cast a shadow.*/
void setOpenSailMaterial()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
}




/*Sets the material for the part of the sail which WILL cast 
 a shadow onto the outer ring.*/
void setClosedSailMaterial()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, light_grey);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, grey);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}
