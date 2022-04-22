#include "a3d.h"
#include<limits.h>
namespace cxnd
{
	bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
        trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, trimesh::vec3& baryPosition)
	{
        auto E1 = v1 - v0;
        auto E2 = v2 - v0;

        auto P = trimesh::cross(dir, E2);
        auto det = trimesh::dot(E1, P);
        auto s = orig - v0;

        if(det < 0)
        {
            det = -det;
            s = v0 - orig;
        }
        if (det < 0.0001f)
            return false;

        auto f = 1.0f / det;
        auto dSP = trimesh::dot(s, P);
        baryPosition.at(0) = f * dSP;
        if (baryPosition.at(0) < 0.0f)
            return false;
        if (baryPosition.at(0) > 1.0f)
            return false;
        auto q = trimesh::cross(s, E1);
        auto dDQ = trimesh::dot(dir, q);
        baryPosition.y = f * dDQ;
        if (baryPosition.at(1) < 0.0f)
            return false;
        if (baryPosition.at(1) + baryPosition.at(0) > 1.0f)
            return false;
        auto dEQ = trimesh::dot(E2, q);
        baryPosition.at(2) = f * dEQ;
        return baryPosition.at(2) >= 0.0f;
	}

	bool rayIntersectTriangle(const trimesh::vec3& orig, const trimesh::vec3& dir,
		trimesh::vec3& v0, trimesh::vec3& v1, trimesh::vec3& v2, float* t, float* u, float* v)
	{
		trimesh::vec3 E1 = v1 - v0;
		trimesh::vec3 E2 = v2 - v0;

		trimesh::vec3 P = trimesh::cross(dir, E2);
		float det = trimesh::dot(E1, P);

		// keep det > 0, modify T accordingly
		trimesh::vec3 T;
		if (det > 0)
		{
			T = orig - v0;
		}
		else
		{
			T = v0 - orig;
			det = -det;
		}

		// If determinant is near zero, ray lies in plane of triangle
		if (det < 0.0001f)
			return false;

		// Calculate u and make sure u <= 1
		*u = trimesh::dot(T, P);
		if (*u < 0.0f || *u > det)
			return false;

		trimesh::vec3 Q = trimesh::cross(T, E1);

		// Calculate v and make sure u + v <= 1
		*v = trimesh::dot(dir, Q);
		if (*v < 0.0f || *u + *v > det)
			return false;

		// Calculate t, scale parameters, ray intersects triangle
		*t = trimesh::dot(E2, Q);

		float fInvDet = 1.0f / det;
		*t *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return true;
	}

	bool rayIntersectPlane(const trimesh::vec3& orig, const trimesh::vec3& dir, trimesh::vec3& vertex, trimesh::vec3& normal, float& t)
	{
		float l = trimesh::dot(normal, dir);
		if (l == 0.0f) return false;

		t = trimesh::dot(normal, (vertex - orig)) / l;
		return true;
	}

	CXND_API bool rayIntersectBoundingBox(const trimesh::dvec3 orig, const trimesh::dvec3 dir, const trimesh::dbox3& b)
	{
		bool intersected = false;

		trimesh::dvec3 bbMin = b.min;
		trimesh::dvec3 bbMax = b.max;

		trimesh::dbox3 boxX;
		boxX += trimesh::dvec3(bbMin.x, orig.y + dir.y * ((bbMin.x - orig.x) / dir.x), orig.z + dir.z * ((bbMin.x - orig.x) / dir.x));
		boxX += trimesh::dvec3(bbMax.x, orig.y + dir.y * ((bbMax.x - orig.x) / dir.x), orig.z + dir.z * ((bbMax.x - orig.x) / dir.x));

		trimesh::dbox3 boxY;
		boxY += trimesh::dvec3(orig.x + dir.x * ((bbMin.y - orig.y) / dir.y), bbMin.y, orig.z + dir.z * ((bbMin.y - orig.y) / dir.y));
		boxY += trimesh::dvec3(orig.x + dir.x * ((bbMax.y - orig.y) / dir.y), bbMax.y, orig.z + dir.z * ((bbMax.y - orig.y) / dir.y));

		trimesh::dbox3 boxZ;
		boxZ += trimesh::dvec3(orig.x + dir.x * ((bbMin.z - orig.z) / dir.z), orig.y + dir.y * ((bbMin.z - orig.z) / dir.z), bbMin.z);
		boxZ += trimesh::dvec3(orig.x + dir.x * ((bbMax.z - orig.z) / dir.z), orig.y + dir.y * ((bbMax.z - orig.z) / dir.z), bbMax.z);

		if (boxX.intersects(boxY) && boxX.intersects(boxZ) && boxZ.intersects(boxY))
			intersected = true;

		return intersected;
	}

	bool PointinTriangle(trimesh::vec3 A, trimesh::vec3 B, trimesh::vec3 C, trimesh::vec3 P)
	{
		trimesh::vec3 v0 = C - A;
		trimesh::vec3 v1 = B - A;
		trimesh::vec3 v2 = P - A;

		float dot00 = v0.dot(v0);
		float dot01 = v0.dot(v1);
		float dot02 = v0.dot(v2);
		float dot11 = v1.dot(v1);
		float dot12 = v1.dot(v2);

		float inverDeno = 1.0 / (dot00 * dot11 - dot01 * dot01);

		float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
		if (u < 0 || u > 1) // if u out of range, return directly
		{
			return false;
		}

		float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
		if (v < 0 || v > 1) // if v out of range, return directly
		{
			return false;
		}

		return u + v <= 1;
	}

	trimesh::box3 extendBox(const trimesh::box3& b, float r)
	{
		if (r <= 0.0f)
			r = 0.1f;

		trimesh::box3 rb = b;
		trimesh::vec3 offset = rb.size() * r;
		rb += (b.max + offset);
		rb += (b.min - offset);
		return rb;
	}

	float angleOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2)
	{
		float radian = radianOfVector3D2(v1, v2);
		return radian * 180.0f / (float)M_PI;
	}

	float radianOfVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2)
	{
		double denominator = sqrt((double)trimesh::len2(v1) * (double)trimesh::len2(v2));
		double cosinus = 0.0;

		if (denominator < INT_MIN)
			cosinus = 1.0; // cos (1)  = 0 degrees

		cosinus = (double)(v1 DOT v2) / denominator;
		cosinus = cosinus > 1.0 ? 1.0 : (cosinus < -1.0 ? -1.0 : cosinus);
		double angle = acos(cosinus);
#ifdef WIN32
        if (!_finite(angle) || angle > M_PI)
            angle = 0.0;
#elif __APPLE__
        if (!isfinite(angle) || angle > M_PI)
            angle = 0.0;
#elif defined(__ANDROID__)
        if (!isfinite(angle) || angle > M_PI)
            angle = 0.0;
#else
        if (!finite(angle) || angle > M_PI)
            angle = 0.0;
#endif
		return (float)angle;
	}

	trimesh::quaternion quaternionFromVector3D2(const trimesh::vec3& v1, const trimesh::vec3& v2)
	{
		trimesh::vec3 nv1 = trimesh::normalized(v1);
		trimesh::vec3 nv2 = trimesh::normalized(v2);

		float angle = angleOfVector3D2(nv1, nv2);
		trimesh::vec3 axis = trimesh::vec3(1.0f, 0.0f, 0.0f);
		if (angle < 180.0f)
		{
			axis = nv2 TRICROSS nv1;
			trimesh::normalize(axis);
		}

		trimesh::quaternion q = trimesh::quaternion::fromAxisAndAngle(axis, angle);
		q.normalize();
		return q;
	}

	void partScale(const trimesh::vec3& center, const trimesh::box3& part,
		const trimesh::vec3& scale, trimesh::vec3& outScale, trimesh::vec3& outOffset)
	{
		trimesh::vec3 c = center;
		trimesh::vec3 pmin = part.min;
		trimesh::vec3 pmax = part.max;
		trimesh::vec3 pcenter = part.center();
		trimesh::vec3 psize = part.size();

		trimesh::vec3 newPMin = (pmin - c) * scale + c;
		trimesh::vec3 newPMax = (pmax - c) * scale + c;
		trimesh::vec3 newCenter = (newPMin + newPMax) / 2.0f;
		trimesh::vec3 newSize = newPMax - newPMin;

		outScale = newSize / psize;
		outOffset = newCenter - pcenter;
	}

	trimesh::box3 extendBoxXY(const trimesh::box3& box, const trimesh::vec3& center)
	{
		float xmax = std::max(std::fabsf(box.max.x - center.x), 
							std::fabsf(box.min.x - center.x));
		float ymax = std::max(std::fabsf(box.max.y - center.y),
							std::fabsf(box.min.y - center.y));

		trimesh::vec3 dmin = center - trimesh::vec3(xmax, ymax, 0.0f);
		dmin.z = box.min.z;
		trimesh::vec3 dmax = center + trimesh::vec3(xmax, ymax, 0.0f);
		dmax.z = box.max.z;

		trimesh::box3 b;
		b += dmin;
		b += dmax;
		return b;
	}

	trimesh::fxform applyBox(const trimesh::box3& tbox, const trimesh::box3& sbox, bool sameScale)
	{
		trimesh::fxform xf = trimesh::fxform::identity();

		if (tbox.valid && sbox.valid)
		{
			trimesh::vec3 tcenter = tbox.center();
			trimesh::vec3 scenter = sbox.center();
			trimesh::vec3 scale = tbox.size() / sbox.size();
			if (sameScale)
			{
				float s = std::min(std::min(scale.x, scale.y), scale.z);
				scale = trimesh::vec3(s, s, s);
			}
			xf = trimesh::fxform::trans(tcenter)
				* trimesh::fxform::scale(scale.x, scale.y, scale.z)
				* trimesh::fxform::trans(scenter);
		}
		return xf;
	}
}
