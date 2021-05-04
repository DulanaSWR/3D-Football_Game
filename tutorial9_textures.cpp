//Dulana Sandes Ratnayake
//w1790251
//2019763
//Youtube Link - https://youtu.be/PAyiue6UElk
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	
#include <gl/freeglut.h>
#include <iostream>
#include <cmath>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//  Created by Philip Trwoga on 20/1/16 to include the many VS2015 changes. Modified by Anastasia Angelopoulou
//  Copyright (c) 2016 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION

//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such
#define PI 3.1415926535897932384626433832795
#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define TEXTURE_COUNT 13
GLuint  textures[TEXTURE_COUNT];

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char* textureFiles[TEXTURE_COUNT] = { "brick_texture_lo_res.tga", "grass_diff.tga", "stormydays_large.tga","targetRed.tga","targetBlue.tga","targetGreen.tga","targetDull.tga","FootballCompleteMap.tga","palmBranchA.tga","yellowFlowerFinal.tga","orangeFlowerFinal5.tga","fillBarHorizontal.tga","fillBarVerticalR.tga" };


//for lighting if you want to experiment with these
GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 100.0f, 100.0f, 50.0f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

//end of intialisation 

//GLfloat ballX, ballY, ballZ;
GLint ballR = 10;
GLfloat targetX = 0.0, targetY = 10.0, targetZ = 0.0;

GLfloat shootX, shootY, shootZ;
BOOLEAN shootBall = false;
GLfloat rollBall;

//------------------------------------//
const int arraySize = 8;
int xPositionsArray[arraySize] = { -110,-110,-50,-20,40,80 ,80,110 };
int yPositionsArray[arraySize] = { 20, -30, 25,-20,10,-10,30,25 };
//{ "x min" , "y min" , "x max" , "y max" , "score"}
GLfloat collision[arraySize][5] = { {-135,50,-95,90,10}, {-140,00,-90,30,30}, {-62,67,-42,87,30}, {-35,15,-5,45,20}, {-27,45,57,75,20}, {62,22,102,62,10}, {74,72,94,92,30}, {105,67,135,87,30} };
GLint status[arraySize] = { 5,3,3,4,4,5,3,3 };
GLfloat score = 0;
char ScoreString[15];
GLint minX, minY, width;

