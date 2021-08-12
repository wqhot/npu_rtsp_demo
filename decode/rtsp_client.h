#pragma once

#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <H264VideoRTPSource.hh>
#include <decode.h>

int startRtspClientNew(Decode *decode1, const char *url);