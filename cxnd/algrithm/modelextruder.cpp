#include "modelextruder.h"

#include "trimesh2/XFORM.H"

using namespace cxnd;


ModelExtruder::ModelExtruder()
	: m_roundAngleFlag(false)
	, m_roundAngleSampleCount(5)
{
}

ModelExtruder::~ModelExtruder()
{
	m_2DMouldVertices.clear();
}

bool ModelExtruder::build2DMould(int edgeCount, double edgeLength, 
	bool roundAngleFlag, double roundAngleRadius,
	trimesh::dvec3 origin)
{
	if (edgeCount <= 2 || edgeLength <= 0 || (roundAngleFlag && roundAngleRadius <= 0.0))
		return false;

	m_roundAngleFlag = roundAngleFlag;
	m_origin = origin;
	m_2DMouldVertices.clear();
	m_roundAngleRadiusRatePerCtrlVertex.clear();

	// 底面模板生成
	double deltaRadians = 2 * M_PI / edgeCount;
	double radius = edgeLength / 2.0 / sin(deltaRadians / 2.0);
	trimesh::dvec3 offsetVector(0.0, 1.0, 0.0);
	trimesh::XForm<double> rotMt = trimesh::XForm<double>::rot(deltaRadians, trimesh::dvec3(0.0, 0.0, 1.0));
	for (int i = 0; i < edgeCount; i++)
	{
		trimesh::dvec3 vertex = m_origin + radius * offsetVector;
		m_2DMouldVertices.push_back(vertex);
		
		offsetVector = rotMt * offsetVector;
	}

	// 圆角处理：以上一步生成的模板顶点，及其向左右相邻顶点方向延伸圆角半径长度所得的点为控制点，进行贝塞尔插值，得到圆角顶点
	if (m_roundAngleFlag)
	{
		// 圆角半径最大只能为边长的一半
		roundAngleRadius = std::min(roundAngleRadius, edgeLength / 2.0);
		// 圆角半径与边长的比例，用于内壁的圆角处理
		double rarRate = roundAngleRadius / edgeLength;
		double t = 0.0;
		double deltaT = 1.0 / m_roundAngleSampleCount;
		m_2DMouldCtrlVertices = m_2DMouldVertices;
		m_2DMouldVertices.clear();
		for (int i = 0; i < m_2DMouldCtrlVertices.size(); i++)
		{
			trimesh::dvec3 curVertex = m_2DMouldCtrlVertices[i];
			trimesh::dvec3 preVertex = m_2DMouldCtrlVertices[i - 1 < 0 ? m_2DMouldCtrlVertices.size() - 1 : i - 1];
			trimesh::dvec3 postVertex = m_2DMouldCtrlVertices[i + 1 >= m_2DMouldCtrlVertices.size() ? 0 : i + 1];

			trimesh::dvec3 preControl = curVertex + roundAngleRadius * trimesh::normalized(preVertex - curVertex);
			trimesh::dvec3 postControl = curVertex + roundAngleRadius * trimesh::normalized(postVertex - curVertex);

			t = 0.0;
			for (; t <= 1.0; t+=deltaT)
			{
				trimesh::dvec3 vertex = BezierSample(t, preControl, curVertex, postControl);
				m_2DMouldVertices.push_back(vertex);
			}

			m_roundAngleRadiusRatePerCtrlVertex.push_back({rarRate, rarRate});
		}
	}

	return true;
}

