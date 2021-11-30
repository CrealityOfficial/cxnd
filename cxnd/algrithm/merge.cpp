#include "merge.h"
#include "trimesh2/TriMesh.h"

namespace cxnd
{
	trimesh::TriMesh* mergeMeshes(std::vector<trimesh::TriMesh*>& inputs, bool releaseInput)
	{
		size_t size = inputs.size();
		if (size == 0)
			return nullptr;

		if (size == 1)
		{
			trimesh::TriMesh* mesh = nullptr;
			if (releaseInput)
				mesh = inputs.at(0);
			else
			{
				mesh = new trimesh::TriMesh();
				*mesh = *inputs.at(0);
			}

			inputs.clear();
			return mesh;
		}

		size_t addVertexSize = 0;
		size_t addTriangleSize = 0;
		size_t addUVSize = 0;

		size_t meshSize = inputs.size();
		for (size_t i = 0; i < meshSize; ++i)
		{
			if (inputs.at(i))
			{
				addVertexSize += inputs.at(i)->vertices.size();
				addTriangleSize += inputs.at(i)->faces.size();
				addUVSize += inputs.at(i)->cornerareas.size();
			}
		}

		trimesh::TriMesh* outMesh = nullptr;
		if (addTriangleSize > 0 && addVertexSize > 0)
			outMesh = new trimesh::TriMesh();

		if (outMesh)
		{
			outMesh->vertices.reserve(addVertexSize);
			outMesh->cornerareas.reserve(addUVSize);
			outMesh->faces.reserve(addTriangleSize);

			size_t startFaceIndex = 0;
			size_t startVertexIndex = 0;
			size_t startUVIndex = 0;
			for (size_t i = 0; i < meshSize; ++i)
			{
				trimesh::TriMesh* mesh = inputs.at(i);
				if (mesh)
				{
					int vertexNum = (int)mesh->vertices.size();
					int faceNum = (int)mesh->faces.size();
					int uvNum = (int)mesh->cornerareas.size();
					if (vertexNum > 0 && faceNum > 0)
					{
						outMesh->vertices.insert(outMesh->vertices.end(), mesh->vertices.begin(), mesh->vertices.end());
						outMesh->cornerareas.insert(outMesh->cornerareas.end(), mesh->cornerareas.begin(), mesh->cornerareas.end());
						outMesh->faces.insert(outMesh->faces.end(), mesh->faces.begin(), mesh->faces.end());

						size_t endFaceIndex = startFaceIndex + faceNum;
						if (startVertexIndex > 0)
						{
							for (size_t ii = startFaceIndex; ii < endFaceIndex; ++ii)
							{
								trimesh::TriMesh::Face& face = outMesh->faces.at(ii);
								for (int j = 0; j < 3; ++j)
									face[j] += startVertexIndex;
							}
						}

						startFaceIndex += faceNum;
						startVertexIndex += vertexNum;
						startUVIndex += uvNum;
					}
				}
			}
		}

		if (releaseInput)
		{
			for (trimesh::TriMesh* mesh : inputs)
				delete mesh;
			inputs.clear();
		}
		return outMesh;
	}
}