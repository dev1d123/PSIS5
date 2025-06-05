#include <bits/stdc++.h>
#include <cmath>
#include <iomanip>

using namespace std;

// Estructura para almacenar coordenadas astronómicas
struct Coordenadas {
    double grados;
    string hms;  // Para ascensión recta (horas, minutos, segundos)
    string dms;  // Para declinación (grados, minutos, segundos)
};

// Estructura para almacenar los resultados de los cálculos
struct ResultadosCalculos {
    double jd;          // Día Juliano
    double T;           // Siglos julianos
    Coordenadas L0;     // Longitud media del Sol
    Coordenadas M;      // Anomalía media
    Coordenadas C;      // Ecuación del centro
    Coordenadas Theta;  // Longitud verdadera
    double epsilon;     // Oblicuidad de la eclíptica
    Coordenadas alpha;  // Ascensión recta
    Coordenadas delta;  // Declinación
    string signo;       // Signo zodiacal
};

// Función para convertir grados a HMS (Horas, Minutos, Segundos)
string gradosToHMS(double grados) {
    grados = fmod(grados, 360);
    if (grados < 0) grados += 360;
    
    double horas = grados / 15.0;
    int h = floor(horas);
    double minutos = (horas - h) * 60;
    int m = floor(minutos);
    double segundos = (minutos - m) * 60;
    int s = round(segundos);
    
    if (s == 60) {
        s = 0;
        m += 1;
    }
    if (m == 60) {
        m = 0;
        h += 1;
    }
    
    stringstream ss;
    ss << h << "h " << m << "m " << s << "s";
    return ss.str();
}

// Función para convertir grados a DMS (Grados, Minutos, Segundos)
string gradosToDMS(double grados) {
    grados = fmod(grados, 360);
    bool negativo = grados < 0;
    grados = abs(grados);
    
    int d = floor(grados);
    double minutos = (grados - d) * 60;
    int m = floor(minutos);
    double segundos = (minutos - m) * 60;
    int s = round(segundos);
    
    if (s == 60) {
        s = 0;
        m += 1;
    }
    if (m == 60) {
        m = 0;
        d += 1;
    }
    
    stringstream ss;
    ss << (negativo ? "-" : "") << d << "° " << m << "' " << s << "\"";
    return ss.str();
}

// Función para calcular el Día Juliano
double calcularJulianDay(int year, int month, int day) {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    int A = year / 100;
    int B = 2 - A + (A / 4);
    double jd = floor(365.25 * (year + 4716)) + floor(30.6001 * (month + 1)) + day + B - 1524.5;
    return jd;
}

// Función para determinar el signo zodiacal
string determinarSignoZodiacal(double lambda) {
    const pair<double, double> signos[] = {
        {0.0, 30.0},    // Aries
        {30.0, 60.0},    // Tauro
        {60.0, 90.0},    // Géminis
        {90.0, 120.0},   // Cáncer
        {120.0, 150.0},  // Leo
        {150.0, 180.0},  // Virgo
        {180.0, 210.0},  // Libra
        {210.0, 240.0},  // Escorpio
        {240.0, 270.0},  // Sagitario
        {270.0, 300.0},  // Capricornio
        {300.0, 330.0},  // Acuario
        {330.0, 360.0}   // Piscis
    };
    
    const string nombres[] = {
        "Aries", "Tauro", "Géminis", "Cáncer", "Leo", "Virgo",
        "Libra", "Escorpio", "Sagitario", "Capricornio", "Acuario", "Piscis"
    };
    
    lambda = fmod(lambda, 360);
    if (lambda < 0) lambda += 360;
    
    for (int i = 0; i < 12; ++i) {
        if (lambda >= signos[i].first && lambda < signos[i].second) {
            return nombres[i];
        }
    }
    return "Desconocido";
}

