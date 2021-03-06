#ifndef ENTREGAS_H
#define ENTREGAS_H

//Estructura que almacena los valores leidos del .txt
typedef struct tipoCoordenadas
{
	int identificacion;
	long long coordenadaX;
	long long coordenadaY;
}tipoCoordenadas;

//Estructura que se almacena como posiciones de la ruta
typedef struct tipoEntregas
{
	tipoCoordenadas * posicion; //Almacenar los valores de posicion y el identificador
	double distancia; //Almacenar la distancia entre el punto actual de la persona y este
}tipoEntregas;

//Estructura que sirve para almacenar la ruta completa
typedef struct tipoRuta
{
	tipoEntregas ** arreglo; //Almacenar las posiciones de entregas
	double distanciaTotal; //Almacenar la distancia total recorrida
	char nombreRuta[20]; //Almacenar el nombre de la ruta
	int largo; //Almacenar la cantidad de entregas de la ruta
}tipoRuta;

//Funciones para crear los tipos usados
tipoCoordenadas * crearTipoCoordenadas();
tipoEntregas * crearTipoEntregas();
tipoRuta * crearTipoRuta(int);

//Funcion para voltear dos valores dentro del arreglo del tipoRuta
void cambioEntrega(tipoRuta *, int, int);

#endif