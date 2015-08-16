//
// Created by michael on 14.08.15.
//

#include "ExploreRegionTemplate.h"

Quest ExploreRegionTemplate::ToQuest() const {
    return Quest(Proposed, "Title", "Descr");
}

ExploreRegionTemplate::ExploreRegionTemplate(std::vector<std::string> titles,
                                             std::vector<TemplateQuestProperty> properties,
                                             std::vector<TemplateQuestDescription> descriptions)
        : Template(titles, properties, descriptions) {

}
