#include "tetwrap.hpp"
#include <stdexcept>
#include <iostream>

namespace tetwrap {

std::ostream& operator<<(std::ostream& out, const point& p) {
  out << p.x() << " " << p.y() << " " << p.z() << std::endl;
  return out;
}

std::istream& operator>>(std::istream& in, point& p) {
  REAL x, y, z;
  in >> x >> y >> z;
  p.x(x); p.y(y); p.z(z);
  return in;
}

std::ostream& operator<<(std::ostream& out, const facet& f) {
  out << f.vertices.size();
  for(int k = 0; k < f.vertices.size(); k++) {
    out << " " << f.vertices[k];
  }
  out << std::endl;
  return out;
}

std::istream& operator>>(std::istream& in, facet& f) {
  std::size_t size = 0;
  in >> size;
  for(int k = 0; k < size; k++) {
    std::size_t vertex;
    in >> vertex;
    f.add_vertex(vertex);
  }
  return in;
}

facet make_tetragon(std::size_t a, std::size_t b, std::size_t c, std::size_t d) {
	facet tetragon;
	tetragon.add_vertex(a);
	tetragon.add_vertex(b);
	tetragon.add_vertex(c);
	tetragon.add_vertex(d);
	return tetragon;
}

surface read_off(std::istream& input) {
  surface result;
  
  std::string header;
  input >> std::skipws >> header;
  if(header != "OFF")
    throw std::runtime_error("Input is not an OFF file");

  int num_vertices = 0;
  input >> num_vertices;

  int num_facets = 0;
  input >> num_facets;

  int num_edges = 0;
  input >> num_edges;

  for(int k = 0; k < num_vertices; k++) {
    point p(0.0, 0.0, 0.0);
    input >> p;
    result.add_vertex(p);
  }

  for(int k = 0; k < num_facets; k++) {
    facet f;
    input >> f;
    result.add_facet(f);
  }

  return result;
}

void setfacet(tetgenio::facet* f, int a, int b, int c, int d) {
  f->numberofpolygons = 1;
  f->polygonlist = new tetgenio::polygon[f->numberofpolygons];
  f->numberofholes = 0;
  f->holelist = NULL;
  tetgenio::polygon *p = &f->polygonlist[0];
  p->numberofvertices = 4;
  p->vertexlist = new int[p->numberofvertices];
  p->vertexlist[0] = a;
  p->vertexlist[1] = b;
  p->vertexlist[2] = c;
  p->vertexlist[3] = d;
}

tetgenio generate_input(const geometry& g) {
  tetgenio in;

  // All indices start from 0
  in.firstnumber = 0;
  // Determine number of vertices
  std::vector<std::size_t> vert_ind(g.num_components() + 1);
  for(int i = 0; i < g.num_components(); i++) {
    vert_ind.at(i+1) = vert_ind.at(i) + g.component(i).num_vertices();
    in.numberofpoints += g.component(i).num_vertices();
  }

  // Fill the vertices array
  in.pointlist = new REAL[in.numberofpoints * 3];
  for(int i = 0; i < g.num_components(); i++) {
    for(int j = 0; j < vert_ind.at(i+1) - vert_ind.at(i); j++) {
      in.pointlist[3 * vert_ind.at(i) + 3 * j] = g.component(i).vertex(j).x();
      in.pointlist[3 * vert_ind.at(i) + 3 * j + 1] = g.component(i).vertex(j).y();
      in.pointlist[3 * vert_ind.at(i) + 3 * j + 2] = g.component(i).vertex(j).z();
    }
  }

  // Determine number of facets
  std::vector<std::size_t> fac_ind(g.num_components() + 1);
  for(int i = 0; i < g.num_components(); i++) {
    fac_ind[i+1] = fac_ind[i] + g.component(i).num_facets();
    in.numberoffacets += g.component(i).num_facets();
  }
  // Fill the facets array
  in.facetlist = new tetgenio::facet[in.numberoffacets];
  for(int i = 0; i < g.num_components(); i++) {
    for(int j = 0; j < fac_ind.at(i+1) - fac_ind.at(i); j++) {
      tetgenio::facet *f = &in.facetlist[fac_ind[i] + j];
      setfacet(f, vert_ind[i] + g.component(i).face(j).vertex(0),
	          vert_ind[i] + g.component(i).face(j).vertex(1),
	          vert_ind[i] + g.component(i).face(j).vertex(2),
	          vert_ind[i] + g.component(i).face(j).vertex(3));
    }
  }

  // Set up the holes
  in.numberofholes = g.num_holes();
  in.holelist = new REAL[in.numberofholes * 3];
  for(int i = 0; i < g.num_holes(); i++) {
    in.holelist[3 * i] = g.hole(i).x();
    in.holelist[3 * i + 1] = g.hole(i).y();
    in.holelist[3 * i + 2] = g.hole(i).z();
  }

  return in;
}

}
