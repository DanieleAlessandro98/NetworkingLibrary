# NetworkingLibrary - Network

Questa libreria offre una serie di classi e interfacce per gestire le connessioni di rete, manipolare i pacchetti e gestire le comunicazioni tra client e server. È progettata per rendere le componenti client e le componenti server estendibili, permettendo agli utenti di personalizzarle e adattarle in base alle esigenze specifiche delle loro applicazioni.

- [Componenti Principali](#componenti-principali)
- [SocketWatcher](#socketwatcher)

<br></br>


## Componenti Principali

### 1. **AbstractEntity**
- **Descrizione:** 
  Classe base astratta per le entità di rete, come client e server. Fornisce un'interfaccia per inizializzare, elaborare e chiudere connessioni.
- **Metodi principali:**
  - `Initialize()`: Configura la connessione di rete utilizzando un indirizzo e una porta specifici.
  - `Process()`: Gestisce il ciclo di vita della connessione, includendo la logica di rete, come la lettura/scrittura delle informazioni.
  - `Shutdown()`: Termina la connessione e libera le risorse associate.



### 2. **AbstractClient**
- **Descrizione:** 
  Implementa un client di rete astratto. Gestisce le operazioni principali di rete, come la connessione al server e la gestione del flusso dei dati.
- **Funzionalità principali:**
  - `Initialize()`: Stabilisce la connessione con il server.
  - `Process()`: Gestisce il ciclo di vita della connessione, inclusa la verifica dello stato del socket per la lettura e la scrittura.
  - `Shutdown()`: Termina la connessione.



### 3. **AbstractServer**
- **Descrizione:**
  Implementa un server di rete astratto. Gestisce le operazioni principali di rete, come la creazione del socket del server, l'accettazione delle connessioni e l'elaborazione degli eventi di rete.
- **Metodi principali:**
  - `bool Initialize()`: Inizializza il server e avvia l'ascolto su un indirizzo e porta specificati.
  - `void Process()`: Gestisce il ciclo di vita delle connessioni, inclusa la gestione gli eventi di rete e le connessioni dei peer.
  - `void Shutdown()`: Chiude il server e disconnette tutti i peer.
  - `void HandleNewConnection()`: Gestisce una nuova connessione in arrivo.



### 4. **AbstractPeer**
- **Descrizione:**
  Classe base astratta per la gestione dei peer.
- **Metodi principali:**
  - `void Setup()`: Configura il peer.
  - `void Packet(const void* c_pvData, int iSize)`: Invia un pacchetto di dati.
  - `bool AnalyzePacket(TPacketHeader header)`: Analizza un pacchetto ricevuto.




### 5. **Socket**
- **Descrizione:** 
  Rappresenta un socket TCP e offre metodi per la creazione, chiusura, binding e connessione. Fornisce anche accesso al flusso di dati per inviare e ricevere informazioni.
- **Metodi principali:**
  - `Create()`: Crea un socket TCP.
  - `Connect()`: Collega il socket a un indirizzo server.
  - `Close()`: Chiude il socket.
  - `GetDataStream()`: Restituisce il flusso di dati associato al socket.



### 6. **DataStream**
- **Descrizione:**
  Gestisce il flusso di dati tra le entità di rete, fornendo metodi per inviare e ricevere dati tramite buffer.
- **Metodi principali:**
  - `Send()`: Copia i dati da inviare nel buffer di invio, assicurandosi che ci sia spazio sufficiente.
  - `ProcessSend()`: Trasferisce i dati dal buffer di invio al socket, gestendo solo i dati disponibili nel buffer e aggiornando lo stato del buffer man mano che i dati vengono inviati.
  - `ProcessRecv()`: Legge i dati dal socket e li memorizza nel buffer di ricezione.
  - `Peek()`: Permette di esaminare i dati nel buffer di ricezione senza rimuoverli.
  - `Recv()`: Rimuove i dati dal buffer di ricezione e li copia in un'altra area di memoria per ulteriori elaborazioni.




### 7. **PacketDefinition**
- **Descrizione:** 
  Contiene le definizioni dei tipi di pacchetti inviati e ricevuti tra client e server, rappresentati tramite strutture e utilizzati per la comunicazione di basso livello.
- **Esempi di pacchetti:**
  - `TPacketCGHandshake`: Pacchetto di handshake inviato dal client e ricevuto dal server.
  - `TPacketGCHandshake`: Pacchetto di handshake inviato dal server e ricevuto dal client.




### 8. **AbstractPacketHandler**
- **Descrizione:**
  Definisce i metodi per l'analisi e la gestione dei pacchetti, suddivisi per tipo di entità.
- **Sottoclassi:**
  - `CAbstractPacketBaseHandler`: Fornisce l'interfaccia base per la gestione dei pacchetti.
  - `CAbstractPacketServerHandler`: Specifica l'interfaccia per l'analisi dei pacchetti lato server.
  - `CAbstractPacketClientHandler`: Specifica l'interfaccia per l'analisi dei pacchetti lato client.


### 9. **AbstractPacketManager**
- **Descrizione:**
  Classe base astratta per la gestione dei pacchetti di rete, definendo l'interfaccia per l'inizializzazione, la verifica e l'elaborazione dei pacchetti.
- **Metodi principali:**
  - `void Initialize()`: Inizializza il gestore dei pacchetti, caricando le informazioni sui pacchetti.
  - `void Set()`: Imposta il tipo di pacchetto associato a un header.
  - `bool Get()`: Recupera il tipo di pacchetto associato a un determinato header.
  - `bool CheckPacket(CSocket* socket, TPacketHeader* packetHeader)`: Verifica se il pacchetto ricevuto è valido e recupera l'header del pacchetto.
  - `bool ProcessRecv(CAbstractPacketBaseHandler* packetHandler)`: Gestisce i pacchetti ricevuti utilizzando il gestore dei pacchetti fornito.
  - `bool OnProcessRecv(CAbstractPacketBaseHandler* packetHandler)`: Gestisce il processo di ricezione dei pacchetti e inoltra l'analisi al gestore dei pacchetti fornito.





## SocketWatcher

`SocketWatcher` è una classe progettata per gestire e monitorare i socket di una entità di rete, tracciando gli eventi di lettura e scrittura. Utilizza `select` per determinare quali socket sono pronti per la lettura, la scrittura o la chiusura.

### **Metodi principali:**

- **`SocketWatcher(int max_files)`**: Costruttore che inizializza il watcher con un numero massimo di socket che può monitorare.
- **`~SocketWatcher()`**: Distruttore che pulisce le risorse allocate.
- **`void add()`**: Aggiunge un socket alla lista di quelli monitorati.
- **`void remove()`**: Rimuove un socket dalla lista di quelli monitorati.
- **`int monitor()`**: Monitora i socket per eventi di lettura e scrittura, restituendo il numero di socket che hanno eventi.
- **`int get_ready_flags()`**: Restituisce i flag degli eventi pronti per un dato socket.
- **`CAbstractPeer* get_client_data()`**: Ottiene i dati del client associati a un evento specifico.
- **`int get_fd_from_index()`**: Restituisce il socket dall'indice dell'evento.
- **`void clear_event()`**: Cancella un evento specifico per un socket.
- **`int get_event_status()`**: Verifica se un determinato evento si è verificato per un socket specifico.

### **Dettagli di implementazione:**
- **Struttura dei dati:** Utilizza vettori per tenere traccia dei socket monitorati, dei dati associati e degli eventi.
- **Gestione eventi:** Utilizza `fd_set` per gestire i set di socket per operazioni di lettura e scrittura e `select` per determinare quali socket sono pronti per tali operazioni.
- **Operazioni dinamiche:** I metodi `add` e `remove` gestiscono dinamicamente l'aggiunta e la rimozione di socket dalla lista monitorata. Inoltre, `clear_event` permette di rimuovere eventi specifici senza influenzare gli altri socket.

