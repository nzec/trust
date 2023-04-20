# trust

- First, compile `compiler-clean.c` normally (`cc compiler-clean.c -o compiler-clean`)
- `./compiler-clean input_file out_file` takes in the `input_file`, "compiles it", and gives the binary `out_file` (actually just passes it to `gcc`)
- This is now our main compiler and is innocent.
- Now, compile `auth.c` (`./compiler-clean auth.c -o auth`)
- `./auth` only authorizes if you run as root
- Compile `compiler-trojan.c` using the compiler (`./compiler-clean compiler-trojan.c compiler-trojan`)
- Now if we use it to compile `auth.c` (`./compiler-trojan auth.c auth`), it will give us access even if we are not root.

Now,

- Compile `compiler-clean.c` using the malicious compiler (`compiler-trojan compiler-clean.c compiler-clean`)
- We can throw away `compiler-trojan.c` and `compiler-trojan` and pretend like nothing ever happened.
- Now, if we use our new `compiler-clean` (which is actually not-so-clean) to compile `auth.c` (`compiler-clean auth.c -o auth`), we see that it gives us access even if we are not root.

Moral: We can't trust our compiler.

## Resources:

- [Reflections on Trusting Trust - Ken Thompson](https://www.cs.cmu.edu/~rdriley/487/papers/Thompson_1984_ReflectionsonTrustingTrust.pdf)
- [Security Wednesdays #8 - "Reflections on Trusting Trust" - NUS Greyhats](https://www.youtube.com/watch?v=nQLUtCpt8-4)
- [Quines (self-replicating programs) - David Madore](http://www.madore.org/~david/computers/quine.html)
