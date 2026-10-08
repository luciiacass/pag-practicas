#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>


#include "Renderer.h"
#include "GUI.h"

void render ( GLFWwindow* window ) {
    PAG::GUI::getInstancia().newFrame();   // Empieza el frame de interfaz
    PAG::GUI::getInstancia().draw();       // Pinta las ventanas y sus controles

    // - Si el usuario ha tocado el selector, se lo pasamos al Renderer
    if ( PAG::GUI::getInstancia().hasColorChanged() ) {
        PAG::Renderer::getInstancia().setScreenColor ( PAG::GUI::getInstancia().getBgColor() );
    }

    if (PAG::GUI::getInstancia().hasLoadShaderPressed()) {

        try {
            PAG::Renderer::getInstancia().loadShaderProgram(
                PAG::GUI::getInstancia().getShaderName()
            );

            PAG::GUI::getInstancia().addMessage(
                "Shader program cargado: " +
                PAG::GUI::getInstancia().getShaderName()
            );
        }
        catch (const std::exception& e) {
            PAG::GUI::getInstancia().addMessage(e.what());
        }
    }

    PAG::Renderer::getInstancia().refresh();  // Dibuja la escena (OpenGL)
    PAG::GUI::getInstancia().render();        // La interfaz va encima

    glfwSwapBuffers ( window );
}

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc ){
    std::string aux (desc);
    PAG::GUI::getInstancia().addMessage ( "Error de GLFW número " + std::to_string ( errno ) + ": " + aux );
}

// - Esta función callback será llamada cada vez que el área de dibujo OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window ) {
    render ( window );
    PAG::GUI::getInstancia().addMessage ( "Refresh callback called" );
}

// - Esta función callback será llamada cada vez que se cambie el tamaño del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height ){
    PAG::Renderer::getInstancia().reframe(width, height);
    PAG::GUI::getInstancia().addMessage ( "Resize callback called: " + std::to_string ( width ) + "x" + std::to_string ( height ) );
}

// - Esta función callback será llamada cada vez que se pulse una tecla dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods ){
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS ){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    PAG::GUI::getInstancia().addMessage ( "Key callback called" );
}

// - Esta función callback será llamada cada vez que se pulse algún botón del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods ){
    if ( action == GLFW_PRESS ){
        PAG::GUI::getInstancia().addMessage ( "Pulsado el botón: " + std::to_string ( button ) );
        PAG::GUI::getInstancia().mouseButtonEvent ( button, true );
    }else if ( action == GLFW_RELEASE ){
        PAG::GUI::getInstancia().addMessage ( "Soltado el botón: " + std::to_string ( button ) );
        PAG::GUI::getInstancia().mouseButtonEvent ( button, false );
    }
}

// - Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset ){
    PAG::GUI::getInstancia().addMessage ( "Movida la rueda del ratón "
            + std::to_string ( xoffset ) + " en horizontal y "
            + std::to_string ( yoffset ) + " en vertical" );

    PAG::Renderer::getInstancia().scroll(xoffset, yoffset);
    PAG::GUI::getInstancia().setBgColor ( PAG::Renderer::getInstancia().getScreenColor() );

    glfwSwapBuffers ( window );

}



int main()
{ std::cout << "Starting Application PAG - Prueba 02" << std::endl;
    // - Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback ( (GLFWerrorfun) error_callback );

    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    { std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 3 ); // OpenGL Core Profile 4.3.

    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );

    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    { std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );

    // - Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    { std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );

    PAG::GUI::getInstancia().init ( window );
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    PAG::Renderer::getInstancia().info();

    PAG::GUI::getInstancia().setBgColor ( PAG::Renderer::getInstancia().getScreenColor() );


    //glfwSetScrollCallback ( window, scroll_callback ); ASI SOLO SE CAMBIA EL COLOR CON LA VENTANA


    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de
    // dibujar.
    PAG::Renderer::getInstancia().depth();
    // El triangulo no se carga de primeras, si no al cargar el shader
    //try {
    //    PAG::Renderer::getInstancia().loadShaderProgram("shaders/pag03");
    //    //EJEMPLO FALLO:
    //    //PAG::Renderer::getInstancia().creaShaderProgram("shaders/pag0");
    //} catch ( const std::exception& e ) {
    //    PAG::GUI::getInstancia().addMessage ( e.what() );
    //}
    PAG::Renderer::getInstancia().creaModelo();

    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
        render(window);
        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones
        // de teclas o de ratón, etc. Siempre al final de cada iteración del
        // ciclo de eventos y después de glfwSwapBuffers ( window );
        glfwPollEvents ();
    }

    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;

    PAG::GUI::getInstancia().shutdown();
    glfwDestroyWindow ( window ); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
}

