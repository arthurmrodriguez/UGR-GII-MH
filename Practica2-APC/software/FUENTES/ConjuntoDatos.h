/******************************************************************************
Práctica 2 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
Arthur Mickael Rodríguez Nesterenko 
DNI: Y1680851W
Profesor: Salvador García
Grupo de Computación y Sistemas Inteligentes
Fichero ConjuntoDatos.h: fichero donde se define la clase ConjuntoDatos
que servirá para albergar la información con la que ejecutar los algoritmos
*****************************************************************************/

//Incluimos el fichero de utilidades con todas las dependencias.
#include "Utilidades.h"

using namespace std;


class ConjuntoDatos{

	private:

		vector<vector<float> > datos;
		vector<string> etiquetas;
		vector<vector<int> > particiones;
		int numeroDeCromosomasAGG, numeroDeCromosomasAM, numeroDeGenes;
		float limiteGenAleatorio;

	public:

		const int MAX_EVALUACIONES=15000;
		const int MAX_EVALUACIONES_ILS=1000;

		/*************************************************************************************/
		//	ConjuntoDatos(vector<vector<float> > &datos, vector<string> &etiquetas,
		//	int numeroDeCromosomasAGG,int numeroDeCromosomasAM)
		//
		//Constructor de la clase ConjuntoDatos a partir de un conjunto de <datos> y de <etiquetas>
		//leidos de un fichero. Los datos son normalizados antes de ser introducidos en los
		//atributos de la clase correspondiente. Tambien se lee desde fichero el numero de cromosomas
		//para cada algoritmo: genético y memético 
		/*************************************************************************************/
		ConjuntoDatos(vector<vector<float> > &datos, vector<string> &etiquetas,
			int &numeroDeCromosomasAGG,int &numeroDeCromosomasAM);

		/*************************************************************************************/
		//			~ConjuntoDatos()
		//
		//Destructor de la clase ConjuntoDatos
		/*************************************************************************************/
		~ConjuntoDatos();
		

		/*************************************************************************************/
		//					normalizarDatos(vector<vector<float> > &datos)
		//
		//Realiza las normalizaciones del conjunto de datos contenidos en <datos> al intervalo
		// [0,1] siguiendo la formula que se nos proporciona en las transparencias de clase:
		// Xj = (Xj - MINj)/(MAXj - MINj)
		/*************************************************************************************/
		void normalizarDatos(vector<vector<float> > &datos);


		/**********************************************************************************************/
		//		void calcularAmigoMasCercano(vector<int> particionTest,vector<float> &distanciasAmigo,
		//									int indiceEPrima)
		//
		//Funcion que encuentra en un conjunto de datos <particionTest> el amigo más cercano
		// (aquel de su misma clase y que es distinto de el mismo) de un determinado vector de
		//características indicado por <indiceEPrima> (normalmente el conjunto de train).
		// Devuelve las distancias al amigo mas cercano en <distanciasAmigo>
		/**********************************************************************************************/
		void calcularAmigoMasCercano(const vector<int> &particionTest,vector<float> &distanciasAmigo,
			const int &indiceEPrima);



		/*************************************************************************************/
		//	void calcularEnemigoMasCercano(vector<int> particionTest,vector<float> &distanciasEnemigo,
		//									int indiceEPrima)
		//
		//Funcion que encuentra en un conjunto de datos <particionTest> el enemigo más cercano
		// (aquel de clase distinta) de un determinado vector de
		//características indicado por <indiceEPrima> (normalmente el conjunto de train).
		// Devuelve las distancias al amigo mas cercano en <distanciasAmigo>
		/*************************************************************************************/
		void calcularEnemigoMasCercano(const vector<int> &particionTest,vector<float> &distanciasEnemigo,
			const int &indiceEPrima);

		

		/*_____________________________________________ALGORITMOS______________________________________________*/


