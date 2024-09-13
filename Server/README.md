# NetworkingLibrary - Server

Il sottoprogetto **Server** gestisce le comunicazioni lato server, permettendo di accettare connessioni multiple e scambiare pacchetti con i client connessi. Implementa la logica necessaria per la gestione delle connessioni, l'elaborazione dei pacchetti e il monitoraggio dello stato dei peer.

- [Componenti Principali](#componenti-principali)

<br></br>

## Componenti Principali

### 1. **main**
- **Descrizione:**  
  Questo file rappresenta il punto di ingresso dell'applicazione server.

- **Funzionalità principali:**
  - **Inizializzazione di Winsock:**  
    Il server chiama `Net::CNetDevice::Create()` per inizializzare Winsock.
  - **Avvio del server:**  
    L'inizializzazione del server avviene tramite `server.Initialize("localhost", 8080)`, che configura un listener per accettare connessioni sulla porta specificata.
  - **Ciclo di elaborazione:**  
    Viene eseguito un ciclo while che gestisce il ciclo di vita delle connessioni.
  - **Chiusura del server:**  
    Alla chiusura, le risorse di rete vengono liberate e viene chiamato `Net::CNetDevice::Destroy()`.




### 2. **ServerMain**
- **Descrizione:**  
  Estende `Net::CAbstractServer` e gestisce le operazioni principali del server. È responsabile della configurazione, accettazione delle connessioni, elaborazione dei pacchetti e comunicazione con i peer.

- **Metodi principali:**
  - `OnInitializeCompleted()`: Viene chiamato quando l'inizializzazione del server è stata completata.
  - `OnSocketListening()`: Notifica che il server è pronto ad accettare nuove connessioni.
  - `OnConnectClient()`: Gestisce l'accettazione di un nuovo client.
  - `OnDisconnectClient()`: Gestisce l'evento di disconnessione di un client connesso.




### 3. **Peer**
- **Descrizione:**  
  Rappresenta il singolo peer (client connesso).

- **Metodi principali:**
  - `OnSetupCompleted()`: Viene chiamato quando la configurazione del peer è stata completata.




### 4. **PeerManager**
- **Descrizione:**  
  Gestisce i peer connessi.

- **Metodi principali:**
  - `CanAcceptNewConnection()`: Verifica se il server può accettare nuove connessioni.
  - `OnPeerAccepted()`: Notifica l'evento di accettazione di un nuovo peer.




### 5. **ServerPacketManager**
- **Descrizione:**  
  Questa classe si occupa di definire e caricare i tipi di pacchetti che il server può ricevere e processare durante la comunicazione con i client. In particolare, associa ogni header di pacchetto a una struttura dati specifica, permettendo di riconoscere e trattare i pacchetti ricevuti.

- **Metodi principali:**
  - `__LoadPacketHeaders()`: Carica gli header dei pacchetti supportati dal server e ne definisce le informazioni.





### 6. **ServerComponentsFactory**
- **Descrizione:**  
  La classe è un'implementazione del factory method, che crea i componenti necessari per il funzionamento dell'applicazione server.

- **Metodi principali:**
  - `CreatePacketManager()`: Restituisce un'istanza del `ServerPacketManager`, contenente le informazioni sui pacchetti di cui è prevista l'analisi una volta ricevuti.
  - `CreatePeerManager()`: Restituisce un'istanza di `CPeerManager`, che gestisce l'insieme dei peer connessi.
  - `CreatePeer()`: Restituisce un'istanza di `CPeer`, rappresentante un singolo client connesso.



### 7. **ServerHandshake**
- **Descrizione:**  
  Specifica tutte le operazioni e i comportamenti relativi alla fase di handshake, definita come `PHASE_HANDSHAKE`.

- **Metodi principali:**
  - `Analyze()`: Analizza il pacchetto ricevuto e, se si tratta di un pacchetto riguardante la fase `PHASE_HANDSHAKE`, richiama la logica necessaria per verificarlo.
