//
// Created by luuca on 07/10/2026.
//

#include "Shader.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace PAG {
    Shader::Shader() {}

    Shader::~Shader() {
        if ( idVS != 0 )
        { glDeleteShader ( idVS );
        }
        if ( idFS != 0 )
        { glDeleteShader ( idFS );
        }
        if ( idSP != 0 )
        { glDeleteProgram ( idSP );
        }
    }


    std::string Shader::readFile ( const std::string& filename ) {
        std::ifstream shaderSourceFile;
        shaderSourceFile.open ( filename );
        if ( !shaderSourceFile ) {
            throw std::runtime_error ( "Cannot open shader source file: " + filename );
        }
        std::stringstream shaderSourceStream;
        shaderSourceStream << shaderSourceFile.rdbuf ();

        std::string shaderSourceString = shaderSourceStream.str ();
        shaderSourceFile.close ();

        return shaderSourceString;
    }


    /**
    * Método para crear, compilar y enlazar el shader program
    * @note No se incluye ninguna comprobación de errores
    */
    void PAG::Shader::create( const std::string& name)
    {
        std::string miVertexShader   = readFile ( name + "-vs.glsl" );
        std::string miFragmentShader = readFile ( name + "-fs.glsl" );

        GLuint newVS = 0;
        GLuint newFS = 0;
        GLuint newSP = 0;

        newVS = glCreateShader(GL_VERTEX_SHADER);
        if ( newVS == 0 ) {
            throw std::runtime_error ( "Cannot create vertex shader object." );
        }
        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource ( newVS, 1, &fuenteVS, nullptr );
        glCompileShader ( newVS );

        GLint resultadoVS = 0;
        glGetShaderiv ( newVS, GL_COMPILE_STATUS, &resultadoVS );
        if ( resultadoVS == GL_FALSE ) {
            GLint logLen = 0;
            std::string logString = "";
            glGetShaderiv ( newVS, GL_INFO_LOG_LENGTH, &logLen );
            if ( logLen > 0 ) {
                char* cLogString = new char[logLen];
                GLint written = 0;
                glGetShaderInfoLog ( newVS, logLen, &written, cLogString );
                logString.assign ( cLogString );
                delete[] cLogString;
            }
            throw std::runtime_error ( "Cannot compile vertex shader:\n" + logString );
        }

        newFS  = glCreateShader ( GL_FRAGMENT_SHADER );
        if ( newFS  == 0 ) {
            throw std::runtime_error ( "Cannot create fragment shader object." );
        }
        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource ( newFS , 1, &fuenteFS, nullptr );
        glCompileShader ( newFS  );

        GLint resultadoFS = 0;
        glGetShaderiv ( newFS , GL_COMPILE_STATUS, &resultadoFS );
        if ( resultadoFS == GL_FALSE ) {
            GLint logLen = 0;
            std::string logString = "";
            glGetShaderiv ( newFS , GL_INFO_LOG_LENGTH, &logLen );
            if ( logLen > 0 ) {
                char* cLogString = new char[logLen];
                GLint written = 0;
                glGetShaderInfoLog ( newFS , logLen, &written, cLogString );
                logString.assign ( cLogString );
                delete[] cLogString;
            }
            throw std::runtime_error ( "Cannot compile fragment shader:\n" + logString );
        }

        newSP  = glCreateProgram ();
        if ( newSP  == 0 ) {
            throw std::runtime_error ( "Cannot create shader program." );
        }
        glAttachShader ( newSP , newVS );
        glAttachShader ( newSP , newFS );
        glLinkProgram ( newSP  );

        GLint linkSuccess = 0;
        glGetProgramiv ( newSP , GL_LINK_STATUS, &linkSuccess );
        if ( linkSuccess == GL_FALSE ) {
            GLint logLen = 0;
            std::string logString = "";
            glGetProgramiv ( newSP, GL_INFO_LOG_LENGTH, &logLen );
            if ( logLen > 0 ) {
                char* cLogString = new char[logLen];
                GLint written = 0;
                glGetProgramInfoLog ( newSP, logLen, &written, cLogString );
                logString.assign ( cLogString );
                delete[] cLogString;
            }
            throw std::runtime_error ( "Cannot link shader program:\n" + logString );
        }

        if (idVS != 0) {
            glDeleteShader(idVS);
        }

        if (idFS != 0) {
            glDeleteShader(idFS);
        }

        if (idSP != 0) {
            glDeleteProgram(idSP);
        }

        idVS = newVS;
        idFS = newFS;
        idSP = newSP;
    }

    GLuint Shader::getId () const {
        return idSP;
    }

} // PAG