		/*****************************************************************************************************/
		//		void Clasificador1_NN(vector<int> particionTrain, vector<int> particionTest, 
		//						vector<float> &pesos, float &tasaClasificacion)
		//
		//Clasificador de tipo K_NN con K=1 vecino mas cercano. Este algoritmo calcula a partir de una 
		//<particionTrain> (80% de los datos) y una <particionTest> (20% de los datos) el porcentaje o la tasa de 
		//clasificación siguiendo las directrices del algoritmo que aparece en las transparencias
		//del seminario: considerando el numero de instancias bien clasificadas y además que el numero de
		//características utilizado sea el menor posible, ponderando la importancia del acierto y la reduccion
		//con un parametro <alpha>=0.5 (ambos criterios sin igual de importantes). Utiliza un vector de <pesos>
		//para que la función pueda ser utilizada por los distintos algoritmos posteriores. 
		//Haremos uso de una función auxiliar calcularDistanciaEuclidea para agilizar el proceso.
		//NOTA: Se utiliza el procedimiento <leave one out> en los casos que se requiera (LS, AGG, AGE,etc).
		//A través de una condicion que no permite calculos de distancias para indices iguales.
		//El algoritmo devuelve la tasa de clasificacion, la tasa de reduccion y el agregado teniendo en cuenta 
		//el parametro <alpha>
		/*****************************************************************************************************/
		void Clasificador1_NN(const vector<int> &particionTrain, const vector<int> &particionTest, 
			const vector<float> &pesos, const float &alpha, float &tasaClasificacion, float &tasaReduccion, float &agregado);



		/*****************************************************************************************************/
		//		void Greedy_RELIEF(const vector<int> particionTrain , vector<float> &pesosRELIEF)
		//
		//Algoritmo Greedy que a partir de una <particionTrain> y frente a una <particionTest> 
		// calcula un vector de <pesosRELIEF> con el que posteriormente calcularemos el porcentaje de clasificacion 
		//según el clasificador 1_NN. El metodo considera que leave one out es true, dado que evalua el la particion
		//consigo misma.
		/*****************************************************************************************************/
		void Greedy_RELIEF(const vector<int> &particionTrain, vector<float> &solucionInicial);


		/*****************************************************************************************************/
		//		void EnfriamientoSimulado(const vector<int> &particionTrain, vector<float> &solucionInicial,
		//					float mu, float phi, int numeroCaracteristicas )
		//
		//Algoritmo de Enfriamiento Simulado (ES): partiendo de un conjunto de entrenamiento <particionTrain> y
		//de una <solucionInicial> (vector de pesos), calcula una temperatura Inicial en base a <mu> y <phi> y 
		//aplica el esquema de Cauchy Modificado para simular el enfriamieno de particulas de forma que en cada 
		//iteracion se aplica un Mov(W, sigma) para generar una solucion vecina que se comparará con la actual. 
		//Se enfriará la temperatura hasta un valor minimo (10^-3) ,cuando se haya generado un numero maximo de vecinos
		//(mejores o no) o cuando se haya aceptado un numero maximo de vecinos. El algoritmo termina tras 
		//MAX_EVALUACIONES/numeroVecinos o cuando en la iteracion i-esima el numero maximo de exitos
		//sea igual a 0, devolviendo la mejor solucion en <solucionInicial>.
		/*****************************************************************************************************/
		void EnfriamientoSimulado(const vector<int> &particionTrain, vector<float> &solucionInicial,
			float mu, float phi, int numeroCaracteristicas);

	
		/****************************************************************************************************/
		//LocalSearch(const vector<int> &particionTrain, vector<float> &pesosIniciales, const int &vecinosGenerados,
		//						const int &maxEvals, float &alpha, float &agregado)
		//
		//Algoritmo de Búsqueda Local basado en el primer mejor: en cuanto se genera una solución
		//vecina que mejora a la actual, se aplica el movimiento y se pasa a la siguiente iteración.
		//El algoritmo recibe una <particionTrain> sobre la que aprenderá unos determidos pesos con los que
		//posteriormente validará la particion de Test, para obtener una valor de agregado =
		//(tasaClasif*alpha + tasaReduc*(1-alpha)). Parte de una solucion de <pesosIniciales> para comenzar 
		//a generar soluciones vecinas generando como mucho <vecinosGenerados> (BL).
		//
		//El mejor <agregado> será devuelto junto con los <pesosIniciales> (mejor solucion) al final del algoritmo
		//El <generadorNormal> y la <distribucionNormal> sirven para generar los vecinos cercanos.
		//
		//NOTA:						- El algoritmo utiliza sigma=0.3. 
		//							- Condicion de parada: <vecinosGenerados> 
		//							- MAX_Evaluaciones: 15000 evaluaciones de 1NN
		/***************************************************************************************************/
		void LocalSearch(const vector<int> &particionTrain, vector<float> &pesosIniciales, const int &vecinosGenerados,
			const int &maxEvals, float alpha, float &agregado);


