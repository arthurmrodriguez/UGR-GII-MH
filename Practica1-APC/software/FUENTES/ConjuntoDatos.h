/******************************************************************************
Práctica 1 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
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

		const int MAX_Evaluaciones=15000;

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
		//Clasificador de tipo K_NN con K=1 vecino mas cercano. Este algoritmo calcula a partir
		//de una <particionTrain> y una <particionTest> del conjunto de datos (1,...,5) el porcentaje o la tasa de 
		//clasificación siguiendo las directrices del algoritmo que aparece en las transparencias
		//del seminario. Utiliza un vector de <pesos> para que la función pueda ser utilizada 
		//por los distintos algoritmos posteriores. Haremos uso de una función auxiliar 
		//calcularDistanciaEuclidea para agilizar el proceso.
		//NOTA: Se utiliza el procedimiento <leave one out> en los casos que se requiera (LS, AGG, AGE,etc).
		//A través de una condicion que no permite calculos de distancias para indices iguales
		/*****************************************************************************************************/
		void Clasificador1_NN(const vector<int> &particionTrain, const vector<int> &particionTest, 
			const vector<float> &pesos, float &tasaClasificacion);



		/*****************************************************************************************************/
		//		void Greedy_RELIEF(const vector<int> particionTrain , vector<float> &pesosRELIEF)
		//
		//Algoritmo Greedy que a partir de una <particionTrain> y frente a una <particionTest> 
		// calcula un vector de <pesosRELIEF> con el que posteriormente calcularemos el porcentaje de clasificacion 
		//según el clasificador 1_NN. El metodo considera que leave one out es true, dado que evalua el la particion
		//consigo misma.
		/*****************************************************************************************************/
		void Greedy_RELIEF(const vector<int> &particionTrain,vector<float> &pesosRELIEF);



	
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
		void LocalSearch(const vector<int> &particionTrain, const int &vecinosGenerados, vector<float> &pesosIniciales,
			float &mejorTasa);



		/*************************************************************************************/
		//			void generarGenAleatorio(float &cotaInferior, float &cotaSuperior, float &genAleatorio)
		//
		//Esta funcion genera un <genAleatorio> en el intervalo definido por los valores de 
		// una <cotaInferior> y <cotaSuperior>.
		//
		/*************************************************************************************/
		void generarGenAleatorio(const float &cotaInferior, const float &cotaSuperior, float &genAleatorio);


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


		/******************************************************************************************/
		//void generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int &numeroDeCromosomas,
		//								int &numeroDeGenes)
		//
		//Funcion que genera una <poblacionInicial> de forma aleatoria partiendo de un <numeroDeCromosomas>
		//y <numeroDeGenes> por cromosoma.
		/******************************************************************************************/
		void generarPoblacionAleatoria(vector<vector<float> > &poblacionInicial, int &numeroDeCromosomas,
			int &numeroDeGenes);

		
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
		inline void CA(vector<float> &Padre1, vector<float> &Padre2, vector<float> &Descendiente);




		/*************************************************************************************/
		//void AGG_CA(vector<int> &particionTrain, const float &probabilidadCruce, 
		//			const float &probabilidadMutacion, vector<float> &mejorSolucion)
		//
		//Algoritmo genético generacional que recibe una <particionTrain> a partir de la cual evaluará
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
		void AGG_CA(const vector<int> &particionTrain, const float &probabilidadCruce, 
			const float &probabilidadMutacion, vector<float> &mejorSolucion);



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
		void AGE_BLX(const vector<int> &particionTrain, const float &alpha, 
			const float &probabilidadMutacion, vector<float> &mejorSolucion);


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
		void AGE_CA(const vector<int> &particionTrain,const float &probabilidadMutacion, vector<float> &mejorSolucion);


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
		void AM_CA_Total(const vector<int> &particionTrain, const float &probabilidadCruce, 
			const float &probabilidadMutacion, vector<float> &mejorSolucion);


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
		void AM_CA_PLS(const vector<int> &particionTrain, const float &probabilidadCruce, 
			const float &probabilidadMutacion,const float &PLS, vector<float> &mejorSolucion);



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
		//vectores de características de forma que la mitad correspondan a los datos de
		//entrenamiento y la otra mitad a los de prueba. En caso de ser impares, el sobrante
		//será para los datos de entrenamiento
		/*************************************************************************************/
		void setParticiones();

		vector<vector<float> > getDatos();
		vector<string> getEtiquetas();
		vector<vector<int> > getParticiones();

};