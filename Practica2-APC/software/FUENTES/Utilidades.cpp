/******************************************************************************
Práctica 2 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
Arthur Mickael Rodríguez Nesterenko 
DNI: Y1680851W
Profesor: Salvador García
Grupo de Computación y Sistemas Inteligentes
Fichero Utilidades.cpp que contiene la implementacion de las principales 
utilidades que nos servirán a lo largo de la práctica.
*****************************************************************************/
#include "Utilidades.h"

using namespace std;

//Inicializamos el generador normal y el objeto de distribucion Normal
//para posteriomente obtener un valor de esa distribucion (usado en todos
//menos en la ILS)
default_random_engine generadorNormal;
normal_distribution<double> distribucionNormal(0.0,0.3);

//Inicializamos el generador normal y el objeto de distribucion Normal
//para posteriomente obtener un valor de esa distribucion. Este generador
//normal se utilizará unicamente en la ILS, para generar una mutacion fuerte
default_random_engine generadorNormal_ILS;
normal_distribution<double> distribucionNormal_ILS(0.0,0.4);
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
vector<string>& clasificaciones){

	//Creamos un flujo de entrada para leer de un fichero
	ifstream ficheroDeDatos(nombreFichero); 
	string lineaDeFichero;

	//Leemos la introduccion al fichero @relation, nos saltamos el espacio
	//y leemos el primer atributo
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero);

	//A partir de aqui comenzamos a leer los atributos
	//y a contabilizar el numero de estos 
	int contadorDeCaracteristicas = -1;
	while(lineaDeFichero[0] == '@'){

		//cout<<"atributos: " <<lineaDeFichero <<endl;
		getline(ficheroDeDatos,lineaDeFichero);
		contadorDeCaracteristicas++;

	}

	//Tras leer el numero de atributos, pasamos a leer los valores de los mismos
	//Nos saltamos el espacio y comprobamos que el fichero no este vacío (tiene
	//valores para las n caracteristicas a considerar)
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero,',');

	//cout<<"Numero de atributos: " <<contadorDeCaracteristicas <<endl;

	float valorDeCaracteristica=0.0;

	//Mientras no lleguemos al final del fichero de datos
	while(!ficheroDeDatos.eof()){

		//Transformamos la cadena leída en un valor de tipo float
		//y lo insertamos en un vector auxiliar que corresponderá
		//al vector de caracteristicas i-esimo
		valorDeCaracteristica = stof(lineaDeFichero);
		vector<float> caracteristicas;
		caracteristicas.push_back(valorDeCaracteristica);

		//Para todas las características contabilizadas
		//realiza el mismo proceso
		for(int i =1;i<contadorDeCaracteristicas;i++){

			getline(ficheroDeDatos,lineaDeFichero,',');
			valorDeCaracteristica = stof(lineaDeFichero);
			caracteristicas.push_back(valorDeCaracteristica);
		}

		//Una vez tenemos todas los valores del vector característico
		//guadamos el vector como una fila de la matriz y repetimos el proceso
		datos.push_back(caracteristicas);
		getline(ficheroDeDatos,lineaDeFichero);
		//El ultimo valor leído será la etiqueta para ese vector de caracteristicas
		clasificaciones.push_back(lineaDeFichero);

		//Leemos otra vez el primer valor del vector de caracteristicas i+1
		getline(ficheroDeDatos,lineaDeFichero,',');

	}

	//Cerramos el fichero
	ficheroDeDatos.close();

}


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
vector<string>& clasificaciones){

	
	//Creamos un flujo de entrada para leer de un fichero
	ifstream ficheroDeDatos(nombreFichero); 
	string lineaDeFichero;

	//Leemos la introduccion al fichero @relation, nos saltamos el espacio
	//y leemos el primer atributo
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero);

	//A partir de aqui comenzamos a leer los atributos
	//y a contabilizar el numero de estos
	int contadorDeCaracteristicas = -1;
	while(lineaDeFichero[0] == '@'){

		//cout<<"atributos: " <<lineaDeFichero <<endl;
		getline(ficheroDeDatos,lineaDeFichero);
		contadorDeCaracteristicas++;

	}

	//Tras leer el numero de atributos, pasamos a leer los valores de los mismos
	//Nos saltamos el espacio y comprobamos que el fichero no este vacío (tiene
	//valores para las n caracteristicas a considerar)
	getline(ficheroDeDatos,lineaDeFichero);
	getline(ficheroDeDatos,lineaDeFichero,',');

	//cout<<"Numero de atributos: " <<contadorDeCaracteristicas <<endl;

	float valorDeCaracteristica=0.0;
	string valorDeCaracteristicaDeClase;

	//Mientras no lleguemos al final del fichero de datos
	while(!ficheroDeDatos.eof()){

		//Dado que el primer valor leido es la etiqueta del vector
		//de características, la introducimos en el vector correspondiente
		valorDeCaracteristicaDeClase = lineaDeFichero;
		clasificaciones.push_back(valorDeCaracteristicaDeClase);
		vector<float> caracteristicas;

		//Para todas las características contabilizadas (menos la ultima)
		//realiza el mismo proceso que la funcion anterior
		for(int i = 0;i<contadorDeCaracteristicas-1;i++){

			getline(ficheroDeDatos,lineaDeFichero,',');
			valorDeCaracteristica = stof(lineaDeFichero);
			caracteristicas.push_back(valorDeCaracteristica);
		}

		//Leemos el ultimo valor del vector de características y lo
		//guardamos en nuestro vector i-ésimo.
		getline(ficheroDeDatos,lineaDeFichero);
		valorDeCaracteristica = stof(lineaDeFichero);
		caracteristicas.push_back(valorDeCaracteristica);

		//Guardamos los datos y volvemos a leer la etiqueta del vector
		//i+1
		datos.push_back(caracteristicas);
		getline(ficheroDeDatos,lineaDeFichero,',');

	}

	//Cerramos el fichero
	ficheroDeDatos.close();


}


