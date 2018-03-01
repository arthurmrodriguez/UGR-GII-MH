/******************************************************************************
Práctica 2 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
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

	vector<int>::const_iterator indiceTrain;
	for(indiceTrain= particionTrain.begin();indiceTrain != particionTrain.end();indiceTrain++){

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
	vector<int>::const_iterator indiceTrain;

	for(indiceTrain= particionTrain.begin();indiceTrain != particionTrain.end();indiceTrain++){

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
void ConjuntoDatos::Clasificador1_NN(const vector<int> &particionTrain, const vector<int> &particionTest, 
	const vector<float> &pesos, const float &alpha, float &tasaClasificacion, float &tasaReduccion, float &agregado){

	//Variables auxiliares
	int aciertosEnClasificacion=0;
	float instanciasDeT=(1.0/particionTest.size()), distanciaActual=0.0, distanciaMin;
	string claseEPrima = etiquetas[0];

	vector<int>::const_iterator indice1,indice2;
	//Para cada vector característico del conjunto de train
	for(indice1=particionTest.begin(); indice1 != particionTest.end();indice1++){

		distanciaMin = 100;
		//Recorremos todos los vectores del conjunto de test e intentamos clasificarlo
		for(indice2=particionTrain.begin();indice2 != particionTrain.end();indice2++){
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

	tasaClasificacion = aciertosEnClasificacion*instanciasDeT;
	calcularTasaDeReduccion(pesos,tasaReduccion);
	agregado = tasaClasificacion*alpha + tasaReduccion*(1.0-alpha);
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
	vector<int>::const_iterator indice1 ;
	int i;
	//Recorremos cada vector característico y calculamos, para cada uno
	//el enemigo y el amigo mas cercano.
	//Posteriormente actualizamos el vector de pesos teniendo en cuenta las distancias
	for(indice1= particionTrain.begin(); indice1 != particionTrain.end();indice1++){

		distanciasAmigo.clear();
		distanciasEnemigo.clear();
		calcularEnemigoMasCercano(particionTrain,distanciasEnemigo,(*indice1));
		calcularAmigoMasCercano(particionTrain,distanciasAmigo,(*indice1));

		for(i=0;i< pesosRELIEF.size();i++)
			pesosRELIEF[i]+= (abs(distanciasEnemigo[i]) - abs(distanciasAmigo[i]));

	}

	//Ahora pasamos a normalizar los pesos
	float pesoMaximo = *max_element(pesosRELIEF.begin(),pesosRELIEF.end());

	for(int i=0;i< pesosRELIEF.size();i++)
		(pesosRELIEF[i] < 0)? pesosRELIEF[i]=0 : pesosRELIEF[i]= pesosRELIEF[i]/pesoMaximo;
}

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
void ConjuntoDatos::EnfriamientoSimulado(const vector<int> &particionTrain, vector<float> &solucionInicial,
	float mu, float phi, int numeroCaracteristicas){

	//Variables auxiliares para la ejecucion del algoritmo
	int maxVecinos = 10*numeroCaracteristicas, maxExitos = 0.1*maxVecinos,
	numEnfriamientosMax = MAX_EVALUACIONES/maxVecinos, 
	numEnfriamientos = 0,
	numExitosActuales = 1,
	componenteAMutar, i;

	float tActual, tFinal=0.001, coste_SActual, coste_SPrima,  costeMejorSolucion,
	tasaClas, tasaReduc,agregado, aleatorio, diferencia,
	alpha=0.5,  
	beta;

	vector<float> solucionActual, solucionSPrima, mejorSolucion;

	//Evaluacion de la solucion inicial
	Clasificador1_NN(particionTrain,particionTrain,solucionInicial,alpha,tasaClas,tasaReduc,agregado);

	//Actualizacion de soluciones
	solucionActual = solucionInicial;
	mejorSolucion = solucionActual;

	coste_SActual = agregado;
	costeMejorSolucion = coste_SActual;

	//Inicializacion de temperaturas
	tActual = (mu*agregado)/-(log(phi));
	beta = (tActual - tFinal)/(numEnfriamientosMax*tActual*tFinal);

	//Condicion de parada: menos de 15000/maxVecinos evaluaciones y al menos una solucion mejor
	//que la actual. Guardamos la mejor solucion y su coste.
	while((numEnfriamientos < numEnfriamientosMax) && (numExitosActuales > 0)){
		//Iniciamos el bucle interno 
		numExitosActuales = 0;
		
		for(i = 0; i < maxVecinos && (numExitosActuales < maxExitos);i++){

			//Seleccionamos la componente aleatoria a mutar y generamos el vecino
			solucionSPrima = solucionActual;
			componenteAMutar = rand() % numeroCaracteristicas;
			vecinoPorMutacionNormal(solucionSPrima, componenteAMutar);

			//Evaluamos la solucion actual
			Clasificador1_NN(particionTrain,particionTrain,solucionSPrima,alpha,tasaClas,tasaReduc,coste_SPrima);
			
			//Generamos el numero aleatorio que será comparado con el exponencial
			//de la diferencia negativa partido de la temperatura actual por el numero de iteracion
			aleatorio = Rand();
			diferencia = coste_SPrima - coste_SActual;

			//Condicion para actualizar la solucion actual
			if(diferencia > 0 || aleatorio <= exp((-diferencia)/(tActual*i))){

				solucionActual = solucionSPrima;
				coste_SActual = coste_SPrima;
				numExitosActuales++;

				//Actualizacion de la MEJOR SOLUCION
				if(coste_SActual > costeMejorSolucion)
					mejorSolucion = solucionActual, costeMejorSolucion = coste_SActual;

			}
		}

		//Enfriamiento de la temperatura
 		tActual = tActual / (1.0 + (beta*tActual));
		numEnfriamientos++;
 
	}
	//Devolvemos la mejor solucion
	solucionInicial = mejorSolucion;

}

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
void ConjuntoDatos::LocalSearch(const vector<int> &particionTrain, vector<float> &pesosIniciales, const int &vecinosGenerados,
	const int &maxEvals, float alpha, float &agregado){

	//Inicializamos el contador de vecinos generados y evaluaciones
	int numVecinosGenerados=0, numeroEvaluaciones=0 , componenteAMutar=-1, i;
	bool hayMejora=false;

	//Inicializamos los indices disponibles para la primera iteracion
	vector<int> indicesDisponibles(pesosIniciales.size(),0);
	iota(indicesDisponibles.begin(),indicesDisponibles.end(),0);
	
	//Solucion actual = pesos
	vector<float> solucionActual(pesosIniciales.begin(), pesosIniciales.end()),
	solucionS_Prima(pesosIniciales.begin(), pesosIniciales.end());
	//Variables auxiliares
	float clasificacion_SolActual =-10, clasificacion_SolSPrima =-10, tasaReduc, agregadoSolActual, agregadoSolSPrima;

	//Calculamos unos primeros resultados de clasificacion con la solucion actual
	Clasificador1_NN(particionTrain, particionTrain,solucionActual,alpha,
		clasificacion_SolActual,tasaReduc,agregadoSolActual);
	
	//Mientras no hayamos superado el numero maximo de evaluaciones ni
	//hayamos generado un numero maximo de vecinos
	while(numeroEvaluaciones < maxEvals && numVecinosGenerados < vecinosGenerados){

		random_shuffle(indicesDisponibles.begin(), indicesDisponibles.end());
		hayMejora=false;

		for(i=0; i < indicesDisponibles.size() && !hayMejora; i++) {

			vecinoPorMutacionNormal(solucionS_Prima,indicesDisponibles[i]);
			numVecinosGenerados++;
			//Obtenemos los resultados de la solucion vecina generada
			Clasificador1_NN(particionTrain, particionTrain,solucionS_Prima, alpha,
				clasificacion_SolSPrima, tasaReduc, agregadoSolSPrima);
			numeroEvaluaciones++;

			//Comprobamos si la solucion S_Prima tiene mejores resultados que la solucion actual
			if(agregadoSolSPrima > agregadoSolActual){

				//En caso de tener mejores resultados, actualizamos
				solucionActual = solucionS_Prima;
				agregadoSolActual = agregadoSolSPrima;
				hayMejora=true;
			}

		}

	}

	pesosIniciales = solucionActual;
	agregado = agregadoSolActual;
}


/****************************************************************************************************/
//void ILS(const vector<int> &particionTrain, vector<float> &solucion,const int &numeroCaracteristicas,
//					const int &vecinosGenerados, const int &iteraciones)
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
//							- MAX_Evaluaciones_ILS: 1000 evaluaciones de 1NN							
/***************************************************************************************************/
void ConjuntoDatos::ILS(const vector<int> &particionTrain, vector<float> &solucion,const int &numeroCaracteristicas,
	const int &vecinosGenerados, const int &iteraciones){

	/*******************************************ITERACION 1*********************************************/ 

	//Generacion y evaluacion de solucion inicial aleatoria
	float tasaSolActual, tasaSolS_Prima, tasaMejorSolucion, tasaReduc, tasaClas, alpha1NN = 0.5;
	vector<float> solucionActual,solucionS_Prima, mejorSolucion;
	generarSolucionAleatoria(solucionActual, numeroCaracteristicas);
	//Aplicamos la BL sobre la solucion actual		
	LocalSearch(particionTrain, solucionActual, vecinosGenerados, MAX_EVALUACIONES_ILS,0.5,tasaSolActual);

	solucionS_Prima = solucionActual;
	mejorSolucion = solucionActual;
	tasaMejorSolucion = 0;
		
	//El numero de mutaciones fuertes que se va a realizar es del 10% del total de caracteristicas
	int t=0.1*numeroCaracteristicas, i;

	//Inicio del algoritmos
	for(i=1; i < 15; i++){

		//Mutacion sobre la solucion SPrima: en cada caso representa la mejor
		//solucion obtenida hasta el momento
		generarMutacionILS(solucionS_Prima, t);

		//Aplicamos la BL sobre la solucion obtenida tras mutar		
		LocalSearch(particionTrain, solucionS_Prima, vecinosGenerados, MAX_EVALUACIONES_ILS,0.5,tasaSolS_Prima);

		//Criterio de aceptacion: cual de las dos soluciones es mejor entre
		//la solucion inicial o la solucion obtenida tras mutacion+LS
		if(tasaSolS_Prima < tasaSolActual)
			solucionS_Prima = solucionActual, tasaSolS_Prima = tasaSolActual;
		else
			solucionActual = solucionS_Prima, tasaSolActual = tasaSolS_Prima;

		//Comprobacion de la mejor solucion
		if(tasaSolS_Prima > tasaMejorSolucion){

			mejorSolucion = solucionS_Prima;
			tasaMejorSolucion = tasaSolS_Prima;
		}
	}
	
	//Devolvemos la mejor solucion
	solucion=mejorSolucion;

}



