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

#ifndef SEWENEW_REDISPROTOBUF_TEST_EXISTS_TEST_H
#define SEWENEW_REDISPROTOBUF_TEST_EXISTS_TEST_H

#include "proto_test.h"

namespace sw {

namespace redis {

namespace pb {

namespace test {

class ExistsTest : public ProtoTest {
public:
    explicit ExistsTest(sw::redis::Redis &r) : ProtoTest("PB.EXISTS", r) {}

private:
    virtual void _run(sw::redis::Redis &r) override;
};

}

}

}

}

#endif // end SEWENEW_REDISPROTOBUF_TEST_EXISTS_TEST_H
