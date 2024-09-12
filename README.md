# NetworkingLibrary




## Panoramica del Progetto
NetworkingLibrary è una libreria in C++ progettata per gestire comunicazioni in un ambiente client-server. La libreria è suddivisa in tre componenti principali: **Network**, **Client**, e **Server**, che realizzano un'infrastruttura di rete avente comunicazione bidirezionale.

### Descrizione del Progetto

1. **Libreria**: Questa parte gestisce il livello più basso dell'infrastruttura di rete e definisce classi astratte per client e server. Facilita la configurazione e personalizzazione dei vari componenti di rete.
2. **Client**: Implementa la logica concreta per la comunicazione con il server, le operazioni client-side e gestisce il flusso di dati.
3. **Server**: Gestisce le operazioni server-side e il flusso di dati tra i peer, permettendo una comunicazione che supporta più client connessi.





## Caratteristiche Principali

- Comunicazione TCP
- Socket non bloccanti
- Programmazione orientata agli oggetti
- Personalizzazione flessibile
- Sistema di pacchetti estendibile
- Monitoraggio dei socket tramite `Watcher`
- Interfaccia intuitiva per operazioni di rete
- Supporto per pacchetti con dimensioni sia statiche che dinamiche





## Flusso del Sistema

1. **Inizializzazione**:
   - Avvia la componente di rete (es. `WSAStartup` su Windows) e crea un socket. Il server ascolta su una porta specifica, mentre il client tenta di stabilire una connessione.
   - Il server gestisce connessioni multiple in modo asincrono e non bloccante, senza attendere il completamento delle operazioni I/O di ciascun client.

2. **Gestione delle Connessioni**:
   - Quando un client si connette, il server crea un `peer` che rappresenta quella connessione, con un socket e un flusso dati associato.
   - La comunicazione avviene tramite pacchetti, definiti da strutture dati contenenti le informazioni da scambiare.

3. **Monitoraggio degli Eventi sui Socket**:
   - Un `watcher`, basato sulla funzione `select()`, monitora gli eventi sui socket.

4. **Gestione del Flusso di Dati**:
   - Il flusso di dati è gestito tramite buffer dedicati: uno per l'invio e uno per la ricezione. I dati sono scritti nel buffer di invio e inviati attraverso il socket, mentre i dati ricevuti sono accumulati nel buffer di ricezione fino alla loro analisi.

5. **Fasi della Comunicazione**:
   - La comunicazione tra client e server si svolge attraverso diverse fasi, partendo dall'handshake iniziale.

6. **Chiusura e Pulizia**:
   - La chiusura delle connessioni e la pulizia delle risorse di rete sono gestite per evitare perdite di risorse o chiusure improvvise.






## Struttura del Progetto

Il progetto è organizzato come segue:

```
NetworkingLibrary/
├── Client/                 # Contiene l'implementazione del client
├── Network/                # Contiene la libreria di rete
├── Server/                 # Contiene l'implementazione del server
└── NetworkingLibrary.sln   # File di soluzione per la compilazione del progetto
```





## Pattern Utilizzati

- Singleton
- Factory Method
- Abstract Factory






## Utilizzo

### Personalizzazione dei Comportamenti del Sistema

Il sistema consente di personalizzare i comportamenti mediante la sovrascrittura dei metodi nelle implementazioni concrete di client e server. Ecco alcuni esempi:

1. **Personalizzazione del Client**:

   Implementa la classe `CAbstractClient` per definire comportamenti specifici del client:

   ```cpp
   class CustomClient : public CAbstractClient
   {
   public:
       void OnSocketCreated() override
       {
           // Codice per gestire la creazione del socket
       }

       void OnConnect() override
       {
           // Codice per gestire la connessione riuscita
       }

       void OnConnectFail() override
       {
           // Codice per gestire il fallimento della connessione
       }

       void OnDisconnect() override
       {
           // Codice per gestire la disconnessione
       }
   };
   ```

