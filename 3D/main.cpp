#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <stack>
#include "Tipos.h"
#include "Render.h"
#include "KdTree.h"
#include "Ray.h"
#include "IntersectRayAABB.h"
#include "IntersectRayTriangle.h"
//#include "Timer.h"
#include "ConvexHull.h"
#include "Point2D.h"
#include "PointInPolygon.h"
//#include <omp.h>

using namespace std;

//controle da camera
Camera cam;

//vetor que determina a teclas clicadas
bool teclas[256] = { false };

//variaveis que armazenam a dimensao da janela principal
float displayW = 500, displayH = 50000;

//variavel que armazena o zoom da camerda
float camZoom = 0;

//variavel pra determinar se a grade xy deve ser renderizada ou nao
bool gridXY = true;

//vetores para iluminacao
GLfloat light_position[] = { 0, -200, 100 };
GLfloat light_position2[] = { 100, 100, 100 };
GLfloat light_position3[] = { -100, 100, 100 };
GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

// variaveis que armazenam a posicao inicial do mouse quando este eh arrastado na tela
int lastPositionMouseMoveX = 0, lastPositionMouseMoveY = 0;

//variavel para armazenar se houve um mouse press na janela principal
bool mousePressLeftButton = false;

//variavel que  armazena a velocidade corrente da camera
double velocityCamera = 0.1;

//vetor de normais da superf�cie
float *normals = 0;

//vetor de coordenaas
float* coordinates = 0;

//vetor com triangulacao
int* triangles = 0;

//instancia da arvore
KdTree* kdTree = 0;

//numero de pontos
int numberPoints = 0;

//numero de triangulos
int numberTriangles = 0;

//angulo de abertura da camera
float fov = 0.0f;

//variavel que determina se a superficie vai ser vizualizada como malha ou faces
bool fillSurface = true;

//variavel pra determinar se a supeficie deve ser renderizada ou nao
bool viewSurface = true;

//varievel que armazena o ID da janela
int janela3D = 0;

int depth = 0;

int cont;
int contAABBTest = 0;


Point3D origin( 0.0, 0.5, 0.0 );
Vector3D<float> direction( 0.0, 0.0, 0.0 );

float xMed, yMed, zMed;

std::vector< Point3D > insideSurface;
int incrementGrid = 1;

bool renderCubes = false;



void renderCube( float xMin, float yMin, float zMin, float xMax, float yMax, float zMax )
{
    float points[8][3] = {
        {xMin, yMin, zMin }, //0
        {xMin, yMin, zMax }, //1
        {xMin, yMax, zMin }, //2
        {xMin, yMax, zMax }, //3
        {xMax, yMin, zMin }, //4
        {xMax, yMin, zMax }, //5
        {xMax, yMax, zMin }, //6
        {xMax, yMax, zMax }//7
    };

    int quads[24] = {
        5, 7, 3, 1,
        0, 2, 6, 4,
        4, 6, 7, 5,
        6, 2, 3, 7,
        2, 0, 1, 3,
        0, 4, 5, 1
    };

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 3, GL_FLOAT, 0, points );

    glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, quads );

    glDisableClientState( GL_VERTEX_ARRAY );

    glDisable( GL_BLEND );
}



void renderNode( const Node * node, int d )
{
    if (node == 0 || d >= depth)
    {
        return;
    }

    if (d == depth - 1)
    {
        float increment = 1.0f / numberTriangles;
        glColor4f( cont * increment, 1.0f, 0.0, 0.6 );
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        renderCube( node->minX, node->minY, node->minZ, node->maxX, node->maxY, node->maxZ );

        glColor3f( 1.0, 0.0, 0.0 );
        glLineWidth( 4.0f );
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        renderCube( node->minX, node->minY, node->minZ, node->maxX, node->maxY, node->maxZ );
        glLineWidth( 1.0f );
    }
    cont++;

    renderNode( node->left, d + 1 );
    renderNode( node->right, d + 1 );
}



void renderKdTree( )
{
    if (kdTree)
    {
        cont = 0;
        renderNode( kdTree->getRoot( ), 0 );
    }
}



void renderRay( )
{
    glLineWidth( 6.0f );
    glColor3f( 0.0f, 0.0f, 1.0f );

    glBegin( GL_LINES );
    glVertex3f( origin.x, origin.y, origin.z );
    glVertex3f( origin.x + 1000 * direction.x, origin.y + 1000 * direction.y, origin.z + 1000 * direction.z );
    glEnd( );

    glLineWidth( 1.0f );
}



