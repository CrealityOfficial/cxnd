#ifndef CXND_SELECTOR_1638527203040_H
#define CXND_SELECTOR_1638527203040_H
#include "cxnd/interface.h"
#include "trimesh2/Vec.h"
#include <list>

namespace cxnd
{
	class PickSource;
	class Pickable;
	class SelectorTracer
	{
	public:
		virtual ~SelectorTracer() {}

		virtual void onSelectionsChanged() = 0;
	};

	class CXND_API Selector
	{
	public:
		Selector();
		virtual ~Selector();

		void setPickerSource(PickSource* source);
		void disableReverseSelect(bool disable);

		void addPickable(Pickable* pickable);
		void removePickable(Pickable* pickable);
		void addTracer(SelectorTracer* tracer);
		void removeTracer(SelectorTracer* tracer);
		std::list<Pickable*> selectionmPickables();

		Pickable* check(float x, float y, int* primitiveID = nullptr);

		void clearHover();
		bool hover(float x, float y);
		void select(float x, float y);
		void select(Pickable* pickable);
		void appendSelect(Pickable* pickable);
		void selectGroup(Pickable* pickable);
		void updateFaceBases();

		void select(std::list<Pickable*>& onList, std::list<Pickable*>& offList);
		void selectAll();
		void setSelectGroup(bool bGroup);
	private:
		void _add(Pickable* pickable);
		void _remove(Pickable* pickable);

		void notifyTracers();
	protected:
		PickSource* m_pickSource;
		std::list<Pickable*> m_pickables;

		Pickable* m_hoverPickable;

		std::list<SelectorTracer*> m_selectorTracers;

		int m_currentFaceBase;
		bool m_selectGroup;
		bool selectNotifying;

		bool m_disableReverseSelect;
	};
}

#endif // CXND_SELECTOR_1638527203040_H