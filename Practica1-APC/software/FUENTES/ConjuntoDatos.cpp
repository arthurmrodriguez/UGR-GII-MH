/******************************************************************************
Práctica 1 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
Arthur Mickael Rodríguez Nesterenko 
DNI: Y1680851W
Profesor: Salvador García
Grupo de Computación y Sistemas Inteligentes
Fichero ConjuntoDatos.cpp: contiene la implementacion de la clase ConjuntoDatos
con todas sus funcionalidades
*****************************************************************************/

//Inclusión del fichero de cabeceras asociado


#include "ConjuntoDatos.h"

using namespace std;
using namespace std::chrono;



/*************************************************************************************/
//	ConjuntoDatos(vector<vector<float> > &datos, vector<string> &etiquetas,
//	int numeroDeCromosomasAGG,int numeroDeCromosomasAM)
//
//Constructor de la clase ConjuntoDatos a partir de un conjunto de <datos> y de <etiquetas>
//leidos de un fichero. Los datos son normalizados antes de ser introducidos en los
//atributos de la clase correspondiente. Tambien se lee desde fichero el numero de cromosomas
//para cada algoritmo: genético y memético 
/*************************************************************************************/
ConjuntoDatos::ConjuntoDatos(vector<vector<float> > &datos, vector<string> &etiquetas,
	int &numeroDeCromosomasAGG,int &numeroDeCromosomasAM){

	normalizarDatos(datos);
	this->datos = datos;
	this-> etiquetas = etiquetas;
	this->numeroDeCromosomasAGG = numeroDeCromosomasAGG;
	this->numeroDeCromosomasAM = numeroDeCromosomasAM;
	this->numeroDeGenes= datos[0].size();
	limiteGenAleatorio = 1 / RAND_MAX;

}



/*************************************************************************************/
//			~ConjuntoDatos()
//
//Destructor de la clase ConjuntoDatos
/*************************************************************************************/
ConjuntoDatos::~ConjuntoDatos(){

	
	this->datos.clear();
	this-> etiquetas.clear();

}

/*************************************************************************************/
//					normalizarDatos(vector<vector<float> > &datos)
//
//Realiza las normalizaciones del conjunto de datos contenidos en <datos> al intervalo
// [0,1] siguiendo la formula que se nos proporciona en las transparencias de clase:
// Xj = (Xj - MINj)/(MAXj - MINj)
/*************************************************************************************/
void ConjuntoDatos::normalizarDatos(vector<vector<float> > &datos){

	//Obtenemos las columnas para calcular sus minimos y maximos
	vector<vector<float> > columnas;
	vector<float> caracteristicas;

	//Obtenemos una matriz en la que cada fila representará
	//una columna, esto es, todos los valores posibles para la característica
	//i-ésima
	for(int i=0; i< datos[i].size();i++){

		caracteristicas.clear();

		for(int j=0;j<datos.size();j++){
			
			caracteristicas.push_back(datos[j][i]);
		}

		columnas.push_back(caracteristicas);
	}

	//Calculamos el minimo y el maximo de cada fila,
	//esto es, de cada característica
	vector<vector<float> > minYMaxColumnas;

	for(int i=0;i<columnas.size();i++){

		float minimo = *min_element(columnas[i].begin(),columnas[i].end());
		float maximo = *max_element(columnas[i].begin(),columnas[i].end());

		vector<float> minMax;
		minMax.push_back(minimo);
		minMax.push_back(maximo);

		minYMaxColumnas.push_back(minMax);

	}

	//Normalizamos cada uno de los datos
	for(int i = 0;i <datos.size(); i++){

		for(int j=0;j< datos[i].size();j++)
			datos[i][j]= (datos[i][j] - minYMaxColumnas[j][0])/(minYMaxColumnas[j][1]- minYMaxColumnas[j][0]);
	}


}




/**********************************************************************************************/
//		void calcularAmigoMasCercano(vector<int> particionTrain,vector<float> &distanciasAmigo,
//									int indiceEPrima)
//
//Funcion que encuentra en un conjunto de datos <particionTrain> el amigo más cercano
// (aquel de su misma clase y que es distinto de el mismo) de un determinado vector de
//características indicado por <indiceEPrima> (normalmente el conjunto de train).
// Devuelve las distancias al amigo mas cercano en <distanciasAmigo>
/**********************************************************************************************/
void ConjuntoDatos::calcularAmigoMasCercano(const vector<int> &particionTrain,vector<float> &distanciasAmigo,
	const int &indiceEPrima){

	vector<float> pesosUnitarios(particionTrain.size(),1.0), distanciasUAUAux;
	distanciasUAUAux.reserve(particionTrain.size());
	float distanciaMin=100, distanciaActual=0.0;
	string claseEprima = etiquetas[indiceEPrima];

	
	for(auto indiceTrain= particionTrain.begin();indiceTrain != particionTrain.end();indiceTrain++){

		distanciasUAUAux.clear();

		if(*indiceTrain != indiceEPrima){
			//Recorremos cada vector de características del conjunto de test y
			//Calculamos la distancia al ejemplo EPrima
			calcularDistanciaEuclideaRELIEF(datos[indiceEPrima],
					datos[*indiceTrain], distanciasUAUAux, pesosUnitarios,distanciaActual);

			//Solo si la distancia es menor que la minima y además son de la misma clase
			//entonces se selecciona como posible amigo mas cercano
			if(distanciaActual < distanciaMin){

				if(claseEprima == etiquetas[*indiceTrain]){

					distanciaMin=distanciaActual;
					distanciasAmigo=distanciasUAUAux;

				}
						
			}

		}

		
	}

}



/*************************************************************************************/
//	void calcularEnemigoMasCercano(vector<int> particionTrain,vector<float> &distanciasEnemigo,
//									int indiceEPrima)
//
//Funcion que encuentra en un conjunto de datos <particionTrain> el enemigo más cercano
// (aquel de clase distinta) de un determinado vector de
//características indicado por <indiceEPrima> (normalmente el conjunto de train).
// Devuelve las distancias al amigo mas cercano en <distanciasAmigo>
/*************************************************************************************/
void ConjuntoDatos::calcularEnemigoMasCercano(const vector<int> &particionTrain,vector<float> &distanciasEnemigo,
	const int &indiceEPrima){

	vector<float> pesosUnitarios(particionTrain.size(),1.0), distanciasUAUAux;
	distanciasUAUAux.reserve(particionTrain.size());
	float distanciaMin=100, distanciaActual=0.0;
	string claseEprima = etiquetas[indiceEPrima];

	for(auto indiceTrain= particionTrain.begin();indiceTrain != particionTrain.end();indiceTrain++){

		distanciasUAUAux.clear();

		if(*indiceTrain != indiceEPrima){
			//Recorremos cada vector de características del conjunto de test y
			//Calculamos la distancia al ejemplo EPrima
			calcularDistanciaEuclideaRELIEF(datos[indiceEPrima],
			datos[*indiceTrain], distanciasUAUAux, pesosUnitarios, distanciaActual);
		
			//Solo si la distancia es menor que la minima y además son de la misma clase
			//entonces se selecciona como posible amigo mas cercano
			if(distanciaActual < distanciaMin){
		
				if(claseEprima != etiquetas[*indiceTrain]){
		
					distanciaMin=distanciaActual;
					distanciasEnemigo=distanciasUAUAux;
		
				}
							
			}
		}

	}
}