/*************************************************************************************/
//leerParametrosAlgoritmos(string nombreFichero, float &alpha, float &probabilidadCruce,
// float &probabilidadMutacion, float &PLS, int &semillaAleatoria, int &numeroDeCromosomasAGG,
//	int &numeroDeCromosomasAM)
// Funcion que lee los parámetros de los algoritmos desde fichero para posteriormente
//cargarlos en la clase (<numeroDeCromosomasAGG> y <numeroDeCromosomasAM>).
//Los demás se inicializarán el main para pasarselo como parámetro a los algoritmos pertinentes.
//NOTA: el fichero contiene en cada línea cada uno de los parámetros especificados en la
//cabecera de esta funcion EN EL MISMO ORDEN: alpha, probabilidadCruce, probabilidadMutacion,
// PLS(Probabilidad de Local Search para los Meméticos=0.1), semillaAleatoria, numeroDeCromosomasAGG,
//numeroDeCromosomasAM.
/*************************************************************************************/

void leerParametrosAlgoritmos(string nombreFichero, float &alpha,float &alpha1NN, float &probabilidadCruce,
float &probabilidadMutacion, float &PLS, int &semillaAleatoria, int &numeroDeCromosomasAGG,
int &numeroDeCromosomasAM){

	//Creamos un flujo de entrada para leer de un fichero
	ifstream ficheroDeDatos(nombreFichero); 
	string lineaDeFichero;

	//Leemos el parametros alpha del operador de cruce BLX=0.3 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	alpha = stof(lineaDeFichero);

	//Leemos el parametro alpha del 1NN alpha=0.5 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	alpha1NN = stof(lineaDeFichero);

	//Leemos el parametro probabilidad de cruce = 0,7 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	probabilidadCruce = stof(lineaDeFichero);

	//Leemos el parametro probabilidad de mutacion = 0,001 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	probabilidadMutacion = stof(lineaDeFichero);

	//Leemos el parametro P_LS para los AM= 0.1 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	PLS = stof(lineaDeFichero);

	//Leemos el parametro de semilla aleatoria=1680851 (Numeros de mi DNI)
	getline(ficheroDeDatos,lineaDeFichero);
	semillaAleatoria = stoi(lineaDeFichero);

	//Leemos el parametro de numero de cromosomas para los AGG=30 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	numeroDeCromosomasAGG = stoi(lineaDeFichero);

	//Leemos el parametro de numero de cromosomas para los AM=10 por defecto
	getline(ficheroDeDatos,lineaDeFichero);
	numeroDeCromosomasAM = stoi(lineaDeFichero);

	//Cerramos el fichero
	ficheroDeDatos.close();

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



/****************************************************************************************/
//void calcularDistanciaEuclideaRELIEF(vector<float> &vectorEPrima, vector<float> &vectorTest,
//	vector<float> &distanciaCompAComp, vector<float> &pesos, float distanciaEuclidea )
//
//Funciona igual que calcularDistanciaEuclidea solo que ahora guardamos las diferencias
// de las distancias caracteristica a caracteristica en un vector <distanciaCompAComp>
/****************************************************************************************/
void calcularDistanciaEuclideaRELIEF(vector<float> &vectorEPrima, vector<float> &vectorTest,
vector<float> &distanciaCompAComp, vector<float> &pesos, float &distanciaEuclidea){

	//Variable que contendrá la distancia Euclidea
	distanciaEuclidea=0.0;

	//Recorremos el vector y componente a componente 
	for(int i=0;i<vectorEPrima.size();i+=3){
		
		distanciaCompAComp.push_back((vectorTest[i]-vectorEPrima[i]));
		distanciaCompAComp.push_back((vectorTest[i+1]-vectorEPrima[i+1]));
		distanciaCompAComp.push_back((vectorTest[i+2]-vectorEPrima[i+2]));

		distanciaEuclidea += (pesos[i]*((vectorTest[i]-vectorEPrima[i])*(vectorTest[i]-vectorEPrima[i])));
		distanciaEuclidea += (pesos[i+1]*((vectorTest[i+1]-vectorEPrima[i+1])*(vectorTest[i+1]-vectorEPrima[i+1])));
		distanciaEuclidea += (pesos[i+2]*((vectorTest[i+2]-vectorEPrima[i+2])*(vectorTest[i+2]-vectorEPrima[i+2])));

	}

	distanciaEuclidea = sqrt(distanciaEuclidea);

}

/*************************************************************************************/
//			void vecinoPorMutacionNormal(vector<float> &pesos, int componenteAMutar,
//			bool ILS=false)
//
//Esta función genera un vecino de la solucion actual <pesos>, modificando la <componenteAMutar>
// a través de la generación de un número perteneciente a una <distribuciónNormal> 
//de media 0 y varianza 0.3 o 0.4 (en caso de que ILS==TRUE), 
//generado a través de un random engine <generadorNormal>.
/*************************************************************************************/
void vecinoPorMutacionNormal(vector<float> &pesos, const int &componenteAMutar, bool ILS){

	//Generamos un valor perteneciente a la distribucion normal
	float valorZ_i;
	(!ILS)? valorZ_i = distribucionNormal(generadorNormal): valorZ_i = distribucionNormal_ILS(generadorNormal_ILS);

	//Actualizamos los pesos y normalizamos la componente i-esima modificada
	pesos[componenteAMutar] += valorZ_i;

	if(pesos[componenteAMutar] >1)
		pesos[componenteAMutar]=1;

	else if (pesos[componenteAMutar] < 0)
		pesos[componenteAMutar]=0;

};

/*************************************************************************************/
//			void generarMutacionILS(vector<float> &pesos, int &componentesAMutar)
//
//Esta funcion aplica una mutacion a las <componentesAMutar> que se elijan de forma aleatoria
//sobre el conjunto inicial de <pesos> basandose en una distribucion N(0,0.4).
/*************************************************************************************/
void generarMutacionILS(vector<float> &pesos, int &componentesAMutar){

	//Generamos un vector de indices asociado al numero de componentes del vector de pesos
	vector<int> indices(pesos.size(),0);
	iota(indices.begin(),indices.end(),0);

	vector<int>::iterator it = indices.begin();
	int aleatorio,i;

	//Mutaremos un 10% de la cantidad de componentes del vector de pesos
	//elegidas de forma aleatoria
	for(i =0;i < componentesAMutar; i++){

		aleatorio = rand() % indices.size();
		//cout<<"Aleatorio -> " <<aleatorio <<endl;
		vecinoPorMutacionNormal(pesos,aleatorio, true);
		it = it + aleatorio;
		indices.erase(it);
		it = indices.begin();

	}

}




	



