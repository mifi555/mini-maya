//#include "skeleton.h"


//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonArray>
//#include <QFile>
//#include <QQuaternion>

//#include <iostream>

//Skeleton::Skeleton(OpenGLContext* context) : Drawable(context), joints()
//{

//}



//Joint* Skeleton::getRootJoint() const {
//    return rootJoint;
//}


//void Skeleton::setRootJoint(Joint* joint) {
//    rootJoint = joint;
//}

//const std::vector<std::unique_ptr<Joint>>& Skeleton::getJoints() const {
//    return joints;
//}


////void Skeleton::create() {
////    std::vector<glm::vec4> pos;
////    std::vector<glm::vec4> col;
////    std::vector<GLuint> idx;

////    for (const auto& joint : this->joints) {

////        joint->create();
////        //calls create on each joint


//////        count = idx.size();

//////        generateIdx();
//////        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
//////        mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

//////        generatePos();
//////        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
//////        mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

//////        generateCol();
//////        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
//////        mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);
////    }


////}


