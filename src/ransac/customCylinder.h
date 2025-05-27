#ifndef CUSTOM_CYLINDER_H
#define CUSTOM_CYLINDER_H

#include <CGAL/Shape_detection/Efficient_RANSAC/Cylinder.h>

namespace ransac {
template <class Traits>
class CustomCylinder : public CGAL::Shape_detection::Cylinder<Traits> {
 public:
  using Base = CGAL::Shape_detection::Cylinder<Traits>;
  using FT = typename Traits::FT;

  // Static member variables for global radius range
  static FT s_minRadius;
  static FT s_maxRadius;

  CustomCylinder()
      : Base(), m_minRadius(s_minRadius), m_maxRadius(s_maxRadius) {}

  // Static method to set global radius range
  static void set_radiusRange(FT minRadius, FT maxRadius) {
    s_minRadius = minRadius;
    s_maxRadius = maxRadius;
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

// Static member variable definitions
template <class Traits>
typename CustomCylinder<Traits>::FT CustomCylinder<Traits>::s_minRadius = 0.0;

template <class Traits>
typename CustomCylinder<Traits>::FT CustomCylinder<Traits>::s_maxRadius = 1.0;

}  // namespace ransac

#endif  // CUSTOM_CYLINDER_H
