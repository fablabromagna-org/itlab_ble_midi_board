Specifica tecnica generale - Firmware
========================================

Autore: Ivan Tarozzi
Revisione: 0.1 - 26/10/2021


> bozza della specifica tecnica del firmware, per definirne funzionalità generali e impostare lo sviluppo


## Firmware a 2 modalità BLE
Il firmware funziona in doppia modalità BLE, selezionabile tramite comando SW o HW. L'ultima modalità selezionata viene salvata in flash.

### Modalità BLE Server
Questa è la classica modalità con cui normalmente funzionano i controller MIDI BLE.

All'avvio il device inizializza il BLE in modalità Advertising, esponendo il service MIDI. In questo modo si rende rilevabile da parte di PC o dispositivi mobili, per l'abbinamento.

Una volta abbinato ad un dispositivo, alla pressione dei footswitch invierà i rispettivi comandi MIDI (write characteristic MIDI).

### Modalità BLE Client
Questa è una modalità generalmente non implementata dai controller MIDI; in questa modalità, il controller funziona come se fosse un PC o un dispositivo mobile, mettendosi alla ricerca di un dispositivo MIDI BLE server.

L'elenco dei dispositivi compatibili può essere definito da sw oppure (forse meglio) sarà possibile identificare come validi tutti i dispositivi che espongano il service BLE MIDI.

In base al dettaglio implementativo adottato, il controller eseguirà l'abbinamento:
* o del primo device BLE MIDI rilevato
* o di uno dei device BLE MIDI rilevati, selezionandolo tramite display integrato o app da remoto (es. wifi)
* o sulla base del MAC address assegnato attraverso una app mobile che precedentemente ha eseguito la scansione (meno indicato? presuppone più lavoro misto sw - app)

Una volta abbinato un dispositivo, il relativo MAC viene salvato in flash, in modo da eseguire automaticamente una riconnessione al successivo avvio

Una volta abbinato ad un dispositivo, alla pressione dei footswitch invierà i rispettivi comandi MIDI (write characteristic MIDI).

> questa modalità può essere ad esempio usata per dispositivi come NUX Mighty o PG Spark che tipicamente accettano i comandi da app su smartphone dedicata.


## Gestione footswitch e comandi MIDI
Al fine di garantire una ampia configurabilità del controller, in modo da adeguarlo a differenti modalità operative, la configurazione dei footswitch e dei relativi comandi MIDI è impostabile via sw.

La programmazione della modalità verrà eseguita o da app mobile o da app su PC oppure attraverso una interfaccia sfruttando il server HTTP di ESP32.

La configurazione può poi essere inviata al firmware che la salverà in flash.

