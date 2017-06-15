#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const glm::vec4 BOID_COLORS[] = {
    glm::vec4(1, 0, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 1, 1),
    glm::vec4(1, 0, 1, 1),
};
const unsigned int BOID_GROUPS = 1; //sizeof(BOID_COLORS) / sizeof(glm::vec4);

#endif // CONSTANTS_HPP