void Collision() {
    if (shootZ <= -376) {
        for (int i = 0; i < arraySize; i++) {
            if ((shootX >= collision[i][0]) && (shootY >= collision[i][1]) && (shootX <= collision[i][2]) && (shootY <= collision[i][3])) {
                score += collision[i][4];
                status[i] = 6;
                std::cout << "score is " << score << std::endl;
            }
        }
    }
}
void displayText(GLfloat x, GLfloat y, GLint r, GLint g, GLint b, const char* string, bool stroke) {
    GLint j = strlen(string);

    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (GLint i = 0; i < j; i++)
    {
        if (stroke)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
        }
        else
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
        }

    }
}
void textNscore() {
    glPushMatrix();

    //draw the text offset from the box
    glTranslatef(10.0, 40.0, 0.0);
    glScalef(0.01, 0.01, 0.01);
    glColor3f(0.0, 1.0, 1.0);
    sprintf_s(ScoreString, "Score:%2.0f", score);



    //if stroke = true then use the tranlate above to move the text
    //if stroke = false then use the first two values of displayText
    displayText(500.0f, 500.0f, 0, 1, 1, ScoreString, true);

    glPopMatrix();

}
void ArrowKeys(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_UP:
        if (targetY >= 90) {
            break;
        }
        else {
            targetY += 5;
            glutPostRedisplay();
            break;
        }
    case GLUT_KEY_DOWN:
        if (targetY <= 10) {
            break;
        }
        else {
            targetY -= 5;
            glutPostRedisplay();
            break;
        }
    case GLUT_KEY_LEFT:
        if (targetX <= -140) {
            break;
        }
        else {
            targetX -= 5;
            glutPostRedisplay();
            break;
        }
    case GLUT_KEY_RIGHT:
        if (targetX >= 140) {
            break;
        }
        else {
            targetX += 5;
            glutPostRedisplay();
            break;
        }
    }
}
void Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'z':
        shootBall = true;
        glutPostRedisplay();
        break;

    case 'f':
        glutPostRedisplay();

    case 'r':
        score = 0;
        status[0] = 5;
        status[1] = 3;
        status[2] = 3;
        status[3] = 4;
        status[4] = 4;
        status[5] = 5;
        status[6] = 3;
        status[7] = 3;
        std::cout << "Game restarted" << std::endl;
        glutPostRedisplay();
        break;

    }


}
// here we declare an array of vertices for the cube
GLfloat cubeVertexData[108] =
{
    //object vertex coordinates for cube made from triangles
    // Data layout for each line below is:
    // positionX, positionY, positionZ
    //wind counter-clockwise

    900.0f, -900.0f, -900.0f,
    900.0f, 900.0f, -900.0f,
    900.0f, -900.0f, 900.0f,
    900.0f, -900.0f, 900.0f,
    900.0f, 900.0f, -900.0f,
    900.0f, 900.0f, 900.0f,

    900.0f, 900.0f, -900.0f,
    -900.0f, 900.0f, -900.0f,
    900.0f, 900.0f, 900.0f,
    900.0f, 900.0f, 900.0f,
    -900.0f, 900.0f, -900.0f,
    -900.0f, 900.0f, 900.0f,

    -900.0f, 900.0f, -900.0f,
    -900.0f, -900.0f, -900.0f,
    -900.0f, 900.0f, 900.0f,
    -900.0f, 900.0f, 900.0f,
    -900.0f, -900.0f, -900.0f,
    -900.0f, -900.0f, 900.0f,
    //4
    -900.0f, -900.0f, -900.0f,
    900.0f, -900.0f, -900.0f,
    -900.0f, -900.0f, 900.0f,
    -900.0f, -900.0f, 900.0f,
    900.0f, -900.0f, -900.0f,
    900.0f, -900.0f, 900.0f,
    //5
    900.0f, 900.0f, 900.0f,
    -900.0f, 900.0f, 900.0f,
    900.0f, -900.0f, 900.0f,
    900.0f, -900.0f, 900.0f,
    -900.0f, 900.0f, 900.0f,
    -900.0f, -900.0f, 900.0f,
    //6
    900.0f, -900.0f, -900.0f,
    -900.0f, -900.0f, -900.0f,
    900.0f, 900.0f, -900.0f,
    900.0f, 900.0f, -900.0f,
    -900.0f, -900.0f, -900.0f,
    -900.0f, 900.0f, -900.0f

};
//the texture coordinates - work will skybox texture of this shape +--
GLfloat textureCoordsSkyBox[72] = {
    //face 1
    0.75,0.33,      //    0,1,
    0.75,0.67,     //    1,1,
    0.5,0.33,     //    0,0,
    0.5,0.33,     //    0,0,
    0.75,0.67,    //    1,0,
    0.5,0.67,   //    1,1,

    //face 2
    0.5,1.0, //    1,1,
    0.25,1, //    0,1,
    0.5,0.67, //    1,0,
    0.5,0.67, //    1,0,
    0.25,1.0, //    0,1,
    0.25,0.67, //    1,1,
    //face 3
    0,0.67,//    1,1,
    0,0.33,//    0,1,
    0.25,0.67,//    1,0,
    0.25,0.67,//    1,0,
    0,0.33,//    0,1,
    0.25,0.33,//    0,0,
    //face 4
    0.25,0.0,//    0,1,
    0.5,0.0,//    1,1,
    0.25,0.33,//    0,0,
    0.25,0.33,//    0,0,
    0.5,0.0,//    1,1,
    0.5,0.33,//    0,0,
    //face 5
    0.5,0.67,//    1,0,
    0.25,0.67,//    0,0,
    0.5,0.33,//    1,1,
    0.5,0.33,//    1,1,
    0.25,0.67,//    0,0,
    0.25,0.33,//    0,1,
    //face 6
    0.75,0.33,//    1,1,
    1.0,0.33,//    0,1,
    0.75,0.67,//    1,0,
    0.75,0.67,//    1,0,
    1.0,0.33,//    0,1,
    1.0,0.67//    0,0


};
//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f

};

//skybox
void Skybox() {
    glFrontFace(GL_CW); //texture the inside
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    //bind your texture here

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, gCubeVertexdataNormals);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertexData);
    //   glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsSkyBox);
    // draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_TEXTURE_2D);
}
void setOrthographicProjection() {
    // switch to projection mode
    glMatrixMode(GL_PROJECTION);
    // save the previous matrix which contains the
    //set up for the perspective projection
    glPushMatrix();
    // reset matrix
    glLoadIdentity();
    // set a 2D orthographic projection
    gluOrtho2D(0, iWidth, 0, iHeight);
    // invert the y axis, down is positive
    glScalef(1, -1, 1);
    // mover the origin from the bottom left corner
    // to the upper left corner
    glTranslatef(0, -iHeight, 0);
    //set for drawing again
    glMatrixMode(GL_MODELVIEW);
}
void resetPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

