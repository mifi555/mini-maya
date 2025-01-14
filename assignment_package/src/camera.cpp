#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    //**added
    eye = glm::vec3(0, 0, 0);
    //
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp),

    //**added
    theta(0.0f),
    phi(0.0f),
    zoom(10.0f)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
    //redefine as unit vectors

    //eye: (0, 0, 0, 1)
    glm::vec4 eye_unit = glm::vec4(0, 0, 0, 1);
    //Forward: (0, 0, 1, 0)
    glm::vec4 forward_unit = glm::vec4(0, 0, 1, 0);
    //Up: (0, 1, 0, 0)
    glm::vec4 up_unit = glm::vec4(0, 1, 0, 0);
    //Right: (1, 0, 0, 0)
    glm::vec4 right_unit = glm::vec4(1, 0, 0, 0);

    //compute mat4 transformation matrix
    glm::mat4 transformationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(up_unit)) //rotation around y
                                     * glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(right_unit)) * //rotation around x
                                     glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom));

    eye_unit = transformationMatrix * eye_unit;
    forward_unit = transformationMatrix * forward_unit;
    up_unit = transformationMatrix * up_unit;
    right_unit = transformationMatrix * right_unit;

    //multiply unit vectors to vec3
    eye = glm::vec3(eye_unit);
    look = glm::vec3(forward_unit);
    up = glm::vec3(up_unit);
    right = glm::vec3(right_unit);

//    look = glm::normalize(ref - eye);
//    right = glm::normalize(glm::cross(look, world_up));
//    up = glm::cross(right, look);

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = glm::length(ref - eye);
    aspect = width / static_cast<float>(height);
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

void Camera::RotateAboutUp(float deg)
{
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), up);
//    ref = ref - eye;
//    ref = glm::vec3(rotation * glm::vec4(ref, 1));
//    ref = ref + eye;

    //***
    theta += deg;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
//    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(deg), right);
//    ref = ref - eye;
//    ref = glm::vec3(rotation * glm::vec4(ref, 1));
//    ref = ref + eye;

    //***
    phi += deg;
    RecomputeAttributes();
}

//zooming in and out

void Camera::TranslateAlongLook(float amt)
{
    //    glm::vec3 translation = look * amt;
    //    eye += translation;
    //    ref += translation;

    zoom -= amt;
    RecomputeAttributes();

}

//panning
void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;

    RecomputeAttributes();

}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;

    RecomputeAttributes();

}
