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

#ifndef _LTSM_LIBRFB_
#define _LTSM_LIBRFB_

#include <list>
#include <mutex>
#include <future>
#include <tuple>

#include "ltsm_sockets.h"
#include "ltsm_framebuffer.h"
#include "ltsm_xcb_wrapper.h"
#include "ltsm_json_wrapper.h"

namespace LTSM
{
    namespace RFB
    {
        // RFB protocol constant
        const int VERSION_MAJOR = 3;
        const int VERSION_MINOR = 8;

        const int SECURITY_TYPE_NONE = 1;
        const int SECURITY_TYPE_VNC = 2;
        const int SECURITY_TYPE_TLS = 18;
        const int SECURITY_TYPE_VENCRYPT = 19;
        const int SECURITY_VENCRYPT01_PLAIN = 19;
        const int SECURITY_VENCRYPT01_TLSNONE = 20;
        const int SECURITY_VENCRYPT01_TLSVNC = 21;
        const int SECURITY_VENCRYPT01_TLSPLAIN = 22;
        const int SECURITY_VENCRYPT01_X509NONE = 23;
        const int SECURITY_VENCRYPT01_X509VNC = 24;
        const int SECURITY_VENCRYPT01_X509PLAIN = 25;
        const int SECURITY_VENCRYPT02_PLAIN = 256;
        const int SECURITY_VENCRYPT02_TLSNONE = 257;
        const int SECURITY_VENCRYPT02_TLSVNC = 258;
        const int SECURITY_VENCRYPT02_TLSPLAIN = 259;
        const int SECURITY_VENCRYPT02_X509NONE = 260;
        const int SECURITY_VENCRYPT02_X509VNC = 261;
        const int SECURITY_VENCRYPT02_X509PLAIN = 261;

        const int SECURITY_RESULT_OK = 0;
        const int SECURITY_RESULT_ERR = 1;

        const int CLIENT_SET_PIXEL_FORMAT = 0;
        const int CLIENT_SET_ENCODINGS = 2;
        const int CLIENT_REQUEST_FB_UPDATE = 3;
        const int CLIENT_EVENT_KEY = 4;
        const int CLIENT_EVENT_POINTER = 5;
        const int CLIENT_CUT_TEXT = 6;
        const int CLIENT_ENABLE_CONTINUOUS_UPDATES = 150;
        const int CLIENT_SET_DESKTOP_SIZE = 251;

        const int SERVER_FB_UPDATE = 0;
        const int SERVER_SET_COLOURMAP = 1;
        const int SERVER_BELL = 2;
        const int SERVER_CUT_TEXT = 3;

        // RFB protocol constants
        const int ENCODING_RAW = 0;
        const int ENCODING_COPYRECT = 1;
        const int ENCODING_RRE = 2;
        const int ENCODING_CORRE = 4;
        const int ENCODING_HEXTILE = 5;
        const int ENCODING_ZLIB = 6;
        const int ENCODING_TIGHT = 7;
        const int ENCODING_ZLIBHEX = 8;
        const int ENCODING_TRLE = 15;
        const int ENCODING_ZRLE = 16;
 
        // hextile constants
        const int HEXTILE_RAW = 1;
        const int HEXTILE_BACKGROUND = 2;
        const int HEXTILE_FOREGROUND = 4;
        const int HEXTILE_SUBRECTS = 8;
        const int HEXTILE_COLOURED = 16;
        const int HEXTILE_ZLIBRAW = 32;
        const int HEXTILE_ZLIB = 64;
 
        // pseudo encodings
        const int ENCODING_DESKTOP_SIZE = -223;
        const int ENCODING_EXT_DESKTOP_SIZE = -308;
        const int ENCODING_CONTINUOUS_UPDATES = -313;
        const int ENCODING_LAST_RECT = -224;
        const int ENCODING_COMPRESS9 = -247;
        const int ENCODING_COMPRESS8 = -248;
        const int ENCODING_COMPRESS7 = -249;
        const int ENCODING_COMPRESS6 = -250;
        const int ENCODING_COMPRESS5 = -251;
        const int ENCODING_COMPRESS4 = -252;
        const int ENCODING_COMPRESS3 = -253;
        const int ENCODING_COMPRESS2 = -254;
        const int ENCODING_COMPRESS1 = -255;

