# socket – TCP/UDP-Socket-Bibliothek

C-Bibliothek für TCP/UDP-Socket-Kommunikation: Verbindungsaufbau,
Senden/Empfangen, Timeout-Steuerung und Ping.

## Umfang

- `socket_create` – Socket erstellen (TCP/UDP) mit Timeout
- `socket_get_own_ip` – Eigene IP-Adresse ermitteln
- `socket_connect` – TCP-Verbindung aufbauen
- `socket_send` / `socket_receive` – Daten senden/empfangen
- `socket_bind_and_listen` / `socket_accept_incomming_connection` – Server-Modus
- `socket_udp_broadcast` – UDP-Broadcast senden und Quell-IP der Antwort erfragen (optional eigene IP ignorieren)
- `socket_ping` – Erreichbarkeit prüfen
- `socket_close` – Socket schließen

## Nutzung

```c
#include <socket/socket.h>

socket_handle_t sock = socket_create(5, true);
socket_connect(sock, "192.168.1.100", 11880);
socket_send(sock, "data", 4);
socket_close(&sock);
```

## Abhängigkeiten

- `string`, `threading`

## Build

```bash
cmake -S . -B build
cmake --build build
```

In ein Projekt einbinden: `add_subdirectory(../../libraries/socket …)`,
Einbindung des Headers über den Include-Pfad `<socket/socket.h>`.

## Wiki

Dokumentation: [socket – TCP/UDP-Socket-Bibliothek](https://czybor.i234.me/wiki/sw-module/socket/) (Quartz-Wiki)
