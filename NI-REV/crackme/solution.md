# Semestrální práce - řešení

Pro řešení semestrální práce byla primárně použita aplikace IDA Free. 

## 1. Heslo

Po otevření programu cracke.exe sem zkusil ihned zmáčknot talčítko OK, to nic nedělá, ale tak zkusit se to musí :D. Je vidět, že do boxu nelze psát. V importech jsem našel funkci `EnableWindow`, která by mohla box odemknout. Přes jump a x-reference jsem našel, že se funkce volá potom, co se stisknou klávesy `FIT`. Používá se zde funkce `GetAsyncKeyState`. Po zmáčknutí těchto kláves se nám tedy box otevře a lze do něj psát.

Heslo jsem hledal tak, že jsem si zase v importech našel funkci `MessageBoxA`. Následně opět přes jump a x-reference našel její použítí. Hned první použítí nastáva po jumpu který kontroluje jestli je `ebp+var_4` nula. Následoval jsem tedy basic bloky směrem nahoru a našel funkci `GetDlgItemTextA` a hned pod ní instrukci `mov edx, lpString2`. Lpstring2 se rovná hodnotě `First day of July 2019`. Následně se provádí funkce `lstrcmpA`, která právě porovnává vstup z boxu s lpString2 a ná základě platnosti rovnosti se do `ebp+var_4` zapíše buďto 0 nebo 1. Tato funkce vrací 0 pokud jsou oba stringy stejné. Do basic bloku, který nám dá messagebox se success se dostaname pokud `lstrcmpA` vrátí 0. Tedy první heslo je `First day of July 2019`.