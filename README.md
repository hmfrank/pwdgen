# Deterministic Password Generator

Pwdgen is a lightweight, deterministic password generator written in C that
strives to offer a swift and convenient, but still secure experience for the
users. It will be available for GNU/Linux and Windows in CLI form.

Below you can find a list of some problems of deterministic generators and how
pwdgen tackles them.


## Problems of determinism
1. Generated passwords are not always accepted by websites
2. Too fast hashing makes brute-force attacks on the master password (relatively) easy
3. Having to enter your account data prior to every generation is inconvenient
4. Once-generated passwords cannot be changed later

## My solutions
1. Hash multiple times if bad passwords are generated
1. Extract a number and a special character from the digest and include them in the password
2. Use scrypt for slower hashing
2. Generate a really strong digest (~1 min of generation) and store it locally. Then use that for every normal generation.
3. Make generations quicker with a locally saved account list
4. Include a version parameter in the generation

## How to compile it
```
cd path/of/your/choice
git clone https://github.com/chlewe/pwdgen.git
make (for a normal executable)
make script (for a script-fit executable)
```

## How to use it
### Normal executable
```
./pwdgen (for normal mode)
./pwdgen -d (for more information)
```
Then simply follow the instructions on the screen.

### Script-fit executable
```
./pwdgen <account> <domain> <version> <password length> <master password>
```

## Disclaimer
I cannot guarantee the security of this application, I am not a professional. Use it at your own risk.
I am open to criticism, however, so if you spot a problem, please feel free to notify me straight away.


## TODO
*This page is not finished and needs reworking.*
