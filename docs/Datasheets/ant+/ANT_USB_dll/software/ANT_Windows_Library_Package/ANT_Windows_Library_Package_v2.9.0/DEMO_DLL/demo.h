/*
 * Dynastream Innovations Inc.
 * Cochrane, AB, CANADA
 *
 * Copyright © 1998-2009 Dynastream Innovations Inc.
 * All rights reserved. This software may not be reproduced by
 * any means without express written approval of Dynastream
 * Innovations Inc.
 */ 

#ifndef TEST_H
#define TEST_H

#include "types.h"

#define CHANNEL_TYPE_MASTER   (0)
#define CHANNEL_TYPE_SLAVE    (1)
#define CHANNEL_TYPE_INVALID  (2)

void Test_Start();
void Test_Init(UCHAR ucDeviceNumber_, UCHAR ucChannelType_);

#endif //TEST_H
