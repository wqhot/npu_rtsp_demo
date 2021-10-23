#pragma once

#include <decode/live/liveMedia/include/liveMedia.hh>
#include <decode/live/BasicUsageEnvironment/include/BasicUsageEnvironment.hh>
#include <decode/live/liveMedia/include/H264VideoRTPSource.hh>
#include <decode.h>

int startRtspClientNew(Decode *decode1, const char *url);