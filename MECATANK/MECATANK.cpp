/*
	Autores:
		Victor Garro Abarca
		Jeffry Araya Chaves
		Gabriel Barranbtes Villalobos
	FechaDeCreacion: 09/03/2024 13:30
	UltimaModificacion: 09/03/2024 15:30
*/

//////////////////////////////////////////////////////


//ESTE CODIGO UTILIZA FUNCIONES MULTIMEDIA

//SE DEBE AGREGAR LA LIBRERIA winmm.lib en PROJECT->SETTINGS->LINK 
//PARA PODER COMPILAR CORRECTAMENTE ESTE CODIGO.

// o COMENTAR LAS LINEAS QUE CONTENGAN:

// PlaySound(TEXT("***.wav"), NULL, SND_ASYNC);

//////////////////////////////////////////////////////

//LIBRERIAS UTILIZADAS

#include <windows.h>
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <iostream>
using namespace std;
#pragma warning(disable:4996);
void menu();
//DEFINICION DE VARIABLES GLOBALES
//************************************************

int cont_aliados_destruidos = 8; //Lleva la cuenta de los cañones de usuario destruidos
int cont_enemigos_destruidos = 8; //Lleva la cuenta de los cañones de la computadora destruidos 

//Aqui se define la estructura de los cañones
typedef struct Canon {
	int num_canon; //Identifica al cañon con un numero del 1 al 8.
	int posicion; //0>Cañon Superior,  1>Cañon Inferior
	int activo;   //1>Puede Disparar,  0>No puede Disparar
	int destruido; //0>Se Muestra en pantalla, 1>No se muestra en pantalla
}C; //Se define el tipo de dato Canon
Canon Canones[15];

//*************************************************
//FIN DE DEFINICION DE VARIABLES GLOBALES


//************************************************
//COMIENZA DECLARACION DE FUNCIONES

typedef struct Bala {
	int num_canon; //Identifica la posicion x de la bala con un numero del 0 al 8.
	int num_vertical; //Identifica la posicion y de la bala.
	int direccion; //0>Bala Defensa,  1>Bala Ofensa
	int posicion; //0>Cañon Superior,  1>Cañon Inferior
	char caracter; //Caracter que representa la bala en pantalla

	Bala* Siguiente; //Puntero que apunta a la siguiente bala en la cola de eventos
}B; //Se define el tipo de dato Bala

typedef Bala* PtrBala;
//******************************************

/*
	Se definen la funcion Inicializar_Cola_Eventos() la cual se encarga de inicializar la lista de balas
	parametros:
		PtrBala& Lista_Balas: Puntero que apunta a la lista de balas
		PtrBala& Nuevo: Puntero que apunta a la nueva bala que se va a encolar
	return:
		void
*/
void Inicializar_Cola_Eventos(PtrBala& Lista_Balas)
{
	Lista_Balas = nullptr;
}

/*
	Se definen la funcion Destruir_Cola_Eventos() la cual se encarga de destruir la lista de balas
	parametros:
		PtrBala& Lista_Balas: Puntero que apunta a la lista de balas
	return:
		void
*/
void Destruir_Cola_Eventos(PtrBala& Lista_Balas)
{
	PtrBala Aux;
	Aux = Lista_Balas;
	while (Aux != NULL)
	{
		if ((Lista_Balas = nullptr) || (Lista_Balas == NULL)) { break; }
		Lista_Balas = Lista_Balas->Siguiente;
		free(Aux);
		Aux = Lista_Balas;
	}
}

/*
	Se definen la funcion CrearBala() la cual se encarga de crear una nueva bala
	parametros:
		int NumCanon: Numero de cañon que dispara la bala
		int NumVertical: Posicion en pantalla de la bala
		int direccion: 0>Bala Defensa,  1>Bala Ofensa
		char caracterr: Caracter que representa la bala en pantalla
	return:
		PtrBala: Puntero que apunta a la nueva bala creada
*/
PtrBala CrearBala(int NumCanon, int NumVertical, int direccion, char caracterr)
{
	PtrBala Bullet = new(Bala);
	Bullet->num_canon = NumCanon;
	Bullet->num_vertical = NumVertical;
	Bullet->direccion = direccion;
	Bullet->caracter = caracterr;
	Bullet->Siguiente = nullptr;
	if (direccion == 0) {
		Bullet->posicion = 15;
	}
	if (direccion == 1) {
		Bullet->posicion = 1;
	}
	return Bullet;
}

/*
	Se definen la funcion Encolar_Cola_Eventos() la cual se encarga de encolar una bala en la lista de balas
	parametros:
		PtrBala& Lista_Balas: Puntero que apunta a la lista de balas
		PtrBala& Nuevo: Puntero que apunta a la nueva bala que se va a encolar
	return:
		void
*/
void Encolar_Cola_Eventos(PtrBala& Lista_Balas, PtrBala& Nuevo)
{
	PtrBala Aux;
	Aux = Lista_Balas;
	if (Aux != nullptr)
	{
		while (Aux->Siguiente != nullptr) // Se recorre la lista de balas hasta llegar al ultimo elemento
		{
			Aux = Aux->Siguiente;

		}

		Aux->Siguiente = Nuevo;
	}
	else
	{
		Lista_Balas = Nuevo;
	}

}

