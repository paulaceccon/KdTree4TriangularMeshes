/* 
 * File:   Vector.h
 * Author: jeferson
 *
 * Created on 23 de Setembro de 2013, 20:28
 */

#ifndef VECTOR3D_H
#define	VECTOR3D_H

#include <cmath>
#include <cstdio>
#include <fstream>

template <class T>
class Vector3D
{
public:
    T x, y, z;



    /**
     * Construtor default
     */
    Vector3D( )
    {
        x = y = z = 0.0;
    }



    /**
     * Construtor com parametros
     * @param vx - coordenada x do vetor
     * @param vy - coordenada y do vetor
     * @param vz - coordenada z do vetor
     */
    Vector3D( T vx, T vy, T vz )
    : x( vx ), y( vy ), z( vz )
    {
    }



    /**
     * Construtor com parametros
     * @param v - vetor de floar com tres posicoes correspondentes as coordenadas
     * x, y e z do vetor
     */
    Vector3D( const T v[3] )
    : x( v[0] ), y( v[1] ), z( v[2] )
    {
    }



    /**
     * Destrutor
     */
    ~Vector3D( )
    {
    }



    /**
     * Define novas coordenadas para o vetor
     * @param vx - coordenada x do vetor
     * @param vy - coordenada y do vetor
     * @param vz - coordenada z do vetor
     */
    void set( T vx, T vy, T vz )
    {
        x = vx;
        y = vy;
        z = vz;
    }



    /**     
     * Define novas coordenadas para o vetor
     * @param v - vetor de floar com tres posicoes correspondentes as coordenadas
     * x, y e z do vetor
     */
    void set( const T v[3] )
    {
        x = v[0];
        y = v[1];
        z = v[2];
    }



    /**
     * Realiza o produto escalar entre o vetor corrente e o vetor q.
     * @param q - vetor o qual sera realizado produto escalar com o vetor
     * corrente
     * @return - valor do produto escalar
     */
    inline T dot( const Vector3D& q ) const
    {
        return x * q.x + y * q.y + z * q.z;
    }



    /**
     * Realiza produto vetorial entre os vetores a e b (a X b) e escreve
     * o resultado no no vetor corrente.
     * @param a - vetor a
     * @param b - vetor b
     */
    inline void cross( const Vector3D& a, const Vector3D& b )
    {
        x = a.y * b.z - a.z * b.y;
        y = a.z * b.x - a.x * b.z;
        z = a.x * b.y - a.y * b.x;
    }



    /**
     * Calcula o comprimento ao quadrado do vetor
     * @return - comprimento ao quadrado do vetor corrente
     */
    inline T sqrLength( ) const
    {
        return (x * x + y * y + z * z);
    }



    /**
     * Calcula o comprimento do vetor
     * @return  - comprimento do vetor
     */
    inline T length( ) const
    {
        return (T) sqrt( x * x + y * y + z * z );
    }



    /**
     * Normaliza o vetor e retorna a norma que o vetor tinha antes de ser
     * normalizado
     * @return - norma do vetor antes de ser normalizado
     */
    inline T normalize( )
    {
        T l = length( );
        if (l != (T) 0.0)
        {
            T d = (T) 1.0 / l;
            x *= d;
            y *= d;
            z *= d;
        }
        return l;
    }



    /**
     * Calculo o angulo entre dois vetores, o corrente e o vetor v passado por
     * parametro.
     * @param v - vetor v passado por parametro para ser calculado o angulo
     * com o vetor corrente
     * @return - angulo entre o vetor v e o vetor corrente
     */
    inline T angle( const Vector3D& v ) const
    {
        return (T) (acos( dot( v ) / (length( ) * v.length( )) ));
    }



    /**
     * Imprime o vetor com uma mensagem passada por parametro
     * @param label - mensagem impressa antes das coordenadas do vetor
     */
    inline void print( const char* label ) const
    {
        printf( "%s = {%g, %g, %g}\n", label, x, y, z );
    }



    /**
     * Sobrecarga do operador +=. Atribui ao vetor corrente a soma das suas coordenadas
     * com as coordenas de outro vetor passado por parametro. Exemplo de uso:
     * a += b.
     * @param other- vetor a direita da operacao de +=
     * @return - retorna uma referencia para o vetor corrente com ja com o resultado
     * da operacao
     */
    inline Vector3D& operator+=(const Vector3D& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }



