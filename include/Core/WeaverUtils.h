//
// Created by michael on 11.10.15.
//

#pragma once

#include <json/json.h>
#include "WeaverTypes.h"


namespace weave {

    /*!
     * Replaces the first occurrence of a given string with another string.
     *
     * @param str The string that will be modified.
     * @param from This string will be searched in the "str" parameter.
     * @param to This string will be used to replace the "from" parameter.
     * @ingroup internal
     * @returns true if the string was found, false otherwise
     */
    bool replace(std::string *str, const std::string &from, const std::string &to) noexcept;

    /*!
     * Replaces all occurrences of a given string with another string.
     *
     * @param str The string that will be modified.
     * @param from This string will be searched in the "str" parameter.
     * @param to This string will be used to replace the "from" parameter.
     * @ingroup internal
     */
    void replaceAll(std::string *str, const std::string &from, const std::string &to) noexcept;

    /*!
     * Encloses a given string with an HTML tag and adds the given classes as "class" attribute.
     *
     * ## Example
     *
     * ```cpp
     * htmlEncloseWithTag("foo", "bar", ["myClass", "yourClass"])
     * // Result: <bar class="myClass yourClass">foo</bar>
     * ```
     *
     * @param str The string to enclose with the tag
     * @param tag The HTML tag to use
     * @param classes A list of CSS classes for the new tag
     * @ingroup internal
     */
    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag,
                                   const std::vector<std::string> &classes) noexcept;

    /*!
     * Encloses a given string with an HTML tag.
     *
     * ## Example
     *
     * ```cpp
     * htmlEncloseWithTag("foo", "bar")
     * // Result: <bar>foo</bar>
     * ```
     *
     * @param str The string to enclose with the tag
     * @param tag The HTML tag to use
     * @ingroup internal
     */
    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag) noexcept;

    /*!
     * Encloses a given string with an HTML tag and adds the given CSS class.
     *
     * ## Example
     *
     * ```cpp
     * htmlEncloseWithTag("foo", "bar", "myClass")
     * // Result: <bar class="myClass">foo</bar>
     * ```
     *
     * @param str The string to enclose with the tag
     * @param tag The HTML tag to use
     * @param class The CSS class for the new tag
     * @ingroup internal
     */
    std::string htmlEncloseWithTag(const std::string &str, const std::string &tag,
                                   const std::string &cssClass) noexcept;

    /*!
     * Reads and parses the given file as JSON.
     * The file is searched for (in this order) in the following directories: mod directory, template directory,
     * current working directory.
     * If the file is not found then a ContractFailedException is thrown.
     * Other exceptions can be thrown when a parsing error or file handling error occurs.
     *
     * @param fileName the fileName of the file to open.
     * @param dirs the directories to search the file in.
     * @ingroup internal
     */
    Json::Value readJsonFromFile(const char *fileName, const Directories &dirs);

    std::vector<TextToken> tokenizeText(std::string rawText) noexcept;
}
