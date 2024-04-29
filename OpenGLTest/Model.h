#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GLFW/glfw3.h>

class Model
{
public:

    struct Vehicle 
    {
        glm::vec3 Pos;
        glm::vec3 CurrentVelocity;
        float currentSpeed;
        float acceleration = 5.0f;
        float deceleration = 2.5f;
        float maxSpeed = 10.0f;
    };

    Vehicle v;

    Model(std::string const& path, glm::vec3& initPos);
    void Draw(Shader& shader, Camera& cam);
    void HandleVehicleInputs(GLFWwindow* window, float deltaTime);

private:
    // model data
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    void ModelPosition(Shader& shader, Camera& cam);
    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string textureType);
};
#endif