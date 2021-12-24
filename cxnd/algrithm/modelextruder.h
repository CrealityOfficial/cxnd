#ifndef CXND_MODELEXTRUDER_1630552672911_H
#define CXND_MODELEXTRUDER_1630552672911_H

#include "trimesh2/TriMesh.h"
#include "cxnd/interface.h"


namespace cxnd
{
	// ���������������Ϣ
	class CXND_API ExtrudedMeshTopoData
	{
	public:
		ExtrudedMeshTopoData(trimesh::TriMesh* mesh, trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0))
			: m_origin(origin)
			, m_mesh(mesh)
		{
		}
		~ExtrudedMeshTopoData() {}

		//void setRoofData();
		//void setBottomData();
		//void setWallData();
		void setWallOuterData(std::vector<int>& lowerVertexIndices, std::vector<int>& upperVertexIndices, std::vector<int>& faceIndices)
		{
			m_wallOuterLowerVertexIndices = lowerVertexIndices;
			m_wallOuterUpperVertexIndices = upperVertexIndices;
			m_wallOuterFaceIndices = faceIndices;
		}
		//void setWallInnerData();

		trimesh::TriMesh* getMesh() { return m_mesh; }
		trimesh::dvec3 getOrigin() { return m_origin; }

		std::vector<int> getWallOuterLowerVertexIndices() { return m_wallOuterLowerVertexIndices; }
		std::vector<int> getWallOuterUpperVertexIndices() { return m_wallOuterUpperVertexIndices; }
		std::vector<int> getWallOuterFaceIndices() { return m_wallOuterFaceIndices; }

	private:
		// ����ԭ��
		trimesh::dvec3 m_origin;
		// ģ������
		trimesh::TriMesh* m_mesh;
		
		// �ϵ������ĵ�
		// �ϵ�����Χ��������
		// �ϵ�������
		

		// �µ������ĵ�
		// �µ�����Χ��������
		// �µ�������


		// ��������
		
		// ��������¶������� *
		std::vector<int> m_wallOuterLowerVertexIndices;
		// ��������϶������� *
		std::vector<int> m_wallOuterUpperVertexIndices;
		// ����������� *
		std::vector<int> m_wallOuterFaceIndices;

		// �����ڱ��¶�������
		// �����ڱ��϶�������
		// �����ڱ�����                                     
	};

	class CXND_API ModelExtruder
	{
	public:
		ModelExtruder();
		virtual ~ModelExtruder();

		// ���ݲ�ͬ������������ģ�壬������Բ�ǣ�Բ�ǰ뾶����С�ڵ��ڱ߳���һ�룬������Զ�����Ϊ�߳���һ��
		bool build2DMould(int edgeCount, double edgeLength,
			bool roundAngleFlag = false, double roundAngleRadius = 0.1,
			trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0));
		bool build2DMould(int edgeCount, double length, double width,
			bool roundAngleFlag = false, double roundAngleRadius = 0.1,
			trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0));
		// ���ڵ���ģ����� z �᷽�������
		trimesh::TriMesh* extrude(double height, double wallThickness, double bottomThickness, bool roofFlag = false, ExtrudedMeshTopoData** topoData = nullptr);

	private:
		trimesh::dvec3 BezierSample(double t, trimesh::dvec3 preCtrl, trimesh::dvec3 midCtrl, trimesh::dvec3 postCtrl);
		// ��������������֮�乹��������
		void generateFaces(std::vector<int> indicesA, std::vector<int> indicesB, trimesh::TriMesh* resultMesh, bool ringFlag, bool ccwFlag = false, std::vector<int>* newFaceIndices = nullptr);
		// �ڶ���Ͷ�������֮�乹������������
		void generateFaces(int originIndex, std::vector<int> indices, trimesh::TriMesh* resultMesh, bool ccwFlag = false, std::vector<int>* newFaceIndices = nullptr);

	protected:
		bool m_roundAngleFlag;
		int m_roundAngleSampleCount;
		std::vector<trimesh::dvec3> m_2DMouldCtrlVertices;
		std::vector<std::pair<double, double>> m_roundAngleRadiusRatePerCtrlVertex;

		trimesh::dvec3 m_origin;
		std::vector<trimesh::dvec3> m_2DMouldVertices;
	};
}

#endif // CXND_MODELEXTRUDER_1630552672911_H