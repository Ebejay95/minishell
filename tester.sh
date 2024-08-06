#!/bin/bash

# Der erste Befehl schlägt fehl
cp nonexistentfile.txt /some/directory/

# Aber das Skript läuft weiter
echo "Dieser Befehl wird trotz des vorherigen Fehlers ausgeführt"

# Setze das "errexit" Flag falls gewünscht
set -e  # Stoppt das Skript, wenn ein Fehler auftritt

# Um spezifische Befehle zu ignorieren:
some_command || true  # Ignoriert Fehler von 'some_command'

# Auch kann man Fehlerausgaben in /dev/null umleiten:
some_command 2>/dev/null

# Um das Verhalten von "set -e" für bestimmte Befehle zu umgehen
set +e  # Deaktiviert "errexit"
some_command
set -e  # Reaktiviert "errexit"
