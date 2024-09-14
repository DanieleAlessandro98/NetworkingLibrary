#pragma once

#include <vector>
#include <memory>
#include "Socket.h"
#include "AbstractPeer.h"

namespace Net
{
    class CAbstractPeer;

    enum ESockwatch
    {
        SOCKW_NONE = 0,
        SOCKW_READ = 1,
        SOCKW_WRITE = 2,
        SOCKW_WRITE_ONESHOT = 4,
        SOCKW_EOF = 8,
    };

    class SocketWatcher
    {
        public:
            explicit SocketWatcher(int max_files);
            ~SocketWatcher();

            // Aggiunge un socket da monitorare, specificando i tipi di eventi (lettura/scrittura) e se deve essere monitorato una sola volta.
            void add_socket(int socket, CAbstractPeer* client_data, int events, int oneshot);

            // Rimuove un socket dalla lista dei socket monitorati.
            void remove_socket(int socket);

            // Monitora i socket per verificare quali sono pronti per la lettura o la scrittura, entro il timeout specificato.
            int monitor(struct timeval* timeout);

            // Controlla se un socket è pronto per la lettura o la scrittura.
            int get_ready_flags(int socket) const;

            // Ottiene i dati del client associati a un socket pronto.
            CAbstractPeer* get_client_data(unsigned int event_index) const;

            // Ottiene il socket dall'indice dell'evento.
            int get_socket_from_index(unsigned int event_index) const;

            // Cancella un evento specifico per un socket.
            void clear_event(int socket, unsigned int event_idx);

            // Verifica se un determinato evento si è verificato per un socket specifico.
            int get_event_status(int socket, unsigned int event_idx) const;

        private:
            // Cerca l'indice del socket. Se il socket è trovato, restituisce il suo indice; altrimenti, restituisce -1.
            int find_socket_index(int socket) const;

            // Utilizzato con "select" per determinare quali socket sono pronti per la lettura
            fd_set              read_sockets_;

            // Utilizzato con "select" per determinare quali socket sono pronti per la scrittura
            fd_set              write_sockets_;

            // Contiene i socket effettivamente monitorati per la lettura e/o scrittura. Ogni elemento del vettore rappresenta un socket da monitorare.
            std::vector<int>    monitored_sockets_;

            // Dopo una chiamata a select, questo vettore è riempito con gli indici dei socket che hanno eventi pronti (lettura o scrittura).
            std::vector<int>    ready_socket_indices_;

            // Copia di read_sockets che viene passato alla chiamata select. È utilizzato per evitare la modifica del set originale durante l'uso di select.
            fd_set              active_read_sockets_;

            // Copia di write_sockets che viene passato alla chiamata select. È utilizzato per evitare la modifica del set originale durante l'uso di select.
            fd_set              active_write_sockets_;

            // Indica la dimensione massima dei vettori "monitored_sockets", "ready_sockets_indices" e "socket_data"
            int                 max_sockets_;

            // Indica quanti socket sono presenti nel vettore "monitored_sockets" e sono attualmente monitorati.
            int                 num_monitored_sockets_;

            // Vettore di puntatori che memorizza dati aggiuntivi associati a ciascun socket monitorato.
            std::vector<CAbstractPeer*> socket_data_;

            // Vettore di flag che indicano gli eventi monitorati per ciascun socket (lettura, scrittura, o entrambi).
            std::vector<int>    socket_event_flags_;
    };
};
