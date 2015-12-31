//
// Created by michael on 26.10.15.
//

#pragma once

#include <cereal.h>
#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"

namespace weave {

    class MetaData {
    public:
        bool HasValue(const std::string &name) const;

        int GetValue(const std::string &name) const;

        MetaData &SetValue(const std::string &name, int value);

        std::vector<std::string> GetValueNames() const;

    private:
        std::unordered_map<std::string, int> data;

        // serialization
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(CEREAL_NVP(data));
        }
    };
}
