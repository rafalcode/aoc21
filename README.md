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
* can definitely be cleaned up. used the mask quite quite often. Should be boolean?

# Day 5
* Moved to using regex to reading input, but found that group dict was annoyingly mixiing up order, easy labels a-d comes out as abdc or something, mad. Not dependable.
* If yesterday was far too C-like, today was worse. A grid, jammed packed with forloops and fiddly indexing up and/or down range, very easy to amek errors.
* however, part 2 was merely an extension of part 1, so in that sense easier than yesterday.

# Day 6 lanternfish
* I decide to move to C, all these puzzles seem to be
* forgot about my overly clean normalize function ... I really should use it only at end of big ops. Kept getting valgrind errors (a separate problem really)
* the "c" program is the part 1 program, it wants string on cmdline. A bit limiting though xargs could help
* this time part 1 clearly led us up garden path with regard to implementation, it would smash memory in part 2!
* actually, preventing this was easy. Most of my time was spent cleaining up my old c code.

# Day 7: position tiny crab submarines in one horizontal position at lowest movement cost
* Quite an easy one, more like the first two days.
* heavy use of forloops continues.
* Even the second part hardly needed much change ... the n*(n+1)/2 formula coming in handy though I alwways seem to think it's n*(n-1)/2
* Not as interesting as the last two days ... the fast big whale and blasting the ocean floor story very weak.
* got error due to my using unsigned char in vector (which actually was abandoned yesterday and was not right solution).

# Day 8 mixed up digit display
* part 1 was very easy, but part 2 wa quite difficult and demanded alot of thinking.
* a key was not finding letter to letter correspondance but finding unique sums for matches of one number in another of certain length.
* Once again done in C ... though I'd say the python Count() fn would have helped a good deal.
* More or less done organically, not much systematic thinking I'm afraid.

# Day 11, flashings
* This one took me a very long time, and I battled with this idea of propagation
* Like day 09, I did take recourse to getting hints, but actually in parallel I began
* the main issue for propagation is that a step is only finished when it's total flashing has finished. So "your function" should return numflashes=zero. (while not zero is required).
* otherwise I had a very thorny time equating my "single pass" output (which I tried in reverse, and many other ways) to what was required.
* I often fell to thinking that they've made a mistake ... which is possible but the correction would have come extremely quickly.
* so yes, the propagation issue had me fairly flustered, I need to remember it's a question of couting the events and waiting till they go to zero.
.
