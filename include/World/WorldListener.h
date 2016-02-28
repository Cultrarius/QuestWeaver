//
// Created by michael on 07.12.15.
//

#pragma once


#include "WorldModelAction.h"

namespace weave {

    /*!
     * Can be attached to a WorldModel to receive change updates.
     * @ingroup worldApi
     */
    class WorldListener {
    public:
        virtual ~WorldListener() = default;

        /*!
         * This method is called every time the world model changed.
         *
         * @param modelActions All the actions the world model executed.
         */
        virtual void WorldChanged(std::vector<WorldModelAction> modelActions) = 0;
    };
}