/**************************************************************************
   Copyright (c) 2019 sewenew

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

#include "exists_command.h"
#include "errors.h"
#include "redis_protobuf.h"
#include "utils.h"
#include "field_ref.h"

namespace sw {

namespace redis {

namespace pb {

int ExistsCommand::run(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) const {
    try {
        assert(ctx != nullptr);

        auto args = _parse_args(argv, argc);

        auto key = api::open_key(ctx, args.key_name, api::KeyMode::READONLY);
        if (!api::key_exists(key.get(), RedisProtobuf::instance().type())) {
            RedisModule_ReplyWithLongLong(ctx, 0);
        } else {
            auto *msg = api::get_msg_by_key(key.get());
            assert(msg != nullptr);

            const auto &path = args.path;
            if (msg->GetTypeName() != path.type()) {
                throw Error("type mismatch");
            }

            if (!path.empty()) {
                ConstFieldRef field(msg, path);
                assert(field != nullptr);
            }

            RedisModule_ReplyWithLongLong(ctx, 1);
        }

    } catch (const WrongArityError &err) {
        return RedisModule_WrongArity(ctx);
    } catch (const Error &err) {
        RedisModule_ReplyWithLongLong(ctx, 0);
    }

    return REDISMODULE_OK;
}

ExistsCommand::Args ExistsCommand::_parse_args(RedisModuleString **argv, int argc) const {
    assert(argv != nullptr);

    if (argc != 3 && argc != 4) {
        throw WrongArityError();
    }

    Path path;
    if (argc == 3) {
        path = Path(argv[2]);
    } else {
        path = Path(argv[2], argv[3]);
    }

    return {argv[1], std::move(path)};
}

}

}

}
