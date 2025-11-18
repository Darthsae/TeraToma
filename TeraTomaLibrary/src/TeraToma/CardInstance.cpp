#include <TeraToma/CardInstance.h>
#include <TeraToma/Status.h>
#include <TeraToma/GameAPI.h>
#include <print>

namespace TeraToma {
    CardInstance::CardInstance() {
        names = std::vector<std::string>();
        statuses = std::vector<std::string>();
    }
    
    CardInstance::CardInstance(std::string a_cardType, size_t a_index) {
        names = std::vector<std::string>();
        names.emplace_back(a_cardType);
        statuses = std::vector<std::string>();
        index = a_index;
    }

    void CardInstance::Respond(GameAPI* a_gameAPI, std::string_view a_response) {
        if (onRespond) {
            onRespond(a_gameAPI, this, a_response);
        }
    }

    void CardInstance::ShowResponse(GameAPI* a_gameAPI) {
        if (onShowResponse) {
            onShowResponse(a_gameAPI, this);
        }
    }

    void CardInstance::HideResponse(GameAPI* a_gameAPI) {
        if (onHideResponse) {
            onHideResponse(a_gameAPI, this);
        }
    }

    void CardInstance::UpdateDisplay(GameAPI* a_gameAPI) {
        if (displayName.empty()) {
            displayName = names.back();
        }
    }

    void CardInstance::CanWinBase(GameAPI* a_gameAPI, const CardType& a_cardType) {
        canWin = a_cardType.canWin;

        for (size_t i = 0; i < statuses.size(); ++i) {
            const StatusType& statusType = a_gameAPI->statusTypes.at(statuses.at(i));
            if (statusType.canPreWinBase) {
                canWin = canWin && statusType.canPreWinBase(a_gameAPI, this, a_cardType);
            }
        }

        if (a_cardType.canWinBase) {
            canWin = a_cardType.canWinBase(a_gameAPI, index);
        }

        for (size_t i = 0; i < statuses.size(); ++i) {
            const StatusType& statusType = a_gameAPI->statusTypes.at(statuses.at(i));
            if (statusType.canPostWinBase) {
                canWin = canWin && statusType.canPostWinBase(a_gameAPI, this, a_cardType);
            }
        }
    }

    void CardInstance::CanWinPost(GameAPI* a_gameAPI, const CardType& a_cardType) {
        for (size_t i = 0; i < statuses.size(); ++i) {
            const StatusType& statusType = a_gameAPI->statusTypes.at(statuses.at(i));
            if (statusType.canPreWinPost) {
                canWin = canWin && statusType.canPreWinPost(a_gameAPI, this, a_cardType);
            }
        }

        if (a_cardType.canWinPost) {
            canWin = a_cardType.canWinPost(a_gameAPI, index);
        }

        for (size_t i = 0; i < statuses.size(); ++i) {
            const StatusType& statusType = a_gameAPI->statusTypes.at(statuses.at(i));
            if (statusType.canPostWinPost) {
                canWin = canWin && statusType.canPostWinPost(a_gameAPI, this, a_cardType);
            }
        }
    }

    CardInstance::~CardInstance() {
        onRespond = NULL;
    }
}