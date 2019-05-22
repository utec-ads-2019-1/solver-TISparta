# Solver

# Idea general de la solución

Tenemos esta jerarquía de operaciones:

1. ()
2. ^
3. *, /
4. +, -

**Observación sobre operadores unarios**
1. -x = (-1) * x
2. +x = x

**Definición 1:** Una expresión es valida si tiene alguna de las
siguientes formas:  
- `val`  
- `var`  
- `(a)`  
- `(a op b)`  
- `a op b`  

Donde:  
- `val`: un valor numético  
- `var`: una variable  
- `a`, `b`: una expresión válida  

**Definición 2:** Llamaremos nivel de una posición a la cantidad de paréntesis
que faltan balancear estando en esa posición.

Luego, podemos solucionar el problema así:

1. Leer la expresión
2. Eliminar espacios
3. Reducir signos continuos por un solo signo
4. Usar un array donde cada elemento sea un paréntesis, un valor, una operación
   o una variable
5. Transformar el array para considerar +, - unarios
6. Determinar el nivel de cada posición
7. Determinar el operando (+, -, *, -, ^) más cercano hacia la derecha para cada posición que este en el mismo nivel
8. Construir un árbol a partir del array creado llamando a `build(0, tamaño del array creado - 1)`
9. Leer los valores de las variables
10. Evaluar la expresión usando el árbol construido y los valores leidos