2. **Personalizzazione del Server**:

   Implementa la classe `CAbstractServer` per gestire eventi del server:

   ```cpp
   class CustomServer : public CAbstractServer
   {
   public:
       void OnInitializeCompleted() override
       {
           // Codice per gestire il completamento dell'inizializzazione
       }

       void OnSocketListening() override
       {
           // Codice per gestire la fase in cui il socket inizia ad ascoltare
       }

       void OnConnectClient(std::shared_ptr<CSocket> newClientSocket) override
       {
           // Codice per gestire una nuova connessione client
       }

       void OnDisconnectClient(CAbstractPeer* peer) override
       {
           // Codice per gestire la disconnessione di un client
       }

       void DisconnectAll() override
       {
           // Codice per disconnettere tutti i client
       }
   };
   ```

3. **Gestione dei Pacchetti**:

   Personalizza la gestione dei pacchetti implementando `CAbstractPacketManager` e i gestori di pacchetti:

   ```cpp
   class CustomPacketManager : public CAbstractPacketManager
   {
   public:
       void __LoadPacketHeaders() override
       {
           // Codice per caricare gli header dei pacchetti
       }
   };

   class CustomPacketServerHandler : public CAbstractPacketServerHandler
   {
   public:
       bool Analyze(CAbstractPeer* peer, TPacketHeader header) override
       {
           // Codice per analizzare i pacchetti ricevuti dal server
           return true;
       }
   };

   class CustomPacketClientHandler : public CAbstractPacketClientHandler
   {
   public:
       bool Analyze(TPacketHeader header) override
       {
           // Codice per analizzare i pacchetti ricevuti dal client
           return true;
       }
   };
   ```

### Personalizzazione dei Componenti del Sistema

Il sistema supporta la creazione di componenti personalizzati, come gestori di pacchetti e peer, tramite il pattern Factory. Ecco come definire e utilizzare una factory personalizzata:

1. **Definire una Factory per il Client**:

   ```cpp
   class CustomClientComponentsFactory : public Net::AbstractClientComponentsFactory
   {
   public:
       std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
       {
           return std::make_shared<CustomPacketManager>();
       }
   };
   ```

2. **Definire una Factory per il Server**:

   ```cpp
   class CustomServerComponentsFactory : public Net::AbstractServerComponentsFactory
   {
   public:
       std::shared_ptr<Net::CAbstractPacketManager> CreatePacketManager() override
       {
           return std::make_shared<CustomPacketManager>();
       }

       std::unique_ptr<Net::CAbstractPeerManager> CreatePeerManager() override
       {
           return std::make_unique<CustomPeerManager>();
       }

       std::shared_ptr<Net::CAbstractPeer> CreatePeer(std::shared_ptr<Net::SocketWatcher> serverWatcher) override
       {
           return std::make_shared<CustomPeer>(serverWatcher);
       }
   };
   ```

3. **Utilizzare la Factory**:

   ```cpp
   CAbstractServer server;
   CustomServerComponentsFactory factory;

   server.SetComponentsFactory(std::make_shared<CustomServerComponentsFactory>());
   ```

### Aggiungere una Nuova Fase di Comunicazione

1. **Definire la Nuova Fase in `Definition.h`**:

   ```cpp
   enum EPhase
   {
       PHASE_CLOSE,
       PHASE_HANDSHAKE,
       PHASE_LOGIN, // Nuova fase aggiunta
   };
   ```

2. **Impostare la Fase al Momento Opportuno**:

   ```cpp
   bool ServerHandshake::RecvHandshake(CAbstractPeer* abstractPeer)
   {
       // ...
       if (peer->HandshakeProcess(handshakePacket.time, handshakePacket.delta))
       {
           // Handshake completato, impostiamo la fase di login
           peer->SetPhase(PHASE_LOGIN);
       }
       // ...
   }
   ```

3. **Creare una Nuova Classe per Gestire la Fase**:

   ```cpp
   class ServerLogin : public Net::CAbstractPacketServerHandler
   {
   public:
       bool Analyze(Net::CAbstractPeer* peer, Net::TPacketHeader header) override;
   };

   bool ServerLogin::Analyze(CAbstractPeer* peer, TPacketHeader header)
   {
       bool ret = true;
       switch (static_cast<PacketCGHeader>(header))
       {
           // Gestione degli header dei pacchetti per questa fase.
       }
       return ret;
   }
   ```

