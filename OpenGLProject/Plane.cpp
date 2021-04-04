#include <cmath>
#include <vector>
#include <iostream>
#include <glm\glm.hpp>
#include "Plane.h"
using namespace std;

Plane::Plane() {
	init(100);
}

Plane::Plane(int range, float a, float b, float c) {
	init(range,a, b,c);
}


void Plane::init(int range, float a, float b, float c,float d) {
	
	int num = range * 2 + 1;
	numVertices = num*num;
	numIndices = (num-1) * (num - 1) * 6;
	for (int i = 0; i < numVertices; i++) { vertices.push_back(glm::vec3()); }
	for (int i = 0; i < numVertices; i++) { texCoords.push_back(glm::vec2()); }
	for (int i = 0; i < numVertices; i++) { normals.push_back(glm::vec3()); }
	//for (int i = 0; i < numVertices; i++) { tangents.push_back(glm::vec3()); }
	for (int i = 0; i < numIndices; i++) { indices.push_back(0); }

	// calculate triangle vertices
	for (int i = 0,x=-range; i != num; i++,x++) {//��i�У���num�У��ʴ�0��ʼֱ��num-1Ϊֹ
		for (int j = 0,y=-range; j !=num; j++,y++) {//��j��
			float z = (-d-a*x-b*y)/c;
			vertices[i*num + j] = glm::vec3(x, y, z);
			texCoords[i * num + j] = glm::vec2(((float)j/(num-1)), ((float)i / (num-1)));//x����j������
			normals[i*num + j] = glm::vec3(a,b,c);

			/* calculate tangent vector��ʱ����
			if (((x == 0) && (y == 1) && (z == 0)) || ((x == 0) && (y == -1) && (z == 0))) {
				tangents[i*(prec + 1) + j] = glm::vec3(0.0f, 0.0f, -1.0f);
			}
			else {
				tangents[i*(prec + 1) + j] = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(x, y, z));
			}
			*/
		}
	}
	// calculate triangle indices
	for (int i = 0; i!=num-1; i++) {
		for (int j = 0; j!=num-1; j++) {//λ�ڵ�i,j�����㣬�������������Σ����Ƿֱ��ǣ�i*num+j.�����0��ʼ��num-2ֹͣ����
			indices[6 * (i*(num-1) + j) + 0] = i*num + j;
			indices[6 * (i* (num - 1) + j) + 1] = i*num + j + 1;
			indices[6 * (i* (num - 1) + j) + 2] = (i + 1)*num + j;
			indices[6 * (i* (num - 1) + j) + 3] = i*num + j + 1;
			indices[6 * (i* (num - 1) + j) + 4] = (i + 1)*num + j + 1;
			indices[6 * (i* (num - 1) + j) + 5] = (i + 1)*num + j;
		}
	}
}

int Plane::getNumVertices() { return numVertices; }
int Plane::getNumIndices() { return numIndices; }
std::vector<int> Plane::getIndices() { return indices; }
std::vector<glm::vec3> Plane::getVertices() { return vertices; }
std::vector<glm::vec2> Plane::getTexCoords() { return texCoords; }
std::vector<glm::vec3> Plane::getNormals() { return normals; }
std::vector<glm::vec3> Plane::getTangents() { return tangents; }