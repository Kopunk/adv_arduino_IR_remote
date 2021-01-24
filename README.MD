# Advanced Arduino IR Remote
A university project developed by Aleksander Kopeć and Wojciech Kopański. <br>
The following documentation is written in polish as it will be taken under grading.

# Założenia Projektu
Projekt ma za zadanie umożliwić rozbudowaną komunikacje pilot<->arduino<->odbiornik. Użytkownik porusza się po menu w którym wybiera poszczególne funkcje urządzenia:
- Forward IR -> Wysyłanie sygnałów w odpowiedzi na odebrany sygnał.
- Send IR    -> Wysyłanie sygnałów bezpośrednio z arduino
- Receive IR -> Wykonuje różne akcje na arduino zależnie od otrzymanego sygnału.
- Connect PC -> Połączenie z komputerem. Za pomocą dedykowanej aplikacji użytkownik łączy się z arduino.
- Settings -> Ustawienia. Zawarta jest tu m.in kalibracja pilotów oraz testowanie buzzera.

# Technologie Wymagane do Działania
### Wymagane Urządzenia
1. Arduino UNO 
2. LCD Keypad Shield
3. Odbiornik Podczerwieni (IR)
4. Nadajnik Podczewieni (IR)
5. Buzzer bez generatora
   
### Wymagane Programy
1. Arduino IDE 1.8 wraz z bibliotekami:
   - LiquidCrystal
   - IRremote
   - EEPROM
2. Python 3.8 wraz z bibliotekami:
   - PySerial
   - Tkinter
   - Time
   - os
3. Pliki projektu:
   - adv_arduino_IR_remote.ino
   - WriteDefaults.h
   - Buzz.h
   - remote_manager.py
   - preset.txt
   - README.MD

### Opis Podłączenia
- Odbiornik IR -> Digital pin 2
- Nadajnik IR -> Digital pin 3
- Buzzer -> Analog pin 1
- <Tutaj link do grafiki z fritzinga>

# Szczegółowe Omówienie Działania i Użytkowania

## Menu i Nawigacja
Użytkownik wybiera funkcjonalność przy pomocy przycisków **up** / **down** i zatwierdza lub przechodzi do niższego menu przy pomocy **select** lub **right**, cofnięcie lub anulowanie operacji następuje po naciśnięciu **left**. <br>
Drzewko menu oraz submenu:
```
Menu
├─ Forward IR
├─ Send IR
│  ├─ Bank #1
│  ├─ ...
│  ├─ Bank #9
├─ Receive IR
├─ Connect PC
├─ Settings
│  ├─ Calibrate Basic Buttons
│  ├─ Calibrate Additional Buttons
│  ├─ Test Buzzer
```

## Główne funkcje

### Forward IR
Korzystając z pilota posiadającego sygnały 'znane' przez arduino uruchamiamy sekwencje sygnałów wysyłane przez nadajnik arduino. Przykładowo: naciskając przycisk skonfgurowany jako Przycisk 4 z nadajnika IR zostanie wysłany sygnał [PRZYCISK3, PRZYCISK4, PRZYCISK5, PRZYCISK6].

### Send IR
Przenosimy się do submenu z listą dostępnych banków i wybieramy którą sekwencję nadać przy pomocy przycisków wbudowanych na LCD Keypad Shield. Modyfikacja nazw banków możliwa jest za pośrednictwem aplikacji komputerowej gdzie możliwe jest natychmiastowe wpisanie i załadowanie nowych nazw na arduino lub wczytanie pliku konfiguracyjnego. Możliwa jest również zmiana domyślnych nazw banków modyfikując plik `WriteDefaults.h`.

### Receive IR
Tryb kontroli arduino przy pomocy pilota podczerwieni, przy naciskaniu skonfigurowanych przycisków na ekranie pokazywane są informacje dot. przycisku oraz odtwarzane są sygnały dźwiękowe.

### Connect PC
Tryb połączenia z komputerem w celu konfiguracji.

### Settings
#### Calibrate Basic Buttons
Pozwala na kalibrację 10 podstawowych przycisków - na ekranie wyświetlane są instrukcje konfiguracji. Użytkownik musi nacisnąć każdy przycisk 3-krotnie lub przytrzymać go aż na ekranie nie pojawi się wiadomośc pozytywna. Przez zakłócenia oraz inne zmienne otoczenia dopiero po otrzymaniu 3-krotnie tego samego sygnału możemy być pewni że kalibracja przebiegła prawidłowo.
#### Calibrate Additional Buttons
Pozwala na kalibrację dodatkowych pięciu przycisków.
#### Test Buzzer
Pozwala sprawdzić czy buzzer został prawidłowo podłączony i jaka jest jego głośność (głośność można zmieniać zmieniając rezystory podłączone do niego szeregowo).

