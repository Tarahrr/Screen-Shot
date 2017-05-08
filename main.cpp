#include <math.h>
#include <GLUI/glui.h>

#include <iostream>
#include <fstream>
using namespace std;
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <sstream>
#include <string>
#include "glm.h"
#include "imageloader.h"
GLMmodel *objmodel_ptr;

void init ();
void display (void);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void entry (int state);

int window_x;
int window_y;
int window_width = 512 ;
int window_height = 512;

GLuint main_window;
int full_screen = 1;

GLUI * glui_window;
int radiogroup_item_id = 0;
int radiogroup_item_id2 = 0;



float fromX=0 , fromY=0 , fromScale=0;
float toX=90 , toY=45 , toScale=0;
float stepX=5 , stepY=5 , stepScale=0;
int isBackground=0;

string X_R="",Y_R="", SCALE_R="";


float color[] = { 1.0, 1.0, 1.0 };

void setupGLUI ();
void idle ();
void glui_callback (int arg);

bool check =false;
bool check_set =false;
float x_r=0 , Y_r=0 , scale = 0;

string mainPath="/Users/Tara/Desktop/Graphic/ScreenShot/ScreenShot/ScreenShot/image/";
string texturePath;
string filename="1";

GLfloat RED=1,GREEN=0,BLUE=0;


void CaptureScreen(string filename)
{
    size_t i, j, k, cur;
    
    
    const int width = glutGet(GLUT_WINDOW_WIDTH);
    const int height = glutGet(GLUT_WINDOW_HEIGHT);
    int pixel_nbytes = 4;
    
    SDL_Surface * image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    void* temp_row;
    int height_div_2;
    
    temp_row = (void *)malloc(image->pitch);
    if(NULL == temp_row)
    {
        SDL_SetError("Not enough memory for image inversion");
    }
    height_div_2 = (int) (image->h * .5);
    for(int index = 0; index < height_div_2; index++)
    {
        memcpy((Uint8 *)temp_row,(Uint8 *)(image->pixels) + image->pitch * index, image->pitch);
        memcpy((Uint8 *)(image->pixels) + image->pitch * index, (Uint8 *)(image->pixels) + image->pitch * (image->h - index-1), image->pitch);
        memcpy((Uint8 *)(image->pixels) + image->pitch * (image->h - index-1), temp_row, image->pitch);
    }
    free(temp_row);
    char *char_Filename = new char[filename.length() + 1];
    strcpy(char_Filename, filename.c_str());
    
    SDL_SaveBMP(image, char_Filename);
    SDL_FreeSurface(image);
    
}
//**************************************************
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
//**************************************************
enum
{
    SCALE=0,
    ROTATIONX,
    ROTATIONY,
    BackGround_RADIOGROUP,
    QUIT_BUTTON,
    SCREENSHOT,
    Set,
    BackGround_RADIOGROUP2,

};

