//
// Created by michael on 15.08.15.
//

#pragma once

#include "../Core/WeaverTypes.h"
#include "../Core/WeaverUtils.h"
#include "../json/json.h"
#include "QuestTemplate.h"

namespace weave {

    /*!
     * A factory that is able to create different "types" of quests (quest templates) on demand.
     *
     * Since all the quest templates are created by the user of the QuestWeaver library, they have to be created
     * somewhere. Subclasses of the QuestTemplateFactory can be registered with QuestWeaver instances which uses them
     * to generate new quests.
     *
     * The factory is not serialized along with the quest system and has to be re-registered on deserialization.
     *
     * @ingroup questApi
     */
    class QuestTemplateFactory {
    public:
        virtual ~QuestTemplateFactory() = default;

        /*!
         * Returns a list of all valid template keys, whereas each key represents a different kind of quest template.
         */
        virtual std::vector<std::string> GetTemplateKeys();

        /*!
         * Creates a new template with the given template key.
         * @see GetTemplateKeys()
         */
        std::shared_ptr<QuestTemplate> CreateTemplate(const std::string &templateKey);

    protected:
        /*!
         * Creates a new quest template from the given JSON parameters.
         */
        virtual std::shared_ptr<QuestTemplate> createFromJsonValues(const Json::Value &root) const = 0;

        virtual std::vector<const char *> getTemplateFiles() const = 0;

        /*!
         * Helper function to extract quest properties from a JSON object.
         */
        static std::vector<TemplateQuestProperty> extractProperties(const Json::Value &root);

        /*!
         * Helper function to extract quest descriptions from a JSON object.
         */
        static std::vector<TemplateQuestDescription> extractDescriptions(const Json::Value &root);

        /*!
         * Helper function to extract a random quest title from a JSON object.
         */
        static std::string extractTitle(const Json::Value &root, std::shared_ptr<RandomStream> randomStream);

        /*!
         * The random stream instance to use for any random choices regarding the quest template.
         */
        std::shared_ptr<RandomStream> randomStream;

        /*!
         * The format to use when creating texts for the quest.
         */
        FormatterType formatterType;

    private:
        friend class TemplateEngine;

        bool isInitialized = false;

        std::unordered_map<std::string, Json::Value> templateMap;

        Directories dirs;

        void initialize();

        static void extractProperties(std::vector<TemplateQuestProperty> *properties,
                                      const Json::Value &jsonMandatory,
                                      bool isMandatory);
    };
}
