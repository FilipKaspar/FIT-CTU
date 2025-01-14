# Homework \#3

## Objectives
1. Prozkoumejte program a nalezněte v něm konstruktory objektů a přiřazování VMT.
    1. Ve výstupu uveďte konkrétní adresy konstruktorů i VMT.
    2. Ujistěte se, že to, co jste našli, je opravdu konstruktor!
2. Z VMT zjistěte, kolik má která třída virtuálních metod.
3. Z RTTI zjistěte, jak se třídy jmenují a která VMT náleží které třídě.
4. Popište hierarchii tříd.
    1. Součástí popisu by měla být i argumentace, co konkrétně v pozorovaných datech svědčí pro vámi zvolenou hierarchii a vylučuje hierarchie alternativní.
    2. Zjistíte, že hierarchie vykazuje určité zvláštnosti. Upozorněte na tyto zvláštnosti a vysvětlete, co znamenají a jak k nim došlo (co je způsobilo). Vaše vysvětlení nemusí být nutně zcela přesné (z binárky nejde jednoduše určit, která z možných příčin je ta správná), ale mělo by být konzistentní s pozorovanými skutečnostmi.

## Results

## Steps

Used software: **IDA Free**

#### 1. & 3. Prozkoumejte program a nalezněte v něm konstruktory objektů a přiřazování VMT

>   1. Ve výstupu uveďte konkrétní adresy konstruktorů i VMT.
>   2. Ujistěte se, že to, co jste našli, je opravdu konstruktor!

Prvně jsem si v IDĚ otevřel záložku strings, kde jsem našel stringy:

- AVCZizala
- AVCUnknown
- AUIUnknown
- AVIZizala
- AVCZizalaApp
- AVIZizalaApp
- AVCZizaliWindow
- AVIZizaliWindow
- AVCObrazovka
- AVIObrazovka

Název TypeDescriptoru se nachází v třídě RTTITypeDescriptor, podíváme se tedy na 2 dwordy nahoru, kde najdeme začátek classy. Víme, že na TypeDescriptor nám odkazuje RTTICompleteObjectLocator a RTTIBaseClasDescriptor. Tyto dvě classy můžeme rozlišit na základě toho, co následuje jako další argument po type descriptoru. V případě BCD následuje dword, který obsahuje počet bázových tříd, zatímco v případě COL následuje pointer na RTTIClassHiearchyDescriptor. Vybral jsem tedy COL, našel začátek třídy a následně na pozici o -1, kde se na záčátek OCL odkazuje, našel VMT daných tříd.

Adresy konstruktorů, přirazování VMT a VMT tříd:

| Class           | Constructor & | VMT Assigned & | Class VMT & | 
| --------------- | ------------- | -------------- | ----------  |
| AVIZizala       | N/A           | N/A            | N/A         |
| AVCZizala       | 0x004010E0    | 0x00401113     | 0x00414570  | 
| AUIUnknown      | N/A           | N/A            | N/A         |
| AVCUnknown      | N/A           | N/A            | 0x004147A4  |
| AVIZizalaApp    | N/A           | N/A            | N/A         |
| AVCZizalaApp    | 0x00401A70    | 0x00401A8A     | 0x004145F8  |
| AVIZizaliWindow | N/A           | N/A            | N/A         |
| AVCZizaliWindow | 0x00402220    | 0x00402248     | 0x00414678  |
| AVIObrazovka    | N/A           | N/A            | N/A         |
| AVCObrazovka    | 0x00402900    | 0x00402933     | 0x004146A4  |

K žádné interface třídě jsem nemohl najít žádné VMT , tudíž potom ani žádný RTTTICompleteObjectLocator. Což si myslím, že dává smysl, protože VMT se vytváří pouze pro konkrétní implementace virtuálních metod. Jelikož interface sám o sobě neimplementuje tyto virtuální metody, ale pouze je definuje, tak nedává smysl, aby pro ně byla vytvořena VMT.