/*
	Se definen la funcion Descolar() la cual se encarga de desencolar una bala de la lista de balas
	parametros:
		PtrBala& Lista_Balas: Puntero que apunta a la lista de balas
	return:
		PtrBala: Puntero que apunta a la bala que se desencola
*/
PtrBala Descolar(PtrBala& Lista_Balas)
{
	PtrBala Aux;
	Aux = Lista_Balas;
	if (Lista_Balas != nullptr) {
		Lista_Balas = Lista_Balas->Siguiente;
		Aux->Siguiente = nullptr;
		return Aux;
	}
	else {
		return nullptr;
	}
}



//GOTOXY
/*
	Se definen la funcion gotoxy() la cual se encarga de mover el cursor por la pantalla.
	parametros:
		int x: Posicion en el eje x
		int y: Posicion en el eje y
	return:
		void

*/
void gotoxy(int x, int y) //Define la funcion para poder mover el cursor por la pantalla.
{
	HANDLE hStdout;
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return;

	SetConsoleCursorPosition(hStdout, pos);
}
//FIN de funcion GOTOXY


//INICIALIZAR CAÑONES------------Crea los cañones.
/*
	Se definen la funcion utilizar_Canones() la cual se encarga de crear los cañones.
	parametros:
		void
	return:
		void
*/
void Inicializar_Canones() {   //Crea 16 cañones y les asigna los valores iniciales de posicion,			
	int i = 0;				  //numero de cañon, y por defecto los pone activos y no destruidos.
	for (i; i < 16; i++) {
		Canones[i].num_canon = i;
		Canones[i].destruido = 0;
		Canones[i].activo = 1;
		if (i < 8) {
			Canones[i].posicion = 0;
		}
		else {
			Canones[i].posicion = 1;
		}
	}
}
//FIN de funcion INICIALIZAR CAÑONES


//INICIALIZAR LETRAS ---------Agrupa las letras del teclado en grupos.
/*
	Se definen la funcion INI la cual se encarga de agrupar las letras del teclado en grupos.
	parametros:
		char Letras[4][8]: Matriz que almacena las letras del teclado
	return:
		void
*/
void INI(char Letras[4][8])
{
	Letras[0][0] = 'q';
	Letras[0][1] = 's';
	Letras[0][2] = 'e';
	Letras[0][3] = 'f';
	Letras[0][4] = 'g';
	Letras[0][5] = 'u';
	Letras[0][6] = 'n';
	Letras[0][7] = 'k';

	Letras[1][0] = 'a';
	Letras[1][1] = 'w';
	Letras[1][2] = 'v';
	Letras[1][3] = 'c';
	Letras[1][4] = 'h';
	Letras[1][5] = 'y';
	Letras[1][6] = 'i';
	Letras[1][7] = 'p';

	Letras[2][0] = 'z';
	Letras[2][1] = 'x';
	Letras[2][2] = 'd';
	Letras[2][3] = 'r';
	Letras[2][4] = 'b';
	Letras[2][5] = 'm';
	Letras[2][6] = 'j';
	Letras[2][7] = 'o';

	Letras[3][0] = 'a';
	Letras[3][1] = 'w';
	Letras[3][2] = 'd';
	Letras[3][3] = 't';
	Letras[3][4] = 'h';
	Letras[3][5] = 'm';
	Letras[3][6] = 'l';
	Letras[3][7] = 'k';
}
//FIN de Funcion INICIALIZAR LETRAS


//Funcion ALEATORIO ---------- Se encarga de los eventos aleatorios del programa, retorna un int 
/*
	Se definen la funcion INA la cual se encarga de inicializar la funcion aleatorio.
	parametros:
		void
	return:
		void
*/
void INA(void) {				// aleatorio dentro del rango r.
	time_t t;
	srand((unsigned)time(&t));
}

/*
	Se definen la funcion aleatorio la cual se encarga de generar un numero aleatorio.
	parametros:
		int r: Rango del numero aleatorio
	return:
		int: Numero aleatorio
*/
int aleatorio(int r) {
	return (rand() % r);
}

//FIN de Funcion ALEATORIO

