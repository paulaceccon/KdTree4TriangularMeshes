/* 
 * File:   PointInPolygon.h
 * Author: jeferson
 *
 * Created on 19 de Outubro de 2013, 11:05
 */

#ifndef POINTINPOLYGON_H
#define	POINTINPOLYGON_H

#include "Point2D.h"

class PointInPolygon
{
public:
    /**
     * Construtor que recebe a lista de pontos de poligono
     * @param polygon - lista de pontos do poligono
     * @param numberPoints - numero de pontos no poligono
     */
    PointInPolygon ( Point2D* polygon, int numberPoints );

    /**
     * Verifica se um ponto p esta dentro (-1), na fronteira (0) ou fora (1) de um
     * poligono. Essa implementacao usa o algoritmo de tracado de raios, onde
     * um raio eh tracado a partir do ponto p para o infinito e numero de
     * intersecoes com o poligono é contada. Se der impar está dentro,
     * se der par está fora.
     * @param p - ponto p a ser testado contra o poligono
     * @return -1 se o ponto estive dentro do poligono, 0 se estiver na fronteira
     * e 1 para o caso do ponto estiver fora do polígono.
     */
    short rayCrossings ( const Point2D& p );

    /**
     * Verifica se um ponto p esta dentro (-1), na fronteira (0) ou fora (1) de um
     * poligono. Essa implementacao usa indice de rotacao, onde o angulo do ponto
     * em relacao a cada segmento de reta é somado com sinal, assim se o ponto
     * estiver dentro o valor do angulo eh 2 PI, caso coontrario o valor do angulo
     * total eh zero
     * @param p - ponto p a ser testado contra o poligono
     * @return -1 se o ponto estive dentro do poligono, 0 se estiver na fronteira
     * e 1 para o caso do ponto estiver fora do polígono.
     */
    short rotationIndex ( const Point2D& p );

    /**
     * Destrutor
     */
    virtual ~PointInPolygon ( );
private:

    /**
     * Lista de ponto que definem o poligono
     */
    Point2D* _polygon;

    /**
     * Numero de pontos do poligono
     */
    int _numberPoints;
    
    /**
     * Ponto a ser testado
     */
    Point2D _testPoint;

    /**
     * Verifica se o ponto p esta sobre a aresta p1p2.
     * @param p1 - ponto da aresta
     * @param p2 - ponto da aresta
     * @return - true se estiver sobre a aresta e false caso contrario
     */
    bool intersectEdge ( const Point2D& p1, const Point2D& p2);

    /**
     * Calcula a intersecao do segmento p1p2 com a reta (x, y) + (1, 0)t
     * @param p1 - ponto do segmento
     * @param p2 - ponto do segmento
     * @param intersection - ponto de intersecao, se existir, calculado
     * @return - true se existir intersecao e false caso contrario
     */
    bool computeIntersectionRay ( const Point2D& p1, const Point2D& p2, Point2D& intersection );
    
    /**
     * Calcula o angulo orientado dos vetores pq rq
     * @return - angulo orientador
     */
    double angle( const Point2D& p,  const Point2D& q,  const Point2D& r);
    
    double pseudoAngle( const Vector2D<double>& v);
};

#endif	/* POINTINPOLYGON_H */

