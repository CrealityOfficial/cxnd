#include "dumplicate.h"
#include "ccglobal/tracer.h"
#include "trimesh2/TriMesh.h"

#include <unordered_map>

namespace cxnd
{
	void dumplicateMesh(trimesh::TriMesh* mesh, ccglobal::Tracer* tracer)
	{
		if (!mesh)
			return;

		size_t vertexNum = mesh->vertices.size();

		struct hash_vec3
		{
			size_t operator()(const trimesh::vec3& v)const
			{
#if _WIN32
				return abs(v.x) * 10000.0f / 23.0f + abs(v.y) * 10000.0f / 19.0f + abs(v.z) * 10000.0f / 17.0f;
#else
				return fabs(v.x) * 10000.0f / 23.0f + fabs(v.y) * 10000.0f / 19.0f + abs(v.z) * 10000.0f / 17.0f;
#endif
			}

		};
		struct equal_vec3
		{
			bool operator()(const trimesh::vec3& v1, const trimesh::vec3& v2) const
			{
				return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
			}
		};
		typedef std::unordered_map<trimesh::vec3, int, hash_vec3, equal_vec3> unique_point;
		unique_point points(vertexNum * 3 / 10 + 1);

		typedef unique_point::iterator point_iterator;

		size_t faceNum = mesh->faces.size();

		if (vertexNum == 0 || faceNum == 0)
			return;

		trimesh::TriMesh* optimizeMesh = new trimesh::TriMesh();
		bool interuptted = false;

		std::vector<int> vertexMapper;
		vertexMapper.resize(vertexNum, -1);

		SAFE_TRACER(tracer, "dumplicateMesh %d", (int)vertexNum);

		size_t pVertex = vertexNum / 20;
		if (pVertex == 0)
			pVertex = vertexNum;

		for (size_t i = 0; i < vertexNum; ++i)
		{
			trimesh::point p = mesh->vertices.at(i);
			point_iterator it = points.find(p);
			if (it != points.end())
			{
				int index = (*it).second;
				vertexMapper.at(i) = index;
			}
			else
			{
				int index = (int)points.size();
				points.insert(unique_point::value_type(p, index));

				vertexMapper.at(i) = index;
			}

			if (i % pVertex == 1)
			{
				if (tracer)
				{
					tracer->formatMessage("dumplicateMesh %i", (int)i);
					tracer->progress((float)i / (float)vertexNum);
					if (tracer->interrupt())
					{
						interuptted = true;
						break;
					}
				}
			}
		}

		SAFE_TRACER(tracer, "dumplicateMesh over %d", (int)points.size());
		if (interuptted)
		{
			delete optimizeMesh;
			return;
		}

		trimesh::TriMesh* omesh = optimizeMesh;
		omesh->vertices.resize(points.size());
		for (point_iterator it = points.begin(); it != points.end(); ++it)
		{
			omesh->vertices.at(it->second) = it->first;
		}

		omesh->faces = mesh->faces;
		for (size_t i = 0; i < faceNum; ++i)
		{
			trimesh::TriMesh::Face& of = omesh->faces.at(i);
			for (int j = 0; j < 3; ++j)
			{
				int index = of[j];
				of[j] = vertexMapper[index];
			}
		}

		mesh->vertices.swap(omesh->vertices);
		mesh->faces.swap(omesh->faces);
		mesh->need_bbox();

		delete omesh;
	}
}