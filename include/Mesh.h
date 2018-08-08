//
// Created by sandy on 05/08/18.
//

#ifndef SCARTERENGINE_MESH_H
#define SCARTERENGINE_MESH_H

#include <string>
#include <vec2.hpp>
#include <vector>

namespace sce::mesh {
typedef glm::vec2 Vertex;
typedef uint32_t Index;
class Mesh {
 public:
  virtual std::string Name() const = 0;
  // TODO subdivide to mesh primitives (submeshes)
  virtual const std::vector<Index> GetIndices() const = 0;
  virtual const std::vector<Vertex> GetVertices() const = 0;
};
}  // namespace sce::mesh

#endif  // SCARTERENGINE_MESH_H
