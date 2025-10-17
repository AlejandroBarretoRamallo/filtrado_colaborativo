#include "RecommenderSystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

RecommenderSystem::RecommenderSystem(const string& filename, Metric m, int k, PredictionType pt) 
    : metric(m), numNeighbors(k), predictionType(pt) {
    
    if (!matrix.loadFromFile(filename)) {
        throw runtime_error("Error al cargar la matriz de utilidad");
    }
}

void RecommenderSystem::calculateAllSimilarities() {
    SimilarityCalculator calc(matrix, metric);
    similarities.resize(matrix.getNumUsers(), vector<double>(matrix.getNumUsers(), 0.0));
    
    for (int i = 0; i < matrix.getNumUsers(); i++) {
        for (int j = 0; j < matrix.getNumUsers(); j++) {
            if (i != j) {
                similarities[i][j] = calc.calculateSimilarity(i, j);
            }
        }
    }
}

vector<pair<int, double>> RecommenderSystem::getNeighbors(int user, int item, int k) const {
    vector<pair<int, double>> neighbors;
    
    for (int i = 0; i < matrix.getNumUsers(); i++) {
        if (i != user && !matrix.isMissing(i, item)) {
            neighbors.push_back({i, similarities[user][i]});
        }
    }
    
    // Ordenar por similitud descendente
    sort(neighbors.begin(), neighbors.end(), 
         [](const pair<int, double>& a, const pair<int, double>& b) {
             return a.second > b.second;
         });
    
    // Tomar solo k vecinos
    if (neighbors.size() > (size_t)k) {
        neighbors.resize(k);
    }
    
    return neighbors;
}

double RecommenderSystem::simplePrediction(int user, int item, const vector<pair<int, double>>& neighbors) const {
    if (neighbors.empty()) return matrix.getUserMean(user);
    
    double numerator = 0.0, denominator = 0.0;
    
    for (const auto& neighbor : neighbors) {
        int neighborId = neighbor.first;
        double similarity = neighbor.second;
        double rating = matrix.getRating(neighborId, item);
        
        numerator += similarity * rating;
        denominator += abs(similarity);
    }
    
    return denominator > 0.0 ? numerator / denominator : matrix.getUserMean(user);
}

double RecommenderSystem::meanDiffPrediction(int user, int item, const vector<pair<int, double>>& neighbors) const {
    if (neighbors.empty()) return matrix.getUserMean(user);
    
    double userMean = matrix.getUserMean(user);
    double numerator = 0.0, denominator = 0.0;
    
    for (const auto& neighbor : neighbors) {
        int neighborId = neighbor.first;
        double similarity = neighbor.second;
        double rating = matrix.getRating(neighborId, item);
        double neighborMean = matrix.getUserMean(neighborId);
        
        numerator += similarity * (rating - neighborMean);
        denominator += abs(similarity);
    }
    
    double prediction = userMean + (denominator > 0.0 ? numerator / denominator : 0.0);
    
    // Limitar predicción al rango válido
    prediction = max(matrix.getMinRating(), min(matrix.getMaxRating(), prediction));
    
    return prediction;
}

void RecommenderSystem::run() {
    cout << "\n========================================" << endl;
    cout << "SISTEMA DE RECOMENDACIÓN - FILTRADO COLABORATIVO" << endl;
    cout << "========================================" << endl;
    
    // Mostrar configuración
    cout << "\n=== CONFIGURACIÓN ===" << endl;
    cout << "Métrica: ";
    switch (metric) {
        case PEARSON: cout << "Correlación de Pearson" << endl; break;
        case COSINE: cout << "Distancia Coseno" << endl; break;
        case EUCLIDEAN: cout << "Distancia Euclídea" << endl; break;
    }
    cout << "Número de vecinos: " << numNeighbors << endl;
    cout << "Tipo de predicción: ";
    switch (predictionType) {
        case SIMPLE: cout << "Predicción Simple" << endl; break;
        case MEAN_DIFF: cout << "Diferencia con la Media" << endl; break;
    }
    
    // Mostrar matriz original
    matrix.print();
    
    // Calcular similitudes
    cout << "\n=== CALCULANDO SIMILITUDES ===" << endl;
    calculateAllSimilarities();
    
    // Mostrar matriz de similitudes
    printSimilarities();
    
    // Realizar predicciones
    cout << "\n=== REALIZANDO PREDICCIONES ===" << endl;
    makePredictions();
    
    // Mostrar matriz con predicciones
    cout << "\n=== MATRIZ DE UTILIDAD CON PREDICCIONES ===" << endl;
    matrix.printPredictions();
    
    // Generar recomendaciones
    cout << "\n=== RECOMENDACIONES POR USUARIO ===" << endl;
    generateRecommendations();
}

