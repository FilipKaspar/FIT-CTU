# Úkol 1: Hra života

Hra života je celulární automat, který simuluje vývoj společenství živých organismů. Odehrává se na matici buněk, vývoj je daný pouze počátečný konfigurací, která se podle daných pravidel bez jakéhokoliv další vstupu vyvíjí. Více informací na Wikipedii - [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)).


## Zadání

Vaším úkolem je implementovat tři funkce definované ve skriptu `game.php`.

První funkce `readInput` dostane na vstup string, který reprezentuje vstup:

- `X` - představuje živou buňku
- `.` - představuje mrtvou buňku

Jednotlivé řádky jsou oddělené pomocí `PHP_EOL`. Můžete počítat s tím, že jsou všechny stejně dlouhé. Cílem této funkce je převést string na vámi zvolenou reprezentaci matice buněk, kterou budete používat při výpočtu dalšího kroku hry (např. 2D pole).

Inverzní k této funkci je funkce `writeOutput`, která naopak převede matici buňěk zpět na string.

Poslední je funkce `gameStep`, která dostane na vstup matici buněk a vrátí matici buněk v následujícm kroku podle pravidel:

1. Každá živá buňka s méně než dvěma živými sousedy zemře
2. Každá živá buňka se dvěma nebo třemi živými sousedy zůstává živá
3. Každá živá buňka s více než třemi živými sousedy zemře
4. Každá mrtvá buňka s právě třemi živými sousedy ožije

Za sousedy považujte všech 8 okolních buněk, tj. včetně těch, které sousedí "šikmo".

Vyhodnocení, zda je buňka naživu se provádí pouze na prostoru matice, ne za její hranou. Buňky za okrajem se počítají vždy jako mrtvé (pro vyhodnocení života buněk v matici).

Funkce ponechte se stejnou anotací, můžete vytvořit další pomocné funkce podle potřeby.


## Struktura projektu

- `input` - složka obsahuje příklady počátečních generací, které můžete použít
- `src/animate.php` - skript, který každou vteřinu vypisuje do terminálu další generaci
- `src/game.php` - skript, ve kterém máte za úkol implementovat výše zmíněné funkce
- `src/next.php` - skript, který vypíše následující generaci vstupní generace


## Spuštění

Ke spuštění a otestování můžete využít příklady ze složky `input` a skripty `src/next.php` a `src/animate.php`, ve kterých už je implementováno načítání ze souboru a vypisování výsledků a které používají funkce z `game.php`.

Vyhodnocení dalšího kroku:

```
$ php src/next.php input/block.txt
```

Spuštění simulace s danou počáteční generací:

```
$ php src/animate.php input/blinker.txt
```

Skripty `animate.php` a `next.php` není potřeba měnit. Můžete přidat další příklady počátečních generací, např. z [Life Lexicon](http://conwaylife.com/ref/lexicon/lex.htm).

## Odevzdání
Do doby dokončení automatické pipeline budou domácí úkoly hodnoceny automaticky hromadně po vyprčení času na úlohu. Automatické hodnocení se bude koukat na obsah `master` větve. Pokud po deadline nebude řešení ve větvy master, bude ohodnoceno 0 body. 

## Poznámky k hodnocení
- Za žadnou cenu neměňte strukturu projektu. Úloha je hodnocena automatickým scriptem, který na ní spoléha. Při její upravě budou pravděpodobně testy padat a neobdržíte žádné body.
- V úloze nejsou povoleny žádné externí knihovny, nebo scripty z internetu.
