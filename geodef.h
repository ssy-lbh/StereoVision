#ifndef __GEODEF__
#define __GEODEF__

#include "define.h"
#include "vecmath.h"
#include "list.h"

class Vertex;
class Edge;
class Face;

class Vertex {
public:
    Vector3 pos;
    Vector3 normal = Vector3::forward;
    Vector3 uv = Vector3::zero;
    Vector3 color = Vector3::one;
    List<Edge*> edges;
    List<Face*> faces;
    size_t index;

    Vertex();
    Vertex(Vector3 pos);
    ~Vertex();

    void UpdateNormal();
    void AddEdge(Edge* e);
    void AddFace(Face* f);
    bool FindEdge(Edge* e);
    bool FindFace(Face* f);
    // @return success
    bool DeleteEdge(Edge* e);
    bool DeleteFace(Face* f);
    Edge* EdgeRelateTo(Vertex* v);
};

class Edge {
public:
    Vertex* v1;
    Vertex* v2;
    List<Face*> faces;

    Edge();
    Edge(Vertex* v1, Vertex* v2);
    ~Edge();

    void AddFace(Face* f);
    bool FindFace(Face* f);
    bool DeleteFace(Face* f);
    void DeleteSelfReference();
    void DeleteSelfReferenceExcept(Vertex* v);
    Face* FaceRelateTo(Vertex* v);
};

class Face {
public:
    Vector3 normal;
    List<Vertex*> vertices;
    List<Edge*> edges;

    Face();
    ~Face();

    void AddVertex(Vertex* v);
    void AddEdge(Edge* e);
    bool FindVertex(Vertex* v);
    bool FindEdge(Edge* e);
    // @return success
    bool DeleteVertex(Vertex* v);
    bool DeleteEdge(Edge* e);
    void DeleteSelfReference();
    void DeleteSelfReferenceExcept(Vertex* v);
};

#endif
