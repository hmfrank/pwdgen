# Deterministic Password Generator

This project is an approach to a better password generator that fixes most problems the deterministic concept suffer from.

## The problems
0. Generated passwords are not always accepted by websites
0. Too fast hashing makes brute-force attacks on the master password (relatively) easy
0. Generators seem unappealing due to inconvenient handling
0. Once generated passwords cannot be changed later

## My solutions
0. Hash multiple times if bad passwords are generated
0. Use scrypt for slower hashing
0. Generate a really strong digest (~1 min of generation) and store it locally. Then use that for every normal generation.
0. Make generations quicker with a locally saved account list
0. Both CLI and GUI for GNU/Linux and Windows
0. Introduce a version parameter that is included in the generation input

## How to use it
```
./pwdgen (for normal mode)
./pwdgen -d (for more information)
```
*This section will be expanded, once the CLI is final*

## Disclaimer
I cannot guarantee the security of this application, I am not a professional. Use it at your own risk.
I am open to criticism, however, so if you spot a problem, please feel free to notify me straight away.


## TODO
*This page is not finished and needs reworking.*