/*****************************************************************************************************/
//		void Clasificador1_NN(vector<int> particionTrain, vector<int> particionTest, 
//						vector<float> &pesos, float &tasaClasificacion)
//
//Clasificador de tipo K_NN con K=1 vecino mas cercano. Este algoritmo calcula a partir
//de una <particionTrain> y una <particionTest> del conjunto de datos (1,...,5) el porcentaje o la tasa de 
//clasificación siguiendo las directrices del algoritmo que aparece en las transparencias
//del seminario. Utiliza un vector de <pesos> para que la función pueda ser utilizada 
//por los distintos algoritmos posteriores. Haremos uso de una función auxiliar 
//calcularDistanciaEuclidea para agilizar el proceso.
//NOTA: Se utiliza el procedimiento <leave one out> en los casos que se requiera (LS, AGG, AGE,etc).
//A través de una condicion que no permite calculos de distancias para indices iguales
/*****************************************************************************************************/
void ConjuntoDatos::Clasificador1_NN(const vector<int> &particionTrain, const vector<int> &particionTest, 
	const vector<float> &pesos, float &tasaClasificacion){

	//Variables auxiliares
	int aciertosEnClasificacion=0;
	float instanciasDeT=(1.0/particionTrain.size()), distanciaActual=0.0, distanciaMin;
	string claseEPrima = etiquetas[0];

	//Para cada vector característico del conjunto de train
	for(auto indice1=particionTrain.begin(); indice1 != particionTrain.end();indice1++){

		distanciaMin = 100;
		//Recorremos todos los vectores del conjunto de test e intentamos clasificarlo
		for(auto indice2=particionTest.begin();indice2 != particionTest.end();indice2++){
			//Leave one out: cuando sea distinto el indice
			//Esto no afecta las particiones distintas, ya que siempre cumplirán la condicion.
			//En cambio cuando train==test, la condicion no permite entrar para las componentes iguales.
			if((*indice1) != (*indice2) ){
				//Calculamos la distancia euclídea
				calcularDistanciaEuclidea(datos[*indice1],datos[*indice2],pesos,distanciaActual);
				//Recorremos el vector y componente a componente 
				//Si la distancia actual es menor que la minima, actualizamos la distancia minima
				//y la clase
				if(distanciaActual < distanciaMin){

					distanciaMin=distanciaActual;
					claseEPrima = etiquetas[*indice2];
				}
			}
		}
		//Si al final hemos clasificado correctamente, contabilizamos un acierto de clasificacion
		(claseEPrima == etiquetas[*indice1])? aciertosEnClasificacion++ : aciertosEnClasificacion=aciertosEnClasificacion;		
	}

	calcularTasaDeClasificacion(aciertosEnClasificacion,instanciasDeT, tasaClasificacion);
}

/*****************************************************************************************************/
//		void Greedy_RELIEF(const vector<int> particionTrain, const vector<int> particionTest, 
//								vector<float> &pesosRELIEF)
//
//Algoritmo Greedy que a partir de una <particionTrain> y frente a una <particionTest> 
// calcula un vector de <pesosRELIEF> con el que posteriormente calcularemos el porcentaje de clasificacion 
//según el clasificador 1_NN. El metodo considera que leave one out es true, dado que evalua el la particion
//consigo misma.
/*****************************************************************************************************/
void ConjuntoDatos::Greedy_RELIEF(const vector<int> &particionTrain, vector<float> &pesosRELIEF){

	//Variables auxiliares
	vector<float> distanciasAmigo, distanciasEnemigo;

	//Recorremos cada vector característico y calculamos, para cada uno
	//el enemigo y el amigo mas cercano.
	//Posteriormente actualizamos el vector de pesos teniendo en cuenta las distancias
	for(auto indice1=particionTrain.begin(); indice1 != particionTrain.end();indice1++){

		distanciasAmigo.clear();
		distanciasEnemigo.clear();
		calcularEnemigoMasCercano(particionTrain,distanciasEnemigo,(*indice1));
		calcularAmigoMasCercano(particionTrain,distanciasAmigo,(*indice1));

		for(int i=0;i< pesosRELIEF.size();i++)
			pesosRELIEF[i]+= (abs(distanciasEnemigo[i]) - abs(distanciasAmigo[i]));

	}

	//Ahora pasamos a normalizar los pesos
	float pesoMaximo = *max_element(pesosRELIEF.begin(),pesosRELIEF.end());

	for(int i=0;i< pesosRELIEF.size();i++)
		(pesosRELIEF[i] < 0)? pesosRELIEF[i]=0 : pesosRELIEF[i]= pesosRELIEF[i]/pesoMaximo;
}


/****************************************************************************************************/
//		void LocalSearch(vector<int> &particionTrain, vector<float> &pesosIniciales, float &mejorTasa)
//
//Algoritmo de Búsqueda Local basado en el primer mejor: en cuanto se genera una solución
//vecina que mejora a la actual, se aplica el movimiento y se pasa a la siguiente iteración.
//El algoritmo recibe una <particionTrain> sobre la que aprenderá unos determidos pesos con los que
//posteriormente validará la particion de Test, para obtener una tasa de clasificacion.
//Parte de una solucion de <pesosIniciales> aleatorios en [0,1] para comenzar a generar soluciones 
//vecinas generando como mucho 20*N <vecinosGenerados> (BL) o 2*N en caso de estar en un AM.
//La <mejorTasa> será devuelta junto con los <pesosIniciales> (mejor solucion) al final del algoritmo
//El <generadorNormal> y la <distribucionNormal> sirven para generar los vecinos cercanos.
//
//NOTA:						- El algoritmo utiliza sigma=0.3. 
//							- Condicion de parada: 20*N vecinos generados (Fuera de AM)
//							- MAX_Evaluaciones: 15000 evaluaciones de 1NN
//							- Cuando este dentro de un AM, generará 2*N	vecinos									
/***************************************************************************************************/
void ConjuntoDatos::LocalSearch(const vector<int> &particionTrain, const int &vecinosGenerados, vector<float> &pesosIniciales,
	float &mejorTasa){

	//Inicializamos el contador de vecinos generados y evaluaciones
	int numVecinosGenerados=0, numeroEvaluaciones=0 , componenteAMutar=-1;
	bool hayMejora=false;

	//Inicializamos los indices disponibles para la primera iteracion
	vector<int> indicesDisponibles(pesosIniciales.size(),0);
	iota(indicesDisponibles.begin(),indicesDisponibles.end(),0);
	
	//Solucion actual = pesos
	vector<float> solucionActual(pesosIniciales.begin(), pesosIniciales.end()),
	solucionS_Prima(pesosIniciales.begin(), pesosIniciales.end());
	//Variables auxiliares
	float clasificacion_SolActual =-10, clasificacion_SolSPrima =-10;

	//Calculamos unos primeros resultados de clasificacion con la solucion actual
	Clasificador1_NN(particionTrain, particionTrain,solucionActual,clasificacion_SolActual);
	
	//Mientras no hayamos superado el numero maximo de evaluaciones ni
	//hayamos generado un numero maximo de vecinos
	while(numeroEvaluaciones < MAX_Evaluaciones && numVecinosGenerados < vecinosGenerados){

		random_shuffle(indicesDisponibles.begin(), indicesDisponibles.end());
		hayMejora=false;

		for(int i=0; i < indicesDisponibles.size() && !hayMejora; i++) {

			vecinoPorMutacionNormal(solucionS_Prima,indicesDisponibles[i]);
			numVecinosGenerados++;
			//Obtenemos los resultados de la solucion vecina generada
			Clasificador1_NN(particionTrain, particionTrain,solucionS_Prima,clasificacion_SolSPrima);
			numeroEvaluaciones++;

			//Comprobamos si la solucion S_Prima tiene mejores resultados que la solucion actual
			if(clasificacion_SolSPrima > clasificacion_SolActual){

				//En caso de tener mejores resultados, actualizamos
				solucionActual = solucionS_Prima;
				clasificacion_SolActual = clasificacion_SolSPrima;
				hayMejora=true;
			}

		}

	}

	pesosIniciales = solucionActual;
	mejorTasa = clasificacion_SolActual;
}

/*************************************************************************************/
//	void generarGenAleatorio(float &cotaInferior, float &cotaSuperior, float &genAleatorio)
//
//Esta funcion genera un <genAleatorio> en el intervalo definido por los valores de 
// una <cotaInferior> y <cotaSuperior>.
/*************************************************************************************/
void ConjuntoDatos::generarGenAleatorio(const float &cotaInferior, const float &cotaSuperior, float &genAleatorio){

	//Generamos un gen aleatorio y lo acotamos segun las cotas inferior y superior
	float aleatorio = rand() * limiteGenAleatorio ;
    genAleatorio = cotaInferior + (aleatorio * (cotaSuperior - cotaInferior));

    //Truncamos en caso de tener valores negativos o mayores que 1
    if(genAleatorio < 0)
    	genAleatorio=0.0;

    else if (genAleatorio > 1)
    	genAleatorio=1.0;

}

