Specifica tecnica UI Applicazione di Configurazione
===================================================

Autore: Ivan Tarozzi
Revisione: 0.1 - 02/11/2021


> bozza della specifica tecnica per la UI dell'app di configurazione
> in quest prima release, più che la UI in se, si cercherà di dettagliare le opzioni da gestire. Chi poi si incaricherà di sviluppare la UI deciderà come meglio implementare il sw.


## Descrizione UI Generale
La UI sarà costituita da varie sezioni (tab o sottofinestre):

- impostazioni generali
- impostazioni footswitch e MIDI
- gestione profili


## Impostazioni generali

Qui verranno impostati i parametri generali del dispisitivo:
- canale MIDI di trasmissione (0-127)
- Numero footswitch (1-8) ? 
- BLE Mode:  Client | Server
  - BLE client: connect to xxxxxx (selezione del device?)   *1
- OTA firmware upgrade


> *1 qui in realtà non abbiamo ancora deciso se la scansione venga fatta dall'app e in che modo


## Impostazioni Footswitch e MIDI

Qui viene impostato, per ogni footswitch, la modalità di gestione e i relativi comandi MIDI da inviare

|Footswitch NR|Mode|Event|
|-------------|----|-----|
|  FS 1       |TAP | **  |
|             |HOLD| **  |

Quindi per ogni switch verranno configurati gli eventi per il modo TAP e per il modo HOLD.

> in futuro vedremo se serve gestire anche UP e DOWN, ma forse è troppo

Ovviamente se una delle due modalità non deve essere usata sarà sufficiente specificare un Event nullo.

Il campo Event può essere selezionato tra uno di queste opzioni, e sulla base dell'evento si potranno definire ulteriori parametri:

- Single 
- Repeat 
- Increment 
- On_Off

Di seguito la descrizione di ogni evento:

### Single
Ad ogni tap/hold configurato in questo modo verrà inviato un comando MIDI.
I parametri aggiuntivi di questo evento saranno quindi:

- MIDI Type: CC|PC
- MIDI Number: ad esempio il numero del CC da impostare
- MIDI Value:  il valore del CC o del PC


### Repeat
Questo può essere selezionato solo per il modo hold. In questo modo verrà ripetuto l'invio del comando midi a intervalli definiti.
> utile ad esempio se ho un CC che passa, sul device controllato, ad un preset successivo. In questo modo, tenendo premuto posso andare avanti senza premere n volte sullo switch

I parametri aggiuntivi di questo evento saranno quindi:

- Repeat interval: msec con incrementi di 10 ? fino max 1000?
- MIDI Type: CC|PC
- MIDI Number: ad esempio il numero del CC da impostare
- MIDI Value:  il valore del CC o del PC


### Incement
Ad ogni tap/hold configurato in questo modo verrà inviato un comando MIDI il cui valore associato verrà incrementato/decrementato ad ogni invio.  Il fw dovrà quindi tenere traccia dell'ultimo valore inviato

I parametri aggiuntivi di questo evento saranno quindi:

- Internal value: potremmo definire una serie di variabili interne (max 4 ?) da abbinare ad ogni switch
- Min Value : valore minimo della variabile interna, e quindi del valore MIDI inviato (0-127)
- Max Value : valore massimo della variabile interna, e quindi del valore MIDI inviato(0-127)
- Step: step di incremento ad ogni invio (+/-1..10 per gestire incremento/decremento) 
- Cycle: True|False : se impostato a true, raggiunto il val massimo ritorna al minimo e viceversa
- Repeat interval: msec  - solo per modalità HOLD
- MIDI Type: CC|PC
- MIDI Number: ad esempio il numero del CC da impostare
- __MIDI Value:  -- non serve - in questo caso prendo direttamente il valore della var interna --__




### On_Off
Ad ogni tap/hold configurato in questo modo verrà inviato un comando MIDI. A differenza però dell'evento "Single", questo lavora in abbinamento al campo "Group". 

Ipotizzando di gestire da 1 a 4 gruppi, se abbinato ad un gruppo lo switch si comporta come ON/OFF esclusivo, rispetto agli altri switch dello stesso gruppo.


Quindi per esempio:
se i primi 3 switch sono abbinati allo stesso gruppo1, se lo switch 1 è attivo e premo il 2 verrà inviato il comando MIDI con ON value per per lo switch2 e il comando MIDI con OFF value allo switch1 (o a tutti gli altri del gruppo, per semplicità).
Inoltre si potrebbe attivare il LED di uscita solo per il pulsante in ON.

> ovviamente occorre tenere memoria dello switch attivo, in modo che al reset del firmware  non si perda sincronia, oppure inizializzare al boot uno stato noto da cui partire.


I parametri aggiuntivi di questo evento saranno quindi:
- Group: 1..4
- MIDI Type: CC|PC
- MIDI Number: ad esempio il numero del CC da impostare
- MIDI ON Value: il valore del CC o del PC quando ON
- MIDI OFF Value: il valore del CC o del PC quando OFF