bool ModelExtruder::build2DMould(int edgeCount, double length, double width,
	bool roundAngleFlag, double roundAngleRadius, 
	trimesh::dvec3 origin)
{
	if (edgeCount <= 2 || length <= 0 || width <= 0)
		return false;

	m_origin = origin;
	m_2DMouldVertices.clear();
	m_roundAngleRadiusRatePerCtrlVertex.clear();

	// 底面模板生成
	double deltaRadians = 2 * M_PI / edgeCount;
	double radius = std::min(length, width) / 2;
	double extendOffset = std::max(length, width) / 2 - radius;
	trimesh::dvec3 extendVector = length > width ? trimesh::dvec3(1.0, 0.0, 0.0) : trimesh::dvec3(0.0, 1.0, 0.0);
	trimesh::dvec3 offsetVector = trimesh::XForm<double>::rot(deltaRadians / 2, trimesh::dvec3(0.0, 0.0, 1.0)) * trimesh::dvec3(0.0, 1.0, 0.0);
	trimesh::XForm<double> rotMt = trimesh::XForm<double>::rot(deltaRadians, trimesh::dvec3(0.0, 0.0, 1.0));
	for (int i = 0; i < edgeCount; i++)
	{
		double dotValue = offsetVector.dot(extendVector);
		dotValue = dotValue > 0 ? 1 : dotValue < 0 ? -1 : 0;
		trimesh::dvec3 tempVector = dotValue *extendVector;
		trimesh::dvec3 vertex = m_origin + radius * offsetVector + extendOffset * tempVector;
		m_2DMouldVertices.push_back(vertex);

		offsetVector = rotMt * offsetVector;
	}

	// 圆角处理：以上一步生成的模板顶点，及其向左右相邻顶点方向延伸圆角半径长度所得的点为控制点，进行贝塞尔插值，得到圆角顶点
	if (m_roundAngleFlag)
	{
		double t = 0.0;
		double deltaT = 1.0 / m_roundAngleSampleCount;
		m_2DMouldCtrlVertices = m_2DMouldVertices;
		m_2DMouldVertices.clear();
		for (int i = 0; i < m_2DMouldCtrlVertices.size(); i++)
		{
			trimesh::dvec3 curVertex = m_2DMouldCtrlVertices[i];
			trimesh::dvec3 preVertex = m_2DMouldCtrlVertices[i - 1 < 0 ? m_2DMouldCtrlVertices.size() - 1 : i - 1];
			trimesh::dvec3 postVertex = m_2DMouldCtrlVertices[i + 1 >= m_2DMouldCtrlVertices.size() ? 0 : i + 1];

			double curpreDis = trimesh::distance(curVertex, preVertex);
			double curpostDis = trimesh::distance(curVertex, postVertex);
			// 圆角半径最大只能为边长的一半
			double curpreRaius = std::min(roundAngleRadius, curpreDis / 2.0);
			double curpostRaius = std::min(roundAngleRadius, curpostDis / 2.0);
			trimesh::dvec3 preControl = curVertex + curpreRaius * trimesh::normalized(preVertex - curVertex);
			trimesh::dvec3 postControl = curVertex + curpostRaius * trimesh::normalized(postVertex - curVertex);

			t = 0.0;
			for (; t <= 1.0; t += deltaT)
			{
				trimesh::dvec3 vertex = BezierSample(t, preControl, curVertex, postControl);
				m_2DMouldVertices.push_back(vertex);
			}

			// 圆角半径与边长的比例，用于内壁的圆角处理
			double curpreRARRate = curpreRaius / curpreDis;
			double curpostRARRate = curpreRaius / curpostDis;
			m_roundAngleRadiusRatePerCtrlVertex.push_back({ curpreRARRate, curpostRARRate });
		}
	}

	return true;
}

