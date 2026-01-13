🧠 Week-01-Flush-Reload-Attack

A practical demonstration of the Flush+Reload cache side-channel attack using a simplified setup inspired by the Mastik micro-architectural attack toolkit.
This repository focuses on educational clarity, especially through separate Victim and Attacker programs.

🧩 Background Theory — Flush+Reload Attack
Flush+Reload is a cache-timing side-channel attack based on:

🔹 Key Idea
If two processes share memory:
1.Victim accesses a memory line → it goes into cache
2.Attacker flushes and reloads the same line
3.Fast reload ⇒ Victim accessed it
4.Slow reload ⇒ Victim did not access it

Thus, attacker learns victim’s memory access behavior, which can leak:
1.Secret keys
2.Conditional branches
3.Cryptographic table lookups

🏗 System Requirements

1.Linux (recommended: Ubuntu)
2.x86 CPU with:
3.clflush instruction support
4.High-resolution timer (RDTSC)
5.GCC compiler
6.Mastik library (or compatible headers)

⚠️ Note: Running inside VirtualBox or VM may reduce accuracy due to noisy timing.

📁 Repository Structure
Week-01-Flush-Reload-Attack/
│
├── demo/
│   ├── attacker.c     # Attacker performing Flush+Reload
│   └── victim.c       # Victim accessing secret memory
│
├── src/               # Supporting source files (if any)
├── Makefile
├── LICENSE
└── README.md

🔧 Changes Compared to Original Mastik Repository
The original Mastik toolkit provides generic APIs and many attack types.
✅ Modifications in This Repository
The demo/ folder was modified to include:
demo/
├── attacker.c
└── victim.c

✔ Purpose of These Changes
1.Clear separation of Victim and Attacker
2.Realistic two-process attack model
3.Easier understanding of attack workflow
4.Suitable for academic demonstrations

🧪 Program Explanation
🟢 victim.c — Victim Program

Simulates a normal application that:
1.Repeatedly accesses a fixed memory location
2.Brings cache line into cache
3.Has no knowledge of attack

Victim Behavior
1.Loads shared memory
2.Enters infinite loop
3.Continuously reads target address

This models secret-dependent memory access as seen in:
1.AES table lookups
2.RSA exponentiation
3.Branch-based secrets

🔴 attacker.c — Attacker Program

Implements Flush+Reload:

Steps
1.Flush cache line using clflush
2.Wait briefly
3.Reload memory
4.Measure access time
5.Compare with threshold
6.Decide HIT or MISS

▶️ How to Compile and Run
Step 1: Go to Demo Folder
cd demo

Step 2: Compile Victim
gcc victim.c -o victim

Step 3: Compile Attacker
(If using Mastik library)
gcc attacker.c -o attacker -lmastik -lpthread

(or basic version)
gcc attacker.c -o attacker


▶️ Running the Attack
Open two terminals.
🟢 Terminal 1 — Run Victim
./victim
Victim will continuously access memory.

🔴 Terminal 2 — Run Attacker
./attacker

You will see output like:
HIT   85 cycles
MISS  320 cycles
HIT   90 cycles
HIT   88 cycles
MISS  310 cycles
