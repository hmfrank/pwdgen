Deterministic Password Generator
================================

This project is an approach to a better password generator that fixes most problems of deterministic generation.

The problems
------------
0. Generated passwords are not accepted by websites because of pseudorandomness
0. The master password is in danger because of too fast hashing
0. Generators appear unappealing due to inconvenient handling
0. Account passwords cannot be changed due to determinism

My solutions
------------
0. Hash until the front of the digest contains lower and upper case letters (in base 64)
0. Interpret the back of the digest as a special character and a number
0. Use scrypt for slower hashing
0. Generate a really strong digest (~1 min of generation) and store it locally. Then use that for every normal generation.
0. A local account list for quicker generations
0. Both CLI and GUI for GNU/Linux and Windows
0. A version parameter to easily change your account passwords

Disclaimer
----------
I cannot guarantee the security of this application by any means, I am not a professional, so use it at your own risk.
If you spot a problem, please feel free to notify me straight away.


TODO
----
This page is not finished and needs reworking.