trimesh::TriMesh* ModelExtruder::extrude(double height, double wallThickness, double bottomThickness, bool roofFlag)
{
	if (m_2DMouldVertices.size() < 3 || height <= 0.0 || wallThickness <= 0.0 || bottomThickness <= 0.0)
		return nullptr;

	trimesh::TriMesh* extrusionMesh = new trimesh::TriMesh;

	int i;
	int mouldPosNum = m_2DMouldVertices.size();
	trimesh::dvec3 zDir(0.0, 0.0, 1.0);
	trimesh::TriMesh::Face face;
	std::vector<int> bottomPosIndices;
	for (i = 0; i < mouldPosNum; i++)
	{
		bottomPosIndices.push_back(extrusionMesh->vertices.size());
		extrusionMesh->vertices.push_back(trimesh::vec3(m_2DMouldVertices[i]));
	}

	int bottomOriginIndex = extrusionMesh->vertices.size();
	extrusionMesh->vertices.push_back(trimesh::vec3(m_origin));
	int bottomUpperOriginIndex = extrusionMesh->vertices.size();
	extrusionMesh->vertices.push_back(trimesh::vec3(m_origin + bottomThickness * zDir));
	int roofOriginIndex = extrusionMesh->vertices.size();
	extrusionMesh->vertices.push_back(trimesh::vec3(m_origin + (bottomThickness + height) * zDir));
	int roofUpperOriginIndex = extrusionMesh->vertices.size();
	extrusionMesh->vertices.push_back(trimesh::vec3(m_origin + (bottomThickness + height + bottomThickness) * zDir));

	// 构建底部
	std::vector<int> bottomUpperPosIndices;
	for (i = 0; i < mouldPosNum; i++)
	{
		trimesh::dvec3 bottomPos = trimesh::dvec3(extrusionMesh->vertices[bottomPosIndices[i]]);
		trimesh::dvec3 upperPos = bottomPos +  bottomThickness * zDir;
		bottomUpperPosIndices.push_back(extrusionMesh->vertices.size());
		extrusionMesh->vertices.push_back(trimesh::vec3(upperPos));
	}

	generateFaces(bottomOriginIndex, bottomPosIndices, extrusionMesh);
	generateFaces(bottomPosIndices, bottomUpperPosIndices, extrusionMesh, true, true);
	generateFaces(bottomUpperOriginIndex, bottomUpperPosIndices, extrusionMesh, true);

	// 构建壁
	trimesh::dvec3 bottomUpperOrigin = m_origin + bottomThickness * zDir;
	std::vector<int> innerWallBottomPosIndices;
	// 生成无圆角内壁底部顶点
	if (!m_roundAngleFlag)
	{
		for (i = 0; i < mouldPosNum; i++)
		{
			trimesh::dvec3 outterPos = trimesh::dvec3(extrusionMesh->vertices[bottomUpperPosIndices[i]]);
			trimesh::dvec3 outterPosNeig1 = trimesh::dvec3(extrusionMesh->vertices[bottomUpperPosIndices[i - 1 < 0 ? mouldPosNum - 1 : i - 1]]);
			trimesh::dvec3 outterPosNeig2 = trimesh::dvec3(extrusionMesh->vertices[bottomUpperPosIndices[i + 1 >= mouldPosNum ? 0 : i + 1]]);
			trimesh::dvec3 innerPos = outterPos + wallThickness * trimesh::normalized(trimesh::normalized(outterPosNeig1 - outterPos) + trimesh::normalized(outterPosNeig2 - outterPos));
			innerWallBottomPosIndices.push_back(extrusionMesh->vertices.size());
			extrusionMesh->vertices.push_back(trimesh::vec3(innerPos));
		}
	}
	// 生成圆角内壁底部顶点
	else
	{
		std::vector<trimesh::dvec3> innerWallBottomCtrlPosArray;
		for (i = 0; i < m_2DMouldCtrlVertices.size(); i++)
		{
			trimesh::dvec3 outterPos = m_2DMouldCtrlVertices[i];
			trimesh::dvec3 outterPosNeig1 = m_2DMouldCtrlVertices[i - 1 < 0 ? m_2DMouldCtrlVertices.size() - 1 : i - 1];
			trimesh::dvec3 outterPosNeig2 = m_2DMouldCtrlVertices[i + 1 >= m_2DMouldCtrlVertices.size() ? 0 : i + 1];
			trimesh::dvec3 innerPos = outterPos + wallThickness * trimesh::normalized(trimesh::normalized(outterPosNeig1 - outterPos) + trimesh::normalized(outterPosNeig2 - outterPos));
			innerPos = innerPos + bottomThickness * zDir;
			innerWallBottomCtrlPosArray.push_back(innerPos);
		}

		double t, deltaT = 1.0 / m_roundAngleSampleCount;
		for (int i = 0; i < innerWallBottomCtrlPosArray.size(); i++)
		{
			trimesh::dvec3 curVertex = innerWallBottomCtrlPosArray[i];
			trimesh::dvec3 preVertex = innerWallBottomCtrlPosArray[i - 1 < 0 ? innerWallBottomCtrlPosArray.size() - 1 : i - 1];
			trimesh::dvec3 postVertex = innerWallBottomCtrlPosArray[i + 1 >= innerWallBottomCtrlPosArray.size() ? 0 : i + 1];

			double curpreRadius = m_roundAngleRadiusRatePerCtrlVertex[i].first * trimesh::distance(curVertex, preVertex);
			double curpostRadius = m_roundAngleRadiusRatePerCtrlVertex[i].second * trimesh::distance(curVertex, postVertex);
			trimesh::dvec3 preControl = curVertex + curpreRadius * trimesh::normalized(preVertex - curVertex);
			trimesh::dvec3 postControl = curVertex + curpostRadius * trimesh::normalized(postVertex - curVertex);

			t = 0.0;
			for (; t <= 1.0; t += deltaT)
			{
				trimesh::dvec3 innerPos = BezierSample(t, preControl, curVertex, postControl);
				innerWallBottomPosIndices.push_back(extrusionMesh->vertices.size());
				extrusionMesh->vertices.push_back(trimesh::vec3(innerPos));
			}
		}
	}

	std::vector<int> outterWallUpperPosIndices;
	std::vector<int> innerWallUpperPosIndices;
	for (i = 0; i < mouldPosNum; i++)
	{
		trimesh::dvec3 outterPos = trimesh::dvec3(extrusionMesh->vertices[bottomUpperPosIndices[i]]);
		trimesh::dvec3 innerPos = trimesh::dvec3(extrusionMesh->vertices[innerWallBottomPosIndices[i]]);

		trimesh::dvec3 outterWallUpperPos = outterPos + height * zDir;
		trimesh::dvec3 innerWallUpperPos = innerPos + height * zDir;

		outterWallUpperPosIndices.push_back(extrusionMesh->vertices.size());
		extrusionMesh->vertices.push_back(trimesh::vec3(outterWallUpperPos));

		innerWallUpperPosIndices.push_back(extrusionMesh->vertices.size());
		extrusionMesh->vertices.push_back(trimesh::vec3(innerWallUpperPos));
	}

	generateFaces(bottomUpperPosIndices, innerWallBottomPosIndices, extrusionMesh, true);
	generateFaces(bottomUpperPosIndices, outterWallUpperPosIndices, extrusionMesh, true, true);
	generateFaces(innerWallBottomPosIndices, innerWallUpperPosIndices, extrusionMesh, true);
	generateFaces(outterWallUpperPosIndices, innerWallUpperPosIndices, extrusionMesh, true, true);

	// 封顶
	if (roofFlag)
	{
		std::vector<int> roofUpperPosIndices;
		for (i = 0; i < mouldPosNum; i++)
		{
			trimesh::dvec3 upperWallPos = trimesh::dvec3(extrusionMesh->vertices[outterWallUpperPosIndices[i]]);
			trimesh::dvec3 upperRoofPos = upperWallPos + bottomThickness * zDir;
			roofUpperPosIndices.push_back(extrusionMesh->vertices.size());
			extrusionMesh->vertices.push_back(trimesh::vec3(upperRoofPos));
		}

		generateFaces(roofOriginIndex, outterWallUpperPosIndices, extrusionMesh);
		generateFaces(outterWallUpperPosIndices, roofUpperPosIndices, extrusionMesh, true, true);
		generateFaces(roofUpperOriginIndex, roofUpperPosIndices, extrusionMesh, true);
	}

	return extrusionMesh;
}

