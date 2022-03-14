#ifndef CXND_MODELEXTRUDER_1630552672911_H
#define CXND_MODELEXTRUDER_1630552672911_H

#include "trimesh2/TriMesh.h"
#include "cxnd/interface.h"
#include <map>


namespace cxnd
{
	// 柱体网格的拓扑信息
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

		void cacheMeshVertices()
		{
			if (m_mesh)
			{
				m_meshVerticesCache.resize(m_mesh->vertices.size());
				for (int i = 0; i < m_mesh->vertices.size(); i++)
				{
					m_meshVerticesCache[i] = m_mesh->vertices[i];
				}
			}
		}
		std::vector<trimesh::vec3>& getMeshVerticesCache() { return m_meshVerticesCache; }

		std::map<int, trimesh::dvec2>& getTessCoordsMap() { return m_vertexIndices2TessCoord; }
		std::map<int, trimesh::vec3>& getTessNormalsMap() { return m_vertexIndices2TessNormal; }

	private:
		// 网格原点
		trimesh::dvec3 m_origin;
		// 模型网格
		trimesh::TriMesh* m_mesh;
		// 顶点数据备份
		std::vector<trimesh::vec3> m_meshVerticesCache;
		
		// 上底面中心点
		// 上底面外围顶点序列
		// 上底面网格
		

		// 下底面中心点
		// 下底面外围顶点序列
		// 下底面网格


		// 侧面网格
		
		// 侧面外壁下顶点序列 *
		std::vector<int> m_wallOuterLowerVertexIndices;
		// 侧面外壁上顶点序列 *
		std::vector<int> m_wallOuterUpperVertexIndices;
		// 侧面外壁网格 *
		std::vector<int> m_wallOuterFaceIndices;

		// 侧面内壁下顶点序列
		// 侧面内壁上顶点序列
		// 侧面内壁网格                         

		// 细分结果
		// 顶点索引到细分坐标的映射
		std::map<int, trimesh::dvec2> m_vertexIndices2TessCoord;
		// 顶点索引到法向量插值的映射
		std::map<int, trimesh::vec3> m_vertexIndices2TessNormal;
	};

	class CXND_API ModelExtruder
	{
	public:
		ModelExtruder();
		virtual ~ModelExtruder();

		// 根据不同参数构建底面模板，若启用圆角，圆角半径必须小于等于边长的一半，否则会自动修正为边长的一半
		bool build2DMould(int edgeCount, double edgeLength,
			bool roundAngleFlag = false, double roundAngleRadius = 0.1,
			trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0));
		bool build2DMould(int edgeCount, double length, double width,
			bool roundAngleFlag = false, double roundAngleRadius = 0.1,
			trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0));
		bool buildRectMould(double width, double length,
			bool roundAngleFlag = false, double roundAngleRadius = 0.1,
			trimesh::dvec3 origin = trimesh::dvec3(0.0, 0.0, 0.0));
		// 基于底面模板进行 z 轴方向的拉伸
		trimesh::TriMesh* extrude(double height, double wallThickness, double bottomThickness, bool roofFlag = false, ExtrudedMeshTopoData** topoData = nullptr);

	private:
		trimesh::dvec3 BezierSample(double t, trimesh::dvec3 preCtrl, trimesh::dvec3 midCtrl, trimesh::dvec3 postCtrl);
		// 在两个顶点序列之间构建三角面
		//void generateFaces(std::vector<int> indicesA, std::vector<int> indicesB, trimesh::TriMesh* resultMesh, bool ringFlag, bool ccwFlag = false, std::vector<int>* newFaceIndices = nullptr);
		void generateFaces(std::vector<int> indicesA, std::vector<int> indicesB, trimesh::TriMesh* resultMesh,
			bool ringFlag, bool ccwFlag = false, std::vector<int>* bufferFaceIndices = nullptr, std::vector<int>* newFaceIndices = nullptr);
		// 在顶点和顶点序列之间构建扇形三角面
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