void renderInsideSurface( )
{
    if (!kdTree)
        return;

    glColor3f( 1.0, 0.5, 0.0 );
    glBegin( GL_POINTS );
    //glutSolidCube( 1000 );   
    for (unsigned int i = 0; i < insideSurface.size( ); i++)
    {
        glPushMatrix( );
        glTranslated( insideSurface[i].x, insideSurface[i].y, insideSurface[i].z );
        glutSolidCube( 1 );
        glPopMatrix( );
    }
    glEnd( );
}



/**
 * Funcao que renderiza a janela principal
 */
void display( void )
{
    glutSetWindow( janela3D );
    glViewport( 0, 0, (int) displayW, (int) displayH );
    glClearColor( 1.0, 1.0, 1.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    if (teclas[(int) '4'])
        cam.rotateX -= cam.velocityRotation;

    if (teclas[(int) '6'])
        cam.rotateX += cam.velocityRotation;

    if (teclas[(int) '8'])
        cam.rotateY -= cam.velocityRotation;

    if (teclas[(int) '2'])
        cam.rotateY += cam.velocityRotation;

    if (teclas[(int) 'w'])
    {
        cam.velocity = velocityCamera;
    }
    else if (teclas[(int) 's'])
    {
        cam.velocity = -velocityCamera;
    }
    else
    {
        cam.velocity = 0.0;
    }

    if (teclas[(int) 'a'])
    {
        cam.lateralVelocity = -velocityCamera;
    }
    else if (teclas[(int) 'd'])
    {
        cam.lateralVelocity = +velocityCamera;
    }
    else
    {
        cam.lateralVelocity = 0.0;
    }

    if (cam.rotateY > 90)
        cam.rotateY = 90;

    if (cam.rotateY <-90)
        cam.rotateY = -90;

    cam.walkingDirectionX = sin( PI * cam.rotateX / 180.0 ) * cos( PI * cam.rotateY / 180.0 );
    cam.walkingDirectionZ = sin( PI * cam.rotateY / 180.0 );
    cam.walkingDirectionY = cos( PI * cam.rotateX / 180.0 ) * cos( PI * cam.rotateY / 180.0 );
    cam.walkingLateralDirectionX = sin( PI * (cam.rotateX + 90) / 180.0 ) * cos( PI * cam.rotateY / 180.0 );
    cam.walkingLateralDirectionZ = sin( PI * cam.rotateY / 180.0 );
    cam.walkingLateralDirectionY = cos( PI * (cam.rotateX + 90) / 180.0 ) * cos( PI * cam.rotateY / 180.0 );

    cam.targetX = cam.walkingDirectionX * 3 + cam.positionX;
    cam.targetY = cam.walkingDirectionY * 3 + cam.positionY;
    cam.targetZ = cam.walkingDirectionZ * 3 + cam.positionZ;
    cam.positionX += cam.walkingDirectionX * cam.velocity;
    cam.positionY += cam.walkingDirectionY * cam.velocity;
    cam.positionZ += cam.walkingDirectionZ * cam.velocity;
    cam.positionX += cam.walkingLateralDirectionX * cam.lateralVelocity;
    cam.positionY += cam.walkingLateralDirectionY * cam.lateralVelocity;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    fov = 45 + camZoom;
    if (fov < 5.0f) fov = 5.0f;
    if (fov > 160.0f) fov = 160.0f;

    gluPerspective( fov, (float) displayW / (float) displayH, 1.0f, 50000.0f );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluLookAt( cam.positionX, cam.positionY, cam.positionZ, // eye
        cam.targetX, cam.targetY, cam.targetZ, // target
        0, 0, 1 ); // up
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT1, GL_POSITION, light_position2 );
    glLightfv( GL_LIGHT2, GL_POSITION, light_position3 );

    glColor3f( 1.0, 0.0, 0.0 );

    if (gridXY)
    {
        renderGridXY( );
    }

    glEnable( GL_LIGHTING );
    if (viewSurface)
    {
        glPushMatrix( );
        //glScalef( 0.10f, 0.10f, 0.50f );
        renderSurfaceWireframe( coordinates, normals, triangles, numberTriangles, fillSurface );
        renderKdTree( );
        renderRay( );
        glPopMatrix( );
    }
    if (renderCubes)
        renderInsideSurface( );
    
    glDisable( GL_LIGHTING );
    glFlush( );
    glutSwapBuffers( );
}