	struct ScreenInfo
	{
            uint32_t		id = 0;
            uint16_t		width = 0;
            uint16_t		height = 0;

            ScreenInfo() = default;
            ScreenInfo(uint32_t id1, uint16_t width1, uint16_t height1) : id(id1), width(width1), height(height1) {}

            bool operator== (const ScreenInfo & si) const { return id == si.id && width == si.width && height == si.height; }
	};

        enum class DesktopResizeMode { Undefined, Disabled, Success, ServerInform, ClientRequest };
        const char* desktopResizeModeString(const DesktopResizeMode &);

        const char* encodingName(int type);
        typedef std::function<void(const FrameBuffer &)> sendEncodingFunc;

        /// SecurityInfo
        struct SecurityInfo
        {
            bool                authNone = false;
            bool                authVnc = false;
            bool                authVenCrypt = false;

            std::string         passwdFile;
            std::string         tlsPriority;
            std::string         caFile;
            std::string         certFile;
            std::string         keyFile;
            std::string         crlFile;
            bool                tlsAnonMode = false;
            int                 tlsDebug = 0;
        };

        /// ServerEncoding
        class ServerEncoding : protected NetworkStream
        {
            std::list< std::future<void> >
                                encodingJobs;
            std::vector<int>    clientEncodings;
            std::list<std::string> disabledEncodings;
            std::list<std::string> prefferedEncodings;

            std::unique_ptr<NetworkStream> socket;      /// socket layer
            std::unique_ptr<TLS::Stream> tls;           /// tls layer
            std::unique_ptr<ZLib::DeflateStream> zlib;  /// zlib layer

            PixelFormat         serverFormat;
            PixelFormat         clientFormat;
            ColorMap            colourMap;
            std::mutex		encodingBusy;
            std::mutex          networkBusy;
            std::atomic<int>    pressedMask{0};
            std::atomic<bool>   fbUpdateProcessing{false};
	    std::atomic<DesktopResizeMode>
                                desktopMode{DesktopResizeMode::Undefined};
	    RFB::ScreenInfo     desktopScreenInfo;
            mutable size_t      netStatRx = 0;
            mutable size_t      netStatTx = 0;
            int                 encodingDebug = 0;
            int                 encodingThreads = 2;
            bool                clientTrueColor = true;
            bool                clientBigEndian = false;
            NetworkStream*      streamIn = nullptr;
            NetworkStream*      streamOut = nullptr;

            std::pair<RFB::sendEncodingFunc, int>
                                prefEncodingsPair;
        protected:
            // librfb interface
            virtual XCB::RootDisplayExt* xcbDisplay(void) const = 0;
            virtual bool        serviceAlive(void) const = 0;
            virtual void        serviceStop(void) = 0;
            virtual void        serverPostProcessingFrameBuffer(FrameBuffer &) {}

           // network stream interface
            void                sendFlush(void) override;
            void                sendRaw(const void* ptr, size_t len) override;
            void                recvRaw(void* ptr, size_t len) const override;
            void                recvRaw(void* ptr, size_t len, size_t timeout) const override;
            bool                hasInput(void) const override;
            size_t              hasData(void) const override;
            uint8_t             peekInt8(void) const override;

	    void		zlibDeflateStart(size_t);
	    std::vector<uint8_t> zlibDeflateStop(void);

            void                setDisabledEncodings(std::list<std::string>);
            void                setPrefferedEncodings(std::list<std::string>);

            std::string         serverEncryptionInfo(void) const;

            void                setEncodingDebug(int v);
            void                setEncodingThreads(int v);
            bool                isClientEncodings(int) const;

            bool                isUpdateProcessed(void) const;
            void                waitUpdateProcess(void);

            bool                serverSelectClientEncoding(void);
            void                serverSetPixelFormat(const PixelFormat &);

