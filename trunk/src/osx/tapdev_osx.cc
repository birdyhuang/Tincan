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

using std::cout;

namespace tincan {

namespace osx {
const char *const TapDevOsx::TUN_PATH = "/dev/tap0";
const char *const TapDevOsx::PATH_2_IFCONFIG = "/sbin/ifconfig";
const char *const TapDevOsx::IFCONFIG = "ifconfig";
const int TapDevOsx::MAX_ADAPTER_ADDRESS_LENGTH = 50;

unique_ptr<BYTE[]> TapDevOsx::getMacAddress(const string & device_name) {
    struct ifaddrs *ifa_list, *cur;
    unique_ptr<BYTE[]> mac_address;
    if (getifaddrs(&ifa_list) < 0) {
        throw OSXEXCEPT("Could not get the network interface list");
        return NULL;
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

void TapDevOsx::Open(const string & device_name) {
    if ((fd = open(TUN_PATH, O_RDWR)) < 0) {
        throw OSXEXCEPT("Opening Tap Device failed. (Are we not root?)");
//        cerr<<"Opening "<<TUN_PATH<<" failed. (Are we not root?)"<<endl;
    }
    
    if (device_name.length() >= IFNAMSIZ) {
        throw OSXEXCEPT("Device name is longer than IFNAMESIZ - 1");
//        cerr<<"Device name "<<device_name<<" is longer than IFNAMESIZ - 1 "<<IFNAMSIZ - 1<<endl;
    }
    
    if ((ipv4_config_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        throw OSXEXCEPT("UDP IPv4 socket creation failed");
    }
    
    if ((ipv6_config_sock = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        throw OSXEXCEPT("UDP IPv6 socket creation failed");
    }
    
    unique_ptr<BYTE[]> mac_address = getMacAddress(device_name);
}
//TapDevMac::TapDevMac(AsyncIoCompletion & iocmpl)
//{
//}
//
//
//TapDevMac::~TapDevMac()
//{
//}
}
}