/*************************************************************************************/
//			void BLX_Alpha(vector<float> &Padre1, vector<float> &Padre2,
//	vector<float> &Descendiente1, vector<float> &Descendiente2, const float &alpha)
//
//Operador de cruce BLX Aplha en el que a partir de dos padres se generan 2 descendientes
//siguiendo las especificaciones del cruce:
//Sea H_k = (h_k1,...h_ki,...h_kn)
//donde cada h_ki se genera en el intervalo [Cmin - I*<alpha>, Cmax +I*<alpha>]
//Cmax = max{C1_i,C2_i}
//Cmin = min{C1_i,C2_i}
//I = Cmax - Cmin
/*************************************************************************************/
void ConjuntoDatos::BLX_Alpha(const vector<float> &Padre1, const vector<float> &Padre2,
	vector<float> &Descendiente1, vector<float> &Descendiente2, const float &alpha){

	//Calculamos los máximos de cada padre
	float maxC1, maxC2, minC1, minC2;
	float Cmax,Cmin, I, cotaInferior, cotaSuperior, genAleatorio1=0.0, genAleatorio2=0.0;
	maxC1 = *max_element(Padre1.begin(), Padre1.end());
	maxC2 = *max_element(Padre2.begin(), Padre2.end());

	//Calculamos los minimos de cada padre
	minC1 = *min_element(Padre1.begin(), Padre1.end());
	minC2 = *min_element(Padre2.begin(), Padre2.end());

	//Obtenemos el Cmax y el Cmin teniendo en cuenta los 
	//maximos y minimos de cada padre
	Cmax = max(maxC1,maxC2);
	Cmin = min(minC1,minC2);

	//Calculamos el intervalo central
	I = Cmax - Cmin;

	//Calculamos las dos cotas
	cotaInferior = Cmin - (I*alpha);
	cotaSuperior = Cmax + (I*alpha);

	Descendiente1.clear();
	Descendiente2.clear();

	//Generamos los hijos teniendo en cuenta la generacion aleatoria
	//descrita anteriormente
	for(int i=0;i < Padre1.size();i++){

		generarGenAleatorio(cotaInferior,cotaSuperior,genAleatorio1);
		generarGenAleatorio(cotaInferior,cotaSuperior,genAleatorio2);

		Descendiente1.push_back(genAleatorio1);
		Descendiente2.push_back(genAleatorio2);
	}

}


/******************************************************************************************/
//void TorneoBinario(vector<float> &evaluacionesDeCromosomas, int &numeroDePadres, vector<int> &mejoresPadres);
//
//Funcion que recibe un conjunto de <evaluacionesCromosomas> y que implementa el Torneo
//Binario: generará <numeroDePadres> basándose en el vector <evaluacionesCromosomas> y 
//seleccionará al azar dos padres y se quedará con el mejor, indexandolo en <mejoresPadres>
//para posteriormente proceder a cruzarlos.
/******************************************************************************************/
void ConjuntoDatos::TorneoBinario(const vector<float> &evaluacionesDeCromosomas,const int &numeroDePadres,
	vector<int> &mejoresPadres){

	srand(1680851);
	int padreAleatorio1 = -1,padreAleatorio2 = -1, tamanioPoblacion=evaluacionesDeCromosomas.size();
	float evaluacionPadre1 = -1, evaluacionPadre2 = -1;
	//Segun el numero de evaluaciones (padres) disponibles
	//seleccionamos las N/2 parejas para cruzas
	for(int i=0; i< numeroDePadres; i++){

		padreAleatorio1 = (rand() % tamanioPoblacion);
		padreAleatorio2 = (rand() % tamanioPoblacion);

		//Evitar que el padre aleatorio 2 sea igual que el 1
		while(padreAleatorio2 == padreAleatorio1)
			padreAleatorio2 = (rand() % tamanioPoblacion);

		evaluacionPadre1 = evaluacionesDeCromosomas[padreAleatorio1];
		evaluacionPadre2 = evaluacionesDeCromosomas[padreAleatorio2];

		evaluacionPadre1 > evaluacionPadre2 ? mejoresPadres.push_back(padreAleatorio1):mejoresPadres.push_back(padreAleatorio2);

	}

}



/******************************************************************************************/
//void generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int &numeroDeCromosomas,
//								int &numeroDeGenes)
//
//Funcion que genera una <poblacionInicial> de forma aleatoria partiendo de un <numeroDeCromosomas>
//y <numeroDeGenes> por cromosoma.
/******************************************************************************************/
void ConjuntoDatos::generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int &numeroDeCromosomas,
	int &numeroDeGenes){

	vector<float> cromosoma;
	cromosoma.reserve(numeroDeCromosomas);
	//Rellenamos los cromosomas con los genes aleatorios (30 cromosomas)
	for(int i=0;i < numeroDeCromosomas;i++){

		for(int j = 0; j< numeroDeGenes;j++)
			cromosoma.push_back((rand() %10000)/10000.0);
		
		poblacionInicial.push_back(cromosoma);
		cromosoma.clear();

	}
}


/*************************************************************************************/
//void AGG_BLX(const vector<int> &particionTrain, const float &alpha, const float &probabilidadCruce, 
//			const float &probabilidadMutacion, vector<float> &mejorSolucion)
//
//Algoritmo genético generacional que recibe una <particionTrain> a partir de la cual evaluará
//la poblacion de cromosomas que obtenga en cada generacion. El operador de cruce 
//BLX_Alpha recibe el valor <alpha> sobre el que calculará el intervalo al que 
//pertenecerán los genes de los padres que se combinan. Cada pareja de padres se 
//combinará siguiendo una <probabilidadCruce> y cada gen tiene una <probabilidadMutacion>. 
//El algoritmo devuelve la mejor solucion en base a la <particionTrain> en un unico vector 
//de pesos llamado <mejorSolucion> tras realizar un numero <MAX_Evaluaciones> de llamadas
//a la funcion de evaluacion 1NN.
//El algoritmo genera una generacion nueva que reemplaza completamente a la actual,
//pero siempre se guarda el mejor padre de la generacion actual, mecanismo que evita
//que nunca se pueda volver hacia peores soluciones.
/*************************************************************************************/
void ConjuntoDatos::AGG_BLX(const vector<int> &particionTrain, const float &alpha, const float &probabilidadCruce, 
	const float &probabilidadMutacion, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDeEvaluaciones=0, numeroDeCruces = round((numeroDeCromosomasAGG/2)*probabilidadCruce),
	numeroDeMutaciones = round(numeroDeCromosomasAGG*(numeroDeGenes)*probabilidadMutacion),
	mejorPadreGeneracionInicial = -1, mejorPadreGeneracionSiguiente =-1 , peorPadreGeneracionSiguiente =-1,
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1;

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial, poblacionGenerada;
	poblacionInicial.reserve(numeroDeCromosomasAGG);
	poblacionGenerada.reserve(numeroDeCromosomasAGG);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente1, Descendiente2;
	Descendiente1.reserve(numeroDeGenes);
	Descendiente2.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAGG, numeroDeGenes);
	
	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0), 
	evaluacionesDeCromosomasGenerados(poblacionInicial.size(),0.0);

	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
		if(evaluacionesDeCromosomas[i] > mejorCromosoma){
			mejorCromosoma = evaluacionesDeCromosomas[i];
			mejorPadreGeneracionInicial= i;
		}

	}
	
	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAGG>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDeCromosomasAGG);
	
	//Comenzamos la ejecucion del algoritmo
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDeCromosomasAGG,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 15 parejas*<probabilidadCruce>
		//En este caso se generan 10,5 cruces, redondeando al alza, 11 cruces
		//Los cruces se realizan de 1 en 1.
		mejorPadrePerdido = true;

		for(int i=0,j=0; i<numeroDeCruces;i++,j+=2){

			BLX_Alpha(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente1,Descendiente2,alpha);

			poblacionGenerada.push_back(Descendiente1);
			poblacionGenerada.push_back(Descendiente2);
		}

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(int i= poblacionGenerada.size();i<numeroDeCromosomasAGG;i++){

			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAGG;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		//Calculamos las nuevas tasas de clasificacion
		for(int i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], 
				evaluacionesDeCromosomasGenerados[i]);

			numeroDeEvaluaciones++;

			//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
			if(evaluacionesDeCromosomasGenerados[i] > mejorCromosoma){
				mejorCromosoma = evaluacionesDeCromosomasGenerados[i];
				mejorPadreGeneracionSiguiente= i;
			}
			//Tambien guardamos el índice del peor padre en caso de que el mejor padre se haya perdido
			//de una generacion a otra, el peor de esta generacion será sustituido por el mejor de la anterior
			//Esto impide volver hacia malas soluciones
			else if(evaluacionesDeCromosomasGenerados[i] < peorCromosoma){
				peorCromosoma = evaluacionesDeCromosomasGenerados[i];
				peorPadreGeneracionSiguiente= i;
			}

		}

		//Si hemos perdido el mejor padre de una generacion a otra
		if(mejorPadrePerdido){

			//El peor padre de la generacion actual se susituye por el mejor de la generacion anterior
			poblacionGenerada[peorPadreGeneracionSiguiente] = poblacionInicial[mejorPadreGeneracionInicial];
			evaluacionesDeCromosomasGenerados[peorPadreGeneracionSiguiente] = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			//Se reevalua en caso de que el mejor padre de la generacion anterior sea además
			//el mejor de esta generacion
			if(mejorCromosoma < evaluacionesDeCromosomas[mejorPadreGeneracionInicial])
				mejorCromosoma = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			else
				mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

		}

		//Sustituimos la poblacion de la iteracion t por la generada en esta iteracion, la t+1
		//y todos sus atributos
		poblacionInicial.clear();
		poblacionInicial = poblacionGenerada;
		poblacionGenerada.clear();
		evaluacionesDeCromosomas.clear();
		evaluacionesDeCromosomas = evaluacionesDeCromosomasGenerados;
		evaluacionesDeCromosomasGenerados.clear();

		mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

	}

	mejorSolucion = poblacionInicial[mejorPadreGeneracionInicial];
}



