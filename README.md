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
