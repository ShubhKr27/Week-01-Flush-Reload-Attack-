# 🧠 Week-01 — Flush+Reload Cache Side-Channel Attack

A practical demonstration of the **Flush+Reload cache side-channel attack** using a simplified setup inspired by the **Mastik micro-architectural attack toolkit**.

This repository have **separate Victim and Attacker programs** to clearly demonstrate real-world two-process attack behavior.

---

## 📌 Background Theory — Flush+Reload Attack

Flush+Reload is a **cache-timing side-channel attack** based on shared memory and cache access latency.

### 🔹 Key Idea

If two processes share a memory line:

1. Victim accesses a memory line → it is loaded into cache
2. Attacker flushes and reloads the same line
3. **Fast reload ⇒ Victim accessed it (Cache HIT)**
4. **Slow reload ⇒ Victim did not access it (Cache MISS)**

Thus, the attacker can infer the victim’s memory access behavior, which may leak:

* 🔐 Secret keys
* 🌿 Conditional branches
* 📊 Cryptographic table lookups

This attack is especially effective against implementations of:

* AES (lookup tables)
* RSA (secret-dependent memory access)
* Branch-based secret logic

---

## 🏗 System Requirements

* Linux (Recommended: **Ubuntu**)
* x86 CPU with:

  * `clflush` instruction support
  * High-resolution timer (`RDTSC`)
* GCC Compiler
* (Optional) Mastik library or compatible headers

⚠️ **Note:** Running inside **VirtualBox or other VMs may reduce accuracy** due to noisy timing and lack of precise cache control. Bare-metal execution is recommended for best results.

---

## 📁 Repository Structure

```
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
```

---

## 🔧 Changes Compared to Original Mastik Repository

The original **Mastik toolkit** provides:

* Generic APIs
* Multiple cache attack techniques
* Complex experimental setup

### ✅ Modifications in This Repository

Only the **demo/** folder is modified to include:

```
demo/
├── attacker.c
└── victim.c
```

### ✔ Purpose of These Changes

* Clear separation of **Victim and Attacker**
* Realistic **two-process attack model**
* Easier understanding of attack workflow
* Suitable for:

  * Academic demonstrations
  * Internships
  * Security research learning

---

## 🧪 Program Explanation

### 🟢 `victim.c` — Victim Program

Simulates a normal application that unknowingly leaks information through cache usage.

#### Victim Behavior

1. Loads shared memory
2. Enters an infinite loop
3. Continuously reads a target address

This brings the cache line into cache repeatedly.

#### Models Real-World Scenarios Like:

* AES table lookups
* RSA exponentiation
* Secret-dependent branching

The victim **has no knowledge of the attacker**.

---

### 🔴 `attacker.c` — Attacker Program

Implements the full **Flush+Reload attack cycle**.

#### Attack Steps

1. Flush cache line using `clflush`
2. Wait briefly
3. Reload memory
4. Measure access time using `RDTSC`
5. Compare with timing threshold
6. Decide **HIT or MISS**

#### Interpretation

* **HIT → Victim accessed the memory**
* **MISS → Victim did not access the memory**

Over time, this reveals the victim’s access pattern.

---

## ▶️ How to Compile and Run

### Step 1: Go to Demo Folder

```bash
cd demo
```

---

### Step 2: Compile Victim

```bash
gcc victim.c -o victim
```

---

### Step 3: Compile Attacker

#### Basic Version (No Mastik API)

```bash
gcc attacker.c -o attacker
```

#### If Using Mastik Library

```bash
gcc attacker.c -o attacker -lmastik -lpthread
```

(Ensure Mastik is correctly installed and linked.)

---

## ▶️ Running the Attack

Open **two terminals**.

### 🟢 Terminal 1 — Run Victim

```bash
./victim
```

Victim will continuously access memory.

---

### 🔴 Terminal 2 — Run Attacker

```bash
./attacker
```

You should observe output similar to:

```
HIT   80 cycles
MISS  320 cycles
HIT   75 cycles
```

This indicates detection of victim memory accesses.


🧠 Prime + Probe Cache Side-Channel Attack (Using Mastik Toolkit)
📌 Overview

This project demonstrates the Prime + Probe cache side-channel attack using the Mastik (Micro-Architectural Side-Channel Toolkit) on Linux (Ubuntu).

Prime+Probe is a cache timing attack where:

The attacker primes (fills) specific cache sets.

The victim executes and may evict some cache lines.

The attacker probes the same cache sets and measures access time.

Higher access time ⇒ victim accessed that cache set.

PrimeProbeProject/
│
├── Mastik/                 # Mastik toolkit folder
│
├── demo/
│   ├── pp_attacker.c       # Attacker code
│   ├── pp_victim.c         # Victim code
│
└── README.md

🔧 Step 1: Install Dependencies

Open terminal and run:
sudo apt update
sudo apt install build-essential git make msr-tools -y

Load MSR module:
sudo modprobe msr

🧾 Step 3: Compile Victim Program

Go to demo folder:
cd demo
gcc PP_VICTIM.c -O0 -o pp_victim

🧾 Step 4: Compile Attacker Program
cd demo
gcc PP_ATTACKER.c -o pp_attacker \
-I.. -I../mastik -I../src \
../src/libmastik.a -lpthread

▶️ Step 5: Run Victim

Open Terminal 1:
cd demo
sudo taskset -c 0 ./pp_victim

▶️ Step 6: Run Attacker

Open Terminal 2:
cd demo
sudo taskset -c 0 ./pp_attacker
