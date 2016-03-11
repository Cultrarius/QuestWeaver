//
// Created by michael on 29.01.16.
//

#pragma once

#include <Core/WeaverTypes.h>
#include <Core/Graph/WeaverGraph.h>
#include <World/WorldEntity.h>
#include <World/WorldModelAction.h>
#include <World/WorldModel.h>

namespace weave {

    typedef std::unordered_map<std::string, std::vector<std::shared_ptr<WorldEntity>>> EntityMap;

    /*!
     * @ingroup storyApi
     */
    struct RawStoryToken {
        bool isMandatory;
        std::string text;
        std::vector<std::string> nuggetOptions;
        std::string id;
    };

    typedef std::vector<std::pair<RawStoryToken, std::vector<ID>>> TokenMapping;
    typedef std::unordered_map<std::string, std::vector<ID>> TokenToEntityMap;

    std::vector<RawStoryToken> getStoryTokens(std::string rawStoryText) noexcept;

    /*!
     * @ingroup storyApi
     */
    class NuggetOption {
    public:
        NuggetOption(std::string nuggetKey, std::vector<ID> entityIDs) noexcept :
                nuggetKey(nuggetKey), entityIDs(entityIDs) { }

        std::string GetNuggetKey() const noexcept {
            return nuggetKey;
        }

        std::vector<ID> GetEntityIDs() const noexcept {
            return entityIDs;
        }


    private:
        std::string nuggetKey;
        std::vector<ID> entityIDs;
    };

    /*!
     * @ingroup storyApi
     */
    struct StoryTemplateResult {
        std::string rawText;
        TokenMapping tokenMap;
        std::vector<WorldModelAction> worldActions;
    };

    /*!
     * @ingroup storyApi
     */
    class StoryTemplate {
    public:
        StoryTemplate(std::string rawText, std::set<std::string> requiredEntities) noexcept;

        virtual ~StoryTemplate() = default;

        std::set<std::string> GetRequiredEntities() const noexcept;

        virtual bool IsValid(const EntityMap &requiredEntities, const WeaverGraph &graph,
                             const WorldModel &worldModel) const;

        virtual StoryTemplateResult CreateStory(const EntityMap &requiredEntities, const WeaverGraph &graph,
                                                const WorldModel &worldModel) const = 0;

    protected:
        std::string rawText;
        std::set<std::string> requiredEntities;

        TokenMapping createTokenMapping(const TokenToEntityMap &idsPerToken) const;
    };
}
