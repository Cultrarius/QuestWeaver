//
// Created by michael on 11.10.15.
//

#include <Core/WeaverUtils.h>
#include <fstream>

using namespace std;
using namespace Json;

void ::weave::replaceAll(std::string *str, const std::string &from, const std::string &to) {
    if (from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str->find(from, start_pos)) != std::string::npos) {
        str->replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

bool ::weave::replace(std::string *str, const std::string &from, const std::string &to) {
    size_t start_pos = str->find(from);
    if (start_pos == std::string::npos) {
        return false;
    }
    str->replace(start_pos, from.length(), to);
    return true;
}

Json::Value weave::readJsonFromFile(const char *fileName, const Directories &dirs) {
    Value root;
    CharReaderBuilder readBuilder;
    readBuilder["collectComments"] = false;
    readBuilder["rejectDupKeys"] = true;
    string errorMessage;
    ifstream inStream;

    string modDir(dirs.modDirectory);
    string dataDir(dirs.templateDirectory);

    // try to use the mods directory
    const char *moddedFile = modDir.append(fileName).c_str();
    inStream.open(moddedFile, ios::in);
    if (inStream.fail()) {
        // try to use the regular directory
        const char *templateFile = dataDir.append(fileName).c_str();
        inStream.open(templateFile, ios::in);
        if (inStream.fail()) {
            // try to open the file as it is
            string currentDir("./");
            const char *localFile = currentDir.append(fileName).c_str();
            inStream.open(localFile, ios::in);
            if (inStream.fail()) {
                std::string errorMsg =
                        string("Unable to find file in any of the following directories: [., ") +
                        dirs.templateDirectory +
                        ", " + dirs.modDirectory + "]";
                throw ContractFailedException(errorMsg);
            }
        }
    }

    if (!Json::parseFromStream(readBuilder, inStream, &root, &errorMessage)) {
        cerr << "Error parsing template file: " << errorMessage << endl;
        throw ContractFailedException(errorMessage);
    }

    return root;
}
