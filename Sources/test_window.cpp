#include "test_window.h"
#include "util.h"
#include <GL/glu.h>

static int Display_SetViewport( int width, int height )
{
    GLfloat ratio;
    if ( height == 0 ) {
        height = 1;
    }

    ratio = ( GLfloat )width / ( GLfloat )height;
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    return 1;
}

void TestWindow::resizeGL()
{
    SDL_Log("RESIZEGL");
    fflush(stdin);
    Display_SetViewport(width(), height());
}

void TestWindow::initGL()
{
    SDL_Log("INITGL");
    glShadeModel( GL_SMOOTH );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}

void TestWindow::displayGL()
{
    static int cnt = 500;
    if (0==--cnt)
        cnt = 500;
    float F = cnt / 2000.0;
    //SDL_Log("DisplayGL");
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glTranslatef( -1.5f , 0.0f, -6.0f );

    glBegin( GL_TRIANGLES ); /* Drawing Using Triangles */
      glVertex3f( 0.0f, 1.0f, 0.0f ); /* Top */
      glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
      glVertex3f( 1.0f, -1.0f, 0.0f ); /* Bottom Right */
    glEnd( ); /* Finished Drawing The Triangle */

    glTranslatef( 3.0f + F, 0.0f, 0.0f );

    glBegin( GL_QUADS ); /* Draw A Quad */
      glVertex3f( -1.0f, 1.0f, 0.0f ); /* Top Left */
      glVertex3f( 1.0f, 1.0f, 0.0f ); /* Top Right */
      glVertex3f( 1.0f, -1.0f, 0.0f ); /* Bottom Right */
      glVertex3f( -1.0f, -1.0f, 0.0f ); /* Bottom Left */
    glEnd( ); /* Done Drawing The Quad */
}
