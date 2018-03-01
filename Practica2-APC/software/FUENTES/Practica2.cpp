/******************************************************************************
Práctica 2 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
Arthur Mickael Rodríguez Nesterenko 
DNI: Y1680851W
Profesor: Salvador García
Grupo de Computación y Sistemas Inteligentes
Fichero que contiene la función Main: ejecuta el programa principal
*****************************************************************************/

//Librerias para I/O y lectura de ficheros
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "ConjuntoDatos.h"

//Espacio de nombres estándar
using namespace std;
using namespace std::chrono;

int main(int agrc, char* argv[]) {

	//Declaracion de las variables a utilizar por el programa
	vector<vector<float> > vectorDeCaracteristicas;
	vector<string> clasificaciones;
	string ficheroACargar, algoritmoAEjecutar;

	//Parámetros que se leen desde ficheros
	float alpha, alpha1NN, probabilidadCruce, probabilidadMutacion, PLS;
	int semillaAleatoria, numeroDeCromosomasAGG,numeroDeCromosomasAM, indiceTrain;
	//Fin parametros que se leen desde ficheros

	int vecinosGenerados;
	float  mejorTasa, tasaClas, tasaReduccion, agregado, tiempoEjecucion;
	high_resolution_clock::time_point tiempoInicio, tiempoFin;
	duration<double> tiempoTotal;
	bool recoleccionDatos=false;

	cout<<"******************************************************************************************" <<endl;
	cout<<"                               Practica 2 - Metaheurísticas"<<endl;
	cout<<"******************************************************************************************" <<endl;
	cout<<"Seleccione la base de datos que desea cargar: "<<endl;
	cout<<"Fichero 1: sonar.arff"<<endl;
	cout<<"Fichero 2: spambase-460.arff" <<endl;
	cout<<"Fichero 3: wdbc.arff"<<endl;
	cout<<"Fichero -> ";
	cin >> ficheroACargar;
	cout<<endl <<endl;
	cout<<"Cargando " <<ficheroACargar <<"..." <<endl;

	if(stoi(ficheroACargar) == 1)
		leerDatosFicheroARFF_Formato1("sonar.arff",vectorDeCaracteristicas,clasificaciones);


	else if(stoi(ficheroACargar)==2)
		leerDatosFicheroARFF_Formato1("spambase-460.arff",vectorDeCaracteristicas,clasificaciones);
		

	else
		leerDatosFicheroARFF_Formato2("wdbc.arff",vectorDeCaracteristicas,clasificaciones);


	cout<<ficheroACargar <<" cargado." <<endl <<endl;
	cout<<"La base de datos " <<ficheroACargar <<" contiene " <<vectorDeCaracteristicas.size() <<" ejemplos con "
	<<vectorDeCaracteristicas[0].size() <<" caracterísicas." <<endl <<endl;

	//Tras cargar los datos, leemos los parámetros desde fichero (argumento de nuestro programa) y
	//creamos el objeto ConjuntoDatos que nos servirá para ejecutar nuestros algoritmos
	leerParametrosAlgoritmos(argv[1],alpha,alpha1NN,probabilidadCruce, probabilidadMutacion,PLS,
		semillaAleatoria,numeroDeCromosomasAGG,numeroDeCromosomasAM);

	ConjuntoDatos baseDeDatos(vectorDeCaracteristicas,clasificaciones,numeroDeCromosomasAGG,numeroDeCromosomasAM);
	baseDeDatos.setParticiones();

	cout<<"Seleccione el algoritmo que desea ejecutar: "<<endl;
	cout<<"Algoritmo 1: 1-NN"<<endl;
	cout<<"Algoritmo 2: RELIEF" <<endl;
	cout<<"Algoritmo 3: ES"<<endl;
	cout<<"Algoritmo 4: ILS"<<endl;
	cout<<"Algoritmo 5: DE_Rand_1"<<endl;
	cout<<"Algoritmo 6: DE_CurrentToBest_1"<<endl;
	cout<<"Algoritmo 7: LS"<<endl;
	cout<<"Algoritmo 8: AGG-BLX"<<endl;
	cout<<"Algoritmo 9: AM-(10,0.1mej)"<<endl;
	
	cout<<"Algoritmo -> ";
	cin>>algoritmoAEjecutar;

	if(argv[2] != NULL){
		cout<<"Se ha indicado formato de salida para recoleccion de datos" <<endl;
		recoleccionDatos = true;
	}
		
	
	//Inicializamos la semilla aleatoria y comienza la ejecucion del programa
	srand(semillaAleatoria);
	
	int numeroCaracteristicas = vectorDeCaracteristicas[0].size();
	//Variables generales de cada algoritmo
	float clasificacionMedia=0.0, reduccionMedia=0.0, agregadoMedio=0.0, tiempoMedio=0.0, mu=0.3 , phi = 0.3;
	vector<float> pesosUnitarios(numeroCaracteristicas,1.0),
	pesosRELIEF(numeroCaracteristicas,0.0),solucionInicial,mejorSolucion,
	pesosInicialesLS;

	vector<vector<int> > particionesDatos = baseDeDatos.getParticiones();
	vector<int> particionTrain, particionTest;

	float probCruceDE = 0.5, factorEscalado = 0.5;
	int numSoluciones = 50;
	
	switch(stoi(algoritmoAEjecutar)){

		//1NN
		case 1:

		cout<<"_____________________________________________1-NN______________________________________________"<<endl <<endl;


		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}
		
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosUnitarios,alpha1NN,tasaClas,tasaReduccion,agregado);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;
			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		1-NN Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		1-NN Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	1-NN Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	1-NN Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}
		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media 1NN " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media 1NN " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) medio 1NN " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio 1NN " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  1NN " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		
		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;


		//RELIEF
		case 2:
	
		cout<<"______________________________________________RELIEF__________________________________________________" <<endl <<endl;


		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.Greedy_RELIEF(particionTrain,pesosRELIEF);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosRELIEF,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		RELIEF Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		RELIEF Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	RELIEF Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	RELIEF Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media RELIEF " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media RELIEF " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media RELIEF " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio RELIEF " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  RELIEF " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;



		//ES
		case 3:
	
		cout<<"______________________________________________ES__________________________________________________" <<endl <<endl;


		//Generamos la solucion inicial aleatoria
		solucionInicial.reserve(numeroCaracteristicas);
		//Generacion de la solucion aleatoria y evaluacion
		generarSolucionAleatoria(solucionInicial, numeroCaracteristicas);

		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.EnfriamientoSimulado(particionTrain,solucionInicial, mu,phi, numeroCaracteristicas);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		ES Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		ES Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	ES Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	ES Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media ES " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media ES " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media ES " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio ES " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  ES " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;



		//ILS
		case 4:
	
		cout<<"______________________________________________ILS__________________________________________________" <<endl <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.ILS(particionTrain,solucionInicial,numeroCaracteristicas,20*numeroCaracteristicas, 15);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		ILS Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		ILS Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	ILS Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	ILS Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media ILS " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media ILS " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media ILS " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio ILS " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  ILS " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;


		//DE_Rand_1
		case 5:
	
		cout<<"______________________________________________DE_Rand_1__________________________________________________" <<endl <<endl;

		
		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.DE_Rand_1(particionTrain,probCruceDE,factorEscalado,numSoluciones, solucionInicial);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		DE_Rand_1 Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		DE_Rand_1 Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	DE_Rand_1 Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	DE_Rand_1 Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media DE_Rand_1 " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media DE_Rand_1 " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media DE_Rand_1 " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio DE_Rand_1 " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  DE_Rand_1 " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;


		//DE_CurrentToBest_1
		case 6:
	
		cout<<"______________________________________________DE_CurrentToBest_1__________________________________________________" <<endl <<endl;

		
		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.DE_CurrentToBest_1(particionTrain,probCruceDE,factorEscalado,numSoluciones, solucionInicial);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		DE_CurrentToBest_1 Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		DE_CurrentToBest_1 Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	DE_CurrentToBest_1 Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	DE_CurrentToBest_1 Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media DE_CurrentToBest_1 " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media DE_CurrentToBest_1 " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media DE_CurrentToBest_1 " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio DE_CurrentToBest_1 " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  DE_CurrentToBest_1 " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;



		//Local Search
		case 7:
	
		cout<<"______________________________________________Local Search__________________________________________________" <<endl <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			pesosInicialesLS.clear();
			generarSolucionAleatoria(pesosInicialesLS, numeroCaracteristicas);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.LocalSearch(particionTrain, pesosInicialesLS, 20*numeroCaracteristicas,15000,alpha1NN, agregado);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosInicialesLS,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		LS Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		LS Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	LS Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	LS Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media LS " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media LS " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media LS " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio LS " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  LS " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;


		//AGG-BLX
		case 8:
	
		cout<<"______________________________________________AGG-BLX__________________________________________________" <<endl <<endl;

		
		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.AGG_BLX(particionTrain, alpha, probabilidadCruce, probabilidadMutacion, solucionInicial);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		AGG-BLX Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		AGG-BLX Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	AGG-BLX Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	AGG-BLX Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media AGG-BLX " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media AGG-BLX " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media AGG-BLX " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio AGG-BLX " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  AGG-BLX " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;


		//AM-(10,0.1Mej)
		case 9:
	
		cout<<"______________________________________________AM-(10,0.1Mej)__________________________________________________" <<endl <<endl;

		
		for(int i =0;i < particionesDatos.size();i++){

			particionTrain.clear();
			particionTest.clear();
			for(indiceTrain=0;indiceTrain<particionesDatos.size();indiceTrain++){

				if(indiceTrain != i){
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTrain.push_back(particionesDatos[indiceTrain][j]);

				}
					
				else{
					for(int j = 0; j < particionesDatos[indiceTrain].size();j++)
						particionTest.push_back(particionesDatos[indiceTrain][j]);
				}
			}

			
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
			baseDeDatos.AM_CA_Mejores(particionTrain, probabilidadCruce, probabilidadMutacion,PLS, solucionInicial);
			tiempoFin = high_resolution_clock::now();

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();
			
			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,solucionInicial,alpha1NN,tasaClas,tasaReduccion,agregado);

			clasificacionMedia+=tasaClas;
			reduccionMedia+=tasaReduccion;
			agregadoMedio += agregado;
			tiempoMedio+=tiempoEjecucion;

			
			if(!recoleccionDatos){
				cout<<"Tasa de Clasificacion 		AM-(10,0.1Mej) Particion " <<i+1 <<" " <<tasaClas*100 <<'%'<<endl;
				cout<<"Tasa de Reduccion 		AM-(10,0.1Mej) Particion " <<i+1 <<" " <<tasaReduccion*100 <<'%'<<endl;
				cout<<"Funcion Objetivo (Agregado) 	AM-(10,0.1Mej) Particion " <<i+1 <<" " <<agregado*100 <<'%'<<endl;
				cout<<"Tiempo de ejecucion      	AM-(10,0.1Mej) Particion " <<i+1 <<" " <<tiempoEjecucion <<"s" <<endl;
				cout <<endl <<endl;
			}

			else{

				cout<<tasaClas*100 <<" " <<tasaReduccion*100 <<" " <<agregado*100 <<" " <<tiempoEjecucion <<endl;
			}

		}

		cout<<endl;
		if(!recoleccionDatos){
			cout<<"Tasa de clasificacion       media AM-(10,0.1Mej) " <<(clasificacionMedia/5)*100 <<endl;
			cout<<"Tasa de reduccion           media AM-(10,0.1Mej) " <<(reduccionMedia/5)*100 <<endl;
			cout<<"Funcion Objetivo (Agregado) media AM-(10,0.1Mej) " <<(agregadoMedio/5)*100 <<endl;
			cout<<"Tiempo de ejecucion         medio AM-(10,0.1Mej) " <<tiempoMedio/5 <<endl;
			cout<<"Tiempo total 			  AM-(10,0.1Mej) " <<tiempoMedio <<endl;
		}

		else{

			cout<<(clasificacionMedia/5)*100 <<" " <<(reduccionMedia/5)*100 <<" " <<(agregadoMedio/5)*100 <<" "
			<<tiempoMedio/5 <<endl;

		}

		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;

	}


	return 0;
}
