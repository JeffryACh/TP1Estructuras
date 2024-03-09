/*
	Autores:
		Walter Rivera Solis. wariso.alc@gmail.com
		Victor Garro Abarca
		Jeffry Araya Chaves
		Gabriel Barrantes Villalobos
	FechaDeCreacion: 09/03/2024 15:30
	UltimaModificacion: 09/03/2024 16:15
*/

//Efecto Matrix

//Dimensiones de la ventana: 120x30
#include <iostream>
#include "windows.h"
const int total = 1;  //Define la cantidad de "hiladas de caracteres" que se generan, max=50
const int longitud = 10; //Define el largo en cantidad de caracteres, de las hiladas
using namespace std;
//Define la estructura de las "hiladas de caracteres"
typedef struct hilera {
	char caracter;
	int X;  //coordenada en X para el caracter de mas abajo de una hilada
	int Y;  //coordenada en Y para el caracter de mas abajo de una hilada
	int contador;  //Sirve para contar los caracteres que faltan de salir de la ventana cuando la hilera llega al final
};
//gotoxy permite posicionar el cursor en cualquier parte de la ventana
/*
	La funcion gotoxy recibe dos parametros, la coordenada en X y la coordenada en Y, y se encarga de posicionar el cursor en la ventana de la consola
	parametros:
		int x: coordenada en X
		int y: coordenada en Y
	return:
		void
*/
void gotoxy(int x, int y)
{
	COORD posicion;
	posicion.X = x;
	posicion.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), posicion);
}
//Inicializa todo el conjunto de hileras, mediante la funcion anterior
/*
	La funcion inicializar1 recibe un arreglo de estructuras de tipo hilera y se encarga de inicializar cada una de las hileras
	parametros:
		hilera hilera[total]: arreglo de estructuras de tipo hilera
	return:
		void
*/
void inicializar1(hilera hilera[total]) {
	for (int i = 0; i < total; i++) {
		hilera[i].X = rand() % 120;
		hilera[i].Y = rand() % 30;
		hilera[i].caracter = ' ';
		hilera[i].contador = 0;
	}
}
//Dibuja las hiladas que han sido generadas en la pantalla
/*
	La funcion dibujar recibe un arreglo de estructuras de tipo hilera y se encarga de dibujar cada una de las hileras
	parametros:
		hilera hilera[total]: arreglo de estructuras de tipo hilera
	return:
		void
*/
void dibujar(hilera hilera[total]) {
	int coordenadaY;  /*lmacena temporalmente el valor de coordenada, es a esta variable que se le
	resta 1 en cada iteracion del ciclo para dibujar los caracteres que van arriba del mas reciente*/
	for (int i = 0; i < total; i++) { //Ciclo que alterna entre el total de hileras
		coordenadaY = hilera[i].Y;
		gotoxy(hilera[i].X, coordenadaY); //se posiciona en el caracter de mas abajo
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //selecciona el color verde
		cout << hilera[i].caracter << endl;
		gotoxy(hilera[i].X, (coordenadaY - longitud));
		cout << ' ' << endl;
		//La hilera llegara al limite de coordenada Y=28
		if (hilera[i].Y < 28) {
			hilera[i].Y = hilera[i].Y + 1;
		}
		//Cuando Y esta en el limite, es necesario ir borrando uno a uno los caracteres de mas arriba a como la 
		//hilera va bajando
		if (hilera[i].Y == 28) {
			gotoxy(hilera[i].X, (coordenadaY - (longitud - hilera[i].contador)));
			cout << ' ' << endl;
			hilera[i].contador = hilera[i].contador + 1;
			if (hilera[i].contador > longitud) {
				hilera[i].Y = rand() % 20;
				hilera[i].X = rand() % 120;
				hilera[i].contador = 0;
			}
		}
	}
}
//genera un nuevo caracter y hace un llamado a la funcion desplazar
/*
	La funcion generar recibe un arreglo de estructuras de tipo hilera y se encarga de generar un nuevo caracter para cada una de las hileras
	parametros:
		hilera hilera[total]: arreglo de estructuras de tipo hilera
	return:
		void
*/
void generar(hilera hilera[total])
{
	char nuevocaracter;
	for (int i = 0; i <= total; i++) {
		nuevocaracter = 33 + rand() % 93;
		hilera[i].caracter = nuevocaracter;
	}
}

//Funcion principal
int main()
{
	hilera conj_hileras[total];
	inicializar1(conj_hileras);
	while (true) {
		generar(conj_hileras);
		dibujar(conj_hileras);
		Sleep(20);
	}
}
