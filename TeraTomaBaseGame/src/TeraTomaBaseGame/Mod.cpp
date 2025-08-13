#include <TeraTomaBaseGame/Mod.h>
#include <print>

using TeraToma::CardType;
using TeraToma::CardAllegiance;

extern "C" void MOD_API ModLoad(TeraToma::GameAPI* const a_gameAPI) {
    std::println("We have entered the Base Game mod loading");
    CardType* doctorCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Doctor", "The Doctor will .", CardAllegiance::VILLAGE);
    CardType* healerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Healer", "The Healer will .", CardAllegiance::VILLAGE);
    CardType* oracleCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Oracle", "The Oracle will .", CardAllegiance::DIVINES);
    CardType* plagueBringerCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Plague Bringer", "The Plague Bringer will .", CardAllegiance::DEMONIC);
    CardType* urchinCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Urchin", "The Urchin will .", CardAllegiance::OUTCAST);
    CardType* werewolfCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Werewolf", "The Werewolf will .", CardAllegiance::MONSTER);
    CardType* witchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Witch", "The Witch will .", CardAllegiance::OUTSIDE);
    CardType* wretchCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Wretch", "The Wretch will .", CardAllegiance::OUTCAST);
    CardType* zombieCard = a_gameAPI->LoadCard(MOD_NAME, 
        "Zombie", "The Zombie will .", CardAllegiance::UNDEATH);
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