/**************************************************************************************************/
//void DE_Rand_1(vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
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
void ConjuntoDatos::DE_Rand_1(const vector<int> &particionTrain, const float &probCruce,  const float &factorEscalado,
	const int &numeroSoluciones,vector<float> &mejorSolucion){

	//Variables iniciales: poblaciones, evaluaciones y offspring
	vector<vector<float> > pobInicial, pobActual;
	pobInicial.reserve(numeroSoluciones), pobActual.reserve(numeroSoluciones);
	vector<float> evalsInicial, evalsActual, offspring;
	evalsInicial.reserve(numeroSoluciones), evalsActual.reserve(numeroSoluciones), offspring.reserve(datos[0].size());

	//Indices de las soluciones
	vector<int> indices(numeroSoluciones,0), indicesAux;
	iota(indices.begin(), indices.end(),0);

	float tasaClas, tasaRed, agregado, alpha1NN = 0.5,
	mejorSolucionInicial=0, mejorSolucionActual=0, recombinacion, aleatorio, jrand;
	int mejorIndiceInicial = 0, mejorIndiceActual=0, numeroEvaluaciones=0, 
	padre1, padre2, padre3, i, individuo, numeroCaracteristicas = datos[0].size();

	//Inicializacion y evaluacion de la poblacion inicial aleatoria
	generarPoblacionAleatoria(pobInicial,numeroSoluciones,datos[0].size());

	for(i = 0; i < pobInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,pobInicial[i],alpha1NN,tasaClas, tasaRed,agregado);
		evalsInicial.push_back(agregado);

		if(agregado > mejorSolucionInicial){

			mejorSolucionInicial = agregado;
			mejorIndiceInicial = i; 

		}
	}

	//Ejecucion del algoritmo
	while(numeroEvaluaciones < MAX_EVALUACIONES){

		pobActual.clear();
		evalsActual.clear();
		mejorSolucionActual = 0;
		mejorIndiceActual = 0;

		for(individuo = 0; individuo < pobInicial.size(); individuo++){

			//Seleccionamos 3 padres de forma aleatoria (obviando el padre actual)
			indicesAux = indices;
			indicesAux.erase(indicesAux.begin()+individuo);
			random_shuffle(indicesAux.begin(), indicesAux.end());

			padre1 = indicesAux[0];
			padre2 = indicesAux[1];
			padre3 = indicesAux[2];
			offspring.clear();

			//Generamos el offspring en funcion de un numero aleatorio
			//y siguiendo recombinacion binomial.
			for(i = 0; i < pobInicial[individuo].size(); i++){

				aleatorio = Rand();
				jrand = rand() % numeroCaracteristicas;
				(aleatorio <= probCruce || jrand == i)?
				recombinacion = pobInicial[padre1][i] + factorEscalado*(pobInicial[padre2][i] - pobInicial[padre3][i])
				:
				recombinacion = pobInicial[individuo][i];

				offspring.push_back(recombinacion);

			}

			//Tras terminar de generar el offspring, lo evaluamos
			//y decidimos si se quedara para la nueva generacion o no
			Clasificador1_NN(particionTrain,particionTrain,offspring,alpha1NN,tasaClas, tasaRed,agregado);
			numeroEvaluaciones++;

			//Si el hijo mejora al padre lo sustituira en la nueva poblacion
			(agregado > evalsInicial[individuo])? 
			pobActual.push_back(offspring),
			evalsActual.push_back(agregado)
			:
			pobActual.push_back(pobInicial[individuo]);
			evalsActual.push_back(evalsInicial[individuo]);
			
			//Guardamos la mejor solucion de la nueva poblacion
			if(evalsActual[individuo] > mejorSolucionActual){

				mejorSolucionActual = evalsActual[individuo];
				mejorIndiceActual = individuo;
			}

		}

		//Comparamos la mejor solucion de la generacion G con la mejor de la generacion
		//G+1, ya que esta sera la mejor solucion en general.
		if(evalsActual[mejorIndiceActual] > evalsInicial[mejorIndiceInicial]){

			mejorSolucionInicial = mejorSolucionActual;
			mejorIndiceInicial = mejorIndiceActual;
		}

		//Actualizamos la poblacion G = G+1
		pobInicial = pobActual;
		evalsInicial = evalsActual;


	}

	//Devolvemos la mejor solucion
	mejorSolucion = pobInicial[mejorSolucionInicial];
}