    /**
     * Sobrecarga do operador +=. Adiciona um vetor escalar as coordenadas do 
     * vetor. Eh equivalente a somar o vetor corrente pelo vetor 
     * (scalar, scalar, scalar).
     * @param scalar - valor escalar a ser somado
     * @return - vetor resultado da soma
     */
    inline Vector3D& operator+=(T scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }



    /**
     * Sobrecarga do operador -=. Atribui ao vetor corrente a subtracao das suas coordenadas
     * pelas coordenas de outro vetor passado por parametro. Exemplo de uso:
     * a -= b.
     * @param other- vetor a direita da operacao de -=
     * @return - retorna uma referencia para o vetor corrente com ja com o resultado
     * da operacao.
     */
    inline Vector3D& operator-=(const Vector3D& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }



    /**
     * Sobrecarga do operador -=. Adiciona um vetor escalar as coordenadas do 
     * vetor. Eh equivalente a subtrair o vetor corrente pelo vetor 
     * (scalar, scalar, scalar).
     * @param scalar - valor escalar a ser subtraido
     * @return - vetor resultado da soma
     */
    inline Vector3D& operator-=(T scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }



    /**
     * Sobrecarga do operador unario -. Inverte o sinal de todas as coordenadas
     * do vetor corrente e salva em um novo vetor
     * @return - retorna novo vetor com as coordenadas com sinal trocado
     */
    inline Vector3D operator-()
    {
        Vector3D v( -x, -y, -z );
        return v;
    }



    /**
     * Sobrecarga do operador *= entre vetores. Multiplica coordenada a coordenada
     * dois vetores (o corrente e o passado por parametro) e salva o resultado
     * no vetor corrente. Exemplo de uso: a *= b.
     * @param other - vetor a ser multiplicado, coordenada a coordenada, pelo
     * vetor corrente
     * @return - retorna referencia para vetor corrente ja com a operacao aplicada
     */
    inline Vector3D& operator*=(const Vector3D& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }



    /**
     * Sobrecarga do operador *= entre vetor e escalar. Multiplica coordenada a
     * coordenada o vetor corrente por um escalar.
     * @param scalar - escalar a ser multiplicado pelo vetor corrente
     * @return - retorna referencia para vetor corrente ja com a operacao aplicada
     */
    inline Vector3D& operator*=(T scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }



