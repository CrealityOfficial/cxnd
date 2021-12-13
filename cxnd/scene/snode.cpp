#include "snode.h"

namespace cxnd
{
	SNode::SNode()
		: m_localDirty(true)
		, m_updating(false)
		, m_parent(nullptr)
		, m_localScale(1.0f, 1.0f, 1.0f)
	{

	}

	SNode::~SNode()
	{
		setParent(nullptr);
	}

	void SNode::setParent(SNode* node)
	{
		if (m_parent == node)
			return;

		if (m_parent)
			m_parent->removeChild(this);

		m_parent = node;

		if (m_parent)
			m_parent->addChild(this);
	}

	SNode* SNode::parent()
	{
		return m_parent;
	}

	void SNode::addChild(SNode* node)
	{
		if (!node)
			return;

		std::list<SNode*>::iterator it = std::find(m_children.begin(), m_children.end(), node);
		if (it != m_children.end())
			return;

		m_children.push_back(node);
		node->updateMatrix();
	}

	void SNode::removeChild(SNode* node)
	{
		if (!node)
			return;

		std::list<SNode*>::iterator it = std::find(m_children.begin(), m_children.end(), node);
		if (it == m_children.end())
			return;

		m_children.erase(it);
		node->updateMatrix();
	}

	trimesh::vec3 SNode::center()
	{
		return m_localCenter;
	}

	void SNode::setCenter(const trimesh::vec3& center, bool update)
	{
		m_localCenter = center;
		m_localDirty = true;

		if (update)
			updateMatrix();
	}

	void SNode::setLocalScale(const trimesh::vec3& scale, bool update)
	{
		m_localScale = scale;
		m_localDirty = true;

		if (update)
			updateMatrix();
	}

	void SNode::resetLocalScale(bool update)
	{
		setLocalScale(trimesh::vec3(1.0f, 1.0f, 1.0f), update);
	}

	trimesh::vec3 SNode::localScale()
	{
		return m_localScale;
	}

	trimesh::quaternion SNode::localQuaternion()
	{
		return m_localRotate;
	}

	void SNode::setLocalQuaternion(const trimesh::quaternion& q, bool update)
	{
		m_localRotate = q;
		m_localDirty = true;

		if (update)
			updateMatrix();
	}

	void SNode::resetLocalQuaternion(bool update)
	{
		setLocalQuaternion(trimesh::quaternion(), update);
	}

	void SNode::setLocalPosition(const trimesh::vec3& position, bool update)
	{
		m_localPosition = position;
		m_localDirty = true;

		if (update)
			updateMatrix();
	}

	void SNode::resetLocalPosition(bool update)
	{
		setLocalPosition(trimesh::vec3(0.0f, 0.0f, 0.0f), update);
	}

	trimesh::vec3 SNode::localPosition()
	{
		return m_localPosition;
	}

	void SNode::updateMatrix()
	{
		if (m_updating)
		{
			//cycle update
			return;
		}

		m_updating = true;
		if (m_localDirty)
		{
			m_localMatrix = trimesh::fxform::identity();
			trimesh::fxform t1 = trimesh::fxform::trans(m_localPosition + m_localCenter);
			trimesh::fxform r = trimesh::fromQuaterian(m_localRotate);
			trimesh::fxform s = trimesh::fxform::scale(m_localScale.x, m_localScale.y, m_localScale.z);
			trimesh::fxform t2 = trimesh::fxform::trans(-m_localCenter);

			m_localMatrix = t1 * r * s * t2;
			m_localDirty = false;
		}

		m_globalMatrix = m_localMatrix;
		if (m_parent)
		{
			trimesh::fxform m = m_parent->globalMatrix();
			m_globalMatrix = m * m_globalMatrix;
		}

		matrixChanged(m_globalMatrix);
		for (SNode* node : m_children)
			node->updateMatrix();

		m_updating = false;
	}

	void SNode::matrixChanged(const trimesh::fxform& matrix)
	{

	}

	trimesh::fxform SNode::globalMatrix()
	{
		return m_globalMatrix;
	}

	trimesh::fxform SNode::localMatrix()
	{
		return m_localMatrix;
	}

	void SNode::translate(const trimesh::vec3& t)
	{
		setLocalPosition(m_localPosition + t);
	}

	void SNode::scale(const trimesh::vec3& s)
	{
		setLocalScale(m_localScale * s);
	}

	void SNode::applyTranslate(const trimesh::vec3& t)
	{
		translate(t);
	}

	void SNode::applyScale(const trimesh::vec3& s)
	{
		scale(s);
	}

	void SNode::applyRotate(const trimesh::vec3& axis, float angle, bool local)
	{
		trimesh::vec3 _axis = axis;
		if (!local)
		{
			_axis = m_localRotate * axis;
			trimesh::normalize(_axis);
		}
		trimesh::quaternion q = trimesh::quaternion::fromAxisAndAngle(_axis, angle);
		setLocalQuaternion(q * m_localRotate);
	}

	void SNode::applyXf(const trimesh::fxform& xf)
	{
		m_localMatrix = xf * m_localMatrix;
		updateMatrix();
	}
}