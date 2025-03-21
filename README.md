# Quoridor 
firmware for a multi-player board game with graphical interface on an ARM based SoC

## Application note
__Autore:__ Giuseppe Arbore, S329535 
__Introduzione:__ L’obbiettivo di questa application note è fornire istruzioni su come giocare a  

### Quoridor 7x7 usando la scheda LandTiger con SoC LPC1768.  
__Descrizione del gioco:__ Quoridor7x7 è una rivisitazione di Quoridor, gioco da tavolo pubblicato nel 1997 dalla casa 
editrice Gigamic. In contrapposizione alla formulazione originaria, la disposizione della scacchiera non segue uno 
schema 9x9, bensì 7x7, e ciascun partecipante è dotato di otto barriere anziché dieci. Ciascun giocatore è dotato 
di un token e inizia la partita al centro di una delle linee perimetrali. L'obiettivo di ciascun partecipante consiste 
nel raggiungere la linea perimetrale opposta. La vittoria è assegnata al primo giocatore che riesce a raggiungere la 
linea perimetrale avversaria.  

__Descrizione implementative:__ Il progetto è suddiviso in più gruppi: 
• Main: contiene l'inizializzazione del sistema e delle varie componenti della LandTiger. 
• Timer: include le funzioni per gestire i 2 timer usati: 
o Timer0: tempo a disposizione dell'utente per eseguire la propria mossa (20 secondi) 
o Timer1: tempo massimo a disposizione dell'utente per effettuare l'handshaking (5 secondi). 
• GLCD: contiene funzioni per la scrittura sullo schermo della LandTiger. 
• Scacchiera: include funzioni per disegnare la scacchiera e per le conversioni da coordinate per la 
scacchiera a pixel dello schermo. 
• Button_EXINT: contiene la funzione di inizializzazione dei bottoni e i relativi listener. 
• Joystick: contiene la funzione per inizializzare il joystick. 
• Gioco: contiene funzioni generali per la gestione del gioco su una singola scheda e su due schede. 
• RIT: contiene l'handler del Repetitive Interrupt Timer (RIT) impostato a 50ms. La funzione RIT_IRQHandler 
verifica se l'utente ha compiuto azioni e, in caso affermativo, richiama le funzioni pertinenti 
• Pedina: contiene le funzioni per la gestione degli spostamenti del token 
• Muro: contiene funzioni per la gestione dei muri, inclusi inserimento e rimozione. Inoltre, presenta una 
funzione "controllaPercorso" che, mediante la chiamata della funzione "verificaValido" sia per l'utente in 
movimento che per l'utente fermo, verifica la possibilità di inserire il muro. 
• CAN: contiene le funzioni per inizializzare il can e per il collegamento tra due schede. 
• AI: contiene la funzione per la gestione del NPC e le relative funzioni di supporto.  

__Descrizione NPC:__ Per quanto riguarda la gestione dell’entità NPC, si procede al calcolo approssimativo della 
distanza dal traguardo per il token gestito dall'avversario e per quello controllato dall'NPC. Nel caso in cui l'NPC 
abbia ancora un muro e il token dell'avversario si trovi in una posizione di vantaggio rispetto a quello dell'NPC, 
viene inserito un muro nella posizione ritenuta ottimale. In alternativa, si procede con lo spostamento del token 
controllato dall'NPC verso la posizione considerata ottimale.  Per il calcolo approssimativo della distanza, si 
esegue una visita in profondità quattro volte, una per ciascuna delle possibili mosse che possono essere 
intraprese a partire dalla posizione iniziale del token. In seguito, si identifica il valore minimo ottenuto e la 
corrispondente prima mossa che conduce a tale valore minimo, indicando così la posizione ottimale. 
La definizione di posizione di vantaggio varia in relazione alla collocazione del pedone avversario sulla scacchiera. 
Nel caso in cui il token avversario si trovi più vicino alla riga di partenza rispetto alla riga di vittoria, viene 
considerato in vantaggio qualora il calcolo approssimativo della distanza dal traguardo risulti in un margine di 
almeno due posizioni rispetto all'NPC. Qualora il pedone avversario sia più vicino alla fila di vittoria anziché a 
quella di partenza, è sufficiente che detenga un margine di una posizione per essere considerato in posizione di 
vantaggio.  

__Descrizione Handshaking:__ Per la gestione della procedura di handshaking, viene impiegato il campo "stato" della 
variabile globale denominata "myscheda". Tale campo assume diversi valori in base allo stato in cui si trova la 
procedura di handshaking. In questa maniera, quando il valore del campo dati del messaggio CAN è pari a '0xFF', 
si procede alla lettura del campo "stato" di "myscheda" al fine di determinare il Player1. Qualora un giocatore 
dovesse rimanere in attesa per un periodo superiore a 5 secondi, l'operazione di handshaking viene interrotta e 
viene visualizzato un messaggio di errore.  

__Nota per giocare:__ premere INT0, selezionare attraverso l’uso del joystick una modalità (modalità selezionata sarà 
evidenziata in giallo), selezionare il player con il joystick, premere INT0 per iniziare la partita/handshaking. 
Istruzioni complete del gioco(versione 9x9): https://www.ultraboardgames.com/quoridor/game-rules 
Documentazione LandTiger: landtiger_v2.0_-_manual__v1.1.pdf (mbed.com) 
