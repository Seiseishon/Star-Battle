//LIBRERIAS
#include <iostream>
#include <cstring>
#include <time.h>
#include <string.h>
#include <stdio.h>

using namespace std;

const int MAX = 100;
const int FILA = 10;
const int COL = 10;

typedef char tcad[20];
typedef tcad tnombres[10];
typedef tcad tapellidos[10];

struct tpuntos{
	int mayor_puntaje;
	int puntaje_acum;
};

struct tpeleas{
	int cant_bat_ganadas;
	int cant_bat_disp;
};

struct tcasilla{
	int nave = 7;
	int xenomorfo = 2;
	int yautija = 2;
	int agujero_negro = 2;
	int asteroide = 3;
};

struct tusuario{
	tcad apellido;
	tcad nombre;
	tcad apodo;
	tpuntos puntajes;
	tpeleas batallas;
	tcasilla evento;
};

struct tevento{
	tcad nombre;
	int puntaje;
};

struct ttablero{
	int id_casilla;
	tevento evento;
	bool marca;
	bool evento_asignado;
};

typedef tusuario tjugadores[MAX];
typedef ttablero tmatriz[FILA][COL];

//PROCEDIMIENTOS
//Estos procedimientos es para la gestion de usuarios
void gestionar_jugador(tjugadores jugadores, int &ocup, tusuario jugador);
void agregar_jugador(tjugadores jugadores, int &ocup, tusuario jugador);
void cargar_jugador(tusuario &jugador);
void insertar_jugador(tjugadores jugadores, int &ocup, tusuario jugador);
void automatico(tjugadores jugadores, int &ocup, int cantidad);
void mostrar(tusuario jugador);
void modificar(tusuario &jugador, char opcion);
void eliminar(tjugadores jugadores, int &ocup, tcad apodo);
void listar(tjugadores jugadores, int ocup);
//Estos procedimientos son para la gestion de tablero

void gestionar_tablero(tmatriz tablero, int &ocupF, int &ocupC);
void generar_tablero(tmatriz tablero, int &ocupF, int &ocupC);
void asignar_evento(tmatriz tablero);
void mostrar_tablero(tmatriz tablero, int ocupF, int ocupC);

//Estos procedimientos son para el iniciador de batallas

void juego(tjugadores jugadores, int ocup, tmatriz tablero,int ocupF, int ocupC);
void seleccionar_jugador(tjugadores jugadores, int ocup, tcad apodo, int &posicion);
void batalla(tjugadores jugadores, tmatriz tablero, int ocupF, int ocupC, int aux_posicion, int posicion);
void tablero_inferior(tmatriz tablero);
void tablero_superior(tmatriz tablero);
void ataque_triang_superior(tmatriz tablero, int casilla, tjugadores jugadores, int posicion, int aux_posicion, bool &turno, bool &saltear_turno_jug_1, int &naves_eliminadas_jug_1);
void ataque_triang_inferior(tmatriz tablero, int casilla, tjugadores jugadores, int aux_posicion, int posicion, bool &turno, bool &saltear_turno_jug_2, int &naves_eliminadas_jug_2);
void perder_nave_azar1(tmatriz tablero);
void perder_nave_azar2(tmatriz tablero);

//Estos procedimientos son para el ranking
void intercambio(tusuario &a, tusuario &b);
void shaker_short(tjugadores, int ocup);
void mostrar_ranking(tjugadores jugadores, int ocup);
void ranking(tjugadores jugadores, int ocup);

//FUNCIONES
//Estas funciones es para la gestion de usuarios
bool existe(tjugadores jugadores, int ocup, tusuario jugador);
int buscar(tjugadores jugadores, int ocup, tcad apodo);


//PRINCIPAL
int main() {
	srand(time(NULL));
	
	tmatriz tablero;
	tjugadores players;
	tusuario player;
	int opcion, ocup = -1, ocupF = -1, ocupC = -1;
	
	do{
		system("cls");
		cout << "BIENVENIDOS A START BATTLE" << endl;
		cout << "1) Gestionar jugadores" << endl;
		cout << "2) Gestionar tablero" << endl;
		cout << "3) Jugar" << endl;
		cout << "4) Ranking" << endl;
		cout << "5) Salir" << endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		
		switch(opcion){
		case 1:
			//Para borrar el menu principal
			system("cls");
			gestionar_jugador(players, ocup, player);
			break;
		case 2:
			//Para borrar el menu principal
			system("cls");
			gestionar_tablero(tablero, ocupF,ocupC);
			break;
		case 3:
			if(ocupF == -1 && ocupC == -1)
				cout << "Antes de seleccionar jugadores debe crear el tablero."<<endl;
			else{
				//Para borrar el menu principal
				system("cls");
				juego(players, ocup, tablero, ocupF, ocupC);
				break;
			}
		case 4:
			ranking(players, ocup);
			break;
		case 5:
			cout << "Muchas gracias por jugar start battle. Vuelva pronto" << endl;
			break;
		default:
			cout << "La opcion que eligio no es correcta. Vuelva a intentarlo"<<endl;
			break;
		}
		system("pause");
	} while(opcion != 5);
	
	return 0;
}

int buscar(tjugadores jugadores, int ocup, tcad apodo){
	int minimo, maximo, central;
	bool existe = false;
	
	if(ocup == -1)
		return -1;
	else{
		minimo = 0;
		maximo = ocup;
		while(minimo <= maximo && existe == false){
			central = (minimo + maximo)/2;
			if(strcmp(jugadores[central].apodo,apodo) == 0)
				existe = true;
			else{
				if(strcmp(apodo,jugadores[central].apodo) < 0)
					maximo = central-1;
				else
					minimo = central+1;
			}
		}
		
		if(existe == true)
			return central;
		else
			return -1;
	}
}

