#include <ncurses.h> //Includo la libreria nCurses per l'interfaccia semigrafica
#include <stdlib.h> //Includo la libreria unistd per utilizzare la funzione exit()
#include <time.h> //Includo la libreria time per la generazione di numeri pseudocasuali
#include <unistd.h> //Includo la libreria unistd per utilizzare la funzione usleep()
#include "engine.h" //Includo l'header del motore del gioco

void InizializzaSchermo() {
	int row, col;
	initscr(); //Credo la finestra di default stdscr
	getmaxyx(stdscr, row, col); //Scrivo nelle variabili row e col rispettivamente le righe e le colonne del terminale in cui è stato eseguito il programma
	if(row>=mondoy+2 && col>=mondox+2) { //Se la risoluzione del terminale è sufficiente per stampare il mondo(compresi i bordi)
		if(has_colors()) { //Se il terminale supporta i colori
			start_color(); //Inizializza i colori
			use_default_colors(); //Utilizzare i colori di default del terminale
		}
		else { //Se il terminale non supporta i colori
			Esci(2); //Esci dal programma stampando l'errore 2
		}
		keypad(stdscr, TRUE); //Abilitare la lettura del keypad
		raw(); //Passa i caratteri di controllo(es. CTRL-Z o CTRL-C) direttamente al programma senza generare un segnale
		noecho(); //Non stampare a schermo i tasti premuti sulla tastiera
		curs_set(0); //Il cursore è invisibile
		timeout(0); //Non aspettare che l'utente prema un tasto per far ritornare un valore alla funzione getch()
	}
	else { //Se la risoluzione del terminale è sbagliata
		Esci(1); //Esci dal programma stampando l'errore 1
	}
}

void Esci(int n) {
	endwin(); //Termina nCurses
	switch(n) { //A seconda del numero dell'errore stampa i seguenti messaggi
		case 0: //Nel caso in cui il numero dell'errore sia 0 non vi è nessun errore
			break;
		case 1:
			printf("Errore 1: La risoluzione del terminale non è almeno di %dX%d\n", mondoy+3, mondox+2);
			break;
		case 2:
			printf("Errore 2: Il terminale non supporta i colori\n");
			break;
	}
	exit(0); //Esci dal programma
}

void LeggiTasti() {
	int ch = getch();
	switch(ch) { //A seconda del tasto premuto fai le seguenti cose
		case 'q': //Se è stato premuto q
			Esci(0); //Esci dal programma senza dare errori
			break;
		case 'p': //Se è stato premuto p
			while(1) { //Entra in un loop
				mvprintw(mondoy+2, mondox-4, "PAUSA");
				if(getch()=='p') { //Se viene ripremuto p
					break; //Esci dal loop
				}
			}
			break;
	}
}

void PulisciMondo(int *mondo) {
	int i, j;
	for(i=0; i<mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		for(j=0; j<mondox; j++) { //Scorri tutte le coordinate X del mondo di gioco
			Morto(mondo, i, j); //Rendi morta la cella corrente
		}	
	}	
}

void RandomizzaMondo(int *mondo) {
	int i, j;
	srand(time(NULL)); //Randomizza usando come seme il tempo
	for(i=0; i<mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		for(j=0; j<mondox; j++) { //Scorri tutte le coordinate X del mondo di gioco
			if(rand()%2) { //Prima possibilità col 50% di probabilità
				Morto(mondo, i, j); //Rendi morta la cella corrente
			}
			else { //Seconda possibilità col 50% di probabilità
				Vivo(mondo, i, j); //Rendi viva la cella corrente
			}
		}	
	}	
}

