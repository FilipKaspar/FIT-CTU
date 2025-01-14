Soubor config.txt musí mít celkem 5 bajtů a to \xA3\x90\x21\x11\xFB.

První byte musí být \xA3 kvůli tomu, že se porovnává u funkce mailaddline s parametrem "Dear Mr. Teacher,". Jelikož chceme slušný email, tak musíme vybrat tento string.

Druhý byte musí mít hodnotu \x90 abychom přidali string "I've completed my first reverse engineering task!".

Následuje uložení čtvrtého byte do registru CL, což je posledních 8 bytů registru ECX. Nicméně následně se ve funkci používá pouze dolní 4 bity této hodnoty.  Ve funkci se jednou projde for loop a následně na základě těchto spodních 4 bitů přídá string do mailu. Slušný string se přidá, pokud jsou zmíněné 4 bity rovny číslu 1. 

Následuje další funkce se switch casem. Třetí byte musí být \x21, abychom vybrali z DS hodnutu 1, která odpovídá slušnému stringu.

Pátý byte musí být vetší než \xFA, ale menší než \xFC, což znamená, že jediná mořnost je \xFB.

Poslední překážka je porovnání md5 hashe. Do výpočtu MD5 hashe se jako argument posílá všch 5 bytů, ale jelikož jediný byte, který můžeme měnit jsou první 4 bity 4 bytu, zkusil jsem hodnoty od \x01 až do \x11 :) 
Kombinace \xA3\x90\x21\x11\xFB dala stejný hash, se kterým se ve funkci porovnávalo prvních 10 bytů.

Tedy výsledná kombinace \xA3\x90\x21\x11\xFB pošle slušný email.