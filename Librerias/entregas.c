//Librerias de C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Librerias creadas por nosotros
#include "TDAs/TDA_Mapa/hashmap.h"
#include "TDAs/TDA_Lista/list.h"
#include "Estructuras/structs.h"
#include "grafos.h"
#include "Interfaz/interfaz.h"

//Funcion para mostrar la información dentro del tipoRuta
void mostrarRuta(tipoRuta * rutaCreada)
{
	printf(green"\nRuta %s\nRecorrido: ", rutaCreada->nombreRuta);
	for(int k = 0; k < rutaCreada->largo; k++)
	{
		printf(blue"%i ", rutaCreada->arreglo[k]->posicion->identificacion);
	}
	printf(green"\nDistancia Total Recorrida:"blue" %.2lf\n"reset, rutaCreada->distanciaTotal);
}

//Funcion para leer la informacion del .txt y almacenar en una variable tipoCoordenadas
tipoCoordenadas * lecturaDeInformacion(char * lineaLeida, int id)
{
	tipoCoordenadas * nuevaPosicion = crearTipoCoordenadas();
	char * fragmento;

	//Posicion X
	fragmento = strtok(lineaLeida, " ");
	nuevaPosicion->coordenadaX = strtol(fragmento, NULL, 10);

	//Posicion Y
	fragmento = strtok(NULL, "\n");
	nuevaPosicion->coordenadaY = strtol(fragmento, NULL, 10);

	//ID
	nuevaPosicion->identificacion = id;
	return nuevaPosicion;
}

//Funcion para validar que la posicion exista (Funcion 2 y 3)
tipoCoordenadas * busquedaPosicion(HashMap * mapaIdentificacion, int identificacion)
{
	tipoCoordenadas * posicionBuscada = searchMap(mapaIdentificacion, &identificacion);
    if(posicionBuscada == NULL)
    {
        printf(red"\nNo existe la entrega\n"reset);
        return NULL;
    }
	return posicionBuscada;
}

//Funcion para verficar que en el mapa no se encuentren nombres repetidos
short nombreRepetido(HashMap * mapaRutas, char * nombreRuta)
{
	tipoRuta * aux = searchMap(mapaRutas, nombreRuta);
	if(aux != NULL)
	{
		printf(red"\nEl nombre ya se encuentra usado, use otro\n"reset);
		return 0;
	}
	return 1;
}

void importarArchivo(HashMap * mapaIdentificacion)
{
	//Se ingresa el nombre del archivo a leer
	char nombreArchivo[50];
	printf("\nIngrese el nombre del archivo a importar: ");
	getchar();
	scanf("%49[^\n]s", nombreArchivo);

	//Se busca el archivo
	FILE * archivo = fopen(nombreArchivo, "r");
	if(archivo == NULL)
	{
		printf(red "\nNo se encontro el archivo!\n" reset);
		return;
	}

	printf(green "\nSe encontro el archivo!\n" reset);

	//Se ingresa la cantidad de lineas a leer
	int cantLineas;
	printf("\nIngrese la cantidad de lineas a leer: ");
	scanf("%i", &cantLineas);

	//Si se piden 0 lineas, se termina
	if(cantLineas == 0)
	{
		printf(red "\nNo se leyo ninguna linea!\n" reset);
		fclose(archivo);
		return;
	}

	char lineaLeida[100];
	int cont = 0;
	
	//Lectura de las lineas
	while(cont != cantLineas && fgets(lineaLeida, 100, archivo) != NULL)
	{
		tipoCoordenadas * nuevaPosicion = lecturaDeInformacion(lineaLeida, cont + 1);
		insertMap(mapaIdentificacion, &nuevaPosicion->identificacion, nuevaPosicion);
		cont++;
	}

	//Si hay menos lineas de la pedida, se indica el total leido
	if(cont < cantLineas)
	{
		printf(blue"\nSe llego al final del archivo\n");
		printf("Se leyeron unicamente %i lineas!\n"reset, cont);
	}

	printf(green "\nEl archivo se importo correctamente!\n" reset);
	fclose(archivo);
}