//FUNCION DIBUJAR CAÑONES-----------------Se encarga de ubicar a los cañones en la pantalla, 
// y de mostrarlos destruidos o activos 
/*
	Se definen la funcion Dibujar_Canones() la cual se encarga de ubicar a los cañones en la pantalla
	parametros:
		char Letras[4][8]: Matriz que almacena las letras del teclado
		int ListaLtr: Numero de grupo de letras
	return:
		void
*/
void Dibujar_Canones(char Letras[4][8], int ListaLtr) {

	int x1 = 5;
	int x2 = 5;
	for (int i = 0; i < 16; i++)
	{
		if (Canones[i].destruido != 1 && Canones[i].posicion == 0)
		{
			gotoxy(x1, 4);
			cout << " |``º''| " << endl;
			x1 = x1 + 9;
		}
		if (Canones[i].destruido != 1 && Canones[i].posicion == 1)
		{
			gotoxy(x2, 20);
			cout << "  //º\\\\  " << endl;
			gotoxy(x2 + 4, 21);
			cout << Letras[ListaLtr][i - 8] << endl;
			x2 = x2 + 9;
		}
		if (Canones[i].destruido == 1 && Canones[i].posicion == 0)
		{
			gotoxy(x1, 4);
			cout << "           " << endl;
			x1 = x1 + 9;
		}
		if (Canones[i].destruido == 1 && Canones[i].posicion == 1)
		{

			gotoxy(x2, 20);
			cout << " .....  " << endl;
			x2 = x2 + 9;

		}

	}

}
//FIN de FUNCION DIBUJAR CAÑONES

//FUNCIONES CREAR y CARGAR ARCHIVO --------- Crea y carga el archivo de resultados.
/*
	Se definen la funcion CrearArchivo() la cual se encarga de crear el archivo de resultados
	parametros:
		char nombre[20]: Nombre del jugador
		int cont_aliados_destruidos: Numero de cañones destruidos por el jugador
		int cont_enemigos_destruidos: Numero de cañones destruidos por la computadora
		int balas_aliados_disparadas: Numero de balas disparadas por el jugador
		int balas_enemigas_disparadas: Numero de balas disparadas por la computadora
		int balas_destruidas: Numero de balas destruidas
		int dificultad: Dificultad del juego
	return:
		void

*/
void CrearArchivo(char nombre[20], int cont_aliados_destruidos, int cont_enemigos_destruidos, int balas_aliados_disparadas, int balas_enemigas_disparadas, int balas_destruidas, int dificultad)
{										//Se crea la funcion CrearArchivo()
	FILE* archivo;							// "handle" para hacer E/S en el archivo
	archivo = fopen("resultados.txt", "a");	//crea el archivo

	if (NULL == archivo) {					// si el archivo es NULL entonces hubo problemas al crearlo genera el siguiente mensaje:.
		fprintf(stderr, "No se pudo crear archivo %s.\n", "resultados.txt");
		exit(-1);
	}
	else {
		//Si se pude crear el archivo, entonces:
		fprintf(archivo, "%s\n", nombre);
		fprintf(archivo, "%i\n", cont_aliados_destruidos);
		fprintf(archivo, "%i\n", cont_enemigos_destruidos);
		fprintf(archivo, "%i\n", balas_aliados_disparadas);
		fprintf(archivo, "%i\n", balas_enemigas_disparadas);
		fprintf(archivo, "%i\n", balas_destruidas);
		fprintf(archivo, "%i\n", dificultad);
		cout << "Puntaje almacenado con exito..." << endl; //Se muestra el mensaje de creado con exito

	}
	fclose(archivo);				//cierra el archivo.

}