bool existe(tjugadores jugadores, int ocup, tusuario jugador){
	bool resultado = false;
	int central,minimo,maximo;
	
	if(ocup == -1)
		return resultado;
	else{
		minimo = 0;
		maximo = ocup;
		
		while(minimo <= maximo && resultado == false){
			central = (minimo + maximo) / 2;
			if(strcmp(jugadores[central].apodo, jugador.apodo) == 0)
				resultado = true;
			else{
				if(strcmp(jugador.apodo,jugadores[central].apodo) < 0)
					maximo = central-1;
				else
					minimo = central+1;
			}
		}
	}
	return resultado;
}
	
void insertar_jugador(tjugadores jugadores, int &ocup, tusuario jugador){
	int i = 0;
	
	if(ocup == MAX-1)
		cout << "Start Battle llego a su limite de jugadores"<<endl;
	else{
		while(i <= ocup && strcmp(jugadores[i].apodo,jugador.apodo) < 0)
			i++;
		
		if(i > ocup){
			ocup++;
			jugadores[ocup] = jugador;
		}else{
			for(int j = ocup; j >= i; j--){
				jugadores[j+1] = jugadores[j];
			}
			jugadores[i] = jugador;
			ocup++;
		}
	}
}

void cargar_jugador(tusuario &jugador){
	do{
		cout << "Ingrese su apellido (Minimo 4 caracteres):"<< endl;
		setbuf(stdin,NULL);
		gets(jugador.apellido);
		cout << "Ingrese su nombre (Minimo 4 caracteres):"<< endl;
		setbuf(stdin,NULL);
		gets(jugador.nombre);
		cout << "Ingrese un apodo (Minimo 4 caracteres):"<< endl;
		setbuf(stdin,NULL);
		gets(jugador.apodo);
		if(strlen(jugador.apellido)<=3 || strlen(jugador.apodo)<=3 || strlen(jugador.nombre)<=3)
			cout << "Algun campo fue nulo o menor de 4 caracteres"<< endl;
	}while(strlen(jugador.apellido)<=3 || strlen(jugador.apodo)<=3 || strlen(jugador.nombre)<=3);

	//Inicializamos los puntajes y batalla en 0, asi cuando
	//el jugador empiece vaya aumentando dependiendo sus actividades
	jugador.puntajes.mayor_puntaje = 0;
	jugador.puntajes.puntaje_acum = 0;
	jugador.batallas.cant_bat_disp = 0;
	jugador.batallas.cant_bat_ganadas = 0;
}

void automatico(tjugadores jugadores, int &ocup, int cantidad){
	int pos;
	int repetido;
	tusuario nuevo;
	
	tnombres nombres = {"Franco", "Gabriel", "Alexis", "Ariel", "Jose", "Laura", "Vanesa", "Lara", "Carla", "Evelin"};
	tapellidos apellidos = {"Sanchez", "Amaya", "Aramayo", "Perez", "Saveedra", "Santander", "Alcaraz", "Leiva", "Ortiz", "Humana"};
	
	for(int i = 0; i < cantidad; i++){
		do{
			pos = rand()%10;
			
			//Guardamos un nombre y apellido al azar de los vectores precargados
			strcpy(nuevo.apellido, apellidos[pos]);
			strcpy(nuevo.nombre, nombres[pos]);
			
			//Guardamos los primeros 3 caracteres de nombre y apellido con un numero al azar de 3 cifras para apodo
			nuevo.apodo[0]=nuevo.apellido[0];
			nuevo.apodo[1]=nuevo.apellido[1];
			nuevo.apodo[2]=nuevo.apellido[2];
			nuevo.apodo[3]=nuevo.nombre[0];
			nuevo.apodo[4]=nuevo.nombre[1];
			nuevo.apodo[5]=nuevo.nombre[2];
			nuevo.apodo[6]=(rand()%(9-0) +0)+48;
			nuevo.apodo[7]=(rand()%(9-0) +0)+48;
			nuevo.apodo[8]=(rand()%(9-0) +0)+48;
			
			nuevo.apodo[9]='\0';
			
			//Inicializamos batallas y puntaje en 0
			nuevo.puntajes.mayor_puntaje = 0;
			nuevo.puntajes.puntaje_acum = 0;
			nuevo.batallas.cant_bat_disp = 0;
			nuevo.batallas.cant_bat_ganadas = 0;
			
			repetido = buscar(jugadores,ocup,nuevo.apodo);
			
			if(repetido != -1)
				cout << "Me repeti"<<endl;
			
		} while(repetido != -1);
		
		insertar_jugador(jugadores, ocup, nuevo);
	}
}

void modificar(tusuario &jugador, char opcion){
	switch(opcion){
	case 'a':
		do{
			cout << "Ingrese el nuevo nombre(Minimo 4 caracteres)" << endl;
			setbuf(stdin,NULL);
			gets(jugador.nombre);
			
			if(strlen(jugador.nombre) <= 3)
				cout << "No se puede guardar el nombre con menos de 4 caracteres"<< endl;
		} while(strlen(jugador.nombre) <= 3);
		
		cout << "Guardando nuevo nombre..."<<endl;
		break;
	case 'b':
		do{
			cout << "Ingrese el nuevo apellido(Minimo 4 caracteres)" << endl;
			setbuf(stdin,NULL);
			gets(jugador.apellido);
			
			if(strlen(jugador.nombre) <= 3)
				cout << "No se puede guardar el apellido con menos de 4 caracteres"<< endl;
		} while(strlen(jugador.nombre) <= 3);
		
		cout << "Guardando nuevo apellido..."<<endl;
		break;
	case 'c':
		do{
			cout << "Ingrese el nuevo apellido(Minimo 4 caracteres)" << endl;
			setbuf(stdin,NULL);
			gets(jugador.apellido);
			cout << "Ingrese el nuevo nombre(Minimo 4 caracteres)" << endl;
			setbuf(stdin,NULL);
			gets(jugador.nombre);
			
			if(strlen(jugador.nombre) <= 3 || strlen(jugador.apellido) <= 3)
				cout << "No se puede guardar el apellido o el nombre con menos de 4 caracteres"<< endl;
		} while(strlen(jugador.nombre) <= 3 || strlen(jugador.apellido) <= 3);
		
		cout << "Guardando los cambios..."<<endl;
		break;
	case 'd':
		cout << "Saliendo..."<<endl;
		break;
	default:
		cout << "Opcion Invalida. Intentelo de nuevo mas tarde"<<endl;
		break;
	}
}