## Łączność z Komputerem
Aby skonfigurować nazwy banków lub wgrać plik z danymi (sygnały, banki, sekwencje) należy uruchomić program `remote_manager.py`. Program posiada interfejs graficzny użytkownika, w wierszu poleceń wyświetlane są wsyzstkie przekazywane dane. Po uruchomieniu pojawi się okno wyboru portu - należy wybrać odpowiedni port do którego podłączone jest arduino. Po zatwierdzeniu arduino zostaje zrestartowane (rozpoczęcie komunikacji), wyświetlona zostaje informacja, że teraz należy wybrać opcję Connect PC na arduino. Gdy wybierzemy opcję na arduino, pojawi się główne okno. Nazwy banków można zmienić przy pomocy 10 pól tekstowych (nazwy dłuższe niż 14 znaków zostaną skrócone), by zatwierdzić należy kliknąć przycisk "Save to Arduino". Wygenerowany zostanie plik z obecną konfiguracją i nowymi nazwami banków `newpreset` i jego dane zostaną wgrane do arduino. Aby załadować i wgrać własny plik należy wpisac jego nazwę w okienku "Preset file name" i zatwierdzić przyciskiem "Load preset". Podczas przesyłania na arduino wyświetlane są informacje o obecnie zapisywanych do EEPROM'u danych.

## Opis działania systemu
Pamięć EEPROM podzielona jest 3 sekcje:
- Zapisane sygnały z konfiguracji przycisków pilotów
- Nazwy banków (sekwencji sygnałów) mogące zostać zdefiniowane przez użytkownika
- Sekwencje (banki) - dziesięć 4-elementowych banków, każdy element jest liczbą odnoszącą się do numeru sygnału (skonfigurowanego wcześniej przycisku pilota)

W trakcie uruchomienia sprawdzany jest adres 0 EEPROM'u, w przypadku wartości 1 cała pamięć zostaje wyzerowana, adres 0 ustawiony na 255 i arduino jest restartowane. W przypadku wystąpienia wartości 255 w adresie 0, wczytywana jest podstawowa konfiguracja nazw banków oraz sekwencji z pliku `WriteDefaults.h`. <br>
Po uruchomieniu systemu konfiguracje zostają wczytane z pamięci EEPROM do pamięci dynamicznej arduino, tworzona jest tablica sekwencji sygnałów.

Nie istnieje domyślna konfiguracja sygnałów więc przed użytkowaniem użytkownik powinien skalibrować lub wczytać sygnały za pośrednictwem aplikacji.

### Adresy pamięci EEPROM dla poszczególnych elementów
Elementy zapisane w EEPROM dla czytelności i uproszczenia są zawarte pod stałymi adresami EEPROM:<br>
| Nazwa adresu | Adres w EEPROM |
| --- | --- |
| startupModeAddr | 0 |
| basicButtonsSignalsAddr | 1 |
| additionalButtonsSignalsAddr | 41  |
| banksNamesAddr | 61 |
| sequenceBanksAddr | 201 |

### Domyślne sekwencje:
Bank 1 = [POWER]<br>
Bank 2 = [VOLUME UP]<br>
Bank 3 = [VOLUME DOWN]<br>
Bank 4 = [PRZYCISK1]<br>
Bank 5 = [PRZYCISK2]<br>
Bank 6 = [POWER, VOLUME UP, VOLUME DOWN, POWER]<br>
Bank 7 = [PRZYCISK3, PRZYCISK4, PRZYCISK5, PRZYCISK6]<br>
Bank 8 = [POWER, POWER]<br>
Bank 9 = [UP, UP, DOWN, DOWN] // Potrzebna rozszerzona kalibracja przyciskow <br>
Bank 10 = [LEFT, RIGHT, LEFT, RIGHT] // Potrzebna rozszerzona kalibracja przyciskow<br>

### Protokoły IR
Przy nadawaniu każdy sygnał wysyłany jest czterokrotnie przy pomocy technologii najpopularniejszych producentów sprzętu RTV - LG, Sony, Samsung, Panasonic. Różnice w technologii IR nie stanowią jednak żadnego problemu przy odbieraniu przez arduino - wszystkie sygnały zapisywane są w 32 bitach.

### Buzzer
Podczas użytkowania (przy włączeniu urządzenia, rozpoczęciu komunikacji, korzystaniu z trybu Receive IR, jeżeli wystąpi błąd) odtwarzane są odpowiednie sygnały dźwiękowe za pośrednictwem funkcji `buzz()` napisanej na potrzeby projektu.


