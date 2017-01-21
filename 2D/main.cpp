/* 
 * File:   main.cpp
 * Author: paulaceccon
 *
 * Created on 10 de Novembro de 2013, 10:43
 */

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cmath>
#include "Node.h"
#include "KdTree.h"

#ifdef __APPLE__
        #include <OpenGL/gl.h>
        #include <GLUT/glut.h>
#elif __WINDOWS__
        #include <GL/gl.h>
        #include <GL/glut.h>
#endif

/** The triangles coordinates **/
float coordinates[] = { -1.0, 1.0, 0.0, 0.0, 2.0, 0.0, 2.0, 3.0, 0.0, 2.0, 1.0, 1.0 };
/** The triangles indexation **/
int triangleList[] = { 0, 4, 5, 0, 1, 5, 1, 5, 2, 2, 3, 5, 3, 5, 4 };
/** Number of trinagles **/
int numTriangles = 5;
/** r-d space of work **/
int rSpace = 2;

// Define o sistema de coordenadas: (-winW, winW) em X
// e (-winH, winH) em Y. 
// Estes valores é sempre calculado na função reshape e valem para o tamanho 
// inicial da janela apenas.
double winW = 25.0, winH = 25.0;

// Define o tamanho do grid desenhado ao fundo. 
// Este valor é usado para calcular o sistema de coordenadas.
const double lenghGrid = 24.0;

// Tamanho da janela em pixeis. Usado para converter do sistema de coordenadas 
// em pixeis para o sistema de coordenadas definido na janela.
int widthWin = 600, heightWin = 600;

// Cor de fundo da janela e do grid. 1.0f para preto e 0.0f para branco.
float colorGrid = 1.0f;

// Ponto selecionado, se houver.
int selectPoint = -2;

// Modo de interacao
enum InteractionMode
{
    INSERT_POINTS = 0,
    EDIT_POINTS = 1
};
InteractionMode interactionMode = INSERT_POINTS;

int depth = 0;

bool viewAllAABB = false;


/**
 * Funcao que renderiza o grid.
 */
void renderGrid( void )
{
    double i = 0.0, j = 0.0;

    // Desenha quadradinhos do grid ao fundo.
    for (i = -winH; i < winH; i++)
    {
        for (j = -winW; j < winW; j++)
        {
            glBegin( GL_POLYGON );
            glColor3f( fabs( colorGrid - 0.94f ), fabs( colorGrid - 0.94f ), fabs( colorGrid - 0.94f ) );
            glVertex2f( i * 2, j * 2 );
            glVertex2f( i * 2 + 1, j * 2 );
            glVertex2f( i * 2 + 1, j * 2 + 1 );
            glVertex2f( i * 2, j * 2 + 1 );
            glEnd( );
        }
    }

    // Desenha linhas do grid.
    glBegin( GL_LINES );

    glColor3f( fabs( colorGrid - 0.9f ), fabs( colorGrid - 0.9f ), fabs( colorGrid - 0.9f ) );
    for (i = -winW; i < winW; i++)
    {
        glVertex2f( i, -winH );
        glVertex2f( i, winH );
    }
    for (i = -winH; i < winH; i++)
    {
        glVertex2f( -winW, i );
        glVertex2f( winW, i );
    }

    // Desenha linhas para os eixos X e Y.
    glColor3f( fabs( colorGrid - 0.4f ), fabs( colorGrid - 0.4f ), fabs( colorGrid - 0.4f ) );
    glVertex2f( 0.0, -winH );
    glVertex2f( 0.0, winH );
    glVertex2f( -winW, 0.0 );
    glVertex2f( winW, 0.0 );
    glEnd( );
}



/**
 * Funcao que renderiza a kdtree.
 */
void renderNode( Node * node, int d )
{
    if (node == 0 || d >= depth)
    {
        return;
    }

    if (d == depth - 1 || viewAllAABB)
    {

        float max_x = node->max_x;
        float max_y = node->max_y;
        float min_x = node->min_x;
        float min_y = node->min_y;

        if (rSpace == 2)
        {
            switch (d)
            {
                case 0:
                    glColor3f( 1, .5, 0 );
                    break;
                case 1:
                    glColor3f( 0, 1, 1 );
                    break;
                case 2:
                    glColor3f( 0, 1, 0 );
                    break;
                case 3:
                    glColor3f( 1, 1, 0 );
            }

            glBegin( GL_LINES );
            glVertex2f( min_x, min_y );
            glVertex2f( min_x, max_y );

            glVertex2f( min_x, min_y );
            glVertex2f( max_x, min_y );

            glVertex2f( max_x, min_y );
            glVertex2f( max_x, max_y );

            glVertex2f( min_x, max_y );
            glVertex2f( max_x, max_y );
            glEnd( );

            int i = node->triangle_index;
            int index_1 = triangleList[i * 3];
            int index_2 = triangleList[i * 3 + 1];
            int index_3 = triangleList[i * 3 + 2];

            float x = (coordinates[index_1 * rSpace] + coordinates[index_2 * rSpace] + coordinates[index_3 * rSpace]) / 3;
            float y = (coordinates[index_1 * rSpace + 1] + coordinates[index_2 * rSpace + 1] + coordinates[index_3 * rSpace + 1]) / 3;

            glPointSize( 6 );

            glEnable( GL_POINT_SMOOTH );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glBegin( GL_POINTS );
            glVertex2f( x, y );
            glEnd( );

        }

        else if (rSpace == 3)
        {
            float max_z = node->max_z;
            float min_z = node->min_z;

            // TO DO: BUILD A CUBE
        }
    }
    renderNode( node->left, d + 1 );
    renderNode( node->right, d + 1 );
}



