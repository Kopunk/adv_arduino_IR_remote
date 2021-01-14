# Advanced Arduino IR Remote
A university project developed by Aleksander Kopeć and Wojciech Kopański. <br>
The following documentation is written in polish as it will be taken under grading.

# Założenia Projektu
Projekt ma za zadanie umożliwić rozbudowaną komunikacje pilot<->arduino<->odbiornik. Użytkownik porusza się po menu w którym wybiera poszczególne funkcje urządzenia:
- Forward IR -> Wysyłanie sygnałów w odpowiedzi na odebrany sygnał.
- Send IR    -> Wysyłanie sygnałów bezpośrednio z arduino
- Receive IR -> Wykonuje różne akcje na arduino zależnie od otrzymanego sygnału.
- Connect PC -> Połączenie z komputerem. Za pomocą dedykowanej aplikacji użytkownik łączy się z arduino. Aplikacja pozwala na m.in podejrzenie wartości i nazw zapisanych w pamięci sygnałów oraz zmianę ich.
- Settings -> Ustawienia. Zawarta jest tu m.in kalibracja pilotów oraz testowanie buzzera.


# Technologie Wymagane do Działania
### Wymagane Urządzenia
1. Arduino UNO 
2. LCD Keypad Shield
3. Odbiornik Podczerwieni (IR)
4. Nadajnik Podczewieni (IR)
   
### Wymagane Programy
1. Arduino IDE 1.8 wraz z bibliotekami:
   - LiquidCrystal
   - IRremote
   - EEPROM
2. Python 3.8 wraz z bibliotekami:
   - PySerial
   - Tkinter
   - 

### Opis Podłączenia
- Odbiornik IR -> Digital pin 2
- Nadajnik IR -> Digital pin 3
- Buzzer -> Analog pin 1
- <Tutaj link do grafiki z fritzinga>

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

## Kalibracja
Po wybraniu kalibracji użytkownik musi klikać 3-krotnie przyciski wyświetlone na ekranie. Przez zakłócenia oraz inne zmienne otoczenia dopiero po otrzymaniu 3-krotnie tego samego sygnału możemy być pewni że kalibracja przebiegła prawidłowo.

## Zapisywanie Sygnałów IR z Nadajnika
jak to działa...

## Nadawanie Zapisanych Sekwencji
Użytkownik po wybraniu opcji SendIR przenosi się do SubMenu w którym może wybrać sekwencje do wysłania.

## Łączność z Komputerem
jak to działa...

## Adresy pamięci EEPROM dla poszczególnych elementów
Elementy zapisane w EEPROM dla czytelności i uproszczenia są zawarte pod stałymi adresami EEPROM:<br>
connectPCAddr = 0<br>
basicButtonsAddr = 1<br>
additionalButtonsAddr = 41<br>
banksNamesAddr = 61<br>
banksAddr = 173<br>
otherSignals = 213<br>

## Banki pamięci:
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

