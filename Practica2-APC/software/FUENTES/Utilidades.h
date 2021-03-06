
/******************************************************************************
Práctica 2 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
Arthur Mickael Rodríguez Nesterenko 
DNI: Y1680851W
Profesor: Salvador García
Grupo de Computación y Sistemas Inteligentes
Fichero Utilidades.h que contiene las cabeceras de las utilidades generales
para la practica
*****************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <set>
#include "random.h"

using namespace std;

/*************************************************************************************/
//leerDatosFicheroARFF_Formato1(string nombreFichero, vector<vector<float> > & datos,
//							vector<string>& clasificaciones)
//
//Me permite leer los datos de un fichero <nombreFichero> de tipo ARFF y me guarda
//en una matriz <datos> n-dimensional los vectores de n caracteristicas y en un vector
//de cadenas de caracteres <clasificaciones> guardamos la etiqueta o clasificacion 
//correspondiente a cada uno de los vectores de caracteristicas. 
//Esta funcion lee los ficheros: sonar.arff y spambase-460.arff
/*************************************************************************************/
void leerDatosFicheroARFF_Formato1(string nombreFichero, vector<vector<float> > & datos,
vector<string>& clasificaciones);



/*************************************************************************************/
//leerDatosFicheroARFF_Formato2(string nombreFichero, vector<vector<float> > & datos,
//							vector<string>& clasificaciones)
//
//Realiza las mismas operaciones sobre el fichero ARFF <nombreFichero> que la funcion
//anterior, guardando los vectores de características en <datos> y sus clasificaciones
//en el vector de cadenas <clasificaciones>. La única diferencia es que ésta función
//se utiliza para leer el fichero wdbc.arff ya que tiene un formato distinto del de 
//los otros ficheros: las clasificaciones de cada vector característico están al inicio.
/*************************************************************************************/
void leerDatosFicheroARFF_Formato2(string nombreFichero, vector<vector<float> > & datos,
vector<string>& clasificaciones);


/*************************************************************************************/
//leerParametrosAlgoritmos(string nombreFichero, float &alpha, float &probabilidadCruce,
// float &probabilidadMutacion, float &PLS, int &semillaAleatoria, int &numeroDeCromosomasAGG,
//	int &numeroDeCromosomasAM)
// Funcion que lee los parámetros de los algoritmos desde fichero para posteriormente
//cargarlos en la clase (<numeroDeCromosomasAGG> y <numeroDeCromosomasAM>).
//Los demás se inicializarán el main para pasarselo como parámetro a los algoritmos pertinentes.
/*************************************************************************************/
void leerParametrosAlgoritmos(string nombreFichero, float &alpha,float &alpha1NN, float &probabilidadCruce,
float &probabilidadMutacion, float &PLS, int &semillaAleatoria, int &numeroDeCromosomasAGG,
int &numeroDeCromosomasAM);


/*************************************************************************************/
//void calcularTasaDeClasificacion(int &aciertosEnClasificacion, int instanciasDeT, 
//							float &tasaClasificacion
//
//Funcion auxiliar que me devuelve la <tasaClasificacion> en tanto por ciento:
// siguiendo la formula tasa_clas= 100*(aciertosEnClasificacion/instanciasDeT)
/*************************************************************************************/
inline void calcularTasaDeClasificacion(int &aciertosEnClasificacion, const float &instanciasDeT, 
	float &tasaClasificacion){

	tasaClasificacion = aciertosEnClasificacion*instanciasDeT;
}

/*************************************************************************************/
//void calcularTasaDeReduccion(vector<float> &pesos, float &tasaReduccion)
//
//Funcion auxiliar que me devuelve la <tasaDeReduccion> en tanto por ciento:
// siguiendo la formula tasa_reduc= 100*(numero de Wi < 0.1/numeroCaracteristicas)
/*************************************************************************************/
inline void calcularTasaDeReduccion(const vector<float> &pesos, float & tasaReduccion){

	int cantidadReducciones=0;
	for(int i = 0; i < pesos.size();i++)
		if(pesos[i] < 0.1)
			cantidadReducciones++;

	tasaReduccion = cantidadReducciones*1.0/pesos.size();

}