    /**
     * Sobrecarga do operador /= . Divide todas as coordenadas do vetor corrente
     * por um escalar.
     * @param scalar - escalar a ser dividido pelo vetor corrente
     * @return - retorna referencia para vetor corrente ja com a operacao aplicada
     */
    inline Vector3D& operator/=(T scalar)
    {
        if (scalar != (T) 0.0)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }
        return *this;
    }
    //* Free operators



    inline friend bool operator ==(const Vector3D& v1, const Vector3D& v2)
    {
        return ( fabs( v1.x - v2.x ) < eps) && (fabs( v1.y - v2.y ) < eps) && (fabs( v1.z - v2.z ) < eps);
    }



    inline friend bool operator !=(const Vector3D& v1, const Vector3D& v2)
    {
        return ( fabs( v1.x - v2.x ) > eps) || (fabs( v1.y - v2.y ) > eps) || (fabs( v1.z - v2.z ) > eps);
    }



    /**
     * Sobrecarga do operador + entre dois vetores: Soma dois vetores e retorna
     * resultado da soma
     * @param one - primeiro vetor
     * @param other - segundo vetor
     * @return - vetor com o resultado da soma dos dois vetores
     */
    inline friend Vector3D operator+(const Vector3D& one, const Vector3D& other)
    {
        Vector3D res( one );
        return res += other;
    }



    /**
     * Sobrecarga do operador + entre vetor e escalar: Soma um valor escalar
     * a todas as coordenadas do vetor. Exemplo de usoo: vetor + escalar
     * @param one - vetor
     * @param scalar - escalar que sera somado a todas as coordenadas do vetor
     * @return - vetor com o resultado da soma do vetor pelo escalars
     */
    inline friend Vector3D operator+(const Vector3D& one, T scalar)
    {
        Vector3D res( one );
        return res += scalar;
    }



    /**
     * Sobrecarga do operador + entre vetor e escalar: Soma um valor escalar
     * a todas as coordenadas do vetor. Exemplo de usoo: escalar + vetor
     * @param scalar - escalar que sera somado a todas as coordenadas do vetor
     * @param one - vetor
     * @return - vetor com o resultado da soma do vetor pelo escalars
     */
    inline friend Vector3D operator+(T scalar, const Vector3D& one)
    {
        Vector3D res( one );
        return res += scalar;
    }



    /**
     * Sobrecarga do operador - entre vetores: Subtrai um vetor pelo outro
     * @param one - primeiro vetor
     * @param other - segundo vetor
     * @return - vetor com o resultado da subtracao entre os vetores
     */
    inline friend Vector3D operator-(const Vector3D& one, const Vector3D& other)
    {
        Vector3D res( one );
        return res -= other;
    }



    /**
     * Sobrecarga do operador - entre vetor e escalar: Subtrai um valor escalar
     * a todas as coordenadas do vetor. Exemplo de uso: vetor - scalar
     * @param one - vetor
     * @param scalar - escalar que sera subtraido de todas as coordenadas do vetor
     * @return - vetor com o resultado da subtração pelo escalar
     */
    inline friend Vector3D operator-(const Vector3D& one, T scalar)
    {
        Vector3D res( one );
        return res -= scalar;
    }



    /**
     * Sobrecarga do operador - entre vetor e escalar: Subtrai um valor escalar
     * a todas as coordenadas do vetor. Equivalente a (scalar, scalar, scalar) -
     * (v1, v2, v3). Exemplo de uso: scalar - vetor
     * @param scalar - escalar que sera subtraido de todas as coordenadas do vetor
     * @param one - vetor
     * @return - vetor com o resultado da subtração pelo escalar
     */
    inline friend Vector3D operator-(T scalar, const Vector3D& one)
    {
        Vector3D res( one );
        res *= -1;
        return res += scalar;
    }



    /**
     * Multiplica dois vetores componente a componente
     * @param one - primeiro vetor
     * @param other - segundo vetor
     * @return - vetor resultante da multiplicacao componente a componente
     */
    inline friend double operator*(const Vector3D& one, const Vector3D& other)
    {
        return one.x * other.x + one.y * other.y + one.z * other.z;
    }



    /**
     * Multiplica todas as componentes de um vetor po um escalar
     * @param one - vetor
     * @param scalar - escalar
     * @return - vetor resultante da multiplicacao do escalar por todas as
     * componentes
     */
    inline friend Vector3D operator*(const Vector3D& one, T scalar)
    {
        Vector3D res( one );
        return res *= scalar;
    }



    /**
     * Multiplica todas as componentes de um vetor po um escalar
     * @param scalar - escalar
     * @param one - vetor
     * @return - vetor resultante da multiplicacao do escalar por todas as
     * componentes
     */
    inline friend Vector3D operator*(T scalar, const Vector3D& one)
    {
        Vector3D res( one );
        return res *= scalar;
    }



    /**
     * Divide todas as componentes de um vetor por escalar
     * @param one - vetor
     * @param scalar - escalar
     * @return vetor resultante da divisao das coordenadas por um escalar
     */
    inline friend Vector3D operator/(const Vector3D& one, T scalar)
    {
        Vector3D res( one );
        return res /= scalar;
    }



    /**
     * Controi um novo vetor onde cada coordenada e resultado da divisao do 
     * escalar pela coordenada do vetor
     * @param scalar - escalar
     * @param one - vetor
     * @return - vetor resultante da operacaos
     */
    inline friend Vector3D operator/(T scalar, const Vector3D& one)
    {
        Vector3D res( scalar / one.x, scalar / one.y, scalar / one.z );
        return res;
    }



    /**
     * Sobrecarrega do operador de % para realizar o produto vetorial de dois
     * vetores
     * @param a - primeiro vetor
     * @param b - segundo vetor
     * @return - vetor resultante do produto vetorial entre a e b
     */
    inline friend Vector3D operator %(const Vector3D& a, const Vector3D& b)
    {
        Vector3D res( a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y );
        return res;
    }



    friend std::ostream& operator <<(std::ostream& o, const Vector3D& v)
    {
        return o << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
    static const double eps = 10E-5;
};

typedef Vector3D<float> Point3D;

#endif	/* VECTOR3D_H */

