#ifndef CXND_SNODE_1639130199169_H
#define CXND_SNODE_1639130199169_H
#include "cxnd/interface.h"
#include "trimesh2/XForm.h"
#include "trimesh2/Vec.h"
#include "trimesh2/Box.h"
#include "trimesh2/quaternion.h"
#include <list>

namespace cxnd
{
	class CXND_API SNode
	{
	public:
		SNode();
		~SNode();

		void setParent(SNode* node);
		SNode* parent();

		trimesh::vec3 center();
		void setCenter(const trimesh::vec3& center, bool update = true);

		void setLocalScale(const trimesh::vec3& scale, bool update = true);
		void resetLocalScale(bool update = true);
		trimesh::vec3 localScale();

		trimesh::quaternion localQuaternion();
		void setLocalQuaternion(const trimesh::quaternion& q, bool update = true);
		void resetLocalQuaternion(bool update = true);

		void setLocalPosition(const trimesh::vec3& position, bool update = true);
		void resetLocalPosition(bool update = true);
		trimesh::vec3 localPosition();

		trimesh::fxform globalMatrix();
		trimesh::fxform localMatrix();

		void translate(const trimesh::vec3& t);
		void scale(const trimesh::vec3& s);

		////// matrix apply
		void applyTranslate(const trimesh::vec3& t);
		void applyScale(const trimesh::vec3& s);
		void applyRotate(const trimesh::vec3& axis, float angle, bool local);
		void applyRotate(const trimesh::vec3& v1, const trimesh::vec3& v2, bool local);
		virtual trimesh::box3 calculateBox();

		void placeNormalOnPlane(const trimesh::vec3& normal);
		
		void roateCC(int index, int count);  //test
		void applyVisualMatrix(const trimesh::fxform& xf);
		void updateVisual();
	protected:
		void applyXf(const trimesh::fxform& xf);
	protected:
		void updateMatrix();
		virtual void matrixChanged(const trimesh::fxform& matrix);

		void addChild(SNode* node);
		void removeChild(SNode* node);
	protected:
		trimesh::vec3 m_localCenter;

		trimesh::vec3 m_localPosition;
		trimesh::vec3 m_localScale;
		trimesh::quaternion m_localRotate;

		trimesh::fxform m_globalMatrix;
		trimesh::fxform m_localMatrix;
		bool m_localDirty;

		SNode* m_parent;
		std::list<SNode*> m_children;
		bool m_updating;

		trimesh::box3 m_box;
	};
}

#endif // CXND_SNODE_1639130199169_H