trimesh::dvec3 ModelExtruder::BezierSample(double t, trimesh::dvec3 preCtrl, trimesh::dvec3 midCtrl, trimesh::dvec3 postCtrl)
{
	return (1 - t) * (1 - t) * preCtrl + 2 * t * (1 - t) * midCtrl + t * t * postCtrl;
}

void ModelExtruder::generateFaces(std::vector<int> indicesA, std::vector<int> indicesB, trimesh::TriMesh* resultMesh, bool ringFlag, bool ccwFlag)
{
	int i, j;
	int fvIndexA = 0, fvIndexB = 1, fvIndexC = 2;
	if (ccwFlag)
	{
		fvIndexA = 2;
		fvIndexB = 1;
		fvIndexC = 0;
	}
	trimesh::TriMesh::Face face;
	for (i = 1, j = 1; i < indicesA.size() || j < indicesB.size();)
	{
		face[fvIndexA] = indicesA[i];
		face[fvIndexB] = indicesA[i - 1];
		face[fvIndexC] = indicesB[j - 1];
		resultMesh->faces.push_back(face);

		face[fvIndexA] = indicesA[i];
		face[fvIndexB] = indicesB[j - 1];
		face[fvIndexC] = indicesB[j];
		resultMesh->faces.push_back(face);

		if (i < indicesA.size())
			i++;
		if (j < indicesB.size())
			j++;
	}

	if (ringFlag)
	{
		face[fvIndexA] = indicesA[0];
		face[fvIndexB] = indicesA[i - 1];
		face[fvIndexC] = indicesB[j - 1];
		resultMesh->faces.push_back(face);

		face[fvIndexA] = indicesA[0];
		face[fvIndexB] = indicesB[j - 1];
		face[fvIndexC] = indicesB[0];
		resultMesh->faces.push_back(face);
	}
}

void ModelExtruder::generateFaces(int originIndex, std::vector<int> indices, trimesh::TriMesh* resultMesh, bool ccwFlag)
{
	int i;
	int fvIndexA = 0, fvIndexB = 1, fvIndexC = 2;
	if (ccwFlag)
	{
		fvIndexA = 2;
		fvIndexB = 1;
		fvIndexC = 0;
	}
	trimesh::TriMesh::Face face;
	for (i = 1; i < indices.size(); i++)
	{
		face[fvIndexA] = indices[i];
		face[fvIndexB] = indices[i - 1];
		face[fvIndexC] = originIndex;
		resultMesh->faces.push_back(face);
	}

	face[fvIndexA] = indices[0];
	face[fvIndexB] = indices[i - 1];
	face[fvIndexC] = originIndex;
	resultMesh->faces.push_back(face);
}

