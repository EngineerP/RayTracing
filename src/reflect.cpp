#include <Eigen/Core>
#include "reflect.h"

Eigen::Vector3d reflect(const Eigen::Vector3d & in, const Eigen::Vector3d & n)
{
  ////////////////////////////////////////////////////////////////////////////
  // Replace with your code here:
  Eigen::Vector3d reflected = (in - 2 * (in.dot(n)) * n).normalized();
  return reflected;
  ////////////////////////////////////////////////////////////////////////////
}