/*************************************************************************************/
//void calcularDistanciaEuclidea(vector<float> &vectorEPrima, vector<float> &vectorTest,
//	 vector<float> &pesos, float &distanciaEuclidea )
//
//Calcula la distancia Euclidea, componente a componente de los vectores <vectorEPrima>
//y <vectorTest>, esta es, la raiz cuadrada de la suma de las diferencias al cuadrado
//de cada una de las componentes de ambos vectores multiplicado por los <pesos>.
// Devuelve la <distanciaEuclidea> entre los dos vectores característicos.
/*************************************************************************************/
inline void calcularDistanciaEuclidea(const vector<float> &vectorEPrima, const vector<float> &vectorTest,
 const vector<float> &pesos, float &distanciaEuclidea){

	//Variable que contendrá la distancia Euclidea
	distanciaEuclidea=0.0;
	int i=0;

	//Recorremos el vector y componente a componente 
	for(;i<pesos.size();i+=3){

		if(pesos[i]>=0.1)
			distanciaEuclidea += (pesos[i]*((vectorTest[i]-vectorEPrima[i])*(vectorTest[i]-vectorEPrima[i])));

		if(pesos[i+1]>=0.1)
			distanciaEuclidea += (pesos[i+1]*((vectorTest[i+1]-vectorEPrima[i+1])*(vectorTest[i+1]-vectorEPrima[i+1])));
		
		if(pesos[i+2]>=0.1)
			distanciaEuclidea += (pesos[i+2]*((vectorTest[i+2]-vectorEPrima[i+2])*(vectorTest[i+2]-vectorEPrima[i+2])));
		
	}
		
	distanciaEuclidea = sqrt(distanciaEuclidea);

}



/****************************************************************************************/
//void calcularDistanciaEuclideaRELIEF(vector<float> &vectorEPrima, vector<float> &vectorTest,
//	vector<float> &distanciaCompAComp, vector<float> &pesos, float distanciaEuclidea )
//
//Funciona igual que calcularDistanciaEuclidea solo que ahora guardamos las diferencias
// de las distancias caracteristica a caracteristica en un vector <distanciaCompAComp>
/****************************************************************************************/
void calcularDistanciaEuclideaRELIEF(vector<float> &vectorEPrima, vector<float> &vectorTest,
vector<float> &distanciaCompAComp, vector<float> &pesos, float &distanciaEuclidea);

/*************************************************************************************/
//	inline void generarSolucionAleatoria(vector<float> &solucion, int numeroCaracteristicas)
//
//La funcion genera una <solucion> aleatoria (vector de pesos) con un numero de componenetes
//igual al <numeroCaracteristicas> del vector caracteristico a considerar.
/*************************************************************************************/
inline void generarSolucionAleatoria(vector<float> &solucion, int numeroCaracteristicas){

	solucion.clear();
	float pesoAleatorio;
	for(int i= 0; i< numeroCaracteristicas;i++){

		pesoAleatorio = Rand();
		solucion.push_back(pesoAleatorio);
	}

}


/******************************************************************************************/
//void generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int &numeroDeCromosomas,
//								int &numeroDeGenes)
//
//Funcion que genera una <poblacionInicial> de forma aleatoria partiendo de un <numeroDeCromosomas>
//y <numeroDeGenes> por cromosoma.
/******************************************************************************************/
inline void generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int numeroDeCromosomas,
 int numeroDeGenes){


	poblacionInicial.clear();
	vector<float> solucionAleatoria(numeroDeGenes,0);

	//Generamos la poblacion aleatoria con <numeroDeCromosomas>
	//y cada una con un <numeroDeGenes>
	for(int i=0; i < numeroDeCromosomas; i++){

		generarSolucionAleatoria(solucionAleatoria,numeroDeGenes);
		poblacionInicial.push_back(solucionAleatoria);

	}

};


/*************************************************************************************/
//			void vecinoPorMutacionNormal(vector<float> &pesos, int componenteAMutar,
//			bool ILS=false)
//
//Esta función genera un vecino de la solucion actual <pesos>, modificando la <componenteAMutar>
// a través de la generación de un número perteneciente a una <distribuciónNormal> 
//de media 0 y varianza 0.3 o 0.4 (en caso de que ILS==TRUE), 
//generado a través de un random engine <generadorNormal>.
/*************************************************************************************/
void vecinoPorMutacionNormal(vector<float> &pesos, const int &componenteAMutar, bool ILS = false);

/*************************************************************************************/
//			void generarMutacionILS(vector<float> &pesos, int &componentesAMutar)
//
//Esta funcion aplica una mutacion a las <componentesAMutar> que se elijan de forma aleatoria
//sobre el conjunto inicial de <pesos> basandose en una distribucion N(0,0.4).
/*************************************************************************************/
void generarMutacionILS(vector<float> &pesos, int &componentesAMutar);