Potrebbe essere possibile salvare più di una configurazione eventualmente poi selezionabile da pulsante o encoder sul controller (in modo da avere ad esempio da preset1 a preset4 in flash per passare da uno all'altro senza riprogrammare ogni volta).

La configurazione dei pulsanti e dei relativi comandi MIDI è indipendente dalla modalità MIDI BLE selezionata (client - server). Quest'ultima infatti serve solo a stabilire il collegamento tra i 2 dispositivo, ma una volta fatto il controller funzionerà sempre come MIDI-OUT.

> La modalità MIDI-IN potrebbe essere successivamente implementata per ricevere parametri di configurazione, ma non è utilizzata in normale funzionamento. 

Il controller può avere da un minimo di 2 ad un massimo di 5? footswitch. Il numero di footswitch viene assegnato nel codice (assieme al nr pin di riferimento), sulla base dell'HW disponibile. 

Il prototipo di base funzionerà con 4 footswitch.

A livello di applicazione di configurazione (da ora "configuratore") verrà selezionato il numero di footswitch gestiti, in modo da adeguare coerentemente l'interfaccia utente di configurazione.


## Modalità footswitch
Ogni footswitch può essere configurato in una delle seguenti modalità:
* TAP (evento press-release veloce)
* HOLD (evento press-release rilevato per almeno n msec - 1000? 1500? 2000?)
* UP-DOWN (in questo caso può essere configurato un evento MIDI quando il pulsante viene premuto e uno quando viene rilasciato)

**NOTA:**
Le modalità TAP e HOLD non sono alternative, ma è possibile configurare sia l'una che l'altra per ogni footswitch (quindi configurazione quando premo e rilascio e configurazione per quando tengo premuto)

Invece la modalità UP-DOWN dobbiamo decidere se renderla alternativa alle altre o come gestirla assieme (se serve).

In caso di selezione TAP, può inoltre essere abbinato un numero di gruppo (da 1 a 2? 3?).

Nel caso in cui venga abbinato ad un gruppo, una variabile interna per ogni gruppo viene definita e assegnata con un valore che identifica l'ultimo pulsante premuto.  In questo modo sarà possibile gestire una sorta di modalità mutualmente esclusiva, con un comando MIDI per ogni stato della variabile di gruppo. Inoltre (cosa principale) la variabile di gruppo può essere sfruttata per pilotare coerentemente un LED per ogni pulsante, che identifichi la modalità attiva.

> Esempio: abbino i primi 3 pulsanti allo stesso gruppo, ognuno con un comando MIDI che imposti un preset differente. La pressione di uno switch invia il comando MIDI relativo al preset, e contemporaneamente accende il led del pulsante, spegnendo tutti gli altri led del gruppo.

In caso di TAP e HOLD può essere abbinata una funzione di incremento/decremento con i seguenti parametri:
* valore minimo
* valore massimo
* step (+/-xx  col segno di identifica anche il verso di incremento/decremento)
* ciclico (yes-no) - se yes, raggiunto un estremo MIN/MAX passa all'altro
* repeat (tempo msec - solo per HOLD) - se mantenuto premuto, il valore viene incrementato/decrementato ogni x msec


### Comando MIDI
Sulla base della configurazione stabilita al paragrafo precedente, per ogni evento gestito in fase di configurazione può essere abbinato uno dei seguenti comandi MIDI:
* Control Change (CC)
* Program Change (PC)

> al momento non verranno gestiti i comandi NOTE ON e NOTE OFF.

Per ogni evento deve quindi essere stabilito:

* che tipo di comando MIDI inviare (CC/PC)
* il numero del comando CC/PC
* il valore del comando (può essere fisso o gestito con valore di incremento/decremento !)

A monte, nella configurazione generale del controller, deve essere definito il canale MIDI a cui mandare i comandi sopra descritti.

> per ora si può lavorare su CH0 del MIDI


## ROADMAP per l'implementazione del FW e del SW

Di seguito una possibile roadmap per implementare le funzionalità del Fw e contestualmente del SW per configurarlo.

Dobbiamo tenere il più possibile distinti i mondi e usare metodologie che ci consentamo di portare avanti lo sviluppo senza dipendere troppo uno dall'altro.

Ad esempio, una volta stabilito in che modo creare la UI di configurazione e come salvare la configurazione in flash, il FW deve essere indipendente nel partire, caricare la configurazione concordata e agire di conseguenza.

Quindi, alcuni dei macro-moduli in cui possiamo suddividere lo sviluppo sono:

* definizione del formato di configurazione (JSON? bianrio? MIDI? - impatto anche sulla lettura da FW)
* definizione della modalità di configurazione (APP solo mobile? APP anche su PC? wireless o necessario USB? oppure come minimale applicazione WEB dentro ESP32 con server HTTP?)
* SW APP Config: 
  * sviluppo applicazione
  * test di invio e feedback
  * eventuale configurazione del device a cui collegarsi, in modo BLE client
  * eventuali configurazioni aggiuntive o upgrade FW ?
*  Firmware:
  * gestione della ricezione configurazione da app esterna e salvataggio su flash/storage
  * caricamento configurazione al boot
  * avvio della modalità BLE selezionata (con gestione della riconnessione in caso di errore)
  * gestione della pressione footswitch e gestione della modalità configurata
  * gestione dell'invio del comando MIDI su BLE
  * eventuale gestione dell'invio anche su seriale, se vogliamo implementare anche MIDI-OUT su DIN 5pin
* Firmware (opzionale):
  * gestione della lettura del livello batteria e segnalazione LED
  * gestione del livello batteria tramite BLE (come BLE server)
  * gestione di eventuale upgrade firmware OTA

  


