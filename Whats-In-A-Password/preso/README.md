
## Speaker Notes:

### Cracking Windows Hash Demo
- Show hash file: \demos\winhash\windows.hash
- talk about the dictionary file: \demos\winhash\RockYou-MostPopular500000PassesLetters_less50000.dic
- talk about the rules file: \demos\winhash\rules\d3ad0ne.rule
- run the crack: 
  - NOTE: this takes around 45 seconds to complete on my laptop
  - cd \demos\winhash\
  - oclHashcat64.exe -m 1000 -a 0 windows.hash RockYou-MostPopular500000PassesLetters_less50000.dic -r rules\d3ad0ne.rule
    - -m 1000: Hash Type = NTLM
    - -a 0: Attack Mode = Straight
    - hash file to crack
    - dictionary file to use
    - -r rule file to use

### Password List Statistics Demo
- Show the password list: \demos\stats\pwds.csv
  - file was taken from an online dump of passwords
  - an attempt has been made to remove or alter pwds that clearly included someones first and last name while not changing the length or complexity
  - in some cases, names were generated to replace real names
  - any similarity to real people is purely coincidental
- run the stats tool:
  - cd \demos\stats
  - python statsgen.py pwds.csv



# Hiding in Plain Sight [![Build Status](https://travis-ci.org/hakimel/reveal.js.png?branch=master)](https://travis-ci.org/hakimel/reveal.js)

A presentation that I have given a handful of times on the topic of security, normalcy, and how easy it is to hide your activities amongst the "noise" of a typical computer environment.

## Presented At
- CodeMash 2014
- East Tennessee Cyber Security Summit

## Significant Changes
- Added some stego examples and re-ordered the talk. Also, removed the demo of bypassing the IDS.
- Original Presentation


## Credits
The talks I give are never entirely my own. I have ideas, I may put them together in possibly novel ways, but my technical knowledge is an amalgam of input from others within the community - those whom I have read, whose talks I have attended, who have freely placed code, tools, papers online - these folks deserve credit for any good that comes from these talks.

- The presentation software, reveal.js is developed by Hakim El Hattab and is Copyright (C) 2014 Hakim El Hattab, http://hakim.se. You can obtain a copy yourself [from his github repo](https://github.com/hakimel/reveal.js).

- I'm using the full-screen image plugin from Régis Behmo found here: https://github.com/regisb/reveal.js-fullscreen-img 

## License

This work is licensed under a [Creative Commons Attribution 3.0 License](http://creativecommons.org/licenses/by/3.0/)

[Rob Gillen](http://rob.gillenfamily.net)