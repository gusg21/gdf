#include "character.h"

#include "stdio.h"

/* Step this character like it's a dwarf. */
void _dwarf_step(struct character* character) {
    character->pos.x += 1; // Dwarves always move right for some reason.

    // Dwarves should have jobs...
    // Every job will require some functionality, similar to the way every character kind
    // has some step behavior.
    // Should all characters have jobs? Maybe tasks is more accurate?
    // TODO
}

void _unimplemented_step(struct character* character) {
    printf("Unimplemented step for character kind %u!\n", character->kind);
}

void character_step(struct character* character) {
    switch (character->kind) {
        case CK_DWARF:
            _dwarf_step(character);
            break;

        default:

            break;
    }
}