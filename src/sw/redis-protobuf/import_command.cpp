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

#include "import_command.h"
#include <cassert>
#include "utils.h"
#include "errors.h"
#include "redis_protobuf.h"

namespace sw {

namespace redis {

namespace pb {

int ImportCommand::run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    try {
        assert(ctx != nullptr);

        auto args = _parse_args(argv, argc);

        auto &m= RedisProtobuf::instance();
        m.proto_factory()->load(args.filename, args.content, args.opt == Args::Opt::REPLACE);

        RedisModule_ReplicateVerbatim(ctx);

        RedisModule_ReplyWithSimpleString(ctx, "OK");

        return REDISMODULE_OK;
    } catch (const WrongArityError &err) {
        return RedisModule_WrongArity(ctx);
    } catch (const Error &err) {
        return api::reply_with_error(ctx, err);
    }

    return REDISMODULE_ERR;
}

auto ImportCommand::_parse_args(RedisModuleString **argv, int argc) const -> Args {
    assert(argv != nullptr);

    if (argc < 3) {
        throw WrongArityError();
    }

    Args args;

    auto pos = _parse_opts(argv, argc, args);
    if (pos + 2 != argc) {
        throw WrongArityError();
    }

    args.filename = util::sv_to_string(argv[pos]);
    args.content = util::sv_to_string(argv[pos + 1]);

    return args;
}

int ImportCommand::_parse_opts(RedisModuleString **argv, int argc, Args &args) const {
    auto idx = 1;
    while (idx < argc) {
        auto opt = StringView(argv[idx]);

        if (util::str_case_equal(opt, "--REPLACE")) {
            if (args.opt != Args::Opt::NONE) {
                throw Error("syntax error");
            }

            args.opt = Args::Opt::REPLACE;
        } else {
            // Finish parsing options.
            break;
        }

        ++idx;
    }

    return idx;
}

}

}

}