void RecommenderSystem::printSimilarities() const {
    cout << "\n=== MATRIZ DE SIMILITUDES ===" << endl;
    if (matrix.getNumItems() >= 25 || matrix.getNumUsers() > 25) {
      cout << "Matriz demasiado grande, imprimiendo solo resultados\n";
      return;
    }
    cout << setw(8) << "Usuario";
    for (int j = 0; j < matrix.getNumUsers(); j++) {
        cout << setw(10) << "U" + to_string(j);
    }
    cout << endl;
    
    for (int i = 0; i < matrix.getNumUsers(); i++) {
        cout << setw(8) << "U" + to_string(i);
        for (int j = 0; j < matrix.getNumUsers(); j++) {
            if (i == j) {
                cout << setw(10) << "1.000";
            } else {
                cout << setw(10) << fixed << setprecision(3) << similarities[i][j];
            }
        }
        cout << endl;
    }
    cout << endl;
}

void RecommenderSystem::makePredictions() {
    ofstream outFile("predictions.txt");
    if (!outFile.is_open()) {
        cerr << "Error: No se pudo abrir el archivo predictions.txt" << endl;
        return;
    }
    
    cout << "Escribiendo predicciones en predictions.txt..." << endl;
    
    for (int user = 0; user < matrix.getNumUsers(); user++) {
        outFile << "\n--- Usuario " << user << " ---" << endl;
        
        for (int item = 0; item < matrix.getNumItems(); item++) {
            if (matrix.isMissing(user, item)) {
                // Obtener vecinos
                auto neighbors = getNeighbors(user, item, numNeighbors);
                
                outFile << "\nPredicción para Item " << item << ":" << endl;
                outFile << "  Vecinos seleccionados (" << neighbors.size() << "):" << endl;
                
                for (const auto& neighbor : neighbors) {
                    outFile << "    Usuario " << neighbor.first 
                         << " (similitud: " << fixed << setprecision(3) << neighbor.second 
                         << ", rating: " << fixed << setprecision(3) << matrix.getRating(neighbor.first, item) << ")" << endl;
                }
                
                // Calcular predicción
                double prediction;
                if (predictionType == SIMPLE) {
                    prediction = simplePrediction(user, item, neighbors);
                } else {
                    prediction = meanDiffPrediction(user, item, neighbors);
                }
                
                outFile << "  Cálculo: ";
                if (predictionType == SIMPLE) {
                    double numerator = 0.0, denominator = 0.0;
                    for (const auto& neighbor : neighbors) {
                        numerator += neighbor.second * matrix.getRating(neighbor.first, item);
                        denominator += abs(neighbor.second);
                    }
                    outFile << "(" << fixed << setprecision(3) << numerator << ") / (" 
                         << fixed << setprecision(3) << denominator << ") = ";
                } else {
                    double userMean = matrix.getUserMean(user);
                    outFile << "Media usuario (" << fixed << setprecision(3) << userMean << ") + ajuste = ";
                }
                
                outFile << fixed << setprecision(3) << prediction << endl;
                
                // Guardar predicción
                matrix.setRating(user, item, prediction);
            }
        }
    }
    
    outFile.close();
    cout << "Predicciones completadas y guardadas en predictions.txt" << endl;
}

void RecommenderSystem::generateRecommendations() {
    for (int user = 0; user < matrix.getNumUsers(); user++) {
        vector<pair<int, double>> recommendations;
        
        for (int item = 0; item < matrix.getNumItems(); item++) {
            recommendations.push_back({item, matrix.getRating(user, item)});
        }
        
        // Ordenar por rating descendente
        sort(recommendations.begin(), recommendations.end(),
             [](const pair<int, double>& a, const pair<int, double>& b) {
                 return a.second > b.second;
             });
        
        cout << "\nUsuario " << user << " (Top 5 items recomendados):" << endl;
        for (size_t i = 0; i < min((size_t)5, recommendations.size()); i++) {
            cout << "  " << (i + 1) << ". Item " << recommendations[i].first 
                 << " (rating: " << fixed << setprecision(3) << recommendations[i].second << ")" << endl;
        }
    }
}