void init ()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    
    if (!objmodel_ptr)
    {
        string filename="/Users/Tara/Desktop/Graphic/ScreenShot/ScreenShot/ScreenShot/OBJ/monkey.obj";
        char *char_Filename = new char[filename.length() + 1];
        strcpy(char_Filename, filename.c_str());
        objmodel_ptr = glmReadOBJ(char_Filename);
        if (!objmodel_ptr)
            exit(0);
        
        glmUnitize(objmodel_ptr);
        glmFacetNormals(objmodel_ptr);
        glmVertexNormals(objmodel_ptr, 90.0);
    }
    
    
}
void display (void)
{
    texturePath = "/Users/Tara/Desktop/Graphic/ScreenShot/ScreenShot/ScreenShot/BackGround/"+filename+".bmp";
    char *char_Filename = new char[texturePath.length() + 1];
    strcpy(char_Filename, texturePath.c_str());
    
    Image* image = loadBMP(char_Filename);
    _textureId = loadTexture(image);
    delete image;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glTranslated(0, 0, -10);
    
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    
    
    //*
    if(isBackground){
        glPushMatrix();
        glColor3f(1, 1, 1);
        glRotated(90, 1, 0, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5f, -2.5f, 2.5f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5f, -2.5f, 2.5f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5f, -2.5f, -2.5f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5f, -2.5f, -2.5f);
        glEnd();
        
        glPopMatrix();
    }
    // */
    
    
    
    glColor3f(RED,GREEN,BLUE);
    
    if(check)
    {
        // j = Y_r
        // i = x_r
        
        if( x_r == toX) { check = false;}
        if( Y_r > toY) { x_r+=stepX; Y_r = fromY-stepY;}
        if( scale > toScale/100 ) {  Y_r += stepY; scale=fromScale/100;  }
        
        
        glPushMatrix ();
        
        glScaled(scale, scale, scale);
        glRotatef(Y_r, 0, 1, 0 );
        glRotatef(x_r, 1.0f, 0.0f, 0.0f);
        
        //glColor3f(1, 0, 0);
        //glutSolidTeapot(5);
        glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
        ostringstream x;
        x << x_r;
        X_R = x.str();
        ostringstream y;
        y<< Y_r;
        Y_R = y.str();
        ostringstream s;
        s<< scale*100;
        SCALE_R = s.str();
        if(Y_r >= fromY)
            CaptureScreen(mainPath+X_R+"_"+Y_R+"_"+SCALE_R+".bmp");
        
        scale+=stepScale/100;
        
        glPopMatrix();
        
        
        
    }
    else{
        glPushMatrix ();
        //glutSolidTeapot(5);
        glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
        glPopMatrix();
    }
    
    glutSwapBuffers ();
    
}
void reshape (int w, int h)
{
    if (h == 0 || w == 0) return;
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glPointSize(GLfloat(w)/70.0);
}


void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
            //case 's' : SaveScreenGrab("screenshot.tga"); break;
        case 27:
            exit (1);
            break;
    }
    
    glutPostRedisplay ();
}


