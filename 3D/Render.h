/* 
 * File:   Render.h
 * Author: jeferson
 *
 * Created on 1 de Agosto de 2012, 21:31
 */

#ifndef RENDER_H
#define	RENDER_H
//modos de visualizacao



/**
 * Desenha um gradeado no plano xy para servir como referencia
 */
void renderGridXY( )
{
    glDisable( GL_LIGHTING );

    glColor3f( 0.75f, 0.75f, 0.438f );

    glBegin( GL_LINES );
    for (int i = -500; i <= 500; i += 5)
    {
        glVertex3i( i, 500, 0 );
        glVertex3i( i, -500, 0 );
    }
    for (int i = -500; i <= 500; i += 5)
    {
        glVertex3i( 500, i, 0 );
        glVertex3i( -500, i, 0 );
    }
    glEnd( );

    glEnable( GL_LIGHTING );
}



/**
 * Renderiza a superficie usando shader
 */

void renderSurfaceWireframe( float *vertices, float *normais, int* triangles, int numTriangles, bool fillSurface )
{
    if (fillSurface)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    } 
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE );
    glEnable( GL_LIGHTING );

    glDisable( GL_BLEND );
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_TEXTURE_1D );

    glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glColor3f( 1.0f, 0.0f, 0.0f );
    glVertexPointer( 3, GL_FLOAT, 0, vertices );
    glNormalPointer( GL_FLOAT, 0, normais );

    glDrawElements( GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, triangles );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    
    glPopAttrib( );
    glPopClientAttrib( );
    
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

#endif	/* RENDER_H */

