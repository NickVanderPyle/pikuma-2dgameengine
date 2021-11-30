//
// Created by Nick VanderPyle on 11/28/21.
//

#ifndef INC_2DGAMEENGINE_RIGIDBODYCOMPONENT_H
#define INC_2DGAMEENGINE_RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

struct RigidBodyComponent {
    glm::vec2 velocity;

    RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0)){
        this->velocity = velocity;
    }
};
#endif //INC_2DGAMEENGINE_RIGIDBODYCOMPONENT_H
