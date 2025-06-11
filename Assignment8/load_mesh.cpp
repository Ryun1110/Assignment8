#define _CRT_SECURE_NO_WARNINGS
#include "common.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <float.h>

std::vector<Vector3> gPositions;
std::vector<Vector3> gNormals;
std::vector<Triangle> gTriangles;

void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter) {
    char* token = strtok(string, delimiter);
    while (token != nullptr) {
        tokens.push_back(std::string(token));
        token = strtok(nullptr, delimiter);
    }
}

int face_index(const char* string) {
    int length = strlen(string);
    char* copy = new char[length + 1];
    memset(copy, 0, length + 1);
    strcpy(copy, string);

    std::vector<std::string> tokens;
    tokenize(copy, tokens, "/");
    delete[] copy;
    if (!tokens.empty() && atoi(tokens.front().c_str()) == atoi(tokens.back().c_str())) {
        return atoi(tokens.front().c_str());
    }
    else {
        printf("ERROR: Bad face specifier!\n");
        exit(0);
    }
}

void load_mesh(std::string fileName) {
    std::ifstream fin(fileName.c_str());
    if (!fin.is_open()) {
        printf("ERROR: Unable to load mesh from %s!\n", fileName.c_str());
        exit(0);
    }

    float xmin = FLT_MAX, xmax = -FLT_MAX;
    float ymin = FLT_MAX, ymax = -FLT_MAX;
    float zmin = FLT_MAX, zmax = -FLT_MAX;

    while (!fin.eof()) {
        char line[1024] = { 0 };
        fin.getline(line, 1024);
        if (strlen(line) <= 1) continue;

        std::vector<std::string> tokens;
        tokenize(line, tokens, " ");

        if (tokens[0] == "v") {
            float x = atof(tokens[1].c_str());
            float y = atof(tokens[2].c_str());
            float z = atof(tokens[3].c_str());

            xmin = std::min(x, xmin); xmax = std::max(x, xmax);
            ymin = std::min(y, ymin); ymax = std::max(y, ymax);
            zmin = std::min(z, zmin); zmax = std::max(z, zmax);

            gPositions.push_back({ x, y, z });
        }
        else if (tokens[0] == "vn") {
            float x = atof(tokens[1].c_str());
            float y = atof(tokens[2].c_str());
            float z = atof(tokens[3].c_str());
            gNormals.push_back({ x, y, z });
        }
        else if (tokens[0] == "f") {
            unsigned int a = face_index(tokens[1].c_str());
            unsigned int b = face_index(tokens[2].c_str());
            unsigned int c = face_index(tokens[3].c_str());
            gTriangles.push_back({ a - 1, b - 1, c - 1 });
        }
    }

    fin.close();
    printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", fileName.c_str(), gPositions.size(), gNormals.size(), gTriangles.size());
    printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
}
