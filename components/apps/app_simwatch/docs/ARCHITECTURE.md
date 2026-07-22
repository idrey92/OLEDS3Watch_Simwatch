# SimWatch Architecture v1.0

## Project

**SimWatch**

**Slogan**

> Build your own life game.

---

# Goal

SimWatch is a fully customizable life simulation platform for smartwatches.

The user defines everything:

- Stats
- Actions
- Goals
- Rules
- Economy
- Appearance

No firmware modifications are required.

---

# Core Principle

The firmware must not contain any knowledge about real-life concepts.

There must never be hardcoded entities like:

- Hunger
- Sleep
- Mood
- Eat
- Work
- Buy Car

Instead, the engine operates only on generic objects.

Example:

Instead of

```cpp
Hunger
Sleep
Mood
```

use

```cpp
Stat
```

Instead of

```cpp
Eat
Sleep
Work
```

use

```cpp
Action
```

Instead of

```cpp
BuyCar
```

use

```cpp
Goal
```

---

# Architecture

```
                    SimWatch

            ┌───────────────────┐
            │        UI         │
            └─────────┬─────────┘
                      │
            ┌─────────▼─────────┐
            │    View Model     │
            └─────────┬─────────┘
                      │
            ┌─────────▼─────────┐
            │      Engine       │
            └─────────┬─────────┘
                      │
      ┌───────────────┼───────────────┐
      │               │               │
 ┌────▼────┐     ┌────▼────┐     ┌────▼────┐
 │ Storage │     │ Profiles│     │  Rules  │
 └─────────┘     └─────────┘     └─────────┘
                      │
                 JSON / NVS
```

---

# Engine

The Engine is the heart of SimWatch.

It knows nothing about LVGL, displays, buttons or Wi-Fi.

Engine modules:

- Stat
- Action
- Rule
- Goal
- Event
- Profile
- Economy
- Clock

---

## Stat

Represents any measurable value.

Examples:

- Reading
- Workout
- Water
- Sleep
- Programming
- Cat Care

---

## Action

Represents any user action.

Examples:

- Read
- Eat
- Drink
- Walk
- Study
- Work

---

## Rule

Represents dependencies between objects.

Example:

```
IF

Sleep < 20

THEN

Energy decreases twice as fast.
```

---

## Goal

Represents long-term objectives.

Examples:

- Buy a Car
- Read 50 Books
- Save $100000

---

## Event

Represents scheduled or triggered events.

Examples:

- Every hour update stats
- Goal completed
- Send notification
- Daily reward

---

## Economy

Optional subsystem.

Examples:

- Salary
- Rent
- Food
- Fuel
- Books

---

# Storage

Responsible only for persistence.

Responsibilities:

- Load Profile
- Save Profile
- Import
- Export
- Backup

Storage must not know anything about GUI.

---

# UI

Responsible only for presentation.

Responsibilities:

- Main Screen
- StatWidget
- Action Screen
- Goal Screen
- Notifications

UI never changes game logic directly.

---

# Web

Provides configuration without firmware changes.

Features:

- Create Stat
- Edit Stat
- Delete Stat
- Create Action
- Create Rule
- Create Goal
- Import Profile
- Export Profile
- Backup & Restore

---

# Rule #1

Everything is data.

The firmware must never contain code like:

```cpp
if (stat.id == "hunger")
```

or

```cpp
if (action == "sleep")
```

or

```cpp
if (goal == "car")
```

If such code appears, the architecture is considered broken.

All behavior must be described using:

- Profile
- Rules
- Actions
- Configuration

The Engine should execute generic logic only.

---

# Long-term Vision

SimWatch is not just a smartwatch application.

It is a portable life simulation engine where the smartwatch is only one of the user interfaces.

Future interfaces may include:

- Smartwatch
- Web Application
- Mobile Application
- Desktop Application

All of them share the same Engine and Profile format.