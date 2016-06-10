#if defined(_IPOP_OSX)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ifaddrs.h>    //  struct ifaddrs, getifaddrs(),
#include <net/if_dl.h>  //  struct sockaddr_dl
#include <net/route.h>  //  struct rt_msghdr
#include <sys/errno.h>  //  errno

#include <iostream>

#include "osx/tapdev_osx.h"
#include "osx/osx_exception.h"
#include <aio.h>

using std::cout;

namespace tincan {

namespace osx {
const char *const TapDevOsx::TUN_PATH = "/dev/tap0";
const char *const TapDevOsx::PATH_2_IFCONFIG = "/sbin/ifconfig";
const char *const TapDevOsx::IFCONFIG = "ifconfig";
const int TapDevOsx::MAX_ADAPTER_ADDRESS_LENGTH = 50;
int TapDevOsx::ipv4_config_sock = -1;
int TapDevOsx::ipv6_config_sock = -1;
struct ifreq TapDevOsx::ifr;

/* CALLBACK */
void ReadCompletionRoutine(sigval sigval)
{
    // process the async read, and initiate a new read
    struct aiocb *req = (struct aiocb *) sigval.sival_ptr;
    if (aio_error(req) == 0) {
        // request completed successfully
        if (aio_return(req) < 0) {
            // error handling
        } else {
//            ReadCompletion & rdcompl = ;
//            struct aiocb tmp;
//            tmp.aio_buf = rd_aio_->aio_buf;
//            tmp.aio_fildes = rd_aio_->aio_fildes;
//            tmp.aio_offset = 0;
//            tmp.aio_nbytes = rd_aio_->aio_nbytes;
//            // set up the callback function
//            tmp.aio_sigevent.sigev_notify = SIGEV_THREAD;
//            tmp.aio_sigevent.sigev_notify_function = ReadCompletionRoutine;
//            tmp.aio_sigevent.sigev_notify_attributes = NULL;
//            tmp.aio_sigevent.sigev_value.sival_ptr = &tmp;
//            int ret = aio_read(&tmp);
        }
    }
    return;
}


TapDevOsx::TapDevOsx(
    const string & tap_name,
    unique_ptr<AsyncRead> async_rd,
    unique_ptr<AsyncWrite> async_wr) :
    rd_aio_(std::move(async_rd)),
    wr_aio_(std::move(async_wr)),
    is_read_started_(false)
{}

TapDevOsx::TapDevOsx():
    is_read_started_(false)
{}

TapDevOsx::~TapDevOsx()
{}


unique_ptr<BYTE[]> TapDevOsx::GetMacAddress(const string & device_name)
{
    struct ifaddrs *ifa_list, *cur;
    unique_ptr<BYTE[]> mac_address;
    if (getifaddrs(&ifa_list) < 0) {
        throw OSXEXCEPT("Could not get the network interface list");
    }
    for (cur = ifa_list; cur != NULL; cur = cur->ifa_next) {
        if (cur->ifa_addr->sa_family == AF_LINK
            && (device_name.compare(cur->ifa_name) == 0)) {
            struct sockaddr_dl *dl = (struct sockaddr_dl *)cur->ifa_addr;
            mac_address.reset(new BYTE[MAX_ADAPTER_ADDRESS_LENGTH]);
            memset(mac_address.get(), 0, MAX_ADAPTER_ADDRESS_LENGTH);
            memcpy(mac_address.get(), (LLADDR(dl)), dl->sdl_len);
        }
    }
    return move(mac_address);
}

void TapDevOsx::Open()
{
    if (tap_name_.length() >= IFNAMSIZ) {
        throw OSXEXCEPT("Device name is longer than IFNAMESIZ - 1");
    }
    auto tap_handle = open(TUN_PATH, O_RDWR);
    if (tap_handle < 0) {
        throw OSXEXCEPT("Opening Tap Device failed. (Are we not root?)");
    }
    
    if ((ipv4_config_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw OSXEXCEPT("UDP IPv4 socket creation failed");
    }
    
    if ((ipv6_config_sock = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        throw OSXEXCEPT("UDP IPv6 socket creation failed");
    }
    
    unique_ptr<BYTE[]> mac_address = GetMacAddress(tap_name_);
    strcpy(ifr.ifr_name, tap_name_.c_str());
    SetDevHandle(&tap_handle);
}

void TapDevOsx::Open(
    const string & tap_name,
    unique_ptr<AsyncRead> async_rd,
    unique_ptr<AsyncWrite> async_wr)
{
    tap_name_ = tap_name;
    rd_aio_ = move(async_rd);
    wr_aio_ = move(async_wr);
    TapDevOsx::Open();
}

void TapDevOsx::StartRead()
{
    if (is_read_started_) { return; }
    is_read_started_ = true;
    // initiate new async read
    struct aiocb tmp;
    tmp.aio_buf = rd_aio_->aio_buf;
    tmp.aio_fildes = rd_aio_->aio_fildes;
    tmp.aio_offset = 0;
    tmp.aio_nbytes = rd_aio_->aio_nbytes;
    // set up the callback function
    tmp.aio_sigevent.sigev_notify = SIGEV_THREAD;
    tmp.aio_sigevent.sigev_notify_function = ReadCompletionRoutine;
    tmp.aio_sigevent.sigev_notify_attributes = NULL;
    tmp.aio_sigevent.sigev_value.sival_ptr = &tmp;
    
    int ret = aio_read(&tmp);
}

void TapDevOsx::Write(TapFrame & frame)
{
    // initiate new async write
}

void TapDevOsx::SetDevHandle(HANDLE handle)
{
    rd_aio_->dev_handle = wr_aio_->dev_handle = handle;
}

void TapDevOsx::SetFlags(short enable, short disable)
{
    if (ioctl(ipv6_config_sock, SIOCGIFFLAGS, &ifr) < 0) {
        Close();
        throw OSXEXCEPT("Could not read device flags for TAP device");
    }
    ifr.ifr_flags |= enable;
    ifr.ifr_flags &= ~disable;
    if (ioctl(ipv6_config_sock, SIOCSIFFLAGS, &ifr) < 0) {
        Close();
        throw OSXEXCEPT("Could not write back device flags for TAP device");
    }
}

void TapDevOsx::EnableArp()
{
    SetFlags((short) 0, (short) IFF_NOARP);
}

void TapDevOsx::Up()
{
    SetFlags(IFF_UP | IFF_RUNNING, (short) 0);
}

void TapDevOsx::Down()
{
    SetFlags((short) 0, IFF_UP | IFF_RUNNING);
}

void TapDevOsx::SetMtu(int mtu)
{
    ifr.ifr_mtu = mtu;
    if (ioctl(ipv6_config_sock, SIOCSIFMTU, &ifr) < 0) {
        Close();
        throw OSXEXCEPT("Set MTU failed");
    }
}

void TapDevOsx::Close()
{}

}   // namespace osx
}   // namespace tincan
#endif