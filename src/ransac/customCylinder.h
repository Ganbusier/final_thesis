#ifndef CUSTOM_CYLINDER_H
#define CUSTOM_CYLINDER_H

#include <CGAL/Shape_detection/Efficient_RANSAC/Cylinder.h>

namespace ransac {
    class CustomCylinder : public Cylinder {
        private:
            FT m_radius;
            Point_3 m_point_on_axis;
            Line_3 m_axis;
            bool m_wrap_u;
            FT m_minRadius = 0.001;
            FT m_maxRadius = 1.0;

        public:
            void setRadiusRange(FT minRadius, FT maxRadius) {
                m_minRadius = minRadius;
                m_maxRadius = maxRadius;
            }

        protected:
            void create_shape(const std::vector<std::size_t> &indices) override {
                Point_3 p1 = this->point(indices[0]);
                Point_3 p2 = this->point(indices[1]);

                Vector_3 n1 = this->normal(indices[0]);
                Vector_3 n2 = this->normal(indices[1]);

                Vector_3 axis = this->cross_pdct(n1, n2);
                FT axisL = CGAL::sqrt(this->sqlen(axis));
                if (axisL < (FT)0.0001) {
                  return;
                }
                axis = this->scale(axis, FT(1.0) / axisL);

                // establish two directions in the plane axis * x = 0,
                // whereas xDir is the projected n1
                Vector_3 xDir = this->sum_vectors(
                  n1, this->scale(axis, -this->scalar_pdct(n1, axis)));
                xDir = this->scale(xDir, (FT)1.0 / CGAL::sqrt(this->sqlen(xDir)));
                Vector_3 yDir = this->cross_pdct(axis, xDir);
                yDir = this->scale(yDir, (FT)1.0 / CGAL::sqrt(this->sqlen(yDir)));

                FT n2x = this->scalar_pdct(n2, yDir);
                FT n2y = -this->scalar_pdct(n2, xDir);

                Vector_3 dist = this->constr_vec(p1, p2);

                FT Ox = this->scalar_pdct(xDir, dist);
                FT Oy = this->scalar_pdct(yDir, dist);

                FT lineDist = n2x * Ox + n2y * Oy;

                m_radius = lineDist / n2x;
                m_point_on_axis = this->transl(p1, this->scale(xDir, m_radius));
                m_radius = CGAL::abs(m_radius);

                m_axis = this->m_traits.construct_line_3_object()(m_point_on_axis, axis);

                if (squared_distance(p1) > this->m_epsilon ||
                    (cos_to_normal(p1, n1) < this->m_normal_threshold) ||
                    m_radius < m_minRadius || m_radius > m_maxRadius)
                  return;

                this->m_is_valid = true;
                this->m_wrap_u = true;
            }
    };
}



#endif // CUSTOM_CYLINDER_H

