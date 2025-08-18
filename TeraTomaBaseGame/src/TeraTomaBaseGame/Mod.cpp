#include <TeraTomaBaseGame/Mod.h>
#include <print>
#include <algorithm>
#include <Random.h>

using TeraToma::CardInstance;
using TeraToma::CardType;
using TeraToma::CardAllegiance;
using TeraToma::StatusType;
using TeraToma::AllegianceStrings;

size_t i = 0;

extern "C" void MOD_API ModLoad(TeraToma::GameAPI* const a_gameAPI) {
    std::println("We have entered the Base Game mod loading");
    StatusType* corruptionStatus = a_gameAPI->LoadStatus(MOD_NAME,
        "Corruption", "The character will lie.");
    corruptionStatus->canApplyToCard = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return true;
    };
    corruptionStatus->applyToCard = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->hand.cards[a_index].lies = true;
    };

    CardType* doctorCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Doctor", "The Doctor will cure all characters of corruption one space away. Killing a doctor does 2 damage.", 
        CardAllegiance::VILLAGE, true);
    doctorCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(2);
    };
    doctorCard->onPostResolve = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        int cured = 0;
        if (a_gameAPI->hand.cards[a_index].lies) {
            cured = RandomUniformUInt32(0, 2);
        } else {
            int left = ((a_gameAPI->hand.cardCount + (int)a_index - 1) % (int)a_gameAPI->hand.cardCount);
            int right = (((int)a_index + 1) % (int)a_gameAPI->hand.cardCount);

            //std::println("Left {} Right {}", left, right);
            //std::println("Left {} Right {}", a_gameAPI->hand.cards[left].statuses.size(), a_gameAPI->hand.cards[right].statuses.size());
            //std::println("Left {} Right {}", a_gameAPI->hand.cards[left].names.back(), a_gameAPI->hand.cards[right].names.back());
            //std::println("Left {} Right {}", AllegianceStrings.at(a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[left].names.back()).allegiance), AllegianceStrings.at(a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[right].names.back()).allegiance));

            if (a_gameAPI->hand.cards[left].statuses.size() != 0 && std::find(a_gameAPI->hand.cards[left].statuses.begin(), a_gameAPI->hand.cards[left].statuses.end(), "Corruption") != a_gameAPI->hand.cards[left].statuses.end()) {
                a_gameAPI->hand.cards[left].statuses.erase(std::find(a_gameAPI->hand.cards[left].statuses.begin(), a_gameAPI->hand.cards[left].statuses.end(), "Corruption"));
                ++cured;
            }

            if (a_gameAPI->hand.cards[right].statuses.size() != 0 && std::find(a_gameAPI->hand.cards[right].statuses.begin(), a_gameAPI->hand.cards[right].statuses.end(), "Corruption") != a_gameAPI->hand.cards[right].statuses.end()) {
                a_gameAPI->hand.cards[right].statuses.erase(std::find(a_gameAPI->hand.cards[right].statuses.begin(), a_gameAPI->hand.cards[right].statuses.end(), "Corruption"));
                ++cured;
            }
        }

        a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, std::format("I cured {} people.", cured));
    };

    CardType* healerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Healer", "The Healer will allow you to use one flipped dead card. Killing a Healer does 4 damage.", 
        CardAllegiance::VILLAGE, true);
    healerCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        for (size_t i = 0; i < a_gameAPI->hand.cardCount; ++i) {
            if (i != a_index && a_gameAPI->hand.cards[i].dead && !a_gameAPI->hand.cards[i].activated) {
                return true;
            }
        }
        return false;
    };
    healerCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->maxSelect = 1;
        a_gameAPI->selecting = a_index;
        a_gameAPI->gameMouseState = TeraToma::GameMouseState::SELECTING;
        a_gameAPI->validCardSelectionFnptr = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            return a_gameAPI->hand.cards[a_index].dead && !a_gameAPI->hand.cards[a_index].activated;
        };
        a_gameAPI->selectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            //std::println("Pre i {}", i);
            i = a_gameAPI->selected[0];
            //std::println("Set i {}", i);
            a_gameAPI->hand.cards[i].activated = true;
        };
        a_gameAPI->postSelectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            //std::println("Post i {}", i);
            if (a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[i].names.back()).onActivate) {
                a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[i].names.back()).onActivate(a_gameAPI, i);
            }
        };
    };
    healerCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(4);
    };

    CardType* knightCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Knight", "The Knight can not die. Killing a Knight does 2 damage.", 
        CardAllegiance::VILLAGE, true);
    knightCard->canKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return !a_gameAPI->hand.cards[a_index].lies;
    };
    knightCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(2);
    };

    CardType* oracleCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Oracle", "The Oracle will tell you the number of each type of allegiance across three other cards. Killing an Oracle does 4 damage.", 
        CardAllegiance::DIVINES, true);
    oracleCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(4);
    };
    oracleCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        for (size_t i = 0; i < a_gameAPI->hand.cardCount; ++i) {
            if (i != a_index) {
                return true;
            }
        }
        return false;
    };
    oracleCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->maxSelect = 3;
        a_gameAPI->selecting = a_index;
        a_gameAPI->gameMouseState = TeraToma::GameMouseState::SELECTING;
        a_gameAPI->validCardSelectionFnptr = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            return true;
        };
        a_gameAPI->selectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            uint32_t villageCount = 0;
            uint32_t neutralCount = 0;
            uint32_t outcastCount = 0;
            uint32_t outsideCount = 0;
            uint32_t undeathCount = 0;
            uint32_t monsterCount = 0;
            uint32_t demonicCount = 0;
            uint32_t divinesCount = 0;
            if (a_gameAPI->hand.cards[a_index].lies) {
                for (size_t index : a_gameAPI->selected) {
                    switch (CardAllegiance(RandomUniformUInt32(0, (uint32_t)CardAllegiance::SIZE_OF - 1))) {
                        case CardAllegiance::VILLAGE:
                            ++villageCount;
                            break;
                        case CardAllegiance::NEUTRAL:
                            ++neutralCount;
                            break;
                        case CardAllegiance::OUTCAST:
                            ++outcastCount;
                            break;
                        case CardAllegiance::OUTSIDE:
                            ++outsideCount;
                            break;
                        case CardAllegiance::UNDEATH:
                            ++undeathCount;
                            break;
                        case CardAllegiance::MONSTER:
                            ++monsterCount;
                            break;
                        case CardAllegiance::DEMONIC:
                            ++demonicCount;
                            break;
                        case CardAllegiance::DIVINES:
                            ++divinesCount;
                            break;
                    }
                }
            } else {
                for (size_t index : a_gameAPI->selected) {
                    switch (a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[index].names.back()).allegiance) {
                        case CardAllegiance::VILLAGE:
                            ++villageCount;
                            break;
                        case CardAllegiance::NEUTRAL:
                            ++neutralCount;
                            break;
                        case CardAllegiance::OUTCAST:
                            ++outcastCount;
                            break;
                        case CardAllegiance::OUTSIDE:
                            ++outsideCount;
                            break;
                        case CardAllegiance::UNDEATH:
                            ++undeathCount;
                            break;
                        case CardAllegiance::MONSTER:
                            ++monsterCount;
                            break;
                        case CardAllegiance::DEMONIC:
                            ++demonicCount;
                            break;
                        case CardAllegiance::DIVINES:
                            ++divinesCount;
                            break;
                    }
                }
            }

            std::sort(a_gameAPI->selected.begin(), a_gameAPI->selected.end());

            std::string cat = std::format("Among #{}, #{}, and #{} there is:", a_gameAPI->selected[0], a_gameAPI->selected[1], a_gameAPI->selected[2]);
            if (villageCount > 0) {
                cat += std::format(" {} {}", villageCount, villageCount > 1 ? "villagers" : "villager");
            }
            if (neutralCount > 0) {
                cat += std::format(" {} {}", neutralCount, neutralCount > 1 ? "neutrals" : "neutral");
            }
            if (outcastCount > 0) {
                cat += std::format(" {} {}", outcastCount, outcastCount > 1 ? "outcasts" : "outcast");
            }
            if (outsideCount > 0) {
                cat += std::format(" {} {}", outsideCount, outsideCount > 1 ? "outsiders" : "outsider");
            }
            if (undeathCount > 0) {
                cat += std::format(" {} undead", undeathCount);
            }
            if (monsterCount > 0) {
                cat += std::format(" {} {}", monsterCount, monsterCount > 1 ? "monsters" : "monster");
            }
            if (demonicCount > 0) {
                cat += std::format(" {} {}", demonicCount, demonicCount > 1 ? "demons" : "demon");
            }
            if (divinesCount > 0) {
                cat += std::format(" {} divine", divinesCount);
            }
            a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, cat);
        };
    };
    oracleCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(4);
    };

    CardType* peasantCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Peasant", "The Peasant will do nothing. Killing a Peasant does 2 damage.", 
        CardAllegiance::VILLAGE, true);
    peasantCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(2);
    };
    peasantCard->onFlip = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->blindness = 750;
    };

    CardType* plagueBringerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Plague Bringer", "The Plague Bringer will corrupt all non Demonic characters two spaces away.", 
        CardAllegiance::DEMONIC, false);
    plagueBringerCard->onPostResolve = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        int left = ((a_gameAPI->hand.cardCount + (int)a_index - 2) % (int)a_gameAPI->hand.cardCount);
        int right = (((int)a_index + 2) % (int)a_gameAPI->hand.cardCount);

        std::println("Left {} Right {}", left, right);
        std::println("Left {} Right {}", a_gameAPI->hand.cards[left].statuses.size(), a_gameAPI->hand.cards[right].statuses.size());
        std::println("Left {} Right {}", a_gameAPI->hand.cards[left].names.back(), a_gameAPI->hand.cards[right].names.back());
        std::println("Left {} Right {}", AllegianceStrings.at(a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[left].names.back()).allegiance), AllegianceStrings.at(a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[right].names.back()).allegiance));

        if (a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[left].names.back()).allegiance != CardAllegiance::DEMONIC && (a_gameAPI->hand.cards[left].statuses.size() == 0 || std::find(a_gameAPI->hand.cards[left].statuses.begin(), a_gameAPI->hand.cards[left].statuses.end(), "Corruption") == a_gameAPI->hand.cards[left].statuses.end())) {
            a_gameAPI->hand.cards[left].statuses.push_back("Corruption");
        }

        if (a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[right].names.back()).allegiance != CardAllegiance::DEMONIC && (a_gameAPI->hand.cards[right].statuses.size() == 0 || std::find(a_gameAPI->hand.cards[right].statuses.begin(), a_gameAPI->hand.cards[right].statuses.end(), "Corruption") == a_gameAPI->hand.cards[right].statuses.end())) {
            a_gameAPI->hand.cards[right].statuses.push_back("Corruption");
        }

        a_gameAPI->hand.cards[a_index].lies = true;
        std::vector<std::string> available = std::vector<std::string>();

        for (const std::string& card : a_gameAPI->deck.cards) {
            switch (a_gameAPI->cardTypes.at(card).allegiance) {
                case CardAllegiance::VILLAGE:
                case CardAllegiance::NEUTRAL:
                case CardAllegiance::OUTCAST:
                case CardAllegiance::OUTSIDE:
                    available.emplace_back(card);
                    break;
            }
        }

        if (available.size() == 0) {
            return;
        }

        uint32_t index = RandomUniformUInt32(0, (uint32_t)(available.size()));
        a_gameAPI->hand.cards[a_index].displayName = available.at(index);
    };
    plagueBringerCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return a_gameAPI->hand.cards[a_index].displayName != a_gameAPI->hand.cards[a_index].names.back() && a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).canActivate && a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).canActivate(a_gameAPI, a_index);
    };
    plagueBringerCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        if (a_gameAPI->hand.cards[a_index].displayName != a_gameAPI->hand.cards[a_index].names.back()) {
            a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).onActivate(a_gameAPI, a_index);
        }
    };


    CardType* urchinCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Urchin", "The Urchin will say one random liar. Killing an Urchin does 2 damage.", 
        CardAllegiance::OUTCAST, true);
    urchinCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(2);
    };
    urchinCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return true;
    };
    urchinCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        std::vector<size_t> liars = std::vector<size_t>();
        if (!a_gameAPI->hand.cards[a_index].lies) {
            for (const CardInstance& cardInstance : a_gameAPI->hand.cards) {
                if (cardInstance.lies) {
                    liars.push_back(cardInstance.index);
                }
            }
        } else {
            for (const CardInstance& cardInstance : a_gameAPI->hand.cards) {
                if (!cardInstance.lies) {
                    liars.push_back(cardInstance.index);
                }
            }
        }

        if (liars.size() > 0) {
            a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, std::format("#{} is lying.", liars[RandomUniformUInt32(0, (uint32_t)liars.size())]));
        } else {
            a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, "There are no liars.");
        }
    };

    CardType* werewolfCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Werewolf", "The Werewolf will hide as a random not in play Village aligned character.", 
        CardAllegiance::MONSTER, false);
    werewolfCard->onPostResolve = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->hand.cards[a_index].lies = true;
        std::vector<std::string> available = std::vector<std::string>();

        for (const std::string& card : a_gameAPI->deck.cards) {
            if (a_gameAPI->cardTypes.at(card).allegiance == CardAllegiance::VILLAGE) {
                available.emplace_back(card);
            }
        }

        if (available.size() == 0) {
            return;
        }

        for (CardInstance& card : a_gameAPI->hand.cards) {
            if (a_gameAPI->cardTypes.at(card.names.back()).allegiance == CardAllegiance::VILLAGE && std::find(available.begin(), available.end(), card.names.back()) != available.end()) {
                available.erase(std::find(available.begin(), available.end(), card.names.back()));
            }
        }

        if (available.size() == 0) {
            return;
        }

        uint32_t index = RandomUniformUInt32(0, (uint32_t)(available.size()));
        a_gameAPI->hand.cards[a_index].displayName = available.at(index);
    };
    werewolfCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return a_gameAPI->hand.cards[a_index].displayName != a_gameAPI->hand.cards[a_index].names.back() && a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).canActivate && a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).canActivate(a_gameAPI, a_index);
    };
    werewolfCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        if (a_gameAPI->hand.cards[a_index].displayName != a_gameAPI->hand.cards[a_index].names.back()) {
            a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_index].displayName).onActivate(a_gameAPI, a_index);
        }
    };

    CardType* witchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Witch", "The Witch will corrupt one other character. Killing a Witch does 1 damage.", 
        CardAllegiance::OUTSIDE, true);
    witchCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(1);
    };
    witchCard->onPostResolve = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        uint32_t newIndex = 0;
        while ((newIndex = RandomUniformUInt32(0, (uint32_t)a_gameAPI->hand.cardCount)) == a_index);
        a_gameAPI->hand.cards.at(newIndex).statuses.push_back("Corruption");
    };

    CardType* wraytheSlaereCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Wraythe Slaere", "The Wraythe Slaere will kill one character if they are undead. Killing a Wraythe Slaere does 4 damage.", 
        CardAllegiance::OUTSIDE, true);
    wraytheSlaereCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return true;
    };
    wraytheSlaereCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->maxSelect = 1;
        a_gameAPI->selecting = a_index;
        a_gameAPI->gameMouseState = TeraToma::GameMouseState::SELECTING;
        a_gameAPI->validCardSelectionFnptr = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            return !a_gameAPI->hand.cards[a_index].dead && a_index != a_gameAPI->selecting;
        };
        a_gameAPI->selectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            if (!a_gameAPI->hand.cards[a_index].lies && a_gameAPI->cardTypes.at(a_gameAPI->hand.cards[a_gameAPI->selected[0]].names.back()).allegiance == CardAllegiance::UNDEATH) {
                a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, std::format("I killed #{}.", a_gameAPI->selected[0]));
                a_gameAPI->TryKill(a_gameAPI->selected[0]);
            } else {
                a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, std::format("I could not kill #{}.", a_gameAPI->selected[0]));
            }
        };
    };
    wraytheSlaereCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->Hurt(4);
    };

    CardType* wretchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Wretch", "The Wretch will complain about their back if they are lying. Killing a Wretch does an amount of damage equal to the number of alive Outcasts, including the Wretch.", 
        CardAllegiance::OUTCAST, true);
    wretchCard->onKill = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        int damage = 1;
        for (CardInstance& card : a_gameAPI->hand.cards) {
            if (!card.dead && a_gameAPI->cardTypes.at(card.names.back()).allegiance == CardAllegiance::OUTCAST) {
                ++damage;
            }
        }
        a_gameAPI->Hurt(damage);
    };
    wretchCard->canActivate= [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        return true;
    };
    wretchCard->onActivate= [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        a_gameAPI->hand.cards[a_index].Respond(a_gameAPI, (a_gameAPI->hand.cards[a_index].lies) ? "My back hurts..." : "My head hurts...");
    };

    CardType* zombieCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Zombie", "The Zombie will kill unflipped characters one space away when flipped.", 
        CardAllegiance::UNDEATH, false);
    zombieCard->onFlip = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        int left = ((a_gameAPI->hand.cardCount + (int)a_index - 1) % (int)a_gameAPI->hand.cardCount);
        int right = (((int)a_index + 1) % (int)a_gameAPI->hand.cardCount);

        if (!a_gameAPI->hand.cards[left].flipped) {
            a_gameAPI->TryKill(left);
        }

        if (!a_gameAPI->hand.cards[right].flipped) {
            a_gameAPI->TryKill(right);
        }
    };

    std::println("We have exited the Base Game mod loading");
}    

extern "C" void MOD_API ModInitialize(TeraToma::GameAPI* const a_gameAPI) {
    std::println("We have entered the Base Game mod initialization");

    std::println("We have exited the Base Game mod initialization");
}

extern "C" void MOD_API ModUninitialize(TeraToma::GameAPI* const a_gameAPI) {

}

extern "C" void MOD_API ModUnload(TeraToma::GameAPI* const a_gameAPI) {

}