/**
 * Funcao que recebe evendo de quando um tecla � solta do teclado
 * @param key - tecla
 * @param x - posicao x do mouse na tela
 * @param y - posicao y do mouse na tela
 */
void keyboardUpInput( unsigned char key, int x, int y )
{
    int a = x;
    a = y;
    teclas[key] = false;
}



/**
 * Funcao de idle da janela principal. Evita que a janela seja travada
 */
void idleWindow3D( void )
{
    glutSetWindow( janela3D );
    glutPostRedisplay( );
}



/**
 * Janela que recebe eventos de reshape da janela 3D
 * @param w - largura da janela em pixeis
 * @param h - altura da janela em pixeis
 */
void reshapeWindow3D( int w, int h )
{
    glutSetWindow( janela3D );
    displayW = w;
    displayH = h;
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 90, (float) w / h, 2, 100 );
    glMatrixMode( GL_MODELVIEW );
    glutPostWindowRedisplay( janela3D );
}



/**
 * Mouse move na janela princial
 * @param x - coordenada x corrente
 * @param y - coordenada y corrente
 */
void mouseMoveWindow3D( GLsizei x, GLsizei y )
{
    if (mousePressLeftButton)
    {
        cam.rotateX += x - lastPositionMouseMoveX;
        cam.rotateY -= y - lastPositionMouseMoveY;
        lastPositionMouseMoveX = x;
        lastPositionMouseMoveY = y;
    }
}



void computeNormals( )
{
    // Inicializa as normais
    for (int i = 0; i < 3 * numberPoints; i++)
        normals[i] = 0.0;

    // Percorre as faces calculando
    for (int i = 0; i < numberTriangles; i++)
    {
        int v0, v1, v2;

        v0 = triangles[3 * i + 0];
        v1 = triangles[3 * i + 1];
        v2 = triangles[3 * i + 2];

        float v1x = coordinates[ 3 * v1 + 0 ] - coordinates[ 3 * v0 + 0 ];
        float v1y = coordinates[ 3 * v1 + 1 ] - coordinates[ 3 * v0 + 1 ];
        float v1z = coordinates[ 3 * v1 + 2 ] - coordinates[ 3 * v0 + 2 ];

        float v2x = coordinates[ 3 * v2 + 0 ] - coordinates[ 3 * v0 + 0 ];
        float v2y = coordinates[ 3 * v2 + 1 ] - coordinates[ 3 * v0 + 1 ];
        float v2z = coordinates[ 3 * v2 + 2 ] - coordinates[ 3 * v0 + 2 ];

        float x = v1y * v2z - v2y * v1z;
        float y = v1z * v2x - v1x * v2z;
        float z = v1x * v2y - v1y * v2x;

        // Adiciona a normal da face a todos os vertices da face
        normals[3 * v0 + 0] += x;
        normals[3 * v0 + 1] += y;
        normals[3 * v0 + 2] += z;

        normals[3 * v1 + 0] += x;
        normals[3 * v1 + 1] += y;
        normals[3 * v1 + 2] += z;

        normals[3 * v2 + 0] += x;
        normals[3 * v2 + 1] += y;
        normals[3 * v2 + 2] += z;
    }

    // normaliza??o das componentes
    for (int i = 0; i < numberPoints; i++)
    {
        float nx, ny, nz;
        nx = normals[3 * i + 0];
        ny = normals[3 * i + 1];
        nz = normals[3 * i + 2];

        float lenght = sqrt( nx * nx + ny * ny + nz * nz );

        if (lenght != 0.0f)
        {
            normals[3 * i + 0] /= lenght;
            normals[3 * i + 1] /= lenght;
            normals[3 * i + 2] /= lenght;
        }
    }
}



