/*
	+-------------------------------------------------------------------+
	|Programma:   cLife                                                 |
	|Versione:    0.1                                                   |
	|Autore:      Andrea Ciceri                                         |
	|Descrizione: Una semplice versione del gioco della vita di Conway  |
	|             scritta in C con interfaccia semigrafica da terminale.|
	|Lcenza:      GNU GPL3                                              |
	+-------------------------------------------------------------------+
*/

#include "engine.h" //Inludo il motore di gioco e il motore grafico

int mondo[mondoy][mondox]; //Creo il mondo di gioco
int generazioni = 0; //Inizialmente il mondo di gioco si trova alla generazione 0

int main() {
	InizializzaSchermo(); //Predispone il terminale alla visualizzazione del gioco e termina il programma se il terminale non ha i giusti requisiti
	PulisciMondo(*mondo); //Rende morta ogni singola cella del mondo di gioco
	RandomizzaMondo(*mondo); //Rendi vive casualmente alcune celle del mondo di gioco
	while(1) { //Ciclo di runtime che si ripete per ogni generazione
		LeggiTasti(); //Leggi i tasti che vengono premuti
		Stampa(*mondo, generazioni); //Stampa a schermo il mondo di gioco attuale e il numero della generazione attuale
		Evolvi(*mondo); //Evolvi il mondo di gioco modificandolo secondo le regole di Conway
		generazioni++; //Aumento il numero della generazione
		Framerate(fps); //Attendi del tempo in modo da bloccare il framerate
	}
	return 0; //Non dovrebbe essere mai eseguito
}