/*
	Se definen la funcion CargarArchivo() la cual se encarga de cargar el archivo de resultados
	parametros:
		void
	return:
		void
*/
void CargarArchivo() {    //Crea la funcion para guardar archivo
	FILE* archivo;//handle" para hacer E/S en el archivo
	char nombre[20],
		cont_aliados_destruidos[5],
		cont_enemigos_destruidos[5],
		balas_aliados_disparadas[5],
		balas_enemigas_disparadas[5],
		balas_destruidas[10],
		nivel[10];
	//Se definen las variables


	archivo = fopen("resultados.txt", "r");		//crea el archivo y lo prepara para su lectura

	if (NULL == archivo) {			// si el archivo es NULL entonces hubo problemas al crearlo y se muestra el mensaje:.
		system("cls");
		gotoxy(15, 10);
		cout << "ERROR: Aun no se ha jugado ningun juego.\n " << endl;
		gotoxy(15, 13);
		fprintf(stderr, "No se pudo abrir el archivo %s.\n", "resultados.txt");
		gotoxy(15, 15);
		cout << "Sugerencia:" << endl;
		gotoxy(15, 16);
		cout << "Termine un juego nuevo para crear el archivo de resultados." << endl;
		gotoxy(15, 20);
		system("pause");
		//exit(-1);
	}
	else {

		while (!feof(archivo)) {	// realiza el ciclo hasta q sea fin del archivo
			//feof(archivo)->verifica si es fin de archivo.
			system("cls");
			fscanf(archivo, "%s\n", nombre);
			fscanf(archivo, "%s\n", cont_aliados_destruidos);
			fscanf(archivo, "%s\n", cont_enemigos_destruidos);
			fscanf(archivo, "%s\n", balas_aliados_disparadas);
			fscanf(archivo, "%s\n", balas_enemigas_disparadas);
			fscanf(archivo, "%s\n", balas_destruidas);
			fscanf(archivo, "%s\n", nivel);
			gotoxy(1, 4);
			cout << "\t\t   ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
			cout << "\t\t   º       RESULTADOS  ULTIMO JUEGO      º±" << endl;
			cout << "\t\t   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼±" << endl;
			cout << "\t\t    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±" << endl;
			cout << "\n";
			cout << "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
				<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþ                                      þþþþ\n"
				<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
				<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n" << endl;
			gotoxy(22, 12);
			cout << "Jugador:	" << endl;
			gotoxy(45, 12);
			cout << nombre << endl;

			gotoxy(22, 13);
			cout << "Cañones Vivos: " << endl;
			gotoxy(55, 13);
			cout << cont_aliados_destruidos << endl;

			gotoxy(22, 14);
			cout << "Cañones Enemigos Destruidos: " << endl;
			gotoxy(55, 14);
			cout << 8 - atoi(cont_enemigos_destruidos) << endl;

			gotoxy(22, 15);
			cout << "Cant. de balas disparadas: " << endl;
			gotoxy(55, 15);
			cout << balas_aliados_disparadas << endl;

			gotoxy(22, 16);
			cout << "Cant. de balas enemigas" << endl;
			gotoxy(22, 17);
			cout << "disparadas: " << endl;

			gotoxy(55, 17);
			cout << balas_enemigas_disparadas << endl;

			gotoxy(22, 18);
			cout << "Total de Balas Destruidas: " << endl;
			gotoxy(55, 18);
			cout << balas_destruidas << endl;
			gotoxy(50, 20);
			cout << "Nivel:" << nivel << endl;
			if (atoi(cont_aliados_destruidos) <= 3)
			{
				gotoxy(30, 20);
				cout << "Estado:  PERDEDOR" << endl;
			}
			if (atoi(cont_enemigos_destruidos) <= 3)
			{
				gotoxy(30, 20);
				cout << "Estado:  GANADOR" << endl;
			}
			gotoxy(10, 25);
			cout << "Presione ESC para el menu, o cualquier tecla para continuar" << endl;
			int k = getch();//Compara si se presiono la tecla ESC o si se presiono otra tecla
			if (k == 27) {
				fclose(archivo);// Si se presiono ESC cierra el archivo y vuelve al menu principal.
				menu();
			}
			else {
				continue;
			}

		}
		fclose(archivo);
	}
	//cierra el archivo.

}
//Fin de FUNCIONES CREAR y CARGAR ARCHIVO

/*
	Se definen la funcion colisiona() la cual se encarga de verificar si dos balas colisionan
	parametros:
		PtrBala TEMPAUX: Puntero que apunta a la lista de balas
		PtrBala TEMP: Puntero que apunta a la lista de balas
		int balas_destruidas: Numero de balas destruidas
	return:
		void
*/
void colisiona(PtrBala TEMPAUX, PtrBala TEMP, int balas_destruidas) {
	char Bomb = '*';
	while ((TEMPAUX != nullptr))// Solo se compara si hay mas de una bala, es decir si la lista temporal no es vacia
	{
		//if (TEMPAUX == nullptr) { break; }
		if ((TEMP->num_canon == TEMPAUX->num_canon && TEMP->direccion != TEMPAUX->direccion && TEMP->caracter == TEMPAUX->caracter) && (TEMP->posicion == TEMPAUX->posicion || TEMP->posicion + 1 == TEMPAUX->posicion))
		{//se busca que temp y temp aux tengan esos atributos en comun y si los encuentra es porq estan en la misma posicion y son de cañones opuestos.

			TEMP->caracter = Bomb;// se le cambia el caracter a TEMP 
			TEMPAUX->caracter = Bomb; //se le cambia el caracter a TEMP aux asi la proxima vez q se impriman ya no van a tener letra si no q tendran un *
			balas_destruidas = balas_destruidas + 1;//Este contador lleva la cuenta de las balas destruidas.
		}
		TEMPAUX = TEMPAUX->Siguiente;//Si la comparacion no fue satisfactoria se apunta al siguiente elemento en la lista y se vuelve a comparar.

	}

}

//---------------------------------------------------------------------------------
//FUNCION JUEGO -- Esta es la funcion que se encarga de la logica del juego.

