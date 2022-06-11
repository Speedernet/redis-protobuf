/**************************************************************************
   Copyright (c) 2022 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_REDISPROTOBUF_TEST_LEN_TEST_H
#define SEWENEW_REDISPROTOBUF_TEST_LEN_TEST_H

#include <sw/redis++/redis++.h>

namespace sw {

namespace redis {

namespace pb {

namespace test {

class LenTest {
public:
    explicit LenTest(sw::redis::Redis &redis) : _redis(redis) {}

    void run();

private:
    sw::redis::Redis &_redis;
};

}

}

}

}

#endif // end SEWENEW_REDISPROTOBUF_TEST_LEN_TEST_H
