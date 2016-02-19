//
// Created by michael on 07.12.15.
//

#pragma once


#include "WorldModelAction.h"

namespace weave {

    /*!
     * @ingroup worldApi
     */
    class WorldListener {
    public:
		virtual ~WorldListener() {}

        virtual void WorldChanged(std::vector<WorldModelAction> modelActions) = 0;
    };
}