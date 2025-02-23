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

#ifndef _LTSM_TOOLS_
#define _LTSM_TOOLS_

#include <list>
#include <chrono>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <iomanip>
#include <iterator>
#include <algorithm>

#include <tuple>
#include <memory>
#include <atomic>
#include <thread>
#include <utility>
#include <filesystem>
#include <functional>

namespace LTSM
{

    namespace Tools
    {
        std::string prettyFuncName(std::string_view);
        std::string randomHexString(size_t len);

        std::vector<uint8_t> randomBytes(size_t bytesCount);

        std::string getTimeZone(void);
        std::string getUsername(void);

        class StringFormat : public std::string
        {
            int             cur = 1;

        public:
            StringFormat(std::string_view);

            StringFormat & arg(std::string_view);
            StringFormat & arg(int);
            StringFormat & arg(double, int prec);

            StringFormat & replace(std::string_view, int);
            StringFormat & replace(std::string_view, std::string_view);
            StringFormat & replace(std::string_view, double, int prec);
        };

        struct StreamBits
        {
            std::vector<uint8_t> vecbuf;
            size_t               bitpos = 0;

            bool empty(void) const;
            const std::vector<uint8_t> & toVector(void) const;
        };

        struct StreamBitsPack : StreamBits
        {
            StreamBitsPack();

            void pushBit(bool v);
            void pushValue(int val, size_t field);
            void pushAlign(void);
        };

        struct StreamBitsUnpack : StreamBits
        {
            StreamBitsUnpack(const std::vector<uint8_t> &, size_t counts, size_t field);

            bool popBit(void);
            int popValue(size_t field);
        };

        std::list<std::string> split(std::string_view str, std::string_view sep);
        std::list<std::string> split(std::string_view str, int sep);

	template<typename Iterator>
        std::string     join(Iterator it1, Iterator it2, std::string_view sep = "")
        {
            std::ostringstream os;
 
            if(sep.empty())
            {
                std::copy(it1, it2, std::ostream_iterator<std::string>(os));
            }
            else
            for(auto it = it1; it != it2; ++it)
            {
                os << *it;
 
                if(std::next(it) != it2)
                    os << sep;
            }
 
            return os.str();
        }

        std::string     join(const std::list<std::string> &, std::string_view sep = "");
        std::string     join(const std::vector<std::string> &, std::string_view sep = "");

        std::string     lower(std::string);
        std::string     runcmd(std::string_view);

        std::string     escaped(std::string_view, bool quote);
        std::string     unescaped(std::string);

        std::string     replace(const std::string & src, std::string_view pred, std::string_view val);
        std::string     replace(const std::string & src, std::string_view pred, int val);

        std::string     hex(int value, int width = 8);

        uint32_t        crc32b(std::string_view);
        uint32_t        crc32b(const uint8_t* ptr, size_t size);
        uint32_t        crc32b(const uint8_t* ptr, size_t size, uint32_t magic);

        bool            checkUnixSocket(const std::filesystem::path &);

        size_t		maskShifted(size_t mask);
        size_t		maskMaxValue(uint32_t mask);

	template<typename Int>
	std::string buffer2hexstring(const Int* data, size_t length, size_t width = 8, std::string_view sep = ",", bool prefix = true)
	{
    	    std::ostringstream os;
    	    for(size_t it = 0; it != length; ++it)
    	    {
                if(prefix)
        	    os << "0x";
                os << std::setw(width) << std::setfill('0') << std::uppercase << std::hex << static_cast<int>(data[it]);
        	if(sep.size() && it + 1 != length) os << sep;
    	    }

    	    return os.str();
	}

        // BaseSpinLock
        class SpinLock
        {
            std::atomic<bool> flag{false};

        public:
            bool tryLock(void) noexcept 
            {
                return ! flag.load(std::memory_order_relaxed) &&
                    ! flag.exchange(true, std::memory_order_acquire);
            }

            void lock(void) noexcept
            {
                for(;;)
                {
                    if(! flag.exchange(true, std::memory_order_acquire))
                        break;

                    while(flag.load(std::memory_order_relaxed))
                        std::this_thread::yield();
                }
            }

            void unlock(void) noexcept
            {
                flag.store(false, std::memory_order_release);
            }
        };


	// BaseTimer
	class BaseTimer
	{
	protected:
	    std::thread         thread;
	    std::atomic<bool>   processed{false};

	public:
	    BaseTimer() {}
            ~BaseTimer() { stop(true); }
    
	    std::thread::id 	getId(void) const
            {
                return thread.get_id();
            }

	    void		stop(bool wait = false)
            {
                processed = false;
                if(wait && thread.joinable()) thread.join();
            }

	    // usage:
	    // auto bt1 = BaseTimer::create<std::chrono::microseconds>(100, repeat, [=](){ func(param1, param2, param3); });
	    // auto bt2 = BaseTimer::create<std::chrono::seconds>(3, repeat, func, param1, param2, param3);
	    //
	    template <class TimeType = std::chrono::milliseconds, class Func>
	    static std::unique_ptr<BaseTimer> create(uint32_t delay, bool repeat, Func&& call)
	    {
    		auto ptr = std::make_unique<BaseTimer>();
    		ptr->thread = std::thread([delay, repeat, timer = ptr.get(), call = std::forward<Func>(call)]()
    		{
        	    timer->processed = true;
        	    auto start = std::chrono::steady_clock::now();
        	    while(timer->processed)
        	    {
            		std::this_thread::sleep_for(TimeType(1));
            		auto cur = std::chrono::steady_clock::now();

            		if(TimeType(delay) <= cur - start)
            		{
			    if(!timer->processed)
				break;

                	    call();

                            if(repeat)
        	                start = std::chrono::steady_clock::now();
                            else
                	        timer->processed = false;
            		}
        	    }
    		});
    		return ptr;
	    }

	    template <class TimeType = std::chrono::milliseconds, class Func, class... Args>
	    static std::unique_ptr<BaseTimer> create(uint32_t delay, bool repeat, Func&& call, Args&&... args)
	    {
    		auto ptr = std::make_unique<BaseTimer>();
    		ptr->thread = std::thread([delay, repeat, timer = ptr.get(),
		    call = std::forward<Func>(call), args = std::make_tuple(std::forward<Args>(args)...)]()
    		{
        	    timer->processed = true;
        	    auto start = std::chrono::steady_clock::now();
        	    while(timer->processed)
        	    {
            		std::this_thread::sleep_for(TimeType(1));

            		if(TimeType(delay) <= std::chrono::steady_clock::now() - start)
            		{
			    if(!timer->processed)
				break;

                	    std::apply(call, args);

                            if(repeat)
        	                start = std::chrono::steady_clock::now();
                            else
                	        timer->processed = false;
            		}
        	    }
    		});
    		return ptr;
	    }
	};

	// 
	template <class TimeType, class Func>
        bool waitCallable(uint32_t delay, uint32_t pause, Func&& call)
	{
            auto now = std::chrono::steady_clock::now();
    	    while(call())
    	    {
            	if(TimeType(delay) <= std::chrono::steady_clock::now() - now)
            	    return false;

        	std::this_thread::sleep_for(TimeType(pause));
	    }
	    return true;
        }
    }

#define NS_FuncName Tools::prettyFuncName(__PRETTY_FUNCTION__)
}

#endif // _LTSM_TOOLS_
