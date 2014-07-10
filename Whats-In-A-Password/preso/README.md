
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


### clarity on the worst passwords slide
- link: https://xato.net/passwords/more-top-worst-passwords/
- has built up a list of over 6,000,000 real-world passwords that have been released publicly *not* including the 30M in the rockyou list.
- From that, he has released a list of 10,000 of what he describes as "the most common" passwords
  - this list is flattened case-wise (all l-cased)
  - summed
  - this list represented 99.8% of all the passwrods in the 6M list (~12,000 not included)
