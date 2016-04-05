//
// Created by michael on 05.04.16.
//

#pragma once

#include "../QuestModel/Quest.h"
#include "../Core/WeaverUtils.h"
#include "../Core/WeaverTypes.h"

namespace weave {
    /*!
     * Marks parts of a quest template that represent a WorldEntity.
     *
     * The quest system usually has to choose between multiple possible entities for each quest property.
     * Instances of this class allow the quest system to tell the template which entity should be used for which part
     * of a quest.
     *
     * @ingroup questApi
     */
    class TemplateQuestProperty {
    public:
        /*!
         * Creates a new quest property with the given name.
         *
         * @param isMandatory If true, then the quest template is unable to create a quest without this property.
         * Otherwise, this property is optional.
         * @param name The name of this quest property.
         */
        TemplateQuestProperty(bool isMandatory, const std::string &name) noexcept;

        /*!
         * Returns true if the quest template is unable to create a quest without this property.
         */
        bool IsMandatory() const noexcept;

        /*!
         * Returns the name of this quest property.
         */
        std::string GetName() const noexcept;

    private:
        bool isMandatory;
        std::string name;
    };


    /*!
     * Data container linking quest templates and world entities.
     *
     * Each quest template requires a number of world entities, which it describes with TemplateQuestProperty values.
     * Once the quest system has found a suitable WorldEntity for such a value, it creates a QuestPropertyValue object.
     */
    class QuestPropertyValue {
    public:
        /*!
         * Creates a new instance linking the given quest property and world entity.
         *
         * @param property the quest property this object describes
         * @param value the world entity to use for the quest property
         */
        QuestPropertyValue(const TemplateQuestProperty &property, std::shared_ptr<WorldEntity> value) noexcept;

        /*!
         * Returns the quest property value described by this object.
         */
        TemplateQuestProperty GetProperty() const noexcept;

        /*!
         * Returns the world entity to use for this object's quest property.
         */
        std::shared_ptr<WorldEntity> GetValue() const noexcept;

        /*!
         * Returns a formatted string version of the WorldEntity contained in this object.
         */
        std::string GetValueString(FormatterType format) const noexcept;

        /*!
         * Returns a formatted string version of the given WorldEntity.
         */
        static std::string GetValueString(std::shared_ptr<WorldEntity> value, bool isMandatory, FormatterType format);

    private:
        TemplateQuestProperty property;
        std::shared_ptr<WorldEntity> value;
    };
}