void distanciaEntregas(HashMap * mapaIdentificacion)
{
	int identificacion1, identificacion2;

	//Se leen las dos entregas a usar
	printf("\nIngrese el numero de identificacion de la entrega 1: ");
	scanf("%i", &identificacion1);
	tipoCoordenadas * entrega1 = busquedaPosicion(mapaIdentificacion, identificacion1);
	if(entrega1 == NULL) return;

	printf("\nIngrese el numero de identificacion de la entrega 2: ");
	scanf("%i", &identificacion2);
	tipoCoordenadas * entrega2 = busquedaPosicion(mapaIdentificacion, identificacion2);
	if(entrega2 == NULL) return;

	printf(green "\nSe encontraron ambas entregas\n" reset);

	//Se calcula la distancia entre las dos entregas
	double distanciaEntreEntregas = distanciaDosPuntos(entrega1->coordenadaX, entrega2->coordenadaX, entrega1->coordenadaY, entrega2->coordenadaY);
	printf(green "\nLa distancia entre entregas es de %.2lf\n" reset, distanciaEntreEntregas);
}

void entregasCercanas(HashMap *mapaIdentificacion)
{
	//PRIMERO ENCONTRAMOS TODO LO QUE VAMOS A OCUPAR, EL ENTREGAAUX SIRVE PARA RECCORER LOS PUNTOS
	long long coordenadaX, coordenadaY;
	printf("\nIngrese la cordenada X: ");
	scanf("%lli",&coordenadaX);
	printf("\nIngrese la cordenada Y: ");
	scanf("%lli",&coordenadaY);

	tipoCoordenadas * posicionAux = firstMap(mapaIdentificacion);

	//AHORA DEFINIMOS TODO LO QUE NECESITAREMOS PARA ENCONTRAR LOS MAS CERCANOS
    int arreglo[3]; //identificador
    double arreglo2[3];//distancia

    double distanciaEntregas;
    double distanciaAux;

    int cont = 0;
	double maximo = 0;
	int i,k;

	/*
		AQUI EMPIEZA LA FUNCION PARA ENCONTRAR AL MAS CERCANO
		lo que pense al principio es ir encontrando los mas cercanos y guardarlos
		en un arreglo, pero como pide mostrar el ID y la distancia, ocupo 2.
		quizas se pueda hacer mejor, pero es una idea.
	*/

    while(posicionAux != NULL)
	{
        distanciaEntregas = distanciaDosPuntos(coordenadaX, posicionAux->coordenadaX, coordenadaY, posicionAux->coordenadaY);

        if(cont != 0)
		{
            if(cont > 2)
			{
				//Recorro el arreglo distancia
				for(i = 0 ; i < 3 ; i++)
				{
					if(maximo == arreglo2[i])
					{
						if(distanciaEntregas < maximo)
						{
							arreglo[i] = posicionAux->identificacion;
							arreglo2[i] = distanciaEntregas;

							//Encontrar otro maximo
							maximo = 0;
							for(k = 0 ; k < 3 ; k++)
								if(maximo < arreglo2[k])	maximo = arreglo2[k];
							
							break;
						}
					}
				}
            }
			else
			{
                arreglo[cont] = posicionAux->identificacion;
                arreglo2[cont] = distanciaEntregas;
				if(maximo < distanciaEntregas) maximo = distanciaEntregas;
            }
        }
		else
		{
            arreglo[0] = posicionAux->identificacion;
            arreglo2[0] = distanciaEntregas;
			maximo = distanciaEntregas;
        }
        
		distanciaAux = distanciaEntregas;
        posicionAux = nextMap(mapaIdentificacion);
		cont++;
    }

	//mostrar EL RESULTADO
	printf(green"\n\nLas distancias mas cercanas a la posicion ingresada son:\n");
	
	int largo = 3; 
	//Esto sirve para cuando no hay mas de 5 lugares
	if(cont < 3) largo = cont;

	for(i = 0 ; i < largo ; i++)
		printf("\nID: %d con distancia %.2lf",arreglo[i],arreglo2[i]);
	
	printf("\n"reset);
}