            bool                desktopResizeModeInit(void);
            void                desktopResizeModeDisable(void);
            void                desktopResizeModeSet(const DesktopResizeMode &, const std::vector<RFB::ScreenInfo> &);
            bool                desktopResizeModeChange(const XCB::Size &);
            DesktopResizeMode   desktopResizeMode(void) const;

            bool                serverAuthVncInit(const std::string &);
            bool                serverAuthVenCryptInit(const SecurityInfo &);
            void                serverSendUpdateBackground(const XCB::Region &);

            void                clientSetPixelFormat(void);
            bool                clientSetEncodings(void);

        public:
            ServerEncoding(int sockfd = 0);

            int                 serverHandshakeVersion(void);
            bool                serverSecurityInit(int protover, const SecurityInfo &);
            void                serverClientInit(std::string_view);

            bool                serverSendFrameBufferUpdate(const XCB::Region &);
            void                serverSendColourMap(int first);
            void                serverSendBell(void);
            void                serverSendCutText(const std::vector<uint8_t> &);
            void                serverSendEndContinuousUpdates(void);

            void                serverSelectEncodings(void);

            void                sendEncodingRaw(const FrameBuffer &);
            void                sendEncodingRawSubRegion(const XCB::Point &, const XCB::Region &, const FrameBuffer &, int jobId);
            void                sendEncodingRawSubRegionRaw(const XCB::Region &, const FrameBuffer &);

            void                sendEncodingRRE(const FrameBuffer &, bool corre);
            void                sendEncodingRRESubRegion(const XCB::Point &, const XCB::Region &, const FrameBuffer &, int jobId, bool corre);
            void                sendEncodingRRESubRects(const XCB::Region &, const FrameBuffer &, int jobId, int back, const std::list<XCB::RegionPixel> &, bool corre);

            void                sendEncodingHextile(const FrameBuffer &, bool zlibver);
            void                sendEncodingHextileSubRegion(const XCB::Point &, const XCB::Region &, const FrameBuffer &, int jobId, bool zlibver);
            void                sendEncodingHextileSubForeground(const XCB::Region &, const FrameBuffer &, int jobId, int back, const std::list<XCB::RegionPixel> &);
            void                sendEncodingHextileSubColored(const XCB::Region &, const FrameBuffer &, int jobId, int back, const std::list<XCB::RegionPixel> &);
            void                sendEncodingHextileSubRaw(const XCB::Region &, const FrameBuffer &, int jobId, bool zlibver);

            void                sendEncodingZLib(const FrameBuffer &);
            void                sendEncodingZLibSubRegion(const XCB::Point &, const XCB::Region &, const FrameBuffer &, int jobId);

            void                sendEncodingTRLE(const FrameBuffer &, bool zrle);
            void                sendEncodingTRLESubRegion(const XCB::Point &, const XCB::Region &, const FrameBuffer &, int jobId, bool zrle);
            void                sendEncodingTRLESubPacked(const XCB::Region &, const FrameBuffer &, int jobId, size_t field, const PixelMapWeight &, bool zrle);
	    void                sendEncodingTRLESubPlain(const XCB::Region &, const FrameBuffer &, const std::list<PixelLength> &);
	    void                sendEncodingTRLESubPalette(const XCB::Region &, const FrameBuffer &, const PixelMapWeight &, const std::list<PixelLength> &);
	    void                sendEncodingTRLESubRaw(const XCB::Region &, const FrameBuffer &);

	    bool		sendEncodingDesktopSize(bool xcbAllow);

            std::pair<bool, XCB::Region>
                                clientFramebufferUpdate(void);
            void                clientKeyEvent(bool xcbAllow, const JsonObject* keymap = nullptr);
            void                clientPointerEvent(bool xcbAllow);
            void                clientCutTextEvent(bool xcbAllow, bool clipboardEnable);
            void                clientSetDesktopSizeEvent(void);
            void                clientEnableContinuousUpdates(void);
            void                clientDisconnectedEvent(int display);

            int                 sendPixel(uint32_t pixel);
            int                 sendCPixel(uint32_t pixel);
            int                 sendRunLength(size_t length);
        };
    }
}

#endif // _LTSM_LIBRFB_