		/****************************************************************************************************/
		//void ILS(const vector<int> &particionTrain, vector<float> &solucion,const int &numeroCaracteristicas,
		//							const int &vecinosGenerados, constint &iteraciones)
		//
		//Algoritmo de Búsqueda Local Iterativa (basado en el criterio del mejor): se genera una solucion aleatoria
		//inicial y se aplica la BL sobre ella. Una vez optimizada la solucion se estudiará si es MEJOR que la MEJOR
		//solucion encontrada hasta el momento y se realizara una mutacion FUERTE sobre la mejor solucion. Esta mutacion
		//se realizará sobre t componentes elegidas al azar, siguiendo un Mov(W,sigma). El algoritmo devolverá la mejor
		//solucion encontrada tras 15 <iteraciones> completas
		//
		//NOTA:						- El algoritmo utiliza sigma=0.4.
		//							- Numero de mutaciones (t) = 0.1* <numeroCaracteristicas> 
		//							- Condicion de parada: 20*N vecinos generados
		//							- MAX_EVALUACIONES_ILS: 1000 evaluaciones de 1NN							
		/***************************************************************************************************/
		void ILS(const vector<int> &particionTrain, vector<float> &solucion,const int &numeroCaracteristicas,
			const int &vecinosGenerados, const int &iteraciones);


		/**************************************************************************************************/
		//void DE_Rand_1(const vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
		//				const int &numeroSoluciones, vector<float> &mejorSolucion)
		//
		//Algoritmo de Evolucion Diferencial: este algoritmo parte de una poblacion inicial de tamaño
		//<numeroSoluciones> que iremos mejorando en cada iteracion. El algoritmo recorrera la poblacion 
		//en cada iteracion, seleccionando en cada momento 3 vectores solucion de forma aleatoria y excluyente. 
		//Posteriormente recorrera cada una de las  caracteristicas e ira conformando una nueva solucion basandose
		// en un <factorEscalado> mediante el metodo de recombinacion binomial seleccionando en cada momento 
		//el mejor entre el padre y el hijo siguiendo una <probCruce>. Al termino de este proceso, se realizara
		// una seleccion one-to-one entre el padre y el offspring generado. Finalmente se evaluaran los hijos
		// generados y se sustituira la poblacion de la generacion actual. El algoritmo termina tras realizar
		//MAX_EVALUACIONES de la funcion objetivo.
		/**************************************************************************************************/
		void DE_Rand_1(const vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
			const int &numeroSoluciones,vector<float> &mejorSolucion);


		/**************************************************************************************************/
		//void DE_CurrentToBest_1(vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
		//				const int &numeroSoluciones, vector<float> &mejorSolucion)
		//
		//Algoritmo de Evolucion Diferencial: este algoritmo parte de una poblacion inicial de tamaño
		//<numeroSoluciones> que iremos mejorando en cada iteracion. El algoritmo recorrera la poblacion 
		//en cada iteracion, seleccionando en cada momento 2 vectores solucion de forma aleatoria y excluyente y 
		//el tercero sera siempre la mejor solucion de la poblacion actual, para desplazarnos a soluciones mejores,
		//Posteriormente recorrera cada una de las  caracteristicas e ira conformando una nueva solucion basandose
		// en un <factorEscalado> mediante el metodo de recombinacion binomial seleccionando en cada momento 
		//el mejor entre el padre y el hijo siguiendo una <probCruce>. Al termino de este proceso, se realizara
		// una seleccion one-to-one entre el padre y el offspring generado. Finalmente se evaluaran los hijos
		// generados y se sustituira la poblacion de la generacion actual. El algoritmo termina tras realizar
		//MAX_EVALUACIONES de la funcion objetivo.
		/**************************************************************************************************/
		void DE_CurrentToBest_1(const vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
			const int &numeroSoluciones,vector<float> &mejorSolucion);


