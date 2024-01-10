# snake_c11_linux
Používateľská príručka Hadík

Spustenie:

najprv príkaz: make
-	Spustí sa aplikácia server príkazom “./hadik” v priečinku kde sa nachádza
-	Spustí sa aplikácia client príkazom “./client” v priečinku kde sa nachádza.
Hranie:
-	Hra beží v dvoch oknách, pre každého hráča jedno
-	Hadík sa ovláda klávesami WASD
-	Hráči nesmú do seba navzájom narážať, ak jeden hráč narazí do druhého, hra skončí a hráč, ktorý narazil do druhého prehrá
-	Cieľom hry je nazbierať čo najviac jabĺk, vždy keď hráč zje jablko, jeho dĺžka narastie o jedno políčko
-	V prípade, že had narazí do steny, hra skončí, pričom hráč, ktorý narazil do steny prehrá a aplikácia zobrazí na obrazovke hráča, ktorý vyhral
-	
-	Hra na tomto okne ostane kým ktorýkoľvek z hráčov stlačí klávesu Q, ktorou vypne obidve okná
-	Hra sa taktiež dá kedykoľvek v priebehu hry vypnúť klávesou Q, čo zavrie obidve okná
