/***************************************************************************
 *   BALLOONS - prick'n'puzzle -                                           *
 *                                                                         *
 *   Copyright (C) 2007 by -S-v-N-  R.& M. Lagarden                        *
 *   svnmike@sourceforge.net                                               *
 *   eriban62@sourceforge.net                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

//  Tile-Tabelle




TileTab Tab[] = { 
   { 1    ,0,  0,0x0000 ,  5,  0 }, //   0 none
   { 1    ,0,  0,0x0ff0 ,  200,  0 }, //   1 Punkte-Item
   { 1    ,0,  0,0x0ff0 ,  100,  0 }, //   2 Punkte-Item
   { 1    ,0,  0,0x0ff0 ,  50,  0 }, //   3 Punkte-Item
   { 1    ,0,  0,0x0ff0 , 20,  0 }, //   4 Punkte-Item
   { 1    ,0,  0,0x0ff0 ,  10,  0 }, //   5 Punkte-Item
   { 1    ,0,  0,0x03f0 ,  0,  0 }, //   6 Hammer
   { 1    ,0,  0,0x03ffc,  0,  0 }, //   7 Schlssel
   { 1    ,0,  0,0x0100 ,  0,  0 }, //   8 neues Leben
   { 1    ,0,  0,0x3c0  ,  30,  0 }, //   9 15 Sec. Spielzeit
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  10 Codecard
   { 1    ,0,  0,0xff0  ,  0,  0 }, //  11 Blitz Leben verloren
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  12 Bombe (zndf�ig)
   { 1    ,0,  0,0x3ffc ,  0,  8 }, //  13 I-Riss
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  14 kleiner Riss
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  15 mittlerer Riss
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  16 grosser Riss
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  17 gr�ter Riss
   { 3    ,0,  0,0xffff ,  0,  0 }, //  18 schwarzes Loch
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  19 Wachseplatte
   { 4    ,0,  0,0x03c  ,  0,  0 }, //  20 Pin oben links
   { 4    ,0,  0,0x3c0  ,  0,  0 }, //  21 Pin mitte
   { 1    ,0,  0,0x3ffc , 20, 16 }, //  22 Pfeil rechts
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  23 Pfeil rechts
   { 1    ,0,  0,0x3ffc , 20, 16 }, //  24 Pfeil links
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  25 Pfeil links
   { 1    ,0,  0,0x3ffc , 20, 16 }, //  26 Pfeil unten
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  27 Pfeil unten
   { 1    ,0,  0,0x3ffc , 20, 16 }, //  28 Pfeil oben
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  29 Pfeil oben
   { 1    ,0,  0,0x100  , 48, 16 }, //  30 Haltestelle animie
   { 1    ,0,  0,0x100  ,  0,  0 }, //  31 Haltestelle animiert
   { 1    ,0,  0,0x3ffc , 16, 32 }, //  32 Beamer animiert
   { 1    ,0,  0,0x3ffc , 28, 16 }, //  33 Beamer
   { 1    ,0,  0,0x3ffc ,  0,  0 }, //  34 Beamer
   { 0    ,0,  0,0x100  , 16, 32 }, //  35 Blinkender Punkt
   { 0    ,0,  0,0x100  , 16, 16 }, //  36 Blinkender Punkt
   { 0    ,0,  0,0x100  ,  0,  0 }, //  37 Blinkender Punkt
   { 1    ,0,  0,0x100  , 48, 16 }, //  38 Ausgang
   { 1    ,0,  0,0x100  ,  0,  0 }, //  39 Ausgang           
   { 129  ,0,  0,0xffff ,  0,  0 }, //  40 Holzstein verschiebbar
   { 128  ,0,  0,0xffff ,  0,  4 }, //  41 Fenster
   { 128  ,0,  0,0xffff ,  0,  4 }, //  42 Fenster
   { 128  ,0,  0,0xffff ,  0,  4 }, //  43 Durchgangplatte 
   { 128  ,0,  0,0xffff ,  0,  4 }, //  44 Durchgangplatte 
   { 128  ,0,  0,0xffff ,  0,  4 }, //  45 Fenster 
   { 128  ,0,  0,0xffff ,  0,  4 }, //  46 Fenster  
   { 130  ,0,  0,0xffff ,  0,  2 }, //  47 zerst�bar      
   { 130  ,0,  0,0xffff ,  0,  2 }, //  48 zerst�bar
   { 128  ,0,  0,0xffff ,  0,  0 }, //  49 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  50 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  51  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  52  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  53  Wand
   { 128  ,0,  0,0xffff ,  0,  0 }, //  54 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  55  
   { 129  ,0,  0,0xffff ,  0,  0 }, //  56 Lochstein verschiebbar
   { 128  ,0,  0,0xffff ,  0,  0 }, //  57  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  58  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  59   
   { 128  ,0,  0,0xffff ,  0,  0 }, //  60  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  61  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  62  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  63  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  64 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  65 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  66  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  67 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  68 Wand
   { 128  ,0,  0,0xffff ,  0,  0 }, //  69  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  70 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  71 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  72  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  73  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  74 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  75  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  76 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  77  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  78  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  79  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  80 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  81  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  82 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  83  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  84  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  85  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  86  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  87 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  88 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  89  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  90  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  91  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  92  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  93 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  94  
   { 128  ,0,  0,0xffff ,  0,  0 }, //  95 
   { 128  ,0,  0,0xffff ,  0,  0 }, //  96 
   { 0    ,0,  0,0x0000 ,  0,  0 }, //  97 
   { 0    ,0,  0,0x0000 ,  0,  0 }, //  98 
   { 0    ,0,  0,0x0000 ,  0,  0 }, //  99 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 100 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 101 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 102 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 103  
   { 128  ,0,  0,0xffff ,  0,  0 }, // 104  
   { 128  ,0,  0,0xffff ,  0,  0 }, // 105  
   { 128  ,0,  0,0xffff ,  0,  0 }, // 106 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 107  
   { 128  ,0,  0,0xffff ,  0,  0 }, // 108  
   { 128  ,0,  0,0xffff ,  0,  0 }, // 109  
   { 0    ,0,  0,0xffff ,  0,  4 }, // 110  offene Roehre
   { 0    ,0,  0,0xffff ,  0,  4 }, // 111  offene Roehre
   { 131  ,0,  0,0xffff ,  0,  0 }, // 112 Ballreflektor
   { 131  ,0,  0,0xffff ,  0,  0 }, // 113 Ballreflektor 
   { 131  ,0,  0,0xffff ,  0,  0 }, // 114 Ballreflektor 
   { 131  ,0,  0,0xffff ,  0,  0 }, // 115 Ballreflektor
   { 129  ,0,  0,0xffff ,  0,  0 }, // 116 verschiebbarer Spieg�
   { 129  ,0,  0,0xffff ,  0,  0 }, // 117 
   { 129  ,0,  0,0xffff ,  0,  0 }, // 118 
   { 129  ,0,  0,0xffff ,  0,  0 }, // 119 
   { 4    ,0,  0,0xffff ,  0,  8 }, // 120 unsichtbare Wand
   { 5    ,0,  0,0x3ffc ,  0,  8 }, // 121 unsichtbarer Bumper
   { 133  ,0,  0,0xffff ,  0,0 }, // 122 Feind-Umwandler
   { 128  ,0,  0,0xffff ,  0,  0 }, // 123 Feind-Passierstein
   { 134  ,0,  0,0xffff ,  0,  0 }, // 124 Flip-Flop Schalter
   { 128  ,0,  0,0xffff ,  0,  0 }, // 125 durchsichtige Mauer
   { 128  ,0,  0,0xffff ,  0,  0 }, // 126 Lochwand quadrat 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 127 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 128 
   { 135  ,0,  0,0xffff ,  0,  0 }, // 129 Tauschstein 
   { 136  ,0,  0,0xffff ,  0,  0 }, // 130 Bumperstein 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 131  
   { 128  ,0,  0,0x3c0  ,  0,  0 }, // 132 schmale Wand
   { 128  ,0,  0,0x3c0  ,  0,  0 }, // 133 schmale Wand 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 134 
   { 129  ,0,  0,0xffff ,  0,  0 }, // 135 Normaler Moveblock
   { 137  ,0,  0,0xffff ,  0,  0 }, // 136 FigurKiller     
   { 138  ,0,  0,0xffff ,  0,  0 }, // 137 Schlssellochstein
   { 139  ,0,  0,0xffff ,  0,  0 }, // 138 Schalter On
   { 139  ,0,  0,0xffff ,  0,  0 }, // 139 Schalter off
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 140                   
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 141    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 142    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 143    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 144    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 145    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 146    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 147    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 148    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 149    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 150 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 151    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 152 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 153   
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 154   
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 155 Eisflaeche
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 156 Eisflaeche        
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 157 Eisflaeche
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 158 Eisflaeche
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 159 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 160 Ballwerfer rechts
   { 128  ,0,  0,0xffff ,  0,  0 }, // 161 Ballwerfer links   
   { 128  ,0,  0,0xffff ,  0,  0 }, // 162 Ballwerfer unten
   { 128  ,0,  0,0xffff ,  0,  0 }, // 163 Ballwerfer oben
   { 128  ,0,  0,0xffff ,  0,  0 }, // 164        
   { 128  ,0,  0,0xffff ,  0,  0 }, // 165 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 166 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 167 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 168 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 169 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 170 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 171 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 172    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 173   
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 174 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 175    
   { 2    ,0,  0,0x0000 ,  0,  0 }, // 176 schwarz fr Eisfl�he
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 177    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 178    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 179    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 180    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 181    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 182    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 183    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 184    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 185    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 186    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 187    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 188    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 189    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 190    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 191    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 192    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 193    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 194    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 195    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 196    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 197    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 198    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 199    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 200 Sand (5 Stck.)
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 201 Sand (5 Stck.)    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 202 Sand (5 Stck.) 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 203 Sand (5 Stck.)
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 204 Sand (5 Stck.)
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 205 Mauerwerk
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 206 Mauerwerk
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 207 Mauerwerk
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 208 Mauerwerk
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 209  Pflaster (6 Stck.
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 210                   
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 211    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 212    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 213    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 214    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 215 Platten mit Gruen
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 216 Platten mit Gruen
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 217 Platten mit Gruen
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 218    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 219    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 220    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 221    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 222    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 223    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 224    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 225    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 226    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 227    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 228    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 229    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 230    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 231    
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 232 
   { 128  ,0,  0,0xffff ,  0,  0 }, // 233 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 234 fr Holzstein
   { 1    ,0,  0,0x3ffc ,  0,  0 }, // 235 
   { 1    ,0,  0,0x3ffc ,  0,  0 }, // 236 
   { 1    ,0,  0,0x3ffc ,  0,  0 }, // 237 
   { 1    ,0,  0,0x3ffc ,  0,  0 }, // 238 
   { 1    ,0,  0,0x3ffc ,  0,  0 }, // 239 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 240 
   { 0    ,0,  0,0x0000 ,  0,  0 }, // 241 


};  // Tabelle Ende
