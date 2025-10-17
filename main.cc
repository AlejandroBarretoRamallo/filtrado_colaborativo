#include <iostream>
#include <getopt.h>
#include "RecommenderSystem.h"

using namespace std;

void printUsage(const char* programName) {
    cout << "Uso: " << programName << " [OPCIONES]" << endl;
    cout << "\nOpciones:" << endl;
    cout << "  -f, --file <archivo>       Archivo con la matriz de utilidad (requerido)" << endl;
    cout << "  -m, --metric <métrica>     Métrica de similitud:" << endl;
    cout << "                               pearson  - Correlación de Pearson" << endl;
    cout << "                               cosine   - Distancia Coseno" << endl;
    cout << "                               euclidean - Distancia Euclídea" << endl;
    cout << "  -k, --neighbors <número>   Número de vecinos a considerar" << endl;
    cout << "  -p, --prediction <tipo>    Tipo de predicción:" << endl;
    cout << "                               simple   - Predicción Simple" << endl;
    cout << "                               mean     - Diferencia con la Media" << endl;
    cout << "  -h, --help                 Mostrar esta ayuda" << endl;
    cout << "\nEjemplo:" << endl;
    cout << "  " << programName << " -f utility-matrix-5-10-1.txt -m pearson -k 3 -p simple" << endl;
}

int main(int argc, char *argv[]) {
    string filename;
    Metric metric = PEARSON;
    int numNeighbors = 3;
    PredictionType predictionType = SIMPLE;
    
    // Opciones largas
    static struct option long_options[] = {
        {"file",       required_argument, 0, 'f'},
        {"metric",     required_argument, 0, 'm'},
        {"neighbors",  required_argument, 0, 'k'},
        {"prediction", required_argument, 0, 'p'},
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "f:m:k:p:h", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
                
            case 'm':
                {
                    string metricStr = optarg;
                    if (metricStr == "pearson") {
                        metric = PEARSON;
                    } else if (metricStr == "cosine") {
                        metric = COSINE;
                    } else if (metricStr == "euclidean") {
                        metric = EUCLIDEAN;
                    } else {
                        cerr << "Error: Métrica no válida. Use: pearson, cosine o euclidean" << endl;
                        return 1;
                    }
                }
                break;
                
            case 'k':
                numNeighbors = atoi(optarg);
                if (numNeighbors <= 0) {
                    cerr << "Error: El número de vecinos debe ser mayor que 0" << endl;
                    return 1;
                }
                break;
                
            case 'p':
                {
                    string predStr = optarg;
                    if (predStr == "simple") {
                        predictionType = SIMPLE;
                    } else if (predStr == "mean") {
                        predictionType = MEAN_DIFF;
                    } else {
                        cerr << "Error: Tipo de predicción no válido. Use: simple o mean" << endl;
                        return 1;
                    }
                }
                break;
                
            case 'h':
                printUsage(argv[0]);
                return 0;
                
            default:
                printUsage(argv[0]);
                return 1;
        }
    }
    
    // Verificar que se proporcionó el archivo
    if (filename.empty()) {
        cerr << "Error: Debe especificar un archivo con -f o --file" << endl;
        printUsage(argv[0]);
        return 1;
    }
    
    try {
        RecommenderSystem system(filename, metric, numNeighbors, predictionType);
        system.run();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
