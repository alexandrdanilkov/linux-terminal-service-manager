/***********************************************************************
 *   Copyright © 2021 by Andrey Afletdinov <public.irkutsk@gmail.com>  *
 *                                                                     *
 *   Part of the LTSM: Linux Terminal Service Manager:                 *
 *   https://github.com/AndreyBarmaley/linux-terminal-service-manager  *
 *                                                                     *
 *   This program is free software;                                    *
 *   you can redistribute it and/or modify it under the terms of the   *
 *   GNU Affero General Public License as published by the             *
 *   Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                               *
 *                                                                     *
 *   This program is distributed in the hope that it will be useful,   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU Affero General Public License for more details.       *
 *                                                                     *
 *   You should have received a copy of the                            *
 *   GNU Affero General Public License along with this program;        *
 *   if not, write to the Free Software Foundation, Inc.,              *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.         *
 **********************************************************************/

#ifndef _LTSM_SOCKETS_
#define _LTSM_SOCKETS_

#include <array>
#include <atomic>
#include <vector>
#include <thread>
#include <string>
#include <string_view>
#include <filesystem>
#include <memory>
#include <cstdint>
#include <cstdio>
#include <stdexcept>

#ifdef LTSM_SOCKET_ZLIB
#include <zlib.h>
#endif

#ifdef LTSM_SOCKET_TLS
#include "gnutls/gnutls.h"
#include "gnutls/gnutlsxx.h"
#endif

#include "ltsm_streambuf.h"

#define LTSM_SOCKETS_VERSION 20221002

namespace LTSM
{
    struct network_error : public std::runtime_error
    {
        explicit network_error(const std::string & what) : std::runtime_error(what){}
        explicit network_error(const char* what) : std::runtime_error(what){}
    };

    /// @brief: network stream interface
    class NetworkStream : protected ByteOrderInterface
    {
    protected:
        inline void             getRaw(void* ptr, size_t len) const override { recvRaw(ptr, len); };
        inline void             putRaw(const void* ptr, size_t len) override { sendRaw(ptr, len); };

    public:
        NetworkStream() =  default;
        virtual ~NetworkStream() = default;

        static bool             hasInput(int fd, int timeoutMS = 1);
        static size_t           hasData(int fd);

#ifdef LTSM_SOCKET_TLS
        virtual void            setupTLS(gnutls::session*) const {}
#endif
        inline NetworkStream &  sendIntBE16(uint16_t x) { putIntBE16(x); return *this; }
        inline NetworkStream &  sendIntBE32(uint32_t x) { putIntBE32(x); return *this; }
        inline NetworkStream &  sendIntBE64(uint64_t x) { putIntBE64(x); return *this; }

        inline NetworkStream &  sendIntLE16(uint16_t x) { putIntLE16(x); return *this; }
        inline NetworkStream &  sendIntLE32(uint32_t x) { putIntLE32(x); return *this; }
        inline NetworkStream &  sendIntLE64(uint64_t x) { putIntLE64(x); return *this; }

        NetworkStream &         sendInt8(uint8_t);
        NetworkStream &         sendInt16(uint16_t);
        NetworkStream &         sendInt32(uint32_t);
        NetworkStream &         sendInt64(uint64_t);

        NetworkStream & 	sendZero(size_t);
        NetworkStream & 	sendData(const std::vector<uint8_t> &);

        virtual void            sendFlush(void) {}
        virtual void		sendRaw(const void*, size_t) = 0;

        virtual bool            hasInput(void) const = 0;
        virtual size_t          hasData(void) const = 0;
        virtual uint8_t		peekInt8(void) const = 0;

        inline uint16_t         recvIntBE16(void) const { return getIntBE16(); }
        inline uint32_t	        recvIntBE32(void) const { return getIntBE32(); }
        inline uint64_t	        recvIntBE64(void) const { return getIntBE64(); }

