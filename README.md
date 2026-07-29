# Kung-Fu Chess

מנוע שחמט בזמן אמת ב־C++17 עם לקוח ושרת מבוססי WebSocket.

הפרויקט בונה שני targets עיקריים:

* `KungFuChessClient` – לקוח הקולט קלט משתמש, מציג את המשחק ומשתמש ב־WebSocket לתקשורת.
* `KungFuChessServer` – שרת שמנהל מצב משחק, חוקיות מהלכים, לולאת משחק בזמן אמת, matchmaking וחדרים.

---

# מה חדש בדוקומנטציה

* הוספו הוראות בנייה והרצה.
* הוסברו התפקידים של `client/`, `server/`, `src/` ו־`external/`.
* הובהר כי קיימים `AI.MD` ו־`Server_Design.MD` לתיעוד מבני ועיצובי.

---

# דרישות

* CMake 3.16 או חדש יותר
* קומפיילר תומך C++17
* OpenCV 4.5.1
* Windows או Linux

---

# בנייה

1. פתח מסוף ב-root של הפרויקט:

   ```bash
   cd Chess
   mkdir build
   cd build
   cmake ..
   ```

2. בנייה:

   Windows (Debug/Release):
   ```bash
   cmake --build . --config Release
   ```

   Linux:
   ```bash
   cmake --build .
   ```

3. אחרי בנייה תיווצרנה ההרצות הבאות:

   * `KungFuChessClient`
   * `KungFuChessServer`

---

# הרצה

1. הפעל את השרת:

   ```bash
   ./KungFuChessServer
   ```

2. הפעל את הלקוח:

   ```bash
   ./KungFuChessClient
   ```

3. בצע חיבור לשרת דרך הלקוח כדי להתחיל משחק.

---

# מבנה הפרויקט

```
Chess/
│
├── AI.MD
├── CMakeLists.txt
├── Dockerfile
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
│   ├── MatchmakingManager.cpp
│   ├── MatchmakingManager.hpp
│   ├── RoomManager.cpp
│   ├── RoomManager.hpp
│   ├── Room.hpp
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

# ארכיטקטורה

```
Client
 |
 | WebSocket
 |
WebSocketServer
 |
 +----------------------+      +---------------------+
 |                      |      |                     |
MatchmakingManager   RoomManager  PlayerSession / Room
 |                      |      |                     |
 +----------+-----------+      +---------------------+
            |
       GameManager
            |
         GameLoop
            |
        GameEngine
            |
+----------------------+ 
| RuleEngine           |
| RealTimeArbiter      |
+----------------------+
            |
      Board / Piece
```

---

# מודולים ושכבות

## Client

* `client/main.cpp` – נקודת כניסה של הלקוח.
* `client/NetworkClient.*` – חיבור WebSocket, שליחת הודעות וקבלה של סנאפשוטים, מידע שחקן וסטטוס לובי.
* `src/renderer/` – רינדור מצב המשחק.
* `opencv2/src/img.cpp` – wrapper לרינדור OpenCV.

---

## Server

* `server/main.cpp` – נקודת כניסה של השרת.
* `server/WebSocketServer.*` – קבלת הודעות מלקוחות ושליחת עדכונים.
* `server/MatchmakingManager.*` – התאמת שחקנים ל־Play.
* `server/RoomManager.*` – יצירת חדרים פרטיים וניהול צופים.
* `server/GameManager.*` – ניהול המשחקים הפעילים.
* `server/GameLoop.*` – לולאת משחק והתקדמות טווח זמן.
* `server/Game.*` – מודל משחק בודד.
* `server/PlayerSession.hpp` – נתוני משתמש מחובר.

---

## src

* `src/model/` – מודל משחק בסיסי: לוח, כלי ומשחק.
* `src/movement/` – חוקי תנועה לכל כלי.
* `src/rule_engine/` – בדיקת תקינות מהלך.
* `src/arbiter/` – לוגיקה בזמן אמת, ניהול תנועה וקונפליקטים.
* `src/game_engine/` – תיאום בין כל רכיבי הליבה.
* `src/network/` – פרוטוקול הודעות וסיריאליזציה.
* `src/commands/` – פקודות משחק.
* `src/controllerClick/` – עיבוד קלט לחיצות.
* `src/audio/` – ניהול סאונד.
* `src/config/` – קבצי קונפיגורציה משותפים.
* `src/text_io/` – קלט/פלט טקסטואלי.

---

## external

* `external/websocketpp` – ספריית WebSocket++.
* `external/asio` – ASIO ללא Boost.
* `external/nlohmann/json` – JSON serialization.

---

## נכסים

* `assets/image/` – תמונות כלי.
* `assets/sounds/` – אפקטים קוליים.

---

# המלצות

* קראו גם את `Server_Design.MD` עבור זרימת המשחק והחלטות עיצוביות בצד השרת.
* השתמשו ב־`AI.MD` כנקודת התחלה להבנה מהירה של המבנה.
* בחרו `Release` לסביבת פרודקשן או `Debug` לפיתוח.


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

| מחלקה              | אחריות                                |
| ------------------- | ------------------------------------- |
| `Board`             | מצב הלוח                               |
| `Piece`             | מודל כלי                               |
| `PieceFactory`      | יצירת כלים                             |
| `RuleEngine`        | בדיקת חוקיות                           |
| `RealTimeArbiter`   | ניהול תנועות וקונפליקטים בזמן אמת     |
| `Game`              | משחק בודד                              |
| `GameManager`       | יצירה וניהול של משחקים               |
| `MatchmakingManager`| התאמת שחקנים ויצירת משחק             |
| `RoomManager`       | ניהול חדרים                           |
| `WebSocketServer`   | תקשורת מול הלקוחות                   |
| `Serializer`        | המרת הודעות                           |
| `NetworkClient`     | תקשורת הלקוח                         |
| `Renderer`          | הצגת המשחק                           |

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