void Stampa(int *mondo, int generazioni) {
	int i, j;
	clear(); //Pulisce tutto
	for(i=1; i<=mondox; i++) { //Scorri tutte le coordinate X del mondo di gioco
		mvaddch(0, i, ACS_HLINE); //Per ogni coordinata Y crea il bordo superiore
		mvaddch(mondoy+1,i, ACS_HLINE); //Per ogni coordinata Y crea il bordo inferiore
	}
	for(i=1; i<=mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		mvaddch(i, 0, ACS_VLINE); //Per ogni coordinata X crea il bordo a sinistra
		mvaddch(i, mondox+1, ACS_VLINE); //Per ogni coordinata X crea il bordo a destra
	}
	mvaddch(0, 0, ACS_ULCORNER); //Crea l'angolo del bordo in alto a sinistra
	mvaddch(0, mondox+1, ACS_URCORNER); //Crea l'angolo del bordo in alto a destra
	mvaddch(mondoy+1, 0, ACS_LLCORNER); //Crea l'angolo del bordo in basso a sinistra
	mvaddch(mondoy+1, mondox+1, ACS_LRCORNER); //Crea l'angolo del bordo in basso a destra
	for(i=0; i<mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		for(j=0; j<mondox; j++) { //Scorri tutte le coordinate X del mondo di gioco
			if(ControllaCella(mondo, i ,j)) { //Controlle se la cella alle coordinate attuali è viva
				mvaddch(i+1, j+1, 'X'); //Stampa X alle coordinate attuali
			}
			else { //Altrimenti se è morta
				mvaddch(i+1, j+1, ' '); //Stampa SPACE alle coordinate attuali
			}
		}
	}
	mvprintw(mondoy+2, 1, "Generazioni: %d", generazioni); //Stampa il numero la generazione attuale
	refresh(); //Stampa tutto quanto nel terminale
}

void Framerate(int framerate) {
	usleep(1000000/framerate); //Blocca l'intero programma
}

int ContaVicine(int *mondo, int y, int x) {
	int cellevicine = 0;
	int i, j, a, b;
	for(i=y-1; i<=y+1; i++) { //Scorri le righe in cui si trovano le caselle adiacenti alla casella data
		for(j=x-1; j<=x+1; j++) { //Scorri le colonne in cui si trovano le caselle adiacenti alla casella data
			if(i<0) { //Se la casella attuale esce dal mondo a sinistra
				a=i+mondoy; //Appare a destra
			}
			else if(i>=mondoy) { //Se la casella attuale esce dal mondo a destra
				a=i-mondoy; //Appare a sinistra
			}
			else { //Se la casella attuale non esce dal mondo
				a=i;
			}
			if(j<0) { //Se la casella attuale esce dal mondo sopra
				b=j+mondox; //Appare sotto
			}
			else if(j>=mondox) { //Se la casella attuale esce dal mondo sotto
				b=j-mondox; //Appare sopra
			}
			else { //Se la casella attuale non esce dal mondo
				b = j;
			}
			if(ControllaCella(mondo, a, b)) { //Se la cella adiacente attuale è viva
					cellevicine++; //Incrementa il numero di celle vicine
			}
		}
	}
	return cellevicine; //Resituisce il numero di celle vicine
}

void Evolvi(int *mondo) {
	int i, j;
	int mondobuffer[mondoy][mondox]; //Crea un mondo che viene usato come buffer avente le stesse dimensioni del mondo non buffer
	PulisciMondo(*mondobuffer); //Rende morta ogni singola cella
	for(i=0; i<mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		for(j=0; j<mondox; j++) { //Scorri tutte le coordinate X del mondo di gioco
			if(ControllaCella(mondo, i, j)==1){ //Se la cella attuale è viva
				if(ContaVicine(mondo, i, j)-1==2 || ContaVicine(mondo, i, j)-1==3) { //E se la cella attuale ha 2 o 3 celle adiacenti vive
					Vivo(*mondobuffer, i, j); //La cella nel buffer del mondo diventa viva alle stesse coordinate
				}
			}
			else if(ControllaCella(mondo, i, j)==0) { //Se la cella attuale è morta
				if(ContaVicine(mondo, i, j)==3) { //E se la cella attuale ha 3 celle adiacenti vive
					Vivo(*mondobuffer, i, j); //La cella in buffer del mondo diventa viva alle stesse coordinate
				}
			}
		}	
	}
	for(i=0; i<mondoy; i++) { //Scorri tutte le coordinate Y del mondo di gioco
		for(j=0; j<mondox; j++) { //Scorri tutte le coordinate X del mondo di gioco
			*(mondo+i*mondox+j)=mondobuffer[i][j]; //La cella di del mondo alle attuali coordinate diventa uguale alla cella del buffer del mondo alle stesse coordinate
		}	
	}	
}

int ControllaCella(int *mondo, int y ,int x) {
	return *(mondo+y*mondox+x); //Restituisce 0 o 1 a secondo dello stato della cella alle coordinate date
}

void Vivo(int *mondo, int y ,int x) {
	*(mondo+y*mondox+x) = 1; //Rende viva la cella di cui sono date le coordinate
}

void Morto(int *mondo, int y ,int x) {
	*(mondo+y*mondox+x) = 0; //Rende morta la cella di cui sono date le coordinate
}
