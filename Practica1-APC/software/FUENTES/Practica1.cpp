/******************************************************************************
Práctica 1 Metaheurísticas(MH): APC (Asignación de Pesos a Características)
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
	float alpha, probabilidadCruce, probabilidadMutacion, PLS;
	int semillaAleatoria, numeroDeCromosomasAGG,numeroDeCromosomasAM;
	//Fin parametros que se leen desde ficheros

	int vecinosGenerados;
	float  mejorTasa, tasaClas, tiempoEjecucion;
	high_resolution_clock::time_point tiempoInicio, tiempoFin;
	duration<double> tiempoTotal;

	cout<<"******************************************************************************************" <<endl;
	cout<<"                               Practica 1 - Metaheurísticas"<<endl;
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
	leerParametrosAlgoritmos(argv[1],alpha,probabilidadCruce, probabilidadMutacion,PLS,
		semillaAleatoria,numeroDeCromosomasAGG,numeroDeCromosomasAM);

	ConjuntoDatos baseDeDatos(vectorDeCaracteristicas,clasificaciones,numeroDeCromosomasAGG,numeroDeCromosomasAM);
	baseDeDatos.setParticiones();
	vector<vector<int> > particionesDatos = baseDeDatos.getParticiones();
	vector<float> pesosUnitarios(particionesDatos[0].size(),1.0),mejorSolucion;

	cout<<"Seleccione el algoritmo que desea ejecutar: "<<endl;
	cout<<"Algoritmo 1: 1-NN"<<endl;
	cout<<"Algoritmo 2: RELIEF" <<endl;
	cout<<"Algoritmo 3: BL"<<endl;
	cout<<"Algoritmo 4: AGG-BLX"<<endl;
	cout<<"Algoritmo 5: AGG-CA"<<endl;
	cout<<"Algoritmo 6: AGE-BLX"<<endl;
	cout<<"Algoritmo 7: AGE-CA"<<endl;
	cout<<"Algoritmo 8: AM-(10,1.0)"<<endl;
	cout<<"Algoritmo 9: AM-(10,0.1)"<<endl;
	cout<<"Algoritmo 10: AM-(10,0.1mej)"<<endl;
	cout<<"Algoritmo -> ";
	cin>>algoritmoAEjecutar;

	//Inicializamos la semilla aleatoria y comienza la ejecucion del programa
	srand(semillaAleatoria);
	vector<int> particionTrain,particionTest;
	vector<int>::iterator puntoMedio;

	//Variables generales de cada algoritmo
	float clasificacionMedia=0.0, tiempoMedio=0.0;
	vector<float> pesosRELIEF(vectorDeCaracteristicas[0].size(),0.0);
	vector<float> pesosIniciales_LS,pesosIniciales_LSAux;

	switch(stoi(algoritmoAEjecutar)){

		//1NN
		case 1:

		cout<<"_____________________________________________1-NN______________________________________________"<<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Train frente a test de la particion i-esima

				//Asignamos la particion de train y la de test
				particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
				particionTest.assign(puntoMedio,particionesDatos[i].end());
		
				tiempoInicio=high_resolution_clock::now();
					baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosUnitarios,tasaClas);
				tiempoFin = high_resolution_clock::now();

				tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
				tiempoEjecucion=tiempoTotal.count();

				clasificacionMedia+=tasaClas;
				tiempoMedio+=tiempoEjecucion;

				cout<<"Porcentaje clasificacion 1-NN Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%'<<endl;
				cout<<"Tiempo de ejecucion      1-NN Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

				cout<<endl;
			//Test frente a train de la particion i-esima

				particionTrain.assign(puntoMedio,particionesDatos[i].end());
				particionTest.assign(particionesDatos[i].begin(),puntoMedio);

				tiempoInicio=high_resolution_clock::now();
					baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosUnitarios,tasaClas);
				tiempoFin = high_resolution_clock::now();

				tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
				tiempoEjecucion=tiempoTotal.count();

				clasificacionMedia+=tasaClas;
				tiempoMedio+=tiempoEjecucion;

				cout<<"Porcentaje clasificacion 1-NN Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
				cout<<"Tiempo de ejecucion      1-NN Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion <<"s" <<endl;

			
				cout<<endl <<endl;

		}
				cout<<"Tasa de clasificacion media 1NN " <<clasificacionMedia/10 <<endl;
				cout<<"Tiempo de ejecucion medio   1NN " <<tiempoMedio/10 <<endl;
		
		cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;

		//RELIEF
		case 2:
	
		cout<<"______________________________________________RELIEF__________________________________________________" <<endl;


		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
				particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
				particionTest.assign(puntoMedio,particionesDatos[i].end());
			
				//Ejecutamos el algoritmo y contabilizamos el momento de inicio
				tiempoInicio=high_resolution_clock::now();
					baseDeDatos.Greedy_RELIEF(particionTrain,pesosRELIEF);
				tiempoFin = high_resolution_clock::now();

				//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
				baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosRELIEF,tasaClas);

				tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
				tiempoEjecucion=tiempoTotal.count();

				clasificacionMedia+=tasaClas;
				tiempoMedio+=tiempoEjecucion;

				cout<<"Porcentaje clasificacion RELIEF Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
				cout<<"Tiempo de ejecucion      RELIEF Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

				cout<<endl;
				//Test frente a train de la particion i-esima

				particionTrain.assign(puntoMedio,particionesDatos[i].end());
				particionTest.assign(particionesDatos[i].begin(),puntoMedio);

				//Ejecutamos el algoritmo y contabilizamos el momento de inicio
				tiempoInicio=high_resolution_clock::now();
					baseDeDatos.Greedy_RELIEF(particionTrain,pesosRELIEF);
				tiempoFin = high_resolution_clock::now();

				//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
				baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosRELIEF,tasaClas);

				tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
				tiempoEjecucion=tiempoTotal.count();

				clasificacionMedia+=tasaClas;
				tiempoMedio+=tiempoEjecucion;

				cout<<"Porcentaje clasificacion RELIEF Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
				cout<<"Tiempo de ejecucion      RELIEF Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion <<"s" <<endl;			cout<<endl <<endl;

		}
				cout<<"Tasa de clasificacion media RELIEF " <<clasificacionMedia/10 <<endl;
				cout<<"Tiempo de ejecicion medio   RELIEF " <<tiempoMedio/10 <<endl;
		
				cout<<"_____________________________________________________________________________________________________________" <<endl;
		
		break;

		//BL
		case 3:
	
		cout<<"______________________________________________BL____________________________________________________" <<endl;
	
		//Calculamos un vector de pesos inicial que sirva como punto de partida para la LS
		for(int i= 0; i< vectorDeCaracteristicas[0].size();i++){

			float pesoAleatorio = ((rand() %10000)/10000.0);
			pesosIniciales_LS.push_back(pesoAleatorio);

		}
		pesosIniciales_LSAux = pesosIniciales_LS;
		
		vecinosGenerados = 20*(vectorDeCaracteristicas[0].size());
	
		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.LocalSearch(particionTrain,vecinosGenerados,pesosIniciales_LSAux,mejorTasa);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosIniciales_LSAux,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion LS Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      LS Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima
			pesosIniciales_LSAux = pesosIniciales_LS;
			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.LocalSearch(particionTrain,vecinosGenerados,pesosIniciales_LSAux,mejorTasa);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,pesosIniciales_LSAux,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion LS Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      LS Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media LS " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   LS " <<tiempoMedio/10 <<endl;
	
			cout<<"_____________________________________________________________________________________________________________" <<endl;

		break;

		//AGG-BLX
		case 4:
	
		cout<<"______________________________________________AGG-BLX________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGG_BLX(particionTrain,alpha,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGG-BLX Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGG-BLX Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGG_BLX(particionTrain,alpha,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGG-BLX Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGG-BLX Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AGG-BLX " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   AGG-BLX " <<tiempoMedio/10 <<endl;

		break;

		//AGG-CA
		case 5:

		cout<<"______________________________________________AGG-CA________________________________________________________" <<endl;

		

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGG_CA(particionTrain,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGG-CA Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGG-CA Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGG_CA(particionTrain,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGG-CA Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGG-CA Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AGG-CA " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   AGG-CA " <<tiempoMedio/10 <<endl;

		break;

		//AGE-BLX
		case 6:

		cout<<"______________________________________________AGE-BLX________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGE_BLX(particionTrain,alpha,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGE-BLX Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGE-BLX Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGE_BLX(particionTrain,alpha,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGE-BLX Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGE-BLX Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AGE-BLX " <<clasificacionMedia/10<<endl;
			cout<<"Tiempo de ejecucion medio   AGE-BLX " <<tiempoMedio/10 <<endl;

		break;

		//AGE-CA
		case 7:

		cout<<"______________________________________________AGE-CA________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGE_CA(particionTrain,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGE-CA Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGE-CA Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AGE_CA(particionTrain,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AGE-CA Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AGE-CA Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AGG-CA " <<clasificacionMedia/10<<endl;
			cout<<"Tiempo de ejecucion medio   AGG-CA " <<tiempoMedio/10 <<endl;

		break;

		//AM-(10,1.0)
		case 8:

		cout<<"______________________________________________AM-(10,1.0)________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_Total(particionTrain,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,1.0) Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,1.0) Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_Total(particionTrain,probabilidadCruce,probabilidadMutacion,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,1.0) Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,1.0) Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AM-(10,1.0) " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   AM-(10,1.0) " <<tiempoMedio/10 <<endl;

		break;

		//AM-(10,0.1)
		case 9:

		cout<<"______________________________________________AM-(10,0.1)________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_PLS(particionTrain,probabilidadCruce,probabilidadMutacion,PLS,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,0.1) Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,0.1) Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_PLS(particionTrain,probabilidadCruce,probabilidadMutacion,PLS,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,0.1) Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,0.1) Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AM-(10,0.1) " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   AM-(10,0.1) " <<tiempoMedio/10 <<endl;

		break;

		//AM-(10,0.1Mej)
		case 10:

		cout<<"______________________________________________AM-CA-(10,0.1Mej)________________________________________________________" <<endl;

		for(int i =0;i < particionesDatos.size();i++){

			puntoMedio = particionesDatos[i].begin() + particionesDatos[i].size()/2;
			//Asignamos la particion de train y la de test
			particionTrain.assign(particionesDatos[i].begin(),puntoMedio);
			particionTest.assign(puntoMedio,particionesDatos[i].end());
		
			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_Mejores(particionTrain,probabilidadCruce,probabilidadMutacion,PLS,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);
			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,0.1Mej) Particion " <<i+1 <<"-" <<"1 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,0.1Mej) Particion " <<i+1 <<"-" <<"1 ------> " <<tiempoEjecucion <<"s" <<endl;

			cout<<endl;
			//Test frente a train de la particion i-esima

			particionTrain.assign(puntoMedio,particionesDatos[i].end());
			particionTest.assign(particionesDatos[i].begin(),puntoMedio);

			//Ejecutamos el algoritmo y contabilizamos el momento de inicio
			tiempoInicio=high_resolution_clock::now();
				baseDeDatos.AM_CA_Mejores(particionTrain,probabilidadCruce,probabilidadMutacion,PLS,mejorSolucion);
			tiempoFin = high_resolution_clock::now();

			//Llamamos al clasificador 1NN para obtener la tasa de clasificacion
			baseDeDatos.Clasificador1_NN(particionTrain,particionTest,mejorSolucion,tasaClas);

			tiempoTotal = duration_cast<duration<double>>(tiempoFin - tiempoInicio);
			tiempoEjecucion=tiempoTotal.count();

			clasificacionMedia+=tasaClas;
			tiempoMedio+=tiempoEjecucion;

			cout<<"Porcentaje clasificacion AM-(10,0.1Mej) Particion " <<i+1 <<"-" <<"2 ------> " <<tasaClas <<'%' <<endl;
			cout<<"Tiempo de ejecucion      AM-(10,0.1Mej) Particion " <<i+1 <<"-" <<"2 ------> " <<tiempoEjecucion << "s" <<endl;
		
			cout<<endl <<endl;


		}
			cout<<"Tasa de clasificacion media AM-(10,0.1Mej) " <<clasificacionMedia/10 <<endl;
			cout<<"Tiempo de ejecucion medio   AM-(10,0.1Mej) " <<tiempoMedio/10 <<endl;

		break;

	}


		return 0;
}