void agregar_jugador(tjugadores jugadores,int &ocup, tusuario jugador){
	char opcion;
	int cant;
	
	do{
		system("cls");
		
		cout << "a) Cargar usuario de manera manual" << endl;
		cout << "b) Cargar usuarios de manera automatica" << endl;
		cout << "c) Salir"<< endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		opcion = tolower(opcion); // Esto convierte en minuscula
		
		switch(opcion){
		case 'a':
			cout << "CARGANDO USUARIO:" << endl;
			
			cargar_jugador(jugador);
			cout << "Verificando datos..."<<endl;
			if(existe(jugadores, ocup, jugador) == false)
				insertar_jugador(jugadores, ocup, jugador);
			else
				cout << "El apodo que ingreso ya existe"<<endl;
			break;
		case 'b':
			do{
				cout << "Cuantos jugadores quieres crear de forma automatica:" << endl;
				cout << "Maximo permitido 100 o la cantidad disponible si es que se hizo cargas anteriormente" << endl;
				cin >> cant;
				
				if(cant > 100 || ocup + cant >= MAX)
					cout << "Supero el limite permitido";
			}while(cant > 100 || ocup + cant >= MAX);
			
			cout << "Cargando jugadores..."<< endl;
			system("pause");
			automatico(jugadores, ocup, cant);
			cout << "Jugadores cargados con exito"<< endl;
			break;
		case 'c':
			cout <<"Saliendo del menu de agregar..."<< endl;
			break;
		default:
			cout << "Opcion Invalida"<< endl;
			break;
		}
		system("pause");
	}while(opcion != 'c');
}
	
void eliminar(tjugadores jugadores, int &ocup, tcad apodo){
	int pos;
	char op;
	
	pos = buscar(jugadores, ocup, apodo);
	if(pos == -1)
		cout << "Este jugador no existe"<<endl;
	else{
		cout << "Jugador encontrado."<<endl;
		mostrar(jugadores[pos]);
		cout << endl<<"Seguro quieres eliminarlo? (s/n)"<<endl;
		cin >> op;
		op = tolower(op);
		if(op == 's'){
			cout << "Eliminando jugador. Por favor espere..."<<endl;
			system("pause");
			for(int i = pos; i <= ocup; i++){
				jugadores[i] = jugadores[i+1];
			}
			ocup--;
			cout << "Se elimino el jugador"<<endl;
		}else{
			cout << "No se elimino ningun jugador"<<endl;
		}
	}
}
	
void mostrar(tusuario jugador){
	cout << "Nombre:" << jugador.nombre<< endl;
	cout << "Apellido:" << jugador.apellido<< endl;
	cout << "Apodo:" << jugador.apodo<< endl;
	cout << "PUNTAJES DEL JUGADOR:"<< endl;
	cout << "Mayor puntaje:" << jugador.puntajes.mayor_puntaje<< endl;
	cout << "Puntaje acumulado:" << jugador.puntajes.puntaje_acum<< endl;
	cout << "BATALLAS DEL JUGADOR:"<< endl;
	cout << "Cantidad de batallas disputadas:" << jugador.batallas.cant_bat_disp<< endl;
	cout << "Cantidad de batallas ganadas:" << jugador.batallas.cant_bat_ganadas<< endl;
	cout << "\n";
}
	
void listar(tjugadores jugadores, int ocup){
	if(ocup == -1)
		cout << "No hay jugadores"<<endl;
	else{
		if(ocup == 0)
			mostrar(jugadores[ocup]);
		else{
			listar(jugadores, ocup-1);
			mostrar(jugadores[ocup]);
		}
	}
}
	
