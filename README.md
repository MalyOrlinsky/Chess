# Kung-Fu Chess

מנוע שחמט בזמן אמת ב־C++17 עם לקוח ושרת מבוסס WebSocket.

הפרויקט מחולק לשתי תוכניות עיקריות:

* **Client** – מטפל בקלט משתמש, הצגה ותקשורת WebSocket.
* **Server** – מנהל מצב המשחק, חישובי חוקיות, עדכון תנועות ולולאת משחק בזמן אמת.

---

# מבנה הפרויקט

```
Chess/
│
├── AI.MD
├── CMakeLists.txt
├── README.md
├── Server_Design.MD
├── board.txt
├── client/
│   ├── main.cpp
│   ├── NetworkClient.cpp
│   └── NetworkClient.hpp
├── server/
│   ├── main.cpp
│   ├── GameLoop.cpp
│   ├── GameLoop.hpp
│   ├── Game.cpp
│   ├── Game.hpp
│   ├── GameManager.cpp
│   ├── GameManager.hpp
│   ├── GameManager.tpp
│   ├── PlayerSession.hpp
│   ├── WebSocketServer.cpp
│   └── WebSocketServer.hpp
├── src/
│   ├── audio/
│   ├── commands/
│   ├── config/
│   ├── controllerClick/
│   ├── game_engine/
│   ├── model/
│   ├── movement/
│   ├── network/
│   ├── renderer/
│   ├── rule_engine/
│   └── arbiter/
├── opencv2/
│   ├── OpenCV_451/
│   └── src/
│       ├── img.cpp
│       └── img.hpp
├── external/
│   ├── asio/
│   ├── nlohmann/
│   └── websocketpp/
├── assets/
│   ├── image/
│   └── sounds/
├── core/
│   └── game/
├── tests/
├── type/
└── vcpkg/
```

---

# אדריכלות

```
Client
 |
 | WebSocket
 |
Server
 |
GameLoop
 |
GameEngine
 |
+-------------------+
| RuleEngine        |
| RealTimeArbiter   |
+-------------------+
 |
Board / Piece / State
```

---

# מודולים ושכבות

## Model

מכיל את מודל המשחק הבסיסי:

* `Board`
* `Piece`
* `PieceFactory`

המודל אינו תלוי בממשק משתמש או ברכיבי רינדור.

---

## Movement

מכיל את חוקי התנועה לכל כלי:

* `PawnRule`
* `RookRule`
* `BishopRule`
* `KnightRule`
* `QueenRule`
* `KingRule`

שכבה זו מיישמת את `Strategy Pattern` עבור חוקי התנועה.

---

## Rule Engine

מבצע בדיקות תקינות לתנועה:

* חוקיות מהלך
* גבולות לוח
* מסלול פתוח
* תקינות מקור ויעד

---

## Arbiter

מנהל לוגיקה של תנועות בזמן אמת:

* `Motion`
* `MotionUpdater`
* `MotionAdvancer`
* `CollisionResolver`
* `RealTimeArbiter`

---

## Game Engine

שכבת תיאום בין רכיבי המשחק:

* ניהול מצב המשחק
* יצירת snapshot
* חיבור בין מודל, תנועה וחוקיות

---

## Network

אחראי על תקשורת בין לקוח לשרת:

* `Protocol`
* `Message`
* `Serializer`
* `WebSocketServer`
* `NetworkClient`

---

## Client UI

* `renderer/` – רינדור תמונה ומצב המשחק.
* `controllerClick/` – מיפוי לחיצות וקלט משתמש.
* `audio/` – ניהול אפקטי סאונד.

---

## Configuration

* `src/config/` – קבצי קונפיגורציה משותפים ללקוח ולשרת.

---

# Design Patterns

## Strategy Pattern

```
MoveRule
 |
 +-- PawnRule
 +-- RookRule
 +-- BishopRule
 +-- KnightRule
 +-- QueenRule
 +-- KingRule
```

---

## Factory Pattern

```
PieceFactory
 |
 +-- Piece objects
```

---

## Builder Pattern

```
BoardBuilder
      |
      v
    Board
```

---

## DTO Pattern

העברת מצב המשחק לתצוגה:

```
Game State
    |
    v
GameSnapshot
    |
    v
Renderer
```

---

# דרישות

* C++17
* CMake 3.16+
* OpenCV 4.5.1
* WebSocket++

---

# Build

```bash
cmake -S . -B build
cmake --build build --config Release
```

---

# הרצה

Client (Windows):

```bash
build/Release/KungFuChessClient.exe
```

Server (Windows):

```bash
build/Release/KungFuChessServer.exe
```

---

# אחריות מחלקות מרכזיות

| מחלקה             | אחריות                                |
| ----------------- | ------------------------------------- |
| `Board`           | ניהול מצב הלוח                         |
| `Piece`           | מודל כלי                               |
| `PieceFactory`    | יצירת כלים                             |
| `RuleEngine`      | בדיקת חוקיות ומשחק חוקי                |
| `RealTimeArbiter` | ניהול תנועות וקונפליקטים בזמן אמת     |
| `GameManager`     | ניהול מצבי משחק וסשנים בשרת            |
| `Serializer`      | המרת הודעות לנתונים ובחזרה             |
| `NetworkClient`   | תקשורת לקוח עם השרת                   |
| `WebSocketServer` | תקשורת שרת עם לקוחות WebSocket         |
| `Renderer`        | הצגת מצב המשחק והאנימציה              |

---

# פורמט לוח

דוגמה:

```
wR wN wB wQ wK wB wN wR
wP wP wP wP wP wP wP wP
. . . . . . . .
```

סימונים:

| סימון | כלי    |
| ----- | ------ |
| K     | King   |
| Q     | Queen  |
| R     | Rook   |
| B     | Bishop |
| N     | Knight |
| P     | Pawn   |

---

# מגבלות ידועות

| רכיב          | הערה                           |
| ------------- | ------------------------------ |
| GameEngine    | קיימות עדיין תלויות בין רכיבים |
| SpriteLoader  | חלק מהנתיבים מוגדרים בקוד      |
| Renderer      | קיימים קבועים כפולים           |
| MotionUpdater | קיימים חלקים שאינם בשימוש      |