int getNumIntersection( const Node* n, const Ray& r, bool &bord )
{
    if (n == 0 || bord)
    {
        return 0;
    }

    int cont = 0;
    //contAABBTest++;
    if (intersectRayAABB( r, n->minX, n->minY, n->minZ, n->maxX, n->maxY, n->maxZ ))
    {
        int triangleIndex = n->triangleIndex;

        int v0 = triangles[3 * triangleIndex + 0];
        int v1 = triangles[3 * triangleIndex + 1];
        int v2 = triangles[3 * triangleIndex + 2];

        float x1, x2, x3, y1, y2, y3, z1, z2, z3;
        x1 = coordinates[3 * v0 + 0];
        x2 = coordinates[3 * v1 + 0];
        x3 = coordinates[3 * v2 + 0];

        y1 = coordinates[3 * v0 + 1];
        y2 = coordinates[3 * v1 + 1];
        y3 = coordinates[3 * v2 + 1];

        z1 = coordinates[3 * v0 + 2];
        z2 = coordinates[3 * v1 + 2];
        z3 = coordinates[3 * v2 + 2];

        int ret = intersectRayTriangle( r, x1, y1, z1, x2, y2, z2, x3, y3, z3 );
        if (ret == 2)
        {
            bord = true;
        }
        if (ret)
        {
            cont++;
        }
        cont += getNumIntersection( n->left, r, bord );
        cont += getNumIntersection( n->right, r, bord );
    }
    return cont;
}



void getDirection( Vector3D<float> & direction, float x, float y, float z )
{
    if (x > xMed)
    {
        direction.x = 1.0f;
    }
    else
    {
        direction.x = -1.0f;
    }

    if (y > yMed)
    {
        direction.y = 1.0f;
    }
    else
    {
        direction.y = -1.0f;
    }

    if (z > zMed)
    {
        direction.z = 1.0f;
    }
    else
    {
        direction.z = -1.0f;
    }
}



int testIntersections( float x = 0, float y = 0, float z = 0 )
{
    bool bord = false;
    int nIntersections = 0;
    int a = 0;
    do
    {
        Point3D origin = Point3D( x, y, z );
        //        int triangleIndex = rand( ) % numberTriangles;
        //
        //        int v0 = triangles[3 * triangleIndex + 0];
        //        int v1 = triangles[3 * triangleIndex + 1];
        //        int v2 = triangles[3 * triangleIndex + 2];
        //
        //        float x1, x2, x3, y1, y2, y3, z1, z2, z3;
        //        x1 = coordinates[3 * v0 + 0];
        //        x2 = coordinates[3 * v1 + 0];
        //        x3 = coordinates[3 * v2 + 0];
        //
        //        y1 = coordinates[3 * v0 + 1];
        //        y2 = coordinates[3 * v1 + 1];
        //        y3 = coordinates[3 * v2 + 1];
        //
        //        z1 = coordinates[3 * v0 + 2];
        //        z2 = coordinates[3 * v1 + 2];
        //        z3 = coordinates[3 * v2 + 2];
        //
        //        float xM = (x1 + x2 + x3) / 3.0;
        //        float yM = (y1 + y2 + y3) / 3.0;
        //        float zM = (z1 + z2 + z3) / 3.0;
        //        
        //        

        Point3D direction;
        if (a == 0)
        {
            //getDirection( direction, x, y, z );
            if (z >= (kdTree->getRoot( )->minZ + kdTree->getRoot( )->maxZ) / 2.0)
            {
                direction = Vector3D<float> (0.0, 0.0, 1);
            }
            else
            {
                direction = Vector3D<float> (0.0, 0.0, -1);
            }
        }
        else
        {
            direction = Vector3D<float> (rand( ) % 10, rand( ) % 10, rand( ) % 10);
        }

        bord = false;
        Ray r( origin, direction );
        nIntersections = getNumIntersection( kdTree->getRoot( ), r, bord );
        //        if (bord)
        //            printf( "borda\n" );
        a++;
        if (a > 100)
        {
            printf( "Numero maximo de iteracoes\n" );
            break;
        }
    }
    while (bord);



    return nIntersections;
}



