#include "tetwrap.hpp"
#include <exception>

namespace tetwrap {

std::ostream& point::operator<<(std::ostream& out, const point& p) {
  out << p.x() << " " << p.y() << " " << p.z() << std::endl;
  return out;
}

std::ostream& facet::operator<<(std::ostream& out, const facet& f) {
  out << f.vertices.size();
  for(int k = 0; k < f.vertices.size(); k++) {
    out << " " << f.vertices[k];
  }
  out << std::endl;
  return out;
}

surface read_off(const std::istream& input) {
  surface result;
  
  std::string header;
  input >> header;
  if(header != "OFF")
    throw std::runtime_error("Input is not an OFF file");

  int num_vertices = 0;
  input >> num_vertices;

  int num_facets = 0;
  input >> num_facets;

  int num_edges = 0;
  input >> num_edges;

  for(int k = 0; k < num_vertices; k++) {
    point p;
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

tetgenio generate_input(const geometry& geometry) {
  tetgenio in;
  // All indices start from 0
  in.firstnumber = 0;
  // Determine number of vertices
  std::vector<std::size_t> indices(g.num_components() + 1);
  for(int i = 0; i < g.num_components(); i++) {
    indices[i+1] = indices[i] + g.component(i).num_vertices();
    in.numberofpoints += g.component(i).num_vertices();
  }
  // Fill the vertices array
  in.pointlist = new REAL[in.numberofpoints * 3];
  for(int i = 0; i < g.num_components(); i++) {
    for(int j = 0; j < indices[i+1]; j++) {
      in.pointlist[3 * indices[i] + 3 * j] = g.component(i).vertex(j).x();
      in.pointlist[3 * indices[i] + 3 * j + 1] = g.component(i).vertex(j).y();
      in.pointlist[3 * indices[i] + 3 * j + 2] = g.component(i).vertex(j).z();
    }
  }
  // Determine number of facets
  indices[0] = 0;
  for(int i = 0; i < g.num_components(); i++) {
    indices[i+1] = indices[i] + g.component(i).num_facets();
    in.numberoffacets += g.component(i).num_facets();
  }
  // Fill the facets array
  in.facetlist = new tetgenio::facet[in.numberoffacets];
  for(int i = 0; i < g.num_components(); i++) {
    for(int j = 0; j < indices[i+1]; j++) {
      tetgenio::facet *f = &in.facetlist[indices[i] + j];
      setfacet(f, indices[i] + g.component(i).facet(j).vertex(0),
	          indices[i] + g.component(i).facet(j).vertex(1),
	          indices[i] + g.component(i).facet(j).vertex(2),
	          indices[i] + g.component(i).facet(j).vertex(3));
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