/*************************************************************************************/
//	void CA(vector<float> &Padre1, vector<float> &Padre2, vector<float> &Descendiente)
//
//Operador de Cruce aritmético (CA) en el que a partir de un <Padre1> y un <Padre2>
//generamos un <Descendiente> de forma artimética:
//Para cada componente hi perteneciente a <Padre1> y cada ki perteneciente a <Padre2>
//la componente di del <Descendiente> se calculará como: (hi+ki)/2 
/*************************************************************************************/
inline void ConjuntoDatos::CA(vector<float> &Padre1, vector<float> &Padre2, vector<float> &Descendiente){

	for(int i=0;i<Padre1.size();i++)
		Descendiente.push_back((Padre1[i]+Padre2[i]/2));

}



/*************************************************************************************/
//void AGG_CA(vector<int> &particionTrain, const float &probabilidadCruce, 
//			const float &probabilidadMutacion, vector<float> &mejorSolucion)	//
//Algoritmo genético que recibe una <particionTrain> a partir de la cual evaluará
//la poblacion de cromosomas que obtenga en cada generacion. El operador de cruce 
//CA (Cruce Aritmético) hace que dos padres generen un unico hijo basándose en la media
//aritmética. Cada pareja de padres se combinará siguiendo una <probabilidadCruce> 
//y cada gen tiene una <probabilidadMutacion>. 
//El algoritmo devuelve la mejor solucion en base a la <particionTrain> en un unico vector 
//de pesos llamado <mejorSolucion> tras realizar un numero <MAX_Evaluaciones> de llamadas
//a la funcion de evaluacion 1NN.
//El algoritmo genera una generacion nueva que reemplaza completamente a la actual,
//pero siempre se guarda el mejor padre de la generacion actual, mecanismo que evita
//que nunca se pueda volver hacia peores soluciones.
/*************************************************************************************/
void ConjuntoDatos::AGG_CA(const vector<int> &particionTrain, const float &probabilidadCruce, 
	const float &probabilidadMutacion, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDePadres=numeroDeCromosomasAGG*2, numeroDeEvaluaciones=0, 
	numeroDeCruces = round(numeroDeCromosomasAGG*probabilidadCruce),
	numeroDeMutaciones = round(numeroDeCromosomasAGG*numeroDeGenes*probabilidadMutacion),
	mejorPadreGeneracionInicial = -1, mejorPadreGeneracionSiguiente =-1 , peorPadreGeneracionSiguiente =-1,
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1;

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial, poblacionGenerada;
	poblacionInicial.reserve(numeroDeCromosomasAGG);
	poblacionGenerada.reserve(numeroDeCromosomasAGG);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente;
	Descendiente.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAGG, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0), 
	evaluacionesDeCromosomasGenerados(poblacionInicial.size(),0.0);

	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
		if(evaluacionesDeCromosomas[i] > mejorCromosoma){
			mejorCromosoma = evaluacionesDeCromosomas[i];
			mejorPadreGeneracionInicial= i;
		}

	}
	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAGG>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDeCromosomasAGG);
	
	//Comenzamos la ejecucion del algoritmo
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 30 parejas*<probabilidadCruce>
		//En este caso se generan 21 cruces. Los cruces se realizan de 1 en 1.
		for(int i=0,j=0; i<numeroDeCruces;i++,j+=2){

			Descendiente.clear();
			CA(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente);

			poblacionGenerada.push_back(Descendiente);
		}

		mejorPadrePerdido = true;

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(int i= poblacionGenerada.size();i<numeroDeCromosomasAGG;i++){

			//Es i*2 dado que es el siguiente padre al ultimo que se cruzó con otro
			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i*2]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAGG;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		//Calculamos las nuevas tasas de clasificacion
		for(int i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], 
				evaluacionesDeCromosomasGenerados[i]);

			numeroDeEvaluaciones++;

			//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
			if(evaluacionesDeCromosomasGenerados[i] > mejorCromosoma){
				mejorCromosoma = evaluacionesDeCromosomasGenerados[i];
				mejorPadreGeneracionSiguiente= i;
			}
			//Tambien guardamos el índice del peor padre en caso de que el mejor padre se haya perdido
			//de una generacion a otra, el peor de esta generacion será sustituido por el mejor de la anterior
			//Esto impide volver hacia malas soluciones
			else if(evaluacionesDeCromosomasGenerados[i] < peorCromosoma){
				peorCromosoma = evaluacionesDeCromosomasGenerados[i];
				peorPadreGeneracionSiguiente= i;
			}

		}

		//Si hemos perdido el mejor padre de una generacion a otra
		if(mejorPadrePerdido){

			//El peor padre de la generacion actual se susituye por el mejor de la generacion anterior
			poblacionGenerada[peorPadreGeneracionSiguiente] = poblacionInicial[mejorPadreGeneracionInicial];
			evaluacionesDeCromosomasGenerados[peorPadreGeneracionSiguiente] = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			//Se reevalua en caso de que el mejor padre de la generacion anterior sea además
			//el mejor de esta generacion
			if(mejorCromosoma < evaluacionesDeCromosomas[mejorPadreGeneracionInicial])
				mejorCromosoma = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			else
				mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

		}

		//Sustituimos la poblacion de la iteracion t por la generada en esta iteracion, la t+1
		//y todos sus atributos
		poblacionInicial.clear();
		poblacionInicial = poblacionGenerada;
		poblacionGenerada.clear();
		evaluacionesDeCromosomas.clear();
		evaluacionesDeCromosomas = evaluacionesDeCromosomasGenerados;
		evaluacionesDeCromosomasGenerados.clear();

		mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

	}

	mejorSolucion = poblacionInicial[mejorPadreGeneracionInicial];
}

