# pag-p3


1. PAG::Renderer. Nueva clase que encapsula todas las llamadas a OpenGL. Esto permite cambiar utilizar otra biblioteca en lugas de GLFW y no tener que cambiar nada en Renderer.
2. Dear ImGui añadido con una ventana de mensajes y un control de color de fondo
3. PAG::Gui. Nueva clase que encapsula la biblioteca ImGui
4. main.cpp solo gestiona eventos con GLFW y GLAD y las ventanas

Con esto conseguimos que en vez de mostrar el color de fondo directamente, y cambiarlo moviendo el raton,
ahora solo se cambia el color de fondo con una ventana de control.
Estos cambios (y los movimientos del raton) se muestran en una ventana de mensajes


Renderer dibuja la escena, pero GUI es la interfaz de usuario, por eso debe volver a dibujarse en cada iteración
y no solo cuando se debe refrescar la escena porque ha habido un movimiento de ratón.

**CAMBIOS REALIZADOS:**
1. Representación de un triángulo
    - Nuevos atributos (identificadores de los objetos OpenGL)
    - creaShaderProgram(); crea, compila y enlaza el shader program
    - creaModelo(); crea el VAO (contiene VBO e IBO), los VBO (3 vertices con su pos y color) y el IBO (orden vertices) del triángulo
    - el destructor libera todos los recursos

2. Comprobación de errores, lanzando una excepcion que se muestra en la ventana de mensajes
3. Ficheros aparte con los shaders del triángulo
    - con readFile()
4. Se le añade colores a los vértices del triángulo con un segundo atributo.
    - Implementado tanto de forma entrelazada como no entrelazada

**¿POR QUÉ SE DEFORMA EL TRIANGULO AL REDIMENSIONAR LA VENTANA?**
 
Cuando se crea una ventana, esta se inicializa en el espacio de coordenadas normalizadas(-1,-1) - (1,1).
El último paso de renderizar es 'mover' ese cuadrado a los pixeles que queramos con una formula -> 
x = w(x + 1)/2
y = h(y + 1)/2

Es decir, los vertices del triángulo se determinan en el espacio de coordenadas y el viewport 'recalcula' todas las coordenadas.
Se aplica la formula en los 3 vértices por lo que estos se 'mueven' con la pantalla.

Esto sucede en nuestro codigo ya que no aplicamos ninguna transformación para evitarlo.
Habría que aplicar una transformación de proyección, aplicando una matriz de proyección que incluta la relacion de aspecto (w/h)