/*
	Se definen la funcion juego() la cual se encarga de la logica del juego
	parametros:
		void
	return:
		void
*/
void juego()
{
	system("cls");
	// Declaracion de variable para utilizadas en la funcion juego
	//************************************************************************
	PtrBala Balas, Nuevo, TEMP, TEMPAUX, AUX;
	int ListaLtr, aleator, aleator2, i, canon = 8, g = 0, hit, dificultad = 4, c = 3;
	char k = NULL;
	char Bomb = '*';
	char Letras[4][8];
	char nombre[20];
	int balas_aliados_disparadas = 0;
	int balas_enemigas_disparadas = 0;
	int balas_destruidas = 0;
	//************************************************************************	

	//Proceso de iniciacion o carga del programa, aqui se preparan los cañones las letras,
	//la interfaz, etc.

	Inicializar_Canones();           //Se inicializan los cañones
	Inicializar_Cola_Eventos(Balas); //Se prepara la cola de Eventos para recibir las balas
	INI(Letras);					 //Se crean los grupos de letras.
	INA();  // Se inicializa la funcion aleatorio.
	ListaLtr = aleatorio(4); //Se elige un grupo de letras aleatoriamente
	gotoxy(5, 10);
	//PlaySound(TEXT("tada.wav"), NULL, SND_ASYNC);//Esta linea carga la musica del juego

	gotoxy(1, 5);
	// MENU DE LA INTERFAZ
	cout << "\t\t   ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
	cout << "\t\t   º             MECATANK V1.0           º±" << endl;
	cout << "\t\t   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼±" << endl;
	cout << "\t\t    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±" << endl;

	cout << "\n";
	cout << "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþ Elige la dificultad del juego:       þþþþ\n"
		<< "\t\tþþþþ 1-  Facil                            þþþþ\n"
		<< "\t\tþþþþ 2-  Medio                            þþþþ\n"
		<< "\t\tþþþþ 3-  Dificil                          þþþþ\n"
		<< "\t\tþþþþ                    Dificultad:       þþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n" << endl;

	//
	while (dificultad > 3) {
		gotoxy(53, 16);
		cout << "     " << endl;
		gotoxy(53, 16);
		cin >> dificultad;  // Se elige la dificultad de juego
	}

	system("cls");
	bool prueba = true;
	//Hasta aqui la carga del programa

	//Comienza el ciclo de juego

	while (c == 3) {
		system("color F1");
		hit = 0;
		Dibujar_Canones(Letras, ListaLtr); //Se dibujan los cañones de acuerdo asi estan activos o destruidos.
		Sleep(cont_enemigos_destruidos * 80 - (35 * dificultad));

		for (i = 0; i < dificultad; i++)//Proceso que crea las balas de la computadora
		{
			aleator = aleatorio(8); // Se cargan dos variables aleatorias, aleator y aleator2
			aleator2 = aleatorio(42);

			if (((aleator2) % 7) == 0 && Canones[aleator].activo == 1 && Canones[aleator].destruido == 0)
			{ //Si se cumplen esas condiciones entonces:

				Nuevo = CrearBala(aleator, 5, 1, Letras[ListaLtr][aleator]); //se crea un objeto bala nuevo
				gotoxy((Nuevo->num_canon + 1) * 9, 5);
				cout << Nuevo->caracter << endl;							//se imprime el objeto bala en pantalla
				balas_enemigas_disparadas = balas_enemigas_disparadas + 1;  //se aumenta en uno el contador de balas disparadas por la computadora
				Encolar_Cola_Eventos(Balas, Nuevo);						//Se inserta la nueva bala en la cola de eventos.
				//delete(Nuevo);//Se libera la memoria utilizada para crear la bala nueva.
			}
		}


		if (kbhit()) //Espera Tecla de Usuario Si se presiona una tecla se entra al if y si no se presiona tecla el programa continua.
		{
			k = getch(); //Espera un pulso de teclado.
			Sleep(1);

			//Se compara la tecla presionada con el bloque de letras que se cargo.

			if (k == 'q' && ListaLtr == 0 || k == 'z' && ListaLtr == 2 || k == 'a' && ListaLtr == 1 || k == 'a' && ListaLtr == 3)
			{
				hit = 1;
				canon = 0;
			}
			if (k == 's' && ListaLtr == 0 || k == 'w' && ListaLtr == 1 || k == 'x' && ListaLtr == 2 || k == 'w' && ListaLtr == 3)
			{
				hit = 1;
				canon = 1;
			}
			if (k == 'e' && ListaLtr == 0 || k == 'v' && ListaLtr == 1 || k == 'd' && ListaLtr == 2 || k == 'd' && ListaLtr == 3)
			{
				hit = 1;
				canon = 2;
			}
			if (k == 'f' && ListaLtr == 0 || k == 'c' && ListaLtr == 1 || k == 'r' && ListaLtr == 2 || k == 't' && ListaLtr == 3)
			{
				hit = 1;
				canon = 3;
			}
			if (k == 'g' && ListaLtr == 0 || k == 'h' && ListaLtr == 1 || k == 'b' && ListaLtr == 2 || k == 'h' && ListaLtr == 3)
			{
				hit = 1;
				canon = 4;
			}
			if (k == 'u' && ListaLtr == 0 || k == 'y' && ListaLtr == 1 || k == 'm' && ListaLtr == 2 || k == 'm' && ListaLtr == 3)
			{
				hit = 1;
				canon = 5;
			}
			if (k == 'n' && ListaLtr == 0 || k == 'i' && ListaLtr == 1 || k == 'j' && ListaLtr == 2 || k == 'l' && ListaLtr == 3)
			{
				hit = 1;
				canon = 6;
			}
			if (k == 'k' && ListaLtr == 0 || k == 'p' && ListaLtr == 1 || k == 'o' && ListaLtr == 2 || k == 'k' && ListaLtr == 3)
			{
				hit = 1;
				canon = 7;
			}

			if (Canones[canon].activo == 1 && Canones[canon].destruido == 0 && hit == 1)
			{//si el cañon esta activo y el otro cañon no esta aun destruido entonces: 
				Nuevo = CrearBala(canon, 19, 0, k); //Se crea un nuevo objeto bala
				gotoxy((Nuevo->num_canon + 1) * 9, 20);
				cout << Nuevo->caracter << endl;	 // Se muestra el objeto bala en pantalla
				balas_aliados_disparadas = balas_aliados_disparadas + 1; //Se aumenta el contador de balas disparadas por el usuario
				Encolar_Cola_Eventos(Balas, Nuevo); //Se inserta la nueva bala en la cola de eventos
				//delete(Nuevo); //Se libera la memoria utilizada para crear la bala nueva.

			}

		}//Fin Ingreso datos Usuario if(kbhit())


		 //COMPARACION DE BALAS
		Inicializar_Cola_Eventos(AUX);
		Inicializar_Cola_Eventos(TEMPAUX);//Se crea una cola temporal
		//Inicializar_Cola_Eventos(Balas);
		while (Balas != nullptr)

		{

			TEMPAUX = Balas->Siguiente; //se le asigna a la cola temporal la lista de balas a partir de la segunda bala
			//if (TEMPAUX == nullptr) {
			//	break;
			//}
			//else
			TEMP = Descolar(Balas);// se saca la primera bala de la lista. Esta sera la bala a comprar con las demas.
			//if (TEMPAUX != nullptr) { prueba = true; }
			colisiona(TEMPAUX, TEMP, balas_destruidas);

			//TERMINA COMPRACION DE BALAS

			//Inicia secuencia para mostrar balas en pantalla
			//Imprime balas de cañones de defensa
			if (TEMP->direccion == 0 && TEMP->num_vertical > 4)
			{
				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical + 1); //Borra de la pantalla el caracter que se mostro en el ciclo anterior
				cout << " " << endl;

				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical);
				if (TEMP->caracter == Bomb) {                      //Si la bala tenia el caracter * entonces se muestra un espacio en blanco
					cout << TEMP->caracter << endl;					//La bala ya no se encolara de nuevo en la cola de eventos, ya que si tiene el caracter * 
					Sleep(50);									//significa que exploto	
					gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical);
					cout << " " << endl;
				}
				else {							//Si la bala no tiene el caracter * entonces se muestra en la pantalla
					cout << TEMP->caracter << endl;
					TEMP->num_vertical = TEMP->num_vertical - 1;
					TEMP->posicion = TEMP->posicion - 1;
					Encolar_Cola_Eventos(AUX, TEMP); // Si la bala no exploto y no ha llegado a tocar un cañon entonces se encola de nuevo en la cola de eventos (AUX).
				}
			}
			//Imprime las balas de los cañones de la computadora
			//se maneja bajo la misma logica que antes
			if (TEMP->direccion == 1 && TEMP->num_vertical < 20)
			{
				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical - 1);
				cout << " " << endl;
				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical);
				if (TEMP->caracter == Bomb) {
					cout << TEMP->caracter << endl;
					Sleep(50);
					gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical);
					cout << " " << endl;
				}
				else {
					cout << TEMP->caracter << endl;
					TEMP->num_vertical = TEMP->num_vertical + 1;
					TEMP->posicion = TEMP->posicion + 1;
					Encolar_Cola_Eventos(AUX, TEMP);
				}
			}
			//Cambia los valores de los tanques de defensa para saber si se destruyeron o no, y si quedan activos o no.
			//Si una bala llega a tocar un cañon este cambia sus atributos a destruido y cambia el cañon inverso a inactivo
			if (TEMP->direccion == 0 && TEMP->num_vertical == 4 && Canones[TEMP->num_canon].destruido == 0) {
				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical + 1);
				cout << " " << endl;
				Canones[TEMP->num_canon].destruido = 1;
				Canones[TEMP->num_canon + 8].activo = 0;
				cont_enemigos_destruidos = cont_enemigos_destruidos - 1;

			}
			//Cambia los valores de los tanques de la computadora para saber si se destruyeron o no, y si quedan activos o no.
			//Si una bala llega a tocar un cañon este cambia sus atributos a destruido y cambia el cañon inverso a inactivo
			if (TEMP->direccion == 1 && TEMP->num_vertical == 20 && Canones[TEMP->num_canon + 8].destruido == 0) {
				gotoxy((TEMP->num_canon + 1) * 9, TEMP->num_vertical - 1);
				cout << " " << endl;
				Canones[TEMP->num_canon + 8].destruido = 1;
				Canones[TEMP->num_canon].activo = 0;
				cont_aliados_destruidos = cont_aliados_destruidos - 1;

			}
			//Se libera la memoria utilizada
			//free(TEMP);
			if (TEMPAUX == nullptr) { break; }
			Destruir_Cola_Eventos(TEMPAUX);
		}

		//FIN de secuencia para mostrar balas en pantalla

		Balas = new(Bala);
		Balas->Siguiente = AUX;  //Se crea una nueva cola de eventos y se copian los contenidos de la cola AUX.
		//Esta nueva cola de eventos, por lo tanto no tendra los elementos que explotaron. 