void setupGLUI ()
{
    GLUI_Master.set_glutIdleFunc (idle);
    
    glui_window = GLUI_Master.create_glui ("Guidance", 0, window_x - 235, window_y);
    
    //**********************************************************************************************
    GLUI_Panel *RotateX_panel = glui_window->add_panel ("RotationX");
    
    GLUI_Spinner *FromXRotate = glui_window->add_spinner_to_panel (RotateX_panel, "From", GLUI_SPINNER_FLOAT, &fromX,ROTATIONX, glui_callback);
    
    GLUI_Spinner *ToXRotate = glui_window->add_spinner_to_panel (RotateX_panel, "To", GLUI_SPINNER_FLOAT, &toX, ROTATIONX, glui_callback);
    
    GLUI_Spinner *StepXRotate = glui_window->add_spinner_to_panel (RotateX_panel, "Step", GLUI_SPINNER_FLOAT, &stepX, ROTATIONX, glui_callback);
    
    //**********************************************************************************************
    GLUI_Panel *RotateY_panel = glui_window->add_panel ("RotationY");
    
    GLUI_Spinner *FromYRotate = glui_window->add_spinner_to_panel (RotateY_panel, "From", GLUI_SPINNER_FLOAT, &fromY, ROTATIONY, glui_callback);
    
    GLUI_Spinner *ToYRotate = glui_window->add_spinner_to_panel (RotateY_panel, "To", GLUI_SPINNER_FLOAT, &toY, ROTATIONY, glui_callback);
    
    GLUI_Spinner *StepYRotate = glui_window->add_spinner_to_panel (RotateY_panel, "Step", GLUI_SPINNER_FLOAT, &stepY, ROTATIONY, glui_callback);
    
    //**********************************************************************************************
    GLUI_Panel *Scale_panel = glui_window->add_panel ("Scale");
    
    GLUI_Spinner *FromScale = glui_window->add_spinner_to_panel (Scale_panel, "From", GLUI_SPINNER_FLOAT, &fromScale, SCALE, glui_callback);
    
    GLUI_Spinner *ToScale = glui_window->add_spinner_to_panel (Scale_panel, "To", GLUI_SPINNER_FLOAT, &toScale, SCALE, glui_callback);
    
    GLUI_Spinner *StepScale = glui_window->add_spinner_to_panel (Scale_panel, "Step", GLUI_SPINNER_FLOAT, &stepScale, SCALE, glui_callback);
    
    GLUI_Rollout *ot_rollout = glui_window->add_rollout ("Back Ground");
    
    GLUI_RadioGroup *ot_group = glui_window->add_radiogroup_to_panel
    (ot_rollout, &radiogroup_item_id, BackGround_RADIOGROUP, glui_callback);
    
    glui_window->add_radiobutton_to_group( ot_group, "Image1" );
    glui_window->add_radiobutton_to_group( ot_group, "Image2" );
    glui_window->add_radiobutton_to_group( ot_group, "Image3" );
    glui_window->add_radiobutton_to_group( ot_group, "Image4" );
    
    glui_window->add_separator_to_panel (ot_rollout);
    glui_window->add_checkbox_to_panel (ot_rollout, "BackGround", &isBackground );
    
    GLUI_Rollout *ot_rollout2 = glui_window->add_rollout ("Object Color");
    
    GLUI_RadioGroup *ot_group2 = glui_window->add_radiogroup_to_panel
    (ot_rollout2, &radiogroup_item_id2, BackGround_RADIOGROUP2, glui_callback);
    
    glui_window->add_radiobutton_to_group( ot_group2, "RED" );
    glui_window->add_radiobutton_to_group( ot_group2, "GREEN" );
    glui_window->add_radiobutton_to_group( ot_group2, "BLUE" );
    glui_window->add_radiobutton_to_group( ot_group2, "YELLOW" );
    
    glui_window->add_button ("Screen Shot", SCREENSHOT, glui_callback);
    
    glui_window->add_button ("Quit", QUIT_BUTTON, glui_callback);
    
    // glui_window->set_main_gfx_window( main_window );
}

void glui_callback (int control_id)
{
    switch (control_id)
    {
        case SCREENSHOT :
            x_r=fromX;
            Y_r=fromY;
            scale = fromScale/100;
            check =true;
            check_set=false;
            break;
        case BackGround_RADIOGROUP:
            if(radiogroup_item_id ==0 ){
                filename="1";
            }
            
            if(radiogroup_item_id ==1 ){
                filename="2";
            }
            
            if(radiogroup_item_id ==2 ){
                filename="3";
            }
            
            if(radiogroup_item_id ==3 ){
                filename="4";
            }
            break;  case BackGround_RADIOGROUP2:
            if(radiogroup_item_id2 ==0 ){
                RED=1.0; GREEN =0.0 ; BLUE =0.0;
            }
            
            if(radiogroup_item_id2 ==1 ){
                RED=0.0; GREEN =1.0 ; BLUE =0.0;
            }
            
            if(radiogroup_item_id2 ==2 ){
                RED=0.0; GREEN =0.0 ; BLUE =1.0;
            }
            
            if(radiogroup_item_id2 ==3 ){
                RED=1.0; GREEN =1.0 ; BLUE =0.0;
            }
            break;

        case Set: break;
        case QUIT_BUTTON:
            
            printf ("Quit Button clicked... Exit!\n");
            
            exit (1);
            
            break;
            
    }
}

void idle ()
{
    glutSetWindow (main_window);
    glutPostRedisplay ();
}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize (window_width, window_height);
    glutInitWindowPosition (window_x, window_y);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
    main_window = glutCreateWindow ("ScreenShot");
    init();
    
    glutDisplayFunc (display);
    glutReshapeFunc  (reshape);
    glutKeyboardFunc (keyboard);
    setupGLUI ();
    
    glutMainLoop();
}
