# NetworkingLibrary - Client

Il sottoprogetto **Client** è responsabile della gestione della comunicazione lato client, consentendo la connessione a un server, la gestione delle risorse di rete e l'invio/ricezione dei pacchetti.

- [Componenti Principali](#componenti-principali)

<br></br>

## Componenti Principali

### 1. **main**
- **Descrizione:**  
  Rappresenta il punto di ingresso dell'applicazione client. 

- **Funzionalità principali:**
  - **Inizializzazione di Winsock:**  
    Prima di qualsiasi operazione, il client chiama `Net::CNetDevice::Create()` per inizializzare Winsock.
  - **Avvio del client:**  
    Il client viene inizializzato tramite il metodo `client.Initialize("localhost", 8080)`, che tenta di connettersi a un server in ascolto su un indirizzo ed una porta specificati (in questo caso localhost e 8080).
  - **Ciclo di elaborazione:**  
    Viene poi definito un ciclo while dove il client richiama il metodo `Process()`, implementato nella libreria di rete, che gestisce il ciclo di vita della connessione.
  - **Deallocazione delle risorse :**  
    Alla fine, le risorse utilizzate da Winsock vengono liberate chiamando `Net::CNetDevice::Destroy()`.



### 2. **Client**
- **Descrizione:**  
  Estende `Net::CAbstractClient` e gestisce le operazioni principali di rete per un client. Implementa i comportamenti per la gestione della connessione, l'elaborazione dei pacchetti e la comunicazione con il server.

- **Metodi principali:**
  - `OnSocketCreated()`: Richiamato a seguito della creazione del socket.
  - `OnConnect()`: Indica il successo della connessione al server.
  - `OnConnectFail()`: Notifica il fallimento della connessione.
  - `OnDisconnect()`: Indica la disconnessione del server.
  - `AnalyzePacket(TPacketHeader header)`: Analizza i pacchetti ricevuti.



### 3. **ClientPacketManager**
- **Descrizione:**  
  Questa classe si occupa di definire e caricare i tipi di pacchetti che il client può ricevere e processare durante la comunicazione con il server. In particolare, associa ogni header di pacchetto a una struttura dati specifica, permettendo di riconoscere e trattare i pacchetti ricevuti.

- **Metodi principali:**
  - `__LoadPacketHeaders()`: Carica e registra gli header dei pacchetti che il client può ricevere.




### 4. **ClientComponentsFactory**
- **Descrizione:**  
  La classe è un'implementazione del factory method, che crea i componenti necessari per il funzionamento dell'applicazione client.

- **Metodi principali:**
  - `CreatePacketManager()`: Restituisce un'istanza del `ClientPacketManager`, contenente le informazioni sui pacchetti di cui è prevista l'analisi una volta ricevuti.
