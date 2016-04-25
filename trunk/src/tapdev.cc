#include "tapdev.h"

namespace tincan {

TapDev::TapDev(
  unique_ptr<AsyncRead>async_rd,
  unique_ptr<AsyncWrite> async_wr_) :
#if defined(_IPOP_WIN)
  TapDevWin(std::move(async_rd), std::move(async_wr_))
#elif defined(LINUX)
  TapDevLnx(async_rd), std::move(async_wr_))
#elif defined(OSX)
  TapDevOsx(std::move(async_rd), std::move(async_wr_))
#endif
{}


TapDev::~TapDev()
{}

}
