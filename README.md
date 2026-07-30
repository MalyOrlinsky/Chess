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

## מקומית

1. הפעל שרת + DB עם Docker:

   ```bash
   docker compose up
   ```

2. בנה והרץ את הלקוח:

   ```bash
   cmake -S . -B build -DOpenCV_DIR=C:/opencv/build
   cmake --build build --config Release
   build\Release\KungFuChessClient.exe
   ```

## ענן (AWS EC2)

השרת רץ על EC2 בכתובת `13.60.152.181:8080`.
הקליינט מחובר אוטומטית לשרת זה.

כל הקליינטים מתחברים לאותו שרת משותף.

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
│   ├── database/
│   ├── game/
│   ├── matchmaking/
│   ├── network/
│   ├── room/
│   ├── user/
│   ├── main.cpp
│   ├── PlayerSession.hpp
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
* `server/database/` – חיבור למסד נתונים והפעלת DB.
* `server/game/` – לוגיקת משחק, ניהול משחקים ולולאת המשחק.
* `server/matchmaking/` – התאמת שחקנים ל־Play.
* `server/network/` – WebSocket ותקשורת רשת.
* `server/room/` – יצירת חדרים פרטיים וניהול צופים.
* `server/user/` – ניהול משתמשים וחיבורי Session.
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

Client (Windows):
```bash
cmake -S . -B build -DOpenCV_DIR=C:/opencv/build
cmake --build build --config Release
```

Server (Docker):
```bash
docker compose up
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
