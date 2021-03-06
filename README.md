# Solver

## Ejecución
```bash
make
./run
```

## Idea general de la solución

Tenemos esta jerarquía de operaciones:

1. ()
2. ^
3. *, /
4. +, -

**Observación sobre operadores unarios**
1. -x = ((-1) * x)
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
6. Determinar el nivel de cada posición y con que parentesis se balancea cada parentesis
7. Determinar el operando (+, -, *, -, ^) más cercano hacia la derecha para cada posición que este en el mismo nivel
8. Construir un árbol a partir del array creado llamando a `buildTree(0, tamaño del array creado - 1, root)`
9. Leer los valores de las variables
10. Evaluar la expresión usando el árbol construido y los valores leidos


En el paso 8 notemos que básicamente consiste en:
```python

match[l] = posición del parentesis con el que se matchea el parentesis en esta posición o -1 si no se matchea

double build(l, r, root):
  if (arr[l] == '(' and arr[r] == ')' y match[l] = r):
    return build(l + 1, r - 1, root)
  op = getNextOperation(l, r)
  return build(l, op - 1) operation[op] build(op + 1, r);
```

* Complejidad de construcción: O(n log n)  

* Complejidad de evaluación: O(n log n)  