4. **Registrare l'Handler della Nuova Fase nella Classe `CPeer`**:

   ```cpp
   class CPeer : public Net::CAbstractPeer
   {
       std::unique_ptr<ServerLogin> m_packetHandlerServerLogin; // Nuovo handler
   };

   CPeer::CPeer(std::shared_ptr<Net::SocketWatcher> serverWatcher) :
       Net::CAbstractPeer(serverWatcher)
   {
       m_packetHandlerServerLogin = std::make_unique<ServerLogin>();
       RegisterPhaseHandler(PHASE_LOGIN, m_packetHandlerServerLogin.get());
   }
   ```

### Aggiungere un Nuovo Pacchetto Personalizzato

1. **Definire l'Header del Nuovo Pacchetto in `PacketDefinition.h`**:

   ```cpp
   enum class PacketCGHeader : TPacketHeader
   {
       HEADER_CG_LOGIN = 2, // Nuovo pacchetto per login
   };
   ```

2. **Definire la Struttura del Nuovo Pacchetto in `PacketDefinition.h`**:

   ```cpp
   struct TPacketCGLogin
   {
       PacketCGHeader header = PacketCGHeader::HEADER_CG_LOGIN;
       char username[21]; // 20 caratteri + terminatore null
       char password[21]; // 20 caratteri + terminatore null
   };
   ```

3. **Registrare il Pacchetto nella Classe `PacketManager`**:

   ```cpp
   void CServerPacketManager::__LoadPacketHeaders()
   {
       Set(PacketCGHeader::HEADER_CG_LOGIN, CAbstractPacketManager::TPacketType(sizeof(TPacketCGLogin), STATIC_SIZE_PACKET));
   }
   ```

4. **Analizzare e Gestire il Pacchetto nella Fase Appropriata**:

   ```cpp
   bool ServerLogin::Analyze(CAbstractPeer* peer, TPacketHeader header)
   {
       bool ret = true;

       switch (static_cast<PacketCGHeader>(header))
       {
           case PacketCGHeader::HEADER_CG_LOGIN:
               ret = RecvLogin(peer);
               break;
       }

       return ret;
   }

   bool ServerLogin::RecvLogin(CAbstractPeer* abstractPeer)
   {
       auto* peer = CPeerManager::ValidPeer<ServerPeerType>(abstractPeer);
       if (!peer)
           return false;

       TPacketCGLogin login;
       if (!CPacketIO::ReadPacketData(peer->GetSocket(), login))
           return false;

       // Utilizzare i dati del pacchetto
       // login.username
       // login.password

       return true;
   }
   ```

## Documentazione

Per dettagli specifici sull'architettura e le funzionalità di ciascun componente, consulta i README dei seguenti sottoprogetti:

- [Libreria](./Network/README.md)
- [Client](./Client/README.md)
- [Server](./Server/README.md)

## Requisiti di Sistema

- **Sistema Operativo**: Windows
- **Ambiente di Sviluppo**: Visual Studio 2019 (compilatore MSVC)

## Installazione

1. Apri il file `NetworkingLibrary.sln` presente nella root del progetto.
2. Compila il progetto utilizzando Visual Studio 2019.

## Esecuzione

- **Configurazione x64**: Se compili con configurazione x64, l'output sarà nella cartella `x64` presente nella root del progetto. Il binario si troverà in una sottocartella basata sul tipo di configurazione (Release o Debug).
- **Testato**: Il progetto è stato testato su configurazione x64 sia in modalità Release che Debug.

## Utilizzi e Applicazioni

Esempi di applicazioni includono:

- **Giochi Multiplayer**: Scambio di pacchetti di aggiornamento del gioco tra più giocatori.
- **Sistemi di Chat**: Invio e ricezione di messaggi tra utenti.
- **Applicazioni Distribuite**: Scambio continuo di dati tra nodi in una rete.

## Note Aggiuntive

Per ulteriori dettagli e documentazione, consulta i README specifici dei sottoprogetti.
