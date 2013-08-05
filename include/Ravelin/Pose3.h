/****************************************************************************
 * Copyright 2013 Evan Drumwright
 * This library is distributed under the terms of the GNU Lesser General Public 
 * License (found in COPYING).
 ****************************************************************************/

#ifndef POSE3
#error This class is not to be included by the user directly. Use Pose3d.h or Pose3f.h instead. 
#endif

class AANGLE;
class MATRIX3;

/// A rigid body pose 
class POSE3 : public boost::enable_shared_from_this<POSE3>
{
  public:
    POSE3(boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const POSE3& source) { operator=(source); }
    POSE3(const AANGLE& a, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const MATRIX3& m, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const QUAT& q, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const AANGLE& a, const ORIGIN3& v, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const MATRIX3& m, const ORIGIN3& v, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const QUAT& q, const ORIGIN3& v, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    POSE3(const ORIGIN3& v, boost::shared_ptr<const POSE3> relative_pose = boost::shared_ptr<const POSE3>());
    static POSE3 identity() { POSE3 T; T.set_identity(); return T; }
    static VECTOR3 interpolate_vector(const POSE3& m1, const POSE3& m2, REAL t, const ORIGIN3& o);
    static POINT3 interpolate_point(const POSE3& m1, const POSE3& m2, REAL t, const ORIGIN3& o);
    static bool rel_equal(const POSE3& p1, const POSE3& p2, REAL tol = EPS);
    POINT3 transform(const POINT3& p) const { return transform(rpose, p); }
    VECTOR3 transform(const VECTOR3& v) const { return transform(rpose, v); }
    POINT3 inverse_transform(const POINT3& p) const;
    VECTOR3 inverse_transform(const VECTOR3& v) const;
    static POINT3 transform(boost::shared_ptr<const POSE3> target, const POINT3& v);
    static VECTOR3 transform(boost::shared_ptr<const POSE3> target, const VECTOR3& v);
    SFORCE transform(const SFORCE& w) const;
    SFORCE inverse_transform(const SFORCE& w) const;
    static SFORCE transform(boost::shared_ptr<const POSE3> target, const SFORCE& w);
    static std::vector<SFORCE>& transform(boost::shared_ptr<const POSE3> target, const std::vector<SFORCE>& w, std::vector<SFORCE>& result);
    SMOMENTUM transform(const SMOMENTUM& t) const;
    SMOMENTUM inverse_transform(const SMOMENTUM& t) const;
    SAXIS transform(const SAXIS& t) const;
    SAXIS inverse_transform(const SAXIS& t) const;
    SVELOCITY transform(const SVELOCITY& t) const;
    SVELOCITY inverse_transform(const SVELOCITY& t) const;
    static SAXIS transform(boost::shared_ptr<const POSE3> target, const SAXIS& t);
    static SMOMENTUM transform(boost::shared_ptr<const POSE3> target, const SMOMENTUM& t);
    static SVELOCITY transform(boost::shared_ptr<const POSE3> target, const SVELOCITY& t);
    static std::vector<SVELOCITY>& transform(boost::shared_ptr<const POSE3> target, const std::vector<SVELOCITY>& t, std::vector<SVELOCITY>& result);
    static std::vector<SMOMENTUM>& transform(boost::shared_ptr<const POSE3> target, const std::vector<SMOMENTUM>& t, std::vector<SMOMENTUM>& result);
    static std::vector<SAXIS>& transform(boost::shared_ptr<const POSE3> target, const std::vector<SAXIS>& t, std::vector<SAXIS>& result);
    SACCEL transform(const SACCEL& t) const;
    SACCEL inverse_transform(const SACCEL& t) const;
    static SACCEL transform(boost::shared_ptr<const POSE3> target, const SACCEL& t);
    static std::vector<SACCEL>& transform(boost::shared_ptr<const POSE3> target, const std::vector<SACCEL>& t, std::vector<SACCEL>& result);
    SPATIAL_RB_INERTIA transform(const SPATIAL_RB_INERTIA& j) const { return transform(rpose, j); }
    SPATIAL_RB_INERTIA inverse_transform(const SPATIAL_RB_INERTIA& j) const;
    static SPATIAL_RB_INERTIA transform(boost::shared_ptr<const POSE3> target, const SPATIAL_RB_INERTIA& j);
    SPATIAL_AB_INERTIA transform(const SPATIAL_AB_INERTIA& j) const { return transform(rpose, j); }
    SPATIAL_AB_INERTIA inverse_transform(const SPATIAL_AB_INERTIA& j) const;
    static SPATIAL_AB_INERTIA transform(boost::shared_ptr<const POSE3> target, const SPATIAL_AB_INERTIA& j);
    static TRANSFORM3 calc_relative_pose(boost::shared_ptr<const POSE3> source, boost::shared_ptr<const POSE3> target) { return calc_transform(source, target); }
    VECTOR3 qG_mult(REAL qdw, REAL qdx, REAL qdy, REAL qdz) const;
    VECTOR3 qL_mult(REAL qdw, REAL qdx, REAL qdy, REAL qdz) const;
    QUAT qG_transpose_mult(const VECTOR3& omega) const;
    QUAT qL_transpose_mult(const VECTOR3& omega) const;
    POSE3& set_identity();
    POSE3& invert();
    POSE3 inverse() const { return invert(*this); }
    static POSE3 invert(const POSE3& m);
    POSE3& set(const AANGLE& a);
    POSE3& set(const MATRIX3& m);
    POSE3& set(const QUAT& q);
    POSE3& set(const AANGLE& a, const ORIGIN3& v);
    POSE3& set(const MATRIX3& m, const ORIGIN3& v);
    POSE3& set(const QUAT& q, const ORIGIN3& v);
    POSE3& operator=(const POSE3& source);
    POSE3 operator*(const POSE3& m) const;
    void update_relative_pose(boost::shared_ptr<const POSE3> pose);
    static VECTOR3 interpolate_transform_vector(const POSE3& P1, const POSE3& P2, REAL t, const ORIGIN3& o);
    static POINT3 interpolate_transform_point(const POSE3& P1, const POSE3& P2, REAL t, const ORIGIN3& o);
    static SVELOCITY diff(const POSE3& P1, const POSE3& P2);

    /// the orientation of the pose frame
    QUAT q;

    /// the origin of the pose frame
    ORIGIN3 x;

    /// the pose that *this* pose is relative to (if any)
    boost::shared_ptr<const POSE3> rpose; 

  private:
    static void transform_spatial(boost::shared_ptr<const POSE3> target, const SVECTOR6& v, SVECTOR6& result);
    static void transform_spatial(boost::shared_ptr<const POSE3> target, const SVECTOR6& v, const VECTOR3& rv, const MATRIX3& E, SVECTOR6& result);
    void get_r_E(ORIGIN3& r, MATRIX3& E, bool inverse) const;
    static void get_r_E(const TRANSFORM3& T, ORIGIN3& r, MATRIX3& E);
    TRANSFORM3 calc_transform(boost::shared_ptr<const POSE3> p) const { return calc_transform(shared_from_this(), p); }
    static TRANSFORM3 calc_transform(boost::shared_ptr<const POSE3> source, boost::shared_ptr<const POSE3> target);
    static bool is_common(boost::shared_ptr<const POSE3> source, boost::shared_ptr<const POSE3> p, unsigned& i);
}; // end class

std::ostream& operator<<(std::ostream& out, const POSE3& m);

