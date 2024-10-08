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

#ifndef SEWENEW_REDISPROTOBUF_IMPORT_COMMAND_H
#define SEWENEW_REDISPROTOBUF_IMPORT_COMMAND_H

#include "module_api.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "utils.h"
#include "field_ref.h"

namespace sw {

namespace redis {

namespace pb {

// command: PB.IMPORT ADD [--REPLACE] file-path content
// return:  OK status reply.
// error:   If failing to import, return an error reply.
//
// command: PB.IMPORT DELETE file-path
// return:  OK status reply.
// error:   If failing to delete, return an error reply.
//
// command: PB.IMPORT LOAD
// return:  Array reply of the status of loaded proto files.
class ImportCommand {
public:
    int run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const;

private:
    struct Args {
        std::string filename;
        std::string content;

        enum class Cmd {
            ADD = 0,
            DELETE,
            LOAD,
            NONE
        };

        enum class Opt {
            REPLACE = 0,
            NONE
        };

        Cmd cmd = Cmd::NONE;
        Opt opt = Opt::NONE;
    };

    Args _parse_args(RedisModuleString **argv, int argc) const;

    // Return the position of the first non-option argument.
    int _parse_opts(RedisModuleString **argv, int argc, Args &args) const;
};

}

}

}

#endif // end SEWENEW_REDISPROTOBUF_IMPORT_COMMAND_H
