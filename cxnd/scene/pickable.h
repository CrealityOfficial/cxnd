#ifndef CXND_PICKABLE_1638527203039_H
#define CXND_PICKABLE_1638527203039_H
#include "cxnd/interface.h"
#include <trimesh2/Vec.h>
#include <list>

namespace cxnd
{
	enum class ControlState
	{
		none,
		hover,
		selected
	};

	class CXND_API Pickable
	{
		friend class Selector;
	public:
		Pickable();
		virtual ~Pickable();

		virtual bool isGroup();

		virtual bool hoverPrimitive(int primitiveID);
		virtual void selectPrimitive(int chunk);
		virtual void unHoverPrimitive();

		virtual int primitiveNum();   // 0
		virtual int chunk(int primitiveID);

		ControlState state();
		bool selected();

		void setFaceBase(int faceBase);
		int faceBase();

		bool enableSelect();
		void setEnableSelect(bool enable);

		bool noPrimitive();
		void setNoPrimitive(bool noPrimitive);  // only set once at construct
	protected:
		void setState(ControlState state);
	protected:
		virtual void onStateChanged(ControlState state);
		virtual void faceBaseChanged(int faceBase);
		virtual void noPrimitiveChanged(bool noPrimitive);
	protected:
		ControlState m_state;
		int m_faceBase;
		bool m_noPrimitive;
		bool m_enableSelect;
	};

	class PickSource
	{
	public:
		virtual ~PickSource() {}

		virtual bool pick(int x, int y, int* faceID) = 0;
		virtual bool pick(const trimesh::ivec2& point, int* faceID) = 0;
	};

	CXND_API Pickable* checkPickableFromList(PickSource* source, const trimesh::ivec2& point,
		std::list<Pickable*>& list, int* primitiveID);

}

#endif // CXND_PICKABLE_1638527203039_H