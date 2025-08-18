#include <TeraToma/Status.h>

namespace TeraToma {
    StatusType::StatusType(std::string a_name, std::string a_description) {
        name = a_name;
        description = a_description;
        tags = std::vector<std::string>();
    }

    StatusType::StatusType(std::string a_name, std::string a_description, const std::vector<std::string>& a_tags) {
        name = a_name;
        description = a_description;
        tags = std::vector<std::string>(a_tags);
    }

    StatusType::~StatusType() {
        canApplyToCard = NULL;
        applyToCard = NULL;
        canFlip = NULL;
        onPreFlip = NULL;
        onPostFlip = NULL;
        canKill = NULL;
        onPreKill = NULL;
        onPostKill = NULL;
        canActivate = NULL;
        onPreActivate = NULL;
        onPostActivate = NULL;
        canPreWinBase = NULL;
        canPostWinBase = NULL;
        canPreWinPost = NULL;
        canPostWinPost = NULL;
        lies = NULL;
    }
}