// Función principal para realizar todos los cálculos
ResultadosCalculos calcularPosicionSolar(int year, int month, int day) {
    ResultadosCalculos resultados;
    
    // 1. Calcular Día Juliano
    resultados.jd = calcularJulianDay(year, month, day);
    
    // 2. Calcular T (siglos julianos desde J2000.0)
    resultados.T = (resultados.jd - 2451545.0) / 36525.0;
    
    // 3. Calcular longitud media del Sol (L0)
    resultados.L0.grados = fmod(280.46646 + 36000.76983 * resultados.T + 0.0003032 * resultados.T * resultados.T, 360.0);
    if (resultados.L0.grados < 0) resultados.L0.grados += 360.0;
    resultados.L0.dms = gradosToDMS(resultados.L0.grados);
    
    // 4. Calcular anomalía media (M)
    resultados.M.grados = fmod(357.52911 + 35999.05029 * resultados.T - 0.0001537 * resultados.T * resultados.T, 360.0);
    if (resultados.M.grados < 0) resultados.M.grados += 360.0;
    resultados.M.dms = gradosToDMS(resultados.M.grados);
    
    // 5. Calcular ecuación del centro (C)
    resultados.C.grados = (1.914602 - 0.004817 * resultados.T - 0.000014 * resultados.T * resultados.T) * sin(resultados.M.grados * M_PI / 180.0)
                         + (0.019993 - 0.000101 * resultados.T) * sin(2 * resultados.M.grados * M_PI / 180.0)
                         + 0.000289 * sin(3 * resultados.M.grados * M_PI / 180.0);
    resultados.C.dms = gradosToDMS(resultados.C.grados);
    
    // 6. Calcular longitud verdadera (Theta)
    resultados.Theta.grados = resultados.L0.grados + resultados.C.grados;
    resultados.Theta.grados = fmod(resultados.Theta.grados, 360.0);
    if (resultados.Theta.grados < 0) resultados.Theta.grados += 360.0;
    resultados.Theta.dms = gradosToDMS(resultados.Theta.grados);
    
    // 7. Calcular oblicuidad de la eclíptica (epsilon)
    resultados.epsilon = 23.43929111 - 0.013004167 * resultados.T - 0.0000001639 * resultados.T * resultados.T + 0.0000005036 * resultados.T * resultados.T * resultados.T;
    
    // 8. Convertir a coordenadas ecuatoriales (alpha, delta)
    double lambda = resultados.Theta.grados;
    resultados.alpha.grados = atan2(cos(resultados.epsilon * M_PI / 180.0) * sin(lambda * M_PI / 180.0), cos(lambda * M_PI / 180.0)) * 180.0 / M_PI;
    resultados.alpha.grados = fmod(resultados.alpha.grados, 360.0);
    if (resultados.alpha.grados < 0) resultados.alpha.grados += 360.0;
    resultados.alpha.hms = gradosToHMS(resultados.alpha.grados);
    
    resultados.delta.grados = asin(sin(resultados.epsilon * M_PI / 180.0) * sin(lambda * M_PI / 180.0)) * 180.0 / M_PI;
    resultados.delta.dms = gradosToDMS(resultados.delta.grados);
    
    // 9. Determinar signo zodiacal
    resultados.signo = determinarSignoZodiacal(lambda);
    
    return resultados;
}

int main() {
    int year, month, day;
    
    cout << "Calculadora de posición solar y signo zodiacal" << endl;
    cout << "Ingresa tu fecha de nacimiento (YYYY MM DD): ";
    cin >> year >> month >> day;
    
    ResultadosCalculos resultados = calcularPosicionSolar(year, month, day);
    
    cout << fixed << setprecision(6);
    cout << "\n--- Resultados de los cálculos ---" << endl;
    cout << "1. Día Juliano (JD): " << resultados.jd << endl;
    cout << "2. T (siglos julianos): " << resultados.T << endl;
    cout << "3. Longitud media del Sol (L0): " << resultados.L0.grados << "° (" << resultados.L0.dms << ")" << endl;
    cout << "4. Anomalía media (M): " << resultados.M.grados << "° (" << resultados.M.dms << ")" << endl;
    cout << "5. Ecuación del centro (C): " << resultados.C.grados << "° (" << resultados.C.dms << ")" << endl;
    cout << "6. Longitud verdadera (Theta): " << resultados.Theta.grados << "° (" << resultados.Theta.dms << ")" << endl;
    cout << "7. Oblicuidad de la eclíptica (epsilon): " << resultados.epsilon << "° (" << gradosToDMS(resultados.epsilon) << ")" << endl;
    cout << "8. Ascensión recta (alpha): " << resultados.alpha.grados << "° (" << resultados.alpha.hms << ")" << endl;
    cout << "9. Declinación (delta): " << resultados.delta.grados << "° (" << resultados.delta.dms << ")" << endl;
    
    cout << "\n--- Resultado final ---" << endl;
    cout << "El Sol estaba en " << resultados.Theta.dms << " el " << day << "/" << month << "/" << year << endl;
    cout << "Tu signo zodiacal es: " << resultados.signo << endl;
    
    return 0;
}
