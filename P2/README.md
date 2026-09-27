# pag-p2


1. PAG::Renderer. Nueva clase que encapsula todas las llamadas a OpenGL. Esto permite cambiar utilizar otra biblioteca en lugas de GLFW y no tener que cambiar nada en Renderer.
2. Dear ImGui añadido con una ventana de mensajes y un control de color de fondo
3. PAG::Gui. Nueva clase que encapsula la biblioteca ImGui
4. main.cpp solo gestiona eventos con GLFW y GLAD y las ventanas

Con esto conseguimos que en vez de mostrar el color de fondo directamente, y cambiarlo moviendo el raton,
ahora solo se cambia el color de fondo con una ventana de control.
Estos cambios (y los movimientos del raton) se muestran en una ventana de mensajes


Renderer dibuja la escena, pero GUI es la interfaz de usuario, por eso debe volver a dibujarse en cada iteración
y no solo cuando se debe refrescar la escena porque ha habido un movimiento de ratón.