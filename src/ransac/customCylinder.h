#ifndef CUSTOM_CYLINDER_H
#define CUSTOM_CYLINDER_H

#include <CGAL/Shape_detection/Efficient_RANSAC/Cylinder.h>

namespace ransac {
template <class Traits>
class CustomCylinder : public CGAL::Shape_detection::Cylinder<Traits> {
 public:
  using Base = CGAL::Shape_detection::Cylinder<Traits>;
  using FT = typename Traits::FT;

  CustomCylinder()
      : Base(), m_minRadius(0), m_maxRadius(std::numeric_limits<FT>::max()) {}

  // set radius range
  void set_radius_range(FT minRadius, FT maxRadius) {
    m_minRadius = minRadius;
    m_maxRadius = maxRadius;
  }

  // get radius range
  FT min_radius() const { return m_minRadius; }
  FT max_radius() const { return m_maxRadius; }

 protected:
  // override create_shape function, add radius range check
  virtual void create_shape(const std::vector<std::size_t>& indices) override {
    // call base class create_shape function
    Base::create_shape(indices);

    // if base class create failed, return
    if (!this->m_is_valid) {
      return;
    }

    // check if radius is in range
    if (this->radius() < m_minRadius || this->radius() > m_maxRadius) {
      this->m_is_valid = false;
    }
  }

 private:
  FT m_minRadius;  // minimum radius
  FT m_maxRadius;  // maximum radius
};
}  // namespace ransac

#endif  // CUSTOM_CYLINDER_H
