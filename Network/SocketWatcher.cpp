#include "StdAfx.h"
#include "SocketWatcher.h"

namespace Net
{
    int MIN(int a, int b)
    {
        return a < b ? a : b;
    }

    SocketWatcher::SocketWatcher(int max_files) : max_sockets_(MIN(max_files, FD_SETSIZE)), num_monitored_sockets_(0)
    {
        FD_ZERO(&read_sockets_);
        FD_ZERO(&write_sockets_);
        monitored_sockets_.resize(max_sockets_);
        ready_socket_indices_.resize(max_sockets_);
        socket_data_.resize(max_sockets_);
        socket_event_flags_.resize(max_sockets_);
    }

    SocketWatcher::~SocketWatcher()
    {
    }

    void SocketWatcher::add_socket(int socket, CAbstractPeer* client_data, int events, int oneshot)
    {
        int idx = find_socket_index(socket);

        if (idx < 0)
        {
            if (num_monitored_sockets_ >= max_sockets_)
                return;

            idx = num_monitored_sockets_;
            monitored_sockets_[num_monitored_sockets_++] = socket;
            socket_event_flags_[idx] = events;
        }
        else
        {
            socket_event_flags_[idx] |= events;
        }

        socket_data_[idx] = client_data;

        if (events & SOCKW_READ)
            FD_SET(socket, &read_sockets_);

        if (events & SOCKW_WRITE)
            FD_SET(socket, &write_sockets_);
    }

    void SocketWatcher::remove_socket(int socket)
    {
        if (num_monitored_sockets_ <= 0)
            return;

        int idx = find_socket_index(socket);
        if (idx < 0)
            return;

        monitored_sockets_[idx] = monitored_sockets_[num_monitored_sockets_];
        socket_data_[idx] = socket_data_[num_monitored_sockets_];
        socket_event_flags_[idx] = socket_event_flags_[num_monitored_sockets_];

        FD_CLR(socket, &read_sockets_);
        FD_CLR(socket, &write_sockets_);
    }

    int SocketWatcher::monitor(struct timeval* timeout)
    {
        int r, i, event_idx = 0;
        struct timeval tv;

        active_read_sockets_ = read_sockets_;
        active_write_sockets_ = write_sockets_;

        if (!timeout)
        {
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            r = select(0, &active_read_sockets_, &active_write_sockets_, nullptr, &tv);
        }
        else
        {
            tv = *timeout;
            r = select(0, &active_read_sockets_, &active_write_sockets_, nullptr, &tv);
        }

        if (r == -1)
            return -1;

        for (i = 0; i < num_monitored_sockets_; ++i)
        {
            if (get_ready_flags(monitored_sockets_[i]))
                ready_socket_indices_[event_idx++] = i;
        }

        return event_idx;
    }

    int SocketWatcher::get_ready_flags(int socket) const
    {
        int idx = find_socket_index(socket);
        if (idx < 0)
            return 0;

        int result = 0;
        if ((socket_event_flags_[idx] & SOCKW_READ) && FD_ISSET(socket, &active_read_sockets_))
            result |= SOCKW_READ;

        if ((socket_event_flags_[idx] & SOCKW_WRITE) && FD_ISSET(socket, &active_write_sockets_))
            result |= SOCKW_WRITE;

        return result;
    }

    CAbstractPeer* SocketWatcher::get_client_data(unsigned int event_index) const
    {
        if (event_index >= ready_socket_indices_.size())
            return nullptr;

        int idx = ready_socket_indices_[event_index];
        if (idx < 0 || max_sockets_ <= idx)
            return nullptr;

        return socket_data_[idx];
    }

    int SocketWatcher::get_socket_from_index(unsigned int event_index) const
    {
        if (event_index >= ready_socket_indices_.size())
            return -1;

        int idx = ready_socket_indices_[event_index];
        if (idx < 0 || max_sockets_ <= idx)
            return -1;

        return monitored_sockets_[idx];
    }

    void SocketWatcher::clear_event(int socket, unsigned int event_idx)
    {
        if (event_idx >= ready_socket_indices_.size())
            return;

        int idx = ready_socket_indices_[event_idx];
        if (idx < 0 || max_sockets_ <= idx)
            return;

        int rfd = monitored_sockets_[idx];
        if (socket != rfd)
            return;

        FD_CLR(socket, &active_read_sockets_);
        FD_CLR(socket, &active_write_sockets_);
    }

    int SocketWatcher::get_event_status(int socket, unsigned int event_idx) const
    {
        if (event_idx >= ready_socket_indices_.size())
            return 0;

        int idx = ready_socket_indices_[event_idx];
        if (idx < 0 || max_sockets_ <= idx)
            return 0;

        int rfd = monitored_sockets_[idx];
        if (socket != rfd)
            return 0;

        int result = get_ready_flags(socket);
        if (result & SOCKW_READ)
            return SOCKW_READ;

        if (result & SOCKW_WRITE)
            return SOCKW_WRITE;

        return 0;
    }

    int SocketWatcher::find_socket_index(int socket) const
    {
        for (int i = 0; i < num_monitored_sockets_; ++i)
        {
            if (monitored_sockets_[i] == socket)
                return i;
        }

        return -1;
    }
};
