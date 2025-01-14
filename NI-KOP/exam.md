# Exam notes

SO - simulované ochalzování <br>
GA - genetický algoritmus

1. Evoluční programování pracuje nad reprezentací:
- automatu ✅

2. Algoritmus, který má za běhu upravovat selekční tlak v genetickém algoritmu, může být založen na zjištění:
- diverzity (rozdílnost) jedinců ✅
- změny průměrné zdatnosti mezi generacemi ✅
- poměru zdatnosti např. nejzdatnějšího jedince a jedince v polovině pořadí ✅

3. Máte experimentálně vyhodnotit, zdat vaše aplikace genetického algoritmu správně zachází se selekčním tlakem:
- Budete sledovat vývoj průměrné, minimální a maximální zdatnosti jednotlivých generací ✅
- Použijete lehké i těžké instance ✅
- Výpočet spustíte opakovaně pro každou instance ✅

4. Základní metodou vyhodnocení, se kterou pracuje Fast Messy genetický algoritmus (GA) je:
- Dosazení hodnot fragmentů do referenčního jedince a výpočet jeho zdatnosti ✅

5. Instance problému splnění podmínek má n konfiguračních proměnných, doména každé proměnné má právě d hodnot. Algoritmus má stav odvozený pouze z konfiguračních proměnných.
- Stavový prostor má dⁿ stavů ✅
- Prostor prohledávání má (d+1)ⁿ stavů ✅

6. Algoritmus, který má za běhu upravovat selekční tlak v genetickém algoritmu s **lineárním škálováním**, bude **přímo** nastavovat
- Konstanty přepočtu zdatnosti ✅

7. Máte experimentálně vyhodnotit, zda doba běhu Las Vegas randomizovaného algoritmu roste nejvýše s kvadrátem velikosti instance. Chcete o tom napsat teoretický článek:
- Použijete instance vygenerované tak, aby každá instance zadané velikosti byla stejně pravděpodobná ✅
- Výpočet spustíte opakovaně pro každou instanci ✅

8. Bayesovská optimalizace pracuje se základní jednotkou:
- Statický model závislostí mezi proměnnými ✅

9. Iterativní heuristika, problém obchodního cestujícího v rovině. Operátor je dvojzáměna na úsecích túry. Instance má 5 měst.
- Stavový prostor má silně souvislý graf ✅
- Okolí každého stavu má velikost 5 ✅

10. Genetický algoritmus dobře konverguje až do určité vzdálenosti od předpokládaného globálního minima, pak začne divergovat. Příčina může být:
- Povaha stavového prostoru se v okolí globálního minima prudce mění ✅
- Adaptace selekčního tlaku nepracuje dostatečně dobře ✅

11. Relaxace v iterativních lokálních heuristikách:
- Obvykle obsahuje numerický parametr, který je nutno experimentálně nastavit ✅
- Typicky nahrazuje optimalizační kriterétium heuristickou kombinací původního opt. kritéria a odhadu vzdálenosti konfigurace od řešení ✅
- Zlepšuje dosažitelnost ve stavovém prostoru ✅
- Má za úkol vést iterace od konfigurací, které řešením nejsou, k řešením ✅

12. Genetický algortimus s pravděpodobností mutace 40% připomíná:
- Metodu pouze nejlepší ❌
- Náhodnou procházku ❌

13. Srovnáváme 2 determinstické algoritmy A a B. Pro B různé instance jedné velikosti vykazují velký rozptyl v počtu kroků.
- Zjistíte statistické rozložení počtu kroků a pokud je symetrické, použijete průměr ✅
- Zjistíte statistické rozložení počtu kroků pro oba lagoritmy a vyhodnotíte, zda se překrývají a jak mnoho ✅
- Pokusíte se zjisti, jaká další charakteristika instancí má vliv na počet kroků ✅

14. Iterativní heuristika používá stavový prostor, jehož graf je silně souvislý. Má tyto vlastnosti:
- Může dát optimální řešení při libovolném počátečním stavu ✅
- Může to být simulované ochlazování, silná souvislost je jednou z podmínek úspěšného nasazení simulovaného ochlazování ✅

15. Algoritmus, který má za běhu upravovat selekční tlak v GA s výběrem **univerzálním stochastickým vzorkováním**, může **přímo** nastavovat:
- Konstanty lineárního škálování ✅
- Konstanty rankingu ✅

16. Experimentálně vyhodnotit, zda algoritmus který automaticky nastavaju počáteční teplotu pro SO, pracuje uspokojivě:
- Zjistíte z literatury ✅
- Použijete instance různé velikosti ✅
- Použijete instance s rozdílnou hloubkou lokálních minim ✅
- Výpočet spustíte opakovaně pro každou instanci ✅

17. Ranking v GA:
- Ovlivní pravděpodobnost výběru nejzdatnějšího jedince ✅
- V dané generaci, může způsobit zmenšení selekčního tlaku ✅
- V dané generaci, může způsobit zvětšení selekčního tlaku ✅

18. Heuristika, která nastavuje parametry SO:
- Má vždy brát v úvahu rozsah opt. kritéria nebo jej normovat ✅
- Pokud zjistí hloubku lokálních minim, dá se tato hodnota využít ✅
- Efekt dosažený s hloubkou ekvilibria se dá dosáhnout manipulací s koeficientem ochlazování ✅

