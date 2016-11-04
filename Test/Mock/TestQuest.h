//
// Created by michael on 10.10.15.
//

#pragma once

#include <QuestModel/Quest.h>

namespace weave {
    class TestQuest : public Quest {
    public:
        TestQuest(const std::string &title, const std::string &description);

        std::string GetType() const;

    private:
        TestQuest(ID id, const std::string &title, const std::string &description);

        friend class cereal::access;

        template<class Archive>
        static void load_and_construct(Archive &ar, cereal::construct<TestQuest> &construct) {
            ID id = 0;
            std::string title;
            std::string description;

            ar(id, title, description);
            construct(id, title, description);
        }
    };
}

CEREAL_REGISTER_TYPE(weave::TestQuest);