void crearRuta(HashMap * mapaIdentificacion, HashMap * mapaRutas)
{
	tipoRuta* nuevaRuta = crearTipoRuta(size(mapaIdentificacion));

	//Ingreso de datos
	printf("\nIngrese la coordenada X: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaX);
	printf("\nIngrese la coordenada Y: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaY);
	nuevaRuta->arreglo[0]->posicion->identificacion = 0;
	
	List* lista = get_adj_nodes(mapaIdentificacion,nuevaRuta);
	tipoRuta * auxRuta = firstList(lista);

	while(auxRuta != NULL)
	{
		//Ordenar la entregas a través de la de menor distancia, a la de mayor
		int largoArreglo = 0;
		
		auxRuta = firstList(lista);
		if(auxRuta == NULL) break;

		tipoRuta* orden[size(mapaIdentificacion)];
		while(auxRuta != NULL){
			orden[largoArreglo] = auxRuta;
			auxRuta = nextList(lista);
			largoArreglo++;
		}
		int b;
		auxRuta = firstList(lista);

		int c;
		tipoRuta* temp;
		for(b=0 ; b<largoArreglo-1 ; b++){
			for(c=b ; c<largoArreglo ; c++){
				if(orden[c]->arreglo[orden[b]->largo-1]->distancia < orden[b]->arreglo[orden[b]->largo-1]->distancia){
					temp = orden[c];
					orden[c] = orden[b];
					orden[b] = temp;
				}
			}
		}


		//Muestro las entregas que puede elejir el usuario
		printf(yellow"\nLista de entregas(Distancia total hasta el momento %.2lf): \n"reset, nuevaRuta->distanciaTotal);
		for(b = 0 ; b < largoArreglo ; b++){
			printf(blue"%d) "reset, orden[b]->arreglo[orden[b]->largo-1]->posicion->identificacion);
			if(orden[b]->arreglo[orden[b]->largo-1]->posicion->identificacion < 10) printf(" ");
			printf("Distancia : %.2lf",orden[b]->arreglo[orden[b]->largo-1]->distancia);
			printf("\n");
		}
		
		printf("\nEl ID que elija es: ");
		int opcion;
		scanf("%d",&opcion);

		//Busco la opcion que ingreso el usuario
		auxRuta = firstList(lista);
		while(auxRuta != NULL)
		{
			if(opcion == auxRuta->arreglo[auxRuta->largo-1]->posicion->identificacion) break;
			auxRuta = nextList(lista);
		}

		//Si existe la entrega y no se ha usado, se almacena en el arreglo y avanza
		if(auxRuta != NULL)
		{
			nuevaRuta = auxRuta;
			lista = get_adj_nodes(mapaIdentificacion,nuevaRuta);
		}
		else 
		{
			printf(red"\nNo se encuentra tal entrega\n\n"reset);
			auxRuta = firstList(lista);
		}
	}
	
	//Se ingresa el nombre de la ruta, verificando que no se este usando
	do
	{
		printf("\nIngrese el nombre de la nueva ruta: ");
		getchar();
		scanf("%19[^\n]s",nuevaRuta->nombreRuta);
		convertirEstandar(nuevaRuta->nombreRuta);
	} while (nombreRepetido(mapaRutas,nuevaRuta->nombreRuta) != 1);

	mostrarRuta(nuevaRuta);
	insertMap(mapaRutas,nuevaRuta->nombreRuta,nuevaRuta);
	
}

void crearRutaAleatoria(HashMap * mapaIdentificacion, HashMap * mapaRutas)
{
	//Se crea la ruta que almacenera
	tipoRuta* nuevaRuta = crearTipoRuta(size(mapaIdentificacion));

	//Se leen las coordenadas de la posicion inicial
	printf("\nIngrese la coordenada X: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaX);
	printf("\nIngrese la coordenada Y: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaY);
	nuevaRuta->arreglo[0]->posicion->identificacion = 0;

	//Se crea la primera lista de nodos adyacentes
	List* lista = get_adj_nodes(mapaIdentificacion,nuevaRuta);
	tipoRuta * auxRuta = firstList(lista);

	while(auxRuta != NULL)
	{
		//Reviso que hayan posibles nodos dentro de la lista
		auxRuta = firstList(lista);
		if(auxRuta == NULL) break;
		
		//Genero una posible entrega aleatoria
		int opcion = rand() % size(mapaIdentificacion) + 1;

		//BUSCO LA OPCION
		auxRuta = firstList(lista);
		while(auxRuta != NULL)
		{
			if(opcion == auxRuta->arreglo[auxRuta->largo-1]->posicion->identificacion) break;
			auxRuta = nextList(lista);
		}

		//Si existe la entrega y no se ha usado, se almacena en el arreglo y avanza
		if(auxRuta != NULL)
		{
			nuevaRuta = auxRuta;
			lista = get_adj_nodes(mapaIdentificacion,nuevaRuta);
		}
		else auxRuta = firstList(lista);
	}
	
	//Se ingresa el nombre de la ruta, verificando que no se este usando
	do
	{
		printf("\nIngrese el nombre de la nueva ruta: ");
		getchar();
		scanf("%19[^\n]s",nuevaRuta->nombreRuta);
		convertirEstandar(nuevaRuta->nombreRuta);
	} while (nombreRepetido(mapaRutas,nuevaRuta->nombreRuta) != 1);
	
	mostrarRuta(nuevaRuta);
	insertMap(mapaRutas,nuevaRuta->nombreRuta,nuevaRuta);
}

void mejorarRuta(HashMap * mapaRutas)
{
	//Ingreso el nombre de la ruta a buscar
	char nombreRuta[20];
	printf("\nIngrese el nombre de la ruta: ");
	getchar();
	scanf("%19[^\n]s", nombreRuta);
	convertirEstandar(nombreRuta);
	
	//Si la ruta es optima, se indica que no es necesario hacerle nada
	if(strcmp(nombreRuta, "Ruta optima") == 0)
	{
		printf(blue"\nEs la mejor ruta, no es necesario modificarla\n"reset);
		return;
	}
	
	//La busco dentro del mapa
	tipoRuta * aux = searchMap(mapaRutas, nombreRuta);

	if(aux != NULL)
	{
		//Genero variables auxiliares dentro de la busqueda de posiciones en la ruta
		int identificacion1, identificacion2, posicion1 = -1, posicion2 = -1;

		printf(green"\nSe encontro la ruta %s\n"reset, nombreRuta);
		mostrarRuta(aux);

		//Se ingresa que tipo de cambio quiere hacer
		char cambio[11];
		printf("\nIngrese si quiere un cambio "blue"automatico"reset" o "blue"manual"reset" de posicion: ");
		scanf("%10s", cambio);

		if(strcmp(cambio, "automatico") == 0)
		{
			//Se usa la funcion rand, para generar numeros aleatorios
			identificacion1 = rand() % aux->largo;
			do
			{
				identificacion2 = rand() % aux->largo;
			} while (identificacion1 == identificacion2); //Verificando que sean numeros distintos entre ellos
			
			printf("\nSe eligieron los identificadores %i y %i\n", identificacion1, identificacion2);
		}
		else if(strcmp(cambio, "manual") == 0)
		{
			//El usuario ingresa las identificaciones a cambiar	
			printf("\nElija la identificacion 1: ");
			scanf("%i", &identificacion1);
			printf("\nElija la identificacion 2: ");
			scanf("%i", &identificacion2);
		}
		else
		{
			printf(red"\nNo se hara nada\n"reset);
			return;
		}
		
		//Se busca dentro del arreglo, las identificaciones que se ingresaron
		for(int i = 0; i < aux->largo; i++)
		{
			if(aux->arreglo[i]->posicion->identificacion == identificacion1) posicion1 = i;
			if(aux->arreglo[i]->posicion->identificacion == identificacion2) posicion2 = i;
			if(posicion1 != -1 && posicion2 != -1) break;
		}

		if(posicion1 == -1 || posicion2 == -1) //Si ninguna existe termina
		{
			printf(red"\nNo existe alguna de las 2 entregas ingresadas\n"reset);
			return;
		}

		cambioEntrega(aux, posicion1, posicion2); //Se cambian de posicion
		double distanciaActual = aux->distanciaTotal; //Se almacena la distancia original
		
		//Se calcula la distancia total, con las posiciones cambiadas
		aux->distanciaTotal = 0; 
		for(int i = 0; i < aux->largo - 1; i++)
		{
			aux->distanciaTotal += aux->arreglo[i]->distancia = distanciaDosPuntos(aux->arreglo[i]->posicion->coordenadaX,aux->arreglo[i + 1]->posicion->coordenadaX,aux->arreglo[i]->posicion->coordenadaY,aux->arreglo[i + 1]->posicion->coordenadaY);
		}

		if(aux->distanciaTotal > distanciaActual) //Si la distancia nueva es peor a la original, se mantiene la ruta original
		{
			mostrarRuta(aux);
			cambioEntrega(aux, posicion1, posicion2);
			aux->distanciaTotal = distanciaActual;
			printf(red"\nLa ruta actual es mejor, se mantendra\n"reset);
		}
		else if(aux->distanciaTotal == distanciaActual) //Si es igual, se mantiene la nueva ruta
		{
			mostrarRuta(aux);
			printf(blue"\nLa distancia total son iguales, pero se colocara la nueva ruta\n"reset);		
		}
		else //Si no, se mantiene la nueva ruta
		{
			printf(green"\nLa nueva ruta es mejor, se cambiara\n");
			mostrarRuta(aux);
			
			printf("\n");
		}
	}
	else
	{
		printf(red"\nNo se ha encontrado la ruta %s\n"reset, nombreRuta);
	}
}

void mostrarRutas(HashMap* mapaRutas)
{
	int largo = size(mapaRutas); 
	tipoRuta * ruta = firstMap(mapaRutas);
	tipoRuta * arregloRuta[largo];
	int i = 0;

	//Se ingresa la informacion a un arreglo que almacena los tipos ruta
	while(ruta != NULL) 
	{
		arregloRuta[i] = ruta;
		ruta = nextMap(mapaRutas);
		i++;
	}

	//Se ordenan de menor a mayor
	for (int c = 0 ; c < largo - 1; c++)
  	{
    	for (int d = 0 ; d < largo - c - 1; d++)
    	{
		  	if (arregloRuta[d]->distanciaTotal > arregloRuta[d+1]->distanciaTotal)
		  	{
		    	tipoRuta * swap = arregloRuta[d];
		    	arregloRuta[d] = arregloRuta[d+1];
		    	arregloRuta[d+1] = swap;
		  	}
    	}
  	}


  	//Se muestran todas las rutas
	printf(blue"\nLista de rutas creadas:\n"reset);
	for(int k = 0; k < largo;k++)
	{
		mostrarRuta(arregloRuta[k]);
	}
}

void mejorRuta(HashMap * mapaIdentificacion, HashMap * mapaRutas){
	tipoRuta* nuevaRuta = crearTipoRuta(size(mapaIdentificacion)+1);

	//Se inserta la primera posicion en la tipoRuta "nuevaRuta"
	printf("\nIngrese la coordenada X: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaX);
	printf("\nIngrese la coordenada Y: ");
	scanf("%lld",&nuevaRuta->arreglo[0]->posicion->coordenadaY);
	nuevaRuta->arreglo[0]->posicion->identificacion = 0;

	//Se crea la cola para hacer una busqueda por anchura
	Queue* cola = CreateQueue();
	PushBackQ(cola,nuevaRuta);
	int cont = 0;
	tipoRuta * eficiente;

	while(get_size(cola) != 0){
		//El tipoRuta n sera un auxiliar para ver los nodos adjacentes y para
		//verificar si es una ruta valida para insertar al arregloRuta
		tipoRuta* n= Front(cola);
		if(!n) break;
		PopFrontQ(cola);

		List* adj = get_adj_nodes(mapaIdentificacion,n); //Obtenemos los nodos adjacentes de n

		tipoRuta* aux = firstList(adj); //Esta ruta es solamente para insertarlo a la cola
		
		//Se inserta la ruta a la cola
		while(aux){
			PushBackQ(cola,aux);
			aux = nextList(adj);
		}

		//Si la ruta es valida, se inserta al arregloRuta
		if(n->largo == size(mapaIdentificacion) + 1)
		{
			if(cont == 0) eficiente = n;
			else if(eficiente->distanciaTotal > n->distanciaTotal) eficiente = n;
			cont++;
		}
	}

	//Se inserta la ruta en el mapa
	printf(blue"\nSe debio hacer un total de %i combinaciones\n", cont);
	strcpy(eficiente->nombreRuta,"ruta optima");
	convertirEstandar(eficiente->nombreRuta);
	mostrarRuta(eficiente);
	insertMap(mapaRutas,eficiente->nombreRuta,eficiente);
	
}

void mostrarCoordenadas(HashMap * mapaIdentificacion)
{
	tipoCoordenadas * aux = firstMap(mapaIdentificacion);

	printf(blue"\nLista de entregas: \n");

	while(aux != NULL)
	{
		printf(green"\nIdentificacion: %i\n", aux->identificacion);
		printf("Coordenada X: %lli Coordenada Y: %lli\n"reset, aux->coordenadaX, aux->coordenadaY);

		aux = nextMap(mapaIdentificacion);
	}
}