19. Plánovací alg. Čas výpočtu přes noc. Alg A a B.
- Pro každou instance srovnáte průměrnou hodnotu opt. kritéria pro několik desítek až set spuštění ✅
- Pokud zjístíte že B je třikrát rychlejší než A. Pustim B třikrát, vyberu nejlepší ✅
- Použiju přednostně instance nachytané při předchozím manuální řízení ✅

20. Genetické operátory Fast Messy GA pracují s:
- Podmnožinami genů ✅

21. Alg. který za běhu upravuje selekční tlak v GA s **turnajovým výběrem**, bude **přímo** nastavovat:
- Velikost turnaje ✅

22. Volba selekčního tlaku v GA:
- Je omezena hrozbou divergence při malém selekčním tlaku ✅
- Závís na obtížnosti instance, obtžnější instance vyžadují pomalejší konvergenci ✅
- Může vyžadovat odpovídající nastavení pravděpodobnostní mutace ✅
- Může vyžadovat odpovídající nastavení pravděpodobnostni funkce ✅

23. Máte experimentálně vyhodnotit, zda alg., který automaticky udržuje selekční tlak v GA pracuje uspokojivě. Provedete následující:
- Budetem měřit četnost výběru (selekce) v závislosti na poměrné zdatnosti ✅

24. V GA je třeba zpomalit konvergenci. Pravděpodobně bude účinné:
- Upravit koeficienty lineárníh škálování ✅
- Přednostně snížíme selekční tlak ✅
- Pokud snížíme selekční tlak, může dojít k divergenci a je třeba snížit i pravděpodobnost mutace ✅

25. Experimentálně vyhodnotit zda relativní kvalita Monte Carlo rand. alg. nekledá s roustoucí velikostí intance:
- Budete potřebovat exaktní řešení ✅
- Použijete vygenerované instance, tak aby každá instance zadané velikosti byla stejně pravděpodobná ✅

26. Alg. který za běhu upravuje selekční tlak v GA s **linearním škálováním a ruletovým výběrem**, bude **přímo** nastavovat:
- Koeficienty lineárního škálování ✅

27. Koncová teplota v SO:
- Dá se s výhodou uročovat za běhu sledováním konvergence ✅

28. Evoluční strategie pracuje nad reprezentací:
- Vektoru reálných čísel a odchylek ✅

29. Vnější cyklus FastMessy GA postupně zvyšuje:
- Cílovou velikost fragmentů po generování ✅

30. GA je aplikován v situace, kdy některé části stav. prostoru mají výrazně větší hloubku lok. minim než jiné. Využijeme:
- Některých vlastností linearního škálování ✅

31. Nová generace v bayesovské optimalizaci vzniká:
- Generováním podle stochaistického modelu ✅

32. Stavební blok Fast Messy GAje vždy:
- Ohodnocení podmnožiny genů ✅

33. Metoda první zlepšení (first better) má tyto vlastnosti
- Zaručuje polynomiální složitost ✅

34. Metoda pouze nejlepší (best only)
- Zaručuje polynomiální složitost ✅

35. Metoda nejlepší nejdříve:
- Nezaručuje polynomiální složitost ✅
- Je systematická ✅
- Poskytuje exaktní řešení ✅
- Je úplná ✅

36. Vede snížení velikosti turnaje ke zvýšení intezifikace?
- NE ✅ 

37. Jak se pozná, že má lokální heuristika dostatečnou iterativní sílu?
- Po restartech skončí vždy ve stejném řešení ✅

38. Referenční jedinec v fmGA
- Slouží pro vyhodnocení zdatnosti ✅
- Při použití, jeho proměnné jsou nahrazování proměnnými fragmentů generické informace ✅

39. Genetické programování pracuje nad reprezentací
- Rozkladového stromu výrazu ✅

40. Nová generace v bayesovské optimalizaci vzniká
- Křížením ✅
- Ruletovým výběrem ✅

41. Typická úloha aspiračních kritérií je
- Intenzifikace ✅

42. Metoda Kernighan - Lin má následující vlastnosti
- Toto okolí prohledává metodou pouze nejlepší ✅
- Je založena na konstrukci proměnného okolí ✅

43. Stavební bloky ve Fast Messy GA se generují
- Jako podmnožiny ohodnocených genů zadané délky ✅

44. Pro globální metody platí
- Jsou založené na dekompozici ✅
- Mají rekurzivní formulaci ✅
- Přesná dekompozice dává exaktní výsledek ✅
- Pokud používájí čistou dekompozici a řešení nenalezou, znamená to, že řešení neexistuje ✅

45. Algoritmus, který má za běhu upravovat selekční tlak v genetickém algoritmu s **výběrem ruletou**, může **přímo** nastavovat
- Konstantní převod ranku na pravděpodobnost výběr ✅
- Konstanty lienárního škálování ✅

46. Algoritmus, který má za běhu upravovat selekční tlak v genetickém algoritmu, může být založen na zjištění
- Změny průměrné zdatnosti mezi generacemi ✅
- Diverzity jedinců ✅
- Poměru mzdatnosti např. nejzdatnějšího jedince v polovině pořadí ✅