//Destruir_Cola_Eventos(AUX); //Se libera la memoria

							//Se muestra las estadisticas en pantalla
		gotoxy(6, 3);
		cout << " -----    -----    -----    -----    -----    -----    -----    -----" << endl;
		gotoxy(5, 23);
		cout << "ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
		gotoxy(5, 24);
		cout << "º TANQUES ENEMIGOS RESTANTES: " << cont_enemigos_destruidos << "  º--" << k << "--" << endl;
		gotoxy(44, 23);
		cout << "ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
		gotoxy(44, 24);
		cout << "º TANQUES ALIADOS RESTANTES: " << cont_aliados_destruidos << "  º" << endl;
		gotoxy(5, 25);
		cout << "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼  " << endl;
		gotoxy(41, 25);
		cout << balas_aliados_disparadas << endl;
		gotoxy(43, 25);
		cout << " ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼" << endl;
		gotoxy(5, 2);
		cout << "Balas Destruidas = " << balas_destruidas << endl;
		gotoxy(35, 22);
		cout << "ÉÍÍDisparosÍ»" << endl;

		if (cont_aliados_destruidos <= 3) // Si la cuenta de tanques aliados se reduce a 3 entonces
		{								// El juego termina y se muestra el mensaje perdedor 

			system("cls");
			//PlaySound(TEXT("WAH.wav"), NULL, SND_ASYNC); // Se reproduce un sonido para perdedor
			system("color 47");
			gotoxy(30, 10);
			cout << " FIN DEL JUEGO " << endl;

			Sleep(150);
			while (g < 3)
			{
				gotoxy(30, 12);
				cout << "P" << endl;
				gotoxy(32, 12); Sleep(100);
				cout << "E" << endl;
				gotoxy(34, 12); Sleep(100);
				cout << "R" << endl;
				gotoxy(36, 12); Sleep(100);
				cout << "D" << endl;
				gotoxy(38, 12); Sleep(100);
				cout << "E" << endl;
				gotoxy(40, 12); Sleep(150);
				cout << "D" << endl;
				gotoxy(42, 12); Sleep(150);
				cout << "O" << endl;
				gotoxy(44, 12); Sleep(150);
				cout << "R" << endl;

				gotoxy(30, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(32, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(34, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(36, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(38, 12); Sleep(100);
				cout << " " << endl;
				gotoxy(40, 12); Sleep(100);
				cout << " " << endl;
				gotoxy(42, 12); Sleep(100);
				cout << " " << endl;
				gotoxy(44, 12); Sleep(150);
				cout << " " << endl;
				g = g + 1;
			}
			gotoxy(29, 12);
			cout << "P E R D E R D O R" << endl;
			gotoxy(29, 15);
			cout << "Digite su nombre: " << endl;
			gotoxy(29, 17);
			cin >> nombre;
			CrearArchivo(nombre, cont_aliados_destruidos, cont_enemigos_destruidos, balas_aliados_disparadas, balas_enemigas_disparadas, balas_destruidas, dificultad);
			c = 1;
		}
		if (cont_enemigos_destruidos <= 3)// Si la cuenta de tanques aliados se reduce a 3 entonces
		{								// El juego termina y se muestra el mensaje ganador 

			system("cls");
			//PlaySound(TEXT("winner.wav"), NULL, SND_ASYNC);//Se reproduce un sonido ganador
			system("color 17");
			gotoxy(29, 10);
			cout << " FIN DEL JUEGO " << endl;

			Sleep(150);
			while (g < 5)
			{
				gotoxy(30, 12);
				cout << "G" << endl;
				gotoxy(32, 12); Sleep(100);
				cout << "A" << endl;
				gotoxy(34, 12); Sleep(100);
				cout << "N" << endl;
				gotoxy(36, 12); Sleep(100);
				cout << "A" << endl;
				gotoxy(38, 12); Sleep(100);
				cout << "D" << endl;
				gotoxy(40, 12); Sleep(150);
				cout << "O" << endl;
				gotoxy(42, 12); Sleep(150);
				cout << "R" << endl;

				gotoxy(30, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(32, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(34, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(36, 12); Sleep(150);
				cout << " " << endl;
				gotoxy(38, 12); Sleep(100);
				cout << " " << endl;
				gotoxy(40, 12); Sleep(100);
				cout << " " << endl;
				gotoxy(42, 12); Sleep(100);
				cout << " " << endl;
				g = g + 1;
			}
			gotoxy(30, 12);
			cout << "G A N A D O R" << endl;
			gotoxy(29, 15);
			cout << "Digite su nombre: " << endl;
			gotoxy(29, 17);
			cin >> nombre;
			CrearArchivo(nombre, cont_aliados_destruidos, cont_enemigos_destruidos, balas_aliados_disparadas, balas_enemigas_disparadas, balas_destruidas, dificultad);
			c = 1;
		}

	}//Fin Ingreso datos Usuario while(c==3)
	Destruir_Cola_Eventos(Balas);//Se destruye la cola de eventos 

}
//FIN de funcion JUEGO

//La funcion menu se encarga de mostrar la interfaz antes de iniciar el juego.
/*
	Se definen la funcion menu() la cual se encarga de mostrar la interfaz del juego
	parametros:
		void
	return:
		void
*/
void menu()
{
	system("color F1");
	system("CLS");
	gotoxy(1, 10);

	cout << "\t\t   ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
	cout << "\t\t   º             MECATANK V2.0           º±" << endl;
	cout << "\t\t   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼±" << endl;
	cout << "\t\t    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±" << endl;

	cout << "\n";
	cout << "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþ                                      þþþþ\n"
		<< "\t\tþþþþ 1-  Jugar                            þþþþ\n"
		<< "\t\tþþþþ 2-  Ver Resultados del ultimo juego  þþþþ\n"
		<< "\t\tþþþþ 3-  Ayuda                            þþþþ\n"
		<< "\t\tþþþþ 4-  Salir                            þþþþ\n"
		<< "\t\tþþþþ                                      þþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
		<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n" << endl;
	int choice;
	gotoxy(1, 26);

	gotoxy(1, 27);

	int tank = 5;
	bool c = true;
	while (c)
	{
		gotoxy(tank, 7);
		cout << "\t\t  /---\\            \n" << endl;
		gotoxy(tank, 8);
		cout << "\t\t /______\\===*         \n" << endl;
		gotoxy(tank, 9);
		cout << "\t\t(__________)        \n" << endl; Sleep(250);
		gotoxy(tank, 7);
		cout << "\t\t                    \n" << endl;
		gotoxy(tank, 8);
		cout << "\t\t                       \n" << endl;
		gotoxy(tank, 9);
		cout << "\t\t                    \n" << endl;
		Sleep(100);
		if (tank == 45) { tank = 5; }
		else { tank = tank + 5; }
		if (kbhit())
		{
			choice = getch();
			if (choice == 49)//Si se presiona 1
			{
				cont_aliados_destruidos = 8;
				cont_enemigos_destruidos = 8;
				juego();
				menu();
			}
			if (choice == 50)//Si se presiona 2
			{
				CargarArchivo();
				menu();
			}
			if (choice == 51)//Si se presiona 3
			{
				system("CLS");
				gotoxy(1, 4);
				cout << "\t\t   ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»" << endl;
				cout << "\t\t   º             AYUDA                   º±" << endl;
				cout << "\t\t   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼±" << endl;
				cout << "\t\t    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±" << endl;

				cout << "\n";
				cout << "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
					<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
					<< "\t\tþþþþ                                      þþþþ\n"
					<< "\t\tþþþþ El objetivo del juego es tratar de   þþþþ\n"
					<< "\t\tþþþþ destruir las letras enemigas presio- þþþþ\n"
					<< "\t\tþþþþ nando la correspondiente letra en el þþþþ\n"
					<< "\t\tþþþþ teclado.                             þþþþ\n"
					<< "\t\tþþþþ Si logras que una bala siga una      þþþþ\n"
					<< "\t\tþþþþ trayectoria limpia hacia el tanque   þþþþ\n"
					<< "\t\tþþþþ enemigo lo destruyes.                þþþþ\n"
					<< "\t\tþþþþ Si logras destruir 5 tanqs enemigos  þþþþ\n"
					<< "\t\tþþþþ ganas, pero si te destruyen 5 tanqs  þþþþ\n"
					<< "\t\tþþþþ pierdes                  .           þþþþ\n"
					<< "\t\tþþþþ                                      þþþþ\n"
					<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n"
					<< "\t\tþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþþ\n" << endl;
				gotoxy(20, 26);
				system("pause");
				menu();
			}
			if (choice == 52)//Si se presiona 4
			{
				gotoxy(25, 25);
				c = false;
				exit(-1);
			}
		}
	}
}

//Programa principal
void main() {
	menu(); // se llama al menu.

}