void gestionar_jugador(tjugadores jugadores, int &ocup, tusuario jugador){
	int opcion, pos;
	char op;
	tcad apodo;
	
	do{
		system("cls");
		
		cout << "BIENVENIDO A LA GESTION DE JUGADORES:"<<endl;
		cout << "Que deseas realizar?"<<endl;
		cout << "1) Agregar jugador" << endl;
		cout << "2) Consultar jugador" << endl;
		cout << "3) Modificar jugador"<<endl;
		cout << "4) Eliminar jugador" <<endl;
		cout << "5) Listar jugador"<< endl;
		cout << "6) Salir" << endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		
		switch(opcion){
		case 1:
			//Para borrar el menu y mostrar limpio el otro
			system("cls");
			agregar_jugador(jugadores, ocup, jugador);
			break;
		case 2:
			cout << "Ingrese el apodo del jugador que quiere consultar" << endl;
			setbuf(stdin,NULL);
			gets(apodo);
			cout << "Gracias, lo estamos buscando..."<< endl;
			system("pause");
			pos = buscar(jugadores,ocup, apodo);
			if(pos == -1)
				cout << "No existe jugador"<<endl;
			else{
				cout << "Jugador encontrado"<<endl;
				mostrar(jugadores[pos]);
			}
			system("pause");
			break;
		case 3:
			cout << "Ingrese el apodo del jugador que quiere consultar"<<endl;
			setbuf(stdin,NULL);
			gets(apodo);
			cout << "Gracias, lo estamos buscando..."<< endl;
			system("pause");
			pos = buscar(jugadores, ocup, apodo);
			if(pos == -1)
				cout << "No existe este jugador" << endl;
			else{
				cout << "Jugador encontrado"<< endl;
				mostrar(jugadores[pos]);
				
				cout << "Solo puedes cambiar nombre y apellido"<<endl;
				cout << "a)Cambiar solo nombre"<< endl;
				cout << "b)Cambiar solo apellido"<<endl;
				cout << "c)Cambiar ambos"<<endl;
				cout << "d)Salir y no cambiar nada"<<endl;
				cout << "Elige una opcion: ";
				cin >> op;
				op = tolower(op);
				modificar(jugadores[pos], op);
			}
			system("pause");
			break;
		case 4:
			cout << "Ingrese el apodo del jugador para eliminar"<< endl;
			setbuf(stdin,NULL);
			gets(apodo);
			eliminar(jugadores, ocup, apodo);
			system("pause");
			break;
		case 5:
			listar(jugadores, ocup);
			system("pause");
			break;
		case 6:
			cout << "Saliendo de la gestion..." << endl;
			break;
		default:
			cout << "La opcion que eligio no es correcta. Vuelva a intentarlo"<<endl;
			break;
		}
	}while(opcion != 6);
}
	
void generar_tablero(tmatriz tablero, int &ocupF, int &ocupC){
	int k=0;
	
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i < j){
				tablero[i][j].id_casilla = k+1;
				tablero[i][j].evento_asignado = false;
				k++;
			}
			if(i==j)
			   strcpy(tablero[i][j].evento.nombre, " ");
		}
	}
	
	k=0;
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i > j){
				tablero[i][j].id_casilla = k+1;
				tablero[i][j].evento_asignado = false;
				k++;
			}
		}
	}
	
	ocupF = FILA-1;
	ocupC = COL-1;
	
	asignar_evento(tablero);
}
	
void asignar_evento(tmatriz tablero){
	int i,j;
	
	
	//Eventos para la triangular superior
	for(int nave = 0; nave < 7; nave++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i >= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[N]");
		tablero[i][j].evento.puntaje = 10;
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int xenomorfo = 0; xenomorfo < 2; xenomorfo++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i >= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[X]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int yautja = 0; yautja < 2; yautja++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i >= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[Y]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	
	for(int agujero = 0; agujero < 2; agujero++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i >= j || tablero[i][j].evento_asignado);
		
		//H = agujero negro para no confundir A de asteroide
		strcpy(tablero[i][j].evento.nombre, "[H]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	
	for(int asteroide = 0; asteroide < 3; asteroide++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i >= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[A]");
		tablero[i][j].evento.puntaje = 2;
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i<j && !tablero[i][j].evento_asignado){
				strcpy(tablero[i][j].evento.nombre, "[ ]");
				tablero[i][j].evento.puntaje = -2;
				tablero[i][j].evento_asignado = true;
				tablero[i][j].marca = false;
			}
		}
	}
	
	
	//Eventos para la triangular inferior
	for(int nave = 0; nave < 7; nave++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i <= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[N]");
		tablero[i][j].evento.puntaje = 10;
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int xenomorfo = 0; xenomorfo < 2; xenomorfo++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i <= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[X]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int yautja = 0; yautja < 2; yautja++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i <= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[Y]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	
	for(int agujero = 0; agujero < 2; agujero++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i <= j || tablero[i][j].evento_asignado);
		
		//H = agujero negro para no confundir A de asteroide
		strcpy(tablero[i][j].evento.nombre, "[H]");
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	
	for(int asteroide = 0; asteroide < 3; asteroide++){
		do{
			i = rand()%10;
			j = rand()%10;
		} while(i <= j || tablero[i][j].evento_asignado);
		
		strcpy(tablero[i][j].evento.nombre, "[A]");
		tablero[i][j].evento.puntaje = 2;
		tablero[i][j].evento_asignado = true;
		tablero[i][j].marca = false;
	}
	
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i>j && !tablero[i][j].evento_asignado){
				strcpy(tablero[i][j].evento.nombre, "[ ]");
				tablero[i][j].evento.puntaje = -2;
				tablero[i][j].evento_asignado = true;
				tablero[i][j].marca = false;
			}
		}
	}
}

void mostrar_tablero(tmatriz tablero, int ocupF, int ocupC){
	if(ocupC == -1 && ocupF == -1)
		cout << "No se genero ningun tablero"<<endl;
	else{
		cout << "\n===== TABLERO =====\n"<<endl;
		for(int i = 0; i <= ocupF; i++){
			for(int j = 0; j <= ocupC; j++){
				cout << tablero[i][j].evento.nombre << "\t";
			}
			cout << endl;
		}
	}
}
	
void gestionar_tablero(tmatriz tablero, int &ocupF, int &ocupC){
	char opcion;
	
	do{
		system("cls");
		cout << "BIENVENIDO A LA GENSTION DEL TABLERO \t"<<endl;
		cout << "a) Generar tablero"<<endl;
		cout << "b) Mostrar el tablero"<<endl;
		cout << "c) Salir de la gestion de tablero"<<endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		opcion = tolower(opcion);
		
		switch(opcion){
		case 'a':
			generar_tablero(tablero,ocupF,ocupC);
			system("pause");
			break;
		case 'b':
			mostrar_tablero(tablero, ocupF, ocupC);
			system("pause");
			break;
		case 'c':
			cout << "Saliendo..."<<endl;
			break;
		default:
			cout << "Opcion invalida"<<endl;
			system("pause");
			break;
		}
	}while(opcion != 'c');
}

