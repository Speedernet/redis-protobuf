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

#include "exists_test.h"
#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

namespace test {

void ExistsTest::_run(sw::redis::Redis &r) {
    auto key = test_key("exists");

    KeyDeleter deleter(r, key);

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg") == 0,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.SET", key, "Msg",
                R"({"i" : 1, "arr" : [1, 2], "m" : {"key" : "val"}})") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/i") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/arr") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/arr/0") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/m") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/m/key") == 1,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/j") == 0,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/arr/2") == 0,
            "failed to test pb.exists command");

    REDIS_ASSERT(r.command<long long>("PB.EXISTS", key, "Msg", "/m/val") == 0,
            "failed to test pb.exists command");
}

}

}

}

}
