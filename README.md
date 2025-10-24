# Sistema de Recomendación - Filtrado Colaborativo

## Descripción

Sistema de recomendación basado en filtrado colaborativo implementado en C++. El programa permite realizar predicciones de calificaciones de usuarios sobre ítems utilizando diferentes métricas de similitud y tipos de predicción.

## Características

### Métricas de Similitud
- **Correlación de Pearson**: Mide la correlación lineal entre dos usuarios
- **Distancia Coseno**: Calcula la similitud basada en el ángulo entre vectores
- **Distancia Euclídea**: Usa la distancia geométrica entre puntos (convertida a similitud)

### Tipos de Predicción
- **Predicción Simple**: Promedio ponderado de las calificaciones de los vecinos
- **Diferencia con la Media**: Considera la desviación respecto a la media de cada usuario

## Estructura del Proyecto

```
.
├── main.cc                    # Punto de entrada del programa
├── UtilityMatrix.h            # Definición de la clase UtilityMatrix
├── UtilityMatrix.cc           # Implementación de la clase UtilityMatrix
├── SimilarityCalculator.h     # Definición de la clase SimilarityCalculator
├── SimilarityCalculator.cc    # Implementación de métricas de similitud
├── RecommenderSystem.h        # Definición del sistema de recomendación
├── RecommenderSystem.cc       # Implementación del sistema de recomendación
├── Makefile                   # Archivo para compilar el proyecto
└── README_PROYECTO.md         # Este archivo
```

## Compilación

### Requisitos
- Compilador C++ con soporte para C++11 o superior (g++, clang++)
- Make

### Compilar el proyecto
```bash
make
```

### Limpiar archivos de compilación
```bash
make clean
```

### Recompilar desde cero
```bash
make rebuild
```

## Uso

### Sintaxis
```bash
./recommender [OPCIONES]
```

### Opciones

| Opción corta | Opción larga | Argumento | Descripción |
|--------------|--------------|-----------|-------------|
| `-f` | `--file` | `<archivo>` | Archivo con la matriz de utilidad (requerido) |
| `-m` | `--metric` | `<métrica>` | Métrica de similitud: `pearson`, `cosine`, `euclidean` |
| `-k` | `--neighbors` | `<número>` | Número de vecinos a considerar (por defecto: 3) |
| `-p` | `--prediction` | `<tipo>` | Tipo de predicción: `simple`, `mean` (por defecto: simple) |
| `-h` | `--help` | - | Mostrar ayuda |

### Ejemplos

#### Ejemplo 1: Correlación de Pearson con predicción simple
```bash
./recommender -f utility-matrix-5-10-1.txt -m pearson -k 3 -p simple
```

#### Ejemplo 2: Distancia Coseno con diferencia de media
```bash
./recommender -f utility-matrix-5-10-1.txt -m cosine -k 5 -p mean
```

#### Ejemplo 3: Distancia Euclídea con predicción simple
```bash
./recommender --file utility-matrix-25-100-1.txt --metric euclidean --neighbors 4 --prediction simple
```

## Formato del Archivo de Entrada

El archivo de entrada debe tener el siguiente formato:

```
<valor_mínimo>
<valor_máximo>
<calificación_u0_i0> <calificación_u0_i1> ... <calificación_u0_iN>
<calificación_u1_i0> <calificación_u1_i1> ... <calificación_u1_iN>
...
<calificación_uM_i0> <calificación_uM_i1> ... <calificación_uM_iN>
```

- Primera línea: valor mínimo de calificación
- Segunda línea: valor máximo de calificación
- Líneas siguientes: calificaciones de cada usuario para cada ítem
- Use `-` para indicar valores faltantes

### Ejemplo de archivo
```
0
5
3.142 2.648 1.649 - 1.116 0.883
3.412 0.314 3.796 4.233 2.159 4.513
4.408 4.495 2.052 - 0.051 -
```

## Salida del Programa

El programa proporciona la siguiente información:

1. **Configuración**: Métrica y tipo de predicción utilizados
2. **Matriz de Utilidad Original**: Matriz con las calificaciones originales
3. **Matriz de Similitudes**: Similitud entre cada par de usuarios
4. **Proceso de Predicción**: Detalle del cálculo de cada predicción:
   - Vecinos seleccionados
   - Valores de similitud
   - Fórmula y resultado del cálculo
5. **Matriz con Predicciones**: Matriz completa con valores predichos
6. **Recomendaciones**: Top 5 ítems recomendados para cada usuario

## Pruebas Incluidas en el Makefile

```bash
# Probar con Correlación de Pearson
make test-pearson

# Probar con Distancia Coseno
make test-cosine

# Probar con Distancia Euclídea
make test-euclidean

# Probar con predicción de diferencia con la media
make test-mean
```

## Detalles de Implementación

### Clase UtilityMatrix
- Almacena la matriz de utilidad
- Maneja valores faltantes (representados como -1)
- Calcula la media de calificaciones por usuario

### Clase SimilarityCalculator
- Implementa las tres métricas de similitud
- Trabaja solo con ítems calificados por ambos usuarios

### Clase RecommenderSystem
- Coordina todo el proceso de recomendación
- Calcula similitudes entre todos los usuarios
- Realiza predicciones para valores faltantes
- Genera recomendaciones ordenadas por calificación predicha

## Algoritmos

### Predicción Simple
```
predicción(u, i) = Σ(sim(u, v) × r_v,i) / Σ|sim(u, v)|
```
donde:
- `u` es el usuario
- `i` es el ítem
- `v` son los vecinos
- `sim(u, v)` es la similitud entre usuarios
- `r_v,i` es la calificación del vecino `v` para el ítem `i`

### Predicción con Diferencia de Media
```
predicción(u, i) = r̄_u + Σ(sim(u, v) × (r_v,i - r̄_v)) / Σ|sim(u, v)|
```
donde:
- `r̄_u` es la media de calificaciones del usuario `u`
- `r̄_v` es la media de calificaciones del vecino `v`

## Archivos de Ejemplo

El directorio incluye varios archivos de prueba:
- `utility-matrix-5-10-1.txt`: 5 usuarios, 10 ítems
- `utility-matrix-10-25-1.txt`: 10 usuarios, 25 ítems
- `utility-matrix-25-100-1.txt`: 25 usuarios, 100 ítems
- `utility-matrix-50-250-1.txt`: 50 usuarios, 250 ítems
- `utility-matrix-100-1000-1.txt`: 100 usuarios, 1000 ítems