void seleccionar_jugador(tjugadores jugadores, int ocup, tcad apodo, int &posicion){
	char elegir;

	posicion = buscar(jugadores, ocup, apodo);
	
	if(posicion == -1){
		cout << "No existe jugador con este apodo"<<endl;
		system("pause");
	}
	else{
		mostrar(jugadores[posicion]);
		
		cout << "Estas seguro que quieres seleccionar este jugador (s/n)"<<endl;
		cin >> elegir;
		elegir = tolower(elegir);
		
		if(elegir == 's')
			cout << "Jugador seleccionado.Gracias por confirmar."<<endl;
		else{
			posicion = -1;
			cout << "Jugador no seleccionado.Gracias por confirmar."<<endl;
		}
	}
}
	
void tablero_inferior(tmatriz tablero){
	cout << "\t\t ======== TABLERO ======== \n";
	cout << endl;
	
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i>j)
				cout << tablero[i][j].evento.nombre<<"\t";
			else
				//Esto mostrara al usuario atacante los numero de casilla del usuario atacado
				//Para que el solo ingrese el numero del 1 al 45 y ataque dicha casilla con ese id
				if(i<j)
				cout<< "[" << tablero[i][j].id_casilla << "]"<<"\t";
				else
					cout << "-\t";
		}
		cout << endl;
	}
}
	
void tablero_superior(tmatriz tablero){
	cout << "\t\t ======== TABLERO ======== \n";
	cout << endl;
	
	for(int i = 0; i < FILA; i++){
		for(int j = 0; j < COL; j++){
			if(i<j)
				cout << tablero[i][j].evento.nombre<<"\t";
			else
				//Esto mostrara al usuario atacante los numero de casilla del usuario atacado
				//Para que el solo ingrese el numero del 1 al 45 y ataque dicha casilla con ese id
				if(i>j)
					cout<< "[" << tablero[i][j].id_casilla << "]"<<"\t";
				else
					cout << "-\t";
		}
		cout << endl;
	}
}	
	
