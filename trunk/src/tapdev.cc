#include "tapdev.h"

namespace tincan {

TapDev::TapDev(AsyncIoCompletion & iocmpl) :
#if defined(WINDOWS)
  TapDevWin(iocmpl)
#elif defined(LINUX)
  TapDevLnx(iocmpl)
#elif defined(OSX)
  TapDevMac(iocmpl)
#endif
{}

TapDev::~TapDev()
{}

}