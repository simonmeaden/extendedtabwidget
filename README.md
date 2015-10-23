A simple extension of QTabWidget that allows various things to be added to any spare space next to the tabs.

This allows one of three things

1. A Login button
2. A Digital Clock
3. A Message Box.

Each of these has several options.

The clock can show or hide seconds.

The Login button has two forms, a standard form with username/password
and a simpler form that only allows a password to be set.

The message box allows the background/foreground colours to be changed,
either permanently or temporarily. Marquee'ing of long text messages are
also allowed.

All three can be drawn with or without frames.

This is available under the GPL version 3.0 so anything that is available
under this license is fine.

Of course if your computer blows up or your country sinks into the sea you
are on your own.


There is a complete-ish example in the examples directory with it's own
project file.

Change list.

1.0.0 12/9/2015

Initial version released.

1.0.1 23/10/2015

Originally the passwords were encrypted using a simple Base64 system from
OsDab. However I removed it as that allows the user to encrypt his passwords
however he wants to, so long as the encrypted passwords can be defined as
a QString the comparisons should still work OK. Let me know if it doesn't
work when you encrypt the input passwords.
