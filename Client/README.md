📘 Attendance System – C++ RFID Docházkový Systém
🧠 Popis projektu

Tento projekt je jednoduchý docházkový systém napsaný v C++, který pracuje s RFID čtečkou, SQLite databází a volitelně komunikuje se serverem přes HTTP.

Systém umožňuje:

evidenci příchodů a odchodů zaměstnanců

výpočet odpracovaného času

administraci zaměstnanců

ochranu proti neplatným vstupům

zabezpečení serveru pomocí API klíče

RFID čtečka se chová jako USB klávesnice (HID device) a posílá UID čipu jako hexadecimální řetězec.

🧱 Architektura systému
RFID čip
↓
USB RFID čtečka (HID keyboard)
↓
Pichačka (C++ aplikace)
↓
SQLite databáze / HTTP server


Volitelně:

Pichačka (klient) ──HTTP──▶ Server (C++) ── SQLite DB

🗄️ Databáze (SQLite)

Použity dvě hlavní tabulky:

employees
Sloupec	Typ	Popis
id	INTEGER	ID zaměstnance
name	TEXT	Jméno
rfid_uid	TEXT	UID RFID čipu (hex string)
status	INTEGER	0 = venku, 1 = uvnitř
active	INTEGER	1 = aktivní, 0 = deaktivovaný
is_admin	INTEGER	1 = admin karta
attendance
Sloupec	Typ	Popis
employee_id	INTEGER	Odkaz na zaměstnance
type	TEXT	entry / exit
timestamp	TEXT	YYYY-MM-DD HH:MM
🧠 Logika systému

RFID UID je bráno jako string, nepřevádí se na číslo

Stav zaměstnance je uložen v DB (status)

Systém zabraňuje dvojímu pípnutí během krátké doby

Výpočet odpracovaného času páruje pouze validní entry → exit

Neplatné sekvence jsou ignorovány

🔐 Admin režim

Admin karta je definována pomocí:

konkrétního RFID UID

nebo sloupce is_admin v DB

Admin může:

přidávat zaměstnance

deaktivovat / aktivovat zaměstnance

zobrazit souhrn odpracovaného času

⌨️ Důležitá poznámka k RFID a klávesnici

RFID čtečka posílá scancodes kláves, nikoliv znaky.

Je nutné mít systémové rozložení klávesnice přepnuté na:

English (US)

Jinak se zobrazují nesmyslné znaky místo hex UID.

🌐 Server (volitelný)

Server je napsán v C++ pomocí knihovny cpp-httplib.

běží na jiném PC

používá SQLite databázi

přijímá požadavky přes HTTP

komunikuje s pichačkou

API endpoint
POST /punch


📂 Přenositelnost databáze

Server automaticky hledá databázi relativně ke své .exe cestě:

attendance_server.db je ve stejné složce jako server.exe


Tím je zajištěna přenositelnost mezi zařízeními bez změny kódu.

🛡️ Ochrana vstupu

kontrola, že RFID obsahuje pouze hex znaky

ochrana proti rozbití cin

validace délky vstupu

ignorování neplatných sekvencí

▶️ Spuštění projektu
Pichačka

Spustit aplikaci

Přepnout klávesnici na EN (US)

Přikládat RFID čipy

Server

Spustit server.exe

Naslouchá na portu 8080

Čeká na HTTP požadavky s API klíčem

🎓 Co projekt demonstruje

práci s SQLite v C++

rozdělení projektu do modulů

stavový model zaměstnance

práci s časem a výpočty

komunikaci klient–server přes HTTP

základní aplikační zabezpečení

práci s HID zařízeními (RFID jako klávesnice)

🚀 Možná rozšíření

HTTPS komunikace

GUI rozhraní

reporty do CSV

přechod na PostgreSQL

nasazení na Raspberry Pi

👤 Autor

Projekt vytvořen jako školní práce zaměřená na návrh a implementaci docházkového systému v C++ s RFID a síťovou komunikací.