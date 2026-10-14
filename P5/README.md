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


# pag-p4
**CAMBIOS REALIZADOS:**
1. Desacoplamiento de shader programs. En la práctica anterior se realizaba en PAG::Renderer, ahora se encarga PAG::Shader
   - Se ha movido el método create de Renderer a Shader, con el cambio de que ahora se usan identificadores temporales.
   De esa forma el shader anterior solo se elimina cuando el sustituto ha compilado, evitando que la aplicación use un shader erróneo.
   - Sin embargo se ha definido un objeto shader en Renderer, para poder usarlo durante el renderizado, pero sin crearlo ni nada.
2. Nueva caja en la interfaz para añadir los shaders desde pantalla con un botón 'load'
   - Si se produce un error, se muestra en pantalla
3. El triángulo no se muestra de primeras, si no que aparece en pantalla al cargar un shader válido

Responsabilidad de cada clase (antes->ahora):
- Renderer: dibujaba la escena, gestionando los shader -> dibuja la escena y usa un objeto shader
- GUI: interfaz de usuario (pantalla de mensajes, color y triangulo) -> interfaz de usuario (pantalla de mensajes, color y carga de shader)
- Shader: no existía -> gestiona los shaders