/**************************************************************************************************/
//void DE_CurrentToBest_1(const vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
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
void ConjuntoDatos::DE_CurrentToBest_1(const vector<int> &particionTrain, const float &probCruce, const float &factorEscalado,
	const int &numeroSoluciones,vector<float> &mejorSolucion){


	//Variables iniciales: poblaciones, evaluaciones y offspring
	vector<vector<float> > pobInicial, pobActual;
	pobInicial.reserve(numeroSoluciones), pobActual.reserve(numeroSoluciones);
	vector<float> evalsInicial, evalsActual, offspring;
	evalsInicial.reserve(numeroSoluciones), evalsActual.reserve(numeroSoluciones), offspring.reserve(datos[0].size());

	//Indices de las soluciones
	vector<int> indices(numeroSoluciones,0), indicesAux;
	iota(indices.begin(), indices.end(),0);

	float tasaClas, tasaRed, agregado, alpha1NN = 0.5,
	mejorSolucionInicial=0, mejorSolucionActual=0, recombinacion, aleatorio, jrand;
	int mejorIndiceInicial = 0, mejorIndiceActual=0, numeroEvaluaciones=0,
	padre1, padre2, i, individuo , numeroCaracteristicas = datos[0].size();

	//Inicializacion y evaluacion de la poblacion inicial aleatoria
	generarPoblacionAleatoria(pobInicial,numeroSoluciones,datos[0].size());

	for(i = 0; i < pobInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,pobInicial[i],alpha1NN,tasaClas, tasaRed,agregado);
		evalsInicial.push_back(agregado);

		if(agregado > mejorSolucionInicial){

			mejorSolucionInicial = agregado;
			mejorIndiceInicial = i; 

		}
	}

	//Ejecucion del algoritmo
	while(numeroEvaluaciones < MAX_EVALUACIONES){

		pobActual.clear();
		evalsActual.clear();
		mejorSolucionActual = 0;
		mejorIndiceActual = 0;

		for(individuo = 0; individuo < pobInicial.size(); individuo++){

			//Seleccionamos 3 padres de forma aleatoria (obviando el padre actual)
			indicesAux = indices;
			indicesAux.erase(indicesAux.begin()+individuo);
			random_shuffle(indicesAux.begin(), indicesAux.end());

			padre1 = indicesAux[0];
			padre2 = indicesAux[1];
			offspring.clear();

			//Generamos el offspring en funcion de un numero aleatorio
			//y siguiendo recombinacion binomial.
			for(i = 0; i < pobInicial[individuo].size(); i++){

				aleatorio = Rand();
				jrand = rand() % numeroCaracteristicas;

				if(aleatorio < probCruce || jrand == i)
					recombinacion = pobInicial[individuo][i] + 
				factorEscalado*(pobInicial[mejorIndiceActual][i]- pobInicial[individuo][i])+
				factorEscalado*(pobInicial[padre1][i] - pobInicial[padre2][i]);
				
				else
					recombinacion = pobInicial[individuo][i];

				offspring.push_back(recombinacion);

			}

			//Tras terminar de generar el offspring, lo evaluamos
			//y decidimos si se quedara para la nueva generacion o no
			Clasificador1_NN(particionTrain,particionTrain,offspring,alpha1NN,tasaClas, tasaRed,agregado);
			numeroEvaluaciones++;

			//Si el hijo mejora al padre lo sustituira en la nueva poblacion
			(agregado > evalsInicial[individuo])?

			pobActual.push_back(offspring),
			evalsActual.push_back(agregado)
			:
			pobActual.push_back(pobInicial[individuo]);
			evalsActual.push_back(evalsInicial[individuo]);

			//Guardamos la mejor solucion de la nueva poblacion
			if(evalsActual[individuo] > mejorSolucionActual){

				mejorSolucionActual = evalsActual[individuo];
				mejorIndiceActual = individuo;
			}

		}

		//Comparamos la mejor solucion de la generacion G con la mejor de la generacion
		//G+1, ya que esta sera la mejor solucion en general.
		if(evalsActual[mejorIndiceActual] > evalsInicial[mejorIndiceInicial]){

			mejorSolucionInicial = mejorSolucionActual;
			mejorIndiceInicial = mejorIndiceActual;
		}

		//Actualizamos la poblacion G = G+1
		pobInicial = pobActual;
		evalsInicial = evalsActual;


	}

	//Devolvemos la mejor solucion
	mejorSolucion = pobInicial[mejorSolucionInicial];

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

	int i;

	//Generamos los hijos teniendo en cuenta la generacion aleatoria
	//descrita anteriormente
	for(i=0;i < Padre1.size();i++){

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
	int padreAleatorio1 = -1,padreAleatorio2 = -1, tamanioPoblacion=evaluacionesDeCromosomas.size(), i;
	float evaluacionPadre1 = -1, evaluacionPadre2 = -1;
	//Segun el numero de evaluaciones (padres) disponibles
	//seleccionamos las N/2 parejas para cruzas
	for(i=0; i< numeroDePadres; i++){

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
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1, i,j;

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0, tasaClas, tasaRed, alpha1NN=0.5;

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

	for(i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i],alpha1NN, tasaClas,tasaRed, evaluacionesDeCromosomas[i]);
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
	while(numeroDeEvaluaciones < MAX_EVALUACIONES){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDeCromosomasAGG,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 15 parejas*<probabilidadCruce>
		//En este caso se generan 10,5 cruces, redondeando al alza, 11 cruces
		//Los cruces se realizan de 1 en 1.
		mejorPadrePerdido = true;

		for(i=0,j=0; i<numeroDeCruces;i++,j+=2){

			BLX_Alpha(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente1,Descendiente2,alpha);

			poblacionGenerada.push_back(Descendiente1);
			poblacionGenerada.push_back(Descendiente2);
		}

		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(i= poblacionGenerada.size();i<numeroDeCromosomasAGG;i++){

			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(i=0;i <numeroDeMutaciones;i++){

			cromosomaAleatorioAMutar = rand() % numeroDeCromosomasAGG;
			genAleatorioAMutar = rand() % numeroDeGenes;
			vecinoPorMutacionNormal(poblacionGenerada[cromosomaAleatorioAMutar],genAleatorioAMutar);
		}

		mejorCromosoma = -1.0, peorCromosoma = 100.0;

		//Reiniciamos los valores a 0
		evaluacionesDeCromosomasGenerados.assign(poblacionInicial.size(),0.0);
		//Calculamos las nuevas tasas de clasificacion
		for(i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], alpha1NN, tasaClas, tasaRed, 
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
	cromosomaAleatorioAMutar=-1 ,genAleatorioAMutar = -1, cromosomaAleatorio, maxEvalsLS = 15000, i,j;

	//Reservamos espacio para los PLS*numeroDeCromosomasAM cromosomas
	//que se les aplicará la Local Search y generamos un vector de indices desde
	//0 hasta el numero de cromosomas que nos servirá para elegir el numero 
	//de cromosomas que participarán en la Local Search
	vector<int> indicesDeCromosomas(numeroDeCromosomasAM,0.0);
	iota(indicesDeCromosomas.begin(), indicesDeCromosomas.end(),0.0);

	//Variables aleatorias
	bool mejorPadrePerdido = true;
	float mejorCromosoma = -1.0, peorCromosoma=100.0,mejorTasa, alpha1NN = 0.5, tasaClas, tasaRed;

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

	for(i=0; i < poblacionInicial.size();i++){

		Clasificador1_NN(particionTrain,particionTrain,poblacionInicial[i],alpha1NN, 
			tasaClas,tasaRed, evaluacionesDeCromosomas[i]);
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
	while(numeroDeEvaluaciones < MAX_EVALUACIONES){

		//Seleccion: Lanzamos el torneo binario
		mejoresPadres.clear();
		TorneoBinario(evaluacionesDeCromosomas,numeroDePadres,mejoresPadres);

		//Cruce de los mejores padres: seguimos la esperanza matemática
		//de la cantidad de padres a cruzar -> 30 parejas*<probabilidadCruce>
		//En este caso se generan 21 cruces. Los cruces se realizan de 1 en 1.
		for(i=0,j=0; i<numeroDeCruces;i++,j+=2){

			Descendiente.clear();

			CA(poblacionInicial[mejoresPadres[j]],poblacionInicial[mejoresPadres[j+1]],
				Descendiente);

			poblacionGenerada.push_back(Descendiente);
		}

		mejorPadrePerdido = true;
		//Ahora terminamos de rellenar la poblacion generada con los padres que no se han cruzado
		//Estos padres serán los componentes de la generacion t+1. En caso de que el mejor padre
		//se haya perdido, sustituiremos el peor de la solucion generada
		for(i= poblacionGenerada.size();i<numeroDeCromosomasAM;i++){

			//Es i*2 dado que es el siguiente padre al ultimo que se cruzó con otro
			poblacionGenerada.push_back(poblacionInicial[mejoresPadres[i*2]]);

			if(mejoresPadres[i] == mejorPadreGeneracionInicial)
				mejorPadrePerdido=false;

		}

		//Una vez hemos terminado de recombinar y obtenemos la nueva generacion, mutamos los genes
		//que se requieran
		for(i=0;i <numeroDeMutaciones;i++){

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
		for(i=0; i < poblacionGenerada.size();i++){

			Clasificador1_NN(particionTrain,particionTrain,poblacionGenerada[i], alpha1NN, tasaClas, tasaRed,
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
			for(i=0;i < cromosomasParaLS;i++){

				LocalSearch(particionTrain, poblacionGenerada[iter->second], numeroDeVecinos,maxEvalsLS,alpha1NN,mejorTasa);

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
//vectores de características y partirá el conjunto de datos cada 20% para crear las 5 
//particiones. Cuando realicemos los calculos, 4 de estas particiones se usaran para
//entrenar y la otra para validar.
/*************************************************************************************/
void ConjuntoDatos::setParticiones(){

	//Creamos el vector de indices: desde 0 hasta el tamaño de la matriz de datos - 1
	srand(1680851);
	vector<int> indicesDeParticiones(this->datos.size(),0);
	iota(indicesDeParticiones.begin(),indicesDeParticiones.end(),0);
	random_shuffle(indicesDeParticiones.begin(),indicesDeParticiones.end());

	int aumentoIndices = (indicesDeParticiones.size())*0.2;
	vector<int>::iterator indiceTope = indicesDeParticiones.begin();
	vector<int> particionParcial;
	//Generamos las 4 primeras particiones de los datos (20% cada particion)
	for(int i=0;i<4;i++){

		particionParcial.assign(indiceTope,indiceTope+aumentoIndices);
		particiones.push_back(particionParcial);
		indiceTope+=aumentoIndices;
		
	}

	//La ultima particion puede que tenga mas datos (4 a lo sumo)
	particionParcial.assign(indiceTope,indicesDeParticiones.end());
	particiones.push_back(particionParcial);
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