/*************************************************************************************/
//void AGE_BLX(vector<int> &particionTrain, const float &alpha, const float &probabilidadCruce, 
//			const float &probabilidadMutacion, vector<float> &mejorSolucion)
//
//Algoritmo genético estacionario que recibe una <particionTrain> a partir de la cual evaluará
//la poblacion de cromosomas que obtenga en cada generacion. El operador de cruce 
//BLX_Alpha recibe el valor <alpha> sobre el que calculará el intervalo al que 
//pertenecerán los genes de los padres que se combinan. Como la probabilidad de cruce es 1
//los padres generados se cruzan siempre. Cada gen tiene una <probabilidadMutacion>. 
//El algoritmo devuelve la mejor solucion en base a la <particionTrain> en un unico vector 
//de pesos llamado <mejorSolucion> tras realizar un numero <MAX_Evaluaciones> de llamadas
//a la funcion de evaluacion 1NN.
//El algoritmo genera dos hijos a partir de una pareja de mejores padres y estos sustituyen
//a los dos peores cromosomas de la poblacion actual (en caso de mejorarlos).
/*************************************************************************************/
void ConjuntoDatos::AGE_BLX(const vector<int> &particionTrain, const float &alpha, 
	const float &probabilidadMutacion, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDePadres=2, numeroDeEvaluaciones=0, 
	numeroDeMutaciones = round(numeroDeCromosomasAGG*numeroDeGenes*probabilidadMutacion),
	peorCromosoma=-1, segundoPeorCromosoma =-1, cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar ,mejorDeLosPadres;

	float tasaHijo1, tasaHijo2, tasaMejorHijo, tasaSegundoMejorHijo;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial;
	poblacionInicial.reserve(numeroDeCromosomasAGG);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente1, Descendiente2, mejorHijo1, mejorHijo2;
	Descendiente1.reserve(numeroDeGenes);
	Descendiente2.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAGG, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0);
	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		
	}

	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAGG>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDeCromosomasAGG);

	//Comenzamos la ejecucion del algoritmo
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		mejoresPadres.clear();
		//Seleccion: Lanzamos el torneo binario
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruzamos los dos padres generados
		BLX_Alpha(poblacionInicial[mejoresPadres[0]],poblacionInicial[mejoresPadres[1]],
			Descendiente1,Descendiente2, alpha);

		//Tras generar los nuevos descendientes pasamos a mutar la poblacion actual
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAGG;
			genAleatorioAMutar = rand() % numeroDeGenes;

			vecinoPorMutacionNormal(poblacionInicial[cromosomaAleatorioAMutar],genAleatorioAMutar);

			Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[cromosomaAleatorioAMutar], 
				evaluacionesDeCromosomas[cromosomaAleatorioAMutar]),
			numeroDeEvaluaciones++;
		}

		//Tras mutar pasamos a calcular quienes son los dos peores cromosomas de la poblacion
		peorCromosoma=0, segundoPeorCromosoma=1;
		(evaluacionesDeCromosomas[0] < evaluacionesDeCromosomas[1])? peorCromosoma=0, segundoPeorCromosoma=1:
		peorCromosoma=1, segundoPeorCromosoma=0;

		for(int i=2;i < evaluacionesDeCromosomas.size();i++){

			if(evaluacionesDeCromosomas[i] < evaluacionesDeCromosomas[peorCromosoma]){

				segundoPeorCromosoma=peorCromosoma;
				peorCromosoma=i;

			}

			else if(evaluacionesDeCromosomas[i] < evaluacionesDeCromosomas[segundoPeorCromosoma])
				segundoPeorCromosoma=i;

		}

		//Calculamos las tasas de los descendientes generados
		Clasificador1_NN(particionTrain,particionTrain,Descendiente1,tasaHijo1);
		numeroDeEvaluaciones++;

		Clasificador1_NN(particionTrain,particionTrain,Descendiente2,tasaHijo2);
		numeroDeEvaluaciones++;

		//Nos quedamos con el mejor de los hijos
		if(tasaHijo1 > tasaHijo2){

			mejorHijo1=Descendiente1;
			mejorHijo2=Descendiente2;
			tasaMejorHijo = tasaHijo1;
			tasaSegundoMejorHijo = tasaHijo2;
		} 

		else{

			mejorHijo1=Descendiente2;
			mejorHijo2=Descendiente1;
			tasaMejorHijo = tasaHijo2;
			tasaSegundoMejorHijo = tasaHijo1;

		} 
		//Intentaremos sustituir el peor de los padres por el mejor de los hijos
		if(tasaMejorHijo > evaluacionesDeCromosomas[peorCromosoma]){

			poblacionInicial[peorCromosoma] = mejorHijo1;
			evaluacionesDeCromosomas[peorCromosoma] = tasaMejorHijo;

			if(tasaSegundoMejorHijo > evaluacionesDeCromosomas[segundoPeorCromosoma]){

				poblacionInicial[segundoPeorCromosoma] = mejorHijo2;
				evaluacionesDeCromosomas[segundoPeorCromosoma] = tasaSegundoMejorHijo;
			}

		}

	}

	mejorDeLosPadres=0;
	for(int i=1;i<evaluacionesDeCromosomas.size();i++){

		if(evaluacionesDeCromosomas[i] > evaluacionesDeCromosomas[mejorDeLosPadres])
			mejorDeLosPadres=i;
	}

	mejorSolucion = poblacionInicial[mejorDeLosPadres];
}


