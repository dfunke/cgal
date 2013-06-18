#ifndef CGAL_POINT_INSIDE_POLYHEDRON_AABB_TRIANGLE_ACCESSOR_3_PRIMITIVE_H
#define CGAL_POINT_INSIDE_POLYHEDRON_AABB_TRIANGLE_ACCESSOR_3_PRIMITIVE_H

namespace CGAL {
namespace internal {

template<typename GeomTraits, typename TriangleAccessor_3>
class AABB_triangle_accessor_3_primitive
{
public:
  typedef typename GeomTraits::Point_3                 Point;
  typedef typename TriangleAccessor_3::Triangle_handle Id;
  typedef typename GeomTraits::Triangle_3              Datum;

  AABB_triangle_accessor_3_primitive(const Id& handle)
  : m_triangle_handle(handle) { }

  Datum datum() const { return TriangleAccessor_3().triangle(m_triangle_handle); }
  Point reference_point() const { return datum().vertex(0); }

  const Id& id() const { return m_triangle_handle; }
  Id& id() { return m_triangle_handle; }

private:
  Id m_triangle_handle;
};  

}// namespace internal
}// namespace CGAL


#endif // CGAL_POINT_INSIDE_POLYHEDRON_AABB_TRIANGLE_ACCESSOR_3_PRIMITIVE_H
