#include "complexpolygon2d.h"
#include <fstream>

namespace cxnd
{
	ComplexPolygon2D::ComplexPolygon2D()
	{

	}

	ComplexPolygon2D::ComplexPolygon2D(const ComplexPolygon2D& polygon)
	{
		polygons = polygon.polygons;
		points = polygon.points;
	}

	ComplexPolygon2D::~ComplexPolygon2D()
	{

	}

	void polygonSave(const std::string& fileName, const ComplexPolygon2D& polygon)
	{
		std::fstream out(fileName, std::ios::out | std::ios::binary);
		if (out.is_open())
		{
			int pNum = (int)polygon.points.size();
			out.write((const char*)&pNum, sizeof(int));
			if (pNum > 0)
				out.write((const char*)&polygon.points.at(0), sizeof(trimesh::dvec2) * pNum);

			int plNum = (int)polygon.polygons.size();
			out.write((const char*)&plNum, sizeof(int));
			if (plNum > 0)
			{
				for (int i = 0; i < plNum; ++i)
				{
					int num = (int)polygon.polygons.at(i).size();
					out.write((const char*)&num, sizeof(int));
					if (num)
						out.write((const char*)&polygon.polygons.at(i).at(0), num * sizeof(int));
				}
			}
		}
		out.close();
	}

	void polygonLoad(const std::string& fileName, ComplexPolygon2D& polygon)
	{
		std::fstream in(fileName, std::ios::in | std::ios::binary);
		if (in.is_open())
		{
			int pNum = 0;
			in.read((char*)&pNum, sizeof(int));
			if (pNum > 0)
			{
				polygon.points.resize(pNum);
				in.read((char*)&polygon.points.at(0), sizeof(trimesh::dvec2) * pNum);
			}

			int plNum = 0;
			in.read((char*)&plNum, sizeof(int));
			if (plNum > 0)
			{
				polygon.polygons.resize(plNum);
				for (int i = 0; i < plNum; ++i)
				{
					int num = 0;
					in.read((char*)&num, sizeof(int));
					if (num)
					{
						polygon.polygons.at(i).resize(num);
						in.read((char*)&polygon.polygons.at(i).at(0), num * sizeof(int));
					}
				}
			}
		}
		in.close();
	}
}