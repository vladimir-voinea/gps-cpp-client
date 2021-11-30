#pragma once

#include <restinio/null_mutex.hpp>

#include <chrono>
#include <mutex>
#include <spdlog/spdlog.h>

template < typename Lock >
class spdlog_logger_t
{
	public:
        spdlog_logger_t() noexcept
        {}

		spdlog_logger_t( const spdlog_logger_t & ) = delete;
		spdlog_logger_t & operator = ( const spdlog_logger_t & ) = delete;

		template< typename Message_Builder >
		void
		trace(Message_Builder&& msg_builder )
		{
            std::lock_guard<Lock> hold(m_lock);
            spdlog::trace(msg_builder());
		}

		template< typename Message_Builder >
		void
		info(Message_Builder&& msg_builder )
		{
            std::lock_guard<Lock> hold(m_lock);
            spdlog::info(msg_builder());
		}

		template< typename Message_Builder >
		void
		warn(Message_Builder&& msg_builder )
		{
            std::lock_guard<Lock> hold(m_lock);
            spdlog::warn(msg_builder());
		}

		template< typename Message_Builder >
		void
		error(Message_Builder&& msg_builder )
		{
            std::lock_guard<Lock> hold(m_lock);
            spdlog::error(msg_builder());
		}

	private:
		Lock m_lock;
};

using single_threaded_spdlog_logger_t = spdlog_logger_t<restinio::null_mutex_t>;
using shared_ostream_spdlog_t = spdlog_logger_t<std::mutex>;
