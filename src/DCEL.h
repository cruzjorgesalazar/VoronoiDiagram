#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <cmath>

// Declarar todas las clases por que se usan mutuamente.
class Vertex;
class DirLine;
class Hedge;
class Face;
class DCEL;

class Vertex {
  public:
    Vertex( float, float );

    Vertex() {}; // Agregé un constructor vacío

    void finish_build( Hedge* );

    float norm();

    Vertex operator+( const Vertex& ) const;
    Vertex operator-( const Vertex& ) const;

    // bool operator<( const Vertex& ) const;
    // bool operator>( const Vertex& ) const;
    bool operator==( const Vertex& ) const;

    float x, y; // puse a las coordenadas publicas para poderlas usar en otros lados

  protected:
    // float  x, y;
    Hedge* incident_edge;
};

class DirLine {
  public:
    DirLine( Vertex, Vertex );

    bool IsLeft( const Vertex& ) const;
    bool IsRight( const Vertex& ) const;

    // Ecuación de la recta que pasa por origin y dest, en la forma Ax + By = C.
    // Consideramos está recta con orientación de origin a dest.
    // Esto permite expresar todas las rectas incluyendo verticales.
    float A, B, C;

    Vertex origin, dest;
    // protected:

  private:
    void calc_equation();
};

class Hedge: public DirLine {
  public:
    Hedge( Vertex, Vertex );
    void   finish_build( Hedge* _twin, Face* _incident_face, Hedge* _next, Hedge* _prev );
    Hedge* twin;
    Hedge *next, *prev;

    Face* incident_face;
};

class Face {
  public:
    Face( Vertex );

    void push( Hedge* next_hedge );
    void push( Vertex next_vert );
    void pop();
    void close( bool add_hedge );

    void finish_build( Hedge* _outer_component );

    Vertex get_center();
    Hedge* get_outer_component() const;

  protected:
    Hedge* outer_component;
    Vertex center;
    // inner_component no es necesario.
};

class DCEL {
  public:
    DCEL();

    void update_txt( std::ofstream& file );

  protected:
    std::vector< Vertex > vertices;
    std::vector< Hedge >  hedges;
    std::vector< Face >   faces;
};

///////////////////////
// Extra definitions //
///////////////////////

DirLine bisector( Vertex v1, Vertex v2 );
float   determinant( Vertex a, Vertex b );
bool    is_intersection( Hedge arista, DirLine bisec );

Vertex line_intersection( const Hedge* arista, DirLine bisec );

float dist( Vertex, Vertex );
bool  are_collinear( Vertex, Vertex, Vertex );