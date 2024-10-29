#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
    union {
        glm::mat4 _mat;
        struct {
            glm::vec3 _left;
            glm::mat4::value_type _left_w;
            glm::vec3 _up;
            glm::mat4::value_type _up_w;
            glm::vec3 _fwd;
            glm::mat4::value_type _fwd_w;
            glm::vec3 _pos;
            glm::mat4::value_type _pos_w;
        };
    };

public:
    Transform() : _mat(1.0f) {}
    Transform(const glm::mat4& mat) : _mat(mat) {}

    const glm::mat4& mat() const { return _mat; }
    const glm::vec3& left() const { return _left; }
    const glm::vec3& up() const { return _up; }
    const glm::vec3& fwd() const { return _fwd; }
    const glm::vec3& pos() const { return _pos; }
    glm::vec3& pos() { return _pos; }
    const float* data() const { return &_mat[0][0]; }

    void translate(const glm::vec3& v);
    void rotate(double rads, const glm::vec3& v);
    Transform operator*(const glm::mat4& other) const { return Transform(_mat * other); }
    Transform operator*(const Transform& other) const { return Transform(_mat * other._mat); }
};

inline Transform operator*(const glm::mat4& m, const Transform& t) {
    return Transform(m * t.mat());
}
