/***************************************************************************
 *   Copyright © 2021 by Andrey Afletdinov <public.irkutsk@gmail.com>      *
 *                                                                         *
 *   Part of the LTSM: Linux Terminal Service Manager:                     *
 *   https://github.com/AndreyBarmaley/linux-terminal-service-manager      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <unistd.h>

#include <ctime>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <filesystem>

#include "ltsm_tools.h"
#include "ltsm_application.h"

namespace LTSM
{
    int Application::_debug = 2;

    Application::Application(const char* ident, int argc, const char** argv) : _argc(argc), _argv(argv), _ident(ident), _facility(LOG_USER)
    {
        ::openlog(_ident, 0, _facility);
	std::srand(std::time(0));
    }

    Application::~Application()
    {
        closelog();
    }

    ApplicationJsonConfig::ApplicationJsonConfig(const char* ident, int argc, const char** argv)
        : Application(ident, argc, argv)
    {
        std::string confPath;

        for(int it = 1; it < argc; ++it)
        {
            if(0 == std::strcmp(argv[it], "--config") && it + 1 < argc)
            {
                confPath.assign(argv[it + 1]);
                it = it + 1;
            }
        }

        if(confPath.empty())
        {
            const char* env = std::getenv("LTSM_CONFIG");

            if(env) confPath.assign(env);
        }

        if(confPath.empty())
        {
            for(auto path :
                { "config.json", "/etc/ltsm/config.json" })
            {
		auto st = std::filesystem::status(path);
                if(std::filesystem::file_type::not_found != st.type() &&
		    (st.permissions() & std::filesystem::perms::owner_read) != std::filesystem::perms::none)
                {
                    confPath.assign(path);
                    break;
                }
            }

            if(confPath.empty())
            {
                const std::string local = std::filesystem::path(Tools::dirname(argv[0])) / "config.json";
		auto st = std::filesystem::status(local);
                if(std::filesystem::file_type::not_found != st.type() &&
		    (st.permissions() & std::filesystem::perms::owner_read) != std::filesystem::perms::none)
                    confPath.assign(local);
            }
        }

        if(confPath.empty())
            throw std::string("config.json not found");

        info("used config: %s, running uid: %d", confPath.c_str(), getuid());
        JsonContentFile jsonFile(confPath);

        if(! jsonFile.isValid() || ! jsonFile.isObject())
            throw std::string("json parse error: ").append(confPath);

        _config = jsonFile.toObject();
        std::string str = _config.getString("logging:facility");
	int facility = 0;

        if(6 == str.size() && 0 == str.compare(0, 5, "local"))
        {
            switch(str[5])
            {
                case '0': facility = LOG_LOCAL0; break;
                case '1': facility = LOG_LOCAL1; break;
                case '2': facility = LOG_LOCAL2; break;
                case '3': facility = LOG_LOCAL3; break;
                case '4': facility = LOG_LOCAL4; break;
                case '5': facility = LOG_LOCAL5; break;
                case '6': facility = LOG_LOCAL6; break;
                case '7': facility = LOG_LOCAL7; break;
                default: break;
            }
        }

	if(0 < facility)
        {
	    closelog();
    	    ::openlog(_ident, 0, facility);
	    _facility = facility;
	}
    }
}
