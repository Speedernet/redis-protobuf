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

#include "proto_factory.h"
#include <fstream>
#include <google/protobuf/util/json_util.h>
#include "utils.h"
#include "errors.h"

namespace sw {

namespace redis {

namespace pb {

void FactoryErrorCollector::_add_error(const std::string &type,
                                        const std::string &filename,
                                        int line,
                                        int column,
                                        const std::string &message) {
    auto err = type + ":" + filename + ":"
                + std::to_string(line) + ":"
                + std::to_string(column) + ":"
                + message;

    _errors.push_back(std::move(err));
}

std::string FactoryErrorCollector::last_errors() const {
    std::string err_str;
    for (const auto &err : _errors) {
        if (!err_str.empty()) {
            err_str += "\n";
        }

        err_str += err;
    }

    return err_str;
}

ProtoFactory::ProtoFactory(const std::string &proto_dir) :
                            _proto_dir(_canonicalize_path(proto_dir)),
                            _importer(&_source_tree, &_error_collector) {
    _source_tree.MapPath("", _proto_dir);

    _load_protos(_proto_dir);
}

MsgUPtr ProtoFactory::create(const std::string &type) {
    const auto *desc = descriptor(type);
    if (desc == nullptr) {
        throw Error("unknown protobuf type: " + type);
    }

    const auto *prototype = _factory.GetPrototype(desc);

    assert(prototype != nullptr);

    return MsgUPtr(prototype->New());
}

MsgUPtr ProtoFactory::create(const std::string &type, const StringView &sv) {
    auto msg = create(type);

    const auto *ptr = sv.data();
    auto len = sv.size();
    if (len >= 2 && ptr[0] == '{' && ptr[len - 1] == '}') {
        auto status = gp::util::JsonStringToMessage(gp::StringPiece(ptr, len), msg.get());
        if (!status.ok()) {
            throw Error("failed to parse json to " + type + ": " + status.ToString());
        }
    } else {
        if (!msg->ParseFromArray(ptr, len)) {
            throw Error("failed to parse binary to " + type);
        }
    }

    return msg;
}

const gp::Descriptor* ProtoFactory::descriptor(const std::string &type) {
    auto iter = _descriptor_cache.find(type);
    if (iter != _descriptor_cache.end()) {
        return iter->second;
    }

    const auto *desc = _importer.pool()->FindMessageTypeByName(type);
    if (desc != nullptr) {
        _descriptor_cache.emplace(type, desc);
    }

    return desc;
}

void ProtoFactory::add_proto(const std::string &filename, const std::string &content, bool replace) {
    if (!replace) {
        auto iter = _loaded_files.find(filename);
        if (iter != _loaded_files.end()) {
            throw Error("already imported");
        }
    }

    auto path = _absolute_path(filename);

    io::create_dirs(io::dirname(path), 0);
    std::ofstream file(path);
    if (!file) {
        throw Error("failed to open proto file for writing: " + path);
    }

    _loaded_files.insert(filename);
    file << content;
}

void ProtoFactory::delete_proto(const std::string &filename) {
    auto iter = _loaded_files.find(filename);
    if (iter == _loaded_files.end()) {
        throw Error("unknown import");
    }

    io::remove_file(_absolute_path(filename));
    _loaded_files.erase(iter);
}

std::unordered_map<std::string, std::string> ProtoFactory::load_protos() {
    return _load_protos(_proto_dir);
}

std::unordered_map<std::string, std::string> ProtoFactory::_load_protos(const std::string &proto_dir) {
    std::unordered_map<std::string, std::string> results;
    auto files = io::list_dir(proto_dir);

    for (const auto &file : files) {
        if (!io::is_regular(file) || io::extension(file) != "proto") {
            continue;
        }

        auto prefix_size = proto_dir.size() + 1;
        if (file.size() < prefix_size) {
            continue;
        }

        auto name = file.substr(prefix_size);
        try {
            _load(name);
            results[name] = "OK";
        } catch (const Error &err) {
            results[name] = std::string("ERR ") + err.what();
        }
    }

    return results;
}

void ProtoFactory::_load(const std::string &file) {
    // Clear last errors.
    _error_collector.clear();

    auto *desc = _importer.Import(file);
    if (desc == nullptr || _error_collector.has_error()) {
        throw Error("failed to load " + file + "\n" + _error_collector.last_errors());
    }
}

std::string ProtoFactory::_canonicalize_path(std::string proto_dir) const {
    // Remove trailing '/'
    while (!proto_dir.empty() && proto_dir.back() == '/') {
        proto_dir.resize(proto_dir.size() - 1);
    }

    if (proto_dir.empty()) {
        throw Error("invalid proto dir: " + proto_dir);
    }

    return proto_dir;
}

std::string ProtoFactory::_absolute_path(const std::string &path) const {
    return _proto_dir + "/" + path;
}

}

}

}
