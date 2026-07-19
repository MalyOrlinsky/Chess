# Kung-Fu Chess

מנוע שחמט בזמן אמת הכתוב ב־C++17.

בניגוד לשחמט קלאסי, כל הכלים נעים בו־זמנית. השחקנים יכולים לבצע פקודות תנועה וקפיצה בכל רגע ללא מערכת תורות.

## תוכן עניינים

* [סקירת ארכיטקטורה](#סקירת-ארכיטקטורה)
* [מבנה הפרויקט](#מבנה-הפרויקט)
* [תבניות עיצוב](#תבניות-עיצוב)
* [עקרונות הנדסת תוכנה](#עקרונות-הנדסת-תוכנה)
* [אחריות מחלקות](#אחריות-מחלקות)
* [זרימת ביצוע](#זרימת-ביצוע)
* [הידור והרצה](#הידור-והרצה)
* [פורמט קלט](#פורמט-קלט)
* [מנגנון Jump](#מנגנון-jump)
* [מגבלות ידועות](#מגבלות-ידועות)

---

# סקירת ארכיטקטורה

המערכת מחולקת לשכבות עצמאיות. כל שכבה תלויה רק בשכבות שמתחתיה.

```
main.cpp
    |
ImgRenderer
    |
GameEngine
    |
+----------------+
| RuleEngine     |
| RealTimeArbiter|
+----------------+
    |
controllerClick / BoardMapper
    |
Board / Piece
    |
movement / config / text_io
```

## שכבות המערכת

### Model

מכיל את מודל המשחק:

* Board
* Piece

ללא חוקי משחק וללא תלות בתצוגה.

### Movement

מכיל חוקי תנועה לכל סוג כלי.

### Rule Engine

אחראי על בדיקת חוקיות:

* גבולות לוח
* צבעים
* חוקי תנועה
* מסלולים פנויים

### Arbiter

מנהל תנועות בזמן אמת:

* Motion
* Collision
* Jump
* Tick updates

### Game Engine

שכבת תיאום בין רכיבי המשחק.

### Renderer

מציג את המשחק באמצעות OpenCV.

---

# מבנה הפרויקט

```
Chess/
├── main.cpp
├── board.txt
├── CMakeLists.txt
│
├── src/
│   ├── Constants.hpp
│
│   ├── model/
│   │   ├── Piece.hpp
│   │   └── Board.hpp
│
│   ├── movement/
│   │   ├── MoveRule.hpp
│   │   ├── RookRule
│   │   ├── BishopRule
│   │   ├── QueenRule
│   │   ├── KnightRule
│   │   ├── KingRule
│   │   ├── PawnRule
│   │   └── PathBuilder
│
│   ├── rule_engine/
│   │   ├── RuleResult.hpp
│   │   └── RuleEngine
│
│   ├── arbiter/
│   │   ├── Motion
│   │   ├── RealTimeArbiter
│   │   ├── MotionUpdater
│   │   ├── MotionAdvancer
│   │   └── CollisionResolver
│
│   ├── controllerClick/
│   ├── game_engine/
│   ├── renderer/
│   ├── config/
│   └── text_io/
│
├── tests/
└── assets/
```

---

# תבניות עיצוב

## Strategy Pattern

כל כלי משתמש במחלקת תנועה משלו:

```
MoveRule
   |
   +-- RookRule
   +-- BishopRule
   +-- QueenRule
   +-- KnightRule
   +-- KingRule
   +-- PawnRule
```

הוספת כלי חדש אינה דורשת שינוי ב־RuleEngine.

---

## Adapter Pattern

BoardMapper ממיר:

```
Pixel Coordinate
        |
        v
Cell Position
```

כך שכבת התצוגה מופרדת מחוקי המשחק.

---

## DTO Pattern

Renderer עובד מול:

* GameSnapshot
* CellSnapshot

ואינו ניגש ישירות ל־Board או Piece.

---

## Builder Pattern

BoardBuilder אחראי ליצירת Board מתוך קובץ טקסט.

---

## Factory Pattern

PieceFactory יוצר כלים:

```
wK
bQ
wP
```

ומחזיר אובייקט מתאים.

---

## Observer / Callback

RealTimeArbiter מפעיל Callback כאשר מלך נתפס.

GameEngine משתמש באירוע כדי לעדכן סיום משחק.

---

# עקרונות הנדסת תוכנה

## Single Responsibility Principle

אחריות מרכזית:

| מחלקה           | אחריות            |
| --------------- | ----------------- |
| Board           | אחסון מצב הלוח    |
| Piece           | מודל כלי          |
| RuleEngine      | בדיקת חוקיות      |
| MoveRule        | חוקי תנועה        |
| RealTimeArbiter | ניהול זמן ותנועות |
| BoardMapper     | המרת קואורדינטות  |
| Renderer        | ציור              |

---

# אחריות מחלקות

| מחלקה             | אחריות                |
| ----------------- | --------------------- |
| GameEngine        | תיאום בין רכיבי המשחק |
| Board             | ניהול לוח             |
| Piece             | מחלקת בסיס לכלים      |
| RuleEngine        | ולידציה               |
| RealTimeArbiter   | ניהול תנועות          |
| MotionUpdater     | עדכון Tick            |
| CollisionResolver | טיפול בהתנגשויות      |
| BoardBuilder      | בניית לוח             |
| PieceFactory      | יצירת כלים            |
| SpriteLoader      | טעינת נכסים           |
| GameSnapshot      | DTO לתצוגה            |

---

# זרימת ביצוע

## מצב GUI

```
main.cpp
 |
טעינת board.txt
 |
BoardParser
 |
BoardBuilder
 |
PieceFactory
 |
Board
 |
GameEngine
 |
ImgRenderer
 |
Game Loop
```

בכל Frame:

1. עדכון זמן
2. עדכון תנועות
3. טיפול בקלט
4. יצירת Snapshot
5. ציור

---

# הידור והרצה

## דרישות

* C++17
* CMake 3.16+
* OpenCV 4.5.1

## בנייה

```bash
cmake -S . -B build
cmake --build build --config Release
```

## הרצה

Windows:

```bash
build/Release/KungFuChess.exe
```

Linux/macOS:

```bash
build/KungFuChess
```

---

# פורמט קלט

## לוח

```
wR wN wB wQ wK wB wN wR
wP wP wP wP wP wP wP wP
. . . . . . . .
```

`.` מייצג תא ריק.

---

## פקודות

```
click X Y
jump X Y
wait MS
print board
```

---

# סימוני כלים

| סימון | כלי    |
| ----- | ------ |
| K     | King   |
| Q     | Queen  |
| R     | Rook   |
| B     | Bishop |
| N     | Knight |
| P     | Pawn   |

---

# מנגנון Jump

כאשר כלי מבצע Jump:

* הכלי נשאר במשבצת המקור.
* ניתן ללכוד כלי אויב שנכנס למשבצת בזמן הקפיצה.
* כלי בתנועה אינו יכול לבצע Jump.
* כלי שכבר נמצא במצב Jump אינו מתחיל Jump נוסף.
* לחיצה נוספת על כלי Idle מפעילה Jump.

---

# מגבלות ידועות

| רכיב              | בעיה                           |
| ----------------- | ------------------------------ |
| GameEngine        | board ו־mapper ציבוריים        |
| GameEngine        | snapshot() אינו באחריותו       |
| GameEngine        | cellToNotation() אינו באחריותו |
| Assets            | נתיב כפול בקוד                 |
| SpriteLoader      | נתיב Config קשיח               |
| MotionUpdater     | קוד מת                         |
| CollisionResolver | העברת vector לפי ערך           |
| Board::movePiece  | אפשרות dangling pointer        |
| RookRule          | משתנים לא בשימוש               |
| PieceFactory      | מיקום מחלקות לא מתאים          |
| Renderer          | קבועים כפולים                  |
| ImgRenderer       | drawMoveList ציבורית ללא צורך  |

```
```