        inline uint16_t	        recvIntLE16(void) const { return getIntLE16(); }
        inline uint32_t	        recvIntLE32(void) const { return getIntLE32(); }
        inline uint64_t	        recvIntLE64(void) const { return getIntLE64(); }

        uint8_t		        recvInt8(void) const;
        uint16_t	        recvInt16(void) const;
        uint32_t	        recvInt32(void) const;
        uint64_t	        recvInt64(void) const;

        void                    recvSkip(size_t) const;
        std::vector<uint8_t>    recvData(size_t) const;
        void                    recvData(void* ptr, size_t len) const;

        virtual void            recvRaw(void*, size_t) const = 0;

        NetworkStream &         sendString(std::string_view);
        std::string	        recvString(size_t) const;
    };

    namespace FileDescriptor
    {
        void			write(int fd, const void*, ssize_t);
        void	                read(int fd, void*, ssize_t);
    };

    /// @brief: socket stream
    class SocketStream : public NetworkStream
    {
    protected:
        int                     sock;
        std::vector<uint8_t>    buf;

    public:
        explicit SocketStream(int fd = 0);
        ~SocketStream();

        SocketStream(const SocketStream &) = delete;
        SocketStream & operator=(const SocketStream &) = delete;

#ifdef LTSM_SOCKET_TLS
        void                    setupTLS(gnutls::session*) const override;
#endif
        void                    setSocket(int fd) { sock = fd; }

        bool                    hasInput(void) const override;
        size_t                  hasData(void) const override;
        uint8_t	                peekInt8(void) const override;

        void			sendRaw(const void*, size_t) override;
        void	                recvRaw(void*, size_t) const override;

        void                    sendFlush(void) override;
    };

    /// @brief: inetd stream
    class InetStream : public NetworkStream
    {
    protected:
        std::unique_ptr<FILE, decltype(fclose)*> fin{nullptr, fclose};
        std::unique_ptr<FILE, decltype(fclose)*> fout{nullptr, fclose};
        int                     fdin = 0;
        int                     fdout = 0;
        std::array<char, 1492>  fdbuf;

        void                    inetFdClose(void);

    public:
        InetStream();

#ifdef LTSM_SOCKET_TLS
        void                    setupTLS(gnutls::session*) const override;
#endif
        bool                    hasInput(void) const override;
        size_t                  hasData(void) const override;
        uint8_t		        peekInt8(void) const override;

        void                    sendFlush(void) override;
        bool                    checkError(void) const;

        void			sendRaw(const void*, size_t) override;
        void	                recvRaw(void*, size_t) const override;
    };

    /// @brief: proxy socket: stdin/stdout to local socket
    class ProxySocket : protected InetStream
    {
    protected:
        std::atomic<bool>       loopTransmission{false};
        std::thread             loopThread;
        int                     bridgeSock = -1;
        int                     clientSock = -1;
        std::filesystem::path   socketPath;

    protected:
        bool                    transmitDataIteration(void);

    public:
        ProxySocket() = default;
        ~ProxySocket();
            
        int                     proxyClientSocket(void) const;
        bool                    proxyInitUnixSockets(const std::filesystem::path &);
        bool                    proxyRunning(void) const;

        void                    proxyStartEventLoop(void);
        void                    proxyStopEventLoop(void);
        void                    proxyShutdown(void);
    };

    namespace TCPSocket
    {
        std::string             resolvAddress(std::string_view ipaddr);
        std::string             resolvHostname(std::string_view hostname);
        std::list<std::string>  resolvHostname2(std::string_view hostname);
        int                     connect(std::string_view ipaddr, uint16_t port);
        int                     listen(uint16_t port, int conn = 5);
        int                     listen(std::string_view ipaddr, uint16_t port, int conn = 5);
        int                     accept(int fd);
    }