		/*************************************************************************************/
		//	void generarGenAleatorio(float &cotaInferior, float &cotaSuperior, float &genAleatorio)
		//
		//Esta funcion genera un <genAleatorio> en el intervalo definido por los valores de 
		// una <cotaInferior> y <cotaSuperior>.
		/*************************************************************************************/
		inline void generarGenAleatorio(const float &cotaInferior, const float &cotaSuperior, float &genAleatorio){

			//Generamos un gen aleatorio y lo acotamos segun las cotas inferior y superior
			genAleatorio = Rand();

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
		void BLX_Alpha(const vector<float> &Padre1, const vector<float> &Padre2,
			vector<float> &Descendiente1, vector<float> &Descendiente2, const float &alpha);


		/******************************************************************************************/
		//void TorneoBinario(vector<float> &evaluacionesDeCromosomas, int &numeroDePadres, 
		//								vector<int> &mejoresPadres)
		//
		//Funcion que recibe un conjunto de <evaluacionesCromosomas> y que implementa el Torneo
		//Binario: generará <numeroDePadres> basándose en el vector <evaluacionesCromosomas> y 
		//seleccionará al azar dos padres y se quedará con el mejor, indexandolo en <mejoresPadres>
		//para posteriormente proceder a cruzarlos.
		/******************************************************************************************/
		void TorneoBinario(const vector<float> &evaluacionesDeCromosomas,const int &numeroDePadres,
			vector<int> &mejoresPadres);

		
		/*************************************************************************************/
		//void AGG_BLX(vector<int> &particionTrain, const float &alpha, const float &probabilidadCruce, 
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
		void AGG_BLX(const vector<int> &particionTrain, const float &alpha, const float &probabilidadCruce, 
			const float &probabilidadMutacion, vector<float> &mejorSolucion);



		/*************************************************************************************/
		//	void CA(vector<float> &Padre1, vector<float> &Padre2, vector<float> &Descendiente)
		//
		//Operador de Cruce aritmético (CA) en el que a partir de un <Padre1> y un <Padre2>
		//generamos un <Descendiente> de forma artimética:
		//Para cada componente hi perteneciente a <Padre1> y cada ki perteneciente a <Padre2>
		//la componente di del <Descendiente> se calculará como: (hi+ki)/2 
		/*************************************************************************************/
		inline void CA(vector<float> &Padre1, vector<float> &Padre2, vector<float> &Descendiente){

			Descendiente.clear();
			for(int i=0;i<Padre1.size();i++)
				Descendiente.push_back((Padre1[i]+Padre2[i]/2));

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
		void AM_CA_Mejores(const vector<int> &particionTrain, const float &probabilidadCruce, 
			const float &probabilidadMutacion,const float &PLS, vector<float> &mejorSolucion);


		
		/*******************************************************************************************/

		/*************************************************************************************/
		//							void setParticiones()
		//
		//Función para establecer las 5 particiones del conjunto de datos:
		//El objetivo de esta función es el de generar 5 particiones distintas del conjunto de
		//datos original de la siguiente manera: de forma aleatoria ordenará los índices de los
		//vectores de características y partirá el conjunto de datos cada 20% para crear las 5 
		//particiones. Cuando realicemos los calculos, 4 de estas particiones se usaran para
		//entrenar y la otra para validar.
		/*************************************************************************************/
		void setParticiones();

		vector<vector<float> > getDatos();
		vector<string> getEtiquetas();
		vector<vector<int> > getParticiones();

};