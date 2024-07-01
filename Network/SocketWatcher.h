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

            void add_fd(int fd, std::shared_ptr<CSocket> client_data, int events, int oneshot);
            void remove_fd(int fd);
            int monitor(struct timeval* timeout);
            int get_ready_flags(int fd) const;
            std::shared_ptr<CSocket> get_client_data(unsigned int event_index) const;
            int get_fd_from_index(unsigned int event_index) const;
            void clear_event(int fd, unsigned int event_idx);
            int get_event_status(int fd, unsigned int event_idx) const;
            int get_buffer_size(int fd) const;

        private:
            int find_fd_index(int fd) const;

            fd_set              read_fds_;
            fd_set              write_fds_;
            std::vector<int>    monitored_fds_;
            std::vector<int>    ready_fd_indices_;
            fd_set              active_read_fds_;
            fd_set              active_write_fds_;
            int                 max_fds_;
            int                 num_monitored_fds_;
            std::vector<std::shared_ptr<CSocket>> fd_data_;
            std::vector<int>    fd_event_flags_;
    };
};
