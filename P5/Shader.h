//
// Created by luuca on 07/10/2026.
//

#ifndef P4_SHADER_H
#define P4_SHADER_H

#include <string>
#include "glad/glad.h"

namespace PAG {
    class Shader {
    private:
        GLuint idVS = 0;
        GLuint idFS = 0;
        GLuint idSP = 0;

        std::string readFile(const std::string& filename);

    public:
        Shader();
        ~Shader();

        void create(const std::string& name);
        GLuint getId() const;

    };
    ;
} // PAG

#endif //P4_SHADER_H