# 🕒 Queue Simulator
### *Progetto Reti di Telecomunicazioni - 2025*


## 📄 Descrizione del Progetto  
Questo progetto, interamente sviluppato in **C++**, simula un collegamento tra due nodi basato su un sistema a coda con un servente e coda infinita.  
Sono previste due modalità di simulazione:  
- **Modalità 1:** simulazione di una coda semplice con arrivi di Poisson e servizio esponenziale negativa, senza riscontri o errori;  
- **Modalità 2:** simulazione del protocollo Stop-and-Wait con trasmissione soggetta a errori e riscontri inviati su un canale separato secondo un processo di Poisson.  

Gli arrivi dei pacchetti seguono un processo di Poisson con tasso \(\lambda\), mentre i tempi di servizio sono esponenziali negativi con media \(1/\mu\).  
Durante la trasmissione, un errore può verificarsi con probabilità \(p\).  
Il ricevitore invia riscontri (acknowledgements) su un canale separato in istanti distribuiti secondo un processo di Poisson con tasso \(\delta\).  
Nel protocollo Stop-and-Wait il trasmettitore invia un pacchetto e attende il riscontro prima di procedere.  
Il software riceve in input tutti i parametri e calcola il tempo medio di attraversamento del pacchetto attraverso il sistema.


## 🎯 Obiettivo  
- Simulare il comportamento del sistema a coda con trasmissioni soggette ad errori  
- Calcolare il tempo medio di attraversamento del pacchetto nel sistema  
- Valutare l’effetto degli errori e dei riscontri sulla performance del collegamento  


## 🛠️ Compilazione e avvio del programma  
1. Aprire il terminale e posizionarsi nella cartella `code` del progetto  
2. Eseguire il comando `make clean` (per sicurezza, pulisce i file compilati precedenti)  
3. Compilare il progetto con `make`  
4. Avviare il programma con `./queue`  


## 👨‍💻 Team  
| Nome | Matricola | GitHub |
|-------|----------|--------|
| Daniele Gotti | 1079011 | [@DanieleGotti](https://github.com/DanieleGotti) |  
| Amin Borqal | 1073928 | [@aminb00](https://github.com/aminb00) |
