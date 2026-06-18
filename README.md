This is intended to be a better version of default cp utility on GNU/Linux operating systems with extra features

Usage: multicp FILES[@RENAME] -dDESTINATIONS FILES[@RENAME]

Example: multicp ../cooldir/coolfile another/dircool/filecool -d~ -d~/coolfiles/

As result, coolfile and filecool are going to be copied to both ~ and ~/coolfiles directories!

It currently doesn't support some features of default cp like recursive copying but i will add them as I improve my skills