CUnkown konstruktor nemá separátní konstruktor, ale kompilátor ho skrze interface třídy přidá rovnou do konstruktoru tříd, které dědí z interfaců, když se vytváří instance těchto zděděných tříd.

Takže když se zavolá např. CZizala(...), tak se zavolá konstruktor IZizala, který zavolá konstruktor CUnknown, ale jelikož IZizala žádné metody neimplementuje, pouze definuje, tak místo volání konstruktoru CUnknown, se konstruktor CUnknown inline přímo do konstruktoru CZizala.

#### 2. Z VMT zjistěte, kolik má která třída virtuálních metod

Počet virtuálních metod každé třídy zjistíme tak, že se podívám na její RTTICompleteObjectLocator v .rdata, kde se na ní odkazuje. Pointery pod pointerem COL jsou všechno virtuální metody.

| Class           | # Virtual Methods|
| --------------- | ---------------- |
| AVIZizala       | 0                |
| AVCZizala       | 14               | 
| AUIUnknown      | 0                |
| AVCUnknown      | 4                |
| AVIZizalaApp    | 0                |
| AVCZizalaApp    | 17               |
| AVIZizaliWindow | 0                |
| AVCZizaliWindow | 6                |
| AVIObrazovka    | 0                |
| AVCObrazovka    | 10               |

Jelikož jsem nenašel COL pro interfacy, počet virtuálních metod pro interfacy tedy bude nula.

#### 4. Popište hierarchii tříd

>    1. Součástí popisu by měla být i argumentace, co konkrétně v pozorovaných datech svědčí pro vámi zvolenou hierarchii a vylučuje hierarchie alternativní.
>    2. Zjistíte, že hierarchie vykazuje určité zvláštnosti. Upozorněte na tyto zvláštnosti a vysvětlete, co znamenají a jak k nim došlo (co je způsobilo). Vaše vysvětlení nemusí být nutně zcela přesné (z binárky nejde jednoduše určit, která z možných příčin je ta správná), ale mělo by být konzistentní s pozorovanými skutečnostmi.

| Class           | Base Clases                                              |
| --------------- | -------------------------------------------------------- |
| AVIZizala       | AVIZizala, AVCUnknown, AUIUnknown                        |
| AVCZizala       | AVCZizala, AVIZizala, AVCUnknown, AUIUnknown             | 
| AUIUnknown      | AUIUnknown                                               |
| AVCUnknown      | AVCUnknown, AUIUnknown                                   |
| AVIZizalaApp    | AVIZizalaApp, AVCUnknown, AUIUnknown                     |
| AVCZizalaApp    | AVCZizalaApp, AVIZizalaApp, AVCUnknown, AUIUnknown       |
| AVIZizaliWindow | AVIZizaliWindow, AVCUnknown, AUIUnknown                  |
| AVCZizaliWindow | AVCZizaliWindow, AVIZizaliWindow, AVCUnknown, AUIUnknown |
| AVIObrazovka    | AVIObrazovka, AVCUnknown, AUIUnknown                     |
| AVCObrazovka    | AVCObrazovka, AVIObrazovka, AVCUnknown, AUIUnknown       |

Z těchto znalostí již lze sestavit hiearchii tříd. AUIUnknown musí být jistě v kořenu. Z ní musí dědit AVCUnknown. Následně je vidět, že AVIZizala, AVIZizalaApp, AVIZizaliWindow a AVIObrazovka mají stejné base classy, tudíž musí být na stejným "levelu" v hiearchii. O level níž se pak nacházejí jejich AVC alternativy.

Výsledná hiearchie potom vypadá následovně:

![class_hiearchy](/hiearchy.png)

Co se týče zvláštnostní v hiearchii, tak mi zde toho nepřijde tolik divného. Pouze asi jen typo ve tříde ZizaliWindow místo ZizalaWindow, ale je možné, že je to pouze skloněné :).
