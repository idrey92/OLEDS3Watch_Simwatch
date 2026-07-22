# SimWatch Engine v1.0

## Purpose

The Engine is the heart of SimWatch.

It executes the simulation.

The Engine never knows:

- GUI
- LVGL
- Display
- Buttons
- Wi-Fi
- JSON
- Web Interface

The Engine only operates on data.

---

# Responsibilities

The Engine is responsible for:

- Updating stats
- Executing activities
- Evaluating rules
- Tracking goals
- Managing virtual economy
- Processing events
- Providing data to the UI

---

# Architecture

Profile

↓

Engine

├── Clock

├── Stat Manager

├── Activity Manager

├── Rule Engine

├── Goal Manager

├── Economy Manager

└── Event Queue

↓

View Model

↓

GUI

---

# Engine Modules

## Clock

Responsible for time.

Provides:

- Current timestamp
- Elapsed time
- Time scale
- Offline calculation

The Clock never accesses hardware RTC directly.

It receives timestamps from the platform.

---

## Stat Manager

Stores every Stat.

Responsibilities:

- Increase values
- Decrease values
- Clamp values
- Apply passive decay
- Notify changes

Never contains game rules.

---

## Activity Manager

Executes user activities.

Example:

Read Book

↓

Duration

↓

Effects

↓

Finished

The manager only executes effects.

---

## Rule Engine

Evaluates all Rules.

Example:

IF

Sleep < 20

THEN

Energy decay ×2

Rules are completely data-driven.

No hardcoded conditions.

---

## Goal Manager

Tracks long-term objectives.

Responsibilities:

- Progress
- Completion
- Rewards

Goals never modify GUI directly.

---

## Economy Manager

Optional subsystem.

Handles:

- Money
- Experience
- Credits
- Custom resources

Resources are generic values.

---

## Event Queue

Processes scheduled events.

Examples:

Every hour

↓

Update Stats

Activity finished

↓

Apply Effects

Goal completed

↓

Unlock Reward

Notification

↓

Send Event

---

# Engine Update Cycle

Platform provides current time.

↓

Clock calculates elapsed time.

↓

Passive stat decay.

↓

Execute scheduled activities.

↓

Evaluate rules.

↓

Update goals.

↓

Generate events.

↓

Create View Model.

↓

Return to GUI.

---

# Design Rules

## Rule 1

Engine never creates GUI objects.

---

## Rule 2

Engine never loads JSON.

---

## Rule 3

Engine never stores files.

---

## Rule 4

Engine never knows screen layout.

---

## Rule 5

Everything comes from Profile.

---

# Public API

The Engine should expose only a small API.

Example:

Engine

initialize(Profile)

update(currentTime)

executeActivity(id)

getStats()

getGoals()

getNotifications()

save()

The GUI never modifies objects directly.

All changes go through Engine.

---

# Offline Simulation

The Engine must support offline execution.

Example:

Last update

08:00

Current time

18:00

Elapsed

10 hours

↓

Clock

↓

Stat decay

↓

Rules

↓

Goals

↓

Events

↓

Ready

No timers are required.

---

# Future Features

Planned modules:

- Achievements
- Skills
- Random Events
- NPCs
- Seasons
- Weather
- Multiplayer Sync

These modules should integrate without changing the Engine architecture.