/*************************************************************************************/
//void AGG_CA(vector<int> &particionTrain, const float &probabilidadCruce, 
//			const float &probabilidadMutacion, vector<float> &mejorSolucion)
//
//Algoritmo genético estacionario que recibe una <particionTrain> a partir de la cual evaluará
//la poblacion de cromosomas que obtenga en cada generacion. El operador de cruce 
//CA (Cruce Aritmético) hace que dos padres generen un unico hijo basándose en la media
//aritmética. Cada pareja de padres se combinará siguiendo una <probabilidadCruce> = 1 
//y cada gen tiene una <probabilidadMutacion>.  Como la probabilidad de cruce es 1
//los padres generados se cruzan siempre. El algoritmo devuelve la mejor solucion 
//en base a la <particionTrain> en un unico vector de pesos llamado <mejorSolucion> 
//tras realizar un numero <MAX_Evaluaciones> de llamadas a la funcion de evaluacion 1NN.
//El algoritmo genera dos hijos a partir de una pareja de mejores padres y estos sustituyen
//a los dos peores cromosomas de la poblacion actual (en caso de mejorarlos).
/*************************************************************************************/
void ConjuntoDatos::AGE_CA(const vector<int> &particionTrain,const float &probabilidadMutacion, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDeCromosomasAGG = 30,numeroDePadres=4, numeroDeEvaluaciones=0, 
	numeroDeMutaciones = round(numeroDeCromosomasAGG*numeroDeGenes*probabilidadMutacion),
	peorCromosoma=-1, segundoPeorCromosoma =-1, cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1,
	mejorDeLosPadres;

	float tasaHijo1, tasaHijo2, tasaMejorHijo, tasaSegundoMejorHijo;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial;
	poblacionInicial.reserve(numeroDeCromosomasAGG);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente1, Descendiente2, mejorHijo1, mejorHijo2;
	Descendiente1.reserve(numeroDeGenes);
	Descendiente2.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAGG, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0);
	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		
	}

	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAGG>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDeCromosomasAGG);

	//Comenzamos la ejecucion del algoritmo
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		mejoresPadres.clear();
		//Seleccion: Lanzamos el torneo binario
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruzamos los 4 padres generados
		CA(poblacionInicial[mejoresPadres[0]],poblacionInicial[mejoresPadres[1]],
			Descendiente1);
		CA(poblacionInicial[mejoresPadres[2]],poblacionInicial[mejoresPadres[3]],
			Descendiente2);

		//Tras generar los nuevos descendientes pasamos a mutar la poblacion actual
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAGG;
			genAleatorioAMutar = rand() % numeroDeGenes;

			vecinoPorMutacionNormal(poblacionInicial[cromosomaAleatorioAMutar],genAleatorioAMutar);

			Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[cromosomaAleatorioAMutar], 
				evaluacionesDeCromosomas[cromosomaAleatorioAMutar]),
			numeroDeEvaluaciones++;
		}

		//Tras mutar pasamos a calcular quienes son los dos peores cromosomas de la poblacion
		peorCromosoma=0, segundoPeorCromosoma=1;

		(evaluacionesDeCromosomas[0] < evaluacionesDeCromosomas[1])? peorCromosoma=0, segundoPeorCromosoma=1:
		peorCromosoma=1, segundoPeorCromosoma=0;

		for(int i=2;i < evaluacionesDeCromosomas.size();i++){

			if(evaluacionesDeCromosomas[i] < evaluacionesDeCromosomas[peorCromosoma]){

				segundoPeorCromosoma=peorCromosoma;
				peorCromosoma=i;

			}

			else if(evaluacionesDeCromosomas[i] < evaluacionesDeCromosomas[segundoPeorCromosoma])
				segundoPeorCromosoma=i;

		}

		//Calculamos las tasas de los descendientes generados
		Clasificador1_NN(particionTrain,particionTrain,Descendiente1,tasaHijo1);
		numeroDeEvaluaciones++;

		Clasificador1_NN(particionTrain,particionTrain,Descendiente2,tasaHijo2);
		numeroDeEvaluaciones++;

		//Nos quedamos con el mejor de los hijos
		if(tasaHijo1 > tasaHijo2){

			mejorHijo1=Descendiente1;
			mejorHijo2=Descendiente2;
			tasaMejorHijo = tasaHijo1;
			tasaSegundoMejorHijo = tasaHijo2;
		} 

		else{

			mejorHijo1=Descendiente2;
			mejorHijo2=Descendiente1;
			tasaMejorHijo = tasaHijo2;
			tasaSegundoMejorHijo = tasaHijo1;

		} 
		//Intentaremos sustituir el peor de los padres por el mejor de los hijos
		if(tasaMejorHijo > evaluacionesDeCromosomas[peorCromosoma]){

			poblacionInicial[peorCromosoma] = mejorHijo1;
			evaluacionesDeCromosomas[peorCromosoma] = tasaMejorHijo;

			if(tasaSegundoMejorHijo > evaluacionesDeCromosomas[segundoPeorCromosoma]){

				poblacionInicial[segundoPeorCromosoma] = mejorHijo2;
				evaluacionesDeCromosomas[segundoPeorCromosoma] = tasaSegundoMejorHijo;
			}

		}

	}

	mejorDeLosPadres=0;
	for(int i=1;i<evaluacionesDeCromosomas.size();i++){

		if(evaluacionesDeCromosomas[i] > evaluacionesDeCromosomas[mejorDeLosPadres])
			mejorDeLosPadres=i;
	}

	mejorSolucion = poblacionInicial[mejorDeLosPadres];

}



/*************************************************************************************/
//void AM_CA_Total(vector<int> &particionTrain, const float &probabilidadCruce, 
//			const float &probabilidadMutacion, vector<float> &mejorSolucion)
//
//Algoritmo memético que consiste en una hibridación del algoritmo genético con el 
//Cruce Aritmético como operador de cruce además de añadir una Búsqueda Local cada 10
//generaciones que se aplicará sobre todos los individuos de la poblacion, generando como 
//mucho 2*N vecinos para cada cromosoma. Parte de una <particionTrain> para generar las
//evaluaciones, con una <probabiliadadCruce> y una <probabilidadMutacion> y devuelve
//la <mejorSolucion> como un cromosoma.
/*************************************************************************************/
void ConjuntoDatos::AM_CA_Total(const vector<int> &particionTrain, const float &probabilidadCruce, 
	const float &probabilidadMutacion, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDePadres=numeroDeCromosomasAM*2, numeroDeGeneraciones=0, 
	numeroDeGenes=datos[0].size(), numeroDeEvaluaciones=0, numeroDeVecinos = numeroDeGenes*2, 
	numeroDeCruces = round(numeroDeCromosomasAM*probabilidadCruce),
	numeroDeMutaciones = round(numeroDeCromosomasAM*numeroDeGenes*probabilidadMutacion),
	mejorPadreGeneracionInicial = -1, mejorPadreGeneracionSiguiente =-1 , peorPadreGeneracionSiguiente =-1,
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1;

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0,mejorTasa;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial, poblacionGenerada;
	poblacionInicial.reserve(numeroDeCromosomasAM);
	poblacionGenerada.reserve(numeroDeCromosomasAM);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente;
	Descendiente.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAM, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0), 
	evaluacionesDeCromosomasGenerados(poblacionInicial.size(),0.0);

	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
		if(evaluacionesDeCromosomas[i] > mejorCromosoma){
			mejorCromosoma = evaluacionesDeCromosomas[i];
			mejorPadreGeneracionInicial= i;
		}

	}
	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAM>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDePadres);
	
	//Comenzamos la ejecucion del algoritmo memético
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 10 parejas*<probabilidadCruce>
		//En este caso se generan 21 cruces. Los cruces se realizan de 1 en 1.
		for(int i=0,j=0; i<numeroDeCruces;i++,j+=2){

			Descendiente.clear();

			CA(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente);

			poblacionGenerada.push_back(Descendiente);
		}

		mejorPadrePerdido = true;

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(int i= poblacionGenerada.size();i<numeroDeCromosomasAM;i++){

			//Es i*2 dado que es el siguiente padre al ultimo que se cruzó con otro
			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i*2]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAM;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Obtenemos una nueva generacion tras realizar el cruce, el reemplazamiento y la mutacion
		numeroDeGeneraciones++;

		if((numeroDeGeneraciones %10) == 0){

			for(int i=0;i<poblacionGenerada.size();i++){

				LocalSearch(particionTrain,numeroDeVecinos,poblacionGenerada[i],mejorTasa);
				numeroDeEvaluaciones+=(numeroDeGenes*2)+1;

			}
				
		}

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		//Calculamos las nuevas tasas de clasificacion
		for(int i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], 
				evaluacionesDeCromosomasGenerados[i]);

			numeroDeEvaluaciones++;

			//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
			if(evaluacionesDeCromosomasGenerados[i] > mejorCromosoma){
				mejorCromosoma = evaluacionesDeCromosomasGenerados[i];
				mejorPadreGeneracionSiguiente= i;
			}
			//Tambien guardamos el índice del peor padre en caso de que el mejor padre se haya perdido
			//de una generacion a otra, el peor de esta generacion será sustituido por el mejor de la anterior
			//Esto impide volver hacia malas soluciones
			else if(evaluacionesDeCromosomasGenerados[i] < peorCromosoma){
				peorCromosoma = evaluacionesDeCromosomasGenerados[i];
				peorPadreGeneracionSiguiente= i;
			}

		}

		//Si hemos perdido el mejor padre de una generacion a otra
		if(mejorPadrePerdido){

			//El peor padre de la generacion actual se susituye por el mejor de la generacion anterior
			poblacionGenerada[peorPadreGeneracionSiguiente] = poblacionInicial[mejorPadreGeneracionInicial];
			evaluacionesDeCromosomasGenerados[peorPadreGeneracionSiguiente] = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			//Se reevalua en caso de que el mejor padre de la generacion anterior sea además
			//el mejor de esta generacion
			if(mejorCromosoma < evaluacionesDeCromosomas[mejorPadreGeneracionInicial])
				mejorCromosoma = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			else
				mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

		}

		//Sustituimos la poblacion de la iteracion t por la generada en esta iteracion, la t+1
		//y todos sus atributos
		poblacionInicial.clear();
		poblacionInicial = poblacionGenerada;
		poblacionGenerada.clear();

		evaluacionesDeCromosomas.clear();
		evaluacionesDeCromosomas = evaluacionesDeCromosomasGenerados;
		evaluacionesDeCromosomasGenerados.clear();

		mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

	}

	mejorSolucion = poblacionInicial[mejorPadreGeneracionInicial];

}


