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

#include "import_test.h"
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

namespace test {

void ImportTest::_run(sw::redis::Redis &r) {
    auto key = test_key("import");

    KeyDeleter deleter(r, key);

    std::string name{"test_import.proto"};
    auto proto1 = R"(
syntax = "proto3";
package sw.redis.pb;
message Msg {
    int32 i = 1;
    string s = 2;
}
    )";

    auto proto2 = R"(
syntax = "proto3";
package sw.redis.pb;
message Msg {
    bool b = 1;
    string s = 2;
}
    )";

    REDIS_ASSERT(r.command<std::string>("PB.IMPORT", "ADD", name, proto1) == "OK",
        "failed to test pb.import command");

    auto res1 = r.command<std::unordered_map<std::string, std::string>>("PB.IMPORT", "RELOAD");
    REDIS_ASSERT(res.size() == 1 && res[name] == "OK",
            "failed to test pb.import command");

    REDIS_ASSERT(r.command<long long>("PB.SET", key, "sw.redis.pb.Msg", "/i", 123) &&
            r.command<long long>("PB.GET", key, "sw.redis.pb.Msg", "/i") == 123,
        "failed to test pb.import command");

    REDIS_ASSERT(r.command<std::string>("PB.IMPORT", "ADD", "--RELOAD", name, proto2) == "OK",
        "failed to test pb.import command");

    REDIS_ASSERT(r.command<std::string>("PB.IMPORT", "RELOAD") == "OK",
        "failed to test pb.import command");

    auto res1 = r.command<std::unordered_map<std::string, std::string>>("PB.IMPORT", "RELOAD");
    REDIS_ASSERT(res.size() == 1 && res[name] == "OK",
            "failed to test pb.import command");

    REDIS_ASSERT(r.command<long long>("PB.SET", key, "sw.redis.pb.Msg", "/b", true) &&
            r.command<long long>("PB.GET", key, "sw.redis.pb.Msg", "/b") == 1,
        "failed to test pb.import command");

    REDIS_ASSERT(r.command<long long>("PB.DEL", key, "sw.redis.pb.Msg") == 1,
        "failed to test pb.import command");

    REDIS_ASSERT(r.command<std::string>("PB.IMPORT", "DELETE", name) == "OK",
        "failed to test pb.import command");

    auto res1 = r.command<std::unordered_map<std::string, std::string>>("PB.IMPORT", "RELOAD");
    REDIS_ASSERT(res.size() == 0
            "failed to test pb.import command");

    REDIS_ASSERT(r.command<sw::redis::OptionalString>("PB.SCHEMA",
        "failed to test pb.import command");
}

}

}

}

}
