#include "pickable.h"

namespace cxnd
{
	Pickable::Pickable()
		: m_state(ControlState::none)
		, m_enableSelect(true)
		, m_faceBase(0)
		, m_noPrimitive(false)
	{
	}

	Pickable::~Pickable()
	{
	}

	bool Pickable::hoverPrimitive(int primitiveID)
	{
		return false;
	}

	void Pickable::selectPrimitive(int chunk)
	{

	}

	void Pickable::unHoverPrimitive()
	{
		hoverPrimitive(-1);
	}

	ControlState Pickable::state()
	{
		return m_state;
	}

	void Pickable::setState(ControlState state)
	{
		m_state = state;
		onStateChanged(m_state);
	}

	bool Pickable::selected()
	{
		return m_state == ControlState::selected;
	}

	void Pickable::onStateChanged(ControlState state)
	{

	}

	bool Pickable::enableSelect()
	{
		return m_enableSelect;
	}

	void Pickable::setEnableSelect(bool enable)
	{
		m_enableSelect = enable;
	}

	void Pickable::faceBaseChanged(int faceBase)
	{

	}

	void Pickable::noPrimitiveChanged(bool noPrimitive)
	{

	}

	int Pickable::primitiveNum()
	{
		return 0;
	}

	int Pickable::chunk(int primitiveID)
	{
		return -1;
	}

	bool Pickable::isGroup()
	{
		return false;
	}

	void Pickable::setFaceBase(int faceBase)
	{
		m_faceBase = faceBase;
		faceBaseChanged(m_faceBase);
	}

	int Pickable::faceBase()
	{
		return m_faceBase;
	}

	bool Pickable::noPrimitive()
	{
		return m_noPrimitive;
	}

	void Pickable::setNoPrimitive(bool noPrimitive)
	{
		m_noPrimitive = noPrimitive;
		noPrimitiveChanged(m_noPrimitive);
	}

	Pickable* checkPickableFromList(PickSource* source, const trimesh::ivec2& point,
		std::list<Pickable*>& pickables, int* primitiveID)
	{
		Pickable* pickable = nullptr;
		if (source)
		{
			int faceID = -1;
			int _primitiveID = -1;
			bool picked = source->pick(point, &faceID);
			if (picked)
			{
				for (Pickable* p : pickables)
				{
					int faceBase = p->faceBase();
					int faceEnd = faceBase + (p->noPrimitive() ? 1 : p->primitiveNum());

					if (faceID >= faceBase && faceID < faceEnd)
					{
						pickable = p;
						break;
					}
				}

				if (pickable)
				{
					int faceBase = pickable->faceBase();
					_primitiveID = faceID - faceBase;
				}
				else
					_primitiveID = faceID;
			}

			if (primitiveID)
				*primitiveID = _primitiveID;
		}

		return pickable;
	}
}