/*************************************************************************************/
//		void AM_CA_PLS(vector<int> &particionTrain, const float &probabilidadCruce, 
//	const float &probabilidadMutacion, const float &PLS, vector<float> &mejorSolucion)
//
//Algoritmo memético que consiste en una hibridación del algoritmo genético con el 
//Cruce Aritmético como operador de cruce además de añadir una Búsqueda Local cada 10
//generaciones que se aplicará sobre  N*<PLS> individuos de la poblacion seleccionados de forma
//aleatoria, generando como  mucho 2*N vecinos para cada cromosoma. 
//Parte de una <particionTrain> para generar las evaluaciones, con una <probabiliadadCruce>
//y una <probabilidadMutacion>. Devuelve la <mejorSolucion> como un cromosoma.
/*************************************************************************************/
void ConjuntoDatos::AM_CA_PLS(const vector<int> &particionTrain, const float &probabilidadCruce, 
	const float &probabilidadMutacion,const float &PLS, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDePadres=numeroDeCromosomasAM*2, numeroDeGeneraciones=0, 
	numeroDeEvaluaciones=0, numeroDeVecinos = numeroDeGenes*2, 
	numeroDeCruces = round(numeroDeCromosomasAM*probabilidadCruce), cromosomasParaLS=PLS*numeroDeCromosomasAM,
	numeroDeMutaciones = round(numeroDeCromosomasAM*numeroDeGenes*probabilidadMutacion),
	mejorPadreGeneracionInicial = -1, mejorPadreGeneracionSiguiente =-1 , peorPadreGeneracionSiguiente =-1,
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1, cromosomaAleatorio;

	//Reservamos espacio para los PLS*numeroDeCromosomasAM cromosomas
	//que se les aplicará la Local Search y generamos un vector de indices desde
	//0 hasta el numero de cromosomas que nos servirá para elegir el numero 
	//de cromosomas que participarán en la Local Search
	vector<int> indicesDeCromosomas(numeroDeCromosomasAM,0.0);
	iota(indicesDeCromosomas.begin(), indicesDeCromosomas.end(),0.0);

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0, mejorTasa;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial, poblacionGenerada;
	poblacionInicial.reserve(numeroDeCromosomasAM);
	poblacionGenerada.reserve(numeroDeCromosomasAM);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente;
	Descendiente.reserve(numeroDeGenes);

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAM, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0), 
	evaluacionesDeCromosomasGenerados(poblacionInicial.size(),0.0);

	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
		if(evaluacionesDeCromosomas[i] > mejorCromosoma){
			mejorCromosoma = evaluacionesDeCromosomas[i];
			mejorPadreGeneracionInicial= i;
		}

	}
	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAM>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDePadres);
	
	//Comenzamos la ejecucion del algoritmo memético
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 30 parejas*<probabilidadCruce>
		//En este caso se generan 21 cruces. Los cruces se realizan de 1 en 1.
		for(int i=0,j=0; i<numeroDeCruces;i++,j+=2){

			Descendiente.clear();

			CA(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente);

			poblacionGenerada.push_back(Descendiente);
		}

		mejorPadrePerdido = true;

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(int i= poblacionGenerada.size();i<numeroDeCromosomasAM;i++){

			//Es i*2 dado que es el siguiente padre al ultimo que se cruzó con otro
			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i*2]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAM;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Obtenemos una nueva generacion tras realizar el cruce, el reemplazamiento y la mutacion
		numeroDeGeneraciones++;

		if((numeroDeGeneraciones %10) == 0){

			random_shuffle(indicesDeCromosomas.begin(),indicesDeCromosomas.end());

			for(int i=0; i< cromosomasParaLS;i++){
				LocalSearch(particionTrain, numeroDeVecinos,poblacionGenerada[i],mejorTasa);
				numeroDeEvaluaciones+=(numeroDeGenes*2)+1;
			}		

		}

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		//Calculamos las nuevas tasas de clasificacion
		for(int i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], 
				evaluacionesDeCromosomasGenerados[i]);

			numeroDeEvaluaciones++;

			//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
			if(evaluacionesDeCromosomasGenerados[i] > mejorCromosoma){
				mejorCromosoma = evaluacionesDeCromosomasGenerados[i];
				mejorPadreGeneracionSiguiente= i;
			}
			//Tambien guardamos el índice del peor padre en caso de que el mejor padre se haya perdido
			//de una generacion a otra, el peor de esta generacion será sustituido por el mejor de la anterior
			//Esto impide volver hacia malas soluciones
			else if(evaluacionesDeCromosomasGenerados[i] < peorCromosoma){
				peorCromosoma = evaluacionesDeCromosomasGenerados[i];
				peorPadreGeneracionSiguiente= i;
			}

		}

		//Si hemos perdido el mejor padre de una generacion a otra
		if(mejorPadrePerdido){

			//El peor padre de la generacion actual se susituye por el mejor de la generacion anterior
			poblacionGenerada[peorPadreGeneracionSiguiente] = poblacionInicial[mejorPadreGeneracionInicial];
			evaluacionesDeCromosomasGenerados[peorPadreGeneracionSiguiente] = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			//Se reevalua en caso de que el mejor padre de la generacion anterior sea además
			//el mejor de esta generacion
			if(mejorCromosoma < evaluacionesDeCromosomas[mejorPadreGeneracionInicial])
				mejorCromosoma = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			else
				mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

		}

		//Sustituimos la poblacion de la iteracion t por la generada en esta iteracion, la t+1
		//y todos sus atributos
		poblacionInicial.clear();
		poblacionInicial = poblacionGenerada;
		poblacionGenerada.clear();

		evaluacionesDeCromosomas.clear();
		evaluacionesDeCromosomas = evaluacionesDeCromosomasGenerados;
		evaluacionesDeCromosomasGenerados.clear();

		mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

	}

	mejorSolucion = poblacionInicial[mejorPadreGeneracionInicial];

}


