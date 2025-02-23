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

#ifndef _LTSM_CONNECTOR_X11VNC_
#define _LTSM_CONNECTOR_X11VNC_

#include <unordered_map>
#include <memory>
#include <atomic>

#include "librfb_x11server.h"

namespace LTSM
{
    namespace Connector
    {
        /* Connector::VNC */
        class X11VNC : public RFB::X11Server
        {
            std::unordered_map<uint32_t, int> keymap;

            const JsonObject*           _config = nullptr;
            std::string                 _remoteaddr;

            std::atomic<bool>           _xcbDisableMessages{true};
            XCB::SharedDisplay          _xcbDisplay;

            PixelFormat                _format;

        protected:
            // rfb server encoding
            const PixelFormat &        serverFormat(void) const override;

            XCB::RootDisplayExt*       xcbDisplay(void) override;
            const XCB::RootDisplayExt* xcbDisplay(void) const override;
            bool                       xcbNoDamage(void) const override;
            bool                       xcbAllow(void) const override;
            void                       setXcbAllow(bool) override;

            bool                       rfbClipboardEnable(void) const override;
            bool                       rfbDesktopResizeEnabled(void) const override;
            RFB::SecurityInfo          rfbSecurityInfo(void) const override;
            int                        rfbUserKeycode(uint32_t) const override;

            void                       serverHandshakeVersionEvent(void) override;

            bool                       xcbConnect(void);
            bool                       loadKeymap(void);

        public:
            X11VNC(int fd, const JsonObject & jo);
        };
    }
}

#endif // _LTSM_CONNECTOR_X11VNC_