void openMesh( )
{
    std::string fileName;
    // printf( "Digite o nome do arquivo: " );
    //std::cin >> fileName;
    fileName = "Demos/icosaedro.txt";

    std::ifstream in( fileName.c_str( ) );
    if (in.fail( ))
    {
        printf( "Falha ao abrir arquivo\n" );
        return;
    }
    in >> numberPoints >> numberTriangles;

    printf( "Arquivo carregado\n" );
    if (normals != 0)
    {
        delete [] normals;
    }
    if (coordinates != 0)
    {
        delete []coordinates;
    }
    if (triangles != 0)
    {
        delete []triangles;
    }

    normals = new float[3 * numberPoints];
    coordinates = new float[3 * numberPoints];
    triangles = new int[3 * numberTriangles];

    int lixo;
    for (int i = 0; i < numberPoints; i++)
    {
        in >> lixo >> coordinates[3 * i] >> coordinates[3 * i + 1] >> coordinates[3 * i + 2];
        coordinates[3 * i + 0] *= 50.f;
        coordinates[3 * i + 1] *= 50.f;
        coordinates[3 * i + 2] *= 50.f;
    }
    for (int i = 0; i < numberTriangles; i++)
    {
        in >> lixo >> triangles[3 * i] >> triangles[3 * i + 1] >> triangles[3 * i + 2];
        if (fileName == "Demos/superficie_teste.txt")
        {
                triangles[3 * i]--;
                triangles[3 * i + 1]--;
                triangles[3 * i + 2]--;
        }
    }
    computeNormals( );

    if (kdTree != 0)
    {
        delete kdTree;
    }

    kdTree = new KdTree( coordinates, triangles, numberTriangles, 3 );

    if (fileName == "Demos/superficie_teste.txt")
    {
        for (int i = 0; i < numberPoints; i++)
        {
            coordinates[3 * i + 0] = coordinates[3 * i + 0] - kdTree->getRoot( )->minX;
            coordinates[3 * i + 1] = coordinates[3 * i + 1] - kdTree->getRoot( )->minY;
            coordinates[3 * i + 2] = coordinates[3 * i + 2] - kdTree->getRoot( )->minZ;

            //            coordinates[3 * i + 0] *= 0.1f;
            //            coordinates[3 * i + 1] *= 0.1f;
            //            coordinates[3 * i + 2] *= 0.5f;
        }
    }
    delete kdTree;
    kdTree = new KdTree( coordinates, triangles, numberTriangles, 3 );
    testIntersections( );

    printf( "(%f, %f, %f)\n(%f, %f,%f)\n", kdTree->getRoot( )->minX, kdTree->getRoot( )->minY, kdTree->getRoot( )->minZ,
        kdTree->getRoot( )->maxX, kdTree->getRoot( )->maxY, kdTree->getRoot( )->maxZ );

    xMed = (kdTree->getRoot( )->minX + kdTree->getRoot( )->maxX) / 2.0;
    yMed = (kdTree->getRoot( )->minY + kdTree->getRoot( )->maxY) / 2.0;
    zMed = (kdTree->getRoot( )->minZ + kdTree->getRoot( )->maxZ) / 2.0;
}



void computeConvexHull( std::vector<Point2D >& convexHull )
{
    convexHull.reserve( numberPoints );
    for (int i = 0; i < numberPoints; i++)
    {
        convexHull.push_back( Point2D( coordinates[3 * i], coordinates[3 * i + 1] ) );
    }
    ConvexHull c( &convexHull[0], numberPoints );
    convexHull = c.grahamScan( );
}



void computeSamplesInsideSurface( )
{
    if (!kdTree)
        return;

    //Timer t, t1;

    std::vector< Point2D > convexHull;
    computeConvexHull( convexHull );
    //t1.printTime( "Tempo para construir convexHull(s): " );

    PointInPolygon p( &convexHull[0], convexHull.size( ) );

    //omp_set_num_threads( 8 );
    //printf( "Numero de threads: %d %d\n", omp_get_max_threads( ), omp_get_num_procs( ) );

    const int a1 = kdTree->getRoot( )->minX, a2 = kdTree->getRoot( )->maxX;
    const int b1 = (int) kdTree->getRoot( )->minY, b2 = (int) kdTree->getRoot( )->maxY;
    const int c1 = (int) kdTree->getRoot( )->minZ, c2 = (int) kdTree->getRoot( )->maxZ;

    printf( "Total de amostras: %ld\n", (long int) (a2 - a1) * (b2 - b1) * (c2 - c1) / (incrementGrid * incrementGrid * incrementGrid) );
    srand( time( NULL ) );

    //#pragma omp parallel num_threads(7)
    {
        //#pragma omp for schedule( guided )

        for (int x = a1; x < a2; x += incrementGrid)
        {
            //printf( "%d\n", x );
            //#pragma omp parallel for
            for (int y = b1; y < b2; y += incrementGrid)
            {
                if (p.rotationIndex( Point2D( x, y ) ) == 1)
                {
                    continue;
                }
                //#pragma omp parallel for 
                for (int z = c1; z < c2; z += incrementGrid)
                {
                    if (testIntersections( x, y, z ) % 2)
                    {
                        insideSurface.push_back( Point3D( x, y, z ) );
                    }
                    //                totalTestAABB += contAABBTest;
                    //                if (n % 2)
                    //                {
                    //                    outside++;
                    //                }
                    //                else
                    //                {
                    //                    inside++;
                    //                }
                }
            }
        }
    }
    //    printf( "Inside: %d\nOutside:%d\n", inside, outside );
    //    printf( "Numero de testes medio por amostra: %f\n", (float) totalTestAABB / (inside + outside) );
    //    printf( "Total de amostras: %ld\n", (long int) (a2 - a1) * (b2 - b1) * (c2 - c1) / (increment * increment * increment) );
    //t.printTime( "Tempo (s)" );
}



