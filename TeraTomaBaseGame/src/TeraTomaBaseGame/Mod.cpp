#include <TeraTomaBaseGame/Mod.h>
#include <print>

using TeraToma::CardType;
using TeraToma::CardAllegiance;

extern "C" void MOD_API ModLoad(TeraToma::GameAPI* const a_gameAPI) {
    std::println("We have entered the Base Game mod loading");
    CardType* doctorCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Doctor", "The Doctor will .", 
        CardAllegiance::VILLAGE, true);

    CardType* healerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Healer", "The Healer will allow you to use one flipped dead card.", 
        CardAllegiance::VILLAGE, true);
    healerCard->canActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        for (size_t i = 0; i < a_gameAPI->hand.cardCount; ++i) {
            if (i != a_index && a_gameAPI->hand.cards.at(i).dead && !a_gameAPI->hand.cards.at(i).activated) {
                return true;
            }
        }
        return false;
    };
    healerCard->onActivate = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
        size_t i = 0;
        a_gameAPI->maxSelect = 1;
        a_gameAPI->selecting = a_index;
        a_gameAPI->gameMouseState = TeraToma::GameMouseState::SELECTING;
        a_gameAPI->validCardSelectionFnptr = [](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            return a_gameAPI->hand.cards[a_index].dead && !a_gameAPI->hand.cards[a_index].activated;
        };
        a_gameAPI->selectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            i = a_gameAPI->selected.at(0);
            a_gameAPI->hand.cards.at(i).activated = true;
        };
        a_gameAPI->postSelectedFnptr = [&](TeraToma::GameAPI* a_gameAPI, size_t a_index) {
            if (a_gameAPI->cardTypes.at(a_gameAPI->hand.cards.at(i).names.back()).onActivate) {
                a_gameAPI->cardTypes.at(a_gameAPI->hand.cards.at(i).names.back()).onActivate(a_gameAPI, i);
            }
        };
    };

    CardType* oracleCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Oracle", "The Oracle will tell you the number of each type of allegiance across three other cards.", 
        CardAllegiance::DIVINES, true);

    CardType* plagueBringerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Plague Bringer", "The Plague Bringer will corrupt all characters two spaces away.", 
        CardAllegiance::DEMONIC, false);

    CardType* urchinCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Urchin", "The Urchin will .", 
        CardAllegiance::OUTCAST, true);

    CardType* werewolfCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Werewolf", "The Werewolf will .", 
        CardAllegiance::MONSTER, false);

    CardType* witchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Witch", "The Witch will .", 
        CardAllegiance::OUTSIDE, true);

    CardType* wretchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Wretch", "The Wretch will .", 
        CardAllegiance::OUTCAST, true);

    CardType* zombieCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Zombie", "The Zombie will .", 
        CardAllegiance::UNDEATH, false);

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