void ataque_triang_superior(tmatriz tablero, int casilla, tjugadores jugadores, int posicion, int aux_posicion, bool &turno, bool &saltear_turno_jug_1, int &naves_eliminadas_jug_1){
	int i,j;
	bool existe = false;
	
	i = 0;
	
	while(i < FILA && existe==false){
		j=0;
		while(j < COL && existe==false){
			if(j>i){
				if(tablero[i][j].id_casilla == casilla && tablero[i][j].marca == false){
					existe = true;
					tablero[i][j].marca = true;
					turno = false;
					
					if(strcmp(tablero[i][j].evento.nombre, "[N]") == 0){
						jugadores[aux_posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
						jugadores[posicion].evento.nave--;
						naves_eliminadas_jug_1++;
						strcpy(tablero[i][j].evento.nombre, "[-]");
						cout << "Haz destruido una nave"<<endl;
						cout << "Sumas 10 puntos!"<<endl;
						system("pause");
					}else{
					   if(strcmp(tablero[i][j].evento.nombre, "[X]") == 0){
						   jugadores[aux_posicion].evento.nave--;
						   strcpy(tablero[i][j].evento.nombre, "[-]");
						   perder_nave_azar2(tablero);
						   cout << "Tu ataque a caido en un Xenomorfo..."<<endl;
						   cout << "Perdiste una nave."<<endl;
						   system("pause");
						}else{
						   if(strcmp(tablero[i][j].evento.nombre, "[Y]") == 0){
								saltear_turno_jug_1 = true;
								strcpy(tablero[i][j].evento.nombre, "[-]");
								cout << "Oh no, atacaste a un Yautija y fuiste secuestrado."<<endl;
								cout << "Perderas un turno hasta que puedas salvarte del Yautija"<<endl;
								system("pause");
							}else{
							   if(strcmp(tablero[i][j].evento.nombre, "[H]") == 0){
								   jugadores[aux_posicion].puntajes.puntaje_acum = 0;
								   strcpy(tablero[i][j].evento.nombre, "[-]");
								   cout << "Oh no, en tu ataque te topaste con un agujero negro"<<endl;
								   cout << "Pierdes todos tus puntos"<<endl;
								   system("pause");
								}else{
								   if(strcmp(tablero[i][j].evento.nombre, "[A]") == 0){
									   jugadores[aux_posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
									   strcpy(tablero[i][j].evento.nombre, "[-]");
									   cout << "Haz destruido un asteroide"<<endl;
									   cout << "Sumas 2 puntos!"<<endl;
									   cout << "Mandaste una lluvia de meteoritos a tu enemigo"<<endl;
									   system("pause");
									}else{
									   if(strcmp(tablero[i][j].evento.nombre, "[ ]") == 0){
										   if(jugadores[aux_posicion].puntajes.puntaje_acum -2 < 0)
											   jugadores[aux_posicion].puntajes.puntaje_acum = 0;
										   else
											   jugadores[aux_posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
										   strcpy(tablero[i][j].evento.nombre, "[-]");
										   cout << "Lamentablemente no encontramos a ningun objetivo en este ataque."<<endl;
										   cout << "Pierdes 2 puntos."<<endl;
										   system("pause");
										}
									}
								}
							}
						}
					}
				}
			}
			j++;
		}
		i++;
	}
}

void ataque_triang_inferior(tmatriz tablero, int casilla, tjugadores jugadores, int aux_posicion, int posicion, bool &turno, bool &saltear_turno_jug_2, int &naves_eliminadas_jug_2){
	int i,j;
	bool existe = false;
	
	i = 0;
	
	while(i < FILA && existe==false){
		j=0;
		while(j < COL && existe==false){
			if(i>j){
				if(tablero[i][j].id_casilla == casilla && tablero[i][j].marca == false){
					existe = true;
					tablero[i][j].marca = true;
					turno = false;
					
					if(strcmp(tablero[i][j].evento.nombre, "[N]") == 0){
						jugadores[posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
						jugadores[aux_posicion].evento.nave--;
						naves_eliminadas_jug_2++;
						strcpy(tablero[i][j].evento.nombre, "[-]");
						cout << "Haz destruido una nave"<<endl;
						cout << "Sumas 10 puntos!"<<endl;
						system("pause");
					}else{
						if(strcmp(tablero[i][j].evento.nombre, "[X]") == 0){
							jugadores[posicion].evento.nave--;
							strcpy(tablero[i][j].evento.nombre, "[-]");
							perder_nave_azar1(tablero);
							cout << "Tu ataque a caido en un Xenomorfo..."<<endl;
							cout << "Perdiste una nave."<<endl;
							system("pause");
						}else{
							if(strcmp(tablero[i][j].evento.nombre, "[Y]") == 0){
								saltear_turno_jug_2 = true;
								strcpy(tablero[i][j].evento.nombre, "[-]");
								cout << "Oh no, atacaste a un Yautija y fuiste secuestrado."<<endl;
								cout << "Perderas un turno hasta que puedas salvarte del Yautija"<<endl;
								system("pause");
							}else{
								if(strcmp(tablero[i][j].evento.nombre, "[H]") == 0){
									jugadores[posicion].puntajes.puntaje_acum = 0;
									strcpy(tablero[i][j].evento.nombre, "[-]");
									cout << "Oh no, en tu ataque te topaste con un agujero negro"<<endl;
									cout << "Pierdes todos tus puntos"<<endl;
									system("pause");
								}else{
									if(strcmp(tablero[i][j].evento.nombre, "[A]") == 0){
										jugadores[posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
										strcpy(tablero[i][j].evento.nombre, "[-]");
										cout << "Haz destruido un asteroide"<<endl;
										cout << "Sumas 2 puntos!"<<endl;
										cout << "Mandaste una lluvia de meteoritos a tu enemigo"<<endl;
										system("pause");
									}else{
										if(strcmp(tablero[i][j].evento.nombre, "[ ]") == 0){
											if(jugadores[posicion].puntajes.puntaje_acum -2 < 0)
												jugadores[posicion].puntajes.puntaje_acum = 0;
											else
												jugadores[posicion].puntajes.puntaje_acum += tablero[i][j].evento.puntaje;
											strcpy(tablero[i][j].evento.nombre, "[-]");
											cout << "Lamentablemente no encontramos a ningun objetivo en este ataque."<<endl;
											cout << "Pierdes 2 puntos."<<endl;
											system("pause");
										}
									}
								}
							}
						}
					}
				}
			}
			j++;
		}
		i++;
	}
}

void batalla(tjugadores jugadores, tmatriz tablero, int ocupF, int ocupC, int aux_posicion, int posicion){
	bool turno, saltear_turno_jug_1 = false, saltear_turno_jug_2 = false;
	int naves_eliminadas_jug_1 = 0, naves_eliminadas_jug_2 = 0, casilla;
	
	if((ocupF == -1 && ocupC == -1) || posicion == -1 || aux_posicion == -1)
		cout << "No se puede iniciar una batalla si no hay tablero generado o si no hay jugadores seleccionados"<<endl;
	else{
		cout << "Iniciando batalla..." << endl;
		system("pause");
		system("cls");
		
		cout << "=============== REGLAS DEL JUEGO ===============" << endl << endl;
		
		cout << "Cada jugador tiene:" << endl;
		cout << "   - 7 Naves" << endl;
		cout << "   - 2 Xenomorfos" << endl;
		cout << "   - 2 Yautjas" << endl;
		cout << "   - 2 Agujeros negros" << endl;
		cout << "   - 3 Asteroides" << endl;
		cout << "   - El resto del tablero se completa con vac�os espaciales" << endl << endl;
		
		cout << "Al atacar una casilla puedes:" << endl;
		cout << "   _Destruir una nave enemiga -> +10 puntos" << endl;
		cout << "   _Atacar un Xenomorfo       -> Pierdes 1 nave" << endl;
		cout << "   _Atacar un Yautja          -> Pierdes el turno" << endl;
		cout << "   _Caer en un agujero negro  -> Puntaje acumulado se reinicia a 0" << endl;
		cout << "   _Destruir un asteroide     -> +2 puntos y lluvia de meteoritos" << endl;
		cout << "   _Caer en un vacio espacial -> -2 puntos" << endl;
		
		cout << "\n Buena suerte, comandante!" << endl << endl;
		system("pause");
		
		//Reiniciamos a 0 los puntajes acumulados de los jugadores
		jugadores[aux_posicion].puntajes.puntaje_acum = 0;
		jugadores[posicion].puntajes.puntaje_acum = 0;
		
		while(jugadores[aux_posicion].evento.nave != 0 && jugadores[posicion].evento.nave != 0){
			turno = true;
			do{
				//Datos que se mostraran durante la batalla si es su turno
				system("cls");
				cout << "Turno Del jugador 1: \n"<<endl;
				system("pause");
				cout << "Apodo: "<< jugadores[aux_posicion].apodo<<endl;
				cout << "Puntaje: " <<jugadores[aux_posicion].puntajes.puntaje_acum<<endl;
				cout << "Naves disponibles: " <<jugadores[aux_posicion].evento.nave<<endl;
				cout << "Enemigos derrotados: " <<naves_eliminadas_jug_1<<endl;
				cout << endl;
				
				//controlamos si anteriormente en su ataque fue hacia un Yautija
				if(saltear_turno_jug_1){
					cout << "No podras jugar porque estas secuestrado por los Yautija"<<endl;
					turno = false;
					saltear_turno_jug_1 = false;
					system("pause");
				}else{
					tablero_inferior(tablero);
					
					do{
						cout << "Ingresa la casilla que quieres atacar (1 al 45)"<<endl;
						cin >> casilla;
						
						cout << "Verificando que el numero ingresado sea valido..."<<endl;
						system("pause");
						
						if(casilla < 1 || casilla > 45)
							cout << "Numero invalido de casilla"<<endl;
					} while(casilla < 1 || casilla > 45);
					
					cout << "Atacando..."<<endl;
					system("pause");
					
					ataque_triang_superior(tablero, casilla, jugadores, posicion, aux_posicion, turno, saltear_turno_jug_1, naves_eliminadas_jug_1);
					
					if(turno == true){
						cout << "La casilla seleccionada ya se jugo antes"<<endl;
						system("pause");
					}
				}
				
			} while(turno == true);
			
			//Volvemos a poner turno en true para el segundo jugador 
			turno = true;
			
			do{
				system("cls");
				cout << "Turno Del jugador 2: \n"<<endl;
				system("pause");
				cout << "Apodo: "<< jugadores[posicion].apodo<<endl;
				cout << "Puntaje: " <<jugadores[posicion].puntajes.puntaje_acum<<endl;
				cout << "Naves disponibles: " <<jugadores[posicion].evento.nave<<endl;
				cout << "Enemigos derrotados: " <<naves_eliminadas_jug_2<<endl;
				cout << endl;
				
				//controlamos si anteriormente en su ataque fue hacia un Yautija
				if(saltear_turno_jug_2){
					cout << "No podras jugar porque estas secuestrado por los Yautija"<<endl;
					turno = false;
					saltear_turno_jug_2 = false;
					system("pause");
				}else{
					tablero_superior(tablero);
					
					do{
						cout << "Ingresa la casilla que quieres atacar (1 al 45)"<<endl;
						cin >> casilla;
						
						cout << "Verificando que el numero ingresado sea valido..."<<endl;
						system("pause");
						
						if(casilla < 1 || casilla > 45)
							cout << "Numero invalido de casilla"<<endl;
					} while(casilla < 1 || casilla > 45);
					
					cout << "Atacando..."<<endl;
					system("pause");
					
					ataque_triang_inferior(tablero, casilla, jugadores, aux_posicion, posicion, turno, saltear_turno_jug_2, naves_eliminadas_jug_2);
					
					if(turno == true){
						cout << "La casilla seleccionada ya se jugo antes"<<endl;
						system("pause");
					}
				}
				
			} while(turno == true);
		}
		
		system("cls");
		
		cout << "La batalla ha terminado!!"<<endl;
		cout << "Uno de los jugadores se quedo sin naves!!"<<endl;
		
		system("pause");
		system("cls");
		
		if(jugadores[aux_posicion].evento.nave > 0 && jugadores[posicion].evento.nave == 0){
			if(jugadores[aux_posicion].puntajes.puntaje_acum > 0){
				jugadores[aux_posicion].batallas.cant_bat_ganadas++;
				cout << "El ganador es: " <<jugadores[aux_posicion].apodo << endl;
				if(jugadores[aux_posicion].puntajes.puntaje_acum > jugadores[aux_posicion].puntajes.mayor_puntaje){
					jugadores[aux_posicion].puntajes.mayor_puntaje = jugadores[aux_posicion].puntajes.puntaje_acum;
				}
				jugadores[aux_posicion].batallas.cant_bat_disp++;
				jugadores[posicion].batallas.cant_bat_disp++;
			}else
			   cout << "Hubo un empate debido que el jugador con naves disponibles tiene 0 puntos acumulados."<<endl;
		}
		
		if(jugadores[posicion].evento.nave > 0 && jugadores[aux_posicion].evento.nave == 0){
			if(jugadores[posicion].puntajes.puntaje_acum > 0){
				jugadores[posicion].batallas.cant_bat_ganadas++;
				cout << "El ganador es: " <<jugadores[posicion].apodo << endl;
				if(jugadores[posicion].puntajes.puntaje_acum > jugadores[posicion].puntajes.mayor_puntaje){
					jugadores[posicion].puntajes.mayor_puntaje = jugadores[posicion].puntajes.puntaje_acum;
				}
				jugadores[aux_posicion].batallas.cant_bat_disp++;
				jugadores[posicion].batallas.cant_bat_disp++;
			}else
			   cout << "Hubo un empate debido que el jugador con naves disponibles tiene 0 puntos acumulados."<<endl;
		}
		
		if(jugadores[posicion].evento.nave == 0 && jugadores[aux_posicion].evento.nave == 0){
			jugadores[posicion].batallas.cant_bat_disp++;
			jugadores[aux_posicion].batallas.cant_bat_disp++;
			cout << "Ambos jugadores quedaron sin naves. Es un empate"<<endl;
		}
	}
}
	
void juego(tjugadores jugadores, int ocup,tmatriz tablero, int ocupF, int ocupC){
	char opcion;
	tcad apodo;
	int posicion = -1, aux_posicion = -1;
	
	do{
		system("cls");
		cout << "BIENVENIDO AL INICIADOR DE BATALLA \t" << endl;
		cout << "a) Elegir los jugadores para la batalla"<<endl;
		cout << "b) Iniciar batalla"<<endl;
		cout << "c) Salir"<<endl;
		cout << "Elige una opcion: ";
		cin >> opcion;
		opcion = tolower(opcion);
		
		switch(opcion){
		case 'a':
			if(ocup <= 0){
				cout << "No hay jugadores suficientes. Deben haber minimo 2 jugadores registrados" << endl;
				system("pause");
				break;
			} else {
				// Se repite cada vez que no se haya seleccionado a un jugador
				do {
					system("cls");
					listar(jugadores,ocup);
					cout << "Ingresa el apodo del primer jugador" << endl;
					setbuf(stdin, NULL);
					gets(apodo);
					seleccionar_jugador(jugadores, ocup, apodo, posicion);
					aux_posicion = posicion;
				} while (aux_posicion == -1);
				
				cout << "Guardando el primer jugador..." << endl;
				system("pause");
				
				// Segundo jugador
				do {
					system("cls");
					listar(jugadores,ocup);
					cout << "Ingresa el apodo del segundo jugador" << endl;
					setbuf(stdin, NULL);
					gets(apodo);
					seleccionar_jugador(jugadores, ocup, apodo, posicion);
					
					//Verificamos si el segundo jugador sea diferente al primero
					if(strcmp(jugadores[aux_posicion].apodo, jugadores[posicion].apodo) == 0)
						cout << "No se puede seleccionar el mismo jugador"<<endl;
				} while (posicion == -1 || strcmp(jugadores[aux_posicion].apodo, jugadores[posicion].apodo) == 0);
				
				cout << "Guardando el segundo jugador..." << endl;
				system("pause");
				
				system("cls");
				cout << "Estos son los jugadores que se seleccionaron" << endl;
				cout << "Jugador uno: \n";
				mostrar(jugadores[aux_posicion]);
				cout << "Jugador dos:" << endl;
				mostrar(jugadores[posicion]);
				system("pause");
			}
			break;
		case 'b':
			batalla(jugadores, tablero, ocupF, ocupC, aux_posicion, posicion);
			system("pause");
			break;
		case 'c':
			cout << "Saliendo..."<<endl;
			break;
		default:
			cout << "Opcion invalida"<<endl;
			system("pause");
			break;
		}
	}while(opcion != 'c');
}
	
void intercambio(tusuario &a, tusuario &b){
	tusuario aux;
	aux=a;
	a=b;
	b=aux;
}
	
void shaker_short(tjugadores jugadores, int ocup){
	//Esto ordenara de forma decreciente asi muestra del top 1 al ultimo
	
	int ultimo, primero;
	bool cambio = true;
	
	primero = 0;
	ultimo = ocup;
	
	while(cambio){
		cambio = false;
		for(int i = ultimo; i > primero; i--){
			if(jugadores[i].puntajes.mayor_puntaje > jugadores[i-1].puntajes.mayor_puntaje){
				cambio = true;
				//funciona para intercambiar de lugares los valores
				intercambio(jugadores[i], jugadores[i-1]);
			}
		}
		
		if(cambio)
			cambio = false;
		primero++;
		
		for(int j = primero; j < ultimo; j++){
			if(jugadores[j].puntajes.mayor_puntaje < jugadores[j+1].puntajes.mayor_puntaje){
				cambio = true;
				intercambio(jugadores[j], jugadores[j+1]);
			}
		}
		ultimo++;
	}
}

void mostrar_ranking(tjugadores jugadores, int ocup){
	//caso base
	
	if(ocup == 0){
		if(jugadores[0].batallas.cant_bat_ganadas > 0){
			cout << "------------------------------\n";
			cout << "Apodo: " << jugadores[0].apodo << endl;
			cout << "Mayor Puntaje: " << jugadores[0].puntajes.mayor_puntaje << endl;
			cout << "Batallas ganadas: " << jugadores[0].batallas.cant_bat_ganadas << endl;
		}
	} else {
		mostrar_ranking(jugadores, ocup - 1);
		if(jugadores[ocup].batallas.cant_bat_ganadas > 0){
			cout << "------------------------------\n";
			cout << "Apodo: " << jugadores[ocup].apodo << endl;
			cout << "Mayor Puntaje: " << jugadores[ocup].puntajes.mayor_puntaje << endl;
			cout << "Batallas ganadas: " << jugadores[ocup].batallas.cant_bat_ganadas << endl;
		}
	}
}
	
void ranking(tjugadores jugadores, int ocup){
	//Verificamos que haya al menos 2 jugadores para hacer un ranking
	
	if(ocup <= 0)
		cout << "No hay suficientes jugadores para mostrar un ranking."<<endl;
	else{
		//Primero ordenamos con el modulo shaker_short
		
		shaker_short(jugadores, ocup);
		cout << "=================== RANKING ==================="<<endl;
		mostrar_ranking(jugadores,ocup);
	}
}

void perder_nave_azar1(tmatriz tablero){
	int i,j;
	bool encontrado = false;
	
	i=0;
	
	while(i < FILA && encontrado == false){
		j=0;
		while(j < COL && encontrado == false){
			if(j>i){
				if(strcmp(tablero[i][j].evento.nombre, "[N]") == 0){
					strcpy(tablero[i][j].evento.nombre, "[-]");
					tablero[i][j].marca = true;
					encontrado = true;
				}
			}
			j++;
		}
		i++;
	}
}
	
void perder_nave_azar2(tmatriz tablero){
	int i,j;
	bool encontrado = false;
	
	i=0;
	
	while(i < FILA && encontrado == false){
		j=0;
		while(j < COL && encontrado == false){
			if(i>j){
				if(strcmp(tablero[i][j].evento.nombre, "[N]") == 0){
					strcpy(tablero[i][j].evento.nombre, "[-]");
					tablero[i][j].marca = true;
					encontrado = true;
				}
			}
			j++;
		}
		i++;
	}
}
	
