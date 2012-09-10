#define mondox 118 //La larghezza del mondo di gioco
#define mondoy 37 //L'altezza del mondo di gioco
#define fps 5 //Il framerate in frames/secondo

//FUNZIONI AD "ALTO LIVELLO"
void InizializzaSchermo(); //Predispone il terminale alla visualizzazione del gioco e termina il programma se il terminale non ha i giusti requisiti
void PulisciMondo(int *mondo); //Rende morta ogni singola cella del mondo di gioco
void RandomizzaMondo(int *mondo); //Rendi vive casualmente alcune celle del mondo di gioco
void LeggiTasti(); //Leggi i tasti che vengono premuti
void Stampa(int *mondo, int generazioni); //Stampa a schermo il mondo di gioco attuale e il numero della generazione attuale
void Evolvi(int *mondo); //Evolvi il mondo di gioco modificandolo secondo le regole di Conway
void Framerate(int framerate); //Imposta il framerate in frames al secondo 

//FUNZIONI A "BASSO LIVELLO"
int ContaVicine(int *mondo, int y, int x); //Restituisce il numero di celle adiacenti a quella data(compresa la stessa cella)
int ControllaCella(int *mondo, int y ,int x); //Restituisce 0 se la cella data è morta e 1 se è viva
void Vivo(int *mondo, int y ,int x); //Rende viva una cella data
void Morto(int *mondo, int y ,int x); //Rende morta una cella data
void Esci(int n); //Esce dal programma visualizzando l'errore
