unsigned int setFailureBit(unsigned int selword, TriCuts value) {
// Set corresponding bit in word when event fails the specified cut
    unsigned int bit = 1u << static_cast<unsigned int>(value);
    unsigned int newword = selword | bit;

/*    std::cout << "[setFailureBit] Setting bit for cut: " << to_string(value)
              << " (bit " << static_cast<unsigned int>(value) << ")\n"
              << "    Before: 0x" << std::hex << selword
              << ", After: 0x" << std::hex << newword << std::dec << "\n";  */

    return newword;
}

bool isPassingCut(unsigned int selword, TriCuts value) {
// Check whether the selword indicates that the event passes the specified cut 
    unsigned int mask = 1u << static_cast<unsigned int>(value);
    bool result = (selword & mask) == 0;

/*    std::cout << "[isPassingCut] Checking if bit is NOT set for: " 
              << to_string(value) << " (bit " << static_cast<unsigned int>(value) << ")\n"
              << "Sel Word:     0x" << std::setw(2) << std::setfill('0') << std::hex << selword << " (" << std::bitset<8>(selword) << ")\n"
              << "    Mask:     0x" << std::setw(2) << std::setfill('0') << std::hex << mask   << " (" << std::bitset<8>(mask)   << ")\n"
              << "    selword & mask = 0x" << std::setw(2) << std::setfill('0') << (selword & mask)
              << " → Result: " << std::boolalpha << result << "\n" << std::dec; */

    return result;

}

bool PassesAllCutsSoFar(unsigned int selword, TriCuts value) {
// Checks whether the event satisfies cuts 0 through value.
    unsigned int bitLimit = static_cast<unsigned int>(value);
    unsigned int mask = (1u << (bitLimit + 1)) - 1u;  // mask of all bits 0..value
    return (selword & mask) == 0;
}

bool isOnlyBitSet(unsigned int selword, TriCuts value) {
// Check if the event only fails the specified cut
    unsigned int expected = 1u << static_cast<unsigned int>(value);
    bool result = (selword == expected);

/*    std::cout << "[isOnlyBitSet] Checking if only bit " << static_cast<unsigned int>(value)
              << " (" << to_string(value) << ") is set\n"
              << "    Word:     0x" << std::hex << selword << " (" << std::bitset<8>(selword) << ")\n"
              << "    Expected: 0x" << std::hex << expected << " (" << std::bitset<8>(expected) << ")\n"
              << "    Result:   " << std::boolalpha << result << "\n" << std::dec;   */

    return result; 
}

bool isSelectedOrFailsJustOneCut(unsigned int selword, TriCuts value) {
// Check if the event is selected or only fails the specified cut
    unsigned int mask = 1u << static_cast<unsigned int>(value);
    bool result = (selword == 0 || selword == mask);

/* std::cout << "[isZeroOrOnlyBitSet] Checking if selword is zero or only bit "
              << static_cast<unsigned int>(value) << " (" << to_string(value) << ") is set\n"
              << "    Word:     0x" << std::hex << selword << " (" << std::bitset<8>(selword) << ")\n"
              << "    Allowed:  0x0 OR 0x" << std::hex << mask << " (" << std::bitset<8>(mask) << ")\n"
              << "    Result:   " << std::boolalpha << result << "\n" << std::dec;   */

    return result;
}

bool isSelected(unsigned int selword) {
// Check if the event is selected (selword == 0).
    return selword == 0;
}