    namespace UnixSocket
    {
        int                     connect(const std::filesystem::path &);
        int                     listen(const std::filesystem::path &, int conn = 5);
        int                     accept(int fd);
    }

#ifdef LTSM_SOCKET_TLS
    struct gnutls_error : public std::runtime_error
    {
        explicit gnutls_error(const std::string & what) : std::runtime_error(what){}
        explicit gnutls_error(const char* what) : std::runtime_error(what){}
    };

    /// transport layer security
    namespace TLS
    {
        std::vector<uint8_t>    randomKey(size_t);
        std::vector<uint8_t>    encryptDES(const std::vector<uint8_t> & crypt, std::string_view key);

        class Session : public gnutls::session
        {
        public:
#if GNUTLS_VERSION_NUMBER < 0x030603
            static gnutls_session_t ptr(gnutls::session* sess)
            {
                return sess->*(& Session::s);
            }
#else
            static gnutls_session_t ptr(gnutls::session* sess)
            {
                return sess->ptr();
            }
#endif
        };

        /// @brief: tls stream
        class Stream : public NetworkStream
        {
        private:
            bool                startHandshake(void);
            const NetworkStream* layer = nullptr;

            gnutls::dh_params   dhparams;
            std::unique_ptr<gnutls::credentials> cred;
            std::unique_ptr<gnutls::session> session;

            bool                handshake = false;
            mutable int         peek = -1;

        public:
            explicit Stream(const NetworkStream*);
            ~Stream();

            bool                hasInput(void) const override;
            size_t              hasData(void) const override;
            void                sendFlush(void) override;
            uint8_t	        peekInt8(void) const override;

            void		sendRaw(const void*, size_t) override;
            void                recvRaw(void*, size_t) const override;

	    std::string		sessionDescription(void) const;

            bool                initAnonHandshake(std::string_view priority, bool srvmode, int debug);
            bool                initX509Handshake(std::string_view priority, bool srvmode, const std::string & caFile, const std::string & certFile,
                                                        const std::string & keyFile, const std::string & crlFile, int debug);
        };

        class AnonSession : public Stream
        {
        public:
            AnonSession(const NetworkStream*, std::string_view priority, bool serverMode = true, int debug = 3);
        };

        class X509Session : public Stream
        {
        public:
            X509Session(const NetworkStream*, const std::string & cafile, const std::string & cert, const std::string & key,
                        const std::string & crl, std::string_view priority, bool serverMode = true, int debug = 3);
        };
    } // TLS
#endif // LTSM_SOCKET_TLS

#ifdef LTSM_SOCKET_ZLIB
    struct zlib_error : public std::runtime_error
    {
        explicit zlib_error(const std::string & what) : std::runtime_error(what){}
        explicit zlib_error(const char* what) : std::runtime_error(what){}
    };

    namespace ZLib
    {
        /// @brief: zlib compress output stream only
        class DeflateStream : public NetworkStream
        {
        protected:
            z_stream            zs{0};
            BinaryBuf           bb;
            
        public:
            explicit DeflateStream(int level = Z_BEST_COMPRESSION);
            ~DeflateStream();
    
            std::vector<uint8_t> deflateFlush(void);
            void                prepareSize(size_t);

            bool                hasInput(void) const override;
            size_t              hasData(void) const override;
            void                sendRaw(const void*, size_t) override;
            
        private:
            void                recvRaw(void*, size_t) const override;
            uint8_t             peekInt8(void) const override;
        };

        /// @brief: zlib compress input stream only
        class InflateStream : public NetworkStream
        {
        protected:
            z_stream            zs{0};
            StreamBuf           sb;

        public:
            InflateStream();
            ~InflateStream();

            void                appendData(const std::vector<uint8_t> &);

            bool                hasInput(void) const override;
            size_t              hasData(void) const override;
            void                recvRaw(void*, size_t) const override;
            uint8_t             peekInt8(void) const override;

        private:
            void                sendRaw(const void*, size_t) override;
        };
    } // Zlib
#endif // LTSM_SOCKET_ZLIB
} // LTSM

#endif // _LTSM_SOCKETS_