/**
 * Funcao que recebe eventos de teclado na janela principal
 * @param key - tecla que enviou o evento
 * @param x - coordenada x do mouse em pixeis
 * @param y - coordenada y do mouse em pixeis
 */
void keyboardInput( unsigned char key, int x, int y )
{
    int a = x;
    a = y;
    teclas[key] = true;
    switch (key)
    {
        case 'A':
            openMesh( );
            break;
        case 'p':
            fillSurface = !fillSurface;
            break;
        case 'q':
            viewSurface = !viewSurface;
            break;
        case '+':
            if (velocityCamera < 90)
            {
                velocityCamera += 0.1;
            }
            break;
        case '-':
            if (velocityCamera >= 0)
            {
                velocityCamera -= 0.1;
            }
            break;
        case '/':
            if (camZoom > 0)camZoom -= 1;
            break;
        case 'g':
            gridXY = !gridXY;
            break;
        case '*':
            if (camZoom < 150)camZoom += 1;
            break;
        case 'b':
            depth--;
            if (depth < 0)
                depth = 0;
            break;
        case 'B':
            depth++;
            break;
        case 'T':
        case 't':
            testIntersections( );
            break;
        case 'R':
            computeSamplesInsideSurface( );

        case 'c':
        case 'C':
            renderCubes = !renderCubes;
            break;
    }
    display( );
}



/**
 * Recebe evento de mouse na janela principal
 * @param button - botao que envia o evento
 * @param state - estado do botao
 * @param x - coordenada x do mouse na tela em pixeis
 * @param y - coordenada y do mouse na tela em pixeis
 */
void mouseWindow3D( int button, int state, int x, int y )
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            lastPositionMouseMoveX = x;
            lastPositionMouseMoveY = y;
            mousePressLeftButton = true;
        }
    }
    if (state == GLUT_UP)
    {



        mousePressLeftButton = false;
    }
    glutSetWindow( janela3D );
    glutPostRedisplay( );
}



/**
 * Funcao para inicializar os parametros que forem necessarios
 */
void myInit( void )
{



    glutSetWindow( janela3D );

    //luz 1
    GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    //luz 2   
    glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular );

    glEnable( GL_LIGHT1 );

    glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular );

    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHT2 );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_COLOR_MATERIAL );


    glShadeModel( GL_SMOOTH );
    //  glShadeModel ( GL_FLAT );
    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );

    glColor4f( 1.0f, 1.0f, 1.0f, 0.5f ); // Full Brightness, 50% Alpha ( NEW )
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 90, 1, 2, 100 );

    glMatrixMode( GL_MODELVIEW );

    glClearColor( 1, 1, 1, 0 );
}



int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    //janela 3D
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 600, 600 );
    glutInitWindowPosition( 500, 50 );
    janela3D = glutCreateWindow( "Surface" );
    glutKeyboardFunc( keyboardInput );
    glutKeyboardUpFunc( keyboardUpInput );
    glutReshapeFunc( reshapeWindow3D );
    glutDisplayFunc( display );
    glutIdleFunc( idleWindow3D );
    glutMotionFunc( mouseMoveWindow3D );
    glutMouseFunc( mouseWindow3D );
    myInit( );
    glutMainLoop( );

    if (normals != 0)
    {
        delete[] normals;
    }
    //delete[]noDataPoints;
    return 0;
}