/*************************************************************************************/
//		void AM_CA_Mejores(vector<int> &particionTrain, const float &probabilidadCruce, 
//	const float &probabilidadMutacion, const float &PLS, vector<float> &mejorSolucion)
//
//Algoritmo memético que consiste en una hibridación del algoritmo genético con el 
//Cruce Aritmético como operador de cruce además de añadir una Búsqueda Local cada 10
//generaciones que se aplicará sobre los N*<PLS> MEJORES individuos de la poblacion, 
//generando como  mucho 2*N vecinos para cada cromosoma. 
//Parte de una <particionTrain> para generar las evaluaciones, con una <probabiliadadCruce>
//y una <probabilidadMutacion>. Devuelve la <mejorSolucion> como un cromosoma.
/*************************************************************************************/
void ConjuntoDatos::AM_CA_Mejores(const vector<int> &particionTrain, const float &probabilidadCruce, 
const float &probabilidadMutacion,const float &PLS, vector<float> &mejorSolucion){

	//El algoritmo comienza generando una poblacion inicial.
	//Esta poblacion se compone de cromosomas en los que cada gen
	//pertenece al intervalo [0,1]
	int numeroDePadres=numeroDeCromosomasAM*2, numeroDeGeneraciones=0, 
	numeroDeEvaluaciones=0, numeroDeVecinos = numeroDeGenes*2, 
	numeroDeCruces = round(numeroDeCromosomasAM*probabilidadCruce), cromosomasParaLS=PLS*numeroDeCromosomasAM,
	numeroDeMutaciones = round(numeroDeCromosomasAM*numeroDeGenes*probabilidadMutacion),
	mejorPadreGeneracionInicial = -1, mejorPadreGeneracionSiguiente =-1 , peorPadreGeneracionSiguiente =-1,
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1, cromosomaAleatorio;

	//Reservamos espacio para los PLS*numeroDeCromosomasAM cromosomas
	//que se les aplicará la Local Search y generamos un vector de indices desde
	//0 hasta el numero de cromosomas que nos servirá para elegir el numero 
	//de cromosomas que participarán en la Local Search
	vector<int> indicesDeCromosomas(numeroDeCromosomasAM,0.0);
	iota(indicesDeCromosomas.begin(), indicesDeCromosomas.end(),0.0);

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0,mejorTasa;

	//Poblaciones inicial y la de la generacion T+1
	vector<vector<float> > poblacionInicial, poblacionGenerada;
	poblacionInicial.reserve(numeroDeCromosomasAM);
	poblacionGenerada.reserve(numeroDeCromosomasAM);

	//Cromosoma que representará una de las soluciones iniciales
	vector<float> Descendiente;
	Descendiente.reserve(numeroDeGenes);

	//Declaramos un conjunto de pares (tasa, indice del cromosoma) donde para cada 
	//tasa sabremos el índice del cromosoma correspondiente, conjunto que si ordenamos
	//en funcion de la tasa de clasificacion, nos permitirá acceder a los mejores cromosomas
	//que necesitemos
	pair<float,int> tasaConIndice;
	set<pair<float,int> > tasasIndexadas;

	generarPoblacionAleatoria(poblacionInicial, numeroDeCromosomasAM, numeroDeGenes);

	//Ahora vamos a calcular las tasas de clasificacion de cada cromosoma
	vector<float> evaluacionesDeCromosomas(poblacionInicial.size(),0.0), 
	evaluacionesDeCromosomasGenerados(poblacionInicial.size(),0.0);


	for(int i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i], evaluacionesDeCromosomas[i]);
		numeroDeEvaluaciones++;
		//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
		if(evaluacionesDeCromosomas[i] > mejorCromosoma){
			mejorCromosoma = evaluacionesDeCromosomas[i];
			mejorPadreGeneracionInicial= i;
		}

	}
	//Declaramos los mejores padres de cada generacion y reservamos espacio para
	//<numeroDeCromosomasAM>
	vector<int> mejoresPadres;
	mejoresPadres.reserve(numeroDePadres);
	
	//Comenzamos la ejecucion del algoritmo memético
	while(numeroDeEvaluaciones < MAX_Evaluaciones){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);
		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 30 parejas*<probabilidadCruce>
		//En este caso se generan 21 cruces. Los cruces se realizan de 1 en 1.
		for(int i=0,j=0; i<numeroDeCruces;i++,j+=2){

			Descendiente.clear();
			CA(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente);

			poblacionGenerada.push_back(Descendiente);
		}

		mejorPadrePerdido = true;

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(int i= poblacionGenerada.size();i<numeroDeCromosomasAM;i++){
			
			//Es i*2 dado que es el siguiente padre al ultimo que se cruzó con otro
			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i*2]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(int i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAM;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Obtenemos una nueva generacion tras realizar el cruce, el reemplazamiento y la mutacion
		numeroDeGeneraciones++;

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		tasasIndexadas.clear();
		
		//Calculamos las nuevas tasas de clasificacion
		for(int i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], 
				evaluacionesDeCromosomasGenerados[i]);

			//Guardamos las clasificaciones para obtener los mejores padres mas adelante
			tasaConIndice.first= evaluacionesDeCromosomas[i];
			tasaConIndice.second = i;
			tasasIndexadas.insert(tasaConIndice);

			numeroDeEvaluaciones++;

			//Guardamos el indice del mejor padre para saber si lo hemos perdido mas adelante
			if(evaluacionesDeCromosomasGenerados[i] > mejorCromosoma){
				mejorCromosoma = evaluacionesDeCromosomasGenerados[i];
				mejorPadreGeneracionSiguiente= i;
			}
			//Tambien guardamos el índice del peor padre en caso de que el mejor padre se haya perdido
			//de una generacion a otra, el peor de esta generacion será sustituido por el mejor de la anterior
			//Esto impide volver hacia malas soluciones
			else if(evaluacionesDeCromosomasGenerados[i] < peorCromosoma){
				peorCromosoma = evaluacionesDeCromosomasGenerados[i];
				peorPadreGeneracionSiguiente= i;
			}

		}

		if((numeroDeGeneraciones %10) == 0){

			set<pair<float,int> >::reverse_iterator iter = tasasIndexadas.rbegin();
			for(int i=0;i < cromosomasParaLS;i++){

				LocalSearch(particionTrain, numeroDeVecinos,poblacionGenerada[iter->second],mejorTasa);
				evaluacionesDeCromosomasGenerados[iter->second] = mejorTasa;
				numeroDeEvaluaciones+=(numeroDeGenes*2)+1;
				++iter;
			}		

		}
		//Si hemos perdido el mejor padre de una generacion a otra
		if(mejorPadrePerdido){

			//El peor padre de la generacion actual se susituye por el mejor de la generacion anterior
			poblacionGenerada[peorPadreGeneracionSiguiente] = poblacionInicial[mejorPadreGeneracionInicial];
			evaluacionesDeCromosomasGenerados[peorPadreGeneracionSiguiente] = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			//Se reevalua en caso de que el mejor padre de la generacion anterior sea además
			//el mejor de esta generacion
			if(mejorCromosoma < evaluacionesDeCromosomas[mejorPadreGeneracionInicial])
				mejorCromosoma = evaluacionesDeCromosomas[mejorPadreGeneracionInicial];

			else
				mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

		}

		//Sustituimos la poblacion de la iteracion t por la generada en esta iteracion, la t+1
		//y todos sus atributos
		poblacionInicial.clear();
		poblacionInicial = poblacionGenerada;
		poblacionGenerada.clear();

		evaluacionesDeCromosomas.clear();
		evaluacionesDeCromosomas = evaluacionesDeCromosomasGenerados;
		evaluacionesDeCromosomasGenerados.clear();

		mejorPadreGeneracionInicial = mejorPadreGeneracionSiguiente;

	}

	mejorSolucion = poblacionInicial[mejorPadreGeneracionInicial];
}

/*************************************************************************************/
//							void setParticiones()
//
//Función para establecer las 5 particiones del conjunto de datos:
//El objetivo de esta función es el de generar 5 particiones distintas del conjunto de
//datos original de la siguiente manera: de forma aleatoria ordenará los índices de los
//vectores de características de forma que la mitad correspondan a los datos de
//entrenamiento y la otra mitad a los de prueba. En caso de ser impares, el sobrante
//será para los datos de entrenamiento
/*************************************************************************************/
void ConjuntoDatos::setParticiones(){

	//Creamos el vector de indices: desde 0 hasta el tamaño de la matriz de datos - 1
	vector<int> indicesDeParticiones(this->datos.size(),0);
	iota(indicesDeParticiones.begin(),indicesDeParticiones.end(),0);

	srand(1680851);
	//Generamos las 5 particiones
	for(int i=0;i<5;i++){

		vector<int> particionesAux = indicesDeParticiones;
		random_shuffle(particionesAux.begin(),particionesAux.end());
		particiones.push_back(particionesAux);

	}

}

/*************************************************************************************/
//							vector<vector<float> > getDatos()
//
//Función de tipo consultora que me devuelve los datos dentro del objeto 
//de la clase ConjuntoDatos.
/*************************************************************************************/
vector<vector<float> > ConjuntoDatos::getDatos(){

	return this->datos;
}


/*************************************************************************************/
//							vector<string> getEtiquetas()
//
//Función de tipo consultora que me devuelve el conjunto de etiquetas dentro del objeto 
//de la clase ConjuntoDatos.
/*************************************************************************************/
vector<string> ConjuntoDatos::getEtiquetas(){

	return this->etiquetas;
}


/*************************************************************************************/
//							vector<vector<int> >getParticiones()
//
//Función de tipo consultora que me devuelve las particiones del conjunto de datos
/*************************************************************************************/
vector<vector<int> >ConjuntoDatos::getParticiones(){

	return this->particiones;


}