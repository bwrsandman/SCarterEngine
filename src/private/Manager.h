//
// Created by sandy on 11/05/18.
//

#ifndef SCARTERENGINE_MANAGER_H
#define SCARTERENGINE_MANAGER_H

namespace sce::private_ {
class Manager {
 protected:
  bool isInitialized;

 public:
  Manager() = default;
  virtual ~Manager() = default;

  virtual void Initialize() = 0;
  virtual void Terminate() = 0;
};
}  // namespace sce::private_

#endif  // SCARTERENGINE_MANAGER_H
