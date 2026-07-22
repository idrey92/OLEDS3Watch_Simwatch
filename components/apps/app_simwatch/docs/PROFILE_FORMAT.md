# SimWatch Profile Format v1.0

## Overview

A SimWatch profile is a complete description of a user's life simulation.

The Engine never contains user-specific logic.

Everything is loaded from the profile.

A profile can be:

- Created
- Edited
- Imported
- Exported
- Shared
- Backed up

File extension:

```
.sim
```

Internally the format is JSON.

---

# Structure

```text
Profile
│
├── Metadata
├── Settings
├── Stats
├── Activities
├── Rules
├── Goals
├── Economy
├── Theme
├── Notifications
└── Localization
```

---

# Metadata

General information.

```json
{
  "name": "Developer",
  "author": "Andrew",
  "version": 1,
  "created": "2026-07-07",
  "modified": "2026-07-07",
  "description": "Personal developer profile"
}
```

---

# Settings

Global engine configuration.

```json
{
  "difficulty": "custom",
  "timeScale": 1.0,
  "pauseWhenCharging": false,
  "pauseWhenSleeping": false,
  "allowNegativeValues": false
}
```

---

# Stats

A list of user-defined stats.

```json
[
  {
    "id": "reading",

    "title": "Reading",

    "icon": "📚",

    "color": "#3B82F6",

    "enabled": true,

    "value": 100,

    "min": 0,

    "max": 100,

    "intervalMinutes": 30,

    "delta": -2,

    "status": [
      {
        "min": 80,
        "text": "Excellent"
      },
      {
        "min": 50,
        "text": "Good"
      },
      {
        "min": 20,
        "text": "Need Practice"
      },
      {
        "min": 0,
        "text": "Forgot Everything"
      }
    ]
  }
]
```

---

# Activities

User actions.

```json
[
  {
    "id": "read",

    "title": "Read Book",

    "durationMinutes": 30,

    "effects":
    [
      {
        "stat":"reading",
        "delta":20
      },
      {
        "stat":"energy",
        "delta":-5
      }
    ]
  }
]
```

---

# Rules

Automatic game logic.

```json
[
  {
    "condition":

    {
      "stat":"sleep",

      "operator":"<",

      "value":20
    },

    "effect":

    {
      "stat":"energy",

      "deltaMultiplier":2
    }
  }
]
```

---

# Goals

Long-term objectives.

```json
[
  {
    "id":"car",

    "title":"Buy a Car",

    "progress":12000,

    "target":50000,

    "reward":
    {
      "money":0,

      "experience":500
    }
  }
]
```

---

# Economy

Optional virtual economy.

```json
{
  "currency":"$",

  "balance":2400
}
```

---

# Theme

UI customization.

```json
{
  "name":"OLED",

  "accent":"#39FF14",

  "darkMode":true
}
```

---

# Notifications

Reminder behavior.

```json
{
  "enabled":true,

  "criticalOnly":false,

  "sound":true,

  "vibration":true
}
```

---

# Localization

Language preferences.

```json
{
  "language":"en"
}
```

---

# Design Principles

## Everything is configurable

Nothing is hardcoded.

---

## Everything is portable

One file contains everything.

---

## Engine is generic

Engine never knows:

- Hunger
- Sleep
- Money

Engine only knows:

- Stats
- Activities
- Rules

---

## GUI is generic

GUI renders only what the Profile describes.

---

## Web Editor edits only Profile

The firmware never changes.

---

# Future Compatibility

Each profile contains:

```json
{
    "profileVersion":1
}
```

Future firmware versions migrate old profiles automatically.