void renderKdTree( void )
{
    KdTree tree( coordinates, triangleList, numTriangles, rSpace );

    renderNode( tree.root, 0 );
}



void RenderTriangles( )
{
    glColor3f( 1.0, 0.0, 0.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( rSpace, GL_FLOAT, 0, coordinates );
    glDrawElements( GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, triangleList );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}



/**
 * Funcao que recebe eventos de redesenho da tela.
 * Esta função deve ser chamada todas as vezes que a tela precisar ser
 * redesenhada.
 */
void display( void )
{
    // Limpa a janela com a cor branca ou preta, dependendo do valor de fundo do grid.
    glClearColor( fabs( colorGrid - 1.0 ), fabs( colorGrid - 1.0 ), fabs( colorGrid - 1.0 ), 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Renderiza o grid de referencia que aparece ao fundo da tela.
    renderGrid( );

    glPushMatrix( );
    glTranslated( -8, -8, 0 );

    glScaled( 10, 10, 0 );

    // Renderiza cada triangulo.
    RenderTriangles( );

    // Renderiza a kdtree.
    renderKdTree( );

    glPopMatrix( );

    // Troca os buffers de escrita e desenho.
    glutSwapBuffers( );
}



/**
 * Recebe eventos de redimensionamento da tela.
 * @param width - largura da janela em pixeis.
 * @param heigh - altura da janela em pixeis.
 */
void reshape( int width, int heigh )
{
    // Define qual porção da janela será usada.
    // Os parametros são: coordenadas inferior esquerda (0,0) e a largurda
    // e a altura a partir do ponto.
    glViewport( 0, 0, width, heigh );

    // Salva as dimensoes da janela em pixeis.
    widthWin = width;
    heightWin = heigh;

    // Calcula a variacao do sistema de coordenadas de modo que o (0,0) fique
    // no centro da tela.
    winW = (int) (width / lenghGrid + 0.5);
    winH = (int) (heigh / lenghGrid + 0.5);

    // Define a matriz de projecao como corrente para definir o sistema de
    // coordenadas.
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    // Define o sistema de coordenadas.
    gluOrtho2D( -winW, winW, -winH, winH );

    // Define a matriz de modelview como corrente para realizar o desenho.
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
}



void keyboard( unsigned char key, int x, int y )
{
    switch (key)
    {
        case 'd':
            depth--;
            if (depth < 0)
                depth = 0;
            break;
        case 'D':
            depth++;
            break;
        case 'v':
            viewAllAABB = !viewAllAABB;
            break;
    }
    display( );
}



/**
 * Função principal.
 * @param argc - número de argumentos do programa.
 * @param argv - lista de argumentos do programa.
 * @return 
 */
int main( int argc, char** argv )
{
    // Inicializa a glut passando os argumentos do programa.
    glutInit( &argc, argv );

    // Inicializa o modo de display. Buffer duplo, RGBA e teste de profundidade.
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

    // Inicializa o tamanho da janela inicial. 600x600 pixels.
    glutInitWindowSize( 600, 600 );

    // Inicializa a posicao inicial da janela.
    glutInitWindowPosition( 100, 100 );

    // Cria a janela definindo um titulo.
    glutCreateWindow( "2D KdTree Adapted" );

    // Registra a funcao de display. Esta função é chamada toda vez que a tela
    // precisa ser redesenhada.
    glutDisplayFunc( display );

    glutKeyboardFunc( keyboard );
    // Resgistra a funcao de reshape. Esta funcao é chamada toda vez que a janela
    // precisa ser redimensionada. Inclusive em sua criacao (momento em que ela
    // aparece pela primeira vez na tela.
    glutReshapeFunc( reshape );

    // Inicia o loop da glut, o que faz o programa OpenGL começar a rodar.
    glutMainLoop( );

    return (EXIT_SUCCESS);
}

