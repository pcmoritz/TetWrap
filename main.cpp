/* This is an example to show how the library can be used */

#include <fstream>
#include <stdexcept>
#include "tetwrap.hpp"

using namespace tetwrap;

int main() {
  std::ifstream in("../A.off");
  if(!in)
    throw std::runtime_error("File A.off not found");
  surface s = read_off(in);
  geometry g;
  g.add_component(s);
  tetgenio mesh = generate_input(g);
  tetgenio out;
  char flags[] = "pq";
  tetrahedralize(flags, &mesh, &out);
  char out_name[] = "testout";
  out.save_nodes(out_name);
  out.save_elements(out_name);
  out.save_faces(out_name);
}
