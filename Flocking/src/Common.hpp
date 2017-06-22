#ifndef COMMON_HPP
#define COMMON_HPP

#include <random>

const glm::vec4 BOID_COLORS[] = {
    glm::vec4(1, 0, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 1, 1),
    glm::vec4(1, 0, 1, 1),
};
const unsigned int BOID_GROUPS = 1; //sizeof(BOID_COLORS) / sizeof(glm::vec4);

#endif // COMMON_HPP
