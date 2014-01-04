/* Philipp Moritz <pcmoritz@gmail.com>, written in January 2014 */

#ifndef PCL_HPP
#define PCL_HPP

#include <istream>

namespace tetwrap {

/* A class for three dimensional points in cartesian coordinates */
class point {
  REAL x, y, z;
  friend std::ostream& operator<<(std::ostream& out, const point& p);
public:
  point(REAL x, REAL y, REAL z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  REAL x() const { return x; }
  REAL y() const { return y; }
  REAL z() const { return z; }
  
  void x(REAL x) { this->x = x; }
  void y(REAL y) { this->y = y; }
  void z(REAL z) { this->z = z; }
};

/* A facet that could be part of a surface */
class facet {
  friend std::ostream& operator<<(std::ostream& out, const facet& f);
  std::vector<std::size_t> vertices;
public:
  std::size_t vertex(std::size_t index) { return vertices[index]; }
  void add_vertex(std::size_t value) {vertices.push_back(value); }
};

/* A surface that is the boundary of compact and connected body */
class surface {
  std::vector<point> vertices;
  std::vector<facet> facets;
public:
  void add_vertex(point v) { vertices.push_back(v); }
  void add_facet(const facet& f) { facets.push_back(f); }
  std::size_t num_vertices() const { return vertices.size(); }
  std::size_t vertex(std::size_t index) const { return vertices[index]; }
  std::size_t num_facets() const { return facets.size(); }
  std::size_t face(std::size_t index) const {return facets[index]; }
};

/* A piecewise linear complex with holes */
class geometry {
  std::vector<surface> components;
  std::vector<point> holes;
public:
  void add_component(const surface& s) { components.push_back(s); }
  void add_hole(point point_in_hole) { holes.push_back(point_in_hole); }
  std::size_t num_components() const { return components.size(); }
  surface component(std::size_t index) const {return components[index]; }
  std::size_t num_holes() const { return holes.size(); }
  point hole(std::size_t index) { return holes[index]; }
};

class tetrahedron {
  std::size_t[4] vertices;
public:
  tetrahedron(std::size_t a, std::size_t b, std::size_t c, std::size_t d)
    : vertices[0](a), vertices[1](b), vertices[2](c), vertices[3] (d) {}
  std::size_t vertex(std::size_t index) { return vertices[index]; }
  void vertex(std::size_t index, std::size_t value) {
    vertices[index] = value;
  }
};

/* A mesh, generated by tetgen */
class mesh {
  std::vector<point> vertices;
  std::vector<tetragon> tetrahedra;
};

/* Read a surface from an off file */
void read_off(const std::istream& input);

/* Generate input for mesh generation  */
tetgenio generate_tetrahedra(const geometry& geometry);

}

#endif