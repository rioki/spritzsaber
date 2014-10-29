
Spritz Sabre
============

The spritz sabre is a small and practical implementation of cryptography,
modelled after [Cipher Saber][1], but employing the [Spritz][2] 
stream cipher. 

This is a fun hobby project of mine. You should not use it for anything serious.

Although the Spritz cipher is considered quite strong, it is fairly new and
unproven. Additionally Spritz Saber uses the built in rand function to initialize
the IV, this is not considered the safest thing to do. 

You can do with the code whatever you like, see COPYING.txt for details.

[1]: http://ciphersaber.gurus.org/
[2]: http://people.csail.mit.edu/rivest/pubs/RS14.pdf
