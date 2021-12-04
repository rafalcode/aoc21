# Advent of Code 2021

Code and (particularly) learning points.

## Day 1

* index slicing an array can be done with a variable.
* map() needs to be wrapped in list() so you can operator on its elements.

## Day 2

* started using match until I realised that's a v3.10 innovation, so which to if statements
* copy pasting caused error, incremented [0] element instead of [1] (depth).
* despite a tuple seeming to natural for horizontal and depth positions, its inmutability means it's not useful for incrementing.
* second part was just an addition parameter, aim, from which depth can be derived, a sort of radar effect.

# Day 3
* part 1 was fine, but part 2 was a hike-up in difficulty due to the for loop iterating over a changing (decreasing) list.
* a list of indices for each different list had to be built, and there's always room for confusion when you are index fiddling.
* no sneaky optimizations (avoided bit fiddling!)this time as it took me about 90 minutes to get the second part.
* am now using listcomps profusely .. I tried the R trick of feeding a list of indices to the main list, no go. But listcomps are fine here.

# Day 4
* actually this was bingo cards, part 1 took a while longer this time
* also there's too many for loops quite unelegant.
* got caught with stopping filling in other cards with a number that had produced a winning card, useful for part 1 but not part 2.
* finding the last card to win was a clever turn to the game, made this more interesting.
* bugs crept in .. perhaps I should put in more incisive commenting, especially when the second part asks that one or two things be done in opposite manner to part 1.
* can defintely be cleaned up. used the mask quite quite often. Should be boolean?
