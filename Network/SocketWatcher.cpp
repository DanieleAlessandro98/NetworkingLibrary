#include "StdAfx.h"
#include "SocketWatcher.h"

namespace Net
{
    int MIN(int a, int b)
    {
        return a < b ? a : b;
    }

    SocketWatcher::SocketWatcher(int max_files) : max_fds_(MIN(max_files, FD_SETSIZE)), num_monitored_fds_(0)
    {
        FD_ZERO(&read_fds_);
        FD_ZERO(&write_fds_);
        monitored_fds_.resize(max_fds_);
        ready_fd_indices_.resize(max_fds_);
        fd_data_.resize(max_fds_);
        fd_event_flags_.resize(max_fds_);
    }

    SocketWatcher::~SocketWatcher()
    {
    }

    void SocketWatcher::add_fd(int fd, CAbstractPeer* client_data, int events, int oneshot)
    {
        int idx = find_fd_index(fd);

        if (idx < 0)
        {
            if (num_monitored_fds_ >= max_fds_)
                return;

            idx = num_monitored_fds_;
            monitored_fds_[num_monitored_fds_++] = fd;
            fd_event_flags_[idx] = events;
        }
        else
        {
            fd_event_flags_[idx] |= events;
        }

        fd_data_[idx] = client_data;

        if (events & FDW_READ)
            FD_SET(fd, &read_fds_);

        if (events & FDW_WRITE)
            FD_SET(fd, &write_fds_);
    }

    void SocketWatcher::remove_fd(int fd)
    {
        if (num_monitored_fds_ <= 0)
            return;

        int idx = find_fd_index(fd);
        if (idx < 0)
            return;

        monitored_fds_[idx] = monitored_fds_[num_monitored_fds_];
        fd_data_[idx] = fd_data_[num_monitored_fds_];
        fd_event_flags_[idx] = fd_event_flags_[num_monitored_fds_];

        FD_CLR(fd, &read_fds_);
        FD_CLR(fd, &write_fds_);
    }

    int SocketWatcher::monitor(struct timeval* timeout)
    {
        int r, i, event_idx = 0;
        struct timeval tv;

        active_read_fds_ = read_fds_;
        active_write_fds_ = write_fds_;

        if (!timeout)
        {
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            r = select(0, &active_read_fds_, &active_write_fds_, nullptr, &tv);
        }
        else
        {
            tv = *timeout;
            r = select(0, &active_read_fds_, &active_write_fds_, nullptr, &tv);
        }

        if (r == -1)
            return -1;

        for (i = 0; i < num_monitored_fds_; ++i)
        {
            if (get_ready_flags(monitored_fds_[i]))
                ready_fd_indices_[event_idx++] = i;
        }

        return event_idx;
    }

    int SocketWatcher::get_ready_flags(int fd) const
    {
        int idx = find_fd_index(fd);
        if (idx < 0)
            return 0;

        int result = 0;
        if ((fd_event_flags_[idx] & FDW_READ) && FD_ISSET(fd, &active_read_fds_))
            result |= FDW_READ;

        if ((fd_event_flags_[idx] & FDW_WRITE) && FD_ISSET(fd, &active_write_fds_))
            result |= FDW_WRITE;

        return result;
    }

    CAbstractPeer* SocketWatcher::get_client_data(unsigned int event_index) const
    {
        if (event_index >= ready_fd_indices_.size())
            return nullptr;

        int idx = ready_fd_indices_[event_index];
        if (idx < 0 || max_fds_ <= idx)
            return nullptr;

        return fd_data_[idx];
    }

    int SocketWatcher::get_fd_from_index(unsigned int event_index) const
    {
        if (event_index >= ready_fd_indices_.size())
            return -1;

        int idx = ready_fd_indices_[event_index];
        if (idx < 0 || max_fds_ <= idx)
            return -1;

        return monitored_fds_[idx];
    }

    void SocketWatcher::clear_event(int fd, unsigned int event_idx)
    {
        if (event_idx >= ready_fd_indices_.size())
            return;

        int idx = ready_fd_indices_[event_idx];
        if (idx < 0 || max_fds_ <= idx)
            return;

        int rfd = monitored_fds_[idx];
        if (fd != rfd)
            return;

        FD_CLR(fd, &active_read_fds_);
        FD_CLR(fd, &active_write_fds_);
    }

    int SocketWatcher::get_event_status(int fd, unsigned int event_idx) const
    {
        if (event_idx >= ready_fd_indices_.size())
            return 0;

        int idx = ready_fd_indices_[event_idx];
        if (idx < 0 || max_fds_ <= idx)
            return 0;

        int rfd = monitored_fds_[idx];
        if (fd != rfd)
            return 0;

        int result = get_ready_flags(fd);
        if (result & FDW_READ)
            return FDW_READ;

        if (result & FDW_WRITE)
            return FDW_WRITE;

        return 0;
    }

    int SocketWatcher::find_fd_index(int fd) const
    {
        for (int i = 0; i < num_monitored_fds_; ++i)
        {
            if (monitored_fds_[i] == fd)
                return i;
        }

        return -1;
    }
};
