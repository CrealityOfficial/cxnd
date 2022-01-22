#include "selector.h"
#include "pickable.h"

namespace cxnd
{
	Selector::Selector()
		: m_pickSource(nullptr)
		, m_hoverPickable(nullptr)
		, m_currentFaceBase(0)
		, selectNotifying(false)
		, m_disableReverseSelect(false)
		, m_selectGroup(false)
	{
	}

	Selector::~Selector()
	{
	}

	void Selector::setPickerSource(PickSource* source)
	{
		m_pickSource = source;
	}

	void Selector::disableReverseSelect(bool disable)
	{
		m_disableReverseSelect = disable;
	}

	void Selector::addPickable(Pickable* pickable)
	{
		if (pickable)
		{
			_add(pickable);

			if (m_disableReverseSelect && (m_pickables.size() == 1))
			{
				select(pickable);
			}
		}
	}

	void Selector::removePickable(Pickable* pickable)
	{
		if (pickable)
		{
			_remove(pickable);

			pickable->setState(ControlState::none);
			if (m_hoverPickable && (m_hoverPickable == pickable))
			{
				clearHover();
			}

			if(pickable->enableSelect())
				notifyTracers();
		}
	}

	void Selector::addTracer(SelectorTracer* tracer)
	{
		if (tracer)
			m_selectorTracers.push_back(tracer);
	}

	void Selector::removeTracer(SelectorTracer* tracer)
	{
		if (tracer)
		{
			m_selectorTracers.erase(std::find(m_selectorTracers.begin(),
				m_selectorTracers.end(), tracer));
		}
	}

	std::list<Pickable*> Selector::selectionmPickables()
	{
		std::list<Pickable*> pickables;
		for (Pickable* pickable : m_pickables)
		{
			if (pickable->selected())
				pickables.push_back(pickable);
		}
		return pickables;
	}

	Pickable* Selector::check(float x, float y, int* primitiveID)
	{
		Pickable* pickable = checkPickableFromList(m_pickSource, x, y, m_pickables, primitiveID);

		return pickable;
	}

	void Selector::_add(Pickable* pickable)
	{
		m_pickables.push_back(pickable);

		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		pickable->setFaceBase(m_currentFaceBase);
		m_currentFaceBase += primitiveNum;
	}

	void Selector::_remove(Pickable* pickable)
	{
		//rebase 
		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		int offset = primitiveNum;

		std::list<Pickable*>::iterator it = std::find(m_pickables.begin(),
			m_pickables.end(), pickable);

		if (it != m_pickables.end())
		{
			for (std::list<Pickable*>::iterator eit = it; eit != m_pickables.end(); ++eit)
			{
				Pickable* p = *eit;
				int fBase = p->faceBase();
				fBase -= offset;
				p->setFaceBase(fBase);
			}

			m_pickables.erase(it);
			m_currentFaceBase -= offset;
		}
	}

	void Selector::updateFaceBases()
	{
		m_currentFaceBase = 0;

		for (Pickable* pickable : m_pickables)
		{
			bool noPrimitive = pickable->noPrimitive();
			int primitiveNum = pickable->primitiveNum();
			if (noPrimitive) primitiveNum = 1;

			pickable->setFaceBase(m_currentFaceBase);
			m_currentFaceBase += primitiveNum;
		}
	}

	void Selector::clearHover()
	{
		if (m_hoverPickable)
		{
			m_hoverPickable->setState(ControlState::none);
			m_hoverPickable = nullptr;
		}
	}

	bool Selector::hover(float x, float y)
	{
		int primitiveID = -1;
		Pickable* hoverEntity = check(x, y, &primitiveID);

		if (m_hoverPickable == hoverEntity)
		{
			if (hoverEntity && hoverEntity->isGroup())
			{
				bool changed = hoverEntity->hoverPrimitive(primitiveID);
				return changed;
			}

			return false;
		}

		if (m_hoverPickable)
		{
			if (m_hoverPickable->isGroup())
			{
				m_hoverPickable->unHoverPrimitive();
				m_hoverPickable = nullptr;
			}
			else if (m_hoverPickable->state() != ControlState::selected)
			{
				m_hoverPickable->setState(ControlState::none);
				m_hoverPickable = nullptr;
			}
		}

		if (hoverEntity && hoverEntity->selected())
			return false;

		m_hoverPickable = hoverEntity;

		if (m_hoverPickable && !m_hoverPickable->isGroup())
		{
			m_hoverPickable->setState(ControlState::hover);
		}
		return true;
	}

	void Selector::select(float x, float y)
	{
		Pickable* pickable = check(x, y, nullptr);

		if (pickable && !pickable->enableSelect())
			return;

		if (m_selectGroup) selectGroup(pickable);
		else select(pickable);
	}

	void Selector::selectGroup(Pickable* pickable)
	{
		std::list<Pickable*> offList;
		std::list<Pickable*> onList = selectionmPickables();
		if (pickable) onList.push_back(pickable);
		select(onList, offList);
	}

	void Selector::select(Pickable* pickable)
	{
		std::list<Pickable*> offList = selectionmPickables();
		std::list<Pickable*> onList;
		if (m_disableReverseSelect && !pickable)
			return;

		if (pickable) onList.push_back(pickable);
		select(onList, offList);
	}

	void Selector::unselect(Pickable* pickable)
	{
		if (!pickable)
			return;

		pickable->setState(ControlState::none);
		notifyTracers();
	}

	void Selector::appendSelect(Pickable* pickable)
	{
		if (pickable && !pickable->selected())
		{
			pickable->setState(ControlState::selected);
			notifyTracers();
		}
	}

	void Selector::selectAll()
	{
		std::list<Pickable*> onLists;
		std::list<Pickable*> offlist;
		for (Pickable* pickable : m_pickables)
			onLists.push_back(pickable);
		
		select(onLists, offlist);
	}

	void Selector::setSelectGroup(bool bGroup)
	{
		m_selectGroup = bGroup;
	}

	void Selector::select(std::list<Pickable*>& onList, std::list<Pickable*>& offList)
	{
		if ((onList.size() == 0) && (offList.size() == 0))
			return;

		for (Pickable* pickable : offList)
			pickable->setState(ControlState::none);

		for (Pickable* pickable : onList)
			pickable->setState(ControlState::selected);

		notifyTracers();
	}

	void Selector::notifyTracers()
	{
		if (selectNotifying)
			return;

		selectNotifying = true;

		for (SelectorTracer* tracer : m_selectorTracers)
		{
			tracer->onSelectionsChanged();
		}

		selectNotifying = false;
	}
}