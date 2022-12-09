A Q|pmtn|C_max feladat megoldása Horvath, Lam és Sethi algoritmusával.

Az INPUT fájl első sora a gépek sebességeit tartalmazza szóközzel elválasztva, a második sor a feladatok elvégézéshez szükséges időket, szóközzel elválasztva.
Az INPUT fájlnak a testfiles mappában kell lennie. Néhány példát már tartalmaz a mappa (e_001.txt, ..., e_005.txt). 

--------------------------

A program fordítása:

mkdir build
cd build
cmake ..
make

--------------------------

A program futtatása (az e_001.txt példán):

./Q_pmtn_Cmax_solver e_001.txt -d

A program futtatása kevésbé részletes OUTPUT-tal:

./Q_pmtn_Cmax_solver e_001.txt

---------------------------

A kevésbé részletes OUTPUT csak a blokkfelbontást írja ki.
A részletesebb OUTPUT a blokkokon belüli feladatmegosztást is kiírja.
A könnyebb érthetőség kedvéért a program kiírja azt is, hogy az adott pillanatban az egyes feladatok elvégzéséhez még mennyi idő szükséges.
