#pragma once

#include <vector>
#include <memory>
#include "Socket.h"

namespace Net
{
    enum EFdwatch
    {
        FDW_NONE = 0,
        FDW_READ = 1,
        FDW_WRITE = 2,
        FDW_WRITE_ONESHOT = 4,
        FDW_EOF = 8,
    };

    class SocketWatcher
    {
        public:
            explicit SocketWatcher(int max_files);
            ~SocketWatcher();

            // Aggiunge un file descriptor da monitorare, specificando i tipi di eventi (lettura/scrittura) e se deve essere monitorato una sola volta.
            void add_fd(int fd, std::shared_ptr<CSocket> client_data, int events, int oneshot);

            // Rimuove un file descriptor dalla lista dei file descriptor monitorati.
            void remove_fd(int fd);

            // Monitora i file descriptor per verificare quali sono pronti per la lettura o la scrittura, entro il timeout specificato.
            int monitor(struct timeval* timeout);

            // Controlla se un file descriptor è pronto per la lettura o la scrittura.
            int get_ready_flags(int fd) const;

            // Ottiene i dati del client associati a un file descriptor pronto.
            std::shared_ptr<CSocket> get_client_data(unsigned int event_index) const;

            // Ottiene il file descriptor dall'indice dell'evento.
            int get_fd_from_index(unsigned int event_index) const;

            // Cancella un evento specifico per un file descriptor.
            void clear_event(int fd, unsigned int event_idx);

            // Verifica se un determinato evento si è verificato per un file descriptor specifico.
            int get_event_status(int fd, unsigned int event_idx) const;

            // Ottiene la dimensione del buffer associato a un file descriptor.
            int get_buffer_size(int fd) const;

        private:
            // Cerca l'indice del file descriptor. Se il file descriptor è trovato, restituisce il suo indice; altrimenti, restituisce -1.
            int find_fd_index(int fd) const;

            // Utilizzato con "select" per determinare quali file descriptor sono pronti per la lettura
            fd_set              read_fds_;

            // Utilizzato con "select" per determinare quali file descriptor sono pronti per la scrittura
            fd_set              write_fds_;

            // Contiene i file descriptor effettivamente monitorati per la lettura e/o scrittura. Ogni elemento del vettore rappresenta un file descriptor da monitorare.
            std::vector<int>    monitored_fds_;

            // Dopo una chiamata a select, questo vettore è riempito con gli indici dei file descriptor che hanno eventi pronti (lettura o scrittura).
            std::vector<int>    ready_fd_indices_;

            // Copia di read_fds che viene passato alla chiamata select. È utilizzato per evitare la modifica del set originale durante l'uso di select.
            fd_set              active_read_fds_;

            // Copia di write_fds che viene passato alla chiamata select. È utilizzato per evitare la modifica del set originale durante l'uso di select.
            fd_set              active_write_fds_;

            // Indica la dimensione massima dei vettori "monitored_fds", "ready_fd_indices" e "fd_data"
            int                 max_fds_;

            // Indica quanti file descriptor sono presenti nel vettore "monitored_fds" e sono attualmente monitorati.
            int                 num_monitored_fds_;

            // Vettore di puntatori che memorizza dati aggiuntivi associati a ciascun file descriptor monitorato.
            std::vector<std::shared_ptr<CSocket>> fd_data_;

            // Vettore di flag che indicano gli eventi monitorati per ciascun file descriptor (lettura, scrittura, o entrambi).
            std::vector<int>    fd_event_flags_;
    };
};