GLUquadricObj* quadricFootball;
void drawFootBall(GLfloat x, GLfloat y, GLfloat z)
{

    glFrontFace(GL_CCW);
    glTranslatef(x, y, z);
    // Create and texture the ball
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[7]);
    // glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    quadricFootball = gluNewQuadric();
    gluQuadricDrawStyle(quadricFootball, GLU_FILL);
    gluQuadricNormals(quadricFootball, GLU_SMOOTH);
    gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
    gluQuadricTexture(quadricFootball, GL_TRUE);
    gluSphere(quadricFootball, 10.0, 50, 35);
    glDisable(GL_TEXTURE_2D);

}

// Called to draw scene
void hoops(int image, int size)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0 - size / 2, 50.0 - size / 2, -360.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(0.0 + size / 2, 50.0 - size / 2, -360.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0 + size / 2, 50.0 + size / 2, -360.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0 - size / 2, 50.0 + size / 2, -360.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void Trees(int image)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-149.0, 0.0, -200.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(-149.0, 0.0, -220.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-149.0, 70.0, -220.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-149.0, 70.0, -200.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

}

void Flowers(int image)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-140.0, 0.0, -395.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(-110.0, 0.0, -395.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-110.0, 30.0, -395.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-140.0, 30.0, -395.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

}
void RenderScene(void)
{

    // Clear the window with current clearing colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // view the scene
    gluLookAt(0.00, 30.00, 20.00,//eye
        0.00, 50.00, -400.00,//centre
        0.00, 1.00, 0.00);//up

    //target Collision detection
    Collision();
    //Text and score
    textNscore();
    //football
    glPushMatrix();
    glTranslatef(shootX, shootY, shootZ);
    //glRotatef(rollBall, 1.0, 0.0, 0.0);
    drawFootBall(0.0, ballR, -50.0);
    glPopMatrix();



    glPushMatrix();
    glRotatef(180, 0.0, 1.0, 0.0);
    Skybox();
    glPopMatrix();



    //Left wall
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-150.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-150.0, 0.0, -400.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-150.0, 100.0, -400.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-150.0, 100.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    //Right wall
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(150.0, 0.0, -400.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(150.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(150.0, 100.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(150.0, 100.0, -400.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Back wall
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-150.0, 0.0, -400.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(150.0, 0.0, -400.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(150.0, 100.0, -400.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-150.0, 100.0, -400.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //Ground
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-150.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(150.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(150.0, 0.0, -400.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-150.0, 0.0, -400.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tree
    glPushMatrix();
    glTranslatef(0, 0, 0);
    Trees(8);
    glPopMatrix();

    //tree
    glPushMatrix();
    glTranslatef(0, 0, -40);
    Trees(8);
    glPopMatrix();

    //tree
    glPushMatrix();
    glTranslatef(0, 0, 40);
    Trees(8);
    glPopMatrix();

    //tree
    glPushMatrix();
    glTranslatef(298, 0, 0);
    Trees(8);
    glPopMatrix();

    //tree
    glPushMatrix();
    glTranslatef(298, 0, -40);
    Trees(8);
    glPopMatrix();

    //tree
    glPushMatrix();
    glTranslatef(298, 0, 40);
    Trees(8);
    glPopMatrix();

    //Flowers
    glPushMatrix();
    glTranslatef(50, 0, 0);
    Flowers(9);
    glPopMatrix();
    //Flowers
    glPushMatrix();
    glTranslatef(70, 0, 0);
    Flowers(10);
    glPopMatrix();

    //Flowers
    glPushMatrix();
    glTranslatef(140, 0, 0);
    Flowers(9);
    glPopMatrix();
    //Flowers
    glPushMatrix();
    glTranslatef(160, 0, 0);
    Flowers(10);
    glPopMatrix();

    //Flowers
    glPushMatrix();
    glTranslatef(240, 0, 0);
    Flowers(9);
    glPopMatrix();
    //Flowers
    glPushMatrix();
    glTranslatef(260, 0, 0);
    Flowers(10);
    glPopMatrix();
    /*
    //target
    glPushMatrix();
    glTranslatef(targetX, targetY, 0);
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, -395.0);
    glVertex3f(5, 0, -395.0);
    glVertex3f(5, 5, -395.0);
    glVertex3f(0, 5, -395.0);
    glEnd();
    glPopMatrix();*/


    //hoop 1
    glPushMatrix();
    glTranslatef(xPositionsArray[0], yPositionsArray[0], 0);
    hoops(status[0], 40);
    glPopMatrix();
    //hoop 2
    glPushMatrix();
    glTranslatef(xPositionsArray[1], yPositionsArray[1], 0);
    hoops(status[1], 20);
    glPopMatrix();
    //hoop 3
    glPushMatrix();
    glTranslatef(xPositionsArray[2], yPositionsArray[2], 0);
    hoops(status[2], 20);
    glPopMatrix();
    //hoop 4
    glPushMatrix();
    glTranslatef(xPositionsArray[3], yPositionsArray[3], 0);
    hoops(status[3], 30);
    glPopMatrix();
    //hoop 5
    glPushMatrix();
    glTranslatef(xPositionsArray[4], yPositionsArray[4], 0);
    hoops(status[4], 30);
    glPopMatrix();
    //hoop 6
    glPushMatrix();
    glTranslatef(xPositionsArray[5], yPositionsArray[5], 0);
    hoops(status[5], 40);
    glPopMatrix();
    //hoop 7
    glPushMatrix();
    glTranslatef(xPositionsArray[6], yPositionsArray[6], 0);
    hoops(status[6], 20);
    glPopMatrix();
    //hoop 8
    glPushMatrix();
    glTranslatef(xPositionsArray[7], yPositionsArray[7], 0);
    hoops(status[7], 20);
    glPopMatrix();




    //Horizontal bar
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[11]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-120.0, 100.0, -150.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-40.0, 100.0, -150.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-40.0, 120.0, -150.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-120.0, 120.0, -150.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);



    //Vertical bar
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[12]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(120.0, 0.0, -150.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(140.0, 0.0, -150.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(140.0, 80.0, -150.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(120.0, 80.0, -150.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    GLfloat xHorizontal = targetX / 5;
    glPushMatrix();
    glTranslatef(xHorizontal, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-70.0, 100.0, -149.0);
    glVertex3f(-90.0, 100.0, -149.0);
    glVertex3f(-90.0, 119.0, -149.0);
    glVertex3f(-70.0, 119.0, -149.0);
    glEnd();
    glPopMatrix();

    GLfloat yVertical = targetY / 1.3;
    glPushMatrix();
    glTranslatef(0.0, yVertical, 0.0);
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(120.0, -10.0, -149.0);
    glVertex3f(138.0, -10.0, -149.0);
    glVertex3f(138.0, 10.0, -149.0);
    glVertex3f(120.0, 10.0, -149.0);
    glEnd();
    glPopMatrix();


    glPushMatrix();
    glLoadIdentity(); //need this
    setOrthographicProjection();



    glPopMatrix();
    resetPerspectiveProjection();
    glutSwapBuffers();

}


// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    //textures

    GLuint texture;
    // allocate a texture name
    glGenTextures(1, &texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // photoshop is a good converter to targa (TGA)
    //the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
    //there are quite a few ways of loading images
    // Load textures in a for loop
    glGenTextures(TEXTURE_COUNT, textures);
    //this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
    for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        // Bind to next texture object
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

        // Load texture data, set filter and wrap modes
        //note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
        pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
            &iComponents, &eFormat);

        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

        //set up texture parameters

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //try these too
       // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        free(pBytes0);
    }

    //enable textures
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
    //glFrontFace(GL_CCW);// Counter clock-wise polygons face out
 //	glEnable(GL_CULL_FACE);		// Do not calculate inside
    glCullFace(GL_FRONT_AND_BACK);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightBright);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLightBright);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
    //glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLightBright);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
    //glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
    //glEnable(GL_LIGHT2);

    // Enable colour tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Black blue background clear colour
    glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}

GLint chances = 0;
void TimerFunc(int value)
{
    glutTimerFunc(10, TimerFunc, 1);
    if (shootBall == true) {
        shootZ -= 3;


        if (targetX >= shootX) {
            shootX += targetX / (sin(atan(320 / targetX)) * 150);
        }

        if (targetX <= shootX) {
            shootX -= targetX / (sin(atan(320 / targetX)) * 150);
        }
        if (targetY >= shootY) {
            shootY += targetY / (sin(atan(320 / targetY)) * 150);
        }

        if (targetY <= targetY / (sin(atan(320 / targetY)) * 150)) {
            shootY -= 0.5;
        }


        //rollBall--;

    }
    if (shootZ <= -380) {

        shootX = 0;
        shootY = 0;
        shootZ = 40;
        chances += 1;
        shootBall = false;
    }
    glutPostRedisplay();

}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Calculate aspect ratio of the window
    fAspect = (GLfloat)w / (GLfloat)h;

    // Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // field of view of 45 degrees, near and far planes 1.0 and 1000
    //that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
    gluPerspective(55.0f, fAspect, 1.0, 1000.0);

    // Modelview matrix reset
    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 780);
    glutCreateWindow("Football Game");

    glutSpecialFunc(ArrowKeys);
    glutKeyboardFunc(Keyboard);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutTimerFunc(25, TimerFunc, 1);
    SetupRC();
    glutMainLoop();
    return 0;
}






