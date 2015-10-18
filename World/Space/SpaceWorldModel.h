//
// Created by michael on 06.10.15.
//

#pragma once

#include "../WorldModel.h"
#include "SpaceLocation.h"
#include "../../Core/WeaverUtils.h"
#include "SpaceAgent.h"

namespace weave {
    struct ModelParameters {
        int minLocation = 0;
        int maxLocation = 1000;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(CEREAL_NVP(minLocation), CEREAL_NVP(maxLocation));
        }
    };

    class SpaceWorldModel : public WorldModel {
    public:
        explicit SpaceWorldModel(std::shared_ptr<RandomStream> randomStream);

        ModelParameters GetParameters();

        void SetParameters(ModelParameters parameters);

        // entity creation methods

        std::shared_ptr<SpaceLocation> CreateLocation() const;

        std::shared_ptr<SpaceAgent> CreateAgent() const;

    private:
        ModelParameters param;

        friend class cereal::access;

        template<class Archive>
        void serialize(Archive &ar) {
            ar(make_nvp("base", cereal::base_class<WorldModel>(this)), CEREAL_NVP(param));
        }
    };
}
