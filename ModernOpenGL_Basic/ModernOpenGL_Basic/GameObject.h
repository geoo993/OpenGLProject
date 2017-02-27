#ifndef GAMEOBJECT_LOADER_H_INCLUDED
#define GAMEOBJECT_LOADER_H_INCLUDED

#include "Common.h"

struct GameObjectIndex
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
    
    bool operator<(const GameObjectIndex & r) const { return vertexIndex < r.vertexIndex; }
};

class IndexedModel
{
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    
    void CalculateNormals();
};

class GameObjectModel
{
public:
    std::vector<GameObjectIndex> GameObjectIndices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool hasUVs;
    bool hasNormals;
    
    GameObjectModel(const std::string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<GameObjectIndex*>& indexLookup, const GameObjectIndex * currentIndex, const IndexedModel& result);
    void CreateGameObjectFace(const std::string& line);
    
    glm::vec2 ParseGameObjectVec2(const std::string& line);
    glm::vec3 ParseGameObjectVec3(const std::string& line);
    GameObjectIndex ParseGameObjectIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};

#endif // GAMEOBJECT_LOADER_H_INCLUDED
