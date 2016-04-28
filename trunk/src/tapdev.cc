#include "tapdev.h"

namespace tincan {
using namespace std;

TapDev::TapDev(
  const string & tap_name,
  unique_ptr<AsyncRead>async_rd,
  unique_ptr<AsyncWrite> async_wr_) :
#if defined(_IPOP_WIN)
  TapDevWin(tap_name, move(async_rd), move(async_wr_))
#elif defined(_IPOP_LINUX)
  TapDevLnx(async_rd), std::move(async_wr_))
#elif defined(_IPOP_OSX)
  TapDevMac(async_rd), std::move(async_wr_))
#endif
{}


TapDev::~TapDev()
{}

}
