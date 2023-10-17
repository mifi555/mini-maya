#include "mesh.h"

#include "face.h"
#include "halfedge.h"
#include "vertex.h"

    //Populates the VBOs of the Drawable.
    //Implemented by subclasses of Drawable

Mesh::Mesh(OpenGLContext* context) : Drawable(context), faces(), halfEdges(), vertices() {}

void Mesh::create() {

        std::vector<glm::vec4> pos;
        std::vector<glm::vec4> nor;
        std::vector<glm::vec4> col;
        std::vector<GLuint> idx;

        int totalNumberOfVerticesInMesh = 0;
        //std::cout << faces.size()<< std::endl;
        for (const auto& face : this->faces) {
            HalfEdge *start_edge = face->getHalfEdge();
            HalfEdge *current_edge = start_edge;

            // Calculate face normal

            std::cout << "vertex" << std::endl;

            std::cout << "vertex id: " << current_edge->getVertex()->getId() << std::endl;



            //GLuint first_vertex_idx = pos.size();

            //make a counter of the number of indices that constitue the face
            int numFaceVerts = 0;

            do {

                glm::vec3 v1 = current_edge->getVertex()->getPosition();
                glm::vec3 v2 = current_edge->getNext()->getVertex()->getPosition();
                glm::vec3 v3 = current_edge->getNext()->getNext()->getVertex()->getPosition();
                glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

                pos.push_back(glm::vec4(current_edge->getVertex()->getPosition(), 1));

                std::cout << pos.back().x << " " << pos.back().y << " " << pos.back().z << std::endl;

                nor.push_back(glm::vec4(normal, 1));
                col.push_back(glm::vec4(face->getColor(), 1));

                current_edge = current_edge->getNext();

                numFaceVerts++;

            } while (current_edge != start_edge);


            // create triangle indices for face
            for (int i = 0; i < numFaceVerts - 2; i++) {
                idx.push_back(totalNumberOfVerticesInMesh);
                idx.push_back(totalNumberOfVerticesInMesh + i + 1);
                idx.push_back(totalNumberOfVerticesInMesh + i + 2);

//                std::cout << totalNumberOfVerticesInMesh << std::endl;
//                std::cout << totalNumberOfVerticesInMesh + i + 1 << std::endl;
//                std::cout << totalNumberOfVerticesInMesh + i + 2 << std::endl;



            }

            totalNumberOfVerticesInMesh += numFaceVerts;
        }

//        std::cout << idx.size() << std::endl;
//        std::cout << pos.size() << std::endl;
//        std::cout << nor.size() << std::endl;
//        std::cout << col.size() << std::endl;



        count = idx.size();

        generateIdx();
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
        mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

        generatePos();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
        mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

        generateNor();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
        mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

        generateCol();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);


    }


