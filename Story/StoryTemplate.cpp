//
// Created by michael on 29.01.16.
//

#include <Story/StoryTemplate.h>
#include <regex>

using namespace weave;
using namespace std;

StoryTemplate::StoryTemplate(string rawText, set<string> requiredEntities) noexcept :
        rawText(rawText), requiredEntities(requiredEntities) {
}

bool StoryTemplate::IsValid(const EntityMap &, const WeaverGraph &, const WorldModel &) const {
    return true;
}

std::set<std::string> StoryTemplate::GetRequiredEntities() const noexcept {
    return requiredEntities;
}

TokenMapping StoryTemplate::createTokenMapping(const TokenToEntityMap &idsPerToken) const {
    TokenMapping result;
    for (auto token : getStoryTokens(rawText)) {
        auto it = idsPerToken.find(token.id);
        if (it != idsPerToken.end()) {
            result.push_back(make_pair(token, it->second));
        } else {
            result.push_back(make_pair(token, vector<ID>()));
        }
    }
    return result;
}

vector<RawStoryToken> weave::getStoryTokens(string rawStoryText) noexcept {
    // Prepare tokenizer regexes
    static string id = "[a-zA-Z0-9]+";  // valid identifier
    static string tokenContent = "(" + id + "(?:\\|" + id + ")*)";  // content without attached ID
    static string tokenContentId = "(" + id + "(?:\\|" + id + ")*):(" + id + ")";  // content with ID
    static string optionalToken = "\\{(" + tokenContent + "|" + tokenContentId + ")\\}";
    static string mandatoryToken = "\\[(" + tokenContent + "|" + tokenContentId + ")\\]";

    static regex tokenRegex("(" + optionalToken + "|" + mandatoryToken + ")");
    static regex mandatoryRegex(mandatoryToken);
    static regex optionalRegex(optionalToken);
    static regex contentIdRegex(tokenContentId);
    static regex idRegex(id);
    static sregex_token_iterator iteratorEnd;

    // Tokenize input
    vector<RawStoryToken> tokens;
    sregex_token_iterator tokenIt(rawStoryText.begin(), rawStoryText.end(), tokenRegex);
    while (tokenIt != iteratorEnd) {
        string text = (*tokenIt).str();
        string id;
        vector<string> nuggetOptions;

        smatch matchResult;
        bool isMandatory = regex_match(text, matchResult, mandatoryRegex);
        if (!isMandatory) {
            regex_match(text, matchResult, optionalRegex);
        }
        string content = matchResult[1].str();
        if (regex_match(content, matchResult, contentIdRegex)) {
            id = matchResult[2].str();
            content = matchResult[1].str();  // this line invalidates the matchResult, do not access afterwards!
        }

        sregex_token_iterator optionIt(content.begin(), content.end(), idRegex);
        copy(optionIt, iteratorEnd, back_inserter(nuggetOptions));

        tokens.push_back({isMandatory, text, nuggetOptions, id});
        tokenIt++;
    }
    return tokens;
}
