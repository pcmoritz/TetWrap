/* Philipp Moritz <pcmoritz@gmail.com>, written in January 2014 */

#ifndef PCL_HPP
#define PCL_HPP

#include <istream>
#include <ostream>
#include <vector>
#ifndef TETLIBRARY
#define TETLIBRARY
#endif
#include "tetgen/tetgen.h"

namespace tetwrap {

/* A class for three dimensional points in cartesian coordinates */
class point {
  REAL x_, y_, z_;
  friend std::ostream& operator<<(std::ostream& out, const point& p);
  friend std::istream& operator>>(std::istream& in, point& p);
public:
  point(REAL x, REAL y, REAL z) : x_(x), y_(y), z_(z) {}
  REAL x() const { return x_; }
  REAL y() const { return y_; }
  REAL z() const { return z_; }
  
  void x(REAL x) { this->x_ = x; }
  void y(REAL y) { this->y_ = y; }
  void z(REAL z) { this->z_ = z; }
};

/* A facet that could be part of a surface */
class facet {
  friend std::ostream& operator<<(std::ostream& out, const facet& f);
  friend std::istream& operator>>(std::istream& in, facet& f);
  std::vector<std::size_t> vertices;
public:
  std::size_t vertex(std::size_t index) { return vertices[index]; }
  void add_vertex(std::size_t value) {vertices.push_back(value); }
};

facet make_tetragon(std::size_t a, std::size_t b, std::size_t c, std::size_t d);

/* A surface that is the boundary of compact and connected body */
class surface {
  std::vector<point> vertices;
  std::vector<facet> facets;
public:
  void add_vertex(point v) { vertices.push_back(v); }
  void add_facet(const facet& f) { facets.push_back(f); }
  std::size_t num_vertices() const { return vertices.size(); }
  point vertex(std::size_t index) const { return vertices[index]; }
  std::size_t num_facets() const { return facets.size(); }
  facet face(std::size_t index) const {return facets[index]; }
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
  point hole(std::size_t index) const { return holes[index]; }
};

/* Read a surface from an off file */
surface read_off(std::istream& input);

/* Generate input for mesh generation  */
tetgenio generate